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
#include <string.h>
#include "result.h"
#include "function.h"
#include "sharememory.h"
#include "./deal/setting_handler.h"
#include "./page/setting-page.h"
#include "./page/home-page.h"
#define _(STRING) gettext(STRING)
extern GtkWidget *list,*real_protect_result_list, *tog_all_check_button,*table7,*table2,*layout_main,*devide_content_box;
extern GtkWidget *real_protect_result_table;
extern GtkWidget *result_tabletotal;
extern int area;
extern GtkWidget *window;
extern GtkWidget *versionimage, *title_layout;
GtkWidget *tog_all_check_box,*progress_win,*del_progress_bar,*delete_progress_value_label;
int delete_count = 0;
int delete_count_copy=0;
extern GtkWidget *setbutton,*antibutton ,*logbutton,*aboutbutton,* mainPageIntroduceImage;
pthread_t deleteVirusResultThreadId;
gint refreshDeleteVirusId;
int deleteVirusOver = 0;
int select_del_num = 0;
int test_num = 0;
int select_addwhitelist_num = 0;
int cur_progress_type = 1;
#define DELETE_VIRUS_TYPE  1
#define ADD_WHITE_LIST_TYPE 2

void deleteVirusThread(int total_delete_num)
{
        GtkListStore *store;
        GtkTreeModel *model;
        GtkTreeIter iter;
        gboolean tog;
        char *value,buffer[1024],*remove_result;
        char getResult[50];
        delete_count = 0;
        delete_count_copy = 0;
	gboolean use_select_all;
//	if (gtk_widget_get_visible (real_protect_result_list))
	printf("当前area的值是%d\n",area);
	if(area==10)
        {
                printf("实时扫描界面删除\n");
                store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(real_protect_result_list)));
                model = gtk_tree_view_get_model(GTK_TREE_VIEW(real_protect_result_list));
		use_select_all = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tog_all_check_button));
        }

//	if (gtk_widget_get_visible (list))
	if(area==7)
	{
		
                printf("主界面扫描界面删除\n");
        	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
        	model = gtk_tree_view_get_model(GTK_TREE_VIEW(list));
		use_select_all = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tog_all_check_box));
	}

        if (gtk_tree_model_get_iter_first(model, &iter) == FALSE)
                return;
                gtk_tree_model_get_iter_from_string(model,&iter,"0");
                do
                {
                        gtk_tree_model_get(model,&iter,0,&tog,-1);
                        if(tog)
                        {
                                gtk_tree_model_get(model,&iter,2,&value,-1);
                                delete_count = delete_data_from_file(value);
                                delete_count_copy=delete_count_copy+delete_count;
                                struct DeleteVirus deleteVirusInfo;
                                deleteVirusInfo.total_select_num = total_delete_num;
                                deleteVirusInfo.cur_delete_num = delete_count_copy;

                                write_share_memory_delete_info("deleteVirusNumMutexKey" , "deleteVirusNumInfoKey", &deleteVirusInfo);
				gtk_tree_model_get(model,&iter,2,&value,-1);
                        	strncpy(buffer,value,1024);
				remove(buffer);			
//                        remove(remove_result);
                        }
                        //gtk_tree_model_get(model,&iter,2,&value,-1);
                        //strncpy(buffer,value,1024);
                        //remove_result=strtok(buffer,"=");
                        //remove(remove_result);
                }
                while (gtk_tree_model_iter_next(model,&iter));
                deleteVirusOver = 1;


}

