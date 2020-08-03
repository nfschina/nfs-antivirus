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
#include <libintl.h>
#include "scan.h"
#include "antivirus.h"
#include "filechooser.h"
#include <locale.h>
#include "function.h"
#define _(STRING) gettext(STRING)
#define _(STRING) gettext(STRING)
GSList *list=NULL;
GSList  *iterator = NULL;
GSList  *iteratorPath = NULL;

extern long long int pathsSize ;
extern thread_run_status;
extern mut;
int filechooser(GtkWidget *window)
{char upgrade_dir[512];
           snprintf(upgrade_dir,sizeof(upgrade_dir),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
   GtkWidget *filew;
   filew =  gtk_file_chooser_dialog_new (_("File selection"),GTK_WINDOW(window),
						GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
						GTK_STOCK_CANCEL, 
						GTK_RESPONSE_CANCEL,
						GTK_STOCK_OK,
						GTK_RESPONSE_OK,
						NULL);

   gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(filew),TRUE); 
   if(gtk_dialog_run (GTK_DIALOG (filew)) == GTK_RESPONSE_OK)
   {
       gtk_widget_hide(filew);
       list=gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(filew));
       check_if_special_path();
       if(list==NULL)
       {
            write_antivirus_status(upgrade_dir,"8");
            gtk_widget_destroy(filew);
            return 0;
       }

	custom_scan_type(window,NULL);

    }
    else
    {
        write_antivirus_status(upgrade_dir,"8");
    }
    gtk_widget_destroy(filew);
    return 0;
}
void freeGlist(){
    if(list)
        g_slist_free(list);
    if(iterator)
        g_slist_free(iterator);
}
void  check_if_special_path()
{
    char *path =NULL;
    GSList  *iterNext=NULL ,*iterPre = NULL;
    for (iterator = list; iterator; iterator = iterator->next)
    {
         path = iterator->data;
         if((strcmp(path, "//usr/lib/python3") == 0) || (strcmp(path, "/usr/lib/python3") == 0)) 
	 {
           char warn_info[256];
           sprintf(warn_info,_("You do not have authorization to scan the directory:%s"),path);
           show_warn_info(warn_info,1);
            if(iterPre !=NULL)
            {
                iterPre->next = iterator->next ; 
            }   
            else
            {
                list = iterator->next ;
            }
        }
        
        iterPre = iterator;

        
    }

    return  ;

}
void selfscanThread()
{ char upgrade_dir[512];
           snprintf(upgrade_dir,sizeof(upgrade_dir),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
    unsigned long fileNumber = 0; 
    char expectTime[10] = {0};
    int check_result = 0;
    char transformPath[1024]= {0};
    long int selfScanSize = 0;
    long int  totalFileNumber = 0;
    pathsSize = 0;
    printf("enter self scan\n");
    pthread_mutex_unlock((pthread_mutex_t *)&mut);
    thread_run_status = RUN;

    for (iteratorPath = list; iteratorPath; iteratorPath = iteratorPath->next)
    {
        memset(transformPath,0,sizeof(transformPath));
        pathTransformSpecialSymbols(iteratorPath->data, transformPath);
        pathsSize  = get_path_size(transformPath) + pathsSize;
        totalFileNumber = countFilesNumber(transformPath) + totalFileNumber;
        selfScanSize = pathsSize/1024;
    }

    long int hour = totalFileNumber/12/60/60;
    if(hour >= 24)
    {
        hour = 24 ;
    }

    snprintf(expectTime, 10, "%02ld:%02ld:%02ld", hour , totalFileNumber/12%3600/60, totalFileNumber/12%60);


    for (iterator = list; iterator; iterator = iterator->next) 
    {
        memset(transformPath,0,sizeof(transformPath));
        pathTransformSpecialSymbols(iterator->data, transformPath);
        fileNumberToTime(transformPath, expectTime);
        listdir(iterator->data, expectTime,3);
    }
    sleep(1);
    scan_finish();
    write_antivirus_status(upgrade_dir,"4");
}
