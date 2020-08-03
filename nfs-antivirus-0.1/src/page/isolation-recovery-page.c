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
#include "./setting-page.h"
#include "../deal/isolation_handle.h"
#include "../page/home-page.h"
#include "isolation-recovery-page.h"
#include "../function.h"
#define _(STRING) gettext(STRING)

extern GtkWidget *thingbox;
extern int select_all_item_num;
extern char devide_file[256];
void set_editable();
void dir_exist();
GtkWidget *radio_button_first,*radio_button_second;
GtkWidget *path_entry;
GtkWidget *browse_button,*rescovery_button;
    GtkWidget *recovery_dialog;

void destroy_recovery_dialog()
{
	gtk_widget_destroy(recovery_dialog);
}
int pathchooser(GtkWidget *widget,gpointer data)
{
	GtkWidget *choose_path;
   gchar *recovery_path;
   choose_path =  gtk_file_chooser_dialog_new (_("Path selection"),GTK_WINDOW(data),
                                                GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                                GTK_STOCK_CANCEL,
                                                GTK_RESPONSE_CANCEL,
                                                GTK_STOCK_OK,
                                                GTK_RESPONSE_OK,
                                                NULL);
    gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(choose_path),TRUE);
   if(gtk_dialog_run (GTK_DIALOG (choose_path)) == GTK_RESPONSE_OK)
   {
        gtk_widget_hide(choose_path);
        recovery_path=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(choose_path));
        gtk_entry_set_text(GTK_ENTRY(path_entry),recovery_path);
   }
   gtk_widget_destroy(choose_path);
   return 0;
}

