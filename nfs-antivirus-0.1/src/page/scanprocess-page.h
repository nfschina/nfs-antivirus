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
#ifndef SCANPROCESS_PAGE_H
#define SCANPROCESS_PAGE_H
//创建扫描过程界面
void create_scan_process_page();
void stop_scaning_no();
void stop_scaning_yes();
void leave_back_button(GtkWidget *widget,gpointer data);
void enter_back_button(GtkWidget *widget,gpointer data);
#endif
