#include <string.h>
#include <stdio.h>
#include "text.h"

/* 中文. */
const char * text_cn[] = {
    /* MENU. */
    "升级", "日志", "恢复出厂\n设置",
    
    /* OTA. */
    "等待升级...",
    "升级中...",
    "升级 LOGO...",
    "升级文本...",
    "升级图片...",
    "升级失败",
    "升级成功",

    /* LOG. */
    "请在电脑中打开\n名为 \"H5-Disk\" 的磁盘\n将日志拷贝出来\n按下返回键 < 退出",

    /* FACTORY RESET. */
    "确认恢复出厂设置吗?\n所有的配置数据将被删除!",
    "处理中, 请勿断电!"
};

/* 英文. */
const char * text_en[] = { 
    /* MENU. */ 
    "UPGRADE", "LOGS", "FACTORY RESET", 
    /* OTA. */ 
    "Waiting for upgrade...", "Upgrading...", "Upgrading LOGO...", "Upgrading text...", "Upgrading images...", "Upgrade failed", "Upgrade successful", 
    /* LOG. */ 
    "Please open the drive\nnamed \"H5-Disk\" on PC\nto copy the logs.\nPress Back key < to exit", 
    /* FACTORY RESET. */ 
    "Confirm factory reset?\nAll configuration data\nwill be deleted!",
    "Processing, please do not power off!"
};

/* 法语. */
const char * text_fr[] = { 
    /* MENU. */ 
    "MISE À JOUR", "JOURNAUX", "RÉINITIALISATION", 
    /* OTA. */ 
    "En attente de mise à jour...", "Mise à jour en cours...", "Mise à jour du LOGO...", "Mise à jour du texte...", "Mise à jour des images...", "Échec de la mise à jour", "Mise à jour réussie", 
    /* LOG. */ 
    "Veuillez ouvrir le lecteur\nnommé \"H5-Disk\" sur PC\npour copier les journaux.\nRetour < pour quitter", 
    /* FACTORY RESET. */ 
    "Confirmer la réinitialisation ?\nToutes les données de\nconfiguration seront effacées !",
    "Traitement en cours,\nne pas éteindre !"
};

/* 西班牙语. */
const char * text_es[] = { 
    /* MENU. */ 
    "ACTUALIZACIÓN", "REGISTROS", "RESTABLECER", 
    /* OTA. */ 
    "Esperando actualización...", "Actualizando...", "Actualizando LOGO...", "Actualizando texto...", "Actualizando imágenes...", "Error de actualización", "Actualización exitosa", 
    /* LOG. */ 
    "Abra la unidad llamada\n\"H5-Disk\" en el PC\npara copiar los registros.\nPulse Atrás < para salir", 
    /* FACTORY RESET. */ 
    "¿Confirmar restablecimiento?\n¡Se borrarán todos los\ndatos de configuración!",
    "Procesando,\n¡no apagar el equipo!"
};

const char * text_de[] = { 
    /* MENU. */ 
    "UPGRADE", "PROTOKOLLE", "WERKSEINSTELLUNG", 
    /* OTA. */ 
    "Warten auf Upgrade...", "Upgrade läuft...", "Upgrade LOGO...", "Upgrade Text...", "Upgrade Bilder...", "Upgrade fehlgeschlagen", "Upgrade erfolgreich", 
    /* LOG. */ 
    "Bitte das Laufwerk\n\"H5-Disk\" am PC öffnen,\num Protokolle zu kopieren.\nZurück < zum Verlassen", 
    /* FACTORY RESET. */ 
    "Werkseinstellung\nbestätigen?\nAlle Konfigurationsdaten\nwerden gelöscht!",
    "Verarbeitung läuft,\nbitte nicht ausschalten!"
};

