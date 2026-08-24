import sys
import os
import time
import struct
import binascii
from intelhex import IntelHex

def process_firmware(bin_path, hex_path, axf_path=None):
    APP_START_ADDR = 0x080E2000
    TARGET_ADDR    = 0x080E3030
    BIN_OFFSET     = TARGET_ADDR - APP_START_ADDR  # 0x1030
    FAST_CRC_BYTES = 256

    if not os.path.exists(bin_path) or not os.path.exists(hex_path):
        print("[Error] 找不到 BIN 或 HEX 文件")
        sys.exit(1)

    # 1. 读取 BIN, 按 4 字节对齐填充 0xFF (Boot 按 32 位字访问尾部时地址才安全对齐)
    with open(bin_path, 'rb') as f:
        bin_data = bytearray(f.read())

    pad = (4 - (len(bin_data) % 4)) % 4
    if pad:
        bin_data.extend(b'\xFF' * pad)
        print(f"-> BIN 已填充 {pad} 字节到 4 对齐")

    app_size_val = len(bin_data)

    # 2. 生成时间数据
    now = time.localtime()
    ymd_val = now.tm_year * 10000 + now.tm_mon * 100 + now.tm_mday
    hms_val = now.tm_hour * 10000 + now.tm_min * 100 + now.tm_sec

    # 3. 前/后各 256 字节做标准 IEEE CRC-32 (与 Boot CSR=0xF6 按字喂入等价)
    #    Boot: 每次写 1 个小端 uint32 到 CRC->DR, 共 128 次 (原按字节 512 次)
    #    先写 size/ymd/hms, 再算 CRC, 最后写回 crc (覆盖范围不含 meta@0x1030)
    struct.pack_into('<I', bin_data, BIN_OFFSET + 0, app_size_val)
    struct.pack_into('<I', bin_data, BIN_OFFSET + 8, ymd_val)
    struct.pack_into('<I', bin_data, BIN_OFFSET + 12, hms_val)

    if app_size_val < FAST_CRC_BYTES * 2:
        print("[Error] 固件小于 512 字节, 无法做前/后 256 校验")
        sys.exit(1)

    crc_buffer = bytes(bin_data[:FAST_CRC_BYTES] + bin_data[-FAST_CRC_BYTES:])
    words = struct.unpack('<%dI' % (len(crc_buffer) // 4), crc_buffer)
    crc_word_stream = b''.join(struct.pack('<I', w) for w in words)
    crc32_val = binascii.crc32(crc_word_stream) & 0xFFFFFFFF
    struct.pack_into('<I', bin_data, BIN_OFFSET + 4, crc32_val)

    # 4. 保存 BIN
    with open(bin_path, 'wb') as f:
        f.write(bin_data)
    print("-> BIN 修改成功。")

    # 5. 同步 HEX (含对齐填充字节)
    ih = IntelHex(hex_path)
    ih.puts(TARGET_ADDR + 0, struct.pack('<I', app_size_val))
    ih.puts(TARGET_ADDR + 4, struct.pack('<I', crc32_val))
    ih.puts(TARGET_ADDR + 8, struct.pack('<I', ymd_val))
    ih.puts(TARGET_ADDR + 12, struct.pack('<I', hms_val))
    if pad:
        for i in range(app_size_val - pad, app_size_val):
            ih[APP_START_ADDR + i] = bin_data[i]
    ih.tofile(hex_path, format='hex')
    print("-> HEX 修改成功。")

    # 6. 同步 AXF, 便于 Keil 直接下载
    if axf_path and os.path.exists(axf_path):
        with open(axf_path, 'rb+') as f:
            axf_data = f.read()
            target_pattern = b'\xFF\xFF\xFF\xFF' * 4
            offset = axf_data.find(target_pattern)
            if offset != -1:
                f.seek(offset)
                f.write(struct.pack('<I', app_size_val))
                f.write(struct.pack('<I', crc32_val))
                f.write(struct.pack('<I', ymd_val))
                f.write(struct.pack('<I', hms_val))
                print("-> AXF 同步修改成功！Keil 直接下载将生效。")
            else:
                print("-> [Warning] 未在 AXF 中匹配到全空特征，请确认代码中初始值是否为 0xFFFFFFFF。")

    # 7. 打印输出
    print(f"\n======================================")
    print(f" 固件大小 (Size) : {app_size_val} 字节 (4 对齐)")
    print(f" 校验和   (CRC32): 0x{crc32_val:08X}  (32-bit x 128 words)")
    print(f" 编译日期 (YMD)  : {ymd_val}")
    print(f" 编译时间 (HMS)  : {hms_val:06d}")
    print(f"======================================\n")

if __name__ == "__main__":
    if len(sys.argv) < 4:
        print("使用方法: crc.exe [BIN] [HEX] [AXF]")
        sys.exit(1)
    process_firmware(sys.argv[1], sys.argv[2], sys.argv[3])