void addWhiteListThread(int total_delete_num)
{
        char conf_white_list[512];
        GtkListStore *store;
        GtkTreeModel *model;
        GtkTreeIter iter;
        gboolean tog;
        int i=0;
        char *file_name;
        delete_count = 0;
        delete_count_copy = 0;
	gboolean use_select_all;

	if(area==10)
        {
                printf("实时扫描界面删除\n");
                store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(real_protect_result_list)));
                model = gtk_tree_view_get_model(GTK_TREE_VIEW(real_protect_result_list));
                use_select_all = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tog_all_check_button));
        }

	if(area==7)
        {

                printf("主页面扫描界面删除\n");
                store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
                model = gtk_tree_view_get_model(GTK_TREE_VIEW(list));
                use_select_all = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tog_all_check_box));
        }



        if (gtk_tree_model_get_iter_first(model, &iter) == FALSE)
                return;
        gtk_tree_model_get_iter_from_string(model,&iter,"0");

       
                do
                {
                        i++;
                        gtk_tree_model_get(model,&iter,0,&tog,-1);
                        gtk_tree_model_get(model,&iter,NAME_COL,&file_name,-1);
                        if(tog)
                        {
                                delete_count = delete_data_from_file(file_name);
                                delete_count_copy=delete_count_copy+delete_count;
                                FILE *pBufSize;
                                char resultBuffer[256] = {0};
                                char cmd[1024];

                                memset(resultBuffer,0,sizeof(resultBuffer));
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
                                        char file_info[1024];
                                        if(strcmp(style,"1")==0)
                                        {
                                                strcpy(file_size,"null");
                                                strcpy(file_time,"null");       
                                        }
                                        else{
                                                get_file_size(file_name,file_size);
                                                file_size[strlen(file_size)-2] ='\0';
                                                get_file_modify_time(file_name,file_time);
                                        }
                                        strcat_white_list(file_info,style,file_name,file_size,file_time);
                                        snprintf(conf_white_list,sizeof(conf_white_list),"%swhite_list.txt",ANTIVIRUS_CONF_DIR);
                                        write_white_list(conf_white_list,file_info);
                                }
                                struct DeleteVirus deleteVirusInfo;
                                deleteVirusInfo.total_select_num = total_delete_num;
                                deleteVirusInfo.cur_delete_num = delete_count_copy;

                                write_share_memory_delete_info("deleteVirusNumMutexKey" , "deleteVirusNumInfoKey", &deleteVirusInfo);

                        }
                }
                while (gtk_tree_model_iter_next(model,&iter));
                deleteVirusOver = 1; 

}

void delete_virus_result_thread(int total_delete_num,int progress_type)
{
    int ret = 0;
    if(progress_type == DELETE_VIRUS_TYPE)
    {
        ret = pthread_create(&deleteVirusResultThreadId,NULL,(void *)deleteVirusThread,total_delete_num );
    }
    else if( progress_type == ADD_WHITE_LIST_TYPE)
    {
        ret = pthread_create(&deleteVirusResultThreadId,NULL,(void *)addWhiteListThread,total_delete_num );
    }
}

gboolean refresh_delete_virus(int total_delete_num )
{
    float value = 0.0 ;
    int readRet = 0;
    struct DeleteVirus deleteVirusInfo;
     GString *text;
    if(deleteVirusOver == 1)
    {
        value = 1 ;
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(del_progress_bar), value);
        select_del_num = 0;
        deleteVirusOver = 0;
        text = g_string_new(gtk_progress_bar_get_text(GTK_PROGRESS_BAR(del_progress_bar)));
        g_string_sprintf(text, "%d %%", (int) (value * 100));
        gtk_label_set_text(delete_progress_value_label,text->str);
        change_font_style((GtkLabel *)delete_progress_value_label,"<span foreground=\"#3dbd8f\" font_desc='12'>%s</span>");
	gtk_widget_destroy(progress_win);
        if(cur_progress_type == ADD_WHITE_LIST_TYPE)
        {
            gtk_widget_destroy(box);
            white_list_settings(while_list_content_box);

        }
        if(table7!=NULL)
               gtk_widget_destroy(table7);
	if(area==10)
	{
		printf("隐藏实时杀毒的结果界面\n");
		gtk_widget_hide(real_protect_result_table);
	}
        if(delete_count_copy>=result_count)
        {
                result(0);
        }
        else
        {
                 result(1);
        }

        return 0;
    }
    
    readRet = read_share_memory_delete_info("deleteVirusNumMutexKey" , "deleteVirusNumInfoKey",&deleteVirusInfo);
    if(readRet != 1)
    {
         return 1;
    }

    value =  deleteVirusInfo.cur_delete_num/deleteVirusInfo.total_select_num; 
    if(value > 0.0  && value < 1)
    {

        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(del_progress_bar), value);
        text = g_string_new(gtk_progress_bar_get_text(GTK_PROGRESS_BAR(del_progress_bar)));
        g_string_sprintf(text, "%d %%", (int) (value * 100));
        gtk_label_set_text(delete_progress_value_label,text->str);
        change_font_style((GtkLabel *)delete_progress_value_label,"<span foreground=\"#3dbd8f\" font_desc='12'>%s</span>");
    }
    return 1;
}
void refreshDeleteVirusProcess(int total_delete_num )
{
    refreshDeleteVirusId = g_timeout_add (1*1000, refresh_delete_virus, total_delete_num);
    sleep(1);
    return;


}
void refresh_delete_virus_process_thread(int total_delete_num)
{
    g_thread_new("refresh_delete_virus_process",refreshDeleteVirusProcess,total_delete_num);
}

