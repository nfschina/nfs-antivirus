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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/stat.h>
#include "../deal/setting_handler.h"
#include "../antivirus.h"
#include "../function.h"
#include "setting-page.h"
#include "isolation-recovery-page.h"
#include "../deal/isolation_handle.h"
#include "home-page.h"
#define _(STRING) gettext(STRING)
extern char g_current_user[256]; //当前启动杀毒软件用户
enum{
WHITE_LIST_TOG_COL,
WHITE_LIST_FILE_COL,
WHITE_LIST_STATUS_COL
};
struct DevideFileInfo 
{
    char devide_file_path[512];
    char devide_time[50];
    struct DevideFileInfo *next;
};
struct DevideFileInfo *phead_devidefile=NULL;
struct WhiteListFileInfo
{
    char file[256];
    char file_type[100];
    char file_size[100];
    char file_time[100];
    struct WhiteListFileInfo *next;
};
struct WhiteListFileInfo *phead_whitefile=NULL;
GSList *white_list;
GtkWidget * regular_window;
extern GtkWidget *list;
extern GtkWindow *window;
GtkWidget *common_settings_fix,*common_setting_content_box,*real_time_prtect_button,*real_time_protect_dir,*choose_protect_button,*real_time_protect_is_label;
GtkWidget *scan_style_setting_fix,*scan_style_setting_content_box;
GtkWidget *anti_scan_settings_fix,*anti_scan_setting_content_box;
GtkWidget *update_settings_fix,*update_setting_content_box,*devide_file_dir;
int auto_timing_anti_flag=1;
int auto_choose_flag=1;
GtkWidget *devide_content_box;
#define _(STRING) gettext(STRING)
#define SETTING_PAGE_WIDTH 200
#define SETTING_PAGE_LENTH 30
extern GtkWidget *table2;
extern GtkWindow *window;
//void select_isolation_region_dir(GtkWidget *widget,gpointer *data);
//void select_devide_dir();
char g_current_user[256]; //当前启动杀毒软件用户

void add_regular_dialog(GtkWidget *widget,gpointer data);
void init_setting(){
	char cmd[256];
        snprintf(cmd,sizeof(cmd),"echo boot-start=0 >>%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
	system(cmd);
	memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd),"echo auto-check-mobile-dev=0 >>%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
	system(cmd);	
        memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd),"echo auto-check-download=0 >>%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
        system(cmd);

	memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd),"echo scan-setting=1 >>%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
        system(cmd);

	memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd),"echo anti-deal-style=1 >>%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
        system(cmd);

	memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd),"echo file-into-deivide=0 >>%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
        system(cmd);

        memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd),"echo start-timing-anti=0 >>%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
        system(cmd);

        memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd),"echo timing-scan-style=1 >>%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
        system(cmd);
	
         memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd),"echo timing-scan-select=1 >>%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
        system(cmd);

	 memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd),"echo timing-scan-month=16,12,30 >>%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
        system(cmd);

         memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd),"echo timing-scan-week=1,12,30 >>%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
        system(cmd);

	  memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd),"echo timing-scan-day=12,30,30 >>%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
        system(cmd);
	
	memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd),"echo update-style=2 >>%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
        system(cmd);

	memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd),"echo timing-update-select=3 >>%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
        system(cmd);

         memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd),"echo timing-update-month=16,12,30 >>%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
        system(cmd);

	memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd),"echo timing-update-week=1,12,30 >>%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
        system(cmd);	

	memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd),"echo timing-update-day=12,0,15 >>%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
        system(cmd);

	memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd),"echo devide-dir=/usr/share/antivirus/isolation >>%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
        system(cmd);

	
	memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd),"echo check-real-time-protect=0 >>%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
        system(cmd);

	  memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd)," mkdir %s",ANTIVIRUS_DEFAULT_ISOLATION_DIR);
        system(cmd);
}

void add_reg_file(GtkWidget *widget,gpointer data)
{
        gchar * regList = "";
        regList = gtk_entry_get_text(data);

        if(strlen(regList) > 200)
        {
            gtk_window_set_transient_for(regular_window,window);
            show_warn_info(_("The rule length of the white list should not exceed 200"),1);
            return ;
        }


        if(strcmp(regList,"") != 0)
        {
           char file_info[1024];
           FILE *pBufSize;
           char resultBuffer[256];
           char cmd[1024];
           snprintf(cmd,sizeof(cmd),"%sadd_white_list.sh %s",ANTIVIRUS_BIN_WHITE_DIR,regList);
           if((pBufSize = popen(cmd,"r")) == NULL)
           {
                return;
           }
           fgets(resultBuffer,256,pBufSize);
           if(strncmp(resultBuffer,"1",1) == 0)
           {
                fclose(pBufSize);
                show_warn_info(_("white list regular expression already exist"),1);
                return ;
           }

           strcat_white_list(file_info,"2",regList,"null","null");
	   char white_list[256];
           snprintf(white_list,sizeof(white_list),"%swhite_list.txt",ANTIVIRUS_CONF_DIR);
           write_white_list(white_list,file_info);
	   gtk_widget_destroy(regular_window);
           if(box!=NULL)
                gtk_widget_destroy(box);
           white_list_settings(while_list_content_box);
        }
        else
        {
		gtk_window_set_transient_for(regular_window,window);
                show_warn_info(_("white list regular can not be null"),1);
                return ;

        }

}


void quit_regular_window(GtkWidget *widget,gpointer data)
{
        gtk_widget_destroy(data);
}


void add_regular_dialog(GtkWidget *widget,gpointer data)
{
        GtkWidget *regular_layout;
	char image_dir[256];
        regular_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(regular_window,_("add regular"));
        gtk_window_set_decorated(GTK_WINDOW(regular_window), FALSE);
        gtk_window_set_position(GTK_WINDOW(regular_window),GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(regular_window),378,246);
        snprintf(image_dir,sizeof(image_dir),_("%sNFSantivirus.png"),ANTIVIRUS_IMAGE_DIR);

        gtk_window_set_icon_from_file(GTK_WINDOW(regular_window),image_dir,NULL);
        gtk_widget_set_events(GTK_WINDOW(regular_window),
                   GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK
                   | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);

        g_signal_connect(GTK_WINDOW(regular_window), "button_press_event", G_CALLBACK (button_press_event), regular_window);       // 加入事件回调
//        g_signal_connect(GTK_WINDOW(regular_window), "motion_notify_event",G_CALLBACK( motion_notify_event), regular_window);
        g_signal_connect(GTK_WINDOW(regular_window), "button_release_event", G_CALLBACK(button_release_event), regular_window);
	gtk_widget_show(regular_window);
	regular_layout = gtk_layout_new(NULL, NULL);
	gtk_widget_set_size_request(regular_layout, 378,246);
	gtk_widget_show(regular_layout);
	gtk_container_add(GTK_CONTAINER(regular_window),(GtkWidget*)regular_layout);
	
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sregular_bg.png"),ANTIVIRUS_IMAGE_DIR);
	GtkWidget *regular_bg_image = gtk_image_new_from_file(image_dir);
	gtk_widget_show(regular_bg_image);
	gtk_layout_put(GTK_LAYOUT(regular_layout),regular_bg_image, 0, 0);

	char tipdialog_css[256];
        snprintf(tipdialog_css,sizeof(tipdialog_css),_("%stipdialog.css"),ANTIVIRUS_CSS_DIR);
	GFile *file=g_file_new_for_path(tipdialog_css);
        GdkScreen *screen;
        screen = gdk_screen_get_default();
        GtkCssProvider *cssProvider;
        cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_file(cssProvider, file, NULL);

	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sclose.png"),ANTIVIRUS_IMAGE_DIR);	
	GtkWidget *close_image = gtk_image_new_from_file(image_dir);
	gtk_widget_show(close_image);
	GtkWidget *close_button = gtk_button_new();
	gtk_button_set_relief((GtkButton *)close_button,GTK_RELIEF_NONE);
//	gtk_button_set_image(close_button,close_image);
//	gtk_widget_show(close_button);
	gtk_widget_set_name(close_button,"tipdialog-close");
        gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);
	gtk_layout_put(GTK_LAYOUT(regular_layout),close_button, 350, 0);
	gtk_widget_show(close_button);
	g_signal_connect(G_OBJECT(close_button),"clicked",G_CALLBACK(quit_regular_window),regular_window);
	GtkWidget *reg_label=(GtkLabel *)gtk_label_new(_("Please input reg:"));
	gtk_layout_put(GTK_LAYOUT(regular_layout),reg_label, 36, 80);
	gtk_widget_show(reg_label);

	GtkWidget *reg_input_label=gtk_entry_new();
        gtk_layout_put(GTK_LAYOUT(regular_layout),reg_input_label, 36, 108);
        gtk_entry_set_max_length(GTK_ENTRY(reg_input_label),200);
	gtk_widget_set_size_request(reg_input_label, 308,26);
        gtk_widget_show(reg_input_label);

	GtkWidget *add_button=gtk_button_new();
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%ssure.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *addimage=gtk_image_new_from_file(image_dir);
//	g_signal_connect(G_OBJECT(add_button),"enter",G_CALLBACK(enter_add_button),NULL);
        gtk_widget_show(addimage);
	gtk_button_set_image(add_button,addimage);
	gtk_button_set_relief((GtkButton *)add_button,GTK_RELIEF_NONE);
	g_signal_connect(G_OBJECT(add_button),"clicked",G_CALLBACK(add_reg_file),reg_input_label);
        gtk_widget_show(add_button);
        gtk_layout_put(GTK_LAYOUT(regular_layout),add_button, 280, 195);
}

