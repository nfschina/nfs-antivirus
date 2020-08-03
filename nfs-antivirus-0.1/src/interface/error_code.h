#ifndef ERROR_CODE_H
#define ERROR_CODE_H

// 成功
#define ERR_SUCCESS                    0        // 

// 严重异常
#define ERR_UNKNOWN                    -1
#define ERR_INCORRECT_PARAM            -2
#define ERR_NO_ENOUGH_MEM              -3
#define	ERR_BAD_LICENSE                -4
#define	ERR_BAD_AUTH_CODE              -5
#define	ERR_BAD_ENGINE_VER             -6
#define	ERR_BAD_DATABASE               -7
#define	ERR_INIT_COMPONENT_FAILED      -8
#define	ERR_MISS_IMPORTANT_MOD_FILE    -9
#define ERR_MISS_IMPORTANT_DATA_FILE   -10
#define ERR_BAD_WORKING_PATH           -11
#define ERR_LOAD_CONFIG                -12
#define	ERR_SET_CONFIG                 -13
#define	ERR_GET_CONFIG                 -14
#define	ERR_SCAN_FAILED                -15

// 警告类
#define ERR_MISS_MOD_FILE              1
#define	ERR_USER_ABORT                 2 // 用户放弃处理
#define	ERR_RPT_NOT_EXIST              3
#define ERR_NOT_ENOUGH_BUF             4

#endif