int delete_data_from_file(char *new_value)
{
        char cos[200];
        int i=0;
        snprintf(cos,sizeof(cos),"grep -n -R  '%s' %sscan_results.txt | cut -d ':' -f1 >%stest",new_value,ANTIVIRUS_TEMP_DIR,ANTIVIRUS_TEMP_DIR) ;

        system(cos);
	char temp_test[256];
        snprintf(temp_test,sizeof(temp_test),"%stest",ANTIVIRUS_TEMP_DIR) ;
        FILE *fp=fopen(temp_test,"rw");

        char buf[4096];
        while(fgets(buf,4095,fp) != NULL)
        {
             int num,del_line_num;
             num=atoi(buf);
             del_line_num=num-i;
             snprintf(cos,sizeof(cos),"sed -i '%dd' %sscan_results.txt 2>&1",del_line_num,ANTIVIRUS_TEMP_DIR);
             system(cos);
             i++;
       }
	fclose(fp);
        return i;
}
void destroy_progresswin(GtkWidget *widget,gpointer *data)
{

        if(cur_progress_type == ADD_WHITE_LIST_TYPE )
        {
              gtk_widget_destroy(box);
              white_list_settings(while_list_content_box);
        }
      	g_source_remove(refreshDeleteVirusId);
      	pthread_cancel(deleteVirusResultThreadId);
      	waitScanThreadCacelFinish(deleteVirusResultThreadId);
      	select_del_num = 0;
        select_addwhitelist_num = 0;
      	deleteVirusOver = 0;
      	delete_count_copy=0;
      	struct DeleteVirus deleteVirusInfo;
      	deleteVirusInfo.total_select_num = 0;
      	deleteVirusInfo.cur_delete_num = 0;
      	write_share_memory_delete_info("deleteVirusNumMutexKey" , "deleteVirusNumInfoKey", &deleteVirusInfo);

      	gtk_widget_destroy(progress_win);
      	if(table7!=NULL)
        	gtk_widget_destroy(table7);
	if(area==10)
        {
                printf("隐藏实时杀毒的结果界面\n");
                gtk_widget_hide(real_protect_result_table);
        }

      	if(delete_count_copy>=result_count)
        {
        	result(0);
        }
        else
        {
                result(1);
        }
}

