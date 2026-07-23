import serial
import serial.tools.list_ports
import struct
import time
import sys
import threading

# 全局变量
current_ser = None
current_port = None
BAUD = 115200

# 线程控制与传输状态
is_downloading = False
local_file_data = bytearray()
target_filename = ""

def calc_checksum(cmd, data):
    len_hi = (len(data) >> 8) & 0xFF
    len_lo = len(data) & 0xFF
    checksum = cmd ^ len_hi ^ len_lo
    for b in data: checksum ^= b
    return checksum

def send_frame(ser, cmd, data=b""):
    head = b'\xaa\xbb'
    length = len(data)
    len_bytes = struct.pack(">H", length)
    checksum = calc_checksum(cmd, data)
    frame = head + bytes([cmd]) + len_bytes + data + bytes([checksum])
    ser.write(frame)

def read_frame(ser, timeout=1):
    start_time = time.time()
    while True:
        if time.time() - start_time > timeout:
            return None, None
        if ser.read(1) == b'\xaa':
            if ser.read(1) == b'\xbb':
                break
                
    cmd = ser.read(1)
    if not cmd: return None, None
    cmd = cmd[0]
    
    len_bytes = ser.read(2)
    if len(len_bytes) < 2: return None, None
    data_len = struct.unpack(">H", len_bytes)[0]
    
    payload = ser.read(data_len)
    ser.read(1) # 略过校验和
    return cmd, payload

def set_com_port():
    global current_ser, current_port
    if is_downloading:
        print("⚠️  当前正在下载日志，请先停止或等待传输完成！")
        return
        
    print("\n--- 🔍 正在扫描可用串口 ---")
    ports = list(serial.tools.list_ports.comports())
    if not ports:
        print("❌ 未找到任何可用串口！")
        return
    for idx, p in enumerate(ports):
        print(f" [{idx}] {p.device} - {p.description}")
    try:
        choice = int(input("\n👉 请输入要选择的串口序号: "))
        if choice < 0 or choice >= len(ports): return
    except ValueError: return

    if current_ser and current_ser.is_open: current_ser.close()
    target_port = ports[choice].device
    try:
        current_ser = serial.Serial(target_port, BAUD, timeout=0.1)
        current_port = target_port
        print(f"✅ 成功连接到: {current_port}")
    except Exception as e:
        print(f"❌ 错误: {e}")

def bg_download_worker():
    """后台下载线程任务"""
    global is_downloading, local_file_data, target_filename, current_ser
    
    local_file_data = bytearray()
    
    while is_downloading:
        cmd, payload = read_frame(current_ser, timeout=1.5)
        
        if cmd is None:
            # 在后台不阻塞打印，直接报错退出
            print("\n❌ [下载失败] 远端接收超时，传输中断。")
            is_downloading = False
            break
            
        if cmd == 0x10:
            print(f"\n❌ [MCU报错]: {payload.decode('utf-8', errors='ignore')}")
            is_downloading = False
            break
            
        if cmd == 0x04:
            if payload:
                print(f"\n🛑 [传输中止] MCU已安全关闭文件。应答: {payload.decode('utf-8')}")
            else:
                print("\n✅ [下载完成] 文件传输正常结束！")
                with open(target_filename, "wb") as f:
                    f.write(local_file_data)
                print(f"💾 文件成功保存至当前目录: {target_filename}")
            is_downloading = False
            break
            
        if cmd == 0x05: # 0x05 代表 MCU 正在发过来的日志数据包
            if len(payload) >= 2:
                pack_num = struct.unpack(">H", payload[0:2])[0]
                local_file_data.extend(payload[2:])
                print(f"\r📥 正在接收: #{pack_num} (当前大小: {len(local_file_data)} Bytes)", end="", flush=True)

def download_log_files():
    global current_ser, is_downloading, target_filename
    if not current_ser or not current_ser.is_open:
        print("⚠️  请先执行 1 设置并打开 COM 口！")
        return
    if is_downloading:
        print("⚠️  已有日志正在下载中！")
        return

    print("\n🔄 正在获取远端日志文件列表...")
    send_frame(current_ser, 0x01)
    
    cmd, payload = read_frame(current_ser, timeout=3)
    if cmd != 0x01 or not payload:
        print("❌ 获取日志列表失败")
        return
        
    file_list = [f for f in payload.decode('utf-8').split(',') if f]
    if not file_list: 
        print("📂 目录下没有找到日志文件")
        return
        
    print("\n📬 发现以下日志文件:")
    for idx, name in enumerate(file_list):
        print(f"  [{idx}] {name}")
        
    try:
        select_idx = int(input("\n👉 请输入你想下载的日志序号: "))
        if select_idx < 0 or select_idx >= len(file_list): return
    except ValueError: return
        
    target_filename = file_list[select_idx]
    print(f"🚀 已启动后台下载: {target_filename} ...")
    print("💡 提示: 传输期间您可以随时输入 '3' 强制退出上传。")
    
    # 拉起后台线程进行接收，主线程释放出来响应菜单输入
    is_downloading = True
    send_frame(current_ser, 0x02, bytes([select_idx]))
    
    downloader_thread = threading.Thread(target=bg_download_worker, daemon=True)
    downloader_thread.start()

def stop_log_upload():
    """3. 退出日志上传"""
    global is_downloading, current_ser
    # if not is_downloading:
    #     print("⚠️  提示：当前并没有任何正在运行的日志传输。")
    #     return
        
    print("\n🛑 正在向 MCU 发送结束上传指令 (0x03)...")
    send_frame(current_ser, 0x03)
    
    # 强制将本地后台线程的接收状态关闭
    # 后台线程收到 MCU 最终回传的 0x04 帧后会自动打印结束确认
    is_downloading = False

def show_menu():
    port_status = f"({current_port})" if current_port else "(Not Set)"
    print("\n=============================================")
    print(f"1. Set COM port {port_status}")
    print("2. Download Log Files")
    print("3. Stop Log Upload")
    print("help. Show this menu")
    print("exit. Quit")
    print("=============================================")

def main():
    while True:
        try:
            show_menu()
            prompt = f"\n[{current_port if current_port else 'NO COM'}] >> "
            user_input = input(prompt).strip().lower()
            if user_input == '1': 
                set_com_port()
            elif user_input == '2': 
                download_log_files()
            elif user_input == '3':
                stop_log_upload()
            elif user_input == 'help' or user_input == 'h': 
                show_menu()
            elif user_input == 'exit' or user_input == 'q' or user_input == 'Q':
                if current_ser and current_ser.is_open: current_ser.close()
                sys.exit(0)
            elif user_input == '':
                continue
            else:
                print("[ X ] 未知命令，输入 'help' 可以重新查看菜单。")
        except KeyboardInterrupt:
            print("\n输入 exit 即可退出整个程序。")

if __name__ == "__main__":
    main()
