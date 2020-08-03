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
#include "log-page.h"
#include "../scan.h"
#include "../refresh.h"
#include <sys/stat.h>
#include "../result.h"
#include "../filechooser.h"
#include "../writelog.h"
#include "../function.h"
#include "../deal/log_handle.h"
#include "../page/setting-page.h"
#define _(STRING) gettext(STRING)
#define LOG_PAGE_WIDTH 200
#define LOG_PAGE_LENTH 30

extern int area ;
extern GtkWidget *table1,*table2,*table3,*table4,*table6,*table7,*vbox1,*logfix1,*vboxlog,*hboxlog,*thingbox;
extern GtkWidget * mainPageIntroduceImage;
extern GtkWidget *antibutton,*setbutton,*logbutton,*aboutbutton;
char * combo_text;
GtkWidget *log_notebook;
GtkWidget *scan_log_content_box,*scan_log_list_box,*log_list_vbox;
GtkWidget *defense_log_content_box,*update_log_content_box,*update_log_page_box;
GtkWidget *scan_log_list_vbox,*defense_log_list_vbox,*update_log_list_vbox;
//GtkWidget *scan_listlog;
struct ScanLogInfo
{
            char scan_time[30];
            char scan_type[512];
            char scan_result[100];
            struct ScanLogInfo *next;
};
struct ScanLogInfo *phead_scanLogs=NULL;
char g_current_user[256]; //当前启动杀毒软件用户

void get_select_page(GtkWidget *widget,gpointer *data)
{
	printf("当前页号是%d\n",gtk_notebook_get_current_page(log_notebook));
}

/*全选功能*/
void select_all_log(GtkWidget *select_all_button,gpointer *data)
{
        GtkTreeModel *model;
        GtkTreeIter iter;
	gboolean use_select_all;
		model = gtk_tree_view_get_model (GTK_TREE_VIEW(data));
		use_select_all=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(select_all_button));
        gtk_tree_model_get_iter_first(GTK_TREE_MODEL(model),&iter);

        do
        {

                	gtk_list_store_set(GTK_LIST_STORE(model),&iter,0,use_select_all,-1);
        }
        while (gtk_tree_model_iter_next(model,&iter));
}


void free_log_list_memory ()
{
    struct ScanLogInfo *pnext;
    while(phead_scanLogs!=NULL)
    {
       pnext = phead_scanLogs->next;
       free(phead_scanLogs);
       phead_scanLogs = NULL;
       phead_scanLogs = pnext;
    }

}

/*填充日志信息*/
GtkTreeModel *createlogModel(char *filepath)
{
	GtkTreeIter iter;
        char c=0;
        struct ScanLogInfo *pcurrent,*ptempNode;

        store_log = gtk_list_store_new(4,G_TYPE_BOOLEAN,G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING);   //2列
	char logfile_path[100];
        strncpy(logfile_path,filepath,sizeof(logfile_path));
	printf("aaaaaaaaaaaaa logfile_path is :%s\n",logfile_path);
        FILE *fp=fopen(logfile_path,"rw");        
	if(fp == NULL)
        {
		printf("is null \n");
            return GTK_TREE_MODEL(store_log);

        }


        gchar buf[1023];
        gint i=0;
        int j;

        while(fgets(buf,1023,fp) != NULL)
        {
        	int in=0;
            	char *p[20];
            	char *buffer = buf;
//		char buffer[1024];
//		strncpy(buffer,buf,sizeof(buffer));
		
            	int j = 0;
            	while((p[in]=strtok(buffer,","))!=NULL)
            	{
                	in++;
                	buffer=NULL;
            	}
                struct ScanLogInfo *newNode =(struct ScanLogInfo *) malloc(sizeof(struct ScanLogInfo)) ;
                if(i == 0)
                {
                    phead_scanLogs = newNode;
                    phead_scanLogs->next = NULL;
                    pcurrent = newNode;

                }
                if ( newNode->scan_time)
                {
                    strncpy(newNode->scan_time,p[0],29);
                }
                if (newNode->scan_type)
                {
                    strncpy(newNode->scan_type,p[1],511);
                }
                if (newNode->scan_result )
                {
                    strncpy(newNode->scan_result,p[2],99);
                }
                ptempNode = pcurrent;
                ptempNode->next = newNode;
                pcurrent = newNode ;
                newNode->next = NULL;
                i++;

        }
        fclose(fp);
        pcurrent = phead_scanLogs;
        ptempNode = phead_scanLogs;
        for(j=0;j<i;j++)
        {
            if(pcurrent!=NULL)
            {
               
                gtk_list_store_append(store_log,&iter);
                gtk_list_store_set(store_log,&iter,
                    LOGTOG_COL,FALSE, 
                    DATELOG_COL,pcurrent->scan_time,     //PIXBUF_COL也可以用0代替
                    TYPELOG_COL,pcurrent->scan_type,
                    RESULTLOG_COL,pcurrent->scan_result,
                    -1);
                ptempNode = pcurrent->next;
                pcurrent = ptempNode;
            }

       	}
       	return GTK_TREE_MODEL(store_log);
}
/*点击主页面中的日志按钮初始化的日志页面*/
void logwindow(GtkWidget *widget,gpointer data)
{
	char image_dir[256];
	if(area==1)
	{
    		snprintf(image_dir,sizeof(image_dir),_("%santi.png"),ANTIVIRUS_IMAGE_DIR);

		gtk_button_set_relief((GtkButton *)antibutton,GTK_RELIEF_NONE);
                GtkWidget *leave_antibutton_image = gtk_image_new_from_file(image_dir);
//		gtk_button_set_image(antibutton,leave_antibutton_image);
    		gtk_widget_hide(table1);
     	}
     	if(area==2)
	{
		gtk_button_set_relief((GtkButton *)setbutton,GTK_RELIEF_NONE);
		memset(image_dir,0,sizeof(image_dir));
    		snprintf(image_dir,sizeof(image_dir),_("%ssetting.png"),ANTIVIRUS_IMAGE_DIR);

                GtkWidget *leave_setbutton_image = gtk_image_new_from_file(image_dir);
  //              gtk_button_set_image(setbutton,leave_setbutton_image);
    		gtk_widget_hide(table2);
                if(box!=NULL)
                    gtk_widget_destroy(box);
                white_list_settings(while_list_content_box);
                free_devide_file_list_memory();
                free_white_list_file_memory();
    	}
     	if(area==3)
    	{
                gtk_widget_hide(table1);
       	}
     	if(area==4)
	{
		gtk_button_set_relief((GtkButton *)aboutbutton,GTK_RELIEF_NONE);
		memset(image_dir,0,sizeof(image_dir));
    		snprintf(image_dir,sizeof(image_dir),_("%sabout.png"),ANTIVIRUS_IMAGE_DIR);
                GtkWidget *leave_aboutbutton_image = gtk_image_new_from_file(image_dir);

    //            gtk_button_set_image(aboutbutton,leave_aboutbutton_image);
     		gtk_widget_hide(table4);
     	}
     	if(area==5)
	{
     		//gtk_widget_hide(table5);
     	}
	gtk_widget_show(table3);
	gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
	memset(image_dir,0,sizeof(image_dir));
    	snprintf(image_dir,sizeof(image_dir),_("%slog_enter.png"),ANTIVIRUS_IMAGE_DIR);

        GtkWidget *enter_log_image = gtk_image_new_from_file(image_dir);
        //gtk_button_set_image(widget,enter_log_image);
	gtk_notebook_set_current_page((GtkNotebook *)log_notebook,0);
	memset(image_dir,0,sizeof(image_dir));
	snprintf(image_dir,sizeof(image_dir),_("%slogging_introduce_title.png"),ANTIVIRUS_IMAGE_DIR);
	gtk_image_set_from_file (mainPageIntroduceImage,image_dir);
	if(scan_log_list_vbox!=NULL)
	{
                gtk_widget_destroy(scan_log_list_vbox);
        scan_log_page(scan_log_content_box);
	}
	if(defense_log_list_vbox!=NULL)
	{
                gtk_widget_destroy(defense_log_list_vbox);
        defense_log_page(defense_log_content_box);
	}
	if(update_log_list_vbox!=NULL)
         {       gtk_widget_destroy(update_log_list_vbox);
        update_log_page(update_log_content_box);
}
	area=3;
}
/*根据日志种类初始化日志页面*/

