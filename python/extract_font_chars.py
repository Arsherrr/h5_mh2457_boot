#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
从 Applications/res/text.c 提取多语言字符串中的去重字符,
生成 LVGL Font Converter / lv_font_conv 可用的字符清单.

默认额外补全常用 ASCII: 0-9 / a-z / A-Z / 常见标点
(避免 OTA 百分比、错误码缺字形).

用法:
  python extract_font_chars.py
  python extract_font_chars.py path/to/text.c
  python extract_font_chars.py --ascii   # 再强制补全全部可打印 ASCII 0x20-0x7E
"""

from __future__ import annotations

import argparse
import os
import re
import sys
from collections import Counter
from pathlib import Path


# 匹配 C 字符串字面量: "...."（支持 \" \\ \n 等转义）
STRING_RE = re.compile(r'"((?:\\.|[^"\\])*)"')

# 常见 C 转义
ESCAPE_MAP = {
    "n": "\n",
    "r": "\r",
    "t": "\t",
    "\\": "\\",
    '"': '"',
    "'": "'",
    "0": "\0",
    "a": "\a",
    "b": "\b",
    "f": "\f",
    "v": "\v",
}


def decode_c_string(raw: str) -> str:
    """把 C 字符串字面量内容还原成 Python 字符串."""
    out = []
    i = 0
    while i < len(raw):
        if raw[i] == "\\" and i + 1 < len(raw):
            nxt = raw[i + 1]
            if nxt in ESCAPE_MAP:
                out.append(ESCAPE_MAP[nxt])
                i += 2
                continue
            if nxt == "x":
                # \xHH
                hex_digits = ""
                j = i + 2
                while j < len(raw) and len(hex_digits) < 2 and raw[j] in "0123456789abcdefABCDEF":
                    hex_digits += raw[j]
                    j += 1
                if hex_digits:
                    out.append(chr(int(hex_digits, 16)))
                    i = j
                    continue
            if nxt in "01234567":
                # \ooo
                oct_digits = nxt
                j = i + 2
                while j < len(raw) and len(oct_digits) < 3 and raw[j] in "01234567":
                    oct_digits += raw[j]
                    j += 1
                out.append(chr(int(oct_digits, 8)))
                i = j
                continue
            # 未知转义: 保留后一字符
            out.append(nxt)
            i += 2
            continue
        out.append(raw[i])
        i += 1
    return "".join(out)


def extract_strings(source: str) -> list[str]:
    """提取源码中所有字符串字面量（解码后）."""
    # 去掉块注释, 避免注释里的引号干扰
    source = re.sub(r"/\*.*?\*/", "", source, flags=re.S)
    source = re.sub(r"//.*?$", "", source, flags=re.M)
    return [decode_c_string(m.group(1)) for m in STRING_RE.finditer(source)]


def script_name(ch: str) -> str:
    """粗分类, 方便统计."""
    cp = ord(ch)
    if ch.isascii() and ch.isprintable():
        return "ASCII"
    if 0x4E00 <= cp <= 0x9FFF:
        return "CJK(中日韩)"
    if 0x0400 <= cp <= 0x04FF:
        return "Cyrillic(俄语)"
    if 0x00C0 <= cp <= 0x024F:
        return "Latin扩展(西欧重音)"
    if 0x1E00 <= cp <= 0x1EFF:
        return "Latin扩展附加"
    if cp in (0x00A1, 0x00BF) or 0x00A0 <= cp <= 0x00FF:
        return "Latin-1"
    return "其他"


def build_charset(strings: list[str], keep_space: bool, add_ascii: bool) -> list[str]:
    chars: set[str] = set()
    for s in strings:
        chars.update(s)

    # 默认丢掉控制字符; 空格可选保留 (字库通常需要 0x20)
    filtered = set()
    for c in chars:
        if ord(c) < 0x20:
            continue
        if c == " " and not keep_space:
            continue
        if c == "\u3000" and not keep_space:
            continue
        filtered.add(c)

    # 常用基础字符: 数字 / 大小写字母 / 常见标点 (OTA 百分比、错误码等会用到)
    # 即使 text.c 未出现也强制纳入, 避免字库缺字形
    common = (
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        " !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"
    )
    filtered.update(common)

    if add_ascii:
        for cp in range(0x20, 0x7F):
            filtered.add(chr(cp))

    # 字库转换器一般需要空格
    filtered.add(" ")

    return sorted(filtered, key=lambda c: ord(c))


def format_ranges(codepoints: list[int]) -> str:
    """合并连续码点为 lv_font_conv 风格: 0x20-0x7F,0x4E00,..."""
    if not codepoints:
        return ""
    ranges = []
    start = prev = codepoints[0]
    for cp in codepoints[1:]:
        if cp == prev + 1:
            prev = cp
            continue
        ranges.append(f"0x{start:X}" if start == prev else f"0x{start:X}-0x{prev:X}")
        start = prev = cp
    ranges.append(f"0x{start:X}" if start == prev else f"0x{start:X}-0x{prev:X}")
    return ",".join(ranges)


def main() -> int:
    root = Path(__file__).resolve().parent
    default_text_c = root.parent / "Applications" / "res" / "text.c"

    parser = argparse.ArgumentParser(description="从 text.c 提取 LVGL 字体字符集")
    parser.add_argument("input", nargs="?", default=str(default_text_c), help="text.c 路径")
    parser.add_argument("-o", "--outdir", default=str(root / "font_chars"), help="输出目录")
    parser.add_argument("--ascii", action="store_true", help="强制补全 ASCII 0x20-0x7E")
    parser.add_argument("--no-space", action="store_true", help="不强制加入空格")
    args = parser.parse_args()

    in_path = Path(args.input)
    if not in_path.exists():
        print(f"[Error] 找不到文件: {in_path}")
        return 1

    source = in_path.read_text(encoding="utf-8")
    strings = extract_strings(source)
    if not strings:
        print("[Error] 未提取到任何字符串字面量")
        return 1

    # 统计: 每个字符串出现的字符
    char_freq = Counter()
    for s in strings:
        char_freq.update(ch for ch in s if ord(ch) >= 0x20)

    charset = build_charset(strings, keep_space=not args.no_space, add_ascii=args.ascii)
    codepoints = [ord(c) for c in charset]

    outdir = Path(args.outdir)
    outdir.mkdir(parents=True, exist_ok=True)

    # 1) 一串字符: 粘贴到 LVGL Online Font Converter -> Symbols
    symbols_path = outdir / "lvgl_symbols.txt"
    symbols_path.write_text("".join(charset), encoding="utf-8")

    # 2) 每行一个字符, 方便人工核对
    lines_path = outdir / "lvgl_chars_lines.txt"
    lines_path.write_text("\n".join(charset) + "\n", encoding="utf-8")

    # 3) 码点列表 / range (给 lv_font_conv --symbols / --range)
    cps_path = outdir / "lvgl_codepoints.txt"
    cps_path.write_text("\n".join(f"U+{cp:04X}  {chr(cp)}" for cp in codepoints) + "\n", encoding="utf-8")

    ranges = format_ranges(codepoints)
    (outdir / "lvgl_ranges.txt").write_text(ranges + "\n", encoding="utf-8")

    # 4) 简要统计报告
    by_script = Counter(script_name(c) for c in charset)
    report = []
    report.append(f"源文件: {in_path}")
    report.append(f"字符串数量: {len(strings)}")
    report.append(f"去重字符数: {len(charset)}")
    report.append("")
    report.append("按文字系统统计:")
    for name, cnt in sorted(by_script.items(), key=lambda x: (-x[1], x[0])):
        report.append(f"  {name}: {cnt}")
    report.append("")
    report.append("输出文件:")
    report.append(f"  {symbols_path.name}  <- Online Font Converter 的 Symbols 框")
    report.append(f"  {lines_path.name}    <- 每行一字, 便于核对")
    report.append(f"  {cps_path.name} <- 码点对照表")
    report.append(f"  lvgl_ranges.txt     <- 合并区间 (可参考)")
    report.append("")
    report.append("lv_font_conv 示例:")
    report.append(
        "  npx lv_font_conv --font YourFont.ttf --size 42 --bpp 4 "
        f"--format lvgl --symbols \"$(cat {symbols_path.name})\" "
        "-o lv_font_puhui_42.c"
    )
    report_text = "\n".join(report) + "\n"
    (outdir / "report.txt").write_text(report_text, encoding="utf-8")

    print(report_text)
    print(f"字符预览 ({len(charset)}):")
    # 控制台可能对部分字符编码失败, 安全打印
    preview = "".join(charset)
    try:
        print(preview)
    except UnicodeEncodeError:
        sys.stdout.buffer.write((preview + "\n").encode("utf-8", errors="replace"))

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