void antivirus_settings_page(GtkWidget *table)
{
	char setting[256];
	char image_dir[256];

        snprintf(setting,sizeof(setting),"%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
	if(access(setting,F_OK)!=0){
                init_setting();
        }

	printf("aaaaaaaaaaaaa\n");
        GtkWidget *settings_box;
        settings_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_widget_set_size_request(settings_box, 800, 400);
        gtk_widget_show(settings_box);
	GtkWidget *setting_left_layout = gtk_layout_new(NULL, NULL);
        gtk_widget_set_size_request(setting_left_layout, 800, 400);
        gtk_widget_show(setting_left_layout);
        gtk_container_add(GTK_CONTAINER(settings_box), setting_left_layout);

        snprintf(image_dir,sizeof(image_dir),_("%ssetting-left.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *setting_left_image = gtk_image_new_from_file(image_dir);
        gtk_widget_show(setting_left_image);
        gtk_layout_put(GTK_LAYOUT(setting_left_layout), setting_left_image, 0, 0);
	 memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%ssetting-left-mask.png"),ANTIVIRUS_IMAGE_DIR);
 	GtkWidget *setting_left_mask_image = gtk_image_new_from_file(image_dir);
        gtk_widget_show(setting_left_mask_image);
        gtk_layout_put(GTK_LAYOUT(setting_left_layout),setting_left_mask_image, 0, 0);

		printf("ffffffffffffffff\n");

	char setting_note_css[256];
        snprintf(setting_note_css,sizeof(setting_note_css),"%ssetting-note.css",ANTIVIRUS_CSS_DIR);
	GFile *file=g_file_new_for_path(setting_note_css);
        GdkScreen *screen;
        screen = gdk_screen_get_default();
        GtkCssProvider *cssProvider;
        cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_file(cssProvider, file, NULL);


        notebook=gtk_notebook_new();
        gtk_widget_set_size_request(notebook, 800, 400);
        gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook),GTK_POS_LEFT);
        gtk_layout_put(GTK_LAYOUT(setting_left_layout), notebook, 0, 0);

        GtkWidget *common_setting_page_label_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        GtkWidget *common_setting_page_label = gtk_label_new(_("rule-setting"));
	change_font_style(common_setting_page_label,"<span foreground=\"#000000\" font_desc='12' weight='bold'>%s</span>");
	gtk_box_pack_start(GTK_BOX (common_setting_page_label_box),common_setting_page_label,FALSE,FALSE,40);
        gtk_widget_set_size_request(common_setting_page_label_box, SETTING_PAGE_WIDTH, SETTING_PAGE_LENTH);
        common_setting_content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        common_settings(common_setting_content_box);
        gtk_notebook_insert_page(GTK_NOTEBOOK(notebook),common_setting_content_box,common_setting_page_label_box,0);
        gtk_widget_show(common_setting_page_label_box);
        gtk_widget_show(common_setting_page_label);
        gtk_widget_show(common_setting_content_box);
	
		printf("ppppppppppppp\n");
        GtkWidget *anti_scan_setting_page_label_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        GtkWidget *anti_scan_setting_page_label = gtk_label_new(_("anti-scan-setting"));
	change_font_style(anti_scan_setting_page_label,"<span foreground=\"#000000\" font_desc='12' weight='bold'>%s</span>");
        gtk_box_pack_start(GTK_BOX (anti_scan_setting_page_label_box),anti_scan_setting_page_label,FALSE,FALSE,40);
        gtk_widget_set_size_request(anti_scan_setting_page_label_box, SETTING_PAGE_WIDTH, SETTING_PAGE_LENTH);
        anti_scan_setting_content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        anti_scan_settings(anti_scan_setting_content_box);
		printf("lllllllllllll\n");
        gtk_notebook_insert_page(GTK_NOTEBOOK(notebook),anti_scan_setting_content_box,anti_scan_setting_page_label_box,1);
        gtk_widget_show(anti_scan_setting_page_label_box);
        gtk_widget_show(anti_scan_setting_page_label);
        gtk_widget_show(anti_scan_setting_content_box);

		printf("eeeeeeeeeeeeeee\n");

        GtkWidget *update_setting_page_label_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        GtkWidget *update_setting_page_label = gtk_label_new(_("update-set"));
	change_font_style(update_setting_page_label,"<span foreground=\"#000000\" font_desc='12' weight='bold'>%s</span>");
        gtk_box_pack_start(GTK_BOX (update_setting_page_label_box),update_setting_page_label,FALSE,FALSE,40);
        update_setting_content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_widget_set_size_request(update_setting_page_label_box, SETTING_PAGE_WIDTH, SETTING_PAGE_LENTH);
        update_settings(update_setting_content_box);
        gtk_notebook_insert_page(GTK_NOTEBOOK(notebook),update_setting_content_box,update_setting_page_label_box,2);
        gtk_widget_show(update_setting_page_label_box);
        gtk_widget_show(update_setting_page_label);
        gtk_widget_show(update_setting_content_box);

	GtkWidget *white_list_setting_page_label_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        GtkWidget *white_list_setting_page_label = gtk_label_new(_("white-list-setting"));
	change_font_style(white_list_setting_page_label,"<span foreground=\"#000000\" font_desc='12' weight='bold'>%s</span>");
        gtk_box_pack_start(GTK_BOX (white_list_setting_page_label_box),white_list_setting_page_label,FALSE,FALSE,40);
        gtk_widget_set_size_request(white_list_setting_page_label_box, SETTING_PAGE_WIDTH, SETTING_PAGE_LENTH);
        while_list_content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        white_list_settings(while_list_content_box);
        gtk_notebook_insert_page(GTK_NOTEBOOK(notebook),while_list_content_box,white_list_setting_page_label_box,3);
        gtk_widget_show(white_list_setting_page_label_box);
        gtk_widget_show(white_list_setting_page_label);
        gtk_widget_show(while_list_content_box);



		printf("ccccccccccccccccc\n");
	GtkWidget *look_devide_file_page_label_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        GtkWidget *look_devide_file_page_label = gtk_label_new(_("look over devide file"));
	change_font_style(look_devide_file_page_label,"<span foreground=\"#000000\" font_desc='12' weight='bold'>%s</span>");
        gtk_box_pack_start(GTK_BOX (look_devide_file_page_label_box),look_devide_file_page_label,FALSE,FALSE,40);
        devide_content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_widget_set_size_request(look_devide_file_page_label_box, SETTING_PAGE_WIDTH, SETTING_PAGE_LENTH);
        look_devide_file_settings(devide_content_box);
        gtk_notebook_insert_page(GTK_NOTEBOOK(notebook),devide_content_box,look_devide_file_page_label_box,4);
        gtk_widget_show(look_devide_file_page_label_box);
        gtk_widget_show(look_devide_file_page_label);
        gtk_widget_show(devide_content_box);
	gtk_notebook_set_current_page((GtkNotebook *)notebook,0);
	gtk_widget_set_name(notebook,"setting-note");
	gtk_widget_show(notebook);
        gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);
        gtk_grid_attach((GtkGrid *)table2,settings_box,0,0,1,1);
}



void compare_file_size_and_time(char *file_path,char *file_type,char *file_size,char *file_size_now,char *file_time,char *file_time_now,char *is_useful)
{
	if(access(file_path,F_OK)!=0){
                strcpy(is_useful,_("unuseful"));
                return;
        }
        get_file_size(file_path,file_size_now);
        get_file_modify_time(file_path,file_time_now);
        file_size_now[strlen(file_size_now)-2] ='\0';
        if(strcmp(file_type,"0")==0)
        {
                if(strcmp(file_size,file_size_now)==0 && strcmp(file_time,file_time_now)==0)
                        strcpy(is_useful,_("useful"));
                else
                        strcpy(is_useful,_("unuseful"));
        }
        else
                strcpy(is_useful,_("useful forever"));
}
void free_white_list_file_memory ()
{
    struct WhiteListFileInfo *pnext;
    while(phead_whitefile!=NULL)
    {
       pnext = phead_whitefile->next;
       free(phead_whitefile);
       phead_whitefile = NULL;
       phead_whitefile = pnext;
    }

}

GtkTreeModel * create_white_list_model(){
        GtkTreeIter iter;
        char c=0;
        struct WhiteListFileInfo *pcurrent,*ptempNode;
        store = gtk_list_store_new(3,G_TYPE_BOOLEAN,G_TYPE_STRING, G_TYPE_STRING);
	char white_list[256];
        snprintf(white_list,sizeof(white_list),"%swhite_list.txt",ANTIVIRUS_CONF_DIR);

        FILE *fp=fopen(white_list,"rw");
        if(fp == NULL)
        {
            return GTK_TREE_MODEL(store);

        }
        gchar buf[1023];
        gint i=0;
        int j;
        gchar result[100][4][100];

        while(fgets(buf,1023,fp) != NULL)
        {
                int in=0;
                char *p[20];
                char *buffer = buf;
                int j = 0;
                while((p[in]=strtok(buffer,","))!=NULL)
                {
                        in++;
                        buffer=NULL;
                }
               struct WhiteListFileInfo *newNode =(struct WhiteListFileInfo *) malloc(sizeof(struct WhiteListFileInfo)) ;
                if(i == 0)
                {
                    phead_whitefile = newNode;
                    phead_whitefile->next = NULL;
                    pcurrent = newNode;

                }
                 if (newNode->file)
                {
                    strncpy(newNode->file,p[0],255);
                }
                if(newNode->file_type)
                {
                    strncpy(newNode->file_type,p[1],99);
                }
                if(newNode->file_size)
                {
                    strncpy(newNode->file_size,p[2],99);
                }
                if(newNode->file_time)
                {
                    strncpy(newNode->file_time,p[3],49);
                }
                ptempNode = pcurrent;
                ptempNode->next = newNode;
                pcurrent = newNode ;
                newNode->next = NULL;
                i++;

        }
        fclose(fp);
        pcurrent = phead_whitefile;
        ptempNode = phead_whitefile;
        gtk_list_store_clear(store);
        for(j=0;j<i;j++)
        {
                if(pcurrent!=NULL)
                {
                char file_size_now[100];
                char file_time_now[100];
                char is_useful[100];
                if(strcmp(pcurrent->file_type ,"2") == 0 )
                {
                   strcpy(is_useful,_("useful forever"));
                }
                else
                {
                    compare_file_size_and_time(pcurrent->file,pcurrent->file_type,pcurrent->file_size,file_size_now,pcurrent->file_time,file_time_now,is_useful);
                }

                gtk_list_store_append(store,&iter);
                gtk_list_store_set(store,&iter,
                   0,FALSE,
                   1,pcurrent->file,     //PIXBUF_COL也可以用0代替
                   2,is_useful,
                    -1);
                 ptempNode = pcurrent->next;
                pcurrent = ptempNode;
                }

        }
        return GTK_TREE_MODEL(store);
}

int filechooser3(GtkWidget *window,GtkWidget *content_box){
         GtkWidget *filew;
        GSList *iterator;
   g_print("add file\n");
   filew =  gtk_file_chooser_dialog_new (_("File selection"),GTK_WINDOW(window),
                                                GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                         //    GTK_FILE_CHOOSER_ACTION_OPEN,
                                                GTK_STOCK_CANCEL,
                                                GTK_RESPONSE_CANCEL,
                                                GTK_STOCK_OK,
                                                GTK_RESPONSE_OK,
                                                NULL);

   gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(filew),TRUE);
   if(gtk_dialog_run (GTK_DIALOG (filew)) == GTK_RESPONSE_OK)
   {
         gtk_widget_hide(filew);
         white_list=gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(filew));
        for (iterator = white_list; iterator; iterator = iterator->next)
        {

        FILE *pBufSize;
        char resultBuffer[256];
        char cmd[1024];
        snprintf(cmd,sizeof(cmd),"%sadd_white_list.sh %s",ANTIVIRUS_BIN_WHITE_DIR,iterator->data);
        if((pBufSize = popen(cmd,"r")) == NULL)
        {
         return;
         }
         fgets(resultBuffer,256,pBufSize);
         if(strncmp(resultBuffer,"1",1) == 0)
          {
          fclose(pBufSize);
          return 0;
                        }
        else{
                char file_info[1024];
                strcat_white_list(file_info,"1",iterator->data,"null","null");
		char white_list[256];
        	snprintf(white_list,sizeof(white_list),"%swhite_list.txt",ANTIVIRUS_CONF_DIR);
                write_white_list(white_list,file_info);
        }
    }
}
    else
    {
        g_print("");
    }
    gtk_widget_destroy(filew);
if(box!=NULL)
            gtk_widget_destroy(box);
    white_list_settings(while_list_content_box);
    return 0;
}
int filechooser2(GtkWidget *window,GtkWidget *content_box)
{
        GtkWidget *filew;
        GSList *file_iterator;
   g_print("add file\n");
   filew =  gtk_file_chooser_dialog_new (_("File selection"),GTK_WINDOW(window),
                                         //       GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                             GTK_FILE_CHOOSER_ACTION_OPEN,
                                                GTK_STOCK_CANCEL,
                                                GTK_RESPONSE_CANCEL,
                                                GTK_STOCK_OK,
                                                GTK_RESPONSE_OK,
                                                NULL);

   gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(filew),TRUE);
   if(gtk_dialog_run (GTK_DIALOG (filew)) == GTK_RESPONSE_OK)
   {
         gtk_widget_hide(filew);
       white_list=(GtkWidget *)gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(filew));
        for (file_iterator = white_list; file_iterator; file_iterator = file_iterator->next)
        {
                
		FILE *pBufSize;
                char resultBuffer[256];
        	char cmd[1024];
        snprintf(cmd,sizeof(cmd),"%sadd_white_list.sh %s",ANTIVIRUS_BIN_WHITE_DIR,file_iterator->data);
        if((pBufSize = popen(cmd,"r")) == NULL)
        {
         return;
         }
         fgets(resultBuffer,256,pBufSize);
         if(strncmp(resultBuffer,"1",1) == 0)
          {
          fclose(pBufSize);
          return 0;
                        }
        else{
        char file_size[100];
        char file_time[100];
        char style[50];
        is_file(file_iterator->data,style);
        if(strcmp(style,"0")==0){

        get_file_size(file_iterator->data,file_size);
        file_size[strlen(file_size)-2] ='\0';

        get_file_modify_time(file_iterator->data,file_time);
        }
        else{
                strcpy(file_size,"null");
                strcpy(file_time,"null");
        }
        char file_info[1024];
        strcat_white_list(file_info,style,file_iterator->data,file_size,file_time);
	char white_list[256];
        snprintf(white_list,sizeof(white_list),"%swhite_list.txt",ANTIVIRUS_CONF_DIR);
        write_white_list(white_list,file_info);}
    }
}
    else
    {
        g_print("");
    }
    gtk_widget_destroy(filew);
    if(box!=NULL)
            gtk_widget_destroy(box);
    white_list_settings(while_list_content_box);
    return 0;
}
void start_or_close_real_time_protect(GtkWidget *widget,gpointer data){
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))){
                gtk_widget_set_sensitive(choose_protect_button,TRUE);
                gtk_widget_set_sensitive(real_time_protect_is_label,TRUE);
                gtk_widget_set_sensitive(real_time_protect_dir,TRUE);
        }
        else{
                gtk_widget_set_sensitive(choose_protect_button,FALSE);
                gtk_widget_set_sensitive(real_time_protect_is_label,FALSE);
                gtk_widget_set_sensitive(real_time_protect_dir,FALSE);
        }
}
/*常规设置*/
void common_settings(GtkWidget *content_box){
        GtkWidget *common_frame,*save_common_set_image,*save_common_set_button,*common_settings_fix_up;
	char image_dir[256];
	char set_file_path[1024];
	snprintf(set_file_path,sizeof(set_file_path),"%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);


        common_settings_fix = gtk_fixed_new();
        gtk_widget_show(common_settings_fix);
        gtk_box_pack_start(GTK_BOX (content_box),common_settings_fix,FALSE,FALSE,10);

	common_frame = gtk_frame_new(_("defence-setting"));
	gtk_widget_set_size_request(common_frame,520,120);
        gtk_frame_set_label_align(GTK_FRAME(common_frame),0.03,0.5);
        gtk_widget_show(common_frame);
        gtk_fixed_put((GtkFixed *)common_settings_fix,common_frame,35,10);
	common_settings_fix_up = gtk_fixed_new();
	gtk_widget_show(common_settings_fix_up);
	gtk_container_add(GTK_CONTAINER(common_frame),common_settings_fix_up);
#if 0
        boot_start_button = gtk_check_button_new_with_label(_("boot-start"));
        gtk_widget_show(boot_start_button);
        gtk_fixed_put((GtkFixed *)common_settings_fix_up,boot_start_button,52,10);
	int is_boot_star = get_antivirus_setting_item_inttype("boot-start",set_file_path);
	if(is_boot_star==1){
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(boot_start_button),TRUE);	
	}
#endif
        aotu_usb_button = gtk_check_button_new_with_label(_("auto-check-mobile-dev"));
        gtk_widget_show(aotu_usb_button);
        gtk_fixed_put((GtkFixed *)common_settings_fix_up,aotu_usb_button,52,40);
	int is_auto_usb = get_antivirus_setting_item_inttype("auto-check-mobile-dev",set_file_path);
	if(is_auto_usb==1){
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(aotu_usb_button),TRUE);
        }
