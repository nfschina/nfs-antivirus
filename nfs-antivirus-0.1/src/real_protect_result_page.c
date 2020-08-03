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
#include "real_protect_result_page.h"
#define _(STRING) gettext(STRING)
extern GtkWidget *real_protect_result_table;
extern int area;
GtkWidget *real_protect_result_vbox,*tog_all_check_button;
GtkWidget *real_protect_result_list;
void tog_all_real_result(GtkTreeView *treeview)
{
        GtkListStore *store;
        GtkTreeModel *model;
        GtkTreeIter iter;
        model = gtk_tree_view_get_model (GTK_TREE_VIEW(real_protect_result_list));
        store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(real_protect_result_list)));
        gtk_tree_model_get_iter_first(model,&iter);
        gboolean use_select_all=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tog_all_check_button));
        do
        {
                gtk_list_store_set(store,&iter,0,use_select_all,-1);
        }
        while (gtk_tree_model_iter_next(model,&iter));
}
void remove_item_real_protect_result(GtkWidget *widget,gpointer *data){
        not_tog_warning(real_protect_result_list,0,TRUE,1);
}
void add_real_protect_white(GtkWidget *widget,gpointer *data){
	not_tog_warning(real_protect_result_list,0,TRUE,2);
}
void real_protect_result()
{ 
        GtkWidget *detail_fix,*resultlabel,*scantype_label,*detail_scanType_label,*totaltime_label,*detail_totaltime_label,*totalobject_label,*detail_totalobject_label,*real_protect_result_tabletotal;
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
        char image_need_deal[512] ;char css_result[512];
           snprintf(css_result,sizeof(css_result),"%sresult.css",ANTIVIRUS_CSS_DIR);
           snprintf(image_need_deal,sizeof(image_need_deal),_("%sneed-deal.png"),ANTIVIRUS_IMAGE_DIR);

        char detail_scanType[100],scantype[10];
        real_protect_result_table = gtk_grid_new();
	GtkWidget *real_result_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_grid_attach((GtkGrid *)real_protect_result_table,real_result_vbox,0,0,1,1);
	gtk_widget_show(real_result_vbox);
        GtkWidget *layout;
	layout = gtk_layout_new(NULL, NULL);
        gtk_widget_set_size_request(layout, 800, 88);
	gtk_box_pack_start(GTK_BOX(real_result_vbox), layout, TRUE, TRUE, 0);
	gtk_widget_show(layout);
	GtkWidget *resultlabel_image = gtk_image_new_from_file(image_need_deal);
        gtk_layout_put(GTK_LAYOUT(layout), resultlabel_image, 0, 0);
        gtk_widget_show(resultlabel_image);
	
        GFile *file=g_file_new_for_path(css_result);
        GdkScreen *screen;
        screen = gdk_screen_get_default();
        GtkCssProvider *cssProvider;
        cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_file(cssProvider, file, NULL);
        back_to_detail_result_button=gtk_button_new();
        gtk_button_set_relief((GtkButton *)back_to_detail_result_button,GTK_RELIEF_NONE);
        memset(image_need_deal,0,sizeof(image_need_deal));
        snprintf(image_need_deal,sizeof(image_need_deal),_("%sback_menu.png"),ANTIVIRUS_IMAGE_DIR);
        gtk_button_set_image(GTK_BUTTON(back_to_detail_result_button),gtk_image_new_from_file(image_need_deal));
        gtk_layout_put(GTK_LAYOUT(layout), (GtkWidget *)back_to_detail_result_button, 40, 27);
        gtk_widget_show(back_to_detail_result_button);
	gtk_widget_set_name(back_to_detail_result_button,"result_back_to_mainmenu_button");
        gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);
         g_signal_connect(G_OBJECT(back_to_detail_result_button),"clicked",G_CALLBACK(switch_to_mainmenu),NULL);
        sw = gtk_scrolled_window_new(NULL, NULL);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
        gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(sw), GTK_SHADOW_ETCHED_IN);
        gtk_scrolled_window_set_min_content_height((GtkScrolledWindow *)sw,335);
        gtk_box_pack_start(GTK_BOX(real_result_vbox), sw, TRUE, TRUE, 30);
	real_protect_result_list = gtk_tree_view_new_with_model(createModel());
        gtk_widget_set_size_request(real_protect_result_list, 700, 300);
        gtk_container_add(GTK_CONTAINER(sw), real_protect_result_list);
        renderer_tog = gtk_cell_renderer_toggle_new();
        g_object_set(G_OBJECT(renderer_tog),"activatable",TRUE,NULL);
        column_tog = gtk_tree_view_column_new_with_attributes(_("      "),renderer_tog,"active",TOG_COL,NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(real_protect_result_list),column_tog);
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes(_("Date"),renderer, "text", DATE_COL, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(real_protect_result_list), column);
	g_signal_connect (renderer_tog, "toggled", G_CALLBACK (select_toggled),real_protect_result_list);
        renderer = gtk_cell_renderer_text_new();

        column = gtk_tree_view_column_new_with_attributes(_("Name"),renderer, "text", NAME_COL, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(real_protect_result_list), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes(_("Threatnum"),renderer, "text", THREATNUM_COL, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(real_protect_result_list), column);
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes(_("Threatname"),renderer, "text", THREATNAME_COL, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(real_protect_result_list), column);
        gtk_widget_show_all(sw);
	bottom_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(real_result_vbox), bottom_hbox, TRUE, TRUE, 5);
        gtk_widget_show(bottom_hbox);
	tog_all_check_button=gtk_check_button_new_with_label (_("select all"));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tog_all_check_button),FALSE);
	gtk_box_pack_start(GTK_BOX(bottom_hbox),tog_all_check_button, FALSE, TRUE, 3);

        gtk_widget_show(tog_all_check_button);
	g_signal_connect(G_OBJECT(tog_all_check_button), "toggled", G_CALLBACK(tog_all_real_result), real_protect_result_list);
	remove = gtk_button_new();
        gtk_button_set_relief((GtkButton *)remove,GTK_RELIEF_NONE);
        memset(image_need_deal,0,sizeof(image_need_deal));
        snprintf(image_need_deal,sizeof(image_need_deal),_("%sdel_thread.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *removeimage = gtk_image_new_from_file(image_need_deal);
        gtk_widget_show(removeimage);
	gtk_button_set_image(remove,removeimage);
	gtk_box_pack_start(GTK_BOX(bottom_hbox),remove, FALSE, TRUE, 5);
	gtk_widget_show(remove);
	g_signal_connect(G_OBJECT(remove), "clicked", G_CALLBACK(remove_item_real_protect_result), NULL);
	GtkWidget *add_white_list_button = gtk_button_new();
        memset(image_need_deal,0,sizeof(image_need_deal));
        snprintf(image_need_deal,sizeof(image_need_deal),_("%sadd_white_list.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *add_white_list_image = gtk_image_new_from_file(image_need_deal);
        gtk_widget_show(add_white_list_image);
	gtk_button_set_image(add_white_list_button,add_white_list_image);
        gtk_button_set_relief((GtkButton *)add_white_list_button,GTK_RELIEF_NONE);
	gtk_box_pack_start(GTK_BOX(bottom_hbox),add_white_list_button, FALSE, TRUE, 5);
	gtk_widget_show(add_white_list_button);
	g_signal_connect(G_OBJECT(add_white_list_button), "clicked", G_CALLBACK(add_real_protect_white), NULL);
	GtkTreeIter iter;
        GtkTreeModel *model= gtk_tree_view_get_model (GTK_TREE_VIEW(real_protect_result_list));

}