void init_log_window(GtkWidget *table)
{
 	GtkWidget *log_box;
	char image_dir[256];
        log_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_widget_set_size_request(log_box, 800, 400);
        gtk_widget_show(log_box);

	GtkWidget *log_left_layout = gtk_layout_new(NULL, NULL);
        gtk_widget_set_size_request(log_left_layout, 800, 400);
	gtk_widget_show(log_left_layout);
        snprintf(image_dir,sizeof(image_dir),_("%ssetting-left.png"),ANTIVIRUS_IMAGE_DIR);

	GtkWidget *log_left_image = gtk_image_new_from_file(image_dir);
        gtk_widget_show(log_left_image);
        gtk_layout_put(GTK_LAYOUT(log_left_layout), log_left_image, 0, 0);
	memset(image_dir,0,sizeof(image_dir));
    	snprintf(image_dir,sizeof(image_dir),_("%ssetting-left-mask.png"),ANTIVIRUS_IMAGE_DIR);
	GtkWidget *log_left_mask_image = gtk_image_new_from_file(image_dir);
        gtk_widget_show(log_left_mask_image);
        gtk_layout_put(GTK_LAYOUT(log_left_layout), log_left_mask_image, 0, 0);

	gtk_container_add(GTK_CONTAINER(log_box), log_left_layout);

	char log_note_css[256];
        snprintf(log_note_css,sizeof(log_note_css),"%slog-note.css",ANTIVIRUS_CSS_DIR);
	GFile *file=g_file_new_for_path(log_note_css);
        GdkScreen *screen;
        screen = gdk_screen_get_default();
        GtkCssProvider *cssProvider;
        cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_file(cssProvider, file, NULL);

        log_notebook=gtk_notebook_new();
        gtk_widget_set_size_request(notebook, 800, 400);
        gtk_notebook_set_tab_pos(GTK_NOTEBOOK(log_notebook),GTK_POS_LEFT);
	gtk_layout_put(GTK_LAYOUT(log_left_layout), log_notebook, 0, 0);
//        gtk_widget_show(log_notebook);
        g_signal_connect (log_notebook,"switch-page", G_CALLBACK (get_select_page),NULL);	

        GtkWidget *scan_log_page_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *scan_log_label = gtk_label_new(_("scan log"));
	change_font_style((GtkLabel *)scan_log_label,"<span foreground=\"#000000\" font_desc='12' weight='bold'>%s</span>");
	gtk_widget_show(scan_log_label);
	gtk_box_pack_start(GTK_BOX (scan_log_page_box),scan_log_label,FALSE,FALSE,40);
        gtk_widget_set_size_request(scan_log_page_box, LOG_PAGE_WIDTH, LOG_PAGE_LENTH);
	scan_log_content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_set_size_request(scan_log_content_box, 600, 400);
        scan_log_page(scan_log_content_box);
        gtk_notebook_insert_page(GTK_NOTEBOOK(log_notebook),scan_log_content_box,scan_log_page_box,0);
        gtk_widget_show(scan_log_page_box);
        gtk_widget_show(scan_log_content_box);
	

	GtkWidget *defense_log_page_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *defense_log_label = gtk_label_new(_("defense log"));
	change_font_style((GtkLabel *)defense_log_label,"<span foreground=\"#000000\" font_desc='12' weight='bold'>%s</span>");
	gtk_widget_show(defense_log_label);
	gtk_box_pack_start(GTK_BOX (defense_log_page_box),defense_log_label,FALSE,FALSE,40);
        gtk_widget_set_size_request(defense_log_page_box, LOG_PAGE_WIDTH, LOG_PAGE_LENTH);
        defense_log_content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);	
	gtk_widget_set_size_request(defense_log_content_box, 600, 400);
        defense_log_page(defense_log_content_box);
        gtk_notebook_insert_page(GTK_NOTEBOOK(log_notebook),defense_log_content_box,defense_log_page_box,1);
        gtk_widget_show(defense_log_page_box);
        gtk_widget_show(defense_log_content_box);

	update_log_page_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *update_log_label = gtk_label_new(_("update log"));
	change_font_style((GtkLabel *)update_log_label,"<span foreground=\"#000000\" font_desc='12' weight='bold'>%s</span>");
	gtk_widget_show(update_log_label);
	gtk_box_pack_start(GTK_BOX (update_log_page_box),update_log_label,FALSE,FALSE,40);
        gtk_widget_set_size_request(update_log_page_box, LOG_PAGE_WIDTH, LOG_PAGE_LENTH);
        update_log_content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_set_size_request(update_log_content_box, 600, 400);
        update_log_page(update_log_content_box);
        gtk_notebook_insert_page(GTK_NOTEBOOK(log_notebook),update_log_content_box,update_log_page_box,2);
        gtk_widget_show(update_log_page_box);
        gtk_widget_show(update_log_content_box);
        gtk_widget_show(log_notebook);
	gtk_notebook_set_current_page((GtkNotebook *)log_notebook,0);
	gtk_widget_set_name(log_notebook,"log-note");
        gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);
	gtk_grid_attach((GtkGrid *)table3,log_box,0,0,1,1);

}
void scan_log_page(GtkWidget *content_box)
{
	GtkWidget *scan_swlog,*scan_vboxlog,*scan_hboxlog,*scan_select_all_log_button,*scan_listlog;
        int logType = 0;
	char image_dir[256];
	GtkCellRenderer *scan_rendererlog;
	GtkTreeViewColumn *scan_columnlog;
	GtkWidget *scan_removelog,*scan_exportlog;
	GtkTreeSelection *scan_selectionlog;
	char log_page_css[256];
        snprintf(log_page_css,sizeof(log_page_css),"%slog-page.css",ANTIVIRUS_CSS_DIR);

	GFile *file=g_file_new_for_path(log_page_css);
    	GdkScreen *screen;
    	screen = gdk_screen_get_default();
    	GtkCssProvider *cssProvider;
    	cssProvider = gtk_css_provider_new();
    	gtk_css_provider_load_from_file(cssProvider, file, NULL);

	scan_log_list_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_widget_show(scan_log_list_vbox);

        scan_vboxlog = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_widget_show(scan_vboxlog);

	GtkWidget *scan_log_title;
        snprintf(image_dir,sizeof(image_dir),_("%slog_title.png"),ANTIVIRUS_IMAGE_DIR);
	scan_log_title=gtk_image_new_from_file(image_dir);
	gtk_widget_show(scan_log_title);
	gtk_box_pack_start(GTK_BOX(scan_vboxlog), scan_log_title, TRUE, TRUE, 0);	
	
        scan_swlog = gtk_scrolled_window_new(NULL, NULL);
        gtk_widget_show(scan_swlog);
        gtk_scrolled_window_set_min_content_height((GtkScrolledWindow *)scan_swlog,260);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scan_swlog), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
        gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scan_swlog), GTK_SHADOW_ETCHED_IN);

	char log_file[256];
        snprintf(log_file,sizeof(log_file),"%s%s_log_file",ANTIVIRUS_LOG_DIR,g_current_user);
	scan_listlog = gtk_tree_view_new_with_model(createlogModel(log_file));
        gtk_widget_show(scan_listlog);
        scan_rendererlog = gtk_cell_renderer_toggle_new();
        g_object_set(G_OBJECT(scan_rendererlog),"activatable",TRUE,NULL);
        scan_columnlog = gtk_tree_view_column_new_with_attributes("      ",scan_rendererlog,"active",LOGTOG_COL,NULL);
	gtk_tree_view_column_set_resizable(scan_columnlog,TRUE);
        gtk_tree_view_column_set_sizing (scan_columnlog,GTK_TREE_VIEW_COLUMN_FIXED);
        gtk_tree_view_column_set_fixed_width(scan_columnlog,50);

        gtk_tree_view_append_column(GTK_TREE_VIEW(scan_listlog),scan_columnlog);
        g_signal_connect (scan_rendererlog, "toggled", G_CALLBACK (select_toggled),scan_listlog);

	scan_rendererlog = gtk_cell_renderer_text_new();
        scan_columnlog = gtk_tree_view_column_new_with_attributes(_("Date"),scan_rendererlog, "text", DATELOG_COL, NULL);
	gtk_tree_view_column_set_resizable(scan_columnlog,TRUE);
        gtk_tree_view_column_set_sizing (scan_columnlog,GTK_TREE_VIEW_COLUMN_FIXED);
        gtk_tree_view_column_set_fixed_width(scan_columnlog,150);

        gtk_tree_view_append_column(GTK_TREE_VIEW(scan_listlog), scan_columnlog);

        scan_rendererlog = gtk_cell_renderer_text_new();
        scan_columnlog = gtk_tree_view_column_new_with_attributes(_("Event"),scan_rendererlog, "text", TYPELOG_COL, NULL);
	gtk_tree_view_column_set_resizable(scan_columnlog,TRUE);
        gtk_tree_view_column_set_sizing (scan_columnlog,GTK_TREE_VIEW_COLUMN_FIXED);
        gtk_tree_view_column_set_fixed_width(scan_columnlog,150);
        gtk_tree_view_append_column(GTK_TREE_VIEW(scan_listlog), scan_columnlog);


        scan_rendererlog = gtk_cell_renderer_text_new();
        scan_columnlog = gtk_tree_view_column_new_with_attributes(_("Result"),scan_rendererlog, "text", RESULTLOG_COL, NULL);
	gtk_tree_view_column_set_resizable(scan_columnlog,TRUE);
        gtk_tree_view_column_set_sizing (scan_columnlog,GTK_TREE_VIEW_COLUMN_FIXED);
        gtk_tree_view_column_set_fixed_width(scan_columnlog,200);
        gtk_tree_view_append_column(GTK_TREE_VIEW(scan_listlog), scan_columnlog);
