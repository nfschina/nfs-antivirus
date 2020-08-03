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
#ifndef UPGRADE_PAGE_H
#define UPGRADE_PAGE_H
//升级界面
GtkWidget *updating_box;//升级中主界面
GtkWidget *update_result_box;//升级结果主界面
GtkWidget *update_net_botton;//升级按钮
GtkWidget *updating_net_check_label;//网络检测
GtkWidget *updating_net_check_fix;//网络检测fix
GtkWidget *updating_net_pass_label;//网络检测通过
GtkWidget *updating_net_detail_label;//网络检测结果
GtkWidget *clamav_button;//clamav本版检测按钮
GtkWidget *clamav_version_check;//clamav版本检测
GtkWidget *clamav_version_pass;//clamav版本检测通过
GtkWidget *clamav_version_fix;//camav版本检测结果fix
GtkWidget *clamav_now_version;//clamav目前版本
GtkWidget *clamav_update_success;//clamav升级成功
GtkWidget *clamav_thread_button;//clamav病毒库升级按钮
GtkWidget *updating_clamav_thread_label;//clamav病毒库升级
GtkWidget *updating_clamav_thread_pass_label;//clamav病毒库升级通过
GtkWidget *update_clamav_thread_fix;//clamav病毒库结果fix
GtkWidget *clamav_thread_now_version;//clamav病毒库现在版本
GtkWidget *clamav_thread_update_success_label;//clamav病毒库升级成功

GtkWidget *update_fail_success_image;
GtkWidget *update_net_result_label;
GtkWidget *update_pro_result_label;
int clamav_now_version_label_status;
int clamav_update_success_lable_status;
int clamav_thread_now_version_label_status;
int clamav_thread_update_success_label_status;
void updatewindow(GtkWidget *widget,gpointer data);
//退出升级函数
gboolean quitupgrade(GtkWidget *widget,gpointer data);
#endif