void delete_progress(GtkTreeModel *model,GtkTreeIter iter,int total_delete_num ,int progress_type )
{
        GtkWidget *progress_fix,*cancel_button;
        float value = 0.0;
        GString *text;
        char image_shadu[512];
        char css_tipdialog[512];
        GtkWidget *delete_progress_layout,*delete_progress_bg_image,*quit_deleteprogress_button,*quit_deleteprogress_image,*cancel_button_image;
        progress_win=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(progress_win,_("delete thread"));
        gtk_window_set_transient_for(progress_win,window);
        gtk_window_set_modal(progress_win,TRUE);
        gtk_window_set_decorated(GTK_WINDOW(progress_win), FALSE);
        gtk_window_set_position(GTK_WINDOW( progress_win),GTK_WIN_POS_MOUSE);
        gtk_window_set_default_size(GTK_WINDOW(progress_win),376,247);
        snprintf(image_shadu,sizeof(image_shadu),_("%sNFSantivirus.png"),ANTIVIRUS_IMAGE_DIR);
        gtk_window_set_icon_from_file(GTK_WINDOW(progress_win),image_shadu,NULL);
        gtk_widget_set_events(GTK_WINDOW(progress_win),
                   GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK
                   | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);

        g_signal_connect(GTK_WINDOW(progress_win), "button_press_event", G_CALLBACK (button_press_event), progress_win);       // 加入事件回调
//        g_signal_connect(GTK_WINDOW(progress_win), "motion_notify_event",G_CALLBACK( motion_notify_event), progress_win);
        g_signal_connect(GTK_WINDOW(progress_win), "button_release_event", G_CALLBACK(button_release_event), progress_win);

        delete_progress_layout=gtk_layout_new(NULL, NULL);
        gtk_widget_show(delete_progress_layout);
        gtk_widget_set_size_request(delete_progress_layout,376,247);

        if(progress_type == DELETE_VIRUS_TYPE)
        {       memset(image_shadu,0,sizeof(image_shadu));
                snprintf(image_shadu,sizeof(image_shadu),_("%sdelete_thread.png"),ANTIVIRUS_IMAGE_DIR);
                delete_progress_bg_image=gtk_image_new_from_file(image_shadu);
        }
        else
        {
                memset(image_shadu,0,sizeof(image_shadu));
                snprintf(image_shadu,sizeof(image_shadu),_("%saddwhitelist_progress.png"),ANTIVIRUS_IMAGE_DIR);
                delete_progress_bg_image=gtk_image_new_from_file(image_shadu);
        }

        gtk_widget_show( delete_progress_bg_image);
        gtk_layout_put(GTK_LAYOUT(delete_progress_layout), delete_progress_bg_image, 0, 0);
        snprintf(css_tipdialog,sizeof(css_tipdialog),"%scss_tipdialog",ANTIVIRUS_CSS_DIR);
	GFile *file=g_file_new_for_path(css_tipdialog);
        GdkScreen *screen;
        screen = gdk_screen_get_default();
        GtkCssProvider *cssProvider;
        cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_file(cssProvider, file, NULL);

        quit_deleteprogress_button=gtk_button_new();
        gtk_button_set_relief((GtkButton *)quit_deleteprogress_button,GTK_RELIEF_NONE);
        gtk_widget_set_size_request(quit_deleteprogress_button,24,24);
        gtk_widget_show(quit_deleteprogress_button);
        g_signal_connect(G_OBJECT(quit_deleteprogress_button),"clicked",G_CALLBACK(destroy_progresswin),progress_win);
        memset(image_shadu,0,sizeof(image_shadu));
                snprintf(image_shadu,sizeof(image_shadu),_("%sclose.png"),ANTIVIRUS_IMAGE_DIR);
        quit_deleteprogress_image= gtk_image_new_from_file(image_shadu);
        gtk_widget_show(quit_deleteprogress_image);
        gtk_button_set_image(quit_deleteprogress_button,quit_deleteprogress_image);
        gtk_layout_put(GTK_LAYOUT(delete_progress_layout),quit_deleteprogress_button,349,0);
	gtk_widget_set_name(quit_deleteprogress_button,"tipdialog-close");
        gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);

        del_progress_bar=gtk_progress_bar_new();
        gtk_widget_show(del_progress_bar);
        gtk_widget_set_size_request(del_progress_bar, 270, 20);
        gtk_layout_put(GTK_LAYOUT(delete_progress_layout),del_progress_bar,23,111);

        cancel_button=gtk_button_new();
        gtk_button_set_relief((GtkButton *) cancel_button,GTK_RELIEF_NONE);
        gtk_widget_set_size_request(cancel_button,82,34);
        memset(image_shadu,0,sizeof(image_shadu));
                snprintf(image_shadu,sizeof(image_shadu),_("%squitupgrade_NO.png"),ANTIVIRUS_IMAGE_DIR);
        cancel_button_image=gtk_image_new_from_file(image_shadu);

        delete_progress_value_label = (GtkLabel *)gtk_label_new("0%");
        gtk_widget_show(delete_progress_value_label);
        change_font_style((GtkLabel *)delete_progress_value_label,"<span foreground=\"#3dbd8f\" font_desc='12'>%s</span>");
        gtk_layout_put(GTK_LAYOUT(delete_progress_layout),delete_progress_value_label, 310, 111);

        gtk_button_set_image(cancel_button,cancel_button_image);
        gtk_layout_put(GTK_LAYOUT(delete_progress_layout),cancel_button, 270, 190);
        gtk_widget_show(cancel_button);
        gtk_widget_show(cancel_button_image);

        gtk_container_add(GTK_CONTAINER(progress_win),delete_progress_layout);
        gtk_widget_show(progress_win);

        g_signal_connect(G_OBJECT(cancel_button),"clicked",G_CALLBACK(destroy_progresswin),progress_type);

        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(del_progress_bar), value);
        text = g_string_new(gtk_progress_bar_get_text(GTK_PROGRESS_BAR(del_progress_bar)));
        g_string_sprintf(text, "%d %%", (int) (value * 100));
        gtk_label_set_text(delete_progress_value_label,text->str);
        change_font_style((GtkLabel *)delete_progress_value_label,"<span foreground=\"#3dbd8f\" font_desc='12'>%s</span>");


         //创建刷新删除病毒进度条界面线程
        refresh_delete_virus_process_thread(total_delete_num);

        //创建删除病毒线程
        delete_virus_result_thread(total_delete_num,progress_type);


}


