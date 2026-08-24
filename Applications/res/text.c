#include <string.h>
#include <stdio.h>
#include "text.h"

/* 中文. */
const char * text_cn[] = {
    /* MENU. */
    "升级", "日志", "重置",
    
    /* OTA. */
    "等待升级...",
    "升级中...",
    "升级 LOGO...",
    "升级文本...",
    "升级图片...",
    "升级失败",
    "升级成功",

    /* LOG. */
    "请在电脑中打开\n名为 \"H5\" 的磁盘\n拷贝日志并保存",

    /* FACTORY RESET. */
    "确认重置吗?\n所有的配置数据将被删除!",
    "处理中, 请勿断电!",

    /* SELFTEST. */
    "应用程序",
    "资源文件",
    "去升级",
    "检测到系统文件已损坏",
    "检测到需要修复的项目",
    "正常",
    "损坏",
    "按下确认键继续"
};

/* 英文. */
const char * text_en[] = { 
    /* MENU. */ 
    "UPGRADE", "LOGS", "RESET", 
    /* OTA. */ 
    "Waiting for upgrade...", "Upgrading...", "Upgrading LOGO...", "Upgrading text...", "Upgrading images...", "Upgrade failed", "Upgrade successful", 
    /* LOG. */ 
    "Please open the drive\nnamed \"H5\" on PC\nto copy and save\nthe logs", 
    /* FACTORY RESET. */ 
    "Confirm reset?\nAll configuration data\nwill be deleted!",
    "Processing, please do not power off!",

    /* SELFTEST. */
    "Application",
    "Resource File",
    "Upgrade",
    "System files corrupted",
    "Detected issues that need update",
    "OK",
    "Failed",
    "Press Enter to continue"
};

/* 法语. */
const char * text_fr[] = { 
    /* MENU. */ 
    "MISE À JOUR", "JOURNAUX", "RÉINIT.", 
    /* OTA. */ 
    "En attente de mise à jour...", "Mise à jour en cours...", "Mise à jour du LOGO...", "Mise à jour du texte...", "Mise à jour des images...", "Échec de la mise à jour", "Mise à jour réussie", 
    /* LOG. */ 
    "Veuillez ouvrir le lecteur\nnommé \"H5\" sur PC\npour copier et\nenregistrer les journaux", 
    /* FACTORY RESET. */ 
    "Confirmer le reset ?\nToutes les données de\nconfiguration seront effacées !",
    "Traitement en cours,\nne pas éteindre !",

    /* SELFTEST. */
    "Application",
    "Fichier de ressources",
    "Mettre à jour",
    "Fichiers système endommagés",
    "Problèmes détectés, mise à jour requise",
    "OK",
    "Échec",
    "Appuyez sur Entrée pour continuer"
};

/* 西班牙语. */
const char * text_es[] = { 
    /* MENU. */ 
    "ACTUALIZACIÓN", "REGISTROS", "REINICIO", 
    /* OTA. */ 
    "Esperando actualización...", "Actualizando...", "Actualizando LOGO...", "Actualizando texto...", "Actualizando imágenes...", "Error de actualización", "Actualización exitosa", 
    /* LOG. */ 
    "Abra la unidad llamada\n\"H5\" en el PC\npara copiar y guardar\nlos registros", 
    /* FACTORY RESET. */ 
    "¿Confirmar reset?\n¡Se borrarán todos los\ndatos de configuración!",
    "Procesando,\n¡no apagar el equipo!",

    /* SELFTEST. */
    "Aplicación",
    "Archivo de recursos",
    "Actualizar",
    "Archivos del sistema dañados",
    "Se detectaron problemas que requieren actualización",
    "OK",
    "Error",
    "Pulse Intro para continuar"
};

const char * text_de[] = { 
    /* MENU. */ 
    "UPGRADE", "PROTOKOLLE", "ZURUECKSETZEN", 
    /* OTA. */ 
    "Warten auf Upgrade...", "Upgrade läuft...", "Upgrade LOGO...", "Upgrade Text...", "Upgrade Bilder...", "Upgrade fehlgeschlagen", "Upgrade erfolgreich", 
    /* LOG. */ 
    "Bitte das Laufwerk\n\"H5\" am PC öffnen,\num Protokolle zu kopieren\nund zu speichern", 
    /* FACTORY RESET. */ 
    "Reset\nbestätigen?\nAlle Konfigurationsdaten\nwerden gelöscht!",
    "Verarbeitung läuft,\nbitte nicht ausschalten!",

    /* SELFTEST. */
    "Anwendung",
    "Ressourcendatei",
    "Aktualisieren",
    "Systemdateien beschädigt",
    "Probleme erkannt, Update erforderlich",
    "OK",
    "Fehler",
    "Enter drücken zum Fortfahren"
};

