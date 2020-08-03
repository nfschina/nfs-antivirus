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
#ifndef ABOUT_PAGE_H
#define ABOUT_PAGE_H
//创建关于界面
void create_about_page();
void aboutwindow(GtkWidget *widget,gpointer data);//显示关于界面，隐藏其它界面
void enter_aboutbutton(GtkWidget *widget,gpointer *data);
#endif
