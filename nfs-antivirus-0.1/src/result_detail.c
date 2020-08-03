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
/**此文件为扫描结果信息界面****/
#include<gtk/gtk.h>
#include <locale.h>
#include <libintl.h>
#include <string.h>
#include "result_detail.h"
#include "scan.h"
#include "result.h"
#include "refresh.h"
#include "./deal/home_handle.h"
#include "page/scanprocess-page.h"
#include "function.h"
#include "page/setting-page.h"
#define _(STRING) gettext(STRING)

extern GtkWidget *table7,*table6,*table1,*table2,*table3,*table4,*layout_main,*antiallscanbutton,*antiquickbutton,*antimyselfbutton,*thingbox,*last_scan_label,*real_protect_result_table;
extern GtkWidget *tog_all_check_button;
extern GtkWidget *allscan_button_image,*quickscan_button_image,*selfscan_button_image;
extern GtkWidget *click_start_defence_button,*start_defence_image;
extern GtkWidget *click_start_defence_image;
extern GtkWidget *quickscan_above_image,*allscan_above_image,*selfscan_above_image,*scan_progress_value_label;
extern GtkWidget *antibutton,*setbutton,*logbutton,*aboutbutton;
extern int area ;
extern int fileNum;
extern struct ScanInfo scanInfo;
extern GtkWidget *title_layout, * versionimage;
extern GtkWidget *feedback_label_button_white_bg,*feedback_label_button,*check_update_button,*check_update_button_white_bg;
GtkWidget *detail_result_table,*top_title_image, *result_title_layout,*detail_scanType_label,*detail_totaltime_label,*detail_totalobject_label,*detail_dealresult_label,*detail_needdeal_item_label,*not_deal_button,*immediately_deal_button,*autodeal_detail_button,*dealresult_label,*needdeal_item_label;
extern GtkWidget *click_start_defence_button,*click_close_defence_button;
struct ScanResults *phead_scanResults;
int deal_type=0;
char image_cancle_no_deal[1024];
extern char g_current_user[256]; //当前启动杀毒软件用户
void change_result_style(int cancel, int has_threat, int auto_deal,char *del_num,char *rep_num)
{
    char set_file_path[1024];
	snprintf(set_file_path,sizeof(set_file_path),"%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);

	int file_devide_flag = get_antivirus_setting_item_inttype("file-into-deivide",set_file_path);
	
//0918
	char get_special_threat_num[1024];
	memset(get_special_threat_num,0,sizeof(get_special_threat_num));

        int specail_num_inttype = get_file_line_num("/usr/share/antivirus/temp/scan_result_special.txt");
        snprintf(get_special_threat_num,sizeof(get_special_threat_num),"%d",specail_num_inttype);

        if(has_threat==0)
        {
                if(cancel==1)
                {       snprintf(image_cancle_no_deal,sizeof(image_cancle_no_deal),_("%scancle-no-deal.png"),ANTIVIRUS_IMAGE_DIR);
                        gtk_image_set_from_file(top_title_image,image_cancle_no_deal);
                }
                else
                { 
			
			memset(image_cancle_no_deal,0,sizeof(image_cancle_no_deal));
                        snprintf(image_cancle_no_deal,sizeof(image_cancle_no_deal),_("%sthe-scan-did-not-reveal-any-threat.png"),ANTIVIRUS_IMAGE_DIR);
                        gtk_image_set_from_file(top_title_image,image_cancle_no_deal);
                }
        }
        else
        {
		printf("--------------------======================----------------cancel is %d \n",cancel);
                if(cancel==1)
                {
				printf("090909090909090909090\n");
                        if(auto_deal==AUTO_DEAL)
                        {
				printf(",,,,<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>\n");
				char auto_deal_result[1024];
				memset(auto_deal_result,0,sizeof(auto_deal_result));
				if (file_devide_flag==1)
                                {
					if (strcmp(get_special_threat_num,"0")==0)
					{
                                        	snprintf(auto_deal_result,sizeof(auto_deal_result),_("auto deal delete to file divide area  %s files,repair %s packages"),del_num,rep_num);
					}
					else
					{
						if (strlen(get_special_threat_num)<3)
						{
							snprintf(auto_deal_result,1024,_("auto deal delete to file divide area  %s files,repair %s packages,%s is readfile"),del_num,rep_num,get_special_threat_num);
						}
						else
						{
							snprintf(auto_deal_result,1024,_("auto deal delete to file divide area  %s files,repair %s packages,%s\n is readfile"),del_num,rep_num,get_special_threat_num);
						}
					}
                                }
                                else
                                {
					if (strcmp(get_special_threat_num,"0")==0)
					{
						snprintf(auto_deal_result,sizeof(auto_deal_result),_("auto deal delete %s files,repair %s packages"),del_num,rep_num);
					}
					else
					{
						if (strlen(get_special_threat_num)<3)
						{
                                        		snprintf(auto_deal_result,1024,_("auto deal delete %s files,repair %s packages,%s is readfile"),del_num,rep_num,get_special_threat_num);
						}
						else
						{
                                        		snprintf(auto_deal_result,1024,_("auto deal delete %s files,repair %s packages,%s\n is readfile"),del_num,rep_num,get_special_threat_num);
							
						}
					}
                                }
				gtk_label_set_text(detail_dealresult_label,auto_deal_result);
                                 memset(image_cancle_no_deal,0,sizeof(image_cancle_no_deal));
                        snprintf(image_cancle_no_deal,sizeof(image_cancle_no_deal),_("%scancle-auot-deal.png"),ANTIVIRUS_IMAGE_DIR);
                                gtk_image_set_from_file(top_title_image,image_cancle_no_deal);
                        }
                        else
                        {   
			//	printf("[][][][][][][][][][][]\n") ;   
                                memset(image_cancle_no_deal,0,sizeof(image_cancle_no_deal));
                        	snprintf(image_cancle_no_deal,sizeof(image_cancle_no_deal),_("%suncom-found-threat.png"),ANTIVIRUS_IMAGE_DIR);
                                gtk_image_set_from_file(top_title_image,image_cancle_no_deal);
                        }
                }
                else
                {
				printf("[][][][][][][][][][][]\n") ;   
                        if(auto_deal==AUTO_DEAL)
                        {
				char auto_deal_result[1024];
				memset(auto_deal_result,0,sizeof(auto_deal_result));
				if (file_devide_flag==1)
                                {
						printf("haishi haishi这里这里这里这里这里！！！！！！\n");
                                        if (strcmp(get_special_threat_num,"0")==0)
					{
						snprintf(auto_deal_result,1024,_("auto deal delete to file divide area  %s files,repair %s packages"),del_num,rep_num);
					}
					else
					{
						if (strlen(get_special_threat_num)<3)
						{
							snprintf(auto_deal_result,1024,_("auto deal delete to file divide area  %s files,repair %s packages,%s is readfile"),del_num,rep_num,get_special_threat_num);
						}
						else
						{
							snprintf(auto_deal_result,1024,_("auto deal delete to file divide area  %s files,repair %s packages,%s\n is readfile"),del_num,rep_num,get_special_threat_num);
							
						}
					}
                                }
                                else
                                {
					printf("没有加入隔离区,特殊文件个数为%s\n",get_special_threat_num);
					if (strcmp(get_special_threat_num,"0")==0)
					{
                                        	snprintf(auto_deal_result,1024,_("auto deal delete %s files,repair %s packages"),del_num,rep_num);
					}
					else
					{
						if (strlen(get_special_threat_num)<3)
						{
							snprintf(auto_deal_result,1024,_("auto deal delete %s files,repair %s packages,%s is readfile"),del_num,rep_num,get_special_threat_num);
						}
						else
						{
							snprintf(auto_deal_result,1024,_("auto deal delete %s files,repair %s packages,%s\n is readfile"),del_num,rep_num,get_special_threat_num);
						}
					}
                                }
				gtk_label_set_text(detail_dealresult_label,auto_deal_result);
				printf("langshuangADD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
                                gtk_widget_show(dealresult_label);
                                gtk_widget_show(detail_dealresult_label);
                               memset(image_cancle_no_deal,0,sizeof(image_cancle_no_deal));
                        snprintf(image_cancle_no_deal,sizeof(image_cancle_no_deal),_("%scom-auto-deal.png"),ANTIVIRUS_IMAGE_DIR);
                                gtk_image_set_from_file(top_title_image,image_cancle_no_deal);
                        }
                        else
                        {
                                memset(image_cancle_no_deal,0,sizeof(image_cancle_no_deal));
                        snprintf(image_cancle_no_deal,sizeof(image_cancle_no_deal),_("%scom-deal.png"),ANTIVIRUS_IMAGE_DIR);
                                gtk_image_set_from_file(top_title_image,image_cancle_no_deal);
                        }
                }
        
        }
	char needdealitems[1024];
        snprintf(needdealitems,1024,"%d",getThreadNum());
	gtk_label_set_text(detail_needdeal_item_label,needdealitems);
	change_font_style((GtkLabel *)detail_dealresult_label,"<span  font_desc='15'>%s</span>");
	change_font_style((GtkLabel *)detail_needdeal_item_label,"<span  font_desc='15'>%s</span>");
	if(has_threat==0)
        {
                gtk_widget_hide(detail_dealresult_label);
                gtk_widget_hide(dealresult_label);
                gtk_widget_hide(not_deal_button);
                gtk_widget_hide(immediately_deal_button);
                gtk_widget_hide(autodeal_detail_button);
        }
        else
        {
                if(auto_deal==AUTO_DEAL)
                {
	                gtk_widget_show(autodeal_detail_button);
                        gtk_widget_hide(not_deal_button);
                        gtk_widget_hide(immediately_deal_button);
                        gtk_widget_hide(needdeal_item_label);
                        gtk_widget_hide(detail_needdeal_item_label);
                }
                else
                {
			gtk_widget_show(not_deal_button);
                	gtk_widget_show(immediately_deal_button);
                        gtk_widget_hide(detail_dealresult_label);
                        gtk_widget_hide(dealresult_label);
                        gtk_widget_hide(autodeal_detail_button);
                }
        }
}

//获取扫描类型名称
void get_scan_type_name(char *detail_scanType)
{
     if(get_scanType()==ALL_SCAN)
                strcpy(detail_scanType,_("all-scan"));
        if(get_scanType()==FAST_SCAN)
                strcpy(detail_scanType,_("quick-scan"));
        if(get_scanType()==CUSTOM_SCAN || get_scanType()== CLICK_DIR_SCAN || get_scanType()== CLICK_FILE_SCAN )
                strcpy(detail_scanType,_("custom-scan"));
        if(get_scanType()==SPECIFIED_SCAN)
                strcpy(detail_scanType,_("usb-scan"));
        if(get_scanType() == FIREFOX_DOWNLOAD_SCAN)
        {
                strcpy(detail_scanType,_("download scan"));
        }
}

void switch_page_icon()
{     char tmp_avlstatus[512];char image_allscan_image[512];
      snprintf(tmp_avlstatus,sizeof(tmp_avlstatus),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
      int status = get_upgrde_status(tmp_avlstatus);

      if(status==8){
                int scan_type = get_scanType();
           if(scan_type==1){
                snprintf(image_allscan_image,sizeof(image_allscan_image),_("%sallscan_image.png"),ANTIVIRUS_IMAGE_DIR);
		gtk_image_set_from_file(allscan_above_image,image_allscan_image);
                memset(image_allscan_image,0,sizeof(image_allscan_image));
                snprintf(image_allscan_image,sizeof(image_allscan_image),_("%sallscan.png"),ANTIVIRUS_IMAGE_DIR);
		gtk_image_set_from_file((GtkImage *)allscan_button_image,image_allscan_image);
		gtk_widget_show(allscan_button_image);
        }
        if(scan_type==2){
                memset(image_allscan_image,0,sizeof(image_allscan_image));
                snprintf(image_allscan_image,sizeof(image_allscan_image),_("%squickscan_image.png"),ANTIVIRUS_IMAGE_DIR);
		gtk_image_set_from_file(quickscan_above_image,image_allscan_image);
                 memset(image_allscan_image,0,sizeof(image_allscan_image));
                snprintf(image_allscan_image,sizeof(image_allscan_image),_("%squicksacn.png"),ANTIVIRUS_IMAGE_DIR);
		gtk_image_set_from_file((GtkImage *)quickscan_button_image,image_allscan_image);
		gtk_widget_show(quickscan_button_image);
        }
        if(scan_type==3){
                memset(image_allscan_image,0,sizeof(image_allscan_image));
                snprintf(image_allscan_image,sizeof(image_allscan_image),_("%smyselfscan-image.png"),ANTIVIRUS_IMAGE_DIR);
		gtk_image_set_from_file(selfscan_above_image,image_allscan_image);
                 memset(image_allscan_image,0,sizeof(image_allscan_image));
                snprintf(image_allscan_image,sizeof(image_allscan_image),_("%smyselfscan.png"),ANTIVIRUS_IMAGE_DIR);
		gtk_image_set_from_file((GtkImage *)selfscan_button_image,image_allscan_image);
		gtk_widget_show(selfscan_button_image);
        }
     }

}
void page_switch_to_result_detail(int cancel,int has_threat,int auto_deal,char *del_num,char *rep_num)
{   
    char tmp_avlstatus[512];
      snprintf(tmp_avlstatus,sizeof(tmp_avlstatus),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
    char detail_scanType[30];

    if(gtk_widget_get_visible(table6))
    {
        gtk_widget_hide(table6);
    }
    if(gtk_widget_get_visible(table1))
    {
        gtk_widget_hide(table1);
    }
    if(gtk_widget_get_visible(table2))
    {
        gtk_widget_hide(table2);
    }
    if(gtk_widget_get_visible(table3))
    {
        gtk_widget_hide(table3);
    }
    if(gtk_widget_get_visible(table4))
    {
        gtk_widget_hide(table4);
    }
    if(gtk_widget_get_visible(layout_main))
    {
        gtk_widget_hide(layout_main);
    }
    if(gtk_widget_get_visible( title_layout))
    {
        gtk_widget_hide(title_layout);
    }
    if(gtk_widget_get_visible(versionimage))
    {
        gtk_widget_hide(versionimage);
    }

    //根据扫描结果，更改标题样式
    change_result_style(cancel,has_threat,auto_deal,del_num,rep_num);
    
    get_scan_type_name(detail_scanType);
    gtk_label_set_text(detail_scanType_label,detail_scanType);
    change_font_style((GtkLabel *)detail_scanType_label,"<span font_desc='15'>%s</span>");
    char totaltime[1024];
    computeTotaltime(&scanInfo,totaltime,1024);
    gtk_label_set_text(detail_totaltime_label,totaltime);
    change_font_style((GtkLabel *)detail_totaltime_label,"<span  font_desc='15'>%s</span>");
    char scanobjNum[1024] ;
    snprintf(scanobjNum,1024,"%d",fileNum);
    gtk_label_set_text(detail_totalobject_label,scanobjNum);
    change_font_style((GtkLabel *)detail_totalobject_label,"<span  font_desc='15'>%s</span>");
    area = 11;
    gtk_widget_show(detail_result_table);

    write_antivirus_status(tmp_avlstatus,"8");
   
    switch_page_icon();
}


void switch_to_mainmenu()
{      char tmp_scan_results[512];char image_allscanning[512];
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tog_all_check_button),FALSE);
	    if (gtk_widget_get_visible (real_protect_result_table))
    {
        gtk_widget_hide(real_protect_result_table);
    }

    if(area != 6)
    {   
	snprintf(tmp_scan_results,sizeof(tmp_scan_results),"rm -rf %sscan_results.txt %sscan_result_deal.txt",ANTIVIRUS_TEMP_DIR,ANTIVIRUS_TEMP_DIR);
        system(tmp_scan_results);
    }
    gtk_widget_hide(table6);
    if(phead_scanResults!= NULL)
    {
        free_list_memory();
    }
    if(area != 10)
    {
        if(area==7)
           gtk_widget_hide(table7);
    }
    if(area==11)
        gtk_widget_hide(detail_result_table);
    memset(tmp_scan_results,0,sizeof(tmp_scan_results));
    snprintf(tmp_scan_results,sizeof(tmp_scan_results),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR); 
    int status = get_upgrde_status(tmp_scan_results);
    if(status==3){
        int scan_type = get_scanType();
        if(scan_type==1){
                memset(image_allscanning,0,sizeof(image_allscanning));
                snprintf(image_allscanning,sizeof(image_allscanning),_("%sallscanning.gif"),ANTIVIRUS_IMAGE_DIR);
		gtk_image_set_from_file(allscan_above_image,image_allscanning);
                memset(image_allscanning,0,sizeof(image_allscanning));
                snprintf(image_allscanning,sizeof(image_allscanning),_("%sallscan_down.png"),ANTIVIRUS_IMAGE_DIR);
		gtk_image_set_from_file(allscan_button_image,image_allscanning);
		gtk_widget_show(allscan_button_image);
        }
        if(scan_type==2){
                memset(image_allscanning,0,sizeof(image_allscanning));
                snprintf(image_allscanning,sizeof(image_allscanning),_("%squickscanning.gif"),ANTIVIRUS_IMAGE_DIR);
		gtk_image_set_from_file(quickscan_above_image,image_allscanning);
                memset(image_allscanning,0,sizeof(image_allscanning));
                snprintf(image_allscanning,sizeof(image_allscanning),_("%squickscan_down.png"),ANTIVIRUS_IMAGE_DIR);
		gtk_image_set_from_file(quickscan_button_image,image_allscanning);
		gtk_widget_show(quickscan_button_image);
        }
        if(scan_type==3 || scan_type == CLICK_DIR_SCAN || scan_type == CLICK_FILE_SCAN ){
                 memset(image_allscanning,0,sizeof(image_allscanning));                                           
                snprintf(image_allscanning,sizeof(image_allscanning),_("%smyselfscanning.gif"),ANTIVIRUS_IMAGE_DIR);
		 gtk_image_set_from_file(selfscan_above_image,image_allscanning);
                memset(image_allscanning,0,sizeof(image_allscanning));
                snprintf(image_allscanning,sizeof(image_allscanning),_("%smyselfscan_down.png"),ANTIVIRUS_IMAGE_DIR);  
		gtk_image_set_from_file(selfscan_button_image,image_allscanning);
		gtk_widget_show(selfscan_button_image);
        }
    }

    gtk_button_set_relief((GtkButton *)antibutton,GTK_RELIEF_NONE);
    memset(image_allscanning,0,sizeof(image_allscanning));
    snprintf(image_allscanning,sizeof(image_allscanning),_("%santi_enter.png"),ANTIVIRUS_IMAGE_DIR);
    GtkWidget *enter_antibutton_image = gtk_image_new_from_file(image_allscanning);
    gtk_button_set_image(antibutton,enter_antibutton_image);	

    gtk_button_set_relief((GtkButton *)setbutton,GTK_RELIEF_NONE);
    memset(image_allscanning,0,sizeof(image_allscanning));
    snprintf(image_allscanning,sizeof(image_allscanning),_("%ssetting.png"),ANTIVIRUS_IMAGE_DIR);
    GtkWidget *enter_setbutton_image = gtk_image_new_from_file(image_allscanning);
    gtk_button_set_image(setbutton,enter_setbutton_image);

    gtk_button_set_relief((GtkButton *)logbutton,GTK_RELIEF_NONE);
     memset(image_allscanning,0,sizeof(image_allscanning));
    snprintf(image_allscanning,sizeof(image_allscanning),_("%slogging.png"),ANTIVIRUS_IMAGE_DIR);
    GtkWidget *leave_logbutton_image = gtk_image_new_from_file(image_allscanning);
    gtk_button_set_image(logbutton,leave_logbutton_image);

    gtk_button_set_relief((GtkButton *)aboutbutton,GTK_RELIEF_NONE);
    memset(image_allscanning,0,sizeof(image_allscanning));
    snprintf(image_allscanning,sizeof(image_allscanning),_("%sabout.png"),ANTIVIRUS_IMAGE_DIR);
    GtkWidget *leave_aboutbutton_image = gtk_image_new_from_file(image_allscanning);
    gtk_button_set_image(aboutbutton,leave_aboutbutton_image);

    if (gtk_widget_get_visible (start_defence_image))
    {
	gtk_widget_hide(start_defence_image);
    }

    /*char year[10],month[10],day[10];
    get_last_scan_time(year,month,day);
    
    char last_scan_time[200];
    sprintf(last_scan_time,_("you last scan is:%syear,%smonth,%sday"),year,month,day);*/
    char year[10],month[10],day[10];
    char last_scan_time[200];
    if(get_last_scan_time(year,month,day))
    {

    sprintf(last_scan_time,_("you last scan is:%syear,%smonth,%sday"),year,month,day);
    }
    else{
        gtk_label_set_text(last_scan_label,_("NFS anti protect your system"));
    }
    gtk_label_set_text(last_scan_label,last_scan_time); 
    change_font_style((GtkLabel *)last_scan_label,"<span foreground=\"#FFFFFF\" font_desc='10'>%s</span> ");
    area = 1;
    if (!gtk_widget_get_visible( title_layout))
    {
        gtk_widget_show(title_layout);
    }
    if (!gtk_widget_get_visible(versionimage))
    {
        gtk_widget_show(versionimage);
    }

    gtk_widget_show(table1);
    gtk_widget_hide(feedback_label_button_white_bg);
    gtk_widget_hide(check_update_button_white_bg);
    gtk_widget_show(feedback_label_button);
    gtk_widget_show(check_update_button);
    if (get_defence_status()==1)
    {
		gtk_widget_hide(click_start_defence_button);
		gtk_widget_show(click_close_defence_button);
    }
    else
    {
		gtk_widget_hide(click_close_defence_button);
		gtk_widget_show(click_start_defence_button);
    }

    gtk_widget_show(layout_main);
}


void page_switch_to_result(GtkWidget *widget,gpointer *data)
{
	if(area==11)
		gtk_widget_hide(detail_result_table);
        result(data);
}
void detail_result(int cancel,int has_threat,int auto_deal,char *del_num,char *rep_num)
{       char image_cancle_no_deal[512];
        char css_scan_process_page[512];
	printf("进入这个方法啦\n");
	GtkWidget *detail_fix,*title_image,*scantype_label,*totaltime_label,*totalobject_label,*back_to_mainmenu_button,*top_fix,*top_title_label;
        GtkWidget *detail_vbox,*button_hbox,*resultlabel;
        char detail_scanType[1024];
        int n=getThreadNum();
        snprintf(css_scan_process_page,sizeof(css_scan_process_page),"%sscan_process_page-chinese.css",ANTIVIRUS_CSS_DIR);
GFile *file=g_file_new_for_path(css_scan_process_page);
        GdkScreen *screen;
        screen = gdk_screen_get_default();
        GtkCssProvider *cssProvider;
        cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_file(cssProvider, file, NULL);

	detail_result_table=gtk_grid_new ();
	gtk_container_add(GTK_CONTAINER(thingbox),detail_result_table);
	detail_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_grid_attach((GtkGrid *)detail_result_table,detail_vbox,0,0,1,1);
	gtk_widget_show(detail_vbox);
	
        top_fix = gtk_fixed_new();
	gtk_widget_show(top_fix);
        GtkWidget *detail_layout;
        snprintf(image_cancle_no_deal,sizeof(image_cancle_no_deal),_("%scancle-no-deal.png"),ANTIVIRUS_IMAGE_DIR);
        top_title_image = gtk_image_new_from_file(image_cancle_no_deal);
	gtk_widget_show(top_title_image);
        result_title_layout = gtk_layout_new(NULL, NULL);
	gtk_widget_show(result_title_layout);
        gtk_widget_set_size_request(result_title_layout, 800, 106);
        gtk_layout_put(GTK_LAYOUT(result_title_layout), top_title_image, 0, 0);
        gtk_fixed_put(GTK_FIXED(top_fix),result_title_layout,0,0);
        back_to_mainmenu_button = gtk_button_new();
	gtk_widget_show(back_to_mainmenu_button);
        gtk_button_set_relief((GtkButton *)back_to_mainmenu_button,GTK_RELIEF_NONE);
	 gtk_widget_set_size_request(back_to_mainmenu_button,69,37);
        gtk_widget_set_name(back_to_mainmenu_button,"back_to_mainmenu_button");
        memset(image_cancle_no_deal,0,sizeof(image_cancle_no_deal));
        snprintf(image_cancle_no_deal,sizeof(image_cancle_no_deal),_("%sback_menu.png"),ANTIVIRUS_IMAGE_DIR);
       // gtk_button_set_image(GTK_BUTTON(back_to_mainmenu_button),gtk_image_new_from_file(image_cancle_no_deal));
        gtk_layout_put(GTK_LAYOUT(result_title_layout), (GtkWidget *)back_to_mainmenu_button, 40, 27);
         g_signal_connect(G_OBJECT(back_to_mainmenu_button),"clicked",G_CALLBACK(switch_to_mainmenu),NULL);
       // g_signal_connect(G_OBJECT(back_to_mainmenu_button),"enter",G_CALLBACK(enter_back_button),NULL);


        memset(image_cancle_no_deal,0,sizeof(image_cancle_no_deal));

        snprintf(image_cancle_no_deal,sizeof(image_cancle_no_deal),_("%sresult-background.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *detail_layout_backimage = gtk_image_new_from_file(image_cancle_no_deal);
	gtk_widget_show(detail_layout_backimage);
        detail_fix=gtk_fixed_new();
	gtk_widget_show(detail_fix);
	
	detail_layout = gtk_layout_new(NULL, NULL);
	gtk_widget_show(detail_layout);
	gtk_widget_set_size_request(detail_layout, 800, 411);
        gtk_fixed_put(GTK_FIXED(detail_fix),detail_layout,0,6);

        scantype_label=gtk_label_new(_("ScanType:"));
	gtk_widget_show(scantype_label);
	change_font_style((GtkLabel *)scantype_label,"<span  font_desc='15'>%s</span>");
	detail_scanType_label=gtk_label_new("scan style");
	gtk_widget_show(detail_scanType_label);
        change_font_style((GtkLabel *)detail_scanType_label,"<span font_desc='15'>%s</span>");

        totaltime_label=gtk_label_new(_("ScanTime:"));
	gtk_widget_show(totaltime_label);
        change_font_style((GtkLabel *)totaltime_label,"<span  font_desc='15'>%s</span>");
        detail_totaltime_label=gtk_label_new("scan time");
	gtk_widget_show(detail_totaltime_label);
        change_font_style((GtkLabel *)detail_totaltime_label,"<span  font_desc='15'>%s</span>");

        totalobject_label=gtk_label_new(_("ScanObject:"));
	gtk_widget_show(totalobject_label);
        change_font_style((GtkLabel *)totalobject_label,"<span  font_desc='15'>%s</span>");
        detail_totalobject_label=gtk_label_new("scan object");
	gtk_widget_show(detail_totalobject_label);
        change_font_style((GtkLabel *)detail_totalobject_label,"<span  font_desc='15'>%s</span>");

	dealresult_label=gtk_label_new(_("DealResult:"));
	gtk_widget_show(dealresult_label);
        change_font_style((GtkLabel *)dealresult_label,"<span  font_desc='15'>%s</span>");
	detail_dealresult_label=gtk_label_new("deal result");
	gtk_widget_show(detail_dealresult_label);
        change_font_style((GtkLabel *)detail_dealresult_label,"<span  font_desc='15'>%s</span>");

	needdeal_item_label=gtk_label_new(_("NeedDealItems:"));
	gtk_widget_show(needdeal_item_label);
        change_font_style((GtkLabel *)needdeal_item_label,"<span  font_desc='15'>%s</span>");
	detail_needdeal_item_label=gtk_label_new("need deal items");
	gtk_widget_show(detail_needdeal_item_label);
        change_font_style((GtkLabel *)detail_needdeal_item_label,"<span  font_desc='15'>%s</span>");

	gtk_layout_put(GTK_LAYOUT(detail_layout),detail_layout_backimage,15,0);
        gtk_layout_put(GTK_LAYOUT(detail_layout),scantype_label,250,60);
        gtk_layout_put(GTK_LAYOUT(detail_layout),detail_scanType_label,430,60);
        gtk_layout_put(GTK_LAYOUT(detail_layout),totaltime_label,250,110);
        gtk_layout_put(GTK_LAYOUT(detail_layout),detail_totaltime_label,430,110);
        gtk_layout_put(GTK_LAYOUT(detail_layout),totalobject_label,250,160);
        gtk_layout_put(GTK_LAYOUT(detail_layout),detail_totalobject_label,430,160);
        gtk_layout_put(GTK_LAYOUT(detail_layout),dealresult_label,250,210);
        gtk_layout_put(GTK_LAYOUT(detail_layout),detail_dealresult_label,430,210);
        gtk_layout_put(GTK_LAYOUT(detail_layout),needdeal_item_label,250,210);
        gtk_layout_put(GTK_LAYOUT(detail_layout),detail_needdeal_item_label,430,210);
      
	not_deal_button=gtk_button_new();
	gtk_widget_show(not_deal_button);
	gtk_button_set_relief((GtkButton *)not_deal_button,GTK_RELIEF_NONE);
         memset(image_cancle_no_deal,0,sizeof(image_cancle_no_deal));
        snprintf(image_cancle_no_deal,sizeof(image_cancle_no_deal),_("%snotdeal.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *not_deal_button_image = gtk_image_new_from_file(image_cancle_no_deal);
	gtk_widget_show(not_deal_button_image);
        gtk_container_add(GTK_CONTAINER(not_deal_button), not_deal_button_image);
        immediately_deal_button=gtk_button_new();
	gtk_widget_show(immediately_deal_button);
	gtk_button_set_relief((GtkButton *)immediately_deal_button,GTK_RELIEF_NONE);
        memset(image_cancle_no_deal,0,sizeof(image_cancle_no_deal));
        snprintf(image_cancle_no_deal,sizeof(image_cancle_no_deal),_("%simmdeal.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *immediately_deal_button_image = gtk_image_new_from_file(image_cancle_no_deal);
	gtk_widget_show(immediately_deal_button_image);
        gtk_container_add(GTK_CONTAINER(immediately_deal_button),immediately_deal_button_image);
        autodeal_detail_button=gtk_button_new();
	gtk_widget_show(autodeal_detail_button);
	gtk_button_set_relief((GtkButton *)autodeal_detail_button,GTK_RELIEF_NONE);
        memset(image_cancle_no_deal,0,sizeof(image_cancle_no_deal));
        snprintf(image_cancle_no_deal,sizeof(image_cancle_no_deal),_("%sdetail.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *autodeal_detail_button_image = gtk_image_new_from_file(image_cancle_no_deal);
	gtk_widget_show(autodeal_detail_button_image);
        gtk_container_add(GTK_CONTAINER(autodeal_detail_button),autodeal_detail_button_image);
        g_signal_connect(G_OBJECT(not_deal_button),"clicked",G_CALLBACK(switch_to_mainmenu),NULL);
        deal_type=1; 
	g_signal_connect(G_OBJECT(immediately_deal_button),"clicked",G_CALLBACK(page_switch_to_result),deal_type);

	deal_type =2;
        g_signal_connect(G_OBJECT(autodeal_detail_button),"clicked",G_CALLBACK(page_switch_to_result),deal_type);
	
	gtk_layout_put(GTK_LAYOUT(detail_layout),not_deal_button,260,260);
	gtk_layout_put(GTK_LAYOUT(detail_layout),immediately_deal_button,400,260);
	gtk_layout_put(GTK_LAYOUT(detail_layout),autodeal_detail_button,520,320);
	gtk_box_pack_start(GTK_BOX(detail_vbox), top_fix, FALSE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(detail_vbox), detail_fix, FALSE, TRUE, 3);
gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);
//	area = 11;

}