#if 0
        download_button = gtk_check_button_new_with_label(_("auto-check-download"));
        gtk_widget_show(download_button);
        gtk_fixed_put((GtkFixed *)common_settings_fix_up,download_button,52,70);
	int is_auto_check_download = get_antivirus_setting_item_inttype("auto-check-download",set_file_path);
        if(is_auto_check_download==1){
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(download_button),TRUE);
        }

#endif
	char setting_page_css[256];
        snprintf(setting_page_css,sizeof(setting_page_css),_("%ssetting-page.css"),ANTIVIRUS_CSS_DIR);
	GFile *file=g_file_new_for_path(setting_page_css);
    GdkScreen *screen;
    screen = gdk_screen_get_default();
    GtkCssProvider *cssProvider;
    cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_file(cssProvider, file, NULL);

	
        GtkWidget *anitsetfun_frame,*group,*saveantisetimage,*saveantisetbutton;
#if 0
        scan_style_setting_fix = gtk_fixed_new();
        gtk_widget_show(scan_style_setting_fix);
        anitsetfun_frame = gtk_frame_new(_("real-time-protect"));
	gtk_widget_set_size_request(anitsetfun_frame,520,120);
        gtk_frame_set_label_align(GTK_FRAME(anitsetfun_frame),0.03,0.5);
        gtk_widget_show(anitsetfun_frame);
        gtk_fixed_put((GtkFixed *)common_settings_fix,anitsetfun_frame,35,150);
        gtk_container_add(GTK_CONTAINER(anitsetfun_frame),scan_style_setting_fix);
	real_time_prtect_button = gtk_check_button_new_with_label(_("start-real-time-protect"));
        gtk_fixed_put((GtkFixed *)scan_style_setting_fix,real_time_prtect_button,52,40);
        gtk_widget_show(real_time_prtect_button);
#endif

	GtkWidget *bottom_layout = gtk_layout_new(NULL, NULL);
        gtk_widget_set_size_request(bottom_layout, 600, 80);
	gtk_widget_show(bottom_layout);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%ssetting-right.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *setting_right_image = gtk_image_new_from_file(image_dir);
	gtk_widget_show(setting_right_image);
        gtk_layout_put(GTK_LAYOUT(bottom_layout), setting_right_image, 0, 0);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%ssetting-right-mask.png"),ANTIVIRUS_IMAGE_DIR);
	GtkWidget *setting_right_mask_image = gtk_image_new_from_file(image_dir);
        gtk_widget_show(setting_right_mask_image);
        gtk_layout_put(GTK_LAYOUT(bottom_layout), setting_right_mask_image, 0, 0);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%ssave.png"),ANTIVIRUS_IMAGE_DIR);
        saveantisetimage = gtk_image_new_from_file(image_dir);
        gtk_widget_show(saveantisetimage);
        saveantisetbutton = gtk_button_new();
        gtk_widget_set_size_request(saveantisetbutton,87,39);
	gtk_widget_set_name(saveantisetbutton,"save_set_button");
        //gtk_button_set_image(GTK_BUTTON(saveantisetbutton),saveantisetimage);

        gtk_widget_show(saveantisetbutton);
        gtk_button_set_relief((GtkButton *)saveantisetbutton,GTK_RELIEF_NONE);

        g_signal_connect(G_OBJECT(saveantisetbutton),"clicked",G_CALLBACK(save_common_settings),NULL);
       // g_signal_connect(G_OBJECT(saveantisetbutton),"enter",G_CALLBACK(save_set_enter),NULL);

	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%srecovery_default.png"),ANTIVIRUS_IMAGE_DIR);
	GtkWidget *recovery_default_image = gtk_image_new_from_file(image_dir);
	gtk_widget_show(recovery_default_image);
	GtkWidget *recovery_default_button = gtk_button_new();
        gtk_button_set_relief((GtkButton *)recovery_default_button,GTK_RELIEF_NONE);
//	gtk_button_set_image(GTK_BUTTON(recovery_default_button),recovery_default_image);
	gtk_widget_set_size_request(recovery_default_button,87,39);
        gtk_widget_set_name(recovery_default_button,"recovery_default_button");
	gtk_widget_show(recovery_default_button);
//	g_signal_connect(G_OBJECT(recovery_default_button),"enter",G_CALLBACK(recovery_default_enter),NULL);
	g_signal_connect(G_OBJECT(recovery_default_button),"clicked",G_CALLBACK(common_recovery_default),NULL);


	gtk_layout_put(GTK_LAYOUT(bottom_layout), recovery_default_button, 31, 20);
        gtk_layout_put(GTK_LAYOUT(bottom_layout), saveantisetbutton, 400, 20);
        gtk_fixed_put((GtkFixed *)common_settings_fix,bottom_layout,0,310);

        int check_protect = get_antivirus_setting_item_inttype("check-real-time-protect",set_file_path);
        if(check_protect==1){
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(real_time_prtect_button),TRUE);
        }
        else{
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(real_time_prtect_button),FALSE);

        }


 gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);

}     
void anti_scan_settings(GtkWidget *content_box){
		printf("ccccccccccccccccccccc anti_scan_settings \n");
        GtkWidget *anti_deal_style_frame,*group,*timing_anti_frame,*scan_style_label,*month_day_anti_label,*group2,*anti_scan_settings_fix_up,*anti_scan_settings_fix_down;

		char set_file_path[1024];
		snprintf(set_file_path,sizeof(set_file_path),"%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);

        anti_scan_settings_fix = gtk_fixed_new();
        gtk_box_pack_start(GTK_BOX (content_box),anti_scan_settings_fix,FALSE,FALSE,10);
        gtk_widget_show(anti_scan_settings_fix);
/*病毒处理方式*/
        anti_deal_style_frame = gtk_frame_new(_("anti-deal-style"));
	 gtk_widget_set_size_request(anti_deal_style_frame,520,90);
        gtk_frame_set_label_align(GTK_FRAME(anti_deal_style_frame),0.03,0.5);
        gtk_widget_show(anti_deal_style_frame);
        gtk_fixed_put((GtkFixed *)anti_scan_settings_fix,anti_deal_style_frame,35,10);
	anti_scan_settings_fix_up = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(anti_deal_style_frame),anti_scan_settings_fix_up);
 	gtk_widget_show(anti_scan_settings_fix_up);
        manual_deal_button = gtk_radio_button_new_with_label(NULL,_("manual-deal"));
        gtk_fixed_put((GtkFixed *)anti_scan_settings_fix_up,manual_deal_button,52,10);
        gtk_widget_show(manual_deal_button);
        group = (GtkWidget *)gtk_radio_button_get_group(GTK_RADIO_BUTTON(manual_deal_button));
        intelligence_deal_button = gtk_radio_button_new_with_label((GSList *)group,_("intelligence-deal"));
        isolation_region_button = gtk_check_button_new_with_label(_("file inot devide"));
        gtk_fixed_put((GtkFixed *)anti_scan_settings_fix_up,isolation_region_button,180,40);
        gtk_widget_show(isolation_region_button);
        gtk_widget_set_sensitive(isolation_region_button,FALSE);
        g_signal_connect(G_OBJECT(intelligence_deal_button),"toggled",G_CALLBACK(start_isolation_region),isolation_region_button);
        gtk_fixed_put((GtkFixed *)anti_scan_settings_fix_up,intelligence_deal_button,52,40);
        gtk_widget_show(intelligence_deal_button);

        int deal_scan = get_antivirus_setting_item_inttype("anti-deal-style",set_file_path);
        if(deal_scan==1){
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(manual_deal_button),TRUE);
        }
        if(deal_scan==2){
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(intelligence_deal_button),TRUE);
        }
        int file_devide = get_antivirus_setting_item_inttype("file-into-deivide",set_file_path);

	if(file_devide==1){
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(isolation_region_button),TRUE);
	}

	timing_anti_frame = gtk_frame_new(_("timing-anti"));
	gtk_widget_set_size_request(timing_anti_frame,520,160);
        gtk_frame_set_label_align(GTK_FRAME(timing_anti_frame),0.03,0.5);

        gtk_fixed_put((GtkFixed *)anti_scan_settings_fix,timing_anti_frame,35,120);
        gtk_widget_show(timing_anti_frame);
	anti_scan_settings_fix_down = gtk_fixed_new();
        gtk_container_add(GTK_CONTAINER(timing_anti_frame),anti_scan_settings_fix_down);
        gtk_widget_show(anti_scan_settings_fix_down);

        timing_anti_button = gtk_check_button_new_with_label(_("start-timing-anti"));
        g_signal_connect(G_OBJECT(timing_anti_button),"toggled",G_CALLBACK(start_timing_anti),NULL);
        gtk_fixed_put((GtkFixed *)anti_scan_settings_fix_down,timing_anti_button,52,10);
        gtk_widget_show(timing_anti_button);
        scan_style_label = gtk_label_new(_("anti-scan-style"));
        gtk_fixed_put((GtkFixed *)anti_scan_settings_fix_down,scan_style_label,180,10);
        gtk_widget_show(scan_style_label);
        anti_scan_combo = gtk_combo_box_text_new();
        gtk_combo_box_text_append_text((GtkComboBoxText *)anti_scan_combo, _("all-scan"));
        gtk_combo_box_text_append_text((GtkComboBoxText *)anti_scan_combo, _("quick-scan"));
        gtk_combo_box_set_active((GtkComboBox *)anti_scan_combo, 1);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(anti_scan_combo),GTK_SENSITIVITY_OFF);
        gtk_widget_show(anti_scan_combo);
        gtk_fixed_put((GtkFixed *)anti_scan_settings_fix_down,anti_scan_combo,300,10);
	/*每天*/
	month_anti_button = gtk_radio_button_new_with_label(NULL,_("each-month"));
        g_signal_connect(G_OBJECT(month_anti_button),"clicked",G_CALLBACK(choose_anti_month),NULL);
        gtk_widget_show(month_anti_button);
        gtk_fixed_put(GTK_FIXED(anti_scan_settings_fix_down),month_anti_button, 52, 100);
        month_anti_day_combo = gtk_combo_box_text_new();

		printf("fffffffffffffffffff month \n");
        int i = 1;
        while(i<32)
        {
                char a[50];
                snprintf(a,sizeof(a),"%d",i) ;
                gtk_combo_box_text_append_text((GtkComboBoxText *)month_anti_day_combo, a);
                i++;
        }
        gtk_combo_box_set_active(GTK_COMBO_BOX(month_anti_day_combo), 15);
        gtk_widget_show((GtkWidget *)month_anti_day_combo);
        gtk_fixed_put(GTK_FIXED(anti_scan_settings_fix_down),(GtkWidget *)month_anti_day_combo, 100, 100);
        month_day_anti_label = gtk_label_new(_("day"));
        gtk_widget_show(month_day_anti_label);
        gtk_fixed_put(GTK_FIXED(anti_scan_settings_fix_down),month_day_anti_label, 150, 100);
        month_anti_hour_combo = gtk_combo_box_text_new();
        gtk_widget_show((GtkWidget *)month_anti_hour_combo);
        i = 0;
        while(i<24)
        {
                char a[30];
                snprintf(a,sizeof(a),"%d",i) ;
                gtk_combo_box_text_append_text((GtkComboBoxText *)month_anti_hour_combo, a);
                i++;
        }
        gtk_combo_box_set_active(GTK_COMBO_BOX(month_anti_hour_combo), 12);
        gtk_fixed_put(GTK_FIXED(anti_scan_settings_fix_down),(GtkWidget *)month_anti_hour_combo, 200, 100);
        GtkWidget *month_anit_hour_label = gtk_label_new(_("hour"));
        gtk_widget_show(month_anit_hour_label);
        gtk_fixed_put(GTK_FIXED(anti_scan_settings_fix_down),month_anit_hour_label, 250, 105);
        month_anti_min_combo = gtk_combo_box_text_new();
        gtk_widget_show((GtkWidget *)month_anti_min_combo);
        i = 0;
        while(i<60)
        {
                char a[30];
                snprintf(a,sizeof(a),"%d",i) ;
                gtk_combo_box_text_append_text((GtkComboBoxText *)month_anti_min_combo, a);
                i++;
        }

		printf("wwwwwwwwwwwwwwwwwwww \n");
        gtk_combo_box_set_active(GTK_COMBO_BOX(month_anti_min_combo), 30);
        gtk_fixed_put(GTK_FIXED(anti_scan_settings_fix_down),(GtkWidget *)month_anti_min_combo, 300, 100);
        GtkWidget *month_anit_min_label = gtk_label_new(_("min"));
        gtk_widget_show(month_anit_min_label);
        gtk_fixed_put(GTK_FIXED(anti_scan_settings_fix_down),month_anit_min_label, 350, 105);