/**dsj-1225**/
	 gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (scan_listlog),FALSE);
/**dsj-1225-end**/
        gtk_box_pack_start(GTK_BOX(scan_vboxlog), scan_swlog, TRUE, TRUE, 0);
        scan_hboxlog = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_widget_show(scan_hboxlog);
	
	GtkWidget *log_right_layout = gtk_layout_new(NULL, NULL);
	gtk_widget_set_size_request(log_right_layout, 600, 80);
        gtk_widget_show(log_right_layout);
	gtk_container_add(GTK_CONTAINER(scan_hboxlog), log_right_layout);

	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%ssetting-right.png"),ANTIVIRUS_IMAGE_DIR);
	
	GtkWidget *log_right_image = gtk_image_new_from_file(image_dir);
        gtk_widget_show(log_right_image);
        gtk_layout_put(GTK_LAYOUT(log_right_layout), log_right_image, 0, 0);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%ssetting-right-mask.png"),ANTIVIRUS_IMAGE_DIR);

	GtkWidget *log_right_mask_image = gtk_image_new_from_file(image_dir);
        gtk_widget_show(log_right_mask_image);
        gtk_layout_put(GTK_LAYOUT(log_right_layout), log_right_mask_image, 0, 0);

        scan_removelog = gtk_button_new();
        gtk_widget_show(scan_removelog);
        gtk_button_set_relief((GtkButton *)scan_removelog,GTK_RELIEF_NONE);
	gtk_widget_set_size_request(scan_removelog,87,39);
        gtk_widget_set_name(scan_removelog,"scan_removelog");
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sremove.png"),ANTIVIRUS_IMAGE_DIR);

        GtkWidget *removeimage = gtk_image_new_from_file(image_dir);
        gtk_widget_show(removeimage);
     //   gtk_button_set_image(scan_removelog,removeimage);

        scan_exportlog = gtk_button_new();
        gtk_widget_show(scan_exportlog);
        gtk_button_set_relief((GtkButton *)scan_exportlog,GTK_RELIEF_NONE);
	gtk_widget_set_size_request(scan_exportlog,87,39);
        gtk_widget_set_name(scan_exportlog,"scan_exportlog");
	
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sexport.png"),ANTIVIRUS_IMAGE_DIR);

        GtkWidget *exportimage = gtk_image_new_from_file(image_dir);
        gtk_widget_show(exportimage);
