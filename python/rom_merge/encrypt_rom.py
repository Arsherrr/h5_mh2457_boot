import os
import yaml
from cryptography.hazmat.primitives.ciphers.aead import AESGCM

AES_GCM_KEY_SIZE = 16
AES_GCM_NONCE_SIZE = 12
AES_GCM_TAG_SIZE = 16


def _parse_hex_bytes(value, size, name):
    if isinstance(value, bytes):
        data = value
    elif isinstance(value, str):
        text = value.strip().replace("0x", "").replace(" ", "").replace(":", "").replace("-", "")
        data = bytes.fromhex(text)
    else:
        raise ValueError(f"{name} must be a hex string")

    if len(data) != size:
        raise ValueError(f"{name} must be {size} bytes, current length is {len(data)} bytes")
    return data


def _resolve_path(base_dir, path):
    if os.path.isabs(path):
        return path
    return os.path.normpath(os.path.join(base_dir, path))


def encrypt_rom(config_path="encrypt_config.yaml"):
    script_dir = os.path.dirname(os.path.abspath(__file__))
    if not os.path.isabs(config_path):
        config_path = os.path.join(script_dir, config_path)

    if not os.path.exists(config_path):
        print(f"[ERROR] Can not found file: {config_path}")
        return

    with open(config_path, "r", encoding="utf-8-sig") as f:
        config = yaml.safe_load(f) or {}

    input_path = _resolve_path(script_dir, config.get("input_path"))
    output_path = _resolve_path(script_dir, config.get("output_path"))
    aes_key = _parse_hex_bytes(config.get("aes_gcm_key"), AES_GCM_KEY_SIZE, "aes_gcm_key")
    aes_nonce = _parse_hex_bytes(config.get("aes_gcm_nonce"), AES_GCM_NONCE_SIZE, "aes_gcm_nonce")

    if not os.path.exists(input_path):
        print(f"[ERROR] Can not found input file: {input_path}")
        return

    with open(input_path, "rb") as f:
        plaintext = f.read()

    encrypted = AESGCM(aes_key).encrypt(aes_nonce, plaintext, None)
    ciphertext = encrypted[:-AES_GCM_TAG_SIZE]
    tag = encrypted[-AES_GCM_TAG_SIZE:]

    output_dir = os.path.dirname(output_path)
    if output_dir:
        os.makedirs(output_dir, exist_ok=True)

    with open(output_path, "wb") as f:
        f.write(ciphertext)
        f.write(tag)

    print("=" * 50)
    print(f"Input ROM: {input_path}")
    print(f"Output encrypted ROM: {output_path}")
    print(f"Plain size: {len(plaintext)} bytes")
    print(f"Cipher size: {len(ciphertext)} bytes")
    print(f"Tag size: {len(tag)} bytes")
    print(f"Nonce: {aes_nonce.hex()}")
    print(f"Tag: {tag.hex()}")
    print("=" * 50)


if __name__ == "__main__":
    encrypt_rom()