/*每月结束*/
        group2 = (GtkWidget *)gtk_radio_button_get_group(GTK_RADIO_BUTTON(month_anti_button));
/*每周*/
        week_anit_button = gtk_radio_button_new_with_label((GSList *)group2,_("each-week"));
        g_signal_connect(G_OBJECT(week_anit_button),"clicked",G_CALLBACK(choose_week_anti),NULL);
        gtk_widget_show(week_anit_button);
        gtk_widget_set_sensitive(month_anti_button,FALSE);
        gtk_widget_set_sensitive(week_anit_button,FALSE);
        gtk_fixed_put(GTK_FIXED(anti_scan_settings_fix_down),week_anit_button, 52, 70);
        week_anit_week_combo =  gtk_combo_box_text_new();
        gtk_combo_box_text_append_text((GtkComboBoxText *)week_anit_week_combo, _("MON"));
        gtk_combo_box_text_append_text((GtkComboBoxText *)week_anit_week_combo, _("TUE"));
        gtk_combo_box_text_append_text((GtkComboBoxText *)week_anit_week_combo, _("WED"));
        gtk_combo_box_text_append_text((GtkComboBoxText *)week_anit_week_combo, _("THU"));
        gtk_combo_box_text_append_text((GtkComboBoxText *)week_anit_week_combo, _("FRI"));
        gtk_combo_box_text_append_text((GtkComboBoxText *)week_anit_week_combo, _("SAT"));
        gtk_combo_box_text_append_text((GtkComboBoxText *)week_anit_week_combo, _("SUN"));
        gtk_combo_box_set_active(GTK_COMBO_BOX(week_anit_week_combo), 0);
        gtk_fixed_put(GTK_FIXED(anti_scan_settings_fix_down),week_anit_week_combo, 100, 70);
        gtk_widget_show(week_anit_week_combo);
        GtkWidget *week_anit_week_label = gtk_label_new(_("week"));
        gtk_widget_show(week_anit_week_label);
        gtk_fixed_put(GTK_FIXED(anti_scan_settings_fix_down),week_anit_week_label, 150, 75);
        week_anit_hour_combo = gtk_combo_box_text_new();
        i = 0;
        while(i<24)
        {
                char a[65];
                snprintf(a,sizeof(a),"%d",i);
                gtk_combo_box_text_append_text((GtkComboBoxText *)week_anit_hour_combo, a);
                i++;
        }

		printf("ppppppppppppppppppppppppp \n");
        gtk_widget_show((GtkWidget *)week_anit_hour_combo);
        gtk_combo_box_set_active(GTK_COMBO_BOX(week_anit_hour_combo), 12);
        gtk_fixed_put(GTK_FIXED(anti_scan_settings_fix_down),week_anit_hour_combo, 200, 70);
        GtkWidget *week_anit_hour_label = gtk_label_new(_("hour"));
        gtk_widget_show(week_anit_hour_label);
        gtk_fixed_put(GTK_FIXED(anti_scan_settings_fix_down),(GtkWidget *)week_anit_hour_label,250,75);
        week_anit_min_combo = gtk_combo_box_text_new();
        gtk_widget_show((GtkWidget *)week_anit_min_combo);
        i = 0;
        while(i<60)
        {
                char a[65];
                snprintf(a,sizeof(a),"%d",i);
                gtk_combo_box_text_append_text((GtkComboBoxText *)week_anit_min_combo, a);
                i++;
        }
        gtk_combo_box_set_active(GTK_COMBO_BOX(week_anit_min_combo), 30);
        gtk_fixed_put(GTK_FIXED(anti_scan_settings_fix_down),(GtkWidget *)week_anit_min_combo,300,70);
        GtkWidget *week_anit_min_label = gtk_label_new(_("min"));
        gtk_widget_show(week_anit_min_label);
        gtk_fixed_put(GTK_FIXED(anti_scan_settings_fix_down),(GtkWidget *)week_anit_min_label,350,75);

        gtk_fixed_put(GTK_FIXED(anti_scan_settings_fix_down),(GtkWidget *)week_anit_min_label,350,75);
