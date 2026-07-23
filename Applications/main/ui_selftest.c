#include "lvgl.h"

// 外部资源声明
extern lv_draw_buf_t menu_bg_buf;     // 854x480 背景图片缓存
extern lv_draw_buf_t menu_back;      // 错误/损坏 图标缓存（建议 32x32 或 48x48）
extern lv_indev_t *g_indevKeyPad;
#define menu_correct menu_back
#define menu_error   
extern lv_draw_buf_t menu_correct;    // 正常/正确 图标缓存（建议 32x32 或 48x48）

// 全局变量，用于页面切换时的键盘组管理
static lv_obj_t * self_check_scr = NULL;
static lv_group_t * check_group = NULL;

LV_FONT_DECLARE(lv_font_puhui_42);
LV_FONT_DECLARE(lv_font_puhui_40);

// 声明外部的进入升级界面函数
// extern void ui_ota(void);
void ui_ota(void)
{

}

// 升级按钮的按键事件回调
static void check_btn_event_cb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_key_t key = lv_event_get_key(e);

    if (code == LV_EVENT_KEY && key == LV_KEY_ENTER) {
        // 当用户点击确认（Enter 键）后，立刻销毁自检页面并跳转到 OTA 界面
        if (check_group) {
            lv_group_del(check_group);
            check_group = NULL;
        }
        // 执行跳转
        ui_ota();
    }
}

/**
 * @brief 系统上电自检提示页面
 * @param app_ok 1: APP固件正常, 0: APP固件损坏
 * @param res_ok 1: 资源文件正常, 0: 资源文件丢失/错误
 */
void ui_system_self_check_init(int app_ok, int res_ok)
{
    // 1. 创建全新的独立自检屏幕并加载
    self_check_scr = lv_obj_create(NULL);
    lv_screen_load(self_check_scr);

    // 2. 创建全屏背景图像控件
    lv_obj_t * bg_img = lv_image_create(self_check_scr);
    lv_image_set_src(bg_img, &menu_bg_buf);
    lv_obj_set_size(bg_img, 854, 480);
    lv_obj_set_pos(bg_img, 0, 0);

    // 3. 创建主内容容器，将屏幕划分为上下两个轨道
    lv_obj_t * main_cont = lv_obj_create(self_check_scr);
    lv_obj_remove_style_all(main_cont);
    lv_obj_set_size(main_cont, 600, 400);
    lv_obj_center(main_cont);
    
    // 配置主容器为垂直 Flex 布局
    lv_obj_set_layout(main_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(main_cont, LV_FLEX_FLOW_COLUMN);
    // 垂直方向两端对齐：上半部分放自检列表，底部放升级按钮
    lv_obj_set_flex_align(main_cont, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // ----------------------------------------------------
    // 【上半轨道】：自检状态信息展示区域
    // ----------------------------------------------------
    lv_obj_t * info_cont = lv_obj_create(main_cont);
    lv_obj_remove_style_all(info_cont);
    lv_obj_set_size(info_cont, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_layout(info_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(info_cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(info_cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(info_cont, 20, 0); // 两行状态信息之间留 20px 间距
    lv_obj_set_style_pad_top(info_cont, 40, LV_PART_MAIN); // 顶部留白

    // --- 第一行：APP 固件检测状态 ---
    lv_obj_t * row_app = lv_obj_create(info_cont);
    lv_obj_remove_style_all(row_app);
    lv_obj_set_size(row_app, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_layout(row_app, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(row_app, LV_FLEX_FLOW_ROW); // 横向排列图标与文字
    lv_obj_set_flex_align(row_app, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(row_app, 15, 0);       // 图标与文字之间留 15px 间距

    lv_obj_t * icon_app = lv_image_create(row_app);
    lv_image_set_src(icon_app, app_ok ? &menu_back : &menu_back);

    lv_obj_t * label_app = lv_label_create(row_app);
    lv_label_set_text(label_app, app_ok ? "Firmware System Ready" : "Firmware Damaged or Missing");
    lv_obj_set_style_text_font(label_app, &lv_font_puhui_40, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_app, app_ok ? lv_color_hex(0x10B981) : lv_color_hex(0xEF4444), LV_PART_MAIN);

    // --- 第二行：资源文件检测状态 ---
    lv_obj_t * row_res = lv_obj_create(info_cont);
    lv_obj_remove_style_all(row_res);
    lv_obj_set_size(row_res, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_layout(row_res, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(row_res, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row_res, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(row_res, 15, 0);

    lv_obj_t * icon_res = lv_image_create(row_res);
    lv_image_set_src(icon_res, res_ok ? &menu_back : &menu_back);

    lv_obj_t * label_res = lv_label_create(row_res);
    lv_label_set_text(label_res, res_ok ? "Resource Files Checked" : "Resource Files Corrupted");
    lv_obj_set_style_text_font(label_res, &lv_font_puhui_40, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_res, res_ok ? lv_color_hex(0x10B981) : lv_color_hex(0xEF4444), LV_PART_MAIN);


    // ----------------------------------------------------
    // 【下半轨道】：强固定的按钮升级交互区域
    // ----------------------------------------------------
    lv_obj_t * btn_upgrade = lv_button_create(main_cont);
    lv_obj_set_size(btn_upgrade, 200, 60);
    lv_obj_set_style_radius(btn_upgrade, 12, LV_PART_MAIN);
    
    // 为了防止键盘切焦时卡片或按钮产生不必要的物理漂移，保底关闭滚动
    lv_obj_remove_flag(btn_upgrade, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_flag(btn_upgrade, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    // 默认高亮（由于是全屏唯一的按键，使其上电直接呈现蓝色视觉聚焦）
    lv_obj_set_style_bg_color(btn_upgrade, lv_color_hex(0x3B82F6), LV_PART_MAIN);
    lv_obj_set_style_border_color(btn_upgrade, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_border_width(btn_upgrade, 2, LV_PART_MAIN);

    lv_obj_t * btn_label = lv_label_create(btn_upgrade);
    lv_label_set_text(btn_label, "UPGRADE NOW");
    lv_obj_center(btn_label);
    lv_obj_set_style_text_font(btn_label, &lv_font_puhui_40, LV_PART_MAIN);
    lv_obj_set_style_text_color(btn_label, lv_color_white(), LV_PART_MAIN);

    // 5. 绑定独立按键管理组：实现 0 毫秒即时聚焦和按键分发
    check_group = lv_group_create();
    lv_indev_set_group(g_indevKeyPad, check_group);
    
    lv_group_add_obj(check_group, btn_upgrade);
    lv_group_focus_obj(btn_upgrade); // 锁定聚焦到升级按钮上
    lv_obj_add_event_cb(btn_upgrade, check_btn_event_cb, LV_EVENT_KEY, NULL);
}