//        gtk_button_set_image(scan_exportlog,exportimage);

       scan_select_all_log_button = gtk_check_button_new_with_label(_("select all"));
        g_signal_connect (scan_select_all_log_button, "toggled", G_CALLBACK (select_all_log),scan_listlog);
        gtk_widget_show(scan_select_all_log_button);
        scan_selectionlog=gtk_tree_view_get_selection((GTK_TREE_VIEW(scan_listlog)));
        gtk_tree_selection_set_mode(scan_selectionlog,GTK_SELECTION_SINGLE);
	gtk_layout_put(GTK_LAYOUT(log_right_layout), scan_removelog, 200, 20);
	gtk_layout_put(GTK_LAYOUT(log_right_layout), scan_exportlog, 400, 20);
        gtk_box_pack_start(GTK_BOX(scan_vboxlog), scan_select_all_log_button, TRUE, TRUE, 5);

        gtk_box_pack_start(GTK_BOX(scan_vboxlog), scan_hboxlog, FALSE, TRUE, 0);
                gtk_container_add(GTK_CONTAINER(scan_swlog), scan_listlog);
        gtk_box_pack_start(GTK_BOX (scan_log_list_vbox),scan_vboxlog,FALSE,FALSE,0);

        g_signal_connect(G_OBJECT(scan_removelog), "clicked", G_CALLBACK(remove_scanlog_item),scan_listlog);
  //      g_signal_connect(G_OBJECT(scan_removelog), "enter", G_CALLBACK(enter_removelog), NULL);

        g_signal_connect(G_OBJECT(scan_exportlog), "clicked", G_CALLBACK(import_scan_log),scan_listlog);
    //    g_signal_connect(G_OBJECT(scan_exportlog), "enter", G_CALLBACK(enter_exportlog),NULL);

        gtk_box_pack_start(GTK_BOX (content_box),scan_log_list_vbox,FALSE,FALSE,0);

        GtkTreeIter iter;
        GtkTreeModel *scan_model= gtk_tree_view_get_model (GTK_TREE_VIEW(scan_listlog));
        if(!gtk_tree_model_get_iter_first(scan_model,&iter)){
                if(removeimage!=NULL){
                        g_print("开始更换日志图片\n");
                        gtk_widget_destroy(removeimage);
			memset(image_dir,0,sizeof(image_dir));
        		snprintf(image_dir,sizeof(image_dir),_("%sremove-un-use.png"),ANTIVIRUS_IMAGE_DIR);
                        removeimage = gtk_image_new_from_file(image_dir);
                        gtk_widget_show(removeimage);
      //                  gtk_button_set_image(scan_removelog,removeimage);
			memset(image_dir,0,sizeof(image_dir));
        		snprintf(image_dir,sizeof(image_dir),_("%sexport-un-use.png"),ANTIVIRUS_IMAGE_DIR);
			gtk_image_set_from_file(exportimage,image_dir);
                }
		gtk_widget_set_sensitive(scan_exportlog,FALSE);
                gtk_widget_set_sensitive(scan_removelog,FALSE);
        }
 gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);

}
void defense_log_page(GtkWidget *content_box)
{
        GtkWidget *defense_swlog,*defense_listlog,*defense_vboxlog,*defense_hboxlog,*defense_select_all_log_button;
        GtkCellRenderer *defense_rendererlog;
        GtkTreeViewColumn *defense_columnlog;
        GtkWidget *defense_removelog,*defense_exportlog;
        GtkTreeSelection *defense_selectionlog;
	char image_dir[256];
	char log_page_css[256];
        snprintf(log_page_css,sizeof(log_page_css),"%slog-page.css",ANTIVIRUS_CSS_DIR);

        GFile *file=g_file_new_for_path(log_page_css);
        GdkScreen *screen;
        screen = gdk_screen_get_default();
        GtkCssProvider *cssProvider;
        cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_file(cssProvider, file, NULL);

	defense_log_list_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_widget_show(defense_log_list_vbox);

	GtkWidget *defence_log_title;
        snprintf(image_dir,sizeof(image_dir),_("%slog_title.png"),ANTIVIRUS_IMAGE_DIR);

        defence_log_title=gtk_image_new_from_file(image_dir);
        gtk_widget_show(defence_log_title);

        defense_swlog = gtk_scrolled_window_new(NULL, NULL);
        gtk_widget_show(defense_swlog);
        gtk_scrolled_window_set_min_content_height((GtkScrolledWindow *)defense_swlog,260);
//        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(defense_swlog),GTK_POLICY_ALWAYS, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(defense_swlog), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
        gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(defense_swlog), GTK_SHADOW_ETCHED_IN);

	char defense_log[256];
        snprintf(defense_log,sizeof(defense_log),"%s%s_defence_log",ANTIVIRUS_LOG_DIR,g_current_user);

        defense_listlog = gtk_tree_view_new_with_model(createlogModel(defense_log));
        gtk_widget_show(defense_listlog);
        defense_rendererlog = gtk_cell_renderer_toggle_new();
        g_object_set(G_OBJECT(defense_rendererlog),"activatable",TRUE,NULL);
        defense_columnlog = gtk_tree_view_column_new_with_attributes("      ",defense_rendererlog,"active",LOGTOG_COL,NULL);
	gtk_tree_view_column_set_resizable(defense_columnlog,TRUE);
        gtk_tree_view_column_set_sizing (defense_columnlog,GTK_TREE_VIEW_COLUMN_FIXED);
        gtk_tree_view_column_set_fixed_width(defense_columnlog,50);

        gtk_tree_view_append_column(GTK_TREE_VIEW(defense_listlog),defense_columnlog);
        g_signal_connect (defense_rendererlog, "toggled", G_CALLBACK (select_toggled),defense_listlog);

        defense_rendererlog = gtk_cell_renderer_text_new();
        defense_columnlog = gtk_tree_view_column_new_with_attributes(_("Date"),defense_rendererlog, "text", DATELOG_COL, NULL);
	gtk_tree_view_column_set_resizable(defense_columnlog,TRUE);
        gtk_tree_view_column_set_sizing (defense_columnlog,GTK_TREE_VIEW_COLUMN_FIXED);
        gtk_tree_view_column_set_fixed_width(defense_columnlog,150);
        gtk_tree_view_append_column(GTK_TREE_VIEW(defense_listlog), defense_columnlog);

        defense_rendererlog = gtk_cell_renderer_text_new();
        defense_columnlog = gtk_tree_view_column_new_with_attributes(_("Event"),defense_rendererlog, "text", TYPELOG_COL, NULL);
	gtk_tree_view_column_set_resizable(defense_columnlog,TRUE);
        gtk_tree_view_column_set_sizing (defense_columnlog,GTK_TREE_VIEW_COLUMN_FIXED);
        gtk_tree_view_column_set_fixed_width(defense_columnlog,150);
        gtk_tree_view_append_column(GTK_TREE_VIEW(defense_listlog), defense_columnlog);


        defense_rendererlog = gtk_cell_renderer_text_new();
        defense_columnlog = gtk_tree_view_column_new_with_attributes(_("Result"),defense_rendererlog, "text", RESULTLOG_COL, NULL);
	gtk_tree_view_column_set_resizable(defense_columnlog,TRUE);
        gtk_tree_view_column_set_sizing (defense_columnlog,GTK_TREE_VIEW_COLUMN_FIXED);
        gtk_tree_view_column_set_fixed_width(defense_columnlog,180);
        gtk_tree_view_append_column(GTK_TREE_VIEW(defense_listlog), defense_columnlog);

	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (defense_listlog),FALSE);
	gtk_tree_view_set_tooltip_column(GTK_TREE_VIEW(defense_listlog), RESULTLOG_COL);

        defense_vboxlog = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_widget_show(defense_vboxlog);
        gtk_box_pack_start(GTK_BOX(defense_vboxlog), defence_log_title, TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(defense_vboxlog), defense_swlog, TRUE, TRUE, 0);
        defense_hboxlog = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_widget_show(defense_hboxlog);


	GtkWidget *defence_log_right_layout = gtk_layout_new(NULL, NULL);
        gtk_widget_set_size_request(defence_log_right_layout, 600, 80);
        gtk_widget_show(defence_log_right_layout);
        gtk_container_add(GTK_CONTAINER(defense_hboxlog), defence_log_right_layout);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%ssetting-right.png"),ANTIVIRUS_IMAGE_DIR);

        GtkWidget *defence_log_right_image = gtk_image_new_from_file(image_dir);
        gtk_widget_show(defence_log_right_image);
        gtk_layout_put(GTK_LAYOUT(defence_log_right_layout), defence_log_right_image, 0, 0);
	
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%ssetting-right-mask.png"),ANTIVIRUS_IMAGE_DIR);

	GtkWidget *log_right_mask_image = gtk_image_new_from_file(image_dir);
        gtk_widget_show(log_right_mask_image);
        gtk_layout_put(GTK_LAYOUT(defence_log_right_layout), log_right_mask_image, 0, 0);


        defense_removelog = gtk_button_new();
        gtk_widget_show(defense_removelog);
        gtk_button_set_relief((GtkButton *)defense_removelog,GTK_RELIEF_NONE);
	gtk_widget_set_size_request(defense_removelog,87,39);
        gtk_widget_set_name(defense_removelog,"scan_removelog");

        memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sremove.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *removeimage = gtk_image_new_from_file(image_dir);
        gtk_widget_show(removeimage);
  //      gtk_button_set_image(defense_removelog,removeimage);

        defense_exportlog = gtk_button_new();
        gtk_widget_show(defense_exportlog);
        gtk_button_set_relief((GtkButton *)defense_exportlog,GTK_RELIEF_NONE);
	gtk_widget_set_size_request(defense_exportlog,87,39);
        gtk_widget_set_name(defense_exportlog,"scan_exportlog");


	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sexport.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *exportimage = gtk_image_new_from_file(image_dir);
        gtk_widget_show(exportimage);
