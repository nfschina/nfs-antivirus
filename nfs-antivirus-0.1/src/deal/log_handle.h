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
#ifndef LOG_DEAL_H
#define LOG_DEAL_H
GtkWidget *swlog;
GtkWidget *listlog,*select_all_log_button;
GtkListStore *store_log;
GtkCellRenderer *rendererlog;
GtkTreeViewColumn *columnlog;
GtkWidget *removelog,*exportlog;
GtkTreeSelection *selectionlog;
GtkWidget *log_combo;
#define SCAN_LOG_TYPE     1
#define UPGRADE_LOG_TYPE  2
#define DEFENSE_LOG_TYPE  3
enum{
LOGTOG_COL,
DATELOG_COL,
TYPELOG_COL,
RESULTLOG_COL
};
int n;
char log_file_path[50];
void remove_scanlog_item(GtkWidget * widget, gpointer *data);
void remove_defenselog_item(GtkWidget * widget, gpointer *data);
void remove_updatelog_item(GtkWidget * widget, gpointer *data);
void leave_removelog(GtkWidget *widget,gpointer data);
void enter_removelog(GtkWidget *widget,gpointer data);
void leave_exportlog(GtkWidget *widget,gpointer data);
void enter_exportlog(GtkWidget *widget,gpointer data);
void import_scan_log(GtkWidget *widget,gpointer *data);
void import_defense_log(GtkWidget *widget,gpointer *data);
void import_update_log(GtkWidget *widget,gpointer *data);
void import_log_func(int type,char* filename);
#endif
