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
#ifndef TRAY_HANDLE_H
#define TRAY_HANDLE_H
void trayView(GtkMenuItem *item, gpointer data);
void destroy(GtkWidget *widget,gpointer data);
void trayIconActivated(GObject *trayIcon, gpointer window);
void trayIconPopup(GtkStatusIcon *status_icon, guint button, guint32 activate_time, gpointer popUpMenu);
gboolean window_state_event (GtkWidget *widget, GdkEventWindowState *event, gpointer trayIcon) ;
#endif