const char * text_ru[] = { 
    /* MENU. */ 
    "ОБНОВЛЕНИЕ", "ЖУРНАЛЫ", "СБРОС НАСТРОЕК", 
    /* OTA. */ 
    "Ожидание обновления...", "Обновление...", "Обновление LOGO...", "Обновление текста...", "Обновление изображений...", "Ошибка обновления", "Обновление успешно", 
    /* LOG. */ 
    "Откройте на ПК диск\nс именем \"H5-Disk\",\nчтобы скопировать журналы.\nНажмите Назад < для выхода", 
    /* FACTORY RESET. */ 
    "Подтвердить сброс?\nВсе данные конфигурации\nбудут удалены!",
    "Выполняется обработка,\nне выключайте питание!"
};

const char * text_it[] = { 
    /* MENU. */ 
    "AGGIORNAMENTO", "REGISTRI", "RIPRISTINO", 
    /* OTA. */ 
    "In attesa di aggiornamento...", "Aggiornamento in corso...", "Aggiornamento LOGO...", "Aggiornamento testo...", "Aggiornamento immagini...", "Aggiornamento fallito", "Aggiornamento riuscito", 
    /* LOG. */ 
    "Aprire l'unità denominata\n\"H5-Disk\" sul PC\nper copiare i registri.\nPremere Indietro < per uscire", 
    /* FACTORY RESET. */ 
    "Confermare il ripristino?\nTutti i dati di\nconfigurazione verranno eliminati!",
    "Elaborazione in corso,\nnon spegnere il dispositivo!"
};

const char * text_pt[] = { 
    /* MENU. */ 
    "ATUALIZAÇÃO", "REGISTROS", "REPOSIÇÃO", 
    /* OTA. */ 
    "Aguardando atualização...", "Atualizando...", "Atualizando LOGO...", "Atualizando texto...", "Atualizando imagens...", "Falha na atualização", "Atualização bem-sucedida", 
    /* LOG. */ 
    "Abra a unidade chamada\n\"H5-Disk\" no PC\npara copiar os registros.\nPremere Voltar < para sair", 
    /* FACTORY RESET. */ 
    "Confirmar reposição?\nTodos os dados de\nconfiguração serão apagados!",
    "Processando,\nnão desligue o aparelho!"
};

const char * text_pl[] = { 
    /* MENU. */ 
    "AKTUALIZACJA", "LOGI", "RESET FABRYCZNY", 
    /* OTA. */ 
    "Oczekiwanie na aktualizację...", "Aktualizacja...", "Aktualizacja LOGO...", "Aktualizacja tekstu...", "Aktualizacja obrazów...", "Aktualizacja nieudana", "Aktualizacja powiodła się", 
    /* LOG. */ 
    "Otwórz na komputerze dysk\no nazwie \"H5-Disk\",\naby skopiować logi.\nNaciśnij Wstecz < aby wyjść", 
    /* FACTORY RESET. */ 
    "Potwierdzić reset?\nWszystkie dane\nkonfiguracyjne zostaną usunięte!",
    "Przetwarzanie,\nnie wyłączaj zasilania!"
};

const char **text[] = {
    /* 1  */ text_cn,
    /* 2  */ NULL,
    /* 3  */ text_en,
    /* 4  */ text_fr,
    /* 5  */ text_es,
    /* 6  */ text_de,
    /* 7  */ text_ru,
    /* 8  */ text_it,
    /* 9  */ NULL,
    /* 10 */ text_pt,
    /* 11 */ text_pl,
    /* 12 */ NULL,
    /* 13 */ NULL,
    /* 14 */ NULL,
    /* 15 */ NULL,
    /* 16 */ NULL,
};
const int  lang_num = sizeof(text) / sizeof(text[0]);
const char empty = '\0';

char *get_string(int lang, text_id_e id)
{   
    if (id >= TEXT_ID_NUM) {
        printf("[TEXT] Text ID not found.\n");
        return (char *)&empty;
    }

    if ((unsigned int)lang > lang_num || lang == 0) {
        printf("[TEXT] Language (%d) not found, reset to English.\n", lang);
        lang = 2;
    }
    
    char ** lang_text = (char **)text[lang - 1];

    /* 未适配的语言使用英文. */
    if (lang_text == NULL) lang_text = (char **)text[2];

    return (char *)lang_text[id];
}
