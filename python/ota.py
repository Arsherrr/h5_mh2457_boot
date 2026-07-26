import sys
import serial
import struct
import time
import os
import argparse
import serial.tools.list_ports
from typing import List, Tuple, Optional

# ---------- Protocol Constants ----------
FRAME_HEADER = b'@@'
FRAME_HEADER_RESP = b'$$'
FRAME_TRAILER = b'\r\n'
CMD_OFFSET_TO_PC = 0x4000
CMD_OFFSET_FROM_AT425 = 0x0100

# Commands from host to device
CMD_GET_VER = 0x0301
CMD_SEND_VER = 0x0302
CMD_SEND_FILE_SIZE = 0x0303
CMD_SEND_DATA = 0x0304
CMD_SEND_UPD_STATUS = 0x0305
CMD_GET_SN = 0x0201
CMD_SET_SN = 0x0202

# Upgrade types
UPD_APP = 1
UPD_EXT_FLASH = 2
UPD_LOGO = 3
UPD_LOGO_EXT_FLASH = 3   # same code, distinguished by file size
UPD_APP_DUAL = 4         # dual MCU (APP + AT425)

# Packet size
PACK_SIZE_MCU = 4096
PACK_SIZE_FLASH = 4096

# Flash limits
SIZE_MCU = 2 * 1024 * 1024          # 2 MB
SIZE_LOGO = 1 * 1024 * 1024         # 1 MB
SIZE_FLASH = 24 * 1024 * 1024       # 24 MB

# ---------- CRC Table (exactly as in device) ----------
FCSTAB = [
    0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
    0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
    0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
    0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
    0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
    0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
    0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
    0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
    0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
    0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
    0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
    0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
    0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
    0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
    0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
    0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
    0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
    0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
    0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
    0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
    0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
    0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
    0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
    0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
    0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
    0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
    0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
    0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
    0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
    0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
    0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
    0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
]

def crc16(data: bytes, init: int = 0) -> int:
    """
    CRC16 calculation exactly matching device's crc16().
    :param data: bytes to calculate CRC on
    :param init: initial value (same as device's 'init' parameter)
    :return: CRC value
    """
    crc = (init ^ 0xFFFF) & 0xFFFF
    for byte in data:
        idx = ((crc & 0xFF) ^ byte) & 0xFF
        crc = ((crc >> 8) ^ FCSTAB[idx]) & 0xFFFF
    crc ^= 0xFFFF
    return crc & 0xFFFF

# ---------- Frame Building / Parsing ----------
def build_frame(cmd: int, data: bytes) -> bytes:
    """Build a frame with header, length, cmd, data, CRC, trailer.
       CRC includes header bytes (first two bytes)."""
    total_len = 2 + 2 + 2 + len(data) + 2 + 2   # header(2)+len(2)+cmd(2)+data+crc(2)+trailer(2)
    frame = bytearray()
    frame += FRAME_HEADER                # '@@'
    frame += struct.pack('<H', total_len)
    frame += struct.pack('>H', cmd)
    frame += data
    # CRC calculation includes entire frame from index 0 to end of data
    crc_val = crc16(frame)               # frame currently does NOT include CRC and trailer
    frame += struct.pack('<H', crc_val)
    frame += FRAME_TRAILER
    return bytes(frame)

def parse_response(frame: bytes) -> Tuple[int, bytes]:
    if not frame.startswith(FRAME_HEADER_RESP):
        raise ValueError("Invalid response header")
    if len(frame) < 10:
        raise ValueError("Response too short")
    total_len = struct.unpack('<H', frame[2:4])[0]
    if len(frame) != total_len:
        raise ValueError(f"Length mismatch: expected {total_len}, got {len(frame)}")
    cmd = struct.unpack('>H', frame[4:6])[0]
    data_len = total_len - 10
    data = frame[6:6+data_len]
    crc_received = struct.unpack('<H', frame[6+data_len:8+data_len])[0]
    # CRC calculation includes header (from index 0 to end of data)
    crc_calc = crc16(frame[:6+data_len])   # from frame start to end of data
    if crc_received != crc_calc:
        raise ValueError(f"CRC mismatch: received {crc_received:04X}, calc {crc_calc:04X}")
    if frame[-2:] != FRAME_TRAILER:
        raise ValueError("Invalid trailer")
    return cmd, data

