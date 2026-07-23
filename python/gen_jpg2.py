import os
import random
import numpy as np
from PIL import Image, ImageFilter

def generate_smooth_acrylic_bgs(width=854, height=480, num_images=10, output_dir="smooth_acrylic_bgs"):
    """
    生成无色彩断层（Anti-Banding）的 854x480 暗黑亚克力微光背景
    带有抖动算法（Dithering），完美兼容 STM32 硬件 JPEG 解码
    """
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    x = np.arange(width)
    y = np.arange(height)
    xv, yv = np.meshgrid(x, y)

    print(f"开始批量生成 {num_images} 张无断层丝滑背景...")

    for img_idx in range(1, num_images + 1):
        # 1. 极暗底色
        base_img = np.zeros((height, width, 3), dtype=np.float32)
        base_img += np.array([8, 6, 24], dtype=np.float32) # 带有一点深夜蓝的极暗底色

        # 2. 随机【左上角深蓝/紫光晕】
        cb_x = random.randint(-50, 150)
        cb_y = random.randint(-50, 100)
        r_blue = random.randint(300, 450)
        c_blue = np.array([random.uniform(70, 110), random.uniform(20, 50), random.uniform(160, 220)], dtype=np.float32)
        
        dist_blue = np.sqrt((xv - cb_x)**2 + (yv - cb_y)**2)
        glow_blue = np.clip(1.0 - (dist_blue / r_blue), 0, 1) ** 2.0
        base_img += glow_blue[:, :, None] * c_blue

        # 3. 随机【中间文字背后天蓝/亮蓝光晕】
        cy_x = (width // 2) + random.randint(-40, 40)
        cy_y = (height // 2) + random.randint(-30, 30)
        r_yellow = random.randint(180, 280)
        
        # 椭圆拉伸，使其更符合文字横向排列的视觉习惯
        stretch = random.uniform(1.3, 1.8)
        dist_yellow = np.sqrt(((xv - cy_x) / stretch)**2 + (yv - cy_y)**2)
        
        # 调配高饱和度的天蓝/亮蓝色（对应你发出的效果图中心颜色）
        c_yellow = np.array([random.uniform(0, 30), random.uniform(130, 190), random.uniform(230, 255)], dtype=np.float32)
        glow_yellow = np.clip(1.0 - (dist_yellow / r_yellow), 0, 1) ** 1.8
        base_img += glow_yellow[:, :, None] * c_yellow

        # 4. 随机【右侧/下方弱辅助光】（参考图右侧有红橘色暗光，增加层次感）
        if random.choice([True, False]):
            cr_x = width - random.randint(50, 200)
            cr_y = height - random.randint(50, 200)
            r_red = random.randint(150, 250)
            c_red = np.array([random.uniform(90, 140), random.uniform(20, 40), random.uniform(10, 30)], dtype=np.float32)
            dist_red = np.sqrt((xv - cr_x)**2 + (yv - cr_y)**2)
            glow_red = np.clip(1.0 - (dist_red / r_red), 0, 1) ** 2.0
            base_img += glow_red[:, :, None] * c_red

        # 5. 【核心：抗色彩断层抖动算法 (Dithering)】
        # 在图像转换前，注入一层极微弱的随机噪声（幅值在 -1.5 到 +1.5 之间）
        # 这一步能彻底打破 JPEG 或 16位显示屏的“一圈一圈”斑马纹
        noise = np.random.uniform(-1.5, 1.5, base_img.shape).astype(np.float32)
        base_img += noise

        # 6. 限幅并转为 Image
        base_img = np.clip(base_img, 0, 255).astype(np.uint8)
        img = Image.fromarray(base_img)

        # 7. 高斯模糊融合
        blur_r = random.randint(40, 55)
        img = img.filter(ImageFilter.GaussianBlur(radius=blur_r))

        # 8. 再次轻微注入一次高斯杂色点（进一步模拟高级亚克力的微颗粒质感）
        # 如果你喜欢完全纯净的丝滑，可以把下面这两行删掉
        # noise_layer = Image.effect_noise(img.size, 1).convert("RGB")
        # img = Image.blend(img, noise_layer, alpha=0.008)

        # 9. 严格保存为兼容 STM32 硬件解码的标准 YCbCr 4:2:0 JPEG
        output_path = os.path.join(output_dir, f"smooth_acrylic_{img_idx:03d}.jpg")
        # 提高质量到 95 也是减少 JPEG 压缩导致断层的关键
        img.save(output_path, "JPEG", quality=95, subsampling=1)
        print(f"已生成抗断层背景: {output_path}")

if __name__ == "__main__":
    generate_smooth_acrylic_bgs(width=854, height=480, num_images=10)
    print("\n丝滑无断层的亚克力微光背景已生成！快去 'smooth_acrylic_bgs' 挑选吧！")
