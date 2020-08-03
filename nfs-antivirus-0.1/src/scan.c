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
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include "sharememory.h"
#include "refresh.h"
#include  "function.h"
#include "scan.h"
#include "writelog.h"

#define _(STRING) gettext(STRING)

extern struct ScanInfo scanInfo;

extern const char *virname;

int filesize;
long long int sumsize=0;
long long int pathsSize = 0;
long long int scanoverSize = 0;
int fileNum = 0;
int scanThreatNum = 0;
int thread_run_status = RUN;

char allScanExpectTime[10] = {0};
extern avlInit;

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct scanResults  g_scanResultInfo;

dirNode *currentNode= NULL ;
int  isRead(char *filepath)
{
    	int fd;
    	void *start;
    	struct stat sb;
    	fd = open(filepath, O_RDONLY); /*打开文件*/
    	if(fd==-1)
    	{
        	return 0;
    	}
    	fstat(fd, &sb); /* 取得文件大小 */
    	start = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    	if(start == MAP_FAILED) /* 判断是否映射成功 */
    	{
        	close(fd);
        	return 0;
    	}
    	close(fd);
    	munmap(start,sb.st_size);
    	return 1;
}

int pushPathQueue(char *path, dirNode *head)
{
    	dirNode *p = head;
    	while(p->next != NULL)
	{
        	p = p->next;
    	}
    	dirNode *newNode = (dirNode *)malloc(sizeof(dirNode));
    	strncpy(newNode->dirPath, path, MAX_PATH_SIZE);
    	newNode->next = NULL;
    	p->next = newNode;
    	newNode->pre = p;
    	currentNode = newNode;
    	return 0;
}

int popPathQueue(dirNode *head, char outPath[])
{
    	dirNode *p = head;
    	dirNode *tmp = NULL;
    	while(p->next != NULL)
	{
        	p = p->next;
   	}
    	tmp = p->pre;
    	tmp->next = NULL;
    	strcpy(outPath, p->dirPath);
    	currentNode = tmp;
    	return 0;
}


int  getFilesize(char * filepath)
{
        FILE *pBufSize;
        char resultBuffer[256];
        char cmd[512];
        snprintf(cmd,sizeof(cmd),"du -k| grep %s | awk '{print $1}'",filepath);

        if((pBufSize = popen(cmd,"r")) == NULL)
        {
            return 0;
        }
        fgets(resultBuffer,256,pBufSize);
        fclose(pBufSize);
        filesize = atoi(resultBuffer);
        sumsize=sumsize+filesize;
        return sumsize;

}

int isLinkFile(char *filePath, char newFilePath[])
{
    
    	const char *filefullname;
    	GFile *filedir;
    	GFileInfo *file_info = NULL;
    
    	gchar attributes [256] = {G_FILE_ATTRIBUTE_STANDARD_NAME ","
                              G_FILE_ATTRIBUTE_STANDARD_SIZE ","
                              G_FILE_ATTRIBUTE_STANDARD_IS_SYMLINK ","
                              G_FILE_ATTRIBUTE_STANDARD_SYMLINK_TARGET ","
                              G_FILE_ATTRIBUTE_STANDARD_TYPE};
    
    	filedir=g_file_new_for_path (filePath);
    	file_info = g_file_query_info (filedir,attributes,G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS,NULL,NULL);
    	if(file_info == NULL)
    	{
        	return 1;
    	}
    	if (g_file_info_get_file_type(file_info) == G_FILE_TYPE_SYMBOLIC_LINK)
	{
        	if(NULL != (filefullname = g_file_info_get_symlink_target(file_info)))
		{
            		strcpy(newFilePath, filefullname);
            		return 0;
        	}
		else
		{
            		return 1;
        	}
    	}
	else 
	{
        	return 1;
    	}

}

