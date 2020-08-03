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
#include <stdio.h>
#include <locale.h>
#include <libintl.h>
#define _(STRING) gettext(STRING)
#define SETTING_PATH "/usr/share/antivirus/config/setting.txt"
#define ANTIVIRUS_IMAGE_DIR     "/usr/share/antivirus/images/"
#define ANTIVIRUS_CSS_DIR       "/usr/share/antivirus/css/"
#define ANTIVIRUS_CONF_DIR      "/usr/share/antivirus/config/"
char g_current_user[256]; //当前启动杀毒软件用户
gboolean drag = FALSE;   // 只在左键按下时拖动窗体
int nX = 0;
int nY = 0;
gint button_press_event(GtkWidget * widget,GdkEventButton * event, gpointer data);
gint button_release_event(GtkWidget * widget, GdkEventButton * event,gpointer data);
//gint motion_notify_event(GtkWidget * widget, GdkEventButton * event,gpointer data);
int lastIndexOf(char *str1,char *str2)
{
    char *p=str1;
    int i=0,len=strlen(str2);
    p=strstr(str1,str2);
    if(p==NULL)return -1;
    while(p!=NULL)
    {
        for(;str1!=p;str1++)i++;
        p=p+len;
        p=strstr(p,str2);
    }
    return i;
}
int get_antivirus_setting_item_inttype(char *Lable,char *file_info)
{
        char linebuffer[512];
        char label_buffer[512];
        char value_buffer[512];

        int line_len=0;
        int len=0;
        int res;

        /*char file_name[512],file_path[512],read_file_path[1024];
        int i =0 ;
        i = lastIndexOf(file_info,"/");
        if(i > 0 )
        {
            if(i< sizeof(file_path))
            {
                strncpy(file_path,file_info,i);
            }
            snprintf(file_name,sizeof(file_name),"%s_%s",g_current_user,&file_info[i+1]);
            snprintf(read_file_path,sizeof(read_file_path),"%s/%s",file_path,file_name);
        }
        else
        {
            strncpy(read_file_path,file_info,sizeof(read_file_path));
        }
*/

		char set_file_path[1024];
		snprintf(set_file_path,sizeof(set_file_path),"%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);

        FILE *fp=fopen(set_file_path,"r+");
        if (fp == NULL)
        {
                return -1;
        }
        while(fgets(linebuffer,512,fp))
        {
                line_len=strlen(linebuffer);
                len +=line_len; 
                sscanf(linebuffer, "%[^=]=%[^=]", label_buffer,value_buffer);
                if(!strcmp(Lable, label_buffer))
                { 
                        len -= strlen(linebuffer);
                        res = fseek(fp, len, SEEK_SET);
                        if(res < 0)
                        {
                                perror("fseek");
                        }
                        int value=atoi(value_buffer);
                        fclose(fp);
                        return value;
                }
        }
        return 0;
}
void change_font_style(GtkLabel *label,char *cssInfo)
{
        char *markup;
        const gchar *labelTitel = gtk_label_get_text(label);
        markup = g_markup_printf_escaped (cssInfo, labelTitel);
        gtk_label_set_markup (GTK_LABEL (label), markup);
        g_free (markup);
}

void  destroy_tipdialog(GtkWidget *widget ,gpointer *data)
{
        system("rm -f /usr/share/antivirus/log/clamd.log");
        system("rm -f /usr/share/antivirus/temp/real_time_sure.txt");
		char cmd[512];
		snprintf(cmd,sizeof(cmd),"rm -f /usr/share/antivirus/temp/%s_real_time_sure.txt",g_current_user);
		system(cmd);
	gtk_main_quit();
	return FALSE;
}

void save_tipdialog(GtkWidget *widget ,gpointer *data)
{
	  char cmd[512];
	  snprintf(cmd,sizeof(cmd),"echo 1 >> /usr/share/antivirus/temp/%s_real_time_sure.txt",g_current_user);
      system(cmd);
      gtk_main_quit();
}
int main(int argc,char *argv[])
{
	gtk_init(&argc,&argv);

	if(argc >= 2)
	{
		if(strcmp(argv[1],"") != 0)
		{
			snprintf(g_current_user,sizeof(g_current_user),argv[1]);
		}
	}

        //指定杀毒软件语言与系统语言一致
        setlocale(LC_ALL,"");

        //指定汉化文件名
        textdomain("antivirus");

        show_warn_info(_("thread found,please deal now!"),2);
	gtk_main();
        return FALSE;
}
void  show_warn_info(char *warnMessage,int dailog_type)
{
   GtkWidget *tipdialog_layout,*tipdialog_bg_image,*close_tipdialog_button,*close_tipdialog_image,*tip_image,*tip_label,*deal_button,*deal_button_image,*cancel_button,*cancel_button_image,*tip_dialog;
//*thread_name_label,*thread_name,*thread_location_label,*thread_location;
   GdkScreen* rt_screen;
   char image_dir[256];
   gint win_width,win_height,rt_screen_width,rt_screen_height,win_x,win_y;


   rt_screen = gdk_screen_get_default();
   rt_screen_width = gdk_screen_get_width(rt_screen);
   rt_screen_height = gdk_screen_get_height(rt_screen);

   win_width=273;
   win_height=158;
   win_x=rt_screen_width-win_width;
   win_y=rt_screen_height-win_height;
   tip_dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(tip_dialog,_("warning"));
   snprintf(image_dir,sizeof(image_dir),_("%sNFSantivirus.png"),ANTIVIRUS_IMAGE_DIR);
   gtk_window_set_icon_from_file(GTK_WINDOW(tip_dialog),image_dir,NULL);
   gtk_window_set_decorated(GTK_WINDOW(tip_dialog), FALSE);
   gtk_window_set_position(GTK_WINDOW(tip_dialog),GTK_WIN_POS_CENTER);
   gtk_widget_set_events(GTK_WINDOW(tip_dialog),
                   GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK
                   | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);

    g_signal_connect(GTK_WINDOW(tip_dialog), "button_press_event", G_CALLBACK (button_press_event), tip_dialog);       // 加入事件回调
//    g_signal_connect(GTK_WINDOW(tip_dialog), "motion_notify_event",G_CALLBACK( motion_notify_event), tip_dialog);
    g_signal_connect(GTK_WINDOW(tip_dialog), "button_release_event", G_CALLBACK(button_release_event), tip_dialog);

    char tipdialog_css[256];
    snprintf(image_dir,sizeof(image_dir),"%stipdialog.css",ANTIVIRUS_CSS_DIR);
    GFile *file=g_file_new_for_path(tipdialog_css);
    GdkScreen *screen;
    screen = gdk_screen_get_default();
    GtkCssProvider *cssProvider;
    cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_file(cssProvider, file, NULL);

    tipdialog_layout = gtk_layout_new(NULL, NULL);
    gtk_widget_show(tipdialog_layout);
    gtk_widget_set_size_request(tipdialog_layout, win_width, win_height);
    gtk_window_move(GTK_WINDOW(tip_dialog),win_x,win_y);

    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%sreal_protect_bg.png"),ANTIVIRUS_IMAGE_DIR);
    tipdialog_bg_image = gtk_image_new_from_file(image_dir);
    gtk_widget_show(tipdialog_bg_image);
    gtk_layout_put(GTK_LAYOUT(tipdialog_layout),tipdialog_bg_image, 0, 0);

    close_tipdialog_button = gtk_button_new();
    gtk_button_set_relief(close_tipdialog_button,GTK_RELIEF_NONE);
    gtk_widget_show(close_tipdialog_button);
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%sclose.png"),ANTIVIRUS_IMAGE_DIR);
    close_tipdialog_image = gtk_image_new_from_file(image_dir);
    gtk_widget_show(close_tipdialog_image);
    gtk_button_set_image(close_tipdialog_button,close_tipdialog_image);
    gtk_widget_set_size_request(close_tipdialog_button,24,24);
    gtk_widget_set_name(close_tipdialog_button,"tipdialog-close");
    gtk_layout_put(GTK_LAYOUT(tipdialog_layout),close_tipdialog_button, 249, 2);
    g_signal_connect(G_OBJECT(close_tipdialog_button),"clicked",G_CALLBACK(destroy_tipdialog),tip_dialog);
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%sreal_protect_warn.png"),ANTIVIRUS_IMAGE_DIR);
    tip_image = gtk_image_new_from_file(image_dir);
    gtk_layout_put(GTK_LAYOUT(tipdialog_layout),tip_image,29,60);
    gtk_widget_show(tip_image);
    tip_label = gtk_label_new("");
    gtk_widget_show(tip_label);
    gtk_label_set_text(tip_label,warnMessage);
    change_font_style((GtkLabel *)tip_label,"<span foreground=\"#000000\" font_desc='10'>%s</span>");
    gtk_layout_put(GTK_LAYOUT(tipdialog_layout),tip_label,100,75);
  /*  thread_name_label = gtk_label_new(_("thread name:"));
    gtk_widget_show(thread_name_label);
    gtk_layout_put(GTK_LAYOUT(tipdialog_layout),thread_name_label,20,135);
    thread_name = gtk_label_new("ssssssss");
    gtk_widget_show(thread_name);
    gtk_layout_put(GTK_LAYOUT(tipdialog_layout),thread_name,140,135);
    thread_location_label = gtk_label_new(_("thread location:"));
    gtk_widget_show(thread_location_label);
    gtk_layout_put(GTK_LAYOUT(tipdialog_layout),thread_location_label,20,155);
    thread_location = gtk_label_new("ssssssss");
    gtk_widget_show(thread_location);
    gtk_layout_put(GTK_LAYOUT(tipdialog_layout),thread_location,140,155);
*/
    

    deal_button = gtk_button_new();
    gtk_button_set_relief((GtkButton *)deal_button,GTK_RELIEF_NONE);
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%snow_deal.png"),ANTIVIRUS_IMAGE_DIR);
    deal_button_image = gtk_image_new_from_file(image_dir);
    gtk_button_set_image(deal_button,deal_button_image);
    gtk_layout_put(GTK_LAYOUT(tipdialog_layout),deal_button,30,110);
    gtk_widget_show(deal_button);
    g_signal_connect(G_OBJECT(deal_button),"clicked",G_CALLBACK(save_tipdialog),NULL);

    cancel_button = gtk_button_new();
    gtk_button_set_relief((GtkButton *)cancel_button,GTK_RELIEF_NONE);
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%signore_deal.png"),ANTIVIRUS_IMAGE_DIR);
    cancel_button_image = gtk_image_new_from_file(image_dir);
    gtk_button_set_image(cancel_button,cancel_button_image);
    gtk_layout_put(GTK_LAYOUT(tipdialog_layout),cancel_button,161,110);
    gtk_widget_show(cancel_button);
    g_signal_connect(G_OBJECT(cancel_button),"clicked",G_CALLBACK(destroy_tipdialog),tip_dialog);
    gtk_container_add(GTK_CONTAINER(tip_dialog),tipdialog_layout);
    int check_protect = get_antivirus_setting_item_inttype("check-real-time-protect",SETTING_PATH);
    if(check_protect==1){
    printf("小窗口代码走到这里了\n");
    gtk_widget_show(tip_dialog);
}
    else{
	printf("用户关闭了实时防护删除文件\n");
	system("rm -f /usr/share/antivirus/log/clamd.log");
	}
    gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);

}
gint button_press_event(GtkWidget * widget,GdkEventButton * event, gpointer data)
{
   /*     if (event->button == 1)       // 判断是否左键按下
        {
                 drag = TRUE;
                 nX = event->x;  // 取得鼠标相对于窗口的位置
                 nY = event->y;
        }
        return TRUE;
}*/
if(event->type=GDK_BUTTON_PRESS)
                         {
                  if (event->button == 1)       // 判断是否左键按>下
                 {gtk_window_begin_move_drag(GTK_WINDOW(gtk_widget_get_toplevel(widget)),event->button,event->x_root,event->y_root,event->time);
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
}*/