/*每周结束每天开始*/
        day_anit_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(week_anit_button),_("eachday"));
        g_signal_connect(G_OBJECT(day_anit_button),"clicked",G_CALLBACK(choose_anti_day),NULL);
        gtk_fixed_put(GTK_FIXED(anti_scan_settings_fix_down),(GtkWidget *)day_anit_button,52,40);
        gtk_widget_show(day_anit_button);
        gtk_widget_set_sensitive(day_anit_button,FALSE);
        day_anti_hour_combo = gtk_combo_box_text_new();
        i = 0;
        while(i<24)
        {
                char a[65];
                snprintf(a,sizeof(a),"%d",i);
                gtk_combo_box_text_append_text((GtkComboBoxText *)day_anti_hour_combo, a);
                i++;
        }

		printf("iiiiiiiiiiiiiiiiiiiiiiiiiii \n");

        gtk_widget_show((GtkWidget *)day_anti_hour_combo);
        gtk_combo_box_set_active(GTK_COMBO_BOX(day_anti_hour_combo), 12);
        gtk_fixed_put(GTK_FIXED(anti_scan_settings_fix_down),(GtkWidget *)day_anti_hour_combo,100,40);
        GtkWidget *day_anit_hour_label = gtk_label_new(_("hour"));
        gtk_widget_show(day_anit_hour_label);
        gtk_fixed_put(GTK_FIXED(anti_scan_settings_fix_down),(GtkWidget *)day_anit_hour_label,150,45);
        day_anit_min_combo = gtk_combo_box_text_new();
        gtk_widget_show((GtkWidget *)day_anit_min_combo);
        i = 0;
        while(i<60)
        {
                char a[30];
                snprintf(a,sizeof(a),"%d",i) ;
                gtk_combo_box_text_append_text((GtkComboBoxText *)day_anit_min_combo, a);
                i++;
        }
        gtk_combo_box_set_active(GTK_COMBO_BOX(day_anit_min_combo), 30);
        gtk_fixed_put(GTK_FIXED(anti_scan_settings_fix_down),(GtkWidget *)day_anit_min_combo, 200, 40);
        GtkWidget *day_anit_min_label = gtk_label_new(_("min"));
        gtk_widget_show(day_anit_min_label);
        gtk_fixed_put(GTK_FIXED(anti_scan_settings_fix_down),day_anit_min_label, 250, 45);

        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(week_anit_week_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(week_anit_hour_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(week_anit_min_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(month_anti_day_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(month_anti_hour_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(month_anti_min_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(day_anti_hour_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(day_anit_min_combo),GTK_SENSITIVITY_OFF);
	
        printf("uuuuuuuuuuuuuuuuuuu bottom_layout\n");

	GtkWidget *bottom_layout = gtk_layout_new(NULL, NULL);
        gtk_widget_set_size_request(bottom_layout, 600, 80);
	gtk_widget_show(bottom_layout);
	char image_dir[256];
        snprintf(image_dir,sizeof(image_dir),_("%ssetting-right.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *setting_right_image = gtk_image_new_from_file(image_dir);
        gtk_layout_put(GTK_LAYOUT(bottom_layout), setting_right_image, 0, 0);
	gtk_widget_show(setting_right_image);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%ssetting-right-mask.png"),ANTIVIRUS_IMAGE_DIR);
	GtkWidget *setting_right_mask_image = gtk_image_new_from_file(image_dir);
        gtk_widget_show(setting_right_mask_image);
        gtk_layout_put(GTK_LAYOUT(bottom_layout), setting_right_mask_image, 0, 0);
	
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%ssave.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *save_scan_anit_image = gtk_image_new_from_file(image_dir);
        gtk_widget_show(save_scan_anit_image);
        GtkWidget *save_scan_anti_button = gtk_button_new();
	gtk_widget_set_size_request(save_scan_anti_button,87,39);
        gtk_widget_set_name(save_scan_anti_button,"save_set_button");
        gtk_button_set_image(GTK_BUTTON(save_scan_anti_button),save_scan_anit_image);
        gtk_widget_show(save_scan_anti_button);
        gtk_button_set_relief((GtkButton *)save_scan_anti_button,GTK_RELIEF_NONE);
        g_signal_connect(G_OBJECT(save_scan_anti_button),"clicked",G_CALLBACK(save_scan_anti),NULL);
        g_signal_connect(G_OBJECT(save_scan_anti_button),"enter",G_CALLBACK(save_set_enter),NULL);


		printf("bbbbbbbbbbbbbbbbbbbbbbbbbb dir \n");
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%srecovery_default.png"),ANTIVIRUS_IMAGE_DIR);
	GtkWidget *recovery_default_image = gtk_image_new_from_file(image_dir);
        gtk_widget_show(recovery_default_image);
        GtkWidget *recovery_default_button = gtk_button_new();
	 gtk_widget_set_size_request(recovery_default_button,87,39);
        gtk_widget_set_name(recovery_default_button,"recovery_default_button");

        gtk_button_set_image(GTK_BUTTON(recovery_default_button),recovery_default_image);
        gtk_widget_show(recovery_default_button);
        gtk_button_set_relief((GtkButton *)recovery_default_button,GTK_RELIEF_NONE);
        g_signal_connect(G_OBJECT(recovery_default_button),"clicked",G_CALLBACK(anti_scan_recovery_default),NULL);
        g_signal_connect(G_OBJECT(recovery_default_button),"enter",G_CALLBACK(recovery_default_enter),NULL);

	gtk_layout_put(GTK_LAYOUT(bottom_layout), recovery_default_button, 31, 20);
        gtk_layout_put(GTK_LAYOUT(bottom_layout), save_scan_anti_button, 400, 20);
	gtk_fixed_put(GTK_FIXED(anti_scan_settings_fix),bottom_layout, 0, 310);


	   int start_timing_anti = get_antivirus_setting_item_inttype("start-timing-anti",set_file_path);
        if(start_timing_anti==1){
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(timing_anti_button),TRUE);
        }
        int timing_scan_style = get_antivirus_setting_item_inttype("timing-scan-style",set_file_path);
        gtk_combo_box_set_active((GtkComboBox *)anti_scan_combo, timing_scan_style);
        int timing_scan_select = get_antivirus_setting_item_inttype("timing-scan-select",set_file_path);
        if(timing_scan_select==1){
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(month_anti_button),TRUE);
        }
        if(timing_scan_select==2){
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(week_anit_button),TRUE);
        }
        if(timing_scan_select==3){
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(day_anit_button),TRUE);
        }

		printf("lllllllllllllllllllllll month \n");
        char month[20];
        get_antivirus_setting_item_stringtype("timing-scan-month",month,set_file_path);
        int in = 0;
		printf("month is :%s\n",month);
        char *timming_month[10];
		if(strcmp(month,"") != 0 )
		{
            timming_month[in] = strtok(month,",");
            while(timming_month[in])
            {
                        in++;
                        timming_month[in] = strtok(NULL,",");

            }
		}
		printf("strtok dddddddddddffffffffffffffffffffff\n");
        int month_anti_day = atoi(timming_month[0]);
        gtk_combo_box_set_active(GTK_COMBO_BOX(month_anti_day_combo),month_anti_day-1);
        int month_anti_hour = atoi(timming_month[1]);
        gtk_combo_box_set_active(GTK_COMBO_BOX(month_anti_hour_combo),month_anti_hour);
        int month_anti_min = atoi(timming_month[2]);
        gtk_combo_box_set_active(GTK_COMBO_BOX(month_anti_min_combo),month_anti_min);
        char week[20];
        get_antivirus_setting_item_stringtype("timing-scan-week",week,set_file_path);
		printf("week is :%s\n",week);
        in = 0;
        char *timming_week[10];
		if(strcmp(week,"") != 0)
		{
            timming_week[in] = strtok(week,",");

            while(timming_week[in])
            {
                        in++;
                        timming_week[in] = strtok(NULL,",");

            }
		}

		printf("ggggggggggggggggggggggggg strok\n");

        int week_anti_day = atoi(timming_week[0]);
        gtk_combo_box_set_active(GTK_COMBO_BOX(week_anit_week_combo),week_anti_day);        int week_anti_hour = atoi(timming_week[1]);
        gtk_combo_box_set_active(GTK_COMBO_BOX(week_anit_hour_combo),week_anti_hour);
        int week_anti_min = atoi(timming_week[2]);
        gtk_combo_box_set_active(GTK_COMBO_BOX(week_anit_min_combo),week_anti_min);
        char day[20];
        get_antivirus_setting_item_stringtype("timing-scan-day",day,set_file_path);
        in = 0;

		printf("timming ddddddddddddddddddddddddd\n");

        char *timming_day[10] ;
		if(strcmp(day,"") != 0)
		{
            timming_day[in] = strtok(day,",");
            while(timming_day[in])
            {
                        in++;
                        timming_day[in] = strtok(NULL,",");

            }
		}
        int day_anti_hour ;
        if(timming_day[0] != NULL)
        {
            day_anti_hour = atoi(timming_day[0]);

        }
        gtk_combo_box_set_active(GTK_COMBO_BOX(day_anti_hour_combo),day_anti_hour);
        int day_anti_min;
        if(timming_day[1] != NULL)
        {
             day_anti_min = atoi(timming_day[1]);
        }
        gtk_combo_box_set_active(GTK_COMBO_BOX(day_anit_min_combo),day_anti_min);

}
/*白名单*/
void white_list_settings(GtkWidget *content_box){
        GtkWidget *white_list_fix = gtk_fixed_new();
        GtkWidget *white_list_up_frame=gtk_frame_new(NULL);
        gtk_widget_set_size_request(white_list_up_frame,550,300);
        gtk_fixed_put((GtkFixed *)white_list_fix,white_list_up_frame,20,10);
        GtkWidget *white_list_content_fix = gtk_fixed_new();
        GtkListStore *list_store;
        GtkWidget *button_box,*add_file_button,*add_path_button,*button_fix,*remove_button,*add_regular_button;
        box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_box_pack_start(GTK_BOX (content_box),box,FALSE,FALSE,0);
        white_list_sw = gtk_scrolled_window_new(NULL, NULL);
        gtk_widget_set_size_request(white_list_sw,550,242);
        gtk_scrolled_window_set_min_content_height((GtkScrolledWindow *)white_list_sw,242);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(white_list_sw), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
        gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(white_list_sw), GTK_SHADOW_ETCHED_IN);
        white_list_list = gtk_tree_view_new_with_model(create_white_list_model());
        white_list_render_tog = gtk_cell_renderer_toggle_new();
        g_object_set(G_OBJECT(white_list_render_tog),"activatable",TRUE,NULL);
        white_list_column_tog = gtk_tree_view_column_new_with_attributes("      ",white_list_render_tog,"active",WHITE_LIST_TOG_COL,NULL);
	
	gtk_tree_view_column_set_resizable(white_list_column_tog,TRUE);
        gtk_tree_view_column_set_sizing (white_list_column_tog,GTK_TREE_VIEW_COLUMN_FIXED);
        gtk_tree_view_column_set_fixed_width(white_list_column_tog,50);

        gtk_tree_view_append_column(GTK_TREE_VIEW(white_list_list),white_list_column_tog);
        g_signal_connect (white_list_render_tog, "toggled", G_CALLBACK (select_toggled),white_list_list);
        white_list_render = gtk_cell_renderer_text_new();
        white_list_column = gtk_tree_view_column_new_with_attributes(_("file"),white_list_render, "text", WHITE_LIST_FILE_COL, NULL);

	gtk_tree_view_column_set_resizable(white_list_column,TRUE);
        gtk_tree_view_column_set_sizing (white_list_column,GTK_TREE_VIEW_COLUMN_FIXED);
        gtk_tree_view_column_set_fixed_width(white_list_column,330);

        gtk_tree_view_append_column(GTK_TREE_VIEW(white_list_list), white_list_column);
        white_list_render = gtk_cell_renderer_text_new();
        white_list_column = gtk_tree_view_column_new_with_attributes(_("status"),white_list_render, "text", WHITE_LIST_STATUS_COL, NULL);

	gtk_tree_view_column_set_resizable(white_list_column,TRUE);
        gtk_tree_view_column_set_sizing (white_list_column,GTK_TREE_VIEW_COLUMN_FIXED);
        gtk_tree_view_column_set_fixed_width(white_list_column,100);

        gtk_tree_view_append_column(GTK_TREE_VIEW(white_list_list), white_list_column);
        white_selection=gtk_tree_view_get_selection((GTK_TREE_VIEW(white_list_list)));
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (white_list_list),FALSE);
        gtk_tree_selection_set_mode(white_selection,GTK_SELECTION_SINGLE);
	gtk_tree_view_set_tooltip_column(GTK_TREE_VIEW(white_list_list), WHITE_LIST_FILE_COL);
        gtk_container_add(GTK_CONTAINER(white_list_sw), white_list_list);

	GtkWidget *white_list_title;
	char image_dir[256];
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%swhite_list_title_bg.png"),ANTIVIRUS_IMAGE_DIR);
        white_list_title=gtk_image_new_from_file(image_dir);
	gtk_widget_show(white_list_title);
	gtk_fixed_put((GtkFixed *)white_list_content_fix,white_list_title,0,0);
        gtk_fixed_put((GtkFixed *)white_list_content_fix,white_list_sw,0,28);
        gtk_container_add(GTK_CONTAINER(white_list_up_frame), white_list_content_fix);
        gtk_box_pack_start(GTK_BOX (box),white_list_fix,FALSE,FALSE,0);

        GtkWidget *bottom_layout = gtk_layout_new(NULL, NULL);
        gtk_widget_set_size_request(bottom_layout, 600, 80);
        gtk_box_pack_start(GTK_BOX (box),bottom_layout,FALSE,FALSE,10);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%ssetting-right.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *setting_right_image = gtk_image_new_from_file(image_dir);
        gtk_layout_put(GTK_LAYOUT(bottom_layout), setting_right_image, 0, 0);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%ssetting-right-mask.png"),ANTIVIRUS_IMAGE_DIR);
	GtkWidget *setting_right_mask_image = gtk_image_new_from_file(image_dir);
        gtk_widget_show(setting_right_mask_image);
        gtk_layout_put(GTK_LAYOUT(bottom_layout), setting_right_mask_image, 0, 0);

        select_all_button = gtk_check_button_new_with_label(_("select all"));
        g_signal_connect (select_all_button, "toggled", G_CALLBACK (select_all),white_list_list);
        gtk_fixed_put((GtkFixed *)white_list_content_fix,select_all_button,0,273);
        add_file_button = gtk_button_new();
        gtk_button_set_relief((GtkButton *)add_file_button,GTK_RELIEF_NONE);
	gtk_widget_set_size_request(add_file_button,87,39);
	gtk_widget_set_name(add_file_button,"add_file_button");
	
	 memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%saddfile.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *add_file_image = gtk_image_new_from_file(image_dir);
       // gtk_button_set_image(add_file_button,add_file_image);
        gtk_layout_put(GTK_LAYOUT(bottom_layout), add_file_button, 120, 20);
        add_path_button = gtk_button_new();
        gtk_button_set_relief((GtkButton *)add_path_button,GTK_RELIEF_NONE);
	gtk_widget_set_size_request(add_path_button,87,39);
        gtk_widget_set_name(add_path_button,"add_path_button");
 	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sadddire.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *add_path_image = gtk_image_new_from_file(image_dir);
       // gtk_button_set_image(add_path_button,add_path_image);
	gtk_layout_put(GTK_LAYOUT(bottom_layout), add_path_button, 220, 20);
        remove_button = gtk_button_new();
        gtk_button_set_relief((GtkButton *)remove_button,GTK_RELIEF_NONE);
	gtk_widget_set_size_request(remove_button,87,39);
        gtk_widget_set_name(remove_button,"remove_button");
 	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sremove.png"),ANTIVIRUS_IMAGE_DIR);
	GtkWidget *removeimage = gtk_image_new_from_file(image_dir);
       // gtk_button_set_image(remove_button,removeimage);
        gtk_layout_put(GTK_LAYOUT(bottom_layout), remove_button, 320, 20);

        add_regular_button = gtk_button_new();
 	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sadd_reguar.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *addimage=gtk_image_new_from_file(image_dir);
        gtk_button_set_relief((GtkButton *)add_regular_button,GTK_RELIEF_NONE);
	gtk_widget_set_size_request(add_regular_button,87,39);
        gtk_widget_set_name(add_regular_button,"add_regular_button");
       // gtk_button_set_image(add_regular_button,addimage);
        gtk_layout_put(GTK_LAYOUT(bottom_layout), add_regular_button, 420, 20);
        g_signal_connect(G_OBJECT(add_regular_button),"clicked",G_CALLBACK(add_regular_dialog),NULL);
       // g_signal_connect(G_OBJECT(add_regular_button),"enter",G_CALLBACK(enter_add_regular),NULL);

        g_signal_connect(G_OBJECT(remove_button),"clicked",G_CALLBACK(remove_white_list),content_box);
       // g_signal_connect(G_OBJECT(remove_button),"enter",G_CALLBACK(enter_remove_button),NULL);

        g_signal_connect(G_OBJECT(add_file_button),"clicked",G_CALLBACK(add_file),content_box);
      //  g_signal_connect(G_OBJECT(add_file_button),"enter",G_CALLBACK(enter_add_file),NULL);

        g_signal_connect(G_OBJECT(add_path_button),"clicked",G_CALLBACK(add_path),content_box);
        //g_signal_connect(G_OBJECT(add_path_button),"enter",G_CALLBACK(enter_add_path),NULL);

        GtkTreeIter iter;
        GtkTreeModel *model= gtk_tree_view_get_model (GTK_TREE_VIEW(white_list_list));
        if(!gtk_tree_model_get_iter_first(model,&iter)){
                if(removeimage!=NULL){
                        gtk_widget_destroy(removeimage);
			 memset(image_dir,0,sizeof(image_dir));
		        snprintf(image_dir,sizeof(image_dir),_("%sremove-un-use.png"),ANTIVIRUS_IMAGE_DIR);
                        removeimage = gtk_image_new_from_file(image_dir);
		//	gtk_button_set_image(remove_button,removeimage);
                }
                gtk_widget_set_sensitive(remove_button,FALSE);
        }

        gtk_widget_show_all(content_box);
		
		printf("xxxxxxxxxxxxxxxxxxxx\n");

}

void update_settings(GtkWidget *content_box){
	GtkWidget *updateset_frame,*update_iplabel,*timingbox1,*group,*fix2,*fix3,*group2,*saveantisetimage,*saveantisetbutton,*update_settings_fix_detail;
	    char set_file_path[1024];
		snprintf(set_file_path,sizeof(set_file_path),"%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);

        update_settings_fix = gtk_fixed_new();
        gtk_box_pack_start(GTK_BOX (content_box),update_settings_fix,FALSE,FALSE,10);
        gtk_widget_show(update_settings_fix);
        updateset_frame = gtk_frame_new(_("update-set"));
	gtk_widget_set_size_request(updateset_frame,520,260);
        gtk_frame_set_label_align(GTK_FRAME(updateset_frame),0.03,0.5);

        gtk_widget_show(updateset_frame);
        gtk_fixed_put(GTK_FIXED(update_settings_fix), updateset_frame, 35, 10);
        update_settings_fix_detail = gtk_fixed_new();
        gtk_widget_show(update_settings_fix_detail);
        gtk_container_add(GTK_CONTAINER(updateset_frame),update_settings_fix_detail);

        timingbutton = (GtkComboBoxText *)gtk_radio_button_new_with_label(NULL,_("manual-update"));
        gtk_widget_set_size_request((GtkWidget *)timingbutton, 100,20);
        g_signal_connect(G_OBJECT(timingbutton),"clicked",G_CALLBACK(choose_manual),NULL);
        gtk_widget_show((GtkWidget *)timingbutton);
        gtk_fixed_put(GTK_FIXED(update_settings_fix_detail), (GtkWidget *)timingbutton, 52, 10);
        group = (GtkWidget *)gtk_radio_button_get_group(GTK_RADIO_BUTTON(timingbutton));
        periodbuton = (GtkComboBoxText *)gtk_radio_button_new_with_label((GSList *)group,_("automatic-update"));
        gtk_widget_set_size_request((GtkWidget *)periodbuton, 100,20);
        gtk_widget_show((GtkWidget *)periodbuton);
        g_signal_connect(G_OBJECT(periodbuton),"clicked",G_CALLBACK(choose_auto),NULL);
        gtk_fixed_put(GTK_FIXED(update_settings_fix_detail), (GtkWidget *)periodbuton, 52, 60);
	/************************每天********************************/
	month_update_button = gtk_radio_button_new_with_label(NULL,_("each-month"));

        g_signal_connect(G_OBJECT(month_update_button),"clicked",G_CALLBACK(choose_month),NULL);
        gtk_widget_show(month_update_button);
        gtk_fixed_put(GTK_FIXED(update_settings_fix_detail),month_update_button, 52, 180);
	int i = 1;
        Combo_Box_week_week = (GtkComboBoxText *)gtk_combo_box_text_new();
        Combo_Box_week_hour = (GtkComboBoxText *)gtk_combo_box_text_new();
        gtk_combo_box_text_append_text((GtkComboBoxText *)Combo_Box_week_week, _("MON"));
        gtk_combo_box_text_append_text((GtkComboBoxText *)Combo_Box_week_week, _("TUE"));
        gtk_combo_box_text_append_text((GtkComboBoxText *)Combo_Box_week_week, _("WED"));
        gtk_combo_box_text_append_text((GtkComboBoxText *)Combo_Box_week_week, _("THU"));
        gtk_combo_box_text_append_text((GtkComboBoxText *)Combo_Box_week_week, _("FRI"));
        gtk_combo_box_text_append_text((GtkComboBoxText *)Combo_Box_week_week, _("SAT"));
        gtk_combo_box_text_append_text((GtkComboBoxText *)Combo_Box_week_week, _("SUN"));
        gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_week_week), 0);
        GtkWidget *week_week_label = gtk_label_new(_("week"));
        gtk_widget_show(week_week_label);
        i = 0;
        while(i<24)
        {
                char a[65];
                snprintf(a,sizeof(a),"%d",i);
                gtk_combo_box_text_append_text((GtkComboBoxText *)Combo_Box_week_hour, a);
                i++;
        }
	 gtk_widget_show((GtkWidget *)Combo_Box_week_hour);
        gtk_widget_show((GtkWidget *)Combo_Box_week_week);
        gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_week_hour), 12);
        GtkWidget *week_hour_label = gtk_label_new(_("hour"));
        gtk_widget_show(week_hour_label);
        gtk_fixed_put(GTK_FIXED(update_settings_fix_detail),(GtkWidget *)Combo_Box_week_week,100,140);
        gtk_fixed_put(GTK_FIXED(update_settings_fix_detail),week_week_label,150,145);
        gtk_fixed_put(GTK_FIXED(update_settings_fix_detail),(GtkWidget *)Combo_Box_week_hour,200,140);
        gtk_fixed_put(GTK_FIXED(update_settings_fix_detail),week_hour_label,250,145);
        Combo_Box_week_min = (GtkComboBoxText *)gtk_combo_box_text_new();
        gtk_widget_show((GtkWidget *)Combo_Box_week_min);
	 i = 0;
        while(i<60)
        {
                char a[65];
                snprintf(a,sizeof(a),"%d",i);
                gtk_combo_box_text_append_text((GtkComboBoxText *)Combo_Box_week_min, a);
                i++;
        }
	
	gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_week_min), 30);
        GtkWidget *week_min_label = gtk_label_new(_("min"));
        gtk_widget_show(week_min_label);
        gtk_fixed_put(GTK_FIXED(update_settings_fix_detail),(GtkWidget *)Combo_Box_week_min,300,140);
        gtk_fixed_put(GTK_FIXED(update_settings_fix_detail),week_min_label,350,145);
	/***********************每周结束****************/
        group2 = (GtkWidget *)gtk_radio_button_get_group(GTK_RADIO_BUTTON(month_update_button));
        week_update_button = gtk_radio_button_new_with_label((GSList *)group2,_("each-week"));
        g_signal_connect(G_OBJECT(week_update_button),"clicked",G_CALLBACK(choose_week),NULL);
        gtk_widget_show(week_update_button);
        gtk_widget_set_sensitive(month_update_button,FALSE);
        gtk_widget_set_sensitive(week_update_button,FALSE);
        gtk_fixed_put(GTK_FIXED(update_settings_fix_detail),week_update_button, 52, 140);
        /*******************************每月***************************/

	Combo_Box_month_day = (GtkComboBoxText *)gtk_combo_box_text_new();
        i = 1;
        while(i<32)
        {
                char a[50];
                snprintf(a,sizeof(a),"%d",i) ;
                gtk_combo_box_text_append_text((GtkComboBoxText *)Combo_Box_month_day, a);
                i++;
        }
        gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_month_day), 15);
        gtk_widget_show((GtkWidget *)Combo_Box_month_day);
        gtk_fixed_put(GTK_FIXED(update_settings_fix_detail),(GtkWidget *)Combo_Box_month_day, 100, 180);
        GtkWidget *month_day_label = gtk_label_new(_("day"));
        gtk_widget_show(month_day_label);
        gtk_fixed_put(GTK_FIXED(update_settings_fix_detail),month_day_label, 150, 185);
        Combo_Box_month_hour = (GtkComboBoxText *)gtk_combo_box_text_new();
        gtk_widget_show((GtkWidget *)Combo_Box_month_hour);
        i = 0;
        while(i<24)
        {
                char a[30];
                snprintf(a,sizeof(a),"%d",i) ;
                gtk_combo_box_text_append_text((GtkComboBoxText *)Combo_Box_month_hour, a);
                i++;
        }
        gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_month_hour), 12);
        gtk_fixed_put(GTK_FIXED(update_settings_fix_detail),(GtkWidget *)Combo_Box_month_hour, 200, 180);
        GtkWidget *month_hour_label = gtk_label_new(_("hour"));
        gtk_widget_show(month_hour_label);
        gtk_fixed_put(GTK_FIXED(update_settings_fix_detail),month_hour_label, 250, 185);
        Combo_Box_month_min = (GtkComboBoxText *)gtk_combo_box_text_new();
        gtk_widget_show((GtkWidget *)Combo_Box_month_min);
	i = 0;
        while(i<60)
        {
                char a[30];
                snprintf(a,sizeof(a),"%d",i) ;
                gtk_combo_box_text_append_text((GtkComboBoxText *)Combo_Box_month_min, a);
                i++;
        }
        gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_month_min), 30);
        gtk_fixed_put(GTK_FIXED(update_settings_fix_detail),(GtkWidget *)Combo_Box_month_min, 300, 180);
        GtkWidget *month_min_label = gtk_label_new(_("min"));
        gtk_widget_show(month_min_label);
        gtk_fixed_put(GTK_FIXED(update_settings_fix_detail),month_min_label, 350, 185);