int fileNumberToTime(char *mode, char expectTime[])
{
        unsigned long fileNumber = 0;
        char path[128] = {0};

        if(strcmp(mode, "quick_scan") == 0)
        {
            fileNumber = countFilesNumber("/lib/modules");
            fileNumber =  fileNumber + countFilesNumber("/boot");
	    snprintf(expectTime, 10, "%02ld:%02ld:%02ld", fileNumber/21/60/60, fileNumber/21%3600/60, fileNumber/21%60);

            return 0;
        }
        else if(strcmp(mode, "all_scan") == 0)
        {
            //fileNumber =  countFilesNumber("/") /3;
            FILE *pBufSize;
            char resultBuffer[256];
            long int sumsize = 0;
            char cmd[512];
            snprintf(cmd,sizeof(cmd),"df -h |grep /dev/|grep G |grep -v media |awk '{print $3}' |awk '{sum+=$1}END{print sum}'");

            if((pBufSize = popen(cmd,"r")) == NULL)
            {
                return 0;
            }
            fgets(resultBuffer,256,pBufSize);
            fclose(pBufSize);
            if(strcmp(resultBuffer,"")!=0)
            {
                sumsize = atoi(resultBuffer)*60*60;
            }
            else
            {
                return 0;
            }
            snprintf(cmd,sizeof(cmd),"df -h |grep /media |awk '{print $3}' |grep -v M |grep -v K |awk '{sum+=$1}END{print sum}'");

            if((pBufSize = popen(cmd,"r")) == NULL)
            {
                return 0;
            }
            fgets(resultBuffer,256,pBufSize);
            fclose(pBufSize);
            if(strcmp(resultBuffer,"")!=0)
            {
                sumsize += atoi(resultBuffer)*60*6;
            }

            long int hour = sumsize/6/60/60;
            if(hour >= 24)
            {
                hour = 6;
            }
            snprintf(expectTime, 10, "%02ld:%02ld:%02ld", hour, sumsize/6%3600/60, sumsize/6%60);
            return 0;

        }
        else
        {
            fileNumber = countFilesNumber(mode);
        }
        fileNumber += 200;

        long int hour = fileNumber/12/60/60 ;
        if(hour >= 24)
        {
            hour = 24;
        }

        snprintf(expectTime, 10, "%02ld:%02ld:%02ld", hour, fileNumber/12%3600/60, fileNumber/12%60);

        return 0;

}




int pathTransformSpecialSymbols(char *path, char outPath[])
{
	int length = 0, i, j = 0, blankNum = 0;
	length = strlen(path);
    	for(i = 0; i < length; i++)
	{
        	if(path[i] == ' ' || path[i] == ')' || path[i] == '(' || path[i] == '}' || path[i] == '{' || path[i] == '`' || path[i] == ':' || path[i] == '&' || \
           		path[i] == '[' || path[i] == ']' || path[i] == '$' || path[i] == '@')
		{
            		blankNum++;
            		outPath[j++] = '\\';
        	}
        	outPath[j++] = path[i];
    	}
    	outPath[j] = '\0';
    	return 0;
}
int fileType(char * path)
{
//扫描用了该函数
    struct stat buf;
    stat(path, &buf);
    if (S_ISDIR(buf.st_mode))
    {
        return DIRECTORY;
    }

    if (S_ISREG(buf.st_mode))          //结果与标志位比较  
    {
        return REGULARFILE;
    }

    return OTHERTYPE;
}


int check_scan_path(char * childFileName)
{
    char devpath[10] = "/dev", syspath[10] = "/sys", procpath[10] = "/proc";
    char * devpos  = NULL,syspos = NULL, procpos = NULL;

    devpos =  strstr(childFileName,devpath) ;
    syspos = strstr(childFileName,syspath);
    procpos = strstr(childFileName,procpath);
    if(devpos == NULL&& syspos == NULL && procpos == NULL)
    {
        return 1;
    }
    return 0;

}

void write_scan_process_info(char * filepath,char *expectTime)
{
    struct scanResults  scanResultInfo;
    char semkeyName[30] = "scanStatusMutexKey";
    char shmkeyName[30] = "scanResultInfoKey";


    fileNum++;

    strcpy(g_scanResultInfo.filename,filepath);
    g_scanResultInfo.scanStatus = SCAN_CONTINUE;
    g_scanResultInfo.scanObjNum = fileNum;
    g_scanResultInfo.scanThreatNum = scanThreatNum;
    strcpy(g_scanResultInfo.scanExpectTime,expectTime );
    g_scanResultInfo.filesize = 0; 
    scanoverSize  = scanoverSize + get_path_size(filepath);
    g_scanResultInfo.filesize = scanoverSize;
    g_scanResultInfo.pathsSize = pathsSize;
    //write_share_memory(semkeyName , shmkeyName, &scanResultInfo);


}
int run_dir(char *filepath)
{
	if(strncmp(filepath,"/run/",strlen("/run/") )==0 && strncmp(filepath,("/run/media/"),strlen("/run/media/") )!=0)
	{
	    return 1;
	}
	return 0;
}

