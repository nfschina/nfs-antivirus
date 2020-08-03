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
#include<gtk/gtk.h>
#include <time.h>
#include <string.h>
#include <libintl.h>
#include "refresh.h"
#include "result.h"
#include "auto_handle.h"
#include "sharememory.h"
#include "resultcallbackFunc.h"
#include "result_detail.h"
#include "./page/setting-page.h"
#include "function.h"
#include "antivirus.h"
#include "scan.h"
#define _(STRING) gettext(STRING)
extern char scan_starttime[30];
gint refreshId;
int scanTimeuse = 0;
int flag = 0;
extern fileNum;
extern startorendflag;
extern time_accumulation_status;
extern GtkWidget *scan_progress_value_label;
extern GtkStatusIcon *trayIcon;
extern struct scanResults g_scanResultInfo;
extern char g_current_user[256]; //当前启动杀毒软件用户

void computeTotaltime(struct ScanInfo *scanInfo, char *useTime, int length)
{
    time_t endScanTime;
    int num, hour, min, sec;

    if (scanTimeuse > 0)
	{
	    num = scanTimeuse;
	    hour = num / 3600;
	    min = num % 3600 / 60;
	    sec = num % 60;
	    snprintf(useTime, length, "%02d:%02d:%02d", hour, min, sec);
	}
    scanTimeuse = 0;

}

int get_scan_status()
{

    return SCAN_CONTINUE;
}

//创建刷新线程
void refreshPageThread(struct ScanInfo *scanInfo)
{
    int scanTotalSize = 0;
    refreshId = g_timeout_add(1 * 1000, refresh_scan_page, scanInfo);
    sleep(1);
    return;
}

/******* ********************************* 

******函数功能：
      修改标签样式
******
        
******函数参数：
******@param scanType 扫描类型:
        FAST_SCAN 快速扫描、ALL_SCAN 全盘扫描

************************** ************/
void set_lable_style(GtkLabel * label, char *cssInfo)
{
    gchar labelStyleInfo[100];
    const gchar *labelTitel = gtk_label_get_text(label);
    snprintf(labelStyleInfo, sizeof(labelStyleInfo), cssInfo,
	     (char *)labelTitel);
    gtk_label_set_markup(GTK_LABEL(label), labelStyleInfo);
}

gboolean
timeout_callback(GtkWidget * progress_bar, float filesize, float pathsSize)
{
    GString *text;
    float increase_value;
    float value;
    if (pathsSize > 0 && filesize > 0)
	{
	    increase_value = filesize / pathsSize;

	    if (increase_value >= 0.000001)
		{
		    value = increase_value;
		}
	    else
		{
		    return TRUE;
		}
	}
    else
	{
	    return TRUE;
	}

    if (value >= 1.0)
	{
	    if (filesize != 1 && pathsSize != 1)
		{
		    value = 0.99;
		}
	    else
		{
		    value = 1.0;
		}
	}

    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), value);
    text =
	g_string_new(gtk_progress_bar_get_text(GTK_PROGRESS_BAR(progress_bar)));
    g_string_sprintf(text, "%d %%", (int)(value * 100));
    gtk_label_set_text(scan_progress_value_label, text->str);
    change_font_style((GtkLabel *) scan_progress_value_label,
		      "<span foreground=\"#3dbd8f\" font_desc='16'>%s</span>");
    return TRUE;
}

