import os
import random
import numpy as np
from PIL import Image, ImageFilter, ImageEnhance

def generate_random_dark_acrylic(width=854, height=480, num_images=15, output_dir="random_acrylic_bgs"):
    """
    大批量随机生成 854x480 暗黑亚克力微光背景
    左上角：深蓝光晕（随机位置/大小）
    正中间：金黄/琥珀色文字光晕（随机位置/拉伸度/大小）
    """
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    x = np.arange(width)
    y = np.arange(height)
    xv, yv = np.meshgrid(x, y)

    print(f"开始批量生成 {num_images} 张随机暗黑微光背景...")

    for img_idx in range(1, num_images + 1):
        # 1. 基础暗部背景：赋予极低饱和度的暗蓝黑色，比纯死黑更有通透的玻璃感
        base_img = np.zeros((height, width, 3), dtype=np.float32)
        base_img += np.array([4, 6, 10], dtype=np.float32) 

        # 2. 随机生成【左上角深蓝光晕】参数
        center_blue_x = random.randint(10, 200)       # 允许光晕移出边界或偏向中央
        center_blue_y = random.randint(10, 150)
        radius_blue = random.randint(220, 400)         # 随机控制光的扩散范围
        # 随机让蓝光带有一点紫或一点青
        color_blue = np.array([
            random.uniform(5, 15),                     # R
            random.uniform(25, 45),                    # G
            random.uniform(90, 140)                    # B
        ], dtype=np.float32)
        
        # 计算蓝光的距离衰减 (使用随机指数控制光边缘的陡峭度)
        dist_blue = np.sqrt((xv - center_blue_x)**2 + (yv - center_blue_y)**2)
        exponent_blue = random.uniform(1.8, 2.5)
        glow_blue = np.clip(1.0 - (dist_blue / radius_blue), 0, 1) ** exponent_blue
        base_img += glow_blue[:, :, None] * color_blue

        # 3. 随机生成【中间金黄色文字光晕】参数
        # 围绕中心点 (427, 240) 进行小范围随机偏移
        center_yellow_x = (width // 2) + random.randint(-60, 60)
        center_yellow_y = (height // 2) + random.randint(-40, 40)
        radius_yellow = random.randint(150, 280)
        
        # 关键：随机改变椭圆的拉伸比例（1.2到2.0倍），让文字背后的光晕形状每张都不同
        stretch_factor = random.uniform(1.2, 2.2)
        dist_yellow_ellipse = np.sqrt(((xv - center_yellow_x) / stretch_factor)**2 + (yv - center_yellow_y)**2)
        
        # 随机微调金黄/琥珀色的色调，有的偏橙红，有的偏明黄
        color_yellow = np.array([
            random.uniform(110, 160),                  # R (主导)
            random.uniform(70, 110),                   # G 
            random.uniform(10, 25)                     # B
        ], dtype=np.float32)
        
        exponent_yellow = random.uniform(2.0, 3.0)
        glow_yellow = np.clip(1.0 - (dist_yellow_ellipse / radius_yellow), 0, 1) ** exponent_yellow
        base_img += glow_yellow[:, :, None] * color_yellow

        # 4. 随机弱光折射纹理 (模拟亚克力内部的微弱反光层)
        freq = random.uniform(30.0, 60.0)
        warp = np.sin(xv / freq) * np.cos(yv / freq)
        base_img += (warp[:, :, None] + 1.0) * random.uniform(0.5, 2.0)

        # 5. 裁剪并转换为 PIL 图像
        base_img = np.clip(base_img, 0, 255).astype(np.uint8)
        img = Image.fromarray(base_img)

        # 6. 随机模糊半径：决定光晕是凝聚的还是完全散开的氤氲感
        blur_radius = random.randint(35, 55)
        img = img.filter(ImageFilter.GaussianBlur(radius=blur_radius))

        # 7. 随机对比度微调
        contrast_factor = random.uniform(1.0, 1.2)
        enhancer = ImageEnhance.Contrast(img)
        img = enhancer.enhance(contrast_factor)

        # 8. 严格输出为符合 STM32 硬件 JPEG 解码的 YCbCr 4:2:0 格式
        output_path = os.path.join(output_dir, f"acrylic_random_{img_idx:03d}.jpg")
        img.save(output_path, "JPEG", quality=95, subsampling=1)
        print(f"已生成第 {img_idx:03d} 张: {output_path} (模糊度:{blur_radius}, 拉伸比:{stretch_factor:.2f})")

if __name__ == "__main__":
    # 这里设置 15 张，你可以改成 30 或 50 一次性看个够
    generate_random_dark_acrylic(width=854, height=480, num_images=15)
    print("\n批量生成完毕！请进入 'random_acrylic_bgs' 文件夹，挑选最符合你心意的一款。")