/**************************每月结束************************/
	day_update_button=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(week_update_button),_("eachday"));
        g_signal_connect(G_OBJECT(day_update_button),"clicked",G_CALLBACK(choose_day),NULL);
        gtk_widget_show(day_update_button);
        gtk_fixed_put(GTK_FIXED(update_settings_fix_detail),day_update_button, 52, 100);

         gtk_widget_set_sensitive(day_update_button,FALSE);
        Combo_Box_day_hour = (GtkComboBoxText *)gtk_combo_box_text_new();
        i = 0;
        while(i<24)
        {
                char a[65];
                snprintf(a,sizeof(a),"%d",i);
                gtk_combo_box_text_append_text((GtkComboBoxText *)Combo_Box_day_hour, a);
                i++;
        }

        gtk_widget_show((GtkWidget *)Combo_Box_day_hour);
        gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_day_hour), 12);
        GtkWidget *day_hour_label = gtk_label_new(_("hour"));
        gtk_widget_show(day_hour_label);
        gtk_fixed_put(GTK_FIXED(update_settings_fix_detail),(GtkWidget *)Combo_Box_day_hour,100,100);
        gtk_fixed_put(GTK_FIXED(update_settings_fix_detail),day_hour_label,150,105);
/***********************************/
        Combo_Box_day_min = (GtkComboBoxText *)gtk_combo_box_text_new();
        gtk_widget_show((GtkWidget *)Combo_Box_day_min);
        i = 0;
        while(i<60)
        {
                char a[30];
                snprintf(a,sizeof(a),"%d",i) ;
                gtk_combo_box_text_append_text((GtkComboBoxText *)Combo_Box_day_min, a);
                i++;
        
	}
	gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_day_min), 30);
        gtk_fixed_put(GTK_FIXED(update_settings_fix_detail),(GtkWidget *)Combo_Box_day_min, 200, 100);
        GtkWidget *day_min_label = gtk_label_new(_("min"));
        gtk_widget_show(day_min_label);
        gtk_fixed_put(GTK_FIXED(update_settings_fix_detail),day_min_label, 250, 105);

	GtkWidget *bottom_layout = gtk_layout_new(NULL, NULL);
        gtk_widget_set_size_request(bottom_layout, 600, 80);
	gtk_widget_show(bottom_layout);
	char image_dir[256];
        snprintf(image_dir,sizeof(image_dir),_("%ssetting-right.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *setting_right_image = gtk_image_new_from_file(image_dir);
        gtk_layout_put(GTK_LAYOUT(bottom_layout), setting_right_image, 0, 0);
	gtk_widget_show(setting_right_image);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%ssetting-right-mask.png"),ANTIVIRUS_IMAGE_DIR);
	GtkWidget *setting_right_mask_image = gtk_image_new_from_file(image_dir);
        gtk_widget_show(setting_right_mask_image);
        gtk_layout_put(GTK_LAYOUT(bottom_layout), setting_right_mask_image, 0, 0);
 	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%ssave.png"),ANTIVIRUS_IMAGE_DIR);
        saveantisetimage = gtk_image_new_from_file(image_dir);
        gtk_widget_show(saveantisetimage);
        saveantisetbutton = gtk_button_new();
	gtk_widget_set_size_request(saveantisetbutton,87,39);
        gtk_widget_set_name(saveantisetbutton,"save_set_button");
        gtk_button_set_image(GTK_BUTTON(saveantisetbutton),saveantisetimage);
        gtk_widget_show(saveantisetbutton);
        gtk_button_set_relief((GtkButton *)saveantisetbutton,GTK_RELIEF_NONE);
        g_signal_connect(G_OBJECT(saveantisetbutton),"clicked",G_CALLBACK(save_update_set),NULL);
        g_signal_connect(G_OBJECT(saveantisetbutton),"enter",G_CALLBACK(save_set_enter),NULL);

 	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%srecovery_default.png"),ANTIVIRUS_IMAGE_DIR);
	GtkWidget *recovery_default_image = gtk_image_new_from_file(image_dir);
        gtk_widget_show(recovery_default_image);
        GtkWidget *recovery_default_button = gtk_button_new();
	 gtk_widget_set_size_request(recovery_default_button,87,39);
        gtk_widget_set_name(recovery_default_button,"recovery_default_button");
       // g_signal_connect(G_OBJECT(recovery_default_button),"enter",G_CALLBACK(recovery_default_enter),NULL);

       // gtk_button_set_image(GTK_BUTTON(recovery_default_button),recovery_default_image);
        gtk_widget_show(recovery_default_button);
        gtk_button_set_relief((GtkButton *)recovery_default_button,GTK_RELIEF_NONE);
        g_signal_connect(G_OBJECT(recovery_default_button),"clicked",G_CALLBACK(update_recovery_default),NULL);


        gtk_layout_put(GTK_LAYOUT(bottom_layout), recovery_default_button, 31, 20);
	gtk_layout_put(GTK_LAYOUT(bottom_layout), saveantisetbutton, 400, 20);
        gtk_fixed_put(GTK_FIXED(update_settings_fix),bottom_layout, 0,310);

	gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_week_week),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_week_hour),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_week_min),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_month_day),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_month_hour),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_month_min),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_day_hour),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_day_min),GTK_SENSITIVITY_OFF);
	int update_style = get_antivirus_setting_item_inttype("update-style",set_file_path);
	if(update_style==1){
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(timingbutton),TRUE);	

		system("echo 1 >> /tmp/cancelupdate.txt");

	}
	if(update_style==2){
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(periodbuton),TRUE);
	}
	int timing_update_select = get_antivirus_setting_item_inttype("timing-update-select",set_file_path);
	if(timing_update_select==1){
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(month_update_button),TRUE);
	}
	if(timing_update_select==2){
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(week_update_button),TRUE);
	}
	if(timing_update_select==3){
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(day_update_button),TRUE);
	}


	char month_value[20];	
	get_antivirus_setting_item_stringtype("timing-update-month",month_value,set_file_path);
        int in = 0;
        char *update_month[10];
	if(strcmp(month_value,"") != 0)
	{
        update_month[in] = strtok(month_value,",");
        while(update_month[in])
        {
                        in++;
                        update_month[in] = strtok(NULL,",");
        }
	}
	int month_day = atoi(update_month[0]);
	gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_month_day),month_day-1);
	int month_hour = atoi(update_month[1]);
	gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_month_hour),month_hour);
	int month_min = atoi(update_month[2]);
	gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_month_min),month_min);
	char week_value[20];
    get_antivirus_setting_item_stringtype("timing-update-week",week_value,set_file_path);
    in = 0;
    char *update_week[10];
    if(strcmp(week_value,"") != 0)
    {
        update_week[in] = strtok(week_value,",");
        while(update_week[in])
        {
                        in++;
                        update_week[in] = strtok(NULL,",");
        } 
	}
	int week_week = atoi(update_week[0]);
	gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_week_week),week_week);
	int week_hour = atoi(update_week[1]);
	gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_week_hour),week_hour);
	int week_min = atoi(update_week[2]);
	gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_week_min),week_min);
	char day_value[20];
    get_antivirus_setting_item_stringtype("timing-update-day",day_value,set_file_path);
    in = 0;
    char *update_day[10];
	if(strcmp(day_value,"") != 0)
	{
        update_day[in] = strtok(day_value,",");
        while(update_day[in])
        {
                        in++;
                        update_day[in] = strtok(NULL,",");
        }
	}
	int day_day = atoi(update_day[0]);	
	gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_day_hour),day_day);
	int day_min = atoi(update_day[1]);
	gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_day_min),day_min);



	
}
void start_isolation_region(GtkWidget *widget,gpointer data){
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(intelligence_deal_button))){
                gtk_widget_set_sensitive(data,TRUE);
        }
        else{
                gtk_widget_set_sensitive(data,FALSE);
        }
}