void scan_list_file(char * filepath,char *expectTime,int scantype)
{
    char semkeyName[30] = "scanStatusMutexKey";
    char shmkeyName[30] = "scanResultInfoKey";
    char scanExpectTime[10]= {0};
    struct scanResults  scanResultInfo;

    int ret = 0 ;
    ret = delete_white_list(filepath);
    if(ret == 1)
    {
        return ;
    }
    if(is_devide_dir(filepath)==0)
    {
        return ;
    }
   /* if(run_dir(filepath)==1)
    {
	    printf("filepath is :%s\n",filepath);
        return ;
    }*/

    int curr_virus_num = 0;

    char fullPath[1024] = "";
    char outPath[1024] = "";
    pathTransformSpecialSymbols(filepath,outPath);
    if(strcmp(outPath,"") != 0 && isLinkFile(outPath,fullPath) && isRead(outPath) &&strcmp(filepath,"/run/utmp")!=0 && strcmp(filepath,"/run/cockpit/active.motd")!=0 && strcmp(filepath,"/run/tuned/tuned.pid") !=0)
     {
	printf("bbbbbbbbbbbbbbbbbbbb clamav_scan is :%s\n",filepath);
        clamav_scan(filepath);
        write_scan_process_info(filepath,expectTime);
     }
}


void listdir(char *path,char *expectTime,int scantype)
{
    DIR              *pDir ;
    struct dirent    *ent  ;
    int               i=0  ;
    char              childpath[1024];
    char              childFileName[1024];

    if((strcmp(path, "/proc") == 0) || (strcmp(path, "//proc") == 0))
    {
        printprocdir(path, expectTime);
        return;
    }

    if(scantype==5 || scantype == CLICK_FILE_SCAN )
    {
		char file_path[1024];
        pathTransformSpecialSymbols(path, file_path);
        int ret = 0 ;
        ret = delete_white_list(file_path);
        if(ret == 1)
        {
            return ;
        }
	        printf("scan clamav is aaaaaaaaaaaa:%s\n",file_path);
		clamav_scan(file_path);
		write_scan_process_info(file_path,expectTime);
        //scan_list_file(file_path,expectTime, scantype);
                   /* fileNum++;
                    clamav_scan(file_path);                    
                    strcpy(scanResultInfo.filename,path);
                    scanResultInfo.scanStatus = SCAN_CONTINUE;
                    scanResultInfo.scanObjNum = fileNum;
                    scanResultInfo.scanThreatNum = scanThreatNum;
                    strcpy(scanResultInfo.scanExpectTime,scanExpectTime );
                    scanResultInfo.filesize = 0;
                    scanoverSize  = scanoverSize + get_path_size(file_path);
                    scanResultInfo.filesize = scanoverSize;
                    scanResultInfo.pathsSize = pathsSize;
                    usleep(1);
                    write_share_memory(semkeyName , shmkeyName, &scanResultInfo);
                memset(path,0,sizeof(path));*/
         return ;
    }


    pDir=opendir(path);
    if(pDir == NULL)
    {
        return ;
    }
    memset(childpath,0,sizeof(childpath));
    memset(childFileName,0,sizeof(childFileName));


    while((ent=readdir(pDir))!=NULL)
    {
        pthread_mutex_lock(&mut);

        while (!thread_run_status)
        {
            pthread_cond_wait(&cond, &mut);
        }

       pthread_mutex_unlock(&mut);
       if(ent->d_type & DT_DIR)
        {

            if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0)
                continue;

            if(strcmp(path,"/") != 0)
            {

                snprintf(childpath,sizeof(childpath),"%s/%s",path,ent->d_name);

            }
            else
            {

                snprintf(childpath,sizeof(childpath),"%s%s",path,ent->d_name);

            }


            listdir(childpath,expectTime,scantype);

        }
        else
        {
            snprintf(childFileName,sizeof(childFileName),"%s/%s",path,ent->d_name);
            if(fileType(childFileName) == REGULARFILE)
            {
                scan_list_file(childFileName,expectTime, scantype);
            }
            else if(fileType(childFileName) == DIRECTORY && check_scan_path(childFileName))
            {
                listdir(childFileName,expectTime, scantype);
            }

        }
     }
     closedir(pDir);

 }
