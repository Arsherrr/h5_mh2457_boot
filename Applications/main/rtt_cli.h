#ifndef __RTT_CLI_H
#define __RTT_CLI_H

typedef void (*rtt_lang_reload_fn)(void);

void rtt_cli_init(void);
void rtt_cli_poll(void);

/** 登记当前界面的语言刷新回调；进入新界面时覆盖，传 NULL 清除 */
void rtt_cli_bind_reload(rtt_lang_reload_fn fn);

#endif