void choose_week_anti(GtkWidget *widget,gpointer data){
        auto_timing_anti_flag = 2;
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(week_anit_week_combo),GTK_SENSITIVITY_ON);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(week_anit_hour_combo),GTK_SENSITIVITY_ON);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(week_anit_min_combo),GTK_SENSITIVITY_ON);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(month_anti_day_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(month_anti_hour_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(month_anti_min_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(day_anti_hour_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(day_anit_min_combo),GTK_SENSITIVITY_OFF);

}
void choose_anti_month(GtkWidget *widget,gpointer data){
        auto_timing_anti_flag = 1;
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(week_anit_week_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(week_anit_hour_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(week_anit_min_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(month_anti_day_combo),GTK_SENSITIVITY_ON);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(month_anti_hour_combo),GTK_SENSITIVITY_ON);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(month_anti_min_combo),GTK_SENSITIVITY_ON);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(day_anti_hour_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(day_anit_min_combo),GTK_SENSITIVITY_OFF);
}
void choose_anti_day(GtkWidget *widget,gpointer data){
        auto_timing_anti_flag = 3;
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(week_anit_week_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(week_anit_hour_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(week_anit_min_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(month_anti_day_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(month_anti_hour_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(month_anti_min_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(day_anti_hour_combo),GTK_SENSITIVITY_ON);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(day_anit_min_combo),GTK_SENSITIVITY_ON);

}
void start_timing_anti(GtkWidget *widget,gpointer data){
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(timing_anti_button))){
                gtk_widget_set_sensitive(timing_anti_button,TRUE);
                gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(anti_scan_combo),GTK_SENSITIVITY_ON);
                gtk_widget_set_sensitive(month_anti_button,TRUE);
                gtk_widget_set_sensitive(week_anit_button,TRUE);
                gtk_widget_set_sensitive(day_anit_button,TRUE);
                if(auto_timing_anti_flag==1){
                        choose_anti_month(widget,data);

                }
                if(auto_timing_anti_flag==2){
                        choose_week_anti(widget,data);

                }
                if(auto_timing_anti_flag==3){
                        choose_anti_day(widget,data);

                }
        }
        else{
                gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(anti_scan_combo),GTK_SENSITIVITY_OFF);
                gtk_widget_set_sensitive(month_anti_button,FALSE);
                gtk_widget_set_sensitive(week_anit_button,FALSE);
                gtk_widget_set_sensitive(day_anit_button,FALSE);
                gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(week_anit_week_combo),GTK_SENSITIVITY_OFF);
                gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(week_anit_hour_combo),GTK_SENSITIVITY_OFF);
                gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(week_anit_min_combo),GTK_SENSITIVITY_OFF);
                gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(month_anti_day_combo),GTK_SENSITIVITY_OFF);
                gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(month_anti_hour_combo),GTK_SENSITIVITY_OFF);
                gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(month_anti_min_combo),GTK_SENSITIVITY_OFF);
                gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(day_anti_hour_combo),GTK_SENSITIVITY_OFF);
                gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(day_anit_min_combo),GTK_SENSITIVITY_OFF);

        }
}
void choose_manual(GtkWidget *widget,gpointer data)
{
	gtk_widget_set_sensitive(month_update_button,FALSE);
        gtk_widget_set_sensitive(week_update_button,FALSE);
        gtk_widget_set_sensitive(day_update_button,FALSE);

	gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_week_week),GTK_SENSITIVITY_OFF);
	gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_week_hour),GTK_SENSITIVITY_OFF);
	gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_week_min),GTK_SENSITIVITY_OFF);
	gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_month_day),GTK_SENSITIVITY_OFF);
	gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_month_hour),GTK_SENSITIVITY_OFF);
	gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_month_min),GTK_SENSITIVITY_OFF);

	gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_day_hour),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_day_min),GTK_SENSITIVITY_OFF);
}
void choose_month(GtkWidget *widget,gpointer data)
{
        auto_choose_flag=1;
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_week_week),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_week_hour),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_week_min),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_month_day),GTK_SENSITIVITY_ON);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_month_hour),GTK_SENSITIVITY_ON);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_month_min),GTK_SENSITIVITY_ON);

        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_day_hour),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_day_min),GTK_SENSITIVITY_OFF);

}
void choose_week(GtkWidget *widget,gpointer data)
{
        auto_choose_flag=2;
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_week_week),GTK_SENSITIVITY_ON);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_week_hour),GTK_SENSITIVITY_ON);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_week_min),GTK_SENSITIVITY_ON);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_month_day),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_month_hour),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_month_min),GTK_SENSITIVITY_OFF);

        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_day_hour),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_day_min),GTK_SENSITIVITY_OFF);

}
void choose_day(GtkWidget *widget,gpointer data)
{
        auto_choose_flag=3;
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_week_week),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_week_hour),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_week_min),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_month_day),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_month_hour),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_month_min),GTK_SENSITIVITY_OFF);

        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_day_hour),GTK_SENSITIVITY_ON);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(Combo_Box_day_min),GTK_SENSITIVITY_ON);

}
void choose_auto(GtkWidget *widget,gpointer data)
{
        gtk_widget_set_sensitive(month_update_button,TRUE);
        gtk_widget_set_sensitive(week_update_button,TRUE);
        gtk_widget_set_sensitive(day_update_button,TRUE);
        if (auto_choose_flag ==1)
        {
                choose_month(widget,data);
        }
        else if (auto_choose_flag ==2)
        {
                choose_week(widget,data);
        }
        else if (auto_choose_flag ==3)
        {
                choose_day(widget,data);
        }

}
void select_toggled (GtkCellRendererToggle *renderer,gchar *path,GtkTreeView *treeview)
        {
            GtkTreeModel *model;
            GtkTreeIter iter;
            gboolean value;
            /* Toggle the cell renderer's current state to the logical not. */
            model = gtk_tree_view_get_model (GTK_TREE_VIEW(treeview));
            if (gtk_tree_model_get_iter_from_string (model, &iter, path))
            {
                gtk_tree_model_get (model, &iter,0, &value, -1);
                gtk_list_store_set (GTK_LIST_STORE (model), &iter,0, !value, -1);
            }
        }
