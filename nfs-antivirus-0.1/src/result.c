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
#include "antivirus.h"
#include "filechooser.h"
#include "writelog.h"
#include "sharememory.h"
#include "upgradeFunction.h"
#include "scan.h"
#include "detect_usb/detect_usb.h"
#include "function.h"
#include "upgrade/upgradeself.h"
#include "resultcallbackFunc.h"
#include "result_detail.h"
#include "result.h"
#include "page/setting-page.h"
#define _(STRING) gettext(STRING)
extern struct ScanInfo scanInfo;
extern GtkWidget *table7;
extern GtkWidget *detail_result_table;
extern int area;
extern GtkWidget *thingbox;
int flag;
extern fileNum;
GtkWidget *list;
GtkWidget *result_tabletotal;

struct ScanResults
{
            char scan_time[30];
            char file_name[1024];
            char threat_num[10];
            char threat_name[100];
            struct ScanResults *next;
};

struct ScanResults *phead_scanResults=NULL;

GtkWidget *tog_all_check_box,*result_deal_vbox;
char g_current_user[256]; //当前启动杀毒软件用户
void result_switch_result_detail()
{
	gtk_widget_destroy(table7);
	area = 11;
	gtk_widget_show(detail_result_table);
}
void free_list_memory ()
{
    struct ScanResults *pnext;
    while(phead_scanResults!=NULL)
    {
       pnext = phead_scanResults->next;
       free(phead_scanResults);
       phead_scanResults = NULL;
       phead_scanResults = pnext;
    }

}

GtkTreeModel* createModel()
{
        GtkTreeIter iter;
        GtkListStore *store;
        char c = 0;
        int line = 0;
        struct ScanResults *pcurrent,*ptempNode;
        char tmp_scan_results[512];
        snprintf(tmp_scan_results,sizeof(tmp_scan_results),"%sscan_results.txt",ANTIVIRUS_TEMP_DIR);
        store = gtk_list_store_new(5,G_TYPE_BOOLEAN,G_TYPE_STRING, G_TYPE_STRING,G_TYPE_INT,G_TYPE_STRING);   //2列
        FILE *fp=fopen(tmp_scan_results,"rw");
        if(fp == NULL)
        {
            flag=0;    //no threa
            return GTK_TREE_MODEL(store);

        }

        gchar buf[1024];
        gint i= 0;
        while(fgets(buf,1023,fp) != NULL)
        {
                int in=0;
                char *p[20];
                if (buf==NULL)
                {
                        flag=0;
                }
                else
                {
                        flag=1;
                }

                char *buffer = buf;
                result_count = 0;
                while((p[in]=strtok(buffer,";"))!=NULL)
                {
                        in++;
                        buffer=NULL;
                }
                struct ScanResults *newNode =(struct ScanResults *) malloc(sizeof(struct ScanResults)) ;
                if(i == 0)
                {
                    phead_scanResults = newNode;
                    phead_scanResults->next = NULL;
                    pcurrent = newNode;

                }
                if ( newNode->scan_time)
                {
                    memset(newNode->scan_time,0,sizeof(newNode->scan_time));
                    strncpy(newNode->scan_time,p[0],29);
                }
                if (newNode->file_name)
                {
                    memset(newNode->file_name,0,sizeof(newNode->file_name));
                    strncpy(newNode->file_name,p[1],1023);
                }
                if (newNode->threat_num )
                {
                    memset(newNode->threat_num,0,sizeof(newNode->threat_num));
                    strncpy(newNode->threat_num,p[2],9);
                }
                if (newNode->threat_name)
                {
                    memset(newNode->threat_name,0,sizeof(newNode->threat_name));
                    strncpy(newNode->threat_name,p[3],99);
                }
                ptempNode = pcurrent;
                ptempNode->next = newNode;
                pcurrent = newNode ;
                newNode->next = NULL;
                i++;

        }
        fclose(fp);
        pcurrent = phead_scanResults;
        ptempNode = phead_scanResults;
        for(result_count=0;result_count< i;result_count++)
        {
               if(pcurrent!=NULL)
               {

                gtk_list_store_append(store,&iter);
                gtk_list_store_set(store,&iter,
                    TOG_COL,FALSE,
                    DATE_COL,pcurrent->scan_time,     //PIXBUF_COL也可以用0代替
                    NAME_COL,pcurrent->file_name,
                    THREATNUM_COL,1,
                    THREATNAME_COL,pcurrent->threat_name,
                    -1);
                ptempNode = pcurrent->next;
                pcurrent = ptempNode;
                }
       }
       return GTK_TREE_MODEL(store);
}