//        gtk_button_set_image(defense_exportlog,exportimage);

        defense_select_all_log_button = gtk_check_button_new_with_label(_("select all"));
        g_signal_connect (defense_select_all_log_button, "toggled", G_CALLBACK (select_all_log),defense_listlog);
        gtk_widget_show(defense_select_all_log_button);
        defense_selectionlog=gtk_tree_view_get_selection((GTK_TREE_VIEW(defense_listlog)));
        gtk_tree_selection_set_mode(defense_selectionlog,GTK_SELECTION_SINGLE);
        gtk_box_pack_start(GTK_BOX(defense_vboxlog), defense_select_all_log_button, TRUE, TRUE, 5);

	gtk_layout_put(GTK_LAYOUT(defence_log_right_layout), defense_removelog, 200, 20);
        gtk_layout_put(GTK_LAYOUT(defence_log_right_layout), defense_exportlog, 400, 20);

        gtk_box_pack_start(GTK_BOX(defense_vboxlog), defense_hboxlog, FALSE, TRUE, 0);
        gtk_container_add(GTK_CONTAINER(defense_swlog), defense_listlog);
        gtk_box_pack_start(GTK_BOX (defense_log_list_vbox),defense_vboxlog,FALSE,FALSE,0);

        g_signal_connect(G_OBJECT(defense_removelog), "clicked", G_CALLBACK(remove_defenselog_item), defense_listlog);
     //   g_signal_connect(G_OBJECT(defense_removelog), "enter", G_CALLBACK(enter_removelog), NULL);

               g_signal_connect(G_OBJECT(defense_exportlog), "clicked", G_CALLBACK(import_defense_log),defense_listlog);
       // g_signal_connect(G_OBJECT(defense_exportlog), "enter", G_CALLBACK(enter_exportlog),NULL);

	 gtk_box_pack_start(GTK_BOX (content_box),defense_log_list_vbox,FALSE,FALSE,0);
	gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);

        GtkTreeIter iter;
        GtkTreeModel *defense_model= gtk_tree_view_get_model (GTK_TREE_VIEW(defense_listlog));
        if(!gtk_tree_model_get_iter_first(defense_model,&iter)){
                if(removeimage!=NULL){
                        g_print("开始更换日志图片\n");
                        gtk_widget_destroy(removeimage);
			memset(image_dir,0,sizeof(image_dir));
        		snprintf(image_dir,sizeof(image_dir),_("%sremove-un-use.png"),ANTIVIRUS_IMAGE_DIR);
                        removeimage = gtk_image_new_from_file(image_dir);
                        gtk_widget_show(removeimage);
                        gtk_button_set_image(defense_removelog,removeimage);
			memset(image_dir,0,sizeof(image_dir));
        		snprintf(image_dir,sizeof(image_dir),_("%sexport-un-use.png"),ANTIVIRUS_IMAGE_DIR);
			gtk_image_set_from_file(exportimage,image_dir);
                }
		gtk_widget_set_sensitive(defense_exportlog,FALSE);
                gtk_widget_set_sensitive(defense_removelog,FALSE);
        }

}

