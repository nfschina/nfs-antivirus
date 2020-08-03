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
#define _(STRING) gettext(STRING)
#include <gdk/gdk.h>


#define ANTIVIRUS_IMAGE_DIR     "/usr/share/antivirus/images/"
#define ANTIVIRUS_CSS_DIR       "/usr/share/antivirus/css/"
#define ANTIVIRUS_BIN_DIR       "/usr/share/antivirus/bin/"
#define ANTIVIRUS_CONF_DIR      "/usr/share/antivirus/config/"
#define ANTIVIRUS_TEMP_DIR      "/usr/share/antivirus/temp/"
#define ANTIVIRUS_LOG_DIR       "/usr/share/antivirus/log/"
#define ANTIVIRUS_DESKTOP_DIR   "/usr/share/antivirus/desktop/"
#define ANTIVIRUS_DEFAULT_ISOLATION_DIR "/usr/share/antivirus/isolation/"
#define ANTIVIRUS_BIN_WHITE_DIR         "/usr/share/antivirus/bin/white_list/"
#define ANTIVIRUS_BIN_AUTO_HANDLE_DIR   "/usr/share/antivirus/bin/auto_handle/"
#define ANTIVIRUS_BIN_AUTO_DEFENSE_DIR  "/usr/share/antivirus/bin/auto_defense/"
#define ANTIVIRUS_BIN_UPGRADE_DIR       "/usr/share/antivirus/bin/upgrade/"
#define ANTIVIRUS_BIN_TIMEANTIVIRUS_DIR "/usr/share/antivirus/bin/timeantivirus/"
#define ANTIVIRUS_BIN_DETECT_USB_DIR    "/usr/share/antivirus/bin/detect_usb"
#define ANTIVIRUS_REAL_PROTECT_DIR   "/usr/share/antivirus/real_protect/"

int nX = 0;
int nY = 0;
gboolean drag = FALSE; 
void change_font_style(GtkLabel *label,char *cssInfo)
{
        char *markup;
        const gchar *labelTitel = gtk_label_get_text(label);
        markup = g_markup_printf_escaped (cssInfo, labelTitel);
        gtk_label_set_markup (GTK_LABEL (label), markup);
        g_free (markup);
}


void yes_callbackfunc(GtkWidget *widget,gpointer data)
{
//        gtk_widget_destroy(data);
	gtk_main_quit();
}
gint button_press_event(GtkWidget * widget,GdkEventButton * event, gpointer data)
{
       /* if (event->button == 1)       // 判断是否左键按下
        {
                 drag = TRUE;
                 nX = event->x;  // 取得鼠标相对于窗口的位置
                 nY = event->y;
        }
        return TRUE;*/
 if(event->type=GDK_BUTTON_PRESS)
                         {
                  if (event->button == 1)       // 判断是否左键按>下
                 { gtk_window_begin_move_drag(GTK_WINDOW(gtk_widget_get_toplevel(widget)),event->button,event->x_root,event->y_root,event->time);
     return FALSE;
     }
}}


