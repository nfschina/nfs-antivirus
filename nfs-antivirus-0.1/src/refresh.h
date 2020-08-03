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
#define SCAN_CONTINUE 1
#define SCAN_OVER 0
#define SCAN_FINISH 2

#define TIME_NEED_ACCUMULATION   1 
#define TIME_NOT_NEED_ACCUMULATION 0

struct ScanInfo
{
    GtkLabel *scaningfilenamelabel;
    GtkLabel *scanobjlabel;
    GtkLabel *finddangerlabel;
    GtkLabel *scantimelabel;
    GtkLabel *scanfilecountlabel;
    GtkLabel *expectscantimelabel;
    GtkWidget *progress_bar;
    GtkWidget *start_pause_button;
    int scanType ;
    int scanTotalSize ;
    time_t startScanTime;
    GtkWidget *allScanTipLabel;
};

gboolean  refresh_scan_page(struct ScanInfo *scanInfo);
void set_lable_style(GtkLabel *label,char *cssInfo);
void refreshPageThread(struct ScanInfo *scanInfo);
