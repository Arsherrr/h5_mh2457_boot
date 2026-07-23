import os
import sys
from PIL import Image

def convert_to_raw(image_path, output_path=None):
    # 1. 检查文件是否存在
    if not os.path.exists(image_path):
        print(f"Error: 找不到文件 {image_path}")
        return

    # 2. 获取图片宽高
    try:
        with Image.open(image_path) as img:
            width, height = img.size
    except Exception as e:
        print(f"Error: 无法读取图片信息 - {e}")
        return

    # 3. 读取文件的原始二进制数据 (保持原格式，不解码)
    with open(image_path, "rb") as f:
        raw_data = f.read()
    
    data_size = len(raw_data)

    # 4. 生成 C 语言合法的变量名 (去除路径和特殊字符)
    base_name = os.path.splitext(os.path.basename(image_path))[0]
    var_name = "".join([c if c.isalnum() else "_" for c in base_name])

    # 5. 生成 C 源码内容
    c_code = []
    c_code.append(f"/* Original File: {os.path.basename(image_path)} ({width}x{height}) */\n")
    c_code.append("#include \"lvgl.h\"\n")  # 适配 LVGL v9 的通用头文件
    
    # 写入二进制数组
    c_code.append(f"const uint8_t {var_name}_map[] = {{")
    
    # 每 16 个字节换一行
    for i in range(0, data_size, 16):
        chunk = raw_data[i:i+16]
        hex_line = ", ".join([f"0x{b:02x}" for b in chunk])
        if i + 16 < data_size:
            hex_line += ","
        c_code.append(f"    {hex_line}")
        
    c_code.append("};\n")

    # 写入符合 LVGL v9 标准的图片描述结构体
    c_code.append(f"const lv_image_dsc_t {var_name} = {{")
    c_code.append(f"    .header.cf = LV_COLOR_FORMAT_RAW,") # LVGL v9 核心颜色格式
    c_code.append(f"    .header.w = {width},")
    c_code.append(f"    .header.h = {height},")
    c_code.append(f"    .data_size = {data_size},")       # 数组大小
    c_code.append(f"    .data = {var_name}_map,")          # 数组地址
    c_code.append("};")

    # 6. 保存到文件
    if not output_path:
        output_path = os.path.splitext(image_path)[0] + ".c"
        
    with open(output_path, "w", encoding="utf-8") as f:
        f.write("\n".join(c_code))
        
    print(f"=============================")
    print(f"LVGL v9 图像转换成功！")
    print(f"输出文件: {output_path}")
    print(f"图片尺寸: {width} x {height}")
    print(f"原始大小: {data_size} 字节")
    print(f"C代码指针: &{var_name}")
    print(f"=============================")

if __name__ == "__main__":
    if len(sys.argv) > 1:
        convert_to_raw(sys.argv[1])
    else:
        convert_to_raw("menu_bg.jpg")