GTimer *useTimer;
int oldscanTotalNum = 1;
int again_flag = 0;
gboolean refresh_scan_page(struct ScanInfo *scanInfo)
{
    char image_log[512];
    char scanObj[30];
    char finddangerlabel[30];
    char scantimelabel[30];
    char fileName[30] = "test.txt";
    int sanThreatNum = 0;
    int scanTotalNum = 0;
    char useTime[45] = "01:01:01";
    char expectscantimelabel[30];
    char expectTime[15] = "02:20:30";
    char scanFileName[2048];
    char semkeyName[30] = "scanStatusMutexKey";
    char shmkeyName[30] = "scanResultInfoKey";
    struct scanResults scanResultInfo;
    int result = 0;
    int timeuse;
    int num, hour, min, sec;
    int scanStatus = 0;
    time_t endScanTime;

	char set_file_path[1024];
	snprintf(set_file_path,sizeof(set_file_path),"%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);

    if (again_flag == 1)
	{
	    int cancel = 0;
	    how_switch_to_result_detail(cancel);
	    again_flag = 0;
	    return 0;
	}
    //获取执行时间
    time(&endScanTime);
    timeuse = difftime(endScanTime, scanInfo->startScanTime);
    if (time_accumulation_status == TIME_NEED_ACCUMULATION)
	{
	    timeuse = timeuse + scanTimeuse;
	}
    if (timeuse > 0)
	{
	    num = timeuse;
	    hour = num / 3600;
	    min = num % 3600 / 60;
	    sec = num % 60;
	    sprintf(useTime, " %02d: %02d: %02d", hour, min, sec);
	    //更新扫描用时
	    sprintf(scantimelabel, _("scan time:%s"), useTime);
	    gtk_label_set_text(scanInfo->scantimelabel, scantimelabel);
	}
    change_font_style((GtkLabel *) scanInfo->scantimelabel,
		      "<span foreground=\"#4c4554\" font_desc='12'>%s</span>");

    //读取共享内存中的当前的扫描信息
    //  result = read_share_memory(semkeyName ,shmkeyName,&scanResultInfo);
    //  if (result != 1)
    //  {
    //        return 1;
    //   }

    scanStatus = g_scanResultInfo.scanStatus;
    if (scanStatus == SCAN_FINISH)
	{
	    scanTimeuse = timeuse;
	    time_accumulation_status = TIME_NOT_NEED_ACCUMULATION;
	    //更新正在扫描的文件
	    if (get_antivirus_setting_item_inttype
		("anti-deal-style", set_file_path) == AUTO_DEAL
		&& is_has_threat())
		{
		    strcpy(scanFileName,
			   _("is ready for dealing the threat ..."));
		}
	    else
		{
		    strcpy(scanFileName, _("scan complete"));
		}

	    char subscaningfilenamelabel[200];
	    int scanfinish_len = strlen(scanFileName);
	    strncpy(subscaningfilenamelabel, scanFileName, scanfinish_len);
	    subscaningfilenamelabel[scanfinish_len] = '\0';
	    gtk_label_set_text(scanInfo->scaningfilenamelabel,
			       subscaningfilenamelabel);
	    change_font_style((GtkLabel *) scanInfo->scaningfilenamelabel,
			      "<span foreground=\"#4c4554\" font_desc='14'>%s</span>");
	    timeout_callback(scanInfo->progress_bar, 1, 1);
	    startorendflag = 2;
	    if (get_scanType() == SPECIFIED_SCAN
		|| get_scanType() == FIREFOX_DOWNLOAD_SCAN)
		{
		    //printf("火狐下载开始写日志\n");
		    char defence_log[1024];
		    //char *defence_log_type=_("donwload scan");
		    char defence_log_type[1024];
		    if (get_scanType() == SPECIFIED_SCAN)
			{
			    strcpy(defence_log_type, _("USB-dev-scan"));
			}
		    else
			{
			    strcpy(defence_log_type, _("donwload scan"));
			}

		    //char *defenc_log_result=_("open");
		    char defenc_log_result[1024];
		    getsystime();
		    char c = 0;
		    int line = 0;
		    struct ScanResults *pcurrent, *ptempNode;
		    char tmp_scan_results[512];
		    snprintf(tmp_scan_results, sizeof(tmp_scan_results),
			     "%sscan_results.txt", ANTIVIRUS_TEMP_DIR);
		    FILE *fp = fopen(tmp_scan_results, "rw");
		    if (fp == NULL)
			{
			    strcpy(defenc_log_result, _("complete no thread"));
			    snprintf(defence_log, sizeof(defence_log),
				     "%s,%s,%s,", scan_starttime,
				     defence_log_type, defenc_log_result);
			    write_defence_log(defence_log);
			    delete_log_30daysago("defence_log");

			}
		    else
			{
			    gchar buf[1024];
			    gint i = 0;
			    while (fgets(buf, 1023, fp) != NULL)
				{
				    int in = 0;
				    char *p[20];
				    if (buf == NULL)
					{
					    strcmp(defenc_log_result,
						   _("complete no thread"));
					    snprintf(defence_log,
						     sizeof(defence_log),
						     "%s,%s,%s,",
						     scan_starttime,
						     defence_log_type,
						     defenc_log_result);
					    write_defence_log(defence_log);
					}
				    else
					{
					    char *buffer = buf;
					    while ((p[in] =
						    strtok(buffer,
							   ";")) != NULL)
						{
						    in++;
						    buffer = NULL;
						}
					    //strcpy(defenc_log_result,p[1]);
					    snprintf(defenc_log_result,
						     sizeof
						     (defenc_log_result),
						     "%s %s", p[1], p[3]);
					    snprintf(defence_log,
						     sizeof(defence_log),
						     "%s,%s,%s,",
						     scan_starttime,
						     defence_log_type,
						     defenc_log_result);
					    write_defence_log(defence_log);

					}
				}
			    delete_log_30daysago("defence_log");

			    fclose(fp);
			}
		}
	    else
		{
		    writelog();
		}
	    again_flag = 1;
	    return SCAN_CONTINUE;

	}

    scanTotalNum = g_scanResultInfo.scanObjNum;

    //更新扫描对象个数
    sprintf(scanObj, _("scan object:%d"), scanTotalNum);
    gtk_label_set_text(scanInfo->scanobjlabel, scanObj);
    change_font_style((GtkLabel *) scanInfo->scanobjlabel,
		      "<span foreground=\"#4c4554\" font_desc='12'>%s</span>");

    //更新发现威胁数目
    sprintf(finddangerlabel, _("that threat:%d"),
	    g_scanResultInfo.scanThreatNum);
    gtk_label_set_text(scanInfo->finddangerlabel, finddangerlabel);
    change_font_style((GtkLabel *) scanInfo->finddangerlabel,
		      "<span foreground=\"#4c4554\"  font_desc='12'>%s</span>");

    //更新扫描预计时间
    sprintf(expectscantimelabel, _("expect time:%s"),
	    g_scanResultInfo.scanExpectTime);
    gtk_label_set_text(scanInfo->expectscantimelabel, expectscantimelabel);
    change_font_style((GtkLabel *) scanInfo->expectscantimelabel,
		      "<span foreground=\"#4c4554\" font_desc='12'>%s</span>");

    //更新正在扫描的文件
    sprintf(scanFileName, _("scaning file:%s"), g_scanResultInfo.filename);
    char subscaningfilenamelabel1[100];
    strncpy(subscaningfilenamelabel1, scanFileName, 60);

    //更新状态栏图表
    if (flag % 2)
	{
	    snprintf(image_log, sizeof(image_log), _("%slog.png"),
		     ANTIVIRUS_IMAGE_DIR);
	    gtk_status_icon_set_from_file(trayIcon, image_log);
	}
    else
	{
	    memset(image_log, 0, sizeof(image_log));
	    snprintf(image_log, sizeof(image_log), _("%sscaning-usb.png"),
		     ANTIVIRUS_IMAGE_DIR);
	    gtk_status_icon_set_from_file(trayIcon, image_log);
	}
    flag++;

    int num_hanzi = 0;
    int i = 0;
    while (i < 60)
	{
	    if (scanFileName[i] & 0x80)
		{
		    num_hanzi++;
		}
	    i++;
	}
    if (num_hanzi % 3 == 1)
	{
	    subscaningfilenamelabel1[60] = scanFileName[60];
	    subscaningfilenamelabel1[61] = scanFileName[61];
	    subscaningfilenamelabel1[62] = '.';
	    subscaningfilenamelabel1[63] = '.';
	    subscaningfilenamelabel1[64] = '.';
	    subscaningfilenamelabel1[65] = '\0';
	}
    else if (num_hanzi % 3 == 2)
	{
	    subscaningfilenamelabel1[60] = scanFileName[60];
	    subscaningfilenamelabel1[61] = '.';
	    subscaningfilenamelabel1[62] = '.';
	    subscaningfilenamelabel1[63] = '.';
	    subscaningfilenamelabel1[64] = '\0';
	}
    else
	{
	    subscaningfilenamelabel1[60] = '.';
	    subscaningfilenamelabel1[61] = '.';
	    subscaningfilenamelabel1[62] = '.';
	    subscaningfilenamelabel1[63] = '\0';
	}
    gtk_label_set_text(scanInfo->scaningfilenamelabel,
		       subscaningfilenamelabel1);
    change_font_style((GtkLabel *) scanInfo->scaningfilenamelabel,
		      "<span foreground=\"#4c4554\" font_desc='14'>%s</span>");

    if (strncmp(g_scanResultInfo.filename, "//sys/", 6) == 0
	|| strncmp(g_scanResultInfo.filename, "/sys/", 5) == 0)
	{
	    if (g_scanResultInfo.pathsSize == 0)
		{
		    timeout_callback(scanInfo->progress_bar, fileNum, 19000);
		}
	    else
		{
		    timeout_callback(scanInfo->progress_bar,
				     g_scanResultInfo.filesize,
				     g_scanResultInfo.pathsSize);
		}

	}
    else
	{
	    timeout_callback(scanInfo->progress_bar,
			     g_scanResultInfo.filesize,
			     g_scanResultInfo.pathsSize);

	}

    if (scanStatus == SCAN_CONTINUE)
	{
	    return SCAN_CONTINUE;
	}
    else
	{
	    return SCAN_OVER;
	}
    return 1;
}
