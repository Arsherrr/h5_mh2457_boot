import os

def extract_unique_chars(input_file_path, output_file_path):
    """
    读取文件，提取所有不重复的可见字符，并保存到新文件中，以便 LVGL 字库转换器使用。
    """
    unique_chars = set()

    # 1. 尝试使用 utf-8 编码读取输入文件
    try:
        with open(input_file_path, 'r', encoding='utf-8') as infile:
            content = infile.read()
            # 将所有字符加入集合中（集合会自动去重）
            unique_chars.update(content)
    except FileNotFoundError:
        print(f"❌ 错误：找不到输入文件 '{input_file_path}'，请检查路径。")
        return
    except Exception as e:
        print(f"❌ 读取文件时出错: {e}")
        return

    # 2. 过滤掉控制字符、换行符和空格（LVGL 字体自带空格，无需在字模中额外提取字符）
    # 过滤列表中排除了：换行 (\n), 回车 (\r), 制表符 (\t), 英文空格, 以及中文全角空格 (\u3000)
    filtered_chars = {c for c in unique_chars if c not in ['\n', '\r', '\t', ' ', '\u3000']}

    # 3. 将字符排序，确保生成的 txt 文件有序，方便人类肉眼对齐校验
    sorted_chars = sorted(list(filtered_chars))
    result_string = "".join(sorted_chars)

    # 4. 写入输出文件
    try:
        with open(output_file_path, 'w', encoding='utf-8') as outfile:
            outfile.write(result_string)
        
        print(f"📊 提取成功！")
        print(f"  - 输入文件: {input_file_path}")
        print(f"  - 输出文件: {output_file_path}")
        print(f"  - 包含不重复字符数: {len(sorted_chars)} 个")
        print(f"💡 提示：你可以直接复制 '{output_file_path}' 内的全部内容，贴入 LVGL 字库转换器的 'List of characters' 框中。")
    except Exception as e:
        print(f"❌ 写入文件时出错: {e}")

if __name__ == "__main__":
    # 配置你的输入和输出文件名（如果脚本与文件在同一目录，直接写文件名即可）
    INPUT_FILE = "text.c"  # 你的源文件（支持 .txt, .c, .h 等）
    OUTPUT_FILE = "lvgl_chars_list.txt"  # 提炼出的字库清单文件

    # 如果你还没建源文件，脚本会自动为你创建一个测试文件演示效果
    if not os.path.exists(INPUT_FILE):
        print(f"ℹ️ 未检测到 '{INPUT_FILE}'，正在为您自动生成一个包含 9 国语言的模板文件...")
        with open(INPUT_FILE, 'w', encoding='utf-8') as f:
            f.write('// 请将你的 9 语言代码粘贴在这里，支持中文、俄语、西语特殊字符等\n')
            f.write('char * text_cn[] = { "升级", "日志", "恢复出厂设置", "确认恢复出厂设置吗?\\n所有的配置数据将被删除!", "处理中, 请勿断电!" };\n')
            f.write('char * text_fr[] = { "MISE À JOUR", "JOURNAUX", "RÉINITIALISATION", "Traitement en cours,\\nne pas éteindre !" };\n')
            f.write('char * text_ru[] = { "ОБНОВЛЕНИЕ", "ЖУРНАЛЫ", "СБРОС НАСТРОЕК", "Выполняется обработка,\\nне выключайте питание!" };\n')

    # 执行字符提取
    extract_unique_chars(INPUT_FILE, OUTPUT_FILE)