/*

void listdir(char *scanPath, char *expectTime,int scantype)
{
    	DIR *pDir;
    	struct dirent *ent;
    	struct stat buf;
    	int i=0, ret =0;
    	char childpath[1024] = {0};
    	char path[1024] = {0};
    	char filepath[1024] = {0};
    	char filePath[1024] = {0};
    	char file_path[1024] = {0};
    	char semkeyName[30] = "scanStatusMutexKey";
    	char shmkeyName[30] = "scanResultInfoKey";
    	char scanExpectTime[10]= {0};
    	struct scanResults  scanResultInfo;
    	const char *filefullname;
    	char test_path[256];
    	dirNode *head = (dirNode *)malloc(sizeof(dirNode));
    	head->next = NULL;
    	head->pre= NULL;

    	pushPathQueue(scanPath, head);

    	strcpy(scanExpectTime, expectTime);
    	do
	{
        	memset(path, 0, sizeof(path));
        	popPathQueue(head, path);
        	if(strcmp(path, "//usr/lib/python3") == 0 || strcmp(path, "/usr/lib/python3") == 0 || strcmp(path,"/dev/shm")==0 || strcmp(path,"//dev/shm")==0 \
	  		|| strcmp(path,"/sys/devices/pci0000:00")==0 || strcmp(path,"//sys/devices/pci0000:00")==0 \
          		|| strcmp(path,"/sys/devices/pci0000:ff")==0 || strcmp(path,"//sys/devices/pci0000:ff")==0 ) 
		{
            		continue;
        	}
        	if((strcmp(path, "/proc") == 0) || (strcmp(path, "//proc") == 0))
		{
            		printprocdir(path, expectTime);
            		continue;
        	}
		if(scantype==5 || scantype == CLICK_FILE_SCAN )
		{
                	pathTransformSpecialSymbols(path, file_path);
                	int ret = 0 ;
                	ret = delete_white_list(file_path);
                	if(ret == 1)
                	{
                     		continue ;
                	}
                	fileNum++;
                	clamav_scan(file_path);                    
                	strcpy(scanResultInfo.filename,path);
                	scanResultInfo.scanStatus = SCAN_CONTINUE;
                	scanResultInfo.scanObjNum = fileNum;
                	scanResultInfo.scanThreatNum = scanThreatNum;
                	strcpy(scanResultInfo.scanExpectTime,scanExpectTime );
                	scanResultInfo.filesize = 0;
                	scanoverSize  = scanoverSize + get_path_size(file_path);
                	scanResultInfo.filesize = scanoverSize;
                	scanResultInfo.pathsSize = pathsSize;
                	usleep(1);
                	write_share_memory(semkeyName , shmkeyName, &scanResultInfo);
	        	memset(path,0,sizeof(path));
		}
		else
		{    
        		pDir=opendir(path);
        		if(pDir == NULL)
        		{
             			continue;
        		}
        		while((ent=readdir(pDir))!=NULL)
        		{ 
            			pthread_mutex_lock(&mut);
            			while (!thread_run_status)
            			{
                			pthread_cond_wait(&cond, &mut);
            			}
            			pthread_mutex_unlock(&mut);
        			sprintf(test_path,"%s/%s",path,ent->d_name);
			int r = lstat(test_path, &buf);
        		if( r == 0 )
        		{
                		if( buf.st_mode & S_IFDIR )
				{
                    			if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0)
                            			continue;
                    			if(path[strlen(path)-1] =='/')
                    			{
                       				snprintf(childpath,MAX_PATH_SIZE,"%s%s",path,ent->d_name);
                    			}
                    			else
                    			{
                        			snprintf(childpath,MAX_PATH_SIZE,"%s/%s",path,ent->d_name);
                    			}
                    			pushPathQueue(childpath, head);
            			} 
				else if((strcmp(ent->d_name,"build")!=0) && (strcmp(ent->d_name,"source")!=0))
				{
                			snprintf(childpath,MAX_PATH_SIZE,"%s",path);
                			if(strlen(childpath) + strlen(ent->d_name) > 1023)
                    				continue;
                			snprintf(filepath,MAX_PATH_SIZE,"%s/%s",childpath,ent->d_name);
                			pathTransformSpecialSymbols(filepath, file_path);
		                	
                			if(isLinkFile(filepath, filePath) && (access(filepath, 0) == 0))
					{
						if(strcmp(filepath,"/dev/ttyprintk")==0 || strcmp(filepath,"//dev/ttyprintk")==0)
				                        continue;
			                        if(isRead(filepath)==0)
                                                {
                                                        fileNum++;
                        				strcpy(scanResultInfo.filename,filepath);
                    					scanResultInfo.scanStatus = SCAN_CONTINUE;
                    					scanResultInfo.scanObjNum = fileNum;
                    					scanResultInfo.scanThreatNum = scanThreatNum;
                                                        if(scantype==ALL_SCAN)
                                                        {
                                                            strcpy(scanResultInfo.scanExpectTime,allScanExpectTime );

                                                        }
                                                        else
                                                        {
                    					    strcpy(scanResultInfo.scanExpectTime,scanExpectTime );
                                                        }
                   					scanResultInfo.filesize = 0;
                    					scanoverSize  = scanoverSize + get_path_size(file_path);
                    					scanResultInfo.filesize = scanoverSize;
                    					scanResultInfo.pathsSize = pathsSize;
                   					usleep(1);
                    					write_share_memory(semkeyName , shmkeyName, &scanResultInfo);
                        				continue;
                    				}
                                             int ret = 0 ;
                                                ret = delete_white_list(filepath);
                                                if(ret == 1)
                                                {
                                                    continue ;
                                                }
						if(is_devide_dir(filepath)==0)
                                                {
                                                        continue;
                                                }
                                                fileNum++;
						clamav_scan(filepath);
		                    		strcpy(scanResultInfo.filename,filepath);
                		    		scanResultInfo.scanStatus = SCAN_CONTINUE;
                    				scanResultInfo.scanObjNum = fileNum;
                		    		scanResultInfo.scanThreatNum = scanThreatNum;
                    				strcpy(scanResultInfo.scanExpectTime,scanExpectTime );
		                    		scanResultInfo.filesize = 0;
                		    		scanoverSize  = scanoverSize + get_path_size(file_path);
                    				scanResultInfo.filesize = scanoverSize;
		                    		scanResultInfo.pathsSize = pathsSize;
                		    		usleep(1);
                    				write_share_memory(semkeyName , shmkeyName, &scanResultInfo);
                			}
                			memset(filepath,0,sizeof(filepath));
                			memset(filePath,0,sizeof(filePath));
                			memset(file_path,0,sizeof(file_path));
            			}
        		}
		}
        	closedir(pDir);
        	memset(childpath,0,sizeof(childpath));
	}
    }while(currentNode !=NULL && currentNode->pre != NULL);

}*/