void update_log_page(GtkWidget *content_box)
{
	GtkWidget *update_swlog,*update_listlog,*update_vboxlog,*update_hboxlog,*update_select_all_log_button;
        GtkCellRenderer *update_rendererlog;
        GtkTreeViewColumn *update_columnlog;
        GtkWidget *update_removelog,*update_exportlog;
        GtkTreeSelection *update_selectionlog;
	char image_dir[256];
	char log_page_css[256];
        snprintf(log_page_css,sizeof(log_page_css),_("%slog-page.css"),ANTIVIRUS_CSS_DIR);

	  GFile *file=g_file_new_for_path(log_page_css);
        GdkScreen *screen;
        screen = gdk_screen_get_default();
        GtkCssProvider *cssProvider;
        cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_file(cssProvider, file, NULL);

	update_log_list_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_show(update_log_list_vbox);
	 GtkWidget *update_log_title;

	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%slog_title.png"),ANTIVIRUS_IMAGE_DIR);
        update_log_title=gtk_image_new_from_file(image_dir);
        gtk_widget_show(update_log_title);
	update_swlog = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_show(update_swlog);
        gtk_scrolled_window_set_min_content_height((GtkScrolledWindow *)update_swlog,260);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(update_swlog), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
        gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(update_swlog), GTK_SHADOW_ETCHED_IN);
	char avlUpgradeLog[256];
        snprintf(avlUpgradeLog,sizeof(avlUpgradeLog),"%savlUpgradeLog.txt",ANTIVIRUS_LOG_DIR);
	update_listlog = gtk_tree_view_new_with_model(createlogModel(avlUpgradeLog));
	gtk_widget_show(update_listlog);
        update_rendererlog = gtk_cell_renderer_toggle_new();
        g_object_set(G_OBJECT(update_rendererlog),"activatable",TRUE,NULL);
        update_columnlog = gtk_tree_view_column_new_with_attributes("      ",update_rendererlog,"active",LOGTOG_COL,NULL);
	gtk_tree_view_column_set_resizable(update_columnlog,TRUE);
        gtk_tree_view_column_set_sizing (update_columnlog,GTK_TREE_VIEW_COLUMN_FIXED);
        gtk_tree_view_column_set_fixed_width(update_columnlog,50);
        gtk_tree_view_append_column(GTK_TREE_VIEW(update_listlog),update_columnlog);
        g_signal_connect (update_rendererlog, "toggled", G_CALLBACK (select_toggled),update_listlog);

	update_rendererlog = gtk_cell_renderer_text_new();
        update_columnlog = gtk_tree_view_column_new_with_attributes(_("Date"),update_rendererlog, "text", DATELOG_COL, NULL);
	gtk_tree_view_column_set_resizable(update_columnlog,TRUE);
        gtk_tree_view_column_set_sizing (update_columnlog,GTK_TREE_VIEW_COLUMN_FIXED);
        gtk_tree_view_column_set_fixed_width(update_columnlog,150);
        gtk_tree_view_append_column(GTK_TREE_VIEW(update_listlog), update_columnlog);

        update_rendererlog = gtk_cell_renderer_text_new();
        update_columnlog = gtk_tree_view_column_new_with_attributes(_("Event"),update_rendererlog, "text", TYPELOG_COL, NULL);
	gtk_tree_view_column_set_resizable(update_columnlog,TRUE);
        gtk_tree_view_column_set_sizing (update_columnlog,GTK_TREE_VIEW_COLUMN_FIXED);
        gtk_tree_view_column_set_fixed_width(update_columnlog,150);
        gtk_tree_view_append_column(GTK_TREE_VIEW(update_listlog), update_columnlog);


        update_rendererlog = gtk_cell_renderer_text_new();
        update_columnlog = gtk_tree_view_column_new_with_attributes(_("Result"),update_rendererlog, "text", RESULTLOG_COL, NULL);
	gtk_tree_view_column_set_resizable(update_columnlog,TRUE);
        gtk_tree_view_column_set_sizing (update_columnlog,GTK_TREE_VIEW_COLUMN_FIXED);
        gtk_tree_view_column_set_fixed_width(update_columnlog,180);
        gtk_tree_view_append_column(GTK_TREE_VIEW(update_listlog), update_columnlog);
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (update_listlog),FALSE);

	update_vboxlog = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_show(update_vboxlog);
        gtk_box_pack_start(GTK_BOX(update_vboxlog), update_swlog, TRUE, TRUE, 0);
        update_hboxlog = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_widget_show(update_hboxlog);

	GtkWidget *update_log_right_layout = gtk_layout_new(NULL, NULL);
        gtk_widget_set_size_request(update_log_right_layout, 600, 80);
        gtk_widget_show(update_log_right_layout);
        gtk_container_add(GTK_CONTAINER(update_hboxlog), update_log_right_layout);
	
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%ssetting-right.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *update_log_right_image = gtk_image_new_from_file(image_dir);
        gtk_widget_show(update_log_right_image);
        gtk_layout_put(GTK_LAYOUT(update_log_right_layout), update_log_right_image, 0, 0);
	
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%ssetting-right-mask.png"),ANTIVIRUS_IMAGE_DIR);
	GtkWidget *log_right_mask_image = gtk_image_new_from_file(image_dir);
        gtk_widget_show(log_right_mask_image);
        gtk_layout_put(GTK_LAYOUT(update_log_right_layout), log_right_mask_image, 0, 0);


        update_removelog = gtk_button_new();
	gtk_widget_show(update_removelog);
        gtk_button_set_relief((GtkButton *)update_removelog,GTK_RELIEF_NONE);
	gtk_widget_set_size_request(update_removelog,87,39);
        gtk_widget_set_name(update_removelog,"scan_removelog");

	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sremove.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *removeimage = gtk_image_new_from_file(image_dir);
       // gtk_widget_show(removeimage);