gint button_release_event(GtkWidget * widget, GdkEventButton * event,gpointer data)         // 鼠标抬起事件
{
        if (event->button == 1)
                 drag = FALSE;
        return TRUE;
}
/*gint motion_notify_event(GtkWidget * widget, GdkEventButton * event,gpointer data)         // 鼠标移动事件
{
        if (drag)
        {
                 int x, y;
                 GtkWidget *win = (GtkWidget *) data;
                 gtk_window_get_position((GtkWindow *) win, &x, &y);         // 取窗体绝对坐标
                 gtk_window_move((GtkWindow *) win, x + event->x - nX,
                             y + event->y - nY);// 移动窗体
        }
        return TRUE;
}
*/
void warning_dialog()
{
//	int nX = 0;
//	int nY = 0;
//	gboolean drag = FALSE;	
	char image_shadu[512];
        snprintf(image_shadu,sizeof(image_shadu),_("%sNFSantivirus.png"),ANTIVIRUS_IMAGE_DIR);

	GtkWidget *warning_win;
        GtkWidget *warning_vbox,*warning_fix,*warning_label,*yes_button,*no_button,*warning_image,*yes_button_image,*no_button_image;

        GtkWidget *autodeal_warning_layout,*autodeal_title_image,*autodeal_title_quit_button,*autodeal_quit_image;
        warning_win = (GtkWindow *)gtk_window_new(GTK_WINDOW_POPUP);
	gtk_window_set_modal(warning_win,TRUE);
	gtk_window_set_decorated(GTK_WINDOW(warning_win), FALSE);
	gtk_window_set_position(GTK_WINDOW(warning_win),GTK_WIN_POS_CENTER);
	gtk_widget_set_events(GTK_WINDOW(warning_win),
                   GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK
                   | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);
//        gtk_window_set_default_size(GTK_WINDOW(warning_win),380,248);
//        gtk_window_set_icon_from_file(GTK_WINDOW(warning_win),image_shadu,NULL);

        g_signal_connect(GTK_WINDOW(warning_win), "button_press_event", G_CALLBACK (button_press_event), warning_win);       // 加入事件回调
//        g_signal_connect(GTK_WINDOW(warning_win), "motion_notify_event",G_CALLBACK( motion_notify_event), warning_win);
        g_signal_connect(GTK_WINDOW(warning_win), "button_release_event", G_CALLBACK(button_release_event), warning_win);

//        gtk_window_set_transient_for(warning_win,window);
//        gtk_window_set_modal(warning_win,TRUE);
//        gtk_window_set_position(warning_win,GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(warning_win),380,248);
        gtk_window_set_icon_from_file(GTK_WINDOW(warning_win),image_shadu,NULL);
	gtk_window_set_title(warning_win,("警告"));
        gtk_widget_show(warning_win);

        autodeal_warning_layout=gtk_layout_new(NULL, NULL);
        gtk_widget_show(autodeal_warning_layout);
        gtk_widget_set_size_request(autodeal_warning_layout,378,246);
        memset(image_shadu,0,sizeof(image_shadu));
        snprintf(image_shadu,sizeof(image_shadu),_("%sregular_bg.png"),ANTIVIRUS_IMAGE_DIR);
        autodeal_title_image=gtk_image_new_from_file(image_shadu);
        gtk_widget_show(autodeal_title_image);
        gtk_layout_put(GTK_LAYOUT(autodeal_warning_layout),autodeal_title_image, 0, 0);
        char css_tipdialog[512];
        snprintf(css_tipdialog,sizeof(css_tipdialog),"%stipdialog.css",ANTIVIRUS_CSS_DIR);
        GFile *file=g_file_new_for_path(css_tipdialog);
        GdkScreen *screen;
        screen = gdk_screen_get_default();
        GtkCssProvider *cssProvider;
        cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_file(cssProvider, file, NULL);
	
	 autodeal_title_quit_button=gtk_button_new();
        gtk_button_set_relief((GtkButton *)autodeal_title_quit_button,GTK_RELIEF_NONE);
        gtk_widget_set_size_request(autodeal_title_quit_button,24,24);
        gtk_widget_show(autodeal_title_quit_button);
        g_signal_connect(G_OBJECT(autodeal_title_quit_button),"clicked",G_CALLBACK(yes_callbackfunc),warning_win);
        memset(image_shadu,0,sizeof(image_shadu));
        snprintf(image_shadu,sizeof(image_shadu),_("%sclose.png"),ANTIVIRUS_IMAGE_DIR);
        autodeal_quit_image= gtk_image_new_from_file(image_shadu);
        gtk_widget_show(autodeal_quit_image);
        gtk_button_set_image(autodeal_title_quit_button,autodeal_quit_image);
        gtk_layout_put(GTK_LAYOUT(autodeal_warning_layout), autodeal_title_quit_button, 354, 0);
        gtk_widget_set_name(autodeal_title_quit_button,"tipdialog-close");
        gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);
        memset(image_shadu,0,sizeof(image_shadu));
        snprintf(image_shadu,sizeof(image_shadu),_("%squitupdate_icon.png"),ANTIVIRUS_IMAGE_DIR);
        warning_image = gtk_image_new_from_file(image_shadu);
        gtk_widget_show(warning_image);
        gtk_layout_put(GTK_LAYOUT(autodeal_warning_layout),warning_image, 36, 102);

        warning_label = gtk_label_new(("\t您的网络或者apt工具存在问题,\n可能会影响您的处理结果！"));
        gtk_widget_show(warning_label);
        gtk_layout_put(GTK_LAYOUT(autodeal_warning_layout),warning_label, 115, 106);
        change_font_style(warning_label,"<span foreground=\"#686868\" font_desc='11'>%s</span>");

        yes_button=gtk_button_new();
        gtk_button_set_relief((GtkButton *)yes_button,GTK_RELIEF_NONE);
        gtk_widget_set_size_request(yes_button,82,34);
        gtk_widget_show(yes_button);
        memset(image_shadu,0,sizeof(image_shadu));
        snprintf(image_shadu,sizeof(image_shadu),_("%sdetermine.png"),ANTIVIRUS_IMAGE_DIR);
        yes_button_image=gtk_image_new_from_file(image_shadu);
        gtk_widget_show(yes_button_image);
        gtk_button_set_image(yes_button,yes_button_image);
        gtk_layout_put(GTK_LAYOUT(autodeal_warning_layout),yes_button, 260, 192);

/*	 no_button=gtk_button_new();
        gtk_button_set_relief((GtkButton *)no_button,GTK_RELIEF_NONE);
        gtk_widget_set_size_request(no_button,82,34);
        gtk_widget_show( no_button);
        memset(image_shadu,0,sizeof(image_shadu));
        snprintf(image_shadu,sizeof(image_shadu),_("%squitupgrade_NO.png"),ANTIVIRUS_IMAGE_DIR);
        no_button_image=gtk_image_new_from_file(image_shadu);
        gtk_widget_show(no_button_image);
        gtk_button_set_image(no_button,no_button_image);
        gtk_layout_put(GTK_LAYOUT(autodeal_warning_layout),no_button, 280, 192);*/


        gtk_container_add(GTK_CONTAINER(warning_win),autodeal_warning_layout);
        g_signal_connect(G_OBJECT(yes_button),"clicked",G_CALLBACK(yes_callbackfunc),warning_win);
//        g_signal_connect(G_OBJECT(no_button),"clicked",G_CALLBACK(no_callbackfunc),cancel_var);
}

int main(int argc,char *argv[])
{

        gtk_init(&argc,&argv);

	warning_dialog();
        gtk_main();
        return 0;

}




