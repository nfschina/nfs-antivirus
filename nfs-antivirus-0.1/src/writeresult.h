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
#define  QUIT_SCAN_LOG_TYPE   1
void writelog(int logType);
void write_defence_log(char *defence_log);
void delete_log_30daysago(char* log_file);
