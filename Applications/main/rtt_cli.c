#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SEGGER_RTT.h"
#include "rtt_cli.h"

extern int language;

static rtt_lang_reload_fn s_reload_fn = NULL;

#define RTT_CLI_LINE_MAX 64

static char s_line[RTT_CLI_LINE_MAX];
static unsigned s_line_len = 0;

static void rtt_cli_print_help(void)
{
    printf("\n[RTT] commands:\n");
    printf("  help           - show this help\n");
    printf("  lang           - show current language\n");
    printf("  lang <id>      - set language and refresh current UI\n");
    printf("  language ids:\n");
    printf("    1=CN  3=EN  4=FR  5=ES  6=DE\n");
    printf("    7=RU  8=IT  10=PT  11=PL\n");
    printf("  example: lang 1\n\n");
}

static void rtt_cli_print_lang(void)
{
    static const char *names[] = {
        /*1*/ "CN", /*2*/ "(unused->EN)", /*3*/ "EN", /*4*/ "FR",
        /*5*/ "ES", /*6*/ "DE", /*7*/ "RU", /*8*/ "IT",
        /*9*/ "(unused)", /*10*/ "PT", /*11*/ "PL"
    };
    const char *name = "?";

    if (language >= 1 && language <= 11) {
        name = names[language - 1];
    }
    printf("[RTT] language = %d (%s)\n", language, name);
}

static void rtt_cli_reload_ui(void)
{
    if (s_reload_fn) {
        s_reload_fn();
        printf("[RTT] UI reloaded\n");
    } else {
        printf("[RTT] language updated (no UI reload bound)\n");
    }
}

static void rtt_cli_handle_line(char *line)
{
    char *cmd;
    char *arg;

    while (*line == ' ' || *line == '\t') {
        line++;
    }
    if (*line == '\0') {
        return;
    }

    cmd = line;
    arg = strchr(line, ' ');
    if (arg) {
        *arg++ = '\0';
        while (*arg == ' ' || *arg == '\t') {
            arg++;
        }
        if (*arg == '\0') {
            arg = NULL;
        }
    }

    if (strcmp(cmd, "help") == 0 || strcmp(cmd, "?") == 0) {
        rtt_cli_print_help();
        return;
    }

    if (strcmp(cmd, "lang") == 0) {
        if (arg == NULL) {
            rtt_cli_print_lang();
            return;
        }

        {
            int id = atoi(arg);
            if (id < 1 || id > 11) {
                printf("[RTT] invalid language id: %s\n", arg);
                rtt_cli_print_help();
                return;
            }
            language = id;
            rtt_cli_print_lang();
            rtt_cli_reload_ui();
        }
        return;
    }

    printf("[RTT] unknown cmd: %s\n", cmd);
    rtt_cli_print_help();
}

void rtt_cli_bind_reload(rtt_lang_reload_fn fn)
{
    s_reload_fn = fn;
}

void rtt_cli_init(void)
{
    s_line_len = 0;
    s_line[0] = '\0';
    s_reload_fn = NULL;
    printf("[RTT] input ready. type 'help' then Enter.\n");
    rtt_cli_print_lang();
}

void rtt_cli_poll(void)
{
    int ch;

    while (SEGGER_RTT_HasKey()) {
        ch = SEGGER_RTT_GetKey();
        if (ch < 0) {
            break;
        }

        if (ch == '\r' || ch == '\n') {
            SEGGER_RTT_PutChar(0, '\n');
        } else if (ch == 0x08 || ch == 0x7F) {
            if (s_line_len > 0) {
                s_line_len--;
                s_line[s_line_len] = '\0';
                SEGGER_RTT_Write(0, "\b \b", 3);
            }
            continue;
        } else {
            SEGGER_RTT_PutChar(0, (char)ch);
        }

        if (ch == '\r' || ch == '\n') {
            if (s_line_len > 0) {
                s_line[s_line_len] = '\0';
                rtt_cli_handle_line(s_line);
            }
            s_line_len = 0;
            s_line[0] = '\0';
            continue;
        }

        if (s_line_len + 1 < RTT_CLI_LINE_MAX) {
            s_line[s_line_len++] = (char)ch;
            s_line[s_line_len] = '\0';
        }
    }
}