void printprocdir(char *procdir, char *expectTime)
{
	DIR *dir;
        struct dirent *ptr;
        int ret = 0;
        char path[64] = {0};
        char fullname[1024] = {0};
        char filefullname[1024] = {0};
        char semkeyName[30] = "scanStatusMutexKey";
        char shmkeyName[30] = "scanResultInfoKey";
        char scanExpectTime[10]= {0};
        struct scanResults  scanResultInfo;
        int whiteListFlag = 0 ;
 
        strcpy(scanExpectTime, expectTime);

        if((dir=opendir(procdir))!=NULL)
        {
           	while((ptr=readdir(dir))!=NULL)
           	{
              		if((ptr->d_name[0]>='0')&&(ptr->d_name[0]<='9'))
              		{
                  		snprintf(path, MAX_PATH_SIZE,"cd /proc/%s/", ptr->d_name);
                  		chdir(path);
				if(system("find ./ -name exe"));
                  		{
                        		snprintf(fullname,MAX_PATH_SIZE,"/proc/%s/exe",ptr->d_name);
                        		ret = isLinkFile(fullname, filefullname);
                        		if(ret && !filefullname && (access(filefullname, 0) == 0))
                        		{
			   			getFilesize(filefullname);
                           			strcpy(g_scanResultInfo.filename,filefullname );
                           			int ret = 0 ;
                           			ret = delete_white_list(filefullname);
                           			if(ret == 1)
                           			{
                               				continue ;
                           			}
						printf("aaaaaaaaaaaaaaaaaaaaaaaaaa filefullname is :%s\n",filefullname);

			   			clamav_scan(filefullname);
                           			g_scanResultInfo.scanStatus = SCAN_CONTINUE;
                           			fileNum++;
                           			g_scanResultInfo.scanObjNum = fileNum;
                           			g_scanResultInfo.scanThreatNum = scanThreatNum;
                           			strcpy(g_scanResultInfo.scanExpectTime,scanExpectTime );
                           			g_scanResultInfo.filesize = 0;
                           			scanoverSize  = scanoverSize + get_path_size(filefullname);
                           			g_scanResultInfo.filesize = scanoverSize;
                           			g_scanResultInfo.pathsSize = pathsSize;
                           			//write_share_memory(semkeyName , shmkeyName, &scanResultInfo);
                           			usleep(1);
                        		}
                   		}
              		}
           	}
        }
        closedir(dir);
}

