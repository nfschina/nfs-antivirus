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
#ifndef LOG_PAGE_H
#define LOG_PAGE_H
void select_all_log();
char * get_combo_text();
void logwindow(GtkWidget *widget,gpointer data);
GtkTreeModel *createlogModel(char *filepath);
void free_log_list_memory ();
void update_log_page(GtkWidget *content_box);
void enter_logbutton(GtkWidget *widget,gpointer *data);
#endif
