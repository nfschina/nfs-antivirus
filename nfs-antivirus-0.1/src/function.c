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
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <regex.h>
#include <string.h>
#include <sys/stat.h>
#include <locale.h>
#include <libintl.h>
#include <clamav.h>
#include <sys/time.h>
#include <time.h>
#include "function.h"
#define _(STRING) gettext(STRING)
struct cl_engine *engine;
const char *virname;
char scan_starttime[50];
extern char g_current_user[256]; //当前启动杀毒软件用户

int clamav_scan(char *filename)
{
	unsigned long int size = 0;
	int ret;
	struct cl_scan_options scanoptions ;
	scanoptions.general = CL_SCAN_GENERAL_ALLMATCHES ;
      scanoptions.parse = (CL_SCAN_PARSE_ARCHIVE && CL_SCAN_PARSE_ELF && CL_SCAN_PARSE_PDF && CL_SCAN_PARSE_SWF && CL_SCAN_PARSE_HWP3);
      scanoptions.heuristic = CL_SCAN_HEURISTIC_BROKEN;
      scanoptions.mail = CL_SCAN_MAIL_PARTIAL_MESSAGE;
       scanoptions.dev = CL_SCAN_DEV_COLLECT_SHA;


	//if((ret = cl_scanfile(filename, &virname, &size, engine, CL_SCAN_STDOPT)) == CL_VIRUS) 
	if((ret = cl_scanfile(filename, &virname, &size, engine, &scanoptions)) == CL_VIRUS) 
	{
		write_log(filename,virname);
    	} 
	else 
	{
        	if(ret == CL_CLEAN) 
		{
                        return 1;
        	} 
		else 
		{
            		cl_engine_free(engine);
            		return 0;
        	}
    	}
	return 1;
}
int  clamav_init()
{

	int  ret;
        unsigned int sigs = 0;
        long double mb;
//        system("rm  -rf /tmp/firefox_download*");
//        system("rm /tmp/.firefox_info");
    if((ret = cl_init(CL_INIT_DEFAULT)) != CL_SUCCESS) 
    {
        printf("Can't initialize libclamav: %s\n", cl_strerror(ret));
        return 0;
    }

    if(!(engine = cl_engine_new())) 
    {
        printf("Can't create new engine\n");
        return 0;
    }

    /* load all available databases from default directory */
    if((ret = cl_load(cl_retdbdir(), engine, &sigs, CL_DB_STDOPT)) != CL_SUCCESS) 
    {
        printf("cl_load: %s\n", cl_strerror(ret));
        cl_engine_free(engine);
        return 0;
    }

    printf("Loaded %u signatures.\n", sigs);

    /* build engine */
    if((ret = cl_engine_compile(engine)) != CL_SUCCESS) 
    {
        printf("Database initialization error: %s\n", cl_strerror(ret));;
        cl_engine_free(engine);
        return 0;
    }
    return 1;
}

void  getsystime()
{
    time_t now;
    now = time(NULL);
    struct tm *tnow = localtime(&now);
    sprintf(scan_starttime,"%04d/%02d/%02d %02d:%02d:%02d", 1900+tnow->tm_year,tnow->tm_mon+1,tnow->tm_mday,tnow->tm_hour,tnow->tm_min,tnow->tm_sec);

}

struct whiteList
{
    char path_name[1024];
    char file_type[10];
    char file_size[30];
    char file_modify_time[20];
};

void notify_pop(char *pop_title,char *pop_description,char *image_path)
{
    char cmd[256];
	char current_user[100];  
    sprintf(cmd,"su -c 'notify-send -i %s  %s  \"%s\"' %s",image_path,pop_title,pop_description,g_current_user);
    system(cmd);
}