void tog_all(GtkTreeView *treeview)
{
        GtkListStore *store;
	GtkTreeModel *model;
        GtkTreeIter iter;
        model = gtk_tree_view_get_model (GTK_TREE_VIEW(list));
        store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
        gtk_tree_model_get_iter_first(model,&iter);
        gboolean use_select_all=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tog_all_check_box));
        do
        {
                gtk_list_store_set(store,&iter,0,use_select_all,-1);
        }
        while (gtk_tree_model_iter_next(model,&iter));
}


void result(int auto_handle)
{
	GtkWidget *detail_fix,*resultlabel,*scantype_label,*detail_scanType_label,*totaltime_label,*detail_totaltime_label,*totalobject_label,*detail_totalobject_label;
        GtkWidget *sw;
        GtkWidget *remove,*look_devide_file_button;
        GtkWidget *back_to_detail_result_button;
        GtkWidget *detail_vbox;
        GtkWidget *hbox,*devide_hbox,*bottom_hbox;
        GtkCellRenderer *renderer,*renderer_tog;
        GtkTreeViewColumn *column,*column_tog;
        GtkListStore *store;
        GtkWidget *title_image;
        GtkWidget *fix;
        GtkTreeSelection *selection;
	int auto_deel=auto_handle;
	char image_threat[512];
        char css_result[512];
        char detail_scanType[100],scantype[10];
        table7 = gtk_grid_new();
	gtk_widget_show(table7);
        result_tabletotal = gtk_grid_new();
	gtk_grid_attach((GtkGrid *)table7,result_tabletotal,0,0,1,1);
	gtk_widget_show(result_tabletotal);
        snprintf(image_threat,sizeof(image_threat),_("%sthe-scan-did-not-reveal-any-threat.png"),ANTIVIRUS_IMAGE_DIR);
	GtkWidget *resultlabel_image = gtk_image_new_from_file(image_threat);
        fix = gtk_fixed_new();
	gtk_grid_attach((GtkGrid *)result_tabletotal,fix,0,0,1,2);
	gtk_widget_show(fix);
        GtkWidget *layout;

		char set_file_path[1024];
		snprintf(set_file_path,sizeof(set_file_path),"%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);


        if (auto_deel == AUTO_DEAL)
        {memset(image_threat,0,sizeof(image_threat));
       snprintf(image_threat,sizeof(image_threat),_("%sauto-deal-result.png"),ANTIVIRUS_IMAGE_DIR);
                gtk_image_set_from_file(resultlabel_image,image_threat);

        }
        if(auto_deel == MANUAL_DEAL)
        { memset(image_threat,0,sizeof(image_threat));
       snprintf(image_threat,sizeof(image_threat),_("%sneed-deal.png"),ANTIVIRUS_IMAGE_DIR);
                gtk_image_set_from_file(resultlabel_image,image_threat);

        }

        layout = gtk_layout_new(NULL, NULL);
        gtk_widget_set_size_request(layout, 800, 106);
        gtk_layout_put(GTK_LAYOUT(layout), resultlabel_image, 0, 0);
	gtk_widget_show(resultlabel_image);
        gtk_fixed_put(GTK_FIXED(fix),layout,0,0);
	gtk_widget_show(layout);
       snprintf(css_result,sizeof(css_result),"%sresult-chinese.css",ANTIVIRUS_CSS_DIR);
	GFile *file=g_file_new_for_path(css_result);
        GdkScreen *screen;
        screen = gdk_screen_get_default();
        GtkCssProvider *cssProvider;
        cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_file(cssProvider, file, NULL);


        back_to_detail_result_button=gtk_button_new();
        gtk_button_set_relief((GtkButton *)back_to_detail_result_button,GTK_RELIEF_NONE);
         memset(image_threat,0,sizeof(image_threat));
       snprintf(image_threat,sizeof(image_threat),_("%sback_menu.png"),ANTIVIRUS_IMAGE_DIR);
       gtk_widget_show(back_to_detail_result_button);
               gtk_button_set_relief((GtkButton *)back_to_detail_result_button,GTK_RELIEF_NONE);
	                gtk_widget_set_size_request(back_to_detail_result_button,69,37);
      //  gtk_button_set_image(GTK_BUTTON(back_to_detail_result_button),gtk_image_new_from_file(image_threat));
        gtk_layout_put(GTK_LAYOUT(layout), (GtkWidget *)back_to_detail_result_button, 40, 27);
//	gtk_widget_show(back_to_detail_result_button);
         g_signal_connect(G_OBJECT(back_to_detail_result_button),"clicked",G_CALLBACK(switch_to_mainmenu),NULL);

	result_deal_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_grid_attach((GtkGrid *)result_tabletotal,result_deal_vbox,0,2,1,8);
	gtk_widget_show(result_deal_vbox);
	sw = gtk_scrolled_window_new(NULL, NULL);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
        gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(sw), GTK_SHADOW_ETCHED_IN);
        gtk_scrolled_window_set_min_content_height((GtkScrolledWindow *)sw,350);
	gtk_box_pack_start(GTK_BOX(result_deal_vbox), sw, TRUE, TRUE, 5);
        list = gtk_tree_view_new_with_model(createModel());
        gtk_widget_set_size_request(list, 700, 300);
	gtk_container_add(GTK_CONTAINER(sw), list);
	renderer_tog = gtk_cell_renderer_toggle_new();
        g_object_set(G_OBJECT(renderer_tog),"activatable",TRUE,NULL);
        column_tog = gtk_tree_view_column_new_with_attributes(_("      "),renderer_tog,"active",TOG_COL,NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(list),column_tog);
        g_signal_connect (renderer_tog, "toggled", G_CALLBACK (select_toggled),list);
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes(_("Date"),renderer, "text", DATE_COL, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

        renderer = gtk_cell_renderer_text_new();
	
        column = gtk_tree_view_column_new_with_attributes(_("Name"),renderer, "text", NAME_COL, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes(_("Threatnum"),renderer, "text", THREATNUM_COL, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes(_("Threatname"),renderer, "text", THREATNAME_COL, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);
	gtk_widget_show_all(sw);
        int file_devide = get_antivirus_setting_item_inttype("file-into-deivide",set_file_path);
	bottom_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_widget_show(bottom_hbox);
        hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_widget_show(hbox);
	devide_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_widget_show(devide_hbox);
        tog_all_check_box=gtk_check_button_new_with_label (_("select all"));
	gtk_widget_show(tog_all_check_box);
	remove = gtk_button_new();
	gtk_widget_show(remove);
        gtk_button_set_relief((GtkButton *)remove,GTK_RELIEF_NONE);
        memset(image_threat,0,sizeof(image_threat));
       snprintf(image_threat,sizeof(image_threat),_("%sdel_thread.png"),ANTIVIRUS_IMAGE_DIR);

        GtkWidget *removeimage = gtk_image_new_from_file(image_threat);
	gtk_widget_show(removeimage);
        gtk_container_add(GTK_CONTAINER(remove), removeimage);
	GtkWidget *add_white_list_button = gtk_button_new();
	gtk_widget_show(add_white_list_button);
         memset(image_threat,0,sizeof(image_threat));
       snprintf(image_threat,sizeof(image_threat),_("%sadd_white_list.png"),ANTIVIRUS_IMAGE_DIR);

	GtkWidget *add_white_list_image = gtk_image_new_from_file(image_threat);
	gtk_widget_show(add_white_list_image);
	gtk_container_add(GTK_CONTAINER(add_white_list_button),add_white_list_image);
	gtk_button_set_relief((GtkButton *)add_white_list_button,GTK_RELIEF_NONE);
	look_devide_file_button= gtk_button_new();
	gtk_widget_show(look_devide_file_button);
         memset(image_threat,0,sizeof(image_threat));
       snprintf(image_threat,sizeof(image_threat),_("%slook_devide_file.png"),ANTIVIRUS_IMAGE_DIR);

	GtkWidget *look_devide_file_image = gtk_image_new_from_file(image_threat);
	gtk_widget_show(look_devide_file_image);
        gtk_container_add(GTK_CONTAINER(look_devide_file_button),look_devide_file_image);
        gtk_button_set_relief((GtkButton *)look_devide_file_button,GTK_RELIEF_NONE);
        gtk_box_pack_start(GTK_BOX(hbox), tog_all_check_box, FALSE, TRUE, 3);
	gtk_box_pack_start(GTK_BOX(hbox), remove, FALSE, TRUE, 3);
	gtk_box_pack_start(GTK_BOX(hbox), add_white_list_button, FALSE, TRUE, 3);
        gtk_box_pack_start(GTK_BOX(devide_hbox), look_devide_file_button, FALSE, TRUE, 3);
        gtk_box_pack_start(GTK_BOX(bottom_hbox),hbox, FALSE, TRUE, 3);
        gtk_box_pack_end(GTK_BOX(bottom_hbox),devide_hbox, FALSE, TRUE, 3);
        gtk_box_pack_start(GTK_BOX(result_deal_vbox),bottom_hbox, FALSE, TRUE, 12);

        
        selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
        gtk_tree_selection_set_mode(selection,GTK_SELECTION_SINGLE);
        g_signal_connect(G_OBJECT(tog_all_check_box), "toggled", G_CALLBACK(tog_all), list);
        g_signal_connect(G_OBJECT(remove), "clicked", G_CALLBACK(remove_item), NULL);
	g_signal_connect(G_OBJECT(add_white_list_button), "clicked", G_CALLBACK(add_white_list), NULL);
        g_signal_connect(G_OBJECT(look_devide_file_button), "clicked", G_CALLBACK(switch_to_devide_list), NULL);
	
        gtk_container_add(GTK_CONTAINER(thingbox),table7);
        area = 7;
	gtk_widget_set_name(back_to_detail_result_button,"result_back_to_mainmenu_button");
        gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);
        if(auto_deel == AUTO_DEAL)
        {
		gtk_tree_view_remove_column (GTK_TREE_VIEW(list),column_tog);
		gtk_widget_hide(hbox);
		if(file_devide!=1)
		{
			gtk_widget_hide(devide_hbox);
		}
              
        }
	else
	{
		GtkTreeIter iter;
	        GtkTreeModel *model= gtk_tree_view_get_model (GTK_TREE_VIEW(list));
		if(!gtk_tree_model_get_iter_first(model,&iter)){
		if(removeimage!=NULL){
                     memset(image_threat,0,sizeof(image_threat));
       snprintf(image_threat,sizeof(image_threat),_("%sremove-un-use.png"),ANTIVIRUS_IMAGE_DIR);

			removeimage = gtk_image_new_from_file(image_threat);
			gtk_widget_show(removeimage);
			gtk_button_set_image(remove,removeimage);
		}
		if(add_white_list_image!=NULL){
                   memset(image_threat,0,sizeof(image_threat));
       snprintf(image_threat,sizeof(image_threat),_("%sadd_white_list-un-use.png"),ANTIVIRUS_IMAGE_DIR);

        		add_white_list_image = gtk_image_new_from_file(image_threat);
			gtk_widget_show(add_white_list_image);
			gtk_button_set_image(add_white_list_button,add_white_list_image);
		}	
                gtk_widget_set_sensitive(remove,FALSE);
		gtk_widget_set_sensitive(add_white_list_button,FALSE);
        	}
		gtk_widget_hide(devide_hbox);
	}

}
