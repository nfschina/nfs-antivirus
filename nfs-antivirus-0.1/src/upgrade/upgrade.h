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
#define FILE_EXIT 1
#define FILE_NOT_EXIT 2

#define AVL_NOT_NEED_UPGRADE 1
#define AVL_UPGADE_SUCESS 2
#define AVL_UPGADE_FAIL 3
#define AVL_IS_USE_LIB   4
#define AVL_IS_NOT_USE_LIB 5


#define AVL_INIT_STATUS 1
#define AVL_INIT_OVER   2
#define AVL_SCAN_STATUS 3
#define AVL_SCAN_OVER   4
#define AVL_CLOSE_STATUS 5
#define AVL_UPGADE_STATUS 6
#define AVL_UPGADE_END   7
#define AVL_SWITCH_PAGE_END 8

#define ANTIVIRUS_TEMP_DIR      "/usr/share/antivirus/temp/"
#define ANTIVIRUS_LOG_DIR       "/usr/share/antivirus/log/"
#define ANTIVIRUS_BIN_DIR       "/usr/share/antivirus/bin/"
int if_need_upgrade(char *newpath);
int diffHashValue(char *newpath, char * oldpath);
void  backup_old_lib(char *oldpath);