void remove_white_list(GtkWidget *widget,gpointer data){
        int i = 0;
        GtkListStore *store;
        GtkTreeModel *model;
        GtkTreeIter iter;
        gboolean value;
        store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(white_list_list)));
        model = gtk_tree_view_get_model ( GTK_TREE_VIEW( white_list_list));
        gtk_tree_model_get_iter_first(model,&iter);
	not_tog_warning(white_list_list,WHITE_LIST_TOG_COL,FALSE,3);
        do
        {
                i++;
                gtk_tree_model_get(model,&iter,WHITE_LIST_TOG_COL,&value,-1);
                if(value){
                        char cos[200];
                        snprintf(cos,sizeof(cos),"sed -i '%dd'  %swhite_list.txt 2>&1",i,ANTIVIRUS_CONF_DIR) ;
                        system(cos);
                        if(box!=NULL)
		            gtk_widget_destroy(box);
    			white_list_settings(while_list_content_box);

			i--;
                }
        }
        while (gtk_tree_model_iter_next(model,&iter));
}
void add_file(GtkWidget *widget,gpointer data){
        filechooser2((GtkWidget *)window,data);

}
void add_path(GtkWidget *widget,gpointer data){
        filechooser3((GtkWidget *)window,data);
}
void select_all()
{
        GtkTreeModel *model;
        GtkTreeIter iter;
        model = gtk_tree_view_get_model (GTK_TREE_VIEW(white_list_list));
        store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(white_list_list)));
	gtk_tree_model_get_iter_first(model,&iter);
        gboolean use_select_all=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(select_all_button));
        do
        {
                gtk_list_store_set(store,&iter,0,use_select_all,-1);
        }
        while (gtk_tree_model_iter_next(model,&iter));
}
void free_devide_file_list_memory ()
{
    struct DevideFileInfo *pnext;
    while(phead_devidefile!=NULL)
    {
       pnext = phead_devidefile->next;
       free(phead_devidefile);
       phead_devidefile = NULL;
       phead_devidefile = pnext;
    }

}
GtkTreeModel * create_devide_file_list_model(){
	GtkTreeIter iter;
        char c=0;
        struct DevideFileInfo *pcurrent,*ptempNode;
        gchar *file[1023];
        gchar *status[1023];
        store = gtk_list_store_new(3,G_TYPE_BOOLEAN,G_TYPE_STRING, G_TYPE_STRING);
	char isolation_temp[256];
        snprintf(isolation_temp,sizeof(isolation_temp),"%sisolation_temp.txt",ANTIVIRUS_TEMP_DIR);

        FILE *fp=fopen(isolation_temp,"rw");
        if(fp == NULL)
        {
            return GTK_TREE_MODEL(store);

        }
        gchar buf[1023];
        gint i=0;
        int j;
        gchar result[100][3][100];
        while(fgets(buf,1023,fp) != NULL)
        {
	        int in=0;
                char *p[20];
                char *buffer = buf;
                int j = 0;
                while((p[in]=strtok(buffer,","))!=NULL)
                {
                        in++;
                        buffer=NULL;
                }
               struct DevideFileInfo *newNode =(struct DevideFileInfo *) malloc(sizeof(struct DevideFileInfo)) ;
		if(i == 0)
                {
                    phead_devidefile = newNode;
                    phead_devidefile->next = NULL;
                    pcurrent = newNode;

                }
                if ( newNode->devide_file_path)
                {
                    strncpy(newNode->devide_file_path,p[1],511);
                }
                if (newNode->devide_time)
                {
                    strncpy(newNode->devide_time,p[3],49);
                }
                ptempNode = pcurrent;
                ptempNode->next = newNode;
                pcurrent = newNode ;
                newNode->next = NULL;
                i++;
        }
	fclose(fp);
	pcurrent = phead_devidefile;
        ptempNode = phead_devidefile;
        gtk_list_store_clear(store);
        for(j=0;j<i;j++)
        {
                 if(pcurrent!=NULL)
            {

		gtk_list_store_append(store,&iter);
                gtk_list_store_set(store,&iter,
                   0,FALSE,
                   1,pcurrent->devide_file_path,     //PIXBUF_COL也可以用0代替
                   2,pcurrent->devide_time,
                    -1);
		ptempNode = pcurrent->next;
                pcurrent = ptempNode;
		}
        }
        return GTK_TREE_MODEL(store);
}
void select_toggled_devide_file(GtkCellRendererToggle *renderer,gchar *path,GtkTreeView *treeview){
	 GtkTreeModel *model;
            GtkTreeIter iter;
            gboolean value;
            /* Toggle the cell renderer's current state to the logical not. */
            model = gtk_tree_view_get_model ((GtkTreeView *)treeview);
            if (gtk_tree_model_get_iter_from_string (model, &iter, path))
            {
                gtk_tree_model_get (model, &iter,0, &value, -1);
                gtk_list_store_set (GTK_LIST_STORE (model), &iter,0, !value, -1);
            }
}
void select_all_devide_file(){
	GtkTreeModel *model;
        GtkTreeIter iter;
	int j=0;
        model = gtk_tree_view_get_model (GTK_TREE_VIEW(devide_file_list_list));
        if(gtk_tree_model_get_iter_first(model,&iter))
	{
        gboolean use_select_all=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(select_all_devide_file_button));
        do
        {       ++j;
                gtk_list_store_set(store,&iter,0,use_select_all,-1);
        }
        while (gtk_tree_model_iter_next(model,&iter));
}
}
void look_devide_file_settings(GtkWidget *content_box){
        GtkWidget *label1,*label2,*used_space_label,*used_space_box,*button_box,*button_fix,*add_file_button,*remove_button,*recovery_button,*add_path_button,*label1_box,*fix,*choose_devide_dir;
        devide_file_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_widget_set_size_request(devide_file_box,600,320);
        gtk_box_pack_start(GTK_BOX (content_box),devide_file_box,FALSE,FALSE,0);
        label1_box =  gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        label1 = gtk_label_new(_("The file being processed are here to do a safety backup, you can remove or restore the state before the treatment"));
        gtk_box_pack_start(GTK_BOX (devide_file_box),label1_box,FALSE,FALSE,10);
        gtk_box_pack_start(GTK_BOX (label1_box),label1,FALSE,FALSE,20);
        used_space_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_box_pack_start(GTK_BOX (devide_file_box),used_space_box,FALSE,FALSE,10);
        fix = gtk_fixed_new();
        label2 = gtk_label_new(_("The devide dir is :"));
        gtk_box_pack_start(GTK_BOX (used_space_box),fix,FALSE,FALSE,0);
        gtk_fixed_put((GtkFixed *)fix,label2,20,0);
	devide_file_dir = gtk_label_new("");
	char devidefile_dir_info[256];
    char set_file_path[1024];
	snprintf(set_file_path,sizeof(set_file_path),"/usr/share/antivirus/config/%s_setting.txt",g_current_user);

    get_antivirus_setting_item_stringtype("devide-dir",devidefile_dir_info,set_file_path);
	if(strlen(devidefile_dir_info)>30){
	
	char devidefile_dir_show_info[40];
	strncpy(devidefile_dir_show_info,devidefile_dir_info, 30);
	int num_hanzi=0;
        int i=0;
        while(i<30){
                if(devidefile_dir_info[i]&0x80){
                        num_hanzi++;
                }
                i++;
        }
	if(num_hanzi%3==1){
                devidefile_dir_show_info[30]=devidefile_dir_info[30];
                devidefile_dir_show_info[31]=devidefile_dir_info[31];
                devidefile_dir_show_info[32]='.';
                devidefile_dir_show_info[33]='.';
                devidefile_dir_show_info[34]='.';
                devidefile_dir_show_info[35]='\0';
        }
        else if(num_hanzi%3==2){
                devidefile_dir_show_info[30]=devidefile_dir_info[30];
                devidefile_dir_show_info[31]='.';
                devidefile_dir_show_info[32]='.';
                devidefile_dir_show_info[33]='.';
                devidefile_dir_show_info[34]='\0';
        }
        else {
                devidefile_dir_show_info[30]='.';
                devidefile_dir_show_info[31]='.';
                devidefile_dir_show_info[32]='.';
                devidefile_dir_show_info[33]='\0';
        }
        gtk_label_set_text(devide_file_dir,devidefile_dir_show_info);
	}
	else{
		gtk_label_set_text(devide_file_dir,devidefile_dir_info);
	}
	
	gtk_widget_set_tooltip_text(devide_file_dir,devidefile_dir_info);

        gtk_fixed_put((GtkFixed *)fix,devide_file_dir,90,0);

	choose_devide_dir = gtk_button_new();
gtk_button_set_relief(choose_devide_dir,GTK_RELIEF_NONE);
	char image_dir[256];
        snprintf(image_dir,sizeof(image_dir),_("%schoose_devide_dir.png"),ANTIVIRUS_IMAGE_DIR);

	GtkWidget *choose_devide_dir_image = gtk_image_new_from_file(image_dir);
	gtk_widget_show(choose_devide_dir_image);
	gtk_button_set_image(choose_devide_dir,choose_devide_dir_image);
        gtk_widget_show(choose_devide_dir);
        gtk_fixed_put((GtkFixed *)fix,choose_devide_dir,300,0);
        g_signal_connect (choose_devide_dir, "clicked", G_CALLBACK (select_devide_file),NULL);


        GtkWidget *look_devide_rigth_fix = gtk_fixed_new();

        GtkWidget *look_devide_right_frame=gtk_frame_new(NULL);
        gtk_widget_set_size_request(look_devide_right_frame,550,160);
        gtk_fixed_put((GtkFixed *)look_devide_rigth_fix,look_devide_right_frame,20,0);

        GtkWidget *fix_test = gtk_fixed_new();
        GtkListStore *devide_file_list_store;
        devide_file_list_sw = gtk_scrolled_window_new(NULL, NULL);
        gtk_widget_set_size_request(devide_file_list_sw,550,132);
        gtk_scrolled_window_set_min_content_height((GtkScrolledWindow *)devide_file_list_sw,132);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(devide_file_list_sw), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
        gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(devide_file_list_sw), GTK_SHADOW_ETCHED_IN);
        devide_file_list_list = gtk_tree_view_new_with_model(create_devide_file_list_model());
        devide_file_list_render_tog = gtk_cell_renderer_toggle_new();
        g_object_set(G_OBJECT(devide_file_list_render_tog),"activatable",TRUE,NULL);
        devide_file_list_column_tog = gtk_tree_view_column_new_with_attributes("      ",devide_file_list_render_tog,"active",DEVIDE_FILE_LIST_TOG_COL,NULL);
	gtk_tree_view_column_set_resizable(devide_file_list_column_tog,TRUE);
        gtk_tree_view_column_set_sizing (devide_file_list_column_tog,GTK_TREE_VIEW_COLUMN_FIXED);
        gtk_tree_view_column_set_fixed_width(devide_file_list_column_tog,50);

        gtk_tree_view_append_column(GTK_TREE_VIEW(devide_file_list_list),devide_file_list_column_tog);
        g_signal_connect (devide_file_list_render_tog, "toggled", G_CALLBACK (select_toggled_devide_file),devide_file_list_list);

        devide_file_list_render = gtk_cell_renderer_text_new();
        devide_file_list_column = gtk_tree_view_column_new_with_attributes(_("name"),devide_file_list_render, "text", DEVIDE_FILE_LIST_NAME_COL,NULL);
	gtk_tree_view_column_set_resizable(devide_file_list_column,TRUE);
        gtk_tree_view_column_set_sizing (devide_file_list_column,GTK_TREE_VIEW_COLUMN_FIXED);
        gtk_tree_view_column_set_fixed_width(devide_file_list_column,330);

        gtk_tree_view_append_column(GTK_TREE_VIEW(devide_file_list_list), devide_file_list_column);

        devide_file_list_render = gtk_cell_renderer_text_new();
        devide_file_list_column = gtk_tree_view_column_new_with_attributes(_("time"),devide_file_list_render, "text", DEVIDE_FILE_LIST_TIME_COL, NULL);
	gtk_tree_view_column_set_resizable(devide_file_list_column,TRUE);
        gtk_tree_view_column_set_sizing (devide_file_list_column,GTK_TREE_VIEW_COLUMN_FIXED);
        gtk_tree_view_column_set_fixed_width(devide_file_list_column,100);

        gtk_tree_view_append_column(GTK_TREE_VIEW(devide_file_list_list), devide_file_list_column);
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (devide_file_list_list),FALSE);
        devide_file_selection = gtk_tree_view_get_selection((GTK_TREE_VIEW(devide_file_list_list)));
        gtk_tree_selection_set_mode(devide_file_selection,GTK_SELECTION_SINGLE);
	gtk_tree_view_set_tooltip_column(GTK_TREE_VIEW(devide_file_list_list), DEVIDE_FILE_LIST_NAME_COL);
        gtk_container_add(GTK_CONTAINER(devide_file_list_sw), devide_file_list_list);
	GtkWidget *devide_file_list_title;
	
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sdevide_list_title_bg.png"),ANTIVIRUS_IMAGE_DIR);
        devide_file_list_title=gtk_image_new_from_file(image_dir);
        gtk_widget_show(devide_file_list_title);
        gtk_fixed_put((GtkFixed *)fix_test,devide_file_list_title,0,0);
        gtk_fixed_put((GtkFixed *)fix_test,devide_file_list_sw,0,28);
        gtk_container_add(GTK_CONTAINER(look_devide_right_frame),fix_test);

        gtk_box_pack_start(GTK_BOX (devide_file_box),look_devide_rigth_fix,FALSE,FALSE,0);

        GtkWidget *bottom_layout = gtk_layout_new(NULL, NULL);
        gtk_widget_set_size_request(bottom_layout, 600, 80);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%ssetting-right.png"),ANTIVIRUS_IMAGE_DIR);
       GtkWidget *setting_right_image = gtk_image_new_from_file(image_dir);
        gtk_layout_put(GTK_LAYOUT(bottom_layout), setting_right_image, 0, 0);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%ssetting-right-mask.png"),ANTIVIRUS_IMAGE_DIR);
	GtkWidget *setting_right_mask_image = gtk_image_new_from_file(image_dir);
        gtk_widget_show(setting_right_mask_image);
        gtk_layout_put(GTK_LAYOUT(bottom_layout), setting_right_mask_image, 0, 0);

        select_all_devide_file_button = gtk_check_button_new_with_label(_("select all"));
	gtk_widget_show(select_all_devide_file_button);
        gtk_fixed_put((GtkFixed *)fix_test,select_all_devide_file_button,0,155);
        int count = gtk_tree_view_get_n_columns (GTK_TREE_VIEW(devide_file_list_list));
        gtk_widget_set_size_request(select_all_devide_file_button, 40, 40);
        remove_button = gtk_button_new();
	gtk_widget_set_size_request(remove_button,87,39);
        gtk_widget_set_name(remove_button,"remove_button_devide");
        gtk_button_set_relief((GtkButton *)remove_button,GTK_RELIEF_NONE);
	gtk_widget_show(remove_button);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sremove.png"),ANTIVIRUS_IMAGE_DIR);

        GtkWidget *removeimage = gtk_image_new_from_file(image_dir);
	gtk_widget_show(removeimage);
	//gtk_button_set_image(remove_button,removeimage);
        gtk_layout_put(GTK_LAYOUT(bottom_layout), remove_button, 140, 20);
        recovery_button = gtk_button_new();
        gtk_button_set_relief((GtkButton *)recovery_button,GTK_RELIEF_NONE);
	gtk_widget_set_size_request(recovery_button,87,39);
        gtk_widget_set_name(recovery_button,"recovery_button");
	gtk_widget_show(recovery_button);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%srestore.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *restore_image = gtk_image_new_from_file(image_dir);
	gtk_widget_show(restore_image);
        //gtk_button_set_image(recovery_button,restore_image);
        gtk_layout_put(GTK_LAYOUT(bottom_layout), recovery_button, 280, 20);
        GtkTreeIter iter;
        GtkTreeModel *model= gtk_tree_view_get_model (GTK_TREE_VIEW(devide_file_list_list));
        if(!gtk_tree_model_get_iter_first(model,&iter)){
                if(removeimage!=NULL){
                        gtk_widget_destroy(removeimage);
			memset(image_dir,0,sizeof(image_dir));
        		snprintf(image_dir,sizeof(image_dir),_("%sremove-un-use.png"),ANTIVIRUS_IMAGE_DIR);
                        removeimage = gtk_image_new_from_file(image_dir);
			//gtk_button_set_image(remove_button,removeimage);
                }
                if(restore_image!=NULL){
                        gtk_widget_destroy(restore_image);
			memset(image_dir,0,sizeof(image_dir));
        		snprintf(image_dir,sizeof(image_dir),_("%srestore-un-use.png"),ANTIVIRUS_IMAGE_DIR);
                        restore_image = gtk_image_new_from_file(image_dir);
		//	gtk_button_set_image(recovery_button,restore_image);
                }
                gtk_widget_set_sensitive(remove_button,FALSE);
                gtk_widget_set_sensitive(recovery_button,FALSE);
        }
        g_signal_connect (select_all_devide_file_button, "toggled", G_CALLBACK (select_all_devide_file),devide_file_list_list);
        g_signal_connect(G_OBJECT(remove_button),"clicked",G_CALLBACK(remove_devide_file_list),content_box);
       // g_signal_connect(G_OBJECT(remove_button),"enter",G_CALLBACK(enter_remove_button),NULL);
        g_signal_connect(G_OBJECT(recovery_button),"clicked",G_CALLBACK(rescovery_func),NULL);
       // g_signal_connect(G_OBJECT(recovery_button),"enter",G_CALLBACK(enter_recovery_button),NULL);


        gtk_box_pack_start(GTK_BOX (devide_file_box),bottom_layout,FALSE,FALSE,28);
        gtk_widget_show_all(content_box);

}