const char * text_ru[] = { 
    /* MENU. */ 
    "ОБНОВЛЕНИЕ", "ЖУРНАЛЫ", "СБРОС", 
    /* OTA. */ 
    "Ожидание обновления...", "Обновление...", "Обновление LOGO...", "Обновление текста...", "Обновление изображений...", "Ошибка обновления", "Обновление успешно", 
    /* LOG. */ 
    "Откройте на ПК диск\nс именем \"H5\",\nчтобы скопировать\nи сохранить журналы", 
    /* FACTORY RESET. */ 
    "Подтвердить сброс?\nВсе данные конфигурации\nбудут удалены!",
    "Выполняется обработка,\nне выключайте питание!",

    /* SELFTEST. */
    "Приложение",
    "Файл ресурсов",
    "Обновить",
    "Системные файлы повреждены",
    "Обнаружены проблемы, требуется обновление",
    "OK",
    "Ошибка",
    "Нажмите Enter для продолжения"
};

const char * text_it[] = { 
    /* MENU. */ 
    "AGGIORNAMENTO", "REGISTRI", "RIPRISTINO", 
    /* OTA. */ 
    "In attesa di aggiornamento...", "Aggiornamento in corso...", "Aggiornamento LOGO...", "Aggiornamento testo...", "Aggiornamento immagini...", "Aggiornamento fallito", "Aggiornamento riuscito", 
    /* LOG. */ 
    "Aprire l'unità denominata\n\"H5\" sul PC\nper copiare e\nsalvare i registri", 
    /* FACTORY RESET. */ 
    "Confermare il reset?\nTutti i dati di\nconfigurazione verranno\neliminati!",
    "Elaborazione in corso,\nnon spegnere il dispositivo!",

    /* SELFTEST. */
    "Applicazione",
    "File risorse",
    "Aggiorna",
    "File di sistema danneggiati",
    "Rilevati problemi che richiedono un aggiornamento",
    "OK",
    "Errore",
    "Premere Invio per continuare"
};

const char * text_pt[] = { 
    /* MENU. */ 
    "ATUALIZAÇÃO", "REGISTROS", "REDEFINIR", 
    /* OTA. */ 
    "Aguardando atualização...", "Atualizando...", "Atualizando LOGO...", "Atualizando texto...", "Atualizando imagens...", "Falha na atualização", "Atualização bem-sucedida", 
    /* LOG. */ 
    "Abra a unidade chamada\n\"H5\" no PC\npara copiar e salvar\nos registros", 
    /* FACTORY RESET. */ 
    "Confirmar reset?\nTodos os dados de\nconfiguração serão apagados!",
    "Processando,\nnão desligue o aparelho!",

    /* SELFTEST. */
    "Aplicação",
    "Arquivo de recursos",
    "Atualizar",
    "Arquivos do sistema corrompidos",
    "Problemas detectados que precisam de atualização",
    "OK",
    "Falha",
    "Pressione Enter para continuar"
};

const char * text_pl[] = { 
    /* MENU. */ 
    "AKTUALIZACJA", "LOGI", "RESETUJ", 
    /* OTA. */ 
    "Oczekiwanie na aktualizację...", "Aktualizacja...", "Aktualizacja LOGO...", "Aktualizacja tekstu...", "Aktualizacja obrazów...", "Aktualizacja nieudana", "Aktualizacja powiodła się", 
    /* LOG. */ 
    "Otwórz na komputerze dysk\no nazwie \"H5\",\naby skopiować\ni zapisać logi", 
    /* FACTORY RESET. */ 
    "Potwierdzić reset?\nWszystkie dane\nkonfiguracyjne zostaną\nusunięte!",
    "Przetwarzanie,\nnie wyłączaj zasilania!",

    /* SELFTEST. */
    "Aplikacja",
    "Plik zasobów",
    "Aktualizuj",
    "Pliki systemowe uszkodzone",
    "Wykryto problemy wymagające aktualizacji",
    "OK",
    "Błąd",
    "Naciśnij Enter, aby kontynuować"
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
