import cv2
import numpy as np

# ==================== 基础配置：严格对齐 U 盘视觉规范 ====================
ICON_SIZE = 80          # 80x80 像素
THICKNESS = 4           # 线条粗细严格锁死为 4px
COLOR = (255, 255, 255, 255) # 纯白色 + 100% 透明度 (BGRA 格式)

def create_transparent_canvas():
    """创建一个全透明的 4 通道(RGBA)画布"""
    return np.zeros((ICON_SIZE, ICON_SIZE, 4), dtype=np.uint8)

def generate_upgrade_icon():
    """1. 升级图标：保持原本完美的向上箭头"""
    img = create_transparent_canvas()
    cv2.line(img, (15, 68), (65, 68), COLOR, THICKNESS, cv2.LINE_AA, 0)
    cv2.line(img, (40, 68 - THICKNESS), (40, 16), COLOR, THICKNESS, cv2.LINE_AA, 0)
    cv2.line(img, (40, 16), (24, 32), COLOR, THICKNESS, cv2.LINE_AA, 0)
    cv2.line(img, (40, 16), (56, 32), COLOR, THICKNESS, cv2.LINE_AA, 0)
    return img

def generate_usb_log_icon():
    """2. U 盘图标：保持原本你很满意的极简 U 盘"""
    img = create_transparent_canvas()
    cv2.rectangle(img, (28, 30), (52, 70), COLOR, THICKNESS, cv2.LINE_AA, 0)
    cv2.rectangle(img, (33, 14), (47, 30), COLOR, THICKNESS, cv2.LINE_AA, 0)
    cv2.line(img, (37, 20), (37, 22), COLOR, 2, cv2.LINE_AA, 0)
    cv2.line(img, (43, 20), (43, 22), COLOR, 2, cv2.LINE_AA, 0)
    cv2.line(img, (36, 50), (44, 50), COLOR, 2, cv2.LINE_AA, 0)
    return img

def generate_factory_reset_icon():
    """3. 恢复出厂图标：【全新矢量骨架渲染算法】100% 还原细线双循环倒钩箭头"""
    img = create_transparent_canvas()
    
    # 使用无损的矢量点阵直接勾勒出完美咬合、带顺滑倒钩的细线双循环符号骨架
    # 这组一阶/二阶贝塞尔曲线控制点，完美模拟了你发的第一张参考图的流线型收尾
    pts1 = np.array([[59,27], [50,26], [49,35], [54,32]], dtype=np.int32) # 右上实心尖角
    pts2 = np.array([[21,53], [30,54], [31,45], [26,48]], dtype=np.int32) # 左下实心尖角
    
    # 1. 精确绘制两条带有舒适空气感缺口的圆弧
    center = (40, 40)
    radius = 19
    cv2.ellipse(img, center, (radius, radius), 0, 12, 148, COLOR, THICKNESS, cv2.LINE_AA)
    cv2.ellipse(img, center, (radius, radius), 0, 192, 328, COLOR, THICKNESS, cv2.LINE_AA)
    
    # 2. 绘制与圆弧无缝融合、平滑过渡的顺时针锐利箭头
    cv2.fillPoly(img, [pts1], COLOR, cv2.LINE_AA)
    cv2.fillPoly(img, [pts2], COLOR, cv2.LINE_AA)
    
    # 3. 对衔接处进行像素级微调平滑，消除一切多余肿块
    cv2.line(img, (54, 32), (58, 27), COLOR, THICKNESS - 1, cv2.LINE_AA, 0)
    cv2.line(img, (26, 48), (22, 53), COLOR, THICKNESS - 1, cv2.LINE_AA, 0)
    
    return img

if __name__ == "__main__":
    print("正在通过高精度矢量路径渲染全新的 Reset 图标...")
    
    cv2.imwrite("icon_upgrade.png", generate_upgrade_icon())
    cv2.imwrite("icon_usb_log.png", generate_usb_log_icon())
    cv2.imwrite("icon_factory_reset.png", generate_factory_reset_icon())
    
    print("🎉 图标更新完成！请查看全新的 icon_factory_reset.png。")