void scan_procdir()
{
        unsigned long fileNumber = 0;
        char *targetDir[]={"/lib","/boot","/proc"};
	int i=0;
        char expectTime[10] = {0};

        fileNumberToTime("quick_scan", expectTime);
        
        while(i<=2)
	{
		if(strcmp("/proc",targetDir[i])==0)
		{	
			printprocdir(targetDir[i], expectTime);
		}
		else
			listdir(targetDir[i], expectTime,2);
		i++;
	}
}


void allscan_dir()
{
        unsigned long fileNumber = 0;
        char *targetDir="/";
	char expectTime[10] = {0};
        fileNumberToTime("all_scan", allScanExpectTime);
	strcpy(expectTime, allScanExpectTime);
        listdir(targetDir, expectTime,1);
}
void scan_finish()
{
    	char semkeyName[30] = "scanStatusMutexKey";
    	char shmkeyName[30] = "scanResultInfoKey";
    	struct scanResults  scanResultInfo;
    	g_scanResultInfo.scanStatus = SCAN_FINISH;
    	//write_share_memory(semkeyName , shmkeyName, &scanResultInfo);
}

unsigned long countFilesNumber(char *path)
{
    	char fullPath[256] = {0};
    	char currentPath[256] = {0};
    	unsigned long filesCount = 0;
    	dirNode *head = (dirNode *)malloc(sizeof(dirNode));
    	head->next = NULL;
    	pushPathQueue(path, head);

    	DIR *pDir;
    	struct dirent *ent;

    	do
	{
        	memset(fullPath, 0, sizeof(fullPath));
        	popPathQueue(head, fullPath);
     
        	pDir=opendir(fullPath);
        	if(pDir == NULL)
        	{
            		continue;
        	}
        	while((ent=readdir(pDir)) != NULL)
		{
            		if(ent->d_type & DT_DIR)
			{
                		if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0)
                    			continue;
                		memset(currentPath, 0, sizeof(currentPath));
                		snprintf(currentPath, MAX_PATH_SIZE, "%s/%s", fullPath, ent->d_name);
                		pushPathQueue(currentPath, head);
            		}
			else
			{
                		filesCount++;
            		}
        	}
        	closedir(pDir);
    	}while(head->next);

    	return filesCount;
}

void getExpectTime(long int pathSize,char expectTime[])
{
    	long int sizeG = pathSize/1024/15;
    	snprintf(expectTime, 10, "%02ld:%02ld:%02ld", sizeG/3600, sizeG%3600/60, sizeG%60);

}
void scan_specified_dir(char *scanPath,long int pathSize,int scantype)
{
        unsigned long fileNumber = 0;
        char expectTime[10] = {0};
        char outPath[1024] = {0};
        fileNumberToTime(scanPath, expectTime);
        replace_string(scanPath, "\\\ "," ");
        listdir(scanPath, expectTime,scantype);
}

