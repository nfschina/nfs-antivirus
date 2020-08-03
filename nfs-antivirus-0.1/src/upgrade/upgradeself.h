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
/****网络检测状态值****/
#define AVL_NETWORK_CHECK_STATUS_SUCCESS 6
#define AVL_NETWORK_CHECK_STATUS_FAIL 7
#define AVL_NETWORK_CHECK_RESULT_PASS 8
#define AVL_NETWORK_CHECK_RESULT_FAIL 9

/****引擎版本检测状态值****/
#define AVL_ANTI_ENGINE_CHECK_SAME_NOW 10
#define AVL_ANTI_ENGINE_CHECK_SAME_LABLE 11
#define AVL_ANTI_ENGINE_CHECK_RESULT_PASS 12

#define AVL_ANTI_ENGINE_CHECK_DIFFERENR_NOW 13
#define AVL_ANTI_ENGINE_CHECK_DIFFERENR_LABEL 14
#define AVL_ANTI_ENGINE_CHECK_RESULT_FAIL 15

/****病毒库升级检测状态值****/

#define AVL_ANTI_DB_CMD_RUN_SUCCESS 16
#define AVL_ANTI_DB_EXIST_NEWEST 17
#define AVL_ANTI_DB_NOW_NEWEST 18
#define AVL_ANTI_DB_UPGRADE_SUCCESS 19
#define AVL_ANTI_DB_UPGRADE_FAIL_REASON 20

#define AVL_INIT_STATUS 1
#define AVL_INIT_OVER   2
#define AVL_SCAN_STATUS 3
#define AVL_SCAN_OVER   4
#define AVL_CLOSE_STATUS 5
#define AVL_UPGADE_STATUS 6
#define AVL_UPGADE_END   7



int if_need_upgrade(char *newpath);
int diffHashValue(char *newpath, char * oldpath);
void  backup_old_lib(char *oldpath);
void write_antivirus_status(char *filename,char *szStatus);
void recover_avl_lib(char * oldpath);
