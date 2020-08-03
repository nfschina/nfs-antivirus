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
#ifndef _UPGRADE_FUNCTION_H
#define _UPGRADE_FUNCTION_H
struct gradeInfo
{
    GtkLabel *isnewlabel;
    GtkWidget *update_status_image;
    GtkWidget *update_dialog;
    GtkWidget *update_fix;
};
void save_avl_lib_ftpaddr(char *update_ip);
void avlUpgradeLibRefresh(struct gradeInfo *grade_info);
void avlUpgradeLibRun();
void recover_lib(char * oldpath);
void write_status_infile(char *filename,char *szStatus);
void write_upgrade_log(char *avl_status_file,char *szAvlStatus);
#endif