gboolean compute_str_len(char *str,GtkWidget *widget)
{	
	 char str_first[70];
        GtkWidget *question_label_end;
        printf("%s,%d\n",str,strlen(str));
        if(strlen(str)>55)
        {
                if(str[54]&0x80){
                                int num_hanzi=0;
                                int i=0;
                                while(i<54){
                                if(str[i]&0x80){
                                        num_hanzi++;
                                }
                                i++;
                                }
                                printf("汉字高位的个数是%d\n",num_hanzi);
                                int first_len;
                                int end_len;
                                if(num_hanzi%3==1){
                                        snprintf(str_first,57,"%s\n",str,56);
                                        FILE *stream;
                                        char   buf[256];
                                        char cmd[256];
                                        sprintf(cmd,"echo %s|cut -c57-%d",str,strlen(str));
                                        stream = popen(cmd,"r");
                                        if(fgets(buf,255,stream)!=NULL)
                                        {
                                        }
                                        strcpy(str,str_first);
                                        question_label_end = gtk_label_new("");
                                        gtk_label_set_text(question_label_end,buf);
                                }
                                if(num_hanzi%3==2){
                                        snprintf(str_first,56,"%s\n",str,55);
                                        FILE *stream;
                                        char   buf[256];
                                        char cmd[256];
                                        sprintf(cmd,"echo %s|cut -c56-%d",str,strlen(str));
                                        stream = popen(cmd,"r");
                                        if(fgets(buf,255,stream)!=NULL)
                                        {
                                        }
                                        strcpy(str,str_first);
                                        question_label_end = gtk_label_new("");
                                        gtk_label_set_text(question_label_end,buf);
                                }
                                else{
                                        snprintf(str_first,55,"%s\n",str,54);
//              if([54]&0x80){
                                        FILE *stream;
                                        char   buf[256];
                                        char cmd[256];
                                        sprintf(cmd,"echo %s|cut -c55-%d",str,strlen(str));
                                        stream = popen(cmd,"r");
                                        if(fgets(buf,255,stream)!=NULL)
                                        {
                                        }
                                        strcpy(str,str_first);
                                        question_label_end = gtk_label_new("");
                                        gtk_label_set_text(question_label_end,buf);
                                }
                }
                else{
                snprintf(str_first,55,"%s\n",str,54);
//              if([54]&0x80){
                FILE *stream;
                char   buf[256];
                char cmd[256];
                sprintf(cmd,"echo %s|cut -c55-%d",str,strlen(str));
                stream = popen(cmd,"r");
                if(fgets(buf,255,stream)!=NULL)
                {
                }
                strcpy(str,str_first);
                question_label_end = gtk_label_new("");
                gtk_label_set_text(question_label_end,buf);
                }
                gtk_layout_put(GTK_LAYOUT(widget),question_label_end,120,130);
                gtk_widget_show(question_label_end);

                return TRUE;
        }
        else
                return FALSE;

}
void leave_recovery_close_button(GtkWidget *widget,gpointer *data){
	char image_dir[256];
        snprintf(image_dir,sizeof(image_dir),_("%sclose.png"),ANTIVIRUS_IMAGE_DIR);

	gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
        GtkWidget *enter_setbutton_image = gtk_image_new_from_file(image_dir);

        gtk_button_set_image(widget,enter_setbutton_image);
}
void enter_recovery_close_button(GtkWidget *widget,gpointer *data){
	gtk_button_set_image(widget,NULL);
        g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_recovery_close_button),NULL);
}
void recovery(GtkWidget *widget,gpointer *data)
{
    GtkWidget *recovery_layout,*recovery_bg_image,*recovery_title,*close_dialog_button,*close_recovery_dialog_image,*browse_button_image_false;
    GtkWidget *title_image;
    GtkWidget *question_label;
    GtkWidget *tip_label;
    GtkWidget *cancel_button;
    GtkWidget *group;
    int select_item;
    char image_dir[256];
    select_item = get_select_items_num(-1);
    int hide=0;
    if(select_item ==1)
    	hide=0;
    if(select_item >1)
        hide=1;

    recovery_dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	char image_dir_recovery[256];
	snprintf(image_dir_recovery,sizeof(image_dir_recovery),_("%sNFSantivirus.png"),ANTIVIRUS_IMAGE_DIR);

        gtk_window_set_icon_from_file(GTK_WINDOW(recovery_dialog),image_dir_recovery,NULL);
gtk_window_set_modal(recovery_dialog,FALSE);
    gtk_window_set_title(recovery_dialog,_("recovery devide"));
    gtk_window_set_decorated(GTK_WINDOW(recovery_dialog), FALSE);
   gtk_window_set_position(GTK_WINDOW(recovery_dialog),GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(recovery_dialog),450,476);
   gtk_widget_set_events(GTK_WINDOW(recovery_dialog),
                   GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK
                   | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);

    g_signal_connect(GTK_WINDOW(recovery_dialog), "button_press_event", G_CALLBACK (button_press_event), recovery_dialog);       // 加入事件回调
//    g_signal_connect(GTK_WINDOW(recovery_dialog), "motion_notify_event",G_CALLBACK( motion_notify_event), recovery_dialog);
    g_signal_connect(GTK_WINDOW(recovery_dialog), "button_release_event", G_CALLBACK(button_release_event), recovery_dialog);

    recovery_layout = gtk_layout_new(NULL, NULL);
    gtk_widget_show(recovery_layout);
    gtk_widget_set_size_request(recovery_layout, 450, 476);
    
    snprintf(image_dir,sizeof(image_dir),_("%srecovery_bg_image.png"),ANTIVIRUS_IMAGE_DIR);
    recovery_bg_image = gtk_image_new_from_file(image_dir);
    gtk_widget_show(recovery_bg_image);
    gtk_layout_put(GTK_LAYOUT(recovery_layout),recovery_bg_image, 0, 0);
    
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%srecovery_title.png"),ANTIVIRUS_IMAGE_DIR);

    recovery_title = gtk_image_new_from_file(image_dir);
    gtk_widget_show(recovery_title);
    gtk_layout_put(GTK_LAYOUT(recovery_layout),recovery_title, 184, 13);
    char tipdialog_css[256];
    snprintf(tipdialog_css,sizeof(tipdialog_css),"%stipdialog.css",ANTIVIRUS_CSS_DIR);
    GFile *file=g_file_new_for_path(tipdialog_css);
        GdkScreen *screen;
        screen = gdk_screen_get_default();
        GtkCssProvider *cssProvider;
        cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_file(cssProvider, file, NULL);

    close_dialog_button = gtk_button_new();
    gtk_button_set_relief(close_dialog_button,GTK_RELIEF_NONE);
    gtk_widget_show(close_dialog_button);
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%sclose.png"),ANTIVIRUS_IMAGE_DIR);
    close_recovery_dialog_image = gtk_image_new_from_file(image_dir);
    gtk_widget_show(close_recovery_dialog_image);
    gtk_button_set_image(close_dialog_button,close_recovery_dialog_image);
    gtk_layout_put(GTK_LAYOUT(recovery_layout),close_dialog_button, 430, 0);
    gtk_widget_set_size_request(close_dialog_button,24,24);
    gtk_widget_set_name(close_dialog_button,"tipdialog-close");
        gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);
    g_signal_connect(G_OBJECT(close_dialog_button),"enter",G_CALLBACK(enter_recovery_close_button),NULL);
    g_signal_connect(G_OBJECT(close_dialog_button),"clicked",G_CALLBACK(destroy_recovery_dialog),recovery_dialog);

    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%srestore_warn.png"),ANTIVIRUS_IMAGE_DIR);
    title_image = gtk_image_new_from_file(image_dir);
    gtk_layout_put(GTK_LAYOUT(recovery_layout),title_image,38,110);
    gtk_widget_show(title_image);

    char question_title[256];
    question_label = gtk_label_new("");
    printf("devide file is %s\n",devide_file);
    if(hide==0)
    	sprintf(question_title,_("Do you want to rescovery item %s ?"),devide_file);
    if(hide==1)
	strcpy(question_title,_("Do you want to rescovery select items?\n"));
    gboolean long_flag = compute_str_len(question_title,recovery_layout);
    gtk_label_set_text(question_label,question_title);
    gtk_layout_put(GTK_LAYOUT(recovery_layout),question_label,120,110);
    gtk_widget_show(question_label);

    tip_label = gtk_label_new("");
    gtk_label_set_text(tip_label,_("After rescovery may be bring threat for you system"));
    if(long_flag)
    	gtk_layout_put(GTK_LAYOUT(recovery_layout),tip_label,120,150);
    else
        gtk_layout_put(GTK_LAYOUT(recovery_layout),tip_label,120,130);
    gtk_widget_show(tip_label);
    
    radio_button_first = gtk_radio_button_new(NULL);
    group = (GtkWidget *)gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio_button_first));
    GtkWidget *radio_button_first_label;
    radio_button_first_label = gtk_label_new(_("rescovery the file to original directory"));
    gtk_widget_show(radio_button_first_label);
    gtk_container_add(GTK_CONTAINER(radio_button_first),radio_button_first_label);
    radio_button_second = gtk_radio_button_new((GSList *)group);
    GtkWidget *radio_button_second_label;
    radio_button_second_label = gtk_label_new(_("rescovery the file the following specified directory"));
    gtk_widget_show(radio_button_second_label);
    gtk_container_add(GTK_CONTAINER(radio_button_second),radio_button_second_label);
    g_signal_connect(G_OBJECT(radio_button_second),"toggled",G_CALLBACK(set_editable),NULL);
    gtk_layout_put(GTK_LAYOUT(recovery_layout),radio_button_first,125,195);
    gtk_layout_put(GTK_LAYOUT(recovery_layout),radio_button_second,125,220);
    gtk_widget_show(radio_button_first);
    gtk_widget_show(radio_button_second);

    path_entry = (GtkComboBoxText *)gtk_entry_new();
    gtk_widget_set_sensitive(path_entry,FALSE);
    gtk_widget_set_size_request((GtkWidget *)path_entry, 190, 25);
    gtk_layout_put(GTK_LAYOUT(recovery_layout),path_entry,148,260);
    gtk_widget_show(path_entry);
    g_signal_connect(G_OBJECT(path_entry),"activate",G_CALLBACK(dir_exist),NULL);

    browse_button = gtk_button_new();
    gtk_button_set_relief(browse_button,GTK_RELIEF_NONE);
    gtk_widget_set_sensitive(browse_button,FALSE);
    gtk_layout_put(GTK_LAYOUT(recovery_layout),browse_button,340,255);
    gtk_widget_show(browse_button);
    
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%sbrowse_button_image_false.png"),ANTIVIRUS_IMAGE_DIR);
    browse_button_image_false =  gtk_image_new_from_file(image_dir);
    gtk_widget_show(browse_button_image_false);
    gtk_button_set_image(browse_button,browse_button_image_false);
    g_signal_connect(G_OBJECT(browse_button),"clicked",G_CALLBACK(pathchooser),recovery_dialog);


    trust_checkbutton = gtk_check_button_new();
    GtkWidget *trust_checkbutton_label = gtk_label_new(_("Trust this file after reccovery,not kill again"));
    gtk_widget_show(trust_checkbutton_label);
    gtk_container_add(GTK_CONTAINER(trust_checkbutton),trust_checkbutton_label);
    gtk_layout_put(GTK_LAYOUT(recovery_layout),trust_checkbutton,32,415);
    gtk_widget_show(trust_checkbutton);

    rescovery_button = gtk_button_new();
    gtk_button_set_relief((GtkButton *)rescovery_button,GTK_RELIEF_NONE);
    
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%srestore-ios.png"),ANTIVIRUS_IMAGE_DIR);

    GtkWidget *rescovery_button_image = gtk_image_new_from_file(image_dir); 
    gtk_button_set_image(rescovery_button,rescovery_button_image);
    gtk_layout_put(GTK_LAYOUT(recovery_layout),rescovery_button,230,410);
    gtk_widget_show(rescovery_button);
    g_signal_connect(G_OBJECT(rescovery_button),"clicked",G_CALLBACK(recovery_select_items),hide);

    cancel_button = gtk_button_new();
    gtk_button_set_relief((GtkButton *)cancel_button,GTK_RELIEF_NONE);
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%scancel.png"),ANTIVIRUS_IMAGE_DIR);
    GtkWidget *cancel_button_image = gtk_image_new_from_file(image_dir);
    gtk_button_set_image(cancel_button,cancel_button_image);
    gtk_layout_put(GTK_LAYOUT(recovery_layout),cancel_button,325,410);
    gtk_widget_show(cancel_button);
    g_signal_connect(G_OBJECT(cancel_button),"clicked",G_CALLBACK(destroy_recovery_dialog),NULL);
    gtk_container_add(GTK_CONTAINER(recovery_dialog),recovery_layout);
    gtk_widget_show(recovery_dialog);
    if(hide==1)
    {
    	gtk_widget_hide(radio_button_first);
	gtk_widget_hide(radio_button_second);
	gtk_widget_hide(path_entry);
	gtk_widget_hide(browse_button);
    }
}
void set_editable()
{
	char image_dir[256];
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_button_second)))
	{
		//gtk_widget_set_sensitive(path_entry,TRUE);
		gtk_widget_set_sensitive(browse_button,TRUE);
		GtkWidget *browse_button_true;
		
    		snprintf(image_dir,sizeof(image_dir),_("%sbrowse_button_image_true.png"),ANTIVIRUS_IMAGE_DIR);
		browse_button_true = gtk_image_new_from_file(image_dir);
		gtk_button_set_image(browse_button,browse_button_true);
	}          
	else
	{
		gtk_widget_set_sensitive(path_entry,FALSE);
        	gtk_widget_set_sensitive(browse_button,FALSE);
		GtkWidget *browse_button_false;
		memset(image_dir,0,sizeof(image_dir));
    		snprintf(image_dir,sizeof(image_dir),_("%sbrowse_button_image_false.png"),ANTIVIRUS_IMAGE_DIR);
                browse_button_false = gtk_image_new_from_file(image_dir);
                gtk_button_set_image(browse_button,browse_button_false);
	}    	
}
void dir_exist()
{
	if((opendir(gtk_entry_get_text(path_entry)))!=NULL)
	{
	}
	else
	{	
		char cmd[256];
		sprintf(cmd,"mkdir %s",gtk_entry_get_text(path_entry));
		system(cmd);
	}

}