void switch_to_devide_list(GtkWidget *widget,gpointer *data)
{
	gtk_widget_destroy(table7);
	area=2;
	char image_dir[256];
        gtk_button_set_relief((GtkButton *)antibutton,GTK_RELIEF_NONE);
        snprintf(image_dir,sizeof(image_dir),_("%santi.png"),ANTIVIRUS_IMAGE_DIR);
                GtkWidget *leave_antibutton_image = gtk_image_new_from_file(image_dir);

                gtk_button_set_image(antibutton,leave_antibutton_image);
                gtk_button_set_relief((GtkButton *)logbutton,GTK_RELIEF_NONE);
                memset(image_dir,0,sizeof(image_dir));
                snprintf(image_dir,sizeof(image_dir),_("%slogging.png"),ANTIVIRUS_IMAGE_DIR);

                GtkWidget *leave_logbutton_image = gtk_image_new_from_file(image_dir);

                gtk_button_set_image(logbutton,leave_logbutton_image);

                gtk_button_set_relief((GtkButton *)aboutbutton,GTK_RELIEF_NONE);
                memset(image_dir,0,sizeof(image_dir));
                snprintf(image_dir,sizeof(image_dir),_("%sabout.png"),ANTIVIRUS_IMAGE_DIR);

                GtkWidget *leave_aboutbutton_image = gtk_image_new_from_file(image_dir);

                gtk_button_set_image(aboutbutton,leave_aboutbutton_image);
	gtk_button_set_relief((GtkButton *)setbutton,GTK_RELIEF_NONE);
	
        memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sset_enter.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *enter_set_image = gtk_image_new_from_file(image_dir);
        gtk_button_set_image(setbutton,enter_set_image);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%ssetting_introduce_title.png"),ANTIVIRUS_IMAGE_DIR);
        gtk_image_set_from_file (mainPageIntroduceImage,image_dir);
	gtk_widget_show(table2);
	gtk_widget_show(layout_main);
        gtk_widget_show(versionimage);
        gtk_widget_show(title_layout);
	if(devide_file_box!=NULL)
                gtk_widget_destroy(devide_file_box);
        look_devide_file_settings(devide_content_box);
	gtk_widget_show(layout_main);
        gtk_widget_show(versionimage);
        gtk_widget_show(title_layout);
	gtk_notebook_set_current_page((GtkNotebook *)notebook,4);
}

void remove_item(GtkWidget *widget,gpointer *data){
	not_tog_warning(list,0,TRUE,1);
}

void remove_item_real(){
       GtkListStore *store;
        GtkTreeModel *model;
        GtkTreeIter iter;
        gboolean tog;
        char *value,buffer[1024],*remove_result;
        char getResult[50];
        select_del_num = 0;
        gint rec_remove_flag;
//	if (gtk_widget_get_visible (list))
	if(area==7)
	{
		printf("主界面删除\n");
		store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
	        model = gtk_tree_view_get_model(GTK_TREE_VIEW(list));	
	}
//	if (gtk_widget_get_visible (real_protect_result_list))
	if(area==10)
	{
		printf("实时扫描界面删除\n");
        	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(real_protect_result_list)));
        	model = gtk_tree_view_get_model(GTK_TREE_VIEW(real_protect_result_list));
	}
        if (gtk_tree_model_get_iter_first(model, &iter) == FALSE)
            return;
        do
        {
            gtk_tree_model_get(model,&iter,0,&tog,-1);
            if(tog)
            select_del_num++;
        }
        while (gtk_tree_model_iter_next(model,&iter));
        cur_progress_type = DELETE_VIRUS_TYPE;
        delete_progress(model,iter,select_del_num,DELETE_VIRUS_TYPE);

}

void add_white_list_real()
{
	GtkListStore *store;
        GtkTreeModel *model;
        GtkTreeIter iter;
        gboolean tog;
        int i=0;
        char *file_name;
        int delete_count = 0;
        select_addwhitelist_num = 0;
        gint rec_add_white_flag;

	printf("当前界面area is %d \n",area);


//	if (gtk_widget_get_visible (list))
	if(area==7)
        {
                printf("主界面添加\n");
                store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
                model = gtk_tree_view_get_model(GTK_TREE_VIEW(list));
        }
//        if (gtk_widget_get_visible (real_protect_result_list))
	if(area==10)
        {
                printf("实时扫描界面添加\n");
                store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(real_protect_result_list)));
                model = gtk_tree_view_get_model(GTK_TREE_VIEW(real_protect_result_list));
        }



	if (gtk_tree_model_get_iter_first(model, &iter) == FALSE)
            return;
        do
        {
            gtk_tree_model_get(model,&iter,0,&tog,-1);
            if(tog)
                select_addwhitelist_num++;
        }
        while (gtk_tree_model_iter_next(model,&iter));
        cur_progress_type = ADD_WHITE_LIST_TYPE;
        delete_progress(model,iter,select_addwhitelist_num,ADD_WHITE_LIST_TYPE);

}

void add_white_list(GtkWidget *widget,gpointer *data){
	not_tog_warning(list,0,TRUE,2);
}
