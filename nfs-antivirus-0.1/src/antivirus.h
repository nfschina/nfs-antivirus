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
#ifndef ANTIVIRUS_H
#define ANTIVIRUS_H
#include <gtk/gtk.h>
extern allScanExpectTime;
void custom_scan_type(GtkWidget *widget,gpointer data);
void change_font_style(GtkLabel *label,gchar *style);
int  get_scanType();
gint show_warn_info(char *warnMessage,int dailog_type);
void quit_init_engine();
void avlInitThread();
void waitInitThread(GtkLabel *attentionlabel);
#endif
