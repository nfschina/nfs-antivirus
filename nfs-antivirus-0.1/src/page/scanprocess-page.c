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
#include <gtk/gtk.h>
#include <locale.h>
#include <libintl.h>
#include <stdlib.h>
#include "../refresh.h"
#include "../scan.h"
#include "../result.h"
#include "../result_detail.h"
#include "../function.h"
#include "../writeresult.h"
extern char scan_starttime[30];
extern struct ScanInfo scanInfo;
gint refreshId;
int time_accumulation_status = TIME_NOT_NEED_ACCUMULATION;
extern int startorendflag;
extern GtkStatusIcon *trayIcon;
extern pthread_t refreshThreadId,scanThreadId,specifiedScanThreadId,allScanThreadId,selfScanThreadId;
extern GtkWindow *window;//主窗口指针
int cancel = 0 ;
extern scanTimeuse;
extern fileNum;
extern GtkWidget * table6;
extern GtkWidget *back_to_mainmenu_button;
//extern GtkWidget *feedback_label_button;
GtkWidget *scan_progress_value_label;
GtkWidget *scan_process_title_layout,*scan_process_title_image;
GtkWidget *button3;*quick_scan_button;
extern GtkWidget *feedback_label_button_white_bg,*feedback_label_button,*check_update_button,*check_update_button_white_bg;
#define _(STRING) gettext(STRING)
void leave_back_button(GtkWidget *widget,gpointer data){
	char image_dir[256];
        gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sback_menu.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *enter_restorebutton_image = gtk_image_new_from_file(image_dir);
       // gtk_button_set_image(widget,enter_restorebutton_image);
}