/*******获取usb可用大小******/
int get_specified_path_size(char *path)
{
	FILE *pBufSize;
        char resultBuffer[256];
        long int sumsize = 0;
        char cmd[512];
        char transformPath[1024];

        sleep(3);

        snprintf(cmd,sizeof(cmd),"df -h  %s | awk '{print $3}' | cut -d 'G' -f1 | sed -n 2p ", path);
        if((pBufSize = popen(cmd,"r")) == NULL)
        {
        	return 0;
        }
        fgets(resultBuffer,256,pBufSize);
        fclose(pBufSize);
        
        sumsize = atoi(resultBuffer)*1024*1024;
        if (sumsize > 0)
        {
            	return sumsize;
        }
        else
        {
            return 0;
        }
}

void specifiedScanThread(char *scanPath)
{
    	int tmp_size = 0;
    	char semkeyName[30] = "scanSizeMutexKey";
    	char shmkeyName[30] = "scanSizeInfoKey";
    	struct scanResults  scanResultInfo;

    	int avlStatus = 0;
	char avl_status_temp[1024];
	snprintf(avl_status_temp,sizeof(avl_status_temp),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
    	avlStatus = get_upgrde_status(avl_status_temp);
    	if(avlInit != 1)
    	{
        	strcpy(g_scanResultInfo.filename,_("Engine is initing...\nWait for a moment"));
        	g_scanResultInfo.scanStatus = SCAN_CONTINUE;
        	g_scanResultInfo.scanObjNum = 0;
        	g_scanResultInfo.scanThreatNum = 0;
        	strcpy(g_scanResultInfo.scanExpectTime,"" );
        	g_scanResultInfo.filesize = 0;
        	g_scanResultInfo.pathsSize = 0;
        	//write_share_memory(semkeyName , shmkeyName, &scanResultInfo);

    	}
    	while( avlInit!= 1 )
    	{
        	sleep(1);
        	avlStatus = get_upgrde_status(avl_status_temp);

    	}
    	write_antivirus_status(avl_status_temp,"3");
    	if(scanInfo.scanType == SPECIFIED_SCAN || scanInfo.scanType == CLICK_DIR_SCAN )
    	{
    		tmp_size = get_specified_path_size(scanPath);
    		if (tmp_size  > 0)
   		    {
        		pathsSize = tmp_size;
    		}
    	}
    	else if(scanInfo.scanType == FIREFOX_DOWNLOAD_SCAN || scanInfo.scanType == CLICK_FILE_SCAN )
    	{
    		tmp_size = get_path_size(scanPath);
        	if (tmp_size  > 0)
        	{
                	pathsSize = tmp_size;
        	}

    	}
    	pthread_mutex_unlock(&mut);
    	thread_run_status = RUN;
    
    	scan_specified_dir(scanPath,pathsSize,scanInfo.scanType);
    	sleep(1);
    	scan_finish();
    	write_antivirus_status(avl_status_temp,"4");

}


void accord_type_scan_file(int *scanType)
{
    	int tmp_size = 0;
    	char semkeyName[30] = "scanSizeMutexKey";
    	char shmkeyName[30] = "scanSizeInfoKey";
	char avl_status_temp[1024];
	snprintf(avl_status_temp,sizeof(avl_status_temp),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);

   
    	tmp_size = get_scan_size(*scanType);
    	if (tmp_size  > 0)
    	{
        	pathsSize = tmp_size;
    	} 
    	else
    	{
        	return;
    	}
    	pthread_mutex_unlock(&mut);
    	thread_run_status = RUN;
    	if(*scanType == FAST_SCAN)
    	{
        	scan_procdir();
    	}
    	else if(*scanType == ALL_SCAN)
    	{
        	allscan_dir();
    	}
    	sleep(1);
    	scan_finish();
    	write_antivirus_status(avl_status_temp,"4");

}

int get_path_size(char *path)
{

     	FILE *pBufSize;
     	char resultBuffer[256];
    	char *ret;
     	int size ;
     	char cmd[1024];
     	memset(resultBuffer,0,sizeof(resultBuffer));
     	memset(cmd,0,sizeof(cmd));
     	if(path == NULL)
     	{
          	return -1;
     	}
     	snprintf(cmd,MAX_PATH_SIZE,"du -sk %s",path);
     	if((pBufSize = popen(cmd,"r")) == NULL)
     	{
         	return -1;
     	}
     	fgets(resultBuffer,256,pBufSize);
     	ret = strtok(resultBuffer,"\t");
     	if(ret == NULL)
     	{
         	pclose(pBufSize);
         return 0;
     }
     size = atoi(ret);
     pclose(pBufSize);
     
     return size;
}
 
int get_scan_size(int scanType)
{
      
    	if(scanType == FAST_SCAN)
    	{
        	int sumsize = get_path_size("/boot") + get_path_size("/lib/modules") ;
        	if (sumsize > 0)
        	{
            		return sumsize;
        	}       
        	else
        	{
            		return -1;
        	}
    	}
    	if(scanType == ALL_SCAN )
    	{
            	FILE *pBufSize;
            	char resultBuffer[256];
            	int sumsize = 0;
            
            	if((pBufSize = popen("df -h| grep /dev/ | awk '{print $3}' | awk '{sum+=$1}END{print sum}'","r")) == NULL)
            	{
                	return 0;
            	}
            	fgets(resultBuffer,256,pBufSize);
            	fclose(pBufSize);
            	sumsize = atoi(resultBuffer)*1024*1024;
            	if (sumsize > 0)
            	{
                	return sumsize;
            	}
            	else
            	{
                	return 0;
            	}
       	}

}

void write_log(char *fileName,char *auchar_malware_name)
{
    	struct scanLogInfo one_scanLogInfo;
    	char logFileName[60];
    	time_t now;
    	char semkeyName[30] = "scanStatusMutexKey";
    	char shmkeyName[30] = "scanResultInfoKey";
    	//struct scanResults  scanResultInfo;
    	int result = 0;
	char results_txt_temp[1024];
	snprintf(results_txt_temp,sizeof(results_txt_temp),"%sscan_results.txt",ANTIVIRUS_TEMP_DIR);

    	now = time(0);
    	struct tm *tnow = localtime(&now);
    	sprintf(one_scanLogInfo.starttime,"%04d/%02d/%02d,%02d:%02d:%02d", 1900+tnow->tm_year,
        tnow->tm_mon+1,
        tnow->tm_mday,
        tnow->tm_hour,
        tnow->tm_min,
        tnow->tm_sec);
    	strcpy(logFileName,results_txt_temp);
    	strcpy(one_scanLogInfo.scanFileName,fileName);
    	one_scanLogInfo.threatNum = 1;
    	strcpy(one_scanLogInfo.threatName,auchar_malware_name);

    	scanThreatNum++;
    	write_scan_log(logFileName,&one_scanLogInfo);
   
    	//result = read_share_memory(semkeyName ,shmkeyName,&scanResultInfo);
    	//if(result != 1)
    	//{
        //	return ;
    	//}
    	//scanResultInfo.scanThreatNum = scanThreatNum;
    	//write_share_memory(semkeyName , shmkeyName, &scanResultInfo);
}

void thread_resume()
{
    	if (thread_run_status == STOP)
    	{
        	pthread_mutex_lock(&mut);
        	thread_run_status = RUN;
        	pthread_cond_signal(&cond);
        	pthread_mutex_unlock(&mut);
    	}
}
void thread_pause()
{
    	if (thread_run_status == RUN)
    	{
        	pthread_mutex_lock(&mut);
        	thread_run_status = STOP;
        	pthread_mutex_unlock(&mut);
    	}
}

int getThreadNum()
{
    	return scanThreatNum;
}

void *getExpectThread()
{
    	fileNumberToTime("all_scan", allScanExpectTime);
}
void get_expect_time_thread()
{
    	pthread_t  getExpectTimeThreadId;
    	pthread_create(&getExpectTimeThreadId,NULL,(void *)getExpectThread,NULL);
}

