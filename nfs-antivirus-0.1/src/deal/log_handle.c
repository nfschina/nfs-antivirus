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
#include "../scan.h"
#include "../refresh.h"
#include <sys/stat.h>
#include "../result.h"
#include "../filechooser.h"
#include "../writelog.h"
#include "../sharememory.h"
#include "../upgradeFunction.h"
#include "../detect_usb/detect_usb.h"
#include "../function.h"
#include "../antivirus.h"
#include "../deal/log_handle.h"
#include "../page/log-page.h"
#define _(STRING) gettext(STRING)
extern GtkWidget *vbox1,*logfix1,*vboxlog,*hboxlog,*table3,*update_log_content_box,*defense_log_content_box,*scan_log_content_box,*log_list_vbox;
extern char * combo_text;
extern GtkWidget *scan_log_list_vbox,*defense_log_list_vbox,*update_log_list_vbox;
extern GtkWindow *window;
GtkWidget *filew;
int import_type;
int cancle_scan_log_improt=0;
char *improt_filename;
char *import_log_filename;
extern char g_current_user[256]; //当前启动杀毒软件用户
void import_log_func(int type,char* filename){
        char importCmd[1024];
        if(type==1){
        snprintf(importCmd,1024,"cp %s%s_log_file %s ",ANTIVIRUS_LOG_DIR,g_current_user,filename);
        system(importCmd);}
        if(type==2){
        printf("开始复制防御日志\n");
        snprintf(importCmd,1024,"cp %s%s_defence_log %s ",ANTIVIRUS_LOG_DIR,g_current_user,filename);
        system(importCmd);
        }
        if(type==3){
        printf("开始复制防御日志\n");
        snprintf(importCmd,1024,"cp %savlUpgradeLog.txt %s ",ANTIVIRUS_LOG_DIR,filename);
        system(importCmd);
        }


}

void import_scan_log(GtkWidget *widget,gpointer *data)
{
	GSList * importList = NULL;
   char transformPath[1024]= {0};
    filew =  gtk_file_chooser_dialog_new (_("File selection"),GTK_WINDOW(window),
                                                GTK_FILE_CHOOSER_ACTION_SAVE,
                                                GTK_STOCK_CANCEL,
                                                GTK_RESPONSE_CANCEL,
                                                GTK_STOCK_OK,
                                                GTK_RESPONSE_OK,
                                                NULL);
   if(gtk_dialog_run (GTK_DIALOG (filew)) == GTK_RESPONSE_OK)
   {
        importList = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(filew));
       if( importList == NULL )
       {
            gtk_widget_destroy(filew);
            return ;
       }
        if(access(importList->data,F_OK)==0){

                improt_filename = importList->data;
                import_type = 1;
                show_warn_info(_("file exist,sure to cover"),2);
        }
        else{
        import_log_func(1,importList->data);
        gtk_widget_destroy(filew);
	}
        }
   else{
        gtk_widget_destroy(filew);
   }	
}
void import_defense_log(GtkWidget *widget,gpointer *data)
{
	GSList * importList = NULL;
   char transformPath[1024]= {0};
   filew =  gtk_file_chooser_dialog_new (_("File selection"),GTK_WINDOW(window),
                                                GTK_FILE_CHOOSER_ACTION_SAVE,
                                                GTK_STOCK_CANCEL,
                                                GTK_RESPONSE_CANCEL,
                                                GTK_STOCK_OK,
                                                GTK_RESPONSE_OK,
                                                NULL);
   if(gtk_dialog_run (GTK_DIALOG (filew)) == GTK_RESPONSE_OK)
   {
        importList = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(filew));
       if( importList == NULL )
       {
            gtk_widget_destroy(filew);
            return ;
       }
        if(access(importList->data,F_OK)==0){

                improt_filename = importList->data;
                import_type = 2;
                show_warn_info(_("file exist,sure to cover"),2);
             
        }    
        else{
        import_log_func(2,importList->data);
        gtk_widget_destroy(filew);
	}

        }
   else{
        gtk_widget_destroy(filew);
//      break;
   }	
}
void import_update_log(GtkWidget *widget,gpointer *data)
{
	GSList * importList = NULL;
   char transformPath[1024]= {0};
   filew =  gtk_file_chooser_dialog_new (_("File selection"),GTK_WINDOW(window),
                                                GTK_FILE_CHOOSER_ACTION_SAVE,
                                                GTK_STOCK_CANCEL,
                                                GTK_RESPONSE_CANCEL,
                                                GTK_STOCK_OK,
                                                GTK_RESPONSE_OK,
                                                NULL);
   if(gtk_dialog_run (GTK_DIALOG (filew)) == GTK_RESPONSE_OK)
   {
        importList = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(filew));
       if( importList == NULL )
       {
            gtk_widget_destroy(filew);
            return ;
       }
        if(access(importList->data,F_OK)==0){

                improt_filename = importList->data;
                import_type = 3;
                show_warn_info(_("file exist,sure to cover"),2);

        }
        else{
        import_log_func(3,importList->data);
        gtk_widget_destroy(filew);
	}

        }
   else{
        gtk_widget_destroy(filew);
//      break;
   }

}