# ---------- Serial Communication ----------
class SerialLink:
    def __init__(self, port: str, baudrate: int = 115200, timeout: float = 2.0):
        self.ser = serial.Serial(port, baudrate, timeout=timeout, write_timeout=timeout)
        self.timeout = timeout
        self.port = port
        self.baudrate = baudrate

    def send_frame(self, cmd: int, data: bytes) -> None:
        frame = build_frame(cmd, data)
        self.ser.write(frame)
        self.ser.flush()

    def recv_response(self, expected_cmd: Optional[int] = None, timeout: float = None) -> Tuple[int, bytes]:
        t_end = time.time() + (timeout if timeout else self.timeout)
        buffer = b''
        # 发送命令后短暂等待，给设备处理时间
        time.sleep(0.05)
        while time.time() < t_end:
            # 非阻塞批量读取 — 只在数据可用时读取，避免 read() 超时消耗等待时间
            waiting = self.ser.in_waiting
            if waiting > 0:
                chunk = self.ser.read(waiting)
                if chunk:
                    buffer += chunk
                    if FRAME_HEADER_RESP in buffer:
                        idx = buffer.find(FRAME_HEADER_RESP)
                        buffer = buffer[idx:]
                        if len(buffer) >= 4:
                            total_len = struct.unpack('<H', buffer[2:4])[0]
                            if len(buffer) >= total_len:
                                frame = buffer[:total_len]
                                buffer = buffer[total_len:]
                                cmd_resp, data = parse_response(frame)
                                if expected_cmd is not None:
                                    expected_resp_cmd = expected_cmd + CMD_OFFSET_TO_PC
                                    if cmd_resp != expected_resp_cmd:
                                        raise ValueError(f"Unexpected response cmd: 0x{cmd_resp:04X}, expected 0x{expected_resp_cmd:04X}")
                                return cmd_resp, data
            else:
                # 没有可用数据，短暂休眠避免 CPU 空转
                time.sleep(0.01)
        raise TimeoutError(f"No valid response received within {timeout if timeout else self.timeout}s. "
                           f"Expected cmd 0x{(expected_cmd + CMD_OFFSET_TO_PC) if expected_cmd else 'any':04X}")

    def close(self):
        self.ser.close()

# ---------- Upgrade Core Functions ----------
def get_version(link: SerialLink) -> dict:
    link.send_frame(CMD_GET_VER, b'')
    cmd, data = link.recv_response(CMD_GET_VER)
    if len(data) < 44:
        raise ValueError("Version response too short")
    product = data[0:10].decode(errors='replace').strip('\x00')
    iap_ver = data[10:20].decode(errors='replace').strip('\x00')
    app_ver = data[20:30].decode(errors='replace').strip('\x00')
    flash_ver = data[30:40].decode(errors='replace').strip('\x00')
    pack_mcu = struct.unpack('<H', data[40:42])[0]
    pack_flash = struct.unpack('<H', data[42:44])[0]
    return {
        'product': product,
        'iap_ver': iap_ver,
        'app_ver': app_ver,
        'flash_ver': flash_ver,
        'pack_mcu': pack_mcu,
        'pack_flash': pack_flash
    }

def send_file_size(link: SerialLink, upd_type: int, file_size: int):
    data = struct.pack('<BI', upd_type, file_size)
    link.send_frame(CMD_SEND_FILE_SIZE, data)
    cmd, resp = link.recv_response(CMD_SEND_FILE_SIZE)
    if len(resp) < 1 or resp[0] != 0:
        raise RuntimeError(f"Device rejected file size: status {resp[0] if resp else 'none'}")

