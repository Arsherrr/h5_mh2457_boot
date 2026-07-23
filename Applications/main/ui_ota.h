#ifndef __UI_H
#define __UI_H

typedef enum {
    COLOR_WHITE = 0,
    COLOR_YELLOW,
    COLOR_RED,
    COLOR_GREEN,
} ui_color_e;

void ui_ota_init(void);
void process_update(unsigned char pct);
void ui_set_notice(char *msg, ui_color_e color);
void ui_set_pct_visible(unsigned char visible);
void ui_set_pct_color(ui_color_e color);

#endif