/*删除日志*/
void remove_scanlog_item(GtkWidget * widget, gpointer *data)
{
        int i = 0;
        GtkTreeModel *model;
        GtkTreeIter iter;
        char *value;
        char getResult[50];
        int row[50]={0};
	char scanlog_temp[1024];
	snprintf(scanlog_temp,sizeof(scanlog_temp),"%s%s_log_file",ANTIVIRUS_LOG_DIR,g_current_user);
	strcpy(log_file_path,scanlog_temp);
        model = gtk_tree_view_get_model(GTK_TREE_VIEW(data));
	not_tog_warning(data,LOGTOG_COL,FALSE,3);
	
        if (gtk_tree_model_get_iter_first(model, &iter) == FALSE)
                return;
        do
	{
                i++;
                gtk_tree_model_get(model,&iter,LOGTOG_COL,&value,-1);
                if(value)
		{
                        char cos[200];
                        snprintf(cos,sizeof(cos),"sed -i '%dd' %s 2>&1",i,log_file_path);
                        system(cos);
                        i--;
                }
        }
        while (gtk_tree_model_iter_next(model,&iter));
	if(scan_log_list_vbox!=NULL)
		gtk_widget_destroy(scan_log_list_vbox);
	
	scan_log_page(scan_log_content_box);

}
void remove_defenselog_item(GtkWidget * widget, gpointer *data)
{
        int i = 0;
        GtkTreeModel *model;
        GtkTreeIter iter;
        char *value;
        char getResult[50];
        int row[50]={0};
	char defenselog_temp[1024];
	snprintf(defenselog_temp,sizeof(defenselog_temp),"%s%s_defence_log",ANTIVIRUS_LOG_DIR,g_current_user);
        strcpy(log_file_path,defenselog_temp);
        model = gtk_tree_view_get_model(GTK_TREE_VIEW(data));
        not_tog_warning(data,LOGTOG_COL,FALSE,3);

        if (gtk_tree_model_get_iter_first(model, &iter) == FALSE)
                return;
        do
	{
                i++;
                gtk_tree_model_get(model,&iter,LOGTOG_COL,&value,-1);
                if(value)
		{
                        char cos[200];
                        snprintf(cos,sizeof(cos),"sed -i '%dd' %s 2>&1",i,log_file_path);
                        system(cos);
                        i--;
                }
        }
        while (gtk_tree_model_iter_next(model,&iter));
        if(defense_log_list_vbox!=NULL)
                gtk_widget_destroy(defense_log_list_vbox);

        defense_log_page(defense_log_content_box);

}
void remove_updatelog_item(GtkWidget * widget, gpointer *data)
{
        int i = 0;
        GtkTreeModel *model;
        GtkTreeIter iter;
        char *value;
        char getResult[50];
        int row[50]={0};
	char updatelog_temp[1024];
	snprintf(updatelog_temp,sizeof(updatelog_temp),"%savlUpgradeLog.txt",ANTIVIRUS_LOG_DIR);
        strcpy(log_file_path,updatelog_temp);
        model = gtk_tree_view_get_model(GTK_TREE_VIEW(data));
        not_tog_warning(data,LOGTOG_COL,FALSE,3);

        if (gtk_tree_model_get_iter_first(model, &iter) == FALSE)
                return;
        do
	{
                i++;
                gtk_tree_model_get(model,&iter,LOGTOG_COL,&value,-1);
                if(value)
		{
                        char cos[200];
                        snprintf(cos,sizeof(cos),"sed -i '%dd' %s 2>&1",i,log_file_path);
                        system(cos);
                        i--;
                }
        }
        while (gtk_tree_model_iter_next(model,&iter));
        if(update_log_list_vbox!=NULL)
                gtk_widget_destroy(update_log_list_vbox);

        update_log_page(update_log_content_box);

}
void leave_removelog(GtkWidget *widget,gpointer data)
{
	char removebutton_image_temp[1024];
	snprintf(removebutton_image_temp,sizeof(removebutton_image_temp),_("%sremove.png"),ANTIVIRUS_IMAGE_DIR);
	gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
        GtkWidget *removebutton_image = gtk_image_new_from_file(removebutton_image_temp);
        gtk_button_set_image(widget,removebutton_image);
}

void enter_removelog(GtkWidget *widget,gpointer data)
{
        gtk_button_set_image(widget,NULL);
        g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_removelog),NULL);
}

void leave_exportlog(GtkWidget *widget,gpointer data)
{
	char exportbutton_image_temp[1024];
	snprintf(exportbutton_image_temp,sizeof(exportbutton_image_temp),_("%sexport.png"),ANTIVIRUS_IMAGE_DIR);
        gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
        GtkWidget *exportbutton_image = gtk_image_new_from_file(exportbutton_image_temp);
        gtk_button_set_image(widget,exportbutton_image);
}

void enter_exportlog(GtkWidget *widget,gpointer data)
{
        gtk_button_set_image(widget,NULL);
        g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_exportlog),NULL);
}

