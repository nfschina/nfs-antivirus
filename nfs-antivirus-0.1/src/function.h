/**
* Copyright (c) 2016-2018 CPU and Fundamental Software Research Center, CAS
*
* This software is published by the license of CPU-OS License, you can use and
* distibuted this software under this License. See CPU-OS Liense for more detail.
*
* You shoud have receive a copy of CPU-OS License. If not, please contact us 
* by email <support_os@cpu-os.ac.cn>
*
**/
#ifndef _FUNCTION_H
#define _FUNCTION_H
#define ANTIVIRUS_IMAGE_DIR     "/usr/share/antivirus/images/"
#define ANTIVIRUS_CSS_DIR       "/usr/share/antivirus/css/"
#define ANTIVIRUS_BIN_DIR       "/usr/share/antivirus/bin/"
#define ANTIVIRUS_CONF_DIR      "/usr/share/antivirus/config/"
#define ANTIVIRUS_TEMP_DIR      "/tmp/"
#define ANTIVIRUS_LOG_DIR       "/usr/share/antivirus/log/"
#define ANTIVIRUS_DESKTOP_DIR   "/usr/share/antivirus/desktop/"
#define ANTIVIRUS_DEFAULT_ISOLATION_DIR "/usr/share/antivirus/isolation/"
#define ANTIVIRUS_BIN_WHITE_DIR         "/usr/share/antivirus/bin/white_list/"
#define ANTIVIRUS_BIN_AUTO_HANDLE_DIR   "/usr/share/antivirus/bin/auto_handle/"
#define ANTIVIRUS_BIN_AUTO_DEFENSE_DIR  "/usr/share/antivirus/bin/auto_defense/"
#define ANTIVIRUS_BIN_UPGRADE_DIR       "/usr/share/antivirus/bin/upgrade/"
#define ANTIVIRUS_BIN_TIMEANTIVIRUS_DIR "/usr/share/antivirus/bin/timeantivirus/"
#define ANTIVIRUS_BIN_DETECT_USB_DIR    "/usr/share/antivirus/bin/detect_usb"
#define ANTIVIRUS_REAL_PROTECT_DIR   "/usr/share/antivirus/real_protect/"

#define ENGINE_NOW_VER          "1.0.16"
#define ENGINE_HIGH_VER         "1.0.16"
#define ENGINE_LIB_NOW_VER      "15611"
#define ENGINE_LIB_HIGH_VER     "15611"
#define ENGINE_MAIN_PAGE_VER    "15611"


int delete_white_list();
void set_antivirus_setting_item(char *Lable,char *value,char *conf_path);
int get_antivirus_setting_item_inttype(char *Lable,char *conf_path);
int get_antivirus_setting_item_stringtype(char *Lable,char *value ,char *conf_path);
void get_file_size(char *path,char *file_space);
void get_file_modify_time(char *path,char *file_modify_time);
void strcat_white_list(char *white_list_info,char *style,char *file_name,char *file_space,char *file_time);
void strcat_more_string(char *dest_string,int length,char *str1,char *str2, char *str3);
void is_file(char *path,char *style);
int check_ip_valid(const char* ip);
void notify_pop(char *pop_title,char *pop_description,char *image_path);
int  clamav_init();
int clamav_scan(char *filename);
void  getsystime();
void cmd_shell_result(const char* command,char * result,int length);
int is_devide_dir(char *devide_file);
int lastIndexOf(char *str1,char *str2);
#endif