int
substr_length (const char *str, unsigned start, unsigned end)
{
  unsigned n = end - start;
  static char stbuf[1024];
  strncpy (stbuf, str + start, n);
  stbuf[n] = 0;
  return strlen(stbuf);
}
int filter_regular_white_list(char * pthreatFileName, char * regular_content)
{
  char *pattern;
  int x, z, lno = 0, cflags = 0;
  char ebuf[1024], lbuf[1024];
  char supplement_regular[1024];
  regex_t reg;
  regmatch_t pm[30];
  const size_t nmatch = 30;
  char temp_content[1024];
  if(regular_content[0] == '*')
  {
     strcpy(temp_content,regular_content+1);
     snprintf(supplement_regular,sizeof(supplement_regular),"[a-zA-Z0-9]%s",temp_content);
  }
  z = regcomp (&reg, supplement_regular, cflags);
  if (z != 0)
  {
      regerror (z,&reg, ebuf, sizeof (ebuf));
      return 0;
  }

  /*对每一行应用正则表达式进行匹配 */
  z = regexec (&reg, pthreatFileName, nmatch, pm, 0);
  if (z == REG_NOMATCH)
     return 0;
  else if (z != 0)
  {
     regerror (z,&reg, ebuf, sizeof (ebuf));
     return 0;
  }

  /* 输出处理结果 */
  for (x = 0; x < nmatch && pm[x].rm_so != -1; ++x)
  {
     //if (!x)
     //printf ("%04d: %s\n", lno, pthreatFileName);
     z = substr_length(pthreatFileName, pm[x].rm_so, pm[x].rm_eo);
     if(z > 0 )
         return 1;
  }


}
int is_devide_dir(char *devide_file)
{   char tmp_isolation_temp[512];
           snprintf(tmp_isolation_temp,sizeof(tmp_isolation_temp),"%sisolation_temp.txt",ANTIVIRUS_TEMP_DIR);
    char buf[1024];
    FILE *fp=fopen(tmp_isolation_temp,"rw");
    if(fp == NULL)
    {
            return 1;
    }
   while(fgets(buf,1023,fp) != NULL)
    {
                int in=0;
                char *p[20];
                char *buffer = buf;
                int j = 0;
                if(buffer ==NULL)
                {
                    fclose(fp);
                    return 0;
                }
                while((p[in]=strtok(buffer,","))!=NULL)
                {
                        in++;
                        buffer=NULL;
                }

                if (strlen(p[0])>0)
                {
                        if(strcmp(devide_file,p[0])==0)
                                return 0;
                }
        }
        fclose(fp);
return 1;
}

int delete_white_list(char *ptempFileName)
{

    char buf[1024];

    struct whiteList onewhiteList;

    char whiteListPath[256];

    snprintf(whiteListPath,256,"%swhite_list.txt",ANTIVIRUS_CONF_DIR);

    FILE *fp=fopen(whiteListPath,"rw");
    if(fp == NULL)
    {
            return 0;

    }


    while(fgets(buf,1023,fp) != NULL)
    {
                int in=0;
                char *p[20];
                char *buffer = buf;
                int j = 0;
                if(buffer ==NULL)
                {
                    fclose(fp);
                    return 0;
                }
                while((p[in]=strtok(buffer,","))!=NULL)
                {
                        in++;
                        buffer=NULL;
                }
                if (strlen(p[0])>0 && strlen(p[1])>0 && strlen(p[2])>0 && strlen(p[3])>0)
                {
                        strncpy(onewhiteList.path_name,p[0],sizeof(onewhiteList.path_name));
                        strncpy(onewhiteList.file_type,p[1],sizeof(onewhiteList.file_type));
                        strncpy(onewhiteList.file_size,p[2],sizeof(onewhiteList.file_size));
                        strncpy(onewhiteList.file_modify_time,p[3],sizeof(onewhiteList.file_modify_time));
                        p[0] = NULL;
                        p[1] = NULL;
                        p[2] = NULL;
                        p[3] = NULL;
                        if(strcmp(onewhiteList.file_type,"2") == 0)
                        {
                                int ret = 0;
                                ret = filter_regular_white_list(ptempFileName,onewhiteList.path_name);
                                if(ret > 0 )
                                {
                                    fclose(fp);
                                    return 1;
                                }

                        }

                        if(strncmp(onewhiteList.path_name,ptempFileName,strlen(onewhiteList.path_name)) == 0 )
                        {
                                if( strlen(ptempFileName) > strlen(onewhiteList.path_name))
                                {
                                    if(ptempFileName[strlen(onewhiteList.path_name)]!='/')
                                    {
                                        continue;
                                    }
                                }
                                if(strcmp(onewhiteList.file_type,"1") == 0)
                                {
                                        fclose(fp);
                                        return 1;
                                }
                                else
                                {
                                        char file_size_now[100];
                                        char file_time_now[100];
                                        char is_useful[100];

                                        compare_file_size_and_time(onewhiteList.path_name,onewhiteList.file_type,onewhiteList.file_size,file_size_now,onewhiteList.file_modify_time,file_time_now,is_useful);
                                        if(strcmp(is_useful,_("useful")) == 0)
                                        {
                                            fclose(fp);
                                            return 1;
                                        }
                                }
                        }
                }

    }

    fclose(fp);
    return 0;

}


