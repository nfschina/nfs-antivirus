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
#define MANUAL_DEAL 1
#define AUTO_DEAL 2
#define ISOLATION_UNTOG 0
#define ISOLATION_TOG 1
enum{
 TOG_COL,
 DATE_COL,
 NAME_COL,
 THREATNUM_COL,
 THREATNAME_COL
};
int result_count;
void result(int auto_handle);
GtkTreeModel* createModel();
void free_list_memory ();