void enter_back_button(GtkWidget *widget,gpointer data)
{
	//gtk_button_set_image(widget,NULL);
        g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_back_button),NULL);
}
//退出快速扫描
void quitscan(GtkWidget *widget,gpointer data)
{
	printf("普通快速扫描写日志\n");
        writelog(QUIT_SCAN_LOG_TYPE);
        g_source_remove(refreshId);
        pthread_cancel(scanThreadId);
        waitScanThreadCacelFinish(scanThreadId);
	char avlstatus[256];
        snprintf(avlstatus,sizeof(avlstatus),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
        write_antivirus_status(avlstatus,"4");

}

//退出外界设备扫描
void quitSpecifiedScan(GtkWidget *widget,gpointer data)
{

	char defence_log[1024];
        char defence_log_type[1024];
	strcpy(defence_log_type,_("USB-dev-scan"));
        char defenc_log_result[1024];
        char tmp_scan_results[512];
        snprintf(tmp_scan_results,sizeof(tmp_scan_results),"%sscan_results.txt",ANTIVIRUS_TEMP_DIR);
        FILE *fp=fopen(tmp_scan_results,"rw");
        if(fp == NULL)
        {
		strcpy(defenc_log_result,_("scan quit no thread"));
                snprintf(defence_log,sizeof(defence_log),"%s,%s,%s,",scan_starttime,defence_log_type,defenc_log_result);
		write_defence_log(defence_log);
	        delete_log_30daysago("defence_log");

        }
	else{
        	gchar buf[1024];
        	while(fgets(buf,1023,fp) != NULL)
        	{
		printf("进入while主循环");
                if (buf==NULL)
                {
                        strcpy(defenc_log_result,_("scan quit no thread"));
			snprintf(defence_log,sizeof(defence_log),"%s,%s,%s,",scan_starttime,defence_log_type,defenc_log_result);
			write_defence_log(defence_log);
		}
                else
                {
                        char buf1[100],buf2[200],buf3[200],buf4[200],buf5[200];

			sscanf(buf, "%[^;];%[^;];%[^;];%[^;];%[^;]", buf1, buf2,buf3,buf4,buf5);

			//strcpy(defenc_log_result,p[1]);
			snprintf(defenc_log_result,sizeof(defenc_log_result),"%s %s",buf2,buf4);
                	snprintf(defence_log,sizeof(defence_log),"%s,%s,%s,",scan_starttime,defence_log_type,defenc_log_result);
			printf("开始写防御日志");
			write_defence_log(defence_log);

                	}
                }
	        delete_log_30daysago("defence_log");

		fclose(fp);
	}
	printf("准备停止线程\n");
        g_source_remove(refreshId);
        pthread_cancel(specifiedScanThreadId);
        waitScanThreadCacelFinish(specifiedScanThreadId);
	char avlstatus[256];
	printf("开始写状态");
        snprintf(avlstatus,sizeof(avlstatus),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
        write_antivirus_status(avlstatus,"4");

}

//退出全盘扫描
void quitallscan(GtkWidget *widget,gpointer data)
{
        writelog(QUIT_SCAN_LOG_TYPE);
        g_source_remove(refreshId);
        pthread_cancel(scanThreadId);
        waitScanThreadCacelFinish(scanThreadId);
	    char avlstatus[256];
        snprintf(avlstatus,sizeof(avlstatus),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
        write_antivirus_status(avlstatus,"4");
}

//退出自定义扫描
void quitselfscan(GtkWidget *widget,gpointer data)
{
        writelog(QUIT_SCAN_LOG_TYPE);
        g_source_remove(refreshId);
        pthread_cancel(scanThreadId);
        waitScanThreadCacelFinish(scanThreadId);
	    char avlstatus[256];
        snprintf(avlstatus,sizeof(avlstatus),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
        write_antivirus_status(avlstatus,"4");
        return ;
}

void stop_scaning_yes()
{
		startorendflag=0;
		char image_dir[256];
        	snprintf(image_dir,sizeof(image_dir),_("%sstop.png"),ANTIVIRUS_IMAGE_DIR);

                gtk_button_set_image(GTK_BUTTON(quick_scan_button),gtk_image_new_from_file(image_dir));
                if (scanInfo.scanType == FAST_SCAN)
                {
                        quitscan(button3,NULL);

                }
                else if (scanInfo.scanType == CUSTOM_SCAN)
                {
                        quitselfscan(button3,NULL);
                }
                else if (scanInfo.scanType == ALL_SCAN)
                {
                        quitallscan(button3,NULL);
                }
                else if (scanInfo.scanType == SPECIFIED_SCAN || scanInfo.scanType == FIREFOX_DOWNLOAD_SCAN)
                {
                        quitSpecifiedScan(button3,NULL);
                }
                cancel=1;
                how_switch_to_result_detail(cancel);

}
void stop_scaning_no()
{
	int ret,scanRet;

                time(&scanInfo.startScanTime);
                time_accumulation_status = TIME_NEED_ACCUMULATION;

                g_thread_new("refresh", refreshPageThread, &scanInfo);
		char image_dir[256];
        	snprintf(image_dir,sizeof(image_dir),_("%sstop.png"),ANTIVIRUS_IMAGE_DIR);
                gtk_button_set_image(GTK_BUTTON(quick_scan_button),gtk_image_new_from_file(image_dir));
                startorendflag = 0;
                thread_resume();

}
/*************dsj-0807-停止扫描回调函数*****************/
gint stop_scaning(GtkWidget *widget,gpointer data)
{
        time_t endScanTime;
	char image_dir[256];
        snprintf(image_dir,sizeof(image_dir),_("%sNFSantivirus.png"),ANTIVIRUS_IMAGE_DIR);
	gtk_status_icon_set_from_file(trayIcon,image_dir);
        g_source_remove(refreshId);
        thread_pause();
        time(&endScanTime);
        scanTimeuse = scanTimeuse +  difftime(endScanTime,scanInfo.startScanTime);
        show_warn_info(_("Are you sure to stop scaning?"),2);
}


//暂停或继续按钮处理函数
void startorend(GtkWidget *widget,gpointer data)
{
        time_t endScanTime;
	char image_dir[256];
        if (startorendflag == 0)
        {
        	snprintf(image_dir,sizeof(image_dir),_("%sNFSantivirus.png"),ANTIVIRUS_IMAGE_DIR);
	 	gtk_status_icon_set_from_file(trayIcon,image_dir);

		memset(image_dir,0,sizeof(image_dir));
        	snprintf(image_dir,sizeof(image_dir),_("%scontinue.png"),ANTIVIRUS_IMAGE_DIR);
                gtk_button_set_image(GTK_BUTTON(widget),gtk_image_new_from_file(image_dir));
                g_source_remove(refreshId);
                time(&endScanTime);
                scanTimeuse = scanTimeuse +  difftime(endScanTime,scanInfo.startScanTime);
                thread_pause();
                startorendflag = 1;

        }
        else if (startorendflag == 1)
        {
                int ret,scanRet;
                time(&scanInfo.startScanTime);
                time_accumulation_status = TIME_NEED_ACCUMULATION;
                g_thread_new("refresh", refreshPageThread, &scanInfo);
                thread_resume();
		memset(image_dir,0,sizeof(image_dir));
	        snprintf(image_dir,sizeof(image_dir),_("%sstop.png"),ANTIVIRUS_IMAGE_DIR);
                gtk_button_set_image(GTK_BUTTON(widget),gtk_image_new_from_file(image_dir));
                startorendflag = 0;
        }

}

//创建扫描过程界面
void create_scan_process_page()
{
        GtkWidget *dialog,*table,*progress_bar,*fix1,*progresshbox,*scan_vbox,*vbox1;
        GtkLabel *scantimelabel,*scanfilecountlabel,*scanobjlabel,*finddangerlabel,*expectscantimelabel,*scaningfilenamelabel;
	char image_dir[256];
	char scan_process_page_css[256];
        snprintf(image_dir,sizeof(image_dir),_("%sscan_process_page.css"),ANTIVIRUS_CSS_DIR);
//	GFile *file=g_file_new_for_path("./scan_process_page.css");
        GFile *file=g_file_new_for_path(image_dir);
    	GdkScreen *screen;
    	screen = gdk_screen_get_default();
    	GtkCssProvider *cssProvider;
    	cssProvider = gtk_css_provider_new();
    	gtk_css_provider_load_from_file(cssProvider, file, NULL);
        table6 = gtk_grid_new();
        getsystime();
        scan_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_widget_show(GTK_WIDGET(scan_vbox));
        snprintf(image_dir,sizeof(image_dir),_("%squickscan_process_title.png"),ANTIVIRUS_IMAGE_DIR);
        scan_process_title_image = gtk_image_new_from_file(image_dir);
        gtk_widget_show(GTK_WIDGET(scan_process_title_image));
        scan_process_title_layout = gtk_layout_new(NULL, NULL);
        gtk_widget_show(GTK_WIDGET(scan_process_title_layout));
        gtk_widget_set_size_request(scan_process_title_layout, 800, 106);
        gtk_layout_put(GTK_LAYOUT(scan_process_title_layout), scan_process_title_image, 0, 0);
        gtk_container_add(GTK_CONTAINER(scan_vbox),scan_process_title_layout);
        back_to_mainmenu_button=gtk_button_new();
          gtk_widget_set_size_request(back_to_mainmenu_button,69,37);
        gtk_widget_set_name(back_to_mainmenu_button,"back_to_mainmenu_button");

	gtk_widget_show(GTK_WIDGET(back_to_mainmenu_button));
        gtk_button_set_relief((GtkButton *)back_to_mainmenu_button,GTK_RELIEF_NONE);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sback_menu.png"),ANTIVIRUS_IMAGE_DIR);
       // gtk_button_set_image(GTK_BUTTON(back_to_mainmenu_button),gtk_image_new_from_file(image_dir));

        gtk_layout_put(GTK_LAYOUT(scan_process_title_layout), (GtkWidget *)back_to_mainmenu_button, 40, 27);
        g_signal_connect(G_OBJECT(back_to_mainmenu_button),"clicked",G_CALLBACK(switch_to_mainmenu),NULL);
        //g_signal_connect(G_OBJECT(back_to_mainmenu_button),"enter",G_CALLBACK(enter_back_button),NULL);

        GtkWidget *fix_progress_button,*progress_hbox,*scaninfo_layout,*scaninfo_backgroup_image;
        scaninfo_layout = gtk_layout_new(NULL, NULL);
        gtk_widget_set_size_request(scaninfo_layout, 769, 411);
        gtk_widget_show(scaninfo_layout);
	gtk_box_pack_start(GTK_BOX(scan_vbox), scaninfo_layout, TRUE, TRUE, 5);
	
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sscanprogress_background.png"),ANTIVIRUS_IMAGE_DIR);

        scaninfo_backgroup_image = gtk_image_new_from_file(image_dir);
        gtk_widget_show(scaninfo_backgroup_image);
        gtk_layout_put(GTK_LAYOUT(scaninfo_layout),scaninfo_backgroup_image, 17, 5);

	GtkWidget * allScanTipLabel;

	allScanTipLabel = (GtkLabel *)gtk_label_new(_("For a while, please be patient."));
        gtk_widget_show(allScanTipLabel);
        change_font_style((GtkLabel *)allScanTipLabel,"<span foreground=\"#3dbd8f\" font_desc='14'>%s</span>");
        gtk_layout_put(GTK_LAYOUT(scaninfo_layout),allScanTipLabel, 220, 30);



        scaningfilenamelabel = (GtkLabel *)gtk_label_new("改变这个label");
        gtk_widget_show(scaningfilenamelabel);
        change_font_style((GtkLabel *)scaningfilenamelabel,"<span foreground=\"#4c4554\" font_desc='14'>%s</span>");
        gtk_layout_put(GTK_LAYOUT(scaninfo_layout),scaningfilenamelabel, 80, 87);


        progress_bar=gtk_progress_bar_new();
        gtk_widget_show(progress_bar);
        gtk_widget_set_size_request(progress_bar, 585, 22);
        gtk_layout_put(GTK_LAYOUT(scaninfo_layout),progress_bar, 74, 135);


        scan_progress_value_label = (GtkLabel *)gtk_label_new("0%");
        gtk_widget_show(scan_progress_value_label);
        change_font_style((GtkLabel *)scan_progress_value_label,"<span foreground=\"#3dbd8f\" font_desc='16'>%s</span>");
        gtk_layout_put(GTK_LAYOUT(scaninfo_layout),scan_progress_value_label, 690, 135);



        quick_scan_button = gtk_button_new();
        gtk_widget_show(quick_scan_button);
        gtk_button_set_relief((GtkButton *)quick_scan_button,GTK_RELIEF_NONE);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sstop.png"),ANTIVIRUS_IMAGE_DIR);
        gtk_button_set_image(GTK_BUTTON(quick_scan_button),gtk_image_new_from_file(image_dir));
       	gtk_layout_put(GTK_LAYOUT(scaninfo_layout),quick_scan_button, 445, 254);
        g_signal_connect(G_OBJECT(quick_scan_button),"clicked",G_CALLBACK(startorend),(gpointer)table6);

        scanobjlabel = (GtkLabel *)gtk_label_new(_("scan-object"));
        gtk_widget_show(scanobjlabel);
        change_font_style((GtkLabel *)scanobjlabel,"<span foreground=\"#4c4554\" font_desc='12'>%s</span>");
        gtk_layout_put(GTK_LAYOUT(scaninfo_layout),scanobjlabel, 74, 180);

        finddangerlabel = (GtkLabel *)gtk_label_new(_("that-threat"));
        gtk_widget_show(finddangerlabel);
        change_font_style((GtkLabel *)finddangerlabel,"<span foreground=\"#4c4554\" font_desc='12'>%s</span>");
        gtk_layout_put(GTK_LAYOUT(scaninfo_layout),finddangerlabel, 220, 180);

        scantimelabel = (GtkLabel *)gtk_label_new(_("scan-time"));
        gtk_widget_show(scantimelabel);
        change_font_style((GtkLabel *)scantimelabel,"<span foreground=\"#4c4554\" font_desc='12'>%s</span>");
        gtk_layout_put(GTK_LAYOUT(scaninfo_layout),scantimelabel, 340, 180);

        if(scanInfo.scanType != ALL_SCAN)
        {
                expectscantimelabel = (GtkLabel *)gtk_label_new(_("expect-time"));
                gtk_widget_show(expectscantimelabel);
                change_font_style((GtkLabel *)expectscantimelabel,"<span foreground=\"#4c4554\" font_desc='12'>%s</span>");
                gtk_layout_put(GTK_LAYOUT(scaninfo_layout),expectscantimelabel, 520, 180);
        }

        button3 = gtk_button_new ();
        gtk_widget_show(button3);
        gtk_button_set_relief((GtkButton *)button3,GTK_RELIEF_NONE);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sstop1.png"),ANTIVIRUS_IMAGE_DIR);
        gtk_button_set_image(GTK_BUTTON(button3),gtk_image_new_from_file(image_dir));
        gtk_layout_put(GTK_LAYOUT(scaninfo_layout),button3, 550, 254);
        g_signal_connect(G_OBJECT(button3),"clicked",G_CALLBACK(stop_scaning),NULL);

        scanInfo.scaningfilenamelabel = scaningfilenamelabel;
        scanInfo.scanobjlabel = scanobjlabel;
        scanInfo.finddangerlabel = finddangerlabel;
        scanInfo.scantimelabel = scantimelabel;
        scanInfo.scanfilecountlabel = scanfilecountlabel;
        scanInfo.expectscantimelabel = expectscantimelabel;
        scanInfo.progress_bar = progress_bar;
        scanInfo.start_pause_button = quick_scan_button;
	scanInfo.allScanTipLabel = allScanTipLabel;


        gtk_grid_attach((GtkGrid *)table6,scan_vbox,0,0,1,1);
gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);

}

