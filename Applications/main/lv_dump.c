#include <stdio.h>
#include "lvgl.h"

typedef struct {
    const lv_obj_class_t * cls;
    const char * name;
} widget_type_map_t;

static const char * get_widget_type_name(lv_obj_t * obj)
{
    static const widget_type_map_t map[] = {
        {&lv_label_class,        "Text"},
        {&lv_image_class,        "Img"},
        {&lv_button_class,       "Button"},
        {&lv_slider_class,       "Slider"},
        {&lv_bar_class,          "Bar"},
        {&lv_checkbox_class,     "CheckBox"},
        {&lv_switch_class,       "Switch"},
        {&lv_textarea_class,     "TextArea"},
        {&lv_dropdown_class,     "DropDown"},
        {&lv_canvas_class,       "Canvas"},
        {&lv_roller_class,       "Roller"},
        {&lv_scale_class,        "Scale"},
        {&lv_line_class,         "Line"},
        {&lv_arc_class,          "Arc"},
        {&lv_table_class,        "Table"},
        {&lv_obj_class,          "BaseObj"}
    };

    for (size_t i = 0; i < sizeof(map) / sizeof(map[0]); i++) {
        if (lv_obj_check_type(obj, map[i].cls)) {
            return map[i].name;
        }
    }
    return "Unknown";
}

static int get_obj_level(lv_obj_t * obj, lv_obj_t * root)
{
    int level = 0;
    lv_obj_t * parent = lv_obj_get_parent(obj);
    
    /* 顺着父节点一路向上找, 直到到达设定的根节点. */
    while (parent && parent != root) {
        level++;
        parent = lv_obj_get_parent(parent);
    }
    return level;
}

/* 树状图遍历回调函数. */
static lv_obj_tree_walk_res_t tree_dump_cb(lv_obj_t * obj, void * user_data)
{
    uint32_t * total_count = (uint32_t *)user_data;
    (*total_count)++;

    /* 获取当前活动屏幕作为根节点. */
    lv_obj_t * root = lv_screen_active();
    int level = get_obj_level(obj, root);

    for (int i = 0; i < level; i++) {
        if (i == level - 1) {
            rt_kprintf("|-- "); /* 最后一层显示分支线. */
        } else {
            rt_kprintf("|   "); /* 顶层显示垂直线. */
        }
    }
    
    /* 根节点本身. */
    if (obj == root) {
        rt_kprintf("+-- ");
    }

    /* 获取控件类型名称. */
    const char * type_name = get_widget_type_name(obj);
    
    rt_kprintf("[%s] @ %p\n", type_name, obj);

    return LV_OBJ_TREE_WALK_NEXT;
}

void lv_dump(int argc, char **argv)
{
    lv_display_t * disp = lv_display_get_default();
    if (disp && disp->rendering_in_progress) {
        rt_kprintf("[LV DUMP] Error: LVGL is rendering now. Please try again later.\n");
        return;
    }

    rt_kprintf("\n============== LVGL CONTROL TREE ==============\n");
    
    uint32_t total_widgets = 0;

    /* 深度优先遍历. */
    lv_obj_tree_walk(lv_screen_active(), tree_dump_cb, &total_widgets);
    
    rt_kprintf("-----------------------------------------------\n");
    rt_kprintf(" >>>> Total [ %d ] widgets\n", total_widgets);
    rt_kprintf("===============================================\n");
}
MSH_CMD_EXPORT(lv_dump, Dump lvgl widget.);
