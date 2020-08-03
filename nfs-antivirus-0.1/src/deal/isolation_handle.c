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
#include <string.h>
#include "../writelog.h"
#include "../page/isolation-recovery-page.h"
#include "../page/setting-page.h"
#include "../function.h"
#define _(STRING) gettext(STRING)
extern     GtkWidget *recovery_dialog;
extern GtkWidget *devide_content_box;
int not_tog_warning(GtkWidget *list,int tog,gboolean is_whitelist,int delete_or_add_white_list)
{
        GtkListStore *store;
        GtkTreeModel *model;
        GtkTreeIter iter;
        gboolean value;
        int n=0;
	gint rec_flag;
        store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
        model = gtk_tree_view_get_model (GTK_TREE_VIEW(list));
        gtk_tree_model_get_iter_first(model,&iter);
        do
	{
                gtk_tree_model_get(model,&iter,tog,&value,-1);
                if(value)
		{
                        n++;
                }
        }while(gtk_tree_model_iter_next(model,&iter));
        if(n==0)
        {
        	show_warn_info(_("Any item not selected,please select item to deal again!"),1);
        }
	if(is_whitelist)
        {
                if(n>0)
		{
			if(delete_or_add_white_list==1)
			{
	                        rec_flag=show_warn_info(_("this deal will deal thread from one file!"),2);
			}
			if(delete_or_add_white_list==2)
			{
				rec_flag=show_warn_info(_("this deal will add thread from one file!"),1);
			}
			return rec_flag;
		}

        }

        return -1;

}

void rescovery_func(GtkWidget *widget,gpointer *data)
{
        if(get_select_items_num(-1)==0)
                not_tog_warning(devide_file_list_list,DEVIDE_FILE_LIST_TOG_COL,FALSE,3);
        else
                recovery(NULL,NULL);

}

void recovery_select_items(GtkWidget *widget,gpointer *data)
{
	int flag;
	if(data==0&&gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_button_first))||data==1)
	{
		flag=0;
	}
	if(data==0&&gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_button_second)))
	{
		flag=1;
	}
	get_select_items_num(flag);
        start_rescovery();
	if(recovery_dialog!=NULL)
		gtk_widget_destroy(recovery_dialog);
	if(devide_file_box!=NULL)
                gtk_widget_destroy(devide_file_box);
        look_devide_file_settings(devide_content_box);
	
}
void start_rescovery()
{
    	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(trust_checkbutton)))
	{
		GtkListStore *store;
        	GtkTreeModel *model;
        	GtkTreeIter iter;
        	gboolean value;
        	char *file_name;
        	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(devide_file_list_list)));
        	model = gtk_tree_view_get_model (GTK_TREE_VIEW(devide_file_list_list));
        	gtk_tree_model_get_iter_first(model,&iter);
		do
		{
			gtk_tree_model_get(model,&iter,DEVIDE_FILE_LIST_TOG_COL,&value,-1);
                	gtk_tree_model_get(model,&iter,DEVIDE_FILE_LIST_NAME_COL,&file_name,-1);
                	if(value)
			{
				FILE *pBufSize;
				char resultBuffer[256];
				char cmd[1024];
				snprintf(cmd,sizeof(cmd),"%sadd_white_list.sh %s",ANTIVIRUS_BIN_WHITE_DIR,file_name);
				if((pBufSize = popen(cmd,"r")) == NULL)
	    			{
		            		return;
	    			}
	    			fgets(resultBuffer,256,pBufSize);
	    			if(strncmp(resultBuffer,"1",1) == 0)
	    			{
	        			fclose(pBufSize);
	        			continue;
    				}
				else
				{
					char style[10];
					is_file(file_name,style);	
					char file_size[100];
					char file_time[100];
					if(! strcmp(style,"1"))
					{
						strcpy(file_size,"null");
						strcpy(file_time,"null");
					}
					else
					{
						get_file_size(file_name,file_size);
        					file_size[strlen(file_size)-2] ='\0';
						get_file_modify_time(file_name,file_time);
					}

					char file_info[1024];
					char white_list_temp[1024];
					strcat_white_list(file_info,style,file_name,file_size,file_time);
					snprintf(white_list_temp,sizeof(white_list_temp),"%swhite_list.txt",ANTIVIRUS_CONF_DIR);
					write_white_list(white_list_temp,file_info);		
				}
			}
		}	
		while (gtk_tree_model_iter_next(model,&iter));
		gtk_widget_destroy(box);
    		white_list_settings(while_list_content_box);	
    	}	
    	else
	{
		return;
    	}
}