/*********获取文件行数函数*
参数：fileName 文件名
返回：文件行数
**************************/
int get_file_line(char *fileName)
{
    char *cmd;
    int line = 0;
    FILE *ptr;
    char buf_ps[100];
    cmd = (char * )malloc(strlen(fileName)+50);
    snprintf(cmd,sizeof(cmd),"cat %s |wc -l ",fileName);
    if((ptr = popen(cmd, "r")) != NULL)
    {
        if(fgets(buf_ps,90, ptr) != NULL)
            line = atol(buf_ps);
        pclose(ptr);
    }
    free(cmd);
    return line;
}

void set_antivirus_setting_item(char *Lable,char *value,char *file_info)
{
/*
    char file_name[512],file_path[512],write_file_path[1024];
    int i =0 ;
    i = lastIndexOf(file_info,"/");
    if(i > 0 )
    {
        if(i< sizeof(file_path))
        {
            strncpy(file_path,file_info,i);
        }
        snprintf(file_name,sizeof(file_name),"%s_%s",g_current_user,&file_info[i+1]);
        snprintf(write_file_path,sizeof(write_file_path),"%s/%s",file_path,file_name);
    }
    else
    {
        strncpy(write_file_path,file_info,sizeof(write_file_path));
    }
*/

    char delete_shell_cmd[512];
    snprintf(delete_shell_cmd,sizeof(delete_shell_cmd),"sed -i '/%s/d' %s ",Lable,file_info);
    system(delete_shell_cmd);

    char write_para_cmd[512];
    snprintf(write_para_cmd,sizeof(write_para_cmd),"echo %s=%s >> %s",Lable,value,file_info);
    system(write_para_cmd);
}


int get_antivirus_setting_item_inttype(char *Lable,char *file_info)
{
        char linebuffer[512];
        char label_buffer[512];
        char value_buffer[512];

        int line_len=0;
        int len=0;
        int res;

    /*    char file_name[512],file_path[512], read_file_path[1024];
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
        FILE *fp=fopen(file_info,"r+");
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

int get_antivirus_setting_item_stringtype(char *Lable,char *value ,char *file_conf)
{
        char linebuffer[512];
        char label_buffer[512];
        char value_buffer[512];

        int line_len=0;
        int len=0;
        int res;

		printf("file_conf is :%s\n",file_conf);
	/*	printf("bbbbbbbbbbbbbbb g_current_user is :%s,%s\n",g_current_user,file_conf);

        char file_name[512]={0},file_path[512]={0}, read_file_path[1024]={0}, file_info[1024]={0};

		strncpy(file_info,file_conf,sizeof(file_info));
        int i =0 ;
        i = lastIndexOf(file_info,"/");
        if(i > 0 )
        {
            if(i< sizeof(file_path)&& i < sizeof(file_info))
            {
					printf(" i is %d",i);
				memset(file_path,sizeof(file_path),0);
                memcpy(file_path,file_info,i);
				file_path[i]="\0";
            }
            snprintf(file_name,sizeof(file_name),"%s_%s",g_current_user,&file_info[i+1]);
			printf("file_name is :%s,%s\n",file_name,file_path);
            snprintf(read_file_path,sizeof(read_file_path),"%s/%s",file_path,file_name);
        }
        else
        {
            strncpy(read_file_path,file_info,sizeof(read_file_path));
        }

		printf("read_file_path aaaaaaaaaaaaaaaa is :%s\n",read_file_path);
*/
        FILE *fp=fopen(file_conf,"r+");
        if (fp == NULL)
        {
                printf("open error");
                return -1;
        }
		printf("file_conf is :%s\n",file_conf);
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
                        strcpy(value,value_buffer);
                        value[strlen(value)-1] = '\0';
                        fclose(fp);
                        return 1;
                }
        }
        return 0;
}

/**链接字符*/
void strcat_white_list(char *white_list_info,char *style,char *file_name,char *file_space,char *file_time)
{
    strcpy(white_list_info,file_name);
    strcat(white_list_info,",");
    strcat(white_list_info,style);
    strcat(white_list_info,",");
    strcat(white_list_info,file_space);
    strcat(white_list_info,",");
    strcat(white_list_info,file_time);
}

void strcat_more_string(char *dest_string,int length,char *str1,char *str2, char *str3)
{
    strncpy(dest_string,str1,length);
    strcat(dest_string,",");
    strcat(dest_string,str2);
    strcat(dest_string,",");
    strcat(dest_string,str3);
}