//	gtk_button_set_image(update_removelog,removeimage);

	update_exportlog = gtk_button_new();
        gtk_widget_show(update_exportlog);
        gtk_button_set_relief((GtkButton *)update_exportlog,GTK_RELIEF_NONE);
	gtk_widget_set_size_request(update_exportlog,87,39);
        gtk_widget_set_name(update_exportlog,"scan_exportlog");
	
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sexport.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *exportimage = gtk_image_new_from_file(image_dir);
        gtk_widget_show(exportimage);
	gtk_button_set_image(update_exportlog,exportimage);
	
        update_select_all_log_button = gtk_check_button_new_with_label (_("select all"));
        g_signal_connect (update_select_all_log_button, "toggled", G_CALLBACK (select_all_log),update_listlog);
        gtk_widget_show(update_select_all_log_button);
        update_selectionlog=gtk_tree_view_get_selection((GTK_TREE_VIEW(update_listlog)));
        gtk_tree_selection_set_mode(update_selectionlog,GTK_SELECTION_SINGLE);
        gtk_layout_put(GTK_LAYOUT(update_log_right_layout), update_removelog, 200, 20);
        gtk_layout_put(GTK_LAYOUT(update_log_right_layout), update_exportlog, 400, 20);

        gtk_box_pack_start(GTK_BOX(update_vboxlog), update_select_all_log_button, TRUE, TRUE, 5);
        gtk_box_pack_start(GTK_BOX(update_vboxlog), update_hboxlog, FALSE, TRUE, 0);
        gtk_container_add(GTK_CONTAINER(update_swlog), update_listlog);

        gtk_box_pack_start(GTK_BOX (update_log_list_vbox),update_log_title,FALSE,FALSE,0);
        gtk_box_pack_start(GTK_BOX (update_log_list_vbox),update_vboxlog,FALSE,FALSE,0);

        g_signal_connect(G_OBJECT(update_removelog), "clicked", G_CALLBACK(remove_updatelog_item), update_listlog);
       // g_signal_connect(G_OBJECT(update_removelog), "enter", G_CALLBACK(enter_removelog), NULL);

        g_signal_connect(G_OBJECT(update_exportlog), "clicked", G_CALLBACK(import_update_log),update_listlog);
       // g_signal_connect(G_OBJECT(update_exportlog), "enter", G_CALLBACK(enter_exportlog),NULL);

	gtk_box_pack_start(GTK_BOX (content_box),update_log_list_vbox,FALSE,FALSE,0);
	gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);

        GtkTreeIter iter;
        GtkTreeModel *update_model= gtk_tree_view_get_model (GTK_TREE_VIEW(update_listlog));
        if(!gtk_tree_model_get_iter_first(update_model,&iter)){
                if(removeimage!=NULL){
                        g_print("开始更换日志图片\n");
                        gtk_widget_destroy(removeimage);
			memset(image_dir,0,sizeof(image_dir));
		        snprintf(image_dir,sizeof(image_dir),_("%sremove-un-use.png"),ANTIVIRUS_IMAGE_DIR);
                        removeimage = gtk_image_new_from_file(image_dir);
                        gtk_widget_show(removeimage);
		//	gtk_button_set_image(update_removelog,removeimage);
			memset(image_dir,0,sizeof(image_dir));
		        snprintf(image_dir,sizeof(image_dir),_("%sexport-un-use.png"),ANTIVIRUS_IMAGE_DIR);
			gtk_image_set_from_file(exportimage,image_dir);
                }
		gtk_widget_set_sensitive(update_exportlog,FALSE);
                gtk_widget_set_sensitive(update_removelog,FALSE);
        }
	
}
void leave_logbutton(GtkWidget *widget,gpointer *data){
	char image_dir[256];
        if(area==3){
                gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
        snprintf(image_dir,sizeof(image_dir),_("%slog_enter.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *leave_antibutton_image = gtk_image_new_from_file(image_dir);

       // gtk_button_set_image(widget,leave_antibutton_image);
                return ;
        }
        gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%slogging.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *leave_setbutton_image = gtk_image_new_from_file(image_dir);

       // gtk_button_set_image(widget,leave_setbutton_image);
}
void enter_logbutton(GtkWidget *widget,gpointer *data){

        //gtk_button_set_image(widget,NULL);
	g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_logbutton),NULL);
 }
