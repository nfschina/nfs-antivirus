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
#include "log-page.h"
#include "../function.h"
#define _(STRING) gettext(STRING)
/** table1为扫描按钮，table6为扫描过程界面*/
extern GtkWidget *table1,*table2,*table3,*table4;
extern GtkWidget * mainPageIntroduceImage;
extern int area ; //当前区域标志
extern GtkWidget *box,*while_list_content_box;
extern GtkWidget *antibutton,*setbutton,*logbutton,*aboutbutton;

//创建关于界面
void create_about_page()
{
	char image_dir[256];
      	table4 = gtk_grid_new();
	GtkWidget *aboutvbox,*about_layout,*aboutpage_introduce_image;
	aboutvbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    	gtk_widget_set_size_request(aboutvbox, 800, 400);
    	gtk_widget_show(aboutvbox);

	about_layout = gtk_layout_new(NULL, NULL);
    	gtk_widget_set_size_request(about_layout, 800, 400);
    	gtk_widget_show(about_layout);
	
	snprintf(image_dir,sizeof(image_dir),_("%santi_bg.png"),ANTIVIRUS_IMAGE_DIR);
    	GtkWidget *about_bg_image = gtk_image_new_from_file(image_dir);
    	gtk_widget_show(about_bg_image);

	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%saboutpage_introduce.png"),ANTIVIRUS_IMAGE_DIR);
	aboutpage_introduce_image=gtk_image_new_from_file(image_dir);
	gtk_widget_show(aboutpage_introduce_image);

    	gtk_layout_put(GTK_LAYOUT(about_layout), about_bg_image, 0, 0);
    	gtk_layout_put(GTK_LAYOUT(about_layout), aboutpage_introduce_image, 68, 25);
	
	gtk_box_pack_start(GTK_BOX (aboutvbox),about_layout,FALSE,FALSE,0);
    	gtk_grid_attach((GtkGrid *)table4,aboutvbox,0,0,1,1);
}

//点击关于按钮，显示关于界面，隐藏其它界面
void aboutwindow(GtkWidget *widget,gpointer data)
{
        char image_dir[256];
        if(area==1)
        {
		gtk_button_set_relief((GtkButton *)antibutton,GTK_RELIEF_NONE);
		snprintf(image_dir,sizeof(image_dir),_("%santi.png"),ANTIVIRUS_IMAGE_DIR);
                GtkWidget *leave_antibutton_image = gtk_image_new_from_file(image_dir);

             //   gtk_button_set_image(antibutton,leave_antibutton_image);
                gtk_widget_hide(table1);
        }
        if(area==2)
        {
		gtk_button_set_relief((GtkButton *)setbutton,GTK_RELIEF_NONE);
		memset(image_dir,0,sizeof(image_dir));
	        snprintf(image_dir,sizeof(image_dir),_("%ssetting.png"),ANTIVIRUS_IMAGE_DIR);
                GtkWidget *leave_setbutton_image = gtk_image_new_from_file(image_dir);

               // gtk_button_set_image(setbutton,leave_setbutton_image);
                gtk_widget_hide(table2);
                if(box!=NULL)
                    gtk_widget_destroy(box);
                white_list_settings(while_list_content_box);
                free_devide_file_list_memory();
                free_white_list_file_memory();
        }
        if(area==3)
        {
		gtk_button_set_relief((GtkButton *)logbutton,GTK_RELIEF_NONE);
                memset(image_dir,0,sizeof(image_dir));
                snprintf(image_dir,sizeof(image_dir),_("%slogging.png"),ANTIVIRUS_IMAGE_DIR);
		GtkWidget *leave_logbutton_image = gtk_image_new_from_file(image_dir);

               // gtk_button_set_image(logbutton,leave_logbutton_image);
                gtk_widget_hide(table3);
                free_log_list_memory();
        }
        if(area==4)
        {
                return ;
        }
        if(area==5)
        {
                //gtk_widget_hide(table5);
        }
        gtk_widget_show(table4);
	gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sabout_enter.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *enter_about_image = gtk_image_new_from_file(image_dir);

     //   gtk_button_set_image(widget,enter_about_image);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sabout_introduce_title.png"),ANTIVIRUS_IMAGE_DIR);
	gtk_image_set_from_file (mainPageIntroduceImage,image_dir);
        area = 4;
}

void leave_aboutbutton(GtkWidget *widget,gpointer *data){
	char image_dir[256];
        if(area==4){
                gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
        snprintf(image_dir,sizeof(image_dir),_("%sabout_enter.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *leave_antibutton_image = gtk_image_new_from_file(image_dir);

       // gtk_button_set_image(widget,leave_antibutton_image);
                return ;
        }
        gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sabout.png"),ANTIVIRUS_IMAGE_DIR);

        GtkWidget *leave_setbutton_image = gtk_image_new_from_file(image_dir);

        gtk_button_set_image(widget,leave_setbutton_image);
}
void enter_aboutbutton(GtkWidget *widget,gpointer *data){

        gtk_button_set_image(widget,NULL);
        g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_aboutbutton),NULL);
}