void get_file_size(char *path,char *file_space)
{
    FILE * fp;
    int fd;
    struct stat buf;
    fp=fopen(path,"r");
    if(fp==NULL)
	return;
    fd=fileno(fp);
    fstat(fd, &buf);
    int size = buf.st_size;
    sprintf(file_space,"%d",size);
    fclose(fp);
}
void get_file_modify_time(char *path,char *file_modify_time)
{
    FILE * fp;
    int fd;
    struct stat buf;
    fp=fopen(path,"r");
    if(NULL==fp)
    {
	return ;//要返回错误代码
    }
    fd=fileno(fp);
    fstat(fd, &buf);
    long modify_time=buf.st_ctime;
    snprintf(file_modify_time,100,"%ld",modify_time);
    fclose(fp);
}

void is_file(char *path,char *style){
	struct stat info;
	stat(path,&info);
	if(S_ISDIR(info.st_mode)){
		strcpy(style,"1");
	}
	else{
		strcpy(style,"0");
	}
	printf("is_file style is %s\n",style);
}
/**判断IP的合法性**/
int check_ip_valid(const char* ip)
{
    int n[4];
    char c[4];
    if (sscanf(ip, "%d%c%d%c%d%c%d%c",
               &n[0], &c[0], &n[1], &c[1],
               &n[2], &c[2], &n[3], &c[3])
        == 7)
    {
        int i;
        for(i = 0; i < 3; ++i)
            if (c[i] != '.')
                return 0;
        for(i = 0; i < 4; ++i)
            if (n[i] > 255 || n[i] < 0)
                return 0;
        return 1;
    }
    else
        return 0;
}
int checkConnect(char *dst, int cnt)
{
    int i = 0;
    FILE *stream;
    char recvBuf[16] = {0};
    char cmdBuf[256] = {0};

    if (NULL == dst || cnt <= 0)
        return -1;

    sprintf(cmdBuf, "ping %s -c %d -i 0.2 | grep time= | wc -l", dst, cnt);
    stream = popen(cmdBuf, "r");
    fread(recvBuf, sizeof(char), sizeof(recvBuf)-1, stream);
    pclose(stream);
    if (atoi(recvBuf) > 0)
        return 0;
    return -1;
}
/****update-dsj***/
int Check_Net_Connect()
{
    FILE *stream;
    char recvBuf[16] = {0};
    char cmdBuf[256] = {0};

    snprintf(cmdBuf,sizeof(cmdBuf),"host -t txt current.cvd.clamav.net >/dev/null 2>&1;echo $?");
    stream = popen(cmdBuf, "r");
    fread(recvBuf, sizeof(char), sizeof(recvBuf)-1, stream);
    pclose(stream);
    if (atoi(recvBuf) ==0)
        return 0;
    return -1;
}

int Check_Update_Result_Status(char *update_status_file)
{
        FILE *stream;
        char recvBuf[16] = {0};
        char cmdBuf[256] = {0};
        snprintf(cmdBuf,sizeof(cmdBuf),"freshclam >%s 2>&1;echo $?",update_status_file);
        stream = popen(cmdBuf, "r");
        fread(recvBuf, sizeof(char), sizeof(recvBuf)-1, stream);
        pclose(stream);
        if (atoi(recvBuf) ==0)
                return 0;
        return -1;

}
/****update-dsj***/


void cmd_shell_result(const char* command,char * result,int length)
{
    FILE *fpRead;
    fpRead = popen(command, "r");
    char buf[1024];
    memset(buf,'\0',sizeof(buf));
    while(fgets(buf,1024-1,fpRead)!=NULL)
    {
        strncpy(result,buf,length);
        result = buf;
    }
    if(fpRead!=NULL)
        pclose(fpRead);
    return ;
}

void replace_string(char * source_str,char * targ_str,char *val)/*将字符串中指定子字符串用指定字符串代替，targ_str 是被替换的，val是替
换的字符串*/
{
    char temp_sstr[513],result[513];
    char * p,*q;
    int len;len=0;q=p=NULL;
    memset(result,0,sizeof(result));
    memset(temp_sstr,0,sizeof(temp_sstr));
    strcpy(temp_sstr,source_str);
    p=q=temp_sstr;
    len=strlen(targ_str);
    while(q!=NULL)
    {
        if((q=strstr(p,targ_str))!=NULL)
            {
                strncat(result,p,q-p);
                strcat(result,val);
                strcat(result,"\0");
                q+=len;
                p=q;
            }
        else
            strcat(result,p);
    }
    strcpy(source_str,result);
}

int get_file_line_num(char * file_path)
{
    FILE *fp;
    int file_line_num =0;
    int ch;
    if((fp=fopen(file_path,"r+")) == NULL)
    {
        file_line_num = 0;
    }
    else
    {
        while((ch = fgetc(fp)) != EOF)
        {

            if(ch == '\n')
            {
                file_line_num++;
            }

        }
        fclose(fp);
    }
    return file_line_num;

}

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


