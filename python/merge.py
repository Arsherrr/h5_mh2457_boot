import os
import struct
import yaml


def pack_rom_final(config_path="config.yaml"):
    if not os.path.exists(config_path):
        print(f"错误：找不到配置文件 {config_path}")
        return

    with open(config_path, "r", encoding="utf-8") as f:
        config = yaml.safe_load(f)

    bin1_path = config.get("bin1_path")
    bin2_path = config.get("bin2_path")
    upgrade_addr_str = config.get("upgrade_address")
    output_name = config.get("output_name")
    magic_str = config.get("magic", "0x5A5A5A5A")

    if not os.path.exists(bin1_path) or not os.path.exists(bin2_path):
        print("错误：配置中的 bin 文件不存在，请检查路径。")
        return

    upgrade_address = (
        int(upgrade_addr_str, 16)
        if isinstance(upgrade_addr_str, str)
        else int(upgrade_addr_str)
    )
    magic_val = (
        int(magic_str, 16) if isinstance(magic_str, str) else int(magic_str)
    )

    with open(bin1_path, "rb") as f1:
        bin1_data = f1.read()
    with open(bin2_path, "rb") as f2:
        bin2_data = f2.read()
    bin1_size = len(bin1_data)

    # 1. 初始化 4KB Header 缓冲区
    HEADER_SIZE = 4096
    header_data = bytearray(HEADER_SIZE)

    # 2. 写入前 12 个字节的数据 (小端序 '<I')
    struct.pack_into("<I", header_data, 0x3AC, magic_val)  # 4字节 Magic
    struct.pack_into("<I", header_data, 0x3B0, upgrade_address)  # 4字节 升级地址
    struct.pack_into("<I", header_data, 0x3B4, bin1_size)  # 4字节 Bin1大小

    # 3. 计算前 12 个字节的 XOR 校验和 (从 0x3AC 到 0x3B7)
    calculated_xor = 0
    for addr in range(0x3AC, 0x3B8):
        calculated_xor ^= header_data[addr]

    # 4. 把 XOR 值精准填到 12 字节后面的第 13 个字节 (即 0x3B8 位置)
    header_data[0x3B8] = calculated_xor

    # 5. 第一个 bin 的 4K 对齐填充
    remainder = bin1_size % 4096
    padding_size = (4096 - remainder) % 4096
    padding_data = b"\xFF" * padding_size

    # 6. 拼装输出 Rom 文件
    output_path = f"{output_name}.rom"
    with open(output_path, "wb") as f_out:
        f_out.write(header_data)
        f_out.write(bin1_data)
        f_out.write(padding_data)
        f_out.write(bin2_data)

    print("=" * 50)
    print(f"成功生成 ROM 文件: {output_path}")
    print(f"0x3AC ~ 0x3AF (4字节): Magic = {hex(magic_val)}")
    print(f"0x3B0 ~ 0x3B3 (4字节): 升级地址 = {hex(upgrade_address)}")
    print(f"0x3B4 ~ 0x3B7 (4字节): Bin1大小 = {bin1_size} 字节")
    print(f"0x3B8       (1字节): 填入的 XOR 值 = {hex(calculated_xor)}")
    print(
        f"0x3B9 ~ 0x3C1 (9字节): MCU 循环校验区 = 均为 {hex(header_data[0x3B9])}"
    )
    print("=" * 50)


if __name__ == "__main__":
    pack_rom_final()
