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
#include "../function.h"
#include "../antivirus.h"
int window_is_hide=0;
extern GtkWidget *start_defence_image;
extern int AutoStartFirstShowFlag;
extern pthread_t avlInitThreadId, waitInitThreadId;
extern GtkLabel * attentionlabel;
//显示托盘函数
void trayView(GtkMenuItem *item, gpointer data)
{
    gtk_window_deiconify((GtkWindow *)(data));
    gtk_widget_show((GtkWidget *)(data));
}

//关闭窗口函数
void destroy(GtkWidget *widget,gpointer data)
{
        system("rm -r /tmp/.user");
        quit_init_engine();
	gtk_main_quit();
}

//点击图标函数
void trayIconActivated(GObject *trayIcon, gpointer window)
{
	void *status;
	if(AutoStartFirstShowFlag == 1)
	{
        pthread_create(&avlInitThreadId,NULL,(void *)avlInitThread,NULL);
	}
 	if(!gtk_widget_get_visible(window))
    	{
    		gtk_window_deiconify((GtkWindow *)(window));
		if (gtk_widget_get_visible (start_defence_image))
		{
			gtk_widget_hide(start_defence_image);
		}
    		gtk_widget_show((GtkWidget *)(window));
    		window_is_hide=1;
    	}
    	else
    	{
    		gtk_window_iconify((GtkWindow *)(window));
    		gtk_widget_hide((GtkWidget *)(window));
    		window_is_hide=0;
    	}

		if(AutoStartFirstShowFlag == 1)
	    {
			pthread_create(&waitInitThreadId,NULL,(void *)waitInitThread,attentionlabel);

		}


}

//托盘图标弹出按钮函数
void trayIconPopup(GtkStatusIcon *status_icon, guint button, guint32 activate_time, gpointer popUpMenu)
{
    	gtk_menu_popup(GTK_MENU(popUpMenu), NULL, NULL, gtk_status_icon_position_menu, status_icon, button, activate_time);
}

//根据点击状态，是否显示主界面到托盘
gboolean window_state_event (GtkWidget *widget, GdkEventWindowState *event, gpointer trayIcon)
{
    	if(event->changed_mask == GDK_WINDOW_STATE_ICONIFIED  && ( event->new_window_state ==( GDK_WINDOW_STATE_FOCUSED | GDK_WINDOW_STATE_ICONIFIED)))
    	{
        	gtk_widget_hide (GTK_WIDGET(widget));
        	gtk_status_icon_set_visible(GTK_STATUS_ICON(trayIcon), TRUE);
    		window_is_hide=0;

    	}
    	else if(event->changed_mask == GDK_WINDOW_STATE_WITHDRAWN  && (event->new_window_state == GDK_WINDOW_STATE_ICONIFIED || event->new_window_state == (GDK_WINDOW_STATE_ICONIFIED | GDK_WINDOW_STATE_MAXIMIZED)))
    	{
        	gtk_status_icon_set_visible(GTK_STATUS_ICON(trayIcon), FALSE);
    	}
    	return TRUE;
}