def send_data_packet(link: SerialLink, upd_type: int, pack_num: int, payload: bytes):
    data = struct.pack('<BH', upd_type, pack_num) + payload
    link.send_frame(CMD_SEND_DATA, data)
    cmd, resp = link.recv_response(CMD_SEND_DATA)
    if len(resp) < 3:
        raise RuntimeError("Invalid ACK for data packet")
    status = resp[0]
    ack_num = struct.unpack('<H', resp[1:3])[0]
    if ack_num != pack_num:
        raise RuntimeError(f"Packet number mismatch: sent {pack_num}, ack {ack_num}")
    if status != 0:
        raise RuntimeError(f"Device reported error for packet {pack_num}, status {status}")

def upgrade_file(link: SerialLink, upd_type: int, file_path: str, pack_size: int, max_size: int):
    file_size = os.path.getsize(file_path)
    if file_size > max_size:
        raise ValueError(f"File too large: {file_size} > {max_size}")
    total_packs = (file_size + pack_size - 1) // pack_size
    print(f"Upgrading type {upd_type}, file size {file_size}, pack size {pack_size}, total packs {total_packs}")

    send_file_size(link, upd_type, file_size)

    with open(file_path, 'rb') as f:
        for pack_num in range(total_packs):
            chunk = f.read(pack_size)
            send_data_packet(link, upd_type, pack_num, chunk)
            percent = (pack_num + 1) * 100 // total_packs
            print(f"\rProgress: {percent}% ({pack_num+1}/{total_packs})", end='', flush=True)
    print("\nUpgrade completed successfully.")

def merge_dual_mcu(mcu1_path: str, mcu2_path: str, output_path: str) -> int:
    """
    Merge mcu1.bin (main) and mcu2.bin (AT425) into a single file for dual-MCU upgrade.
    AT425 metadata (addr, size, xor) placed at offset 0x3B0.
    """
    with open(mcu1_path, 'rb') as f1, open(mcu2_path, 'rb') as f2:
        mcu1_data = bytearray(f1.read())
        mcu2_data = f2.read()

    addr_at425 = len(mcu1_data)
    size_at425 = len(mcu2_data)

    meta = struct.pack('<II', addr_at425, size_at425)
    xor_val = 0
    for b in meta:
        xor_val ^= b
    meta += bytes([xor_val])

    if len(mcu1_data) < 0x3B0 + 9:
        mcu1_data.extend(b'\x00' * (0x3B0 + 9 - len(mcu1_data)))
    mcu1_data[0x3B0:0x3B0+9] = meta

    combined = mcu1_data + mcu2_data
    with open(output_path, 'wb') as fout:
        fout.write(combined)
    return len(combined)

# ---------- Interactive Menu ----------
def list_available_ports():
    ports = serial.tools.list_ports.comports()
    if not ports:
        print("No serial ports found.")
        return []
    for i, port in enumerate(ports):
        print(f"{i+1}. {port.device} - {port.description}")
    return ports

def select_port():
    ports = list_available_ports()
    if not ports:
        return None
    while True:
        try:
            choice = input("Select port number (or 0 to cancel): ").strip()
            if choice == '0':
                return None
            idx = int(choice) - 1
            if 0 <= idx < len(ports):
                return ports[idx].device
            else:
                print("Invalid choice.")
        except ValueError:
            print("Please enter a number.")

def show_menu():
    print("\n" + "="*50)
    print("OTA Upgrader Tool - Menu")
    print("="*50)
    print("1. Set COM port")
    print("2. Get device version")
    print("3. Upgrade APP (single MCU)")
    print("4. Upgrade Dual MCU (APP + AT425)")
    print("5. Upgrade External Flash")
    print("6. Upgrade Logo area (max 1MB)")
    print("7. Upgrade Logo + External Flash (file >1MB)")
    print("help. Show this menu")
    print("exit. Quit")
    print("="*50)

