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
#ifndef SETTING_PAGE_H
#define SETTING_PAGE_H
#define SETTING_PATH "/usr/share/antivirus/config/setting.txt"
#define ISOLATION_FILE_PATH "/usr/share/antivirus/temp/isolation_temp.txt"
GtkWidget *notebook;
GtkWidget *timing_anti_button,*anti_scan_combo,*month_anti_button,*month_anti_day_combo,*month_anti_hour_combo,*month_anti_min_combo,*week_anit_button,*week_anit_week_combo,*week_anit_hour_combo,*week_anit_min_combo,*day_anit_button,*day_anti_hour_combo,*day_anit_min_combo,*day_anit_sec_combo;//定时杀毒
GtkWidget *manual_deal_button,*intelligence_deal_button,*isolation_region_button;
GtkWidget *boot_start_button,*aotu_usb_button,*download_button;//常规设置
GtkComboBoxText *Combo_Box_day_hour,*Combo_Box_day_sec,*Combo_Box_day_min,*update_iptext,*timingbutton,*periodbuton;
GtkWidget *month_update_button,*week_update_button,*day_update_button;
GtkComboBoxText *Combo_Box_week_week,*Combo_Box_week_hour,*Combo_Box_week_min,*Combo_Box_month_day,*Combo_Box_month_hour,*Combo_Box_month_min;
GtkWidget *timingtext,*periodtext,*updateporgress,*updatelabel1,*savetimebutton,*higtbutton,*comprehensivebutton;
GtkWidget *box,*while_list_content_box;
GtkWidget *white_list_sw,*white_list_list;
GtkCellRenderer *white_list_render,*white_list_render_tog;
GtkTreeSelection *white_selection;
GtkTreeViewColumn *white_list_column,*white_list_column_tog;//白名单
GtkWidget *select_all_box,*select_all_button;
GtkListStore *store;
GtkWidget *devide_file_list_sw,*devide_file_list_list,*select_all_devide_file_button,*devide_file_box;
GtkCellRenderer *devide_file_list_render_tog,*devide_file_list_render;
GtkTreeViewColumn *devide_file_list_column,*devide_file_list_column_tog;
GtkTreeSelection *devide_file_selection;
enum{
DEVIDE_FILE_LIST_TOG_COL,
DEVIDE_FILE_LIST_NAME_COL,
DEVIDE_FILE_LIST_TIME_COL
};
void common_settings(GtkWidget *content_box);
void anti_scan_settings(GtkWidget *content_box);
void white_list_settings(GtkWidget *content_box);
void update_settings(GtkWidget *content_box);
void scan_style_settings(GtkWidget *content_box);
void start_isolation_region(GtkWidget *widget,gpointer data);
void choose_week_anti(GtkWidget *widget,gpointer data);
void choose_anti_month(GtkWidget *widget,gpointer data);
void choose_anti_day(GtkWidget *widget,gpointer data);
void start_timing_anti(GtkWidget *widget,gpointer data);
void choose_manual(GtkWidget *widget,gpointer data);
void choose_month(GtkWidget *widget,gpointer data);
void choose_week(GtkWidget *widget,gpointer data);
void choose_day(GtkWidget *widget,gpointer data);
void choose_auto(GtkWidget *widget,gpointer data);
void select_toggled (GtkCellRendererToggle *renderer,gchar *path,GtkTreeView *treeview);
void remove_white_list(GtkWidget *widget,gpointer data);
void add_file(GtkWidget *widget,gpointer data);
void add_path(GtkWidget *widget,gpointer data);
void select_all();
GtkTreeModel * create_white_list_model();
int filechooser3(GtkWidget *window,GtkWidget *content_box);
int filechooser2(GtkWidget *window,GtkWidget *content_box);
void look_devide_file_settings(GtkWidget *content_box);
#endif