def main():
    parser = argparse.ArgumentParser(description='OTA Upgrader for device over VCP')
    parser.add_argument('port', nargs='?', help='Serial port (optional)')
    parser.add_argument('baudrate', nargs='?', type=int, default=115200, help='Baud rate (default 115200)')
    args = parser.parse_args()

    current_port = args.port
    current_baudrate = args.baudrate
    link = None

    def ensure_connection():
        nonlocal link, current_port, current_baudrate
        if link is not None:
            link.close()
            link = None
        if not current_port:
            print("No COM port set. Please select option 1 first.")
            return False
        try:
            link = SerialLink(current_port, current_baudrate)
            return True
        except Exception as e:
            print(f"Failed to connect to {current_port}: {e}")
            current_port = None
            return False

    if current_port:
        try:
            link = SerialLink(current_port, current_baudrate)
            print(f"Connected to {current_port} at {current_baudrate} baud")
        except Exception as e:
            print(f"Warning: Could not connect to {current_port}: {e}")
            print("You can set another COM port using menu option 1.")
            link = None

    while True:
        show_menu()
        cmd = input("\nEnter choice (number or command): ").strip().lower()
        if cmd in ('exit', 'quit'):
            break
        elif cmd == 'help':
            continue

        if cmd == '1':
            new_port = select_port()
            if new_port:
                current_port = new_port
                print(f"COM port set to {current_port}")
                if ensure_connection():
                    print("Connected successfully.")
                else:
                    print("Connection failed. Check the port and device.")
            else:
                print("Port selection cancelled.")
            continue

        # For other commands, require connection
        if link is None and not ensure_connection():
            print("No valid COM port. Please set port first (option 1).")
            continue

        try:
            if cmd == '2':
                info = get_version(link)
                print(f"Product  : {info['product']}")
                print(f"IAP ver  : {info['iap_ver']}")
                print(f"APP ver  : {info['app_ver']}")
                print(f"Flash ver: {info['flash_ver']}")
                print(f"Pack MCU : {info['pack_mcu']}, Flash : {info['pack_flash']}")
            elif cmd == '3':
                filepath = input("Enter APP binary file path: ").strip()
                if not os.path.isfile(filepath):
                    print("File not found.")
                    continue
                upgrade_file(link, UPD_APP, filepath, PACK_SIZE_MCU, SIZE_MCU)
            elif cmd == '4':
                mcu1 = input("Enter main MCU binary file (mcu1.bin): ").strip()
                mcu2 = input("Enter AT425 binary file (mcu2.bin): ").strip()
                if not (os.path.isfile(mcu1) and os.path.isfile(mcu2)):
                    print("Both files must exist.")
                    continue
                merged = "merged_dual.bin"
                print(f"Merging {mcu1} and {mcu2} into {merged} ...")
                size = merge_dual_mcu(mcu1, mcu2, merged)
                print(f"Merged file size: {size} bytes")
                upgrade_file(link, UPD_APP_DUAL, merged, PACK_SIZE_MCU, SIZE_MCU)
                # optionally delete merged file after success
                # os.remove(merged)
            elif cmd == '5':
                filepath = input("Enter external Flash binary file path: ").strip()
                if not os.path.isfile(filepath):
                    print("File not found.")
                    continue
                upgrade_file(link, UPD_EXT_FLASH, filepath, PACK_SIZE_FLASH, SIZE_FLASH)
            elif cmd == '6':
                filepath = input("Enter Logo binary file path (max 1MB): ").strip()
                if not os.path.isfile(filepath):
                    print("File not found.")
                    continue
                fsize = os.path.getsize(filepath)
                if fsize > SIZE_LOGO:
                    print(f"File too large for Logo area: {fsize} > {SIZE_LOGO}. Use option 7 for larger files.")
                else:
                    upgrade_file(link, UPD_LOGO, filepath, PACK_SIZE_FLASH, SIZE_LOGO)
            elif cmd == '7':
                filepath = input("Enter Logo+Flash binary file path: ").strip()
                if not os.path.isfile(filepath):
                    print("File not found.")
                    continue
                fsize = os.path.getsize(filepath)
                if fsize > SIZE_LOGO + SIZE_FLASH:
                    print(f"File too large: {fsize} > {SIZE_LOGO+SIZE_FLASH}")
                else:
                    upgrade_file(link, UPD_LOGO_EXT_FLASH, filepath, PACK_SIZE_FLASH, SIZE_LOGO+SIZE_FLASH)
            else:
                print("Unknown option. Type 'help' for menu.")
        except Exception as e:
            print(f"Error during operation: {e}")
            link = None

    if link:
        link.close()
    print("Goodbye.")

if __name__ == '__main__':
    main()