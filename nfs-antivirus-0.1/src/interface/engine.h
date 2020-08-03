#ifndef __AVLSDK_INTERFACE_H__
#define __AVLSDK_INTERFACE_H__

#ifdef __cplusplus
extern "C"{
#endif

// Macros about continue or abort
#define	OD_CONTINUE							(1)						// 
#define	OD_ABORT							(2)						// 

#define	CUR_ENGINE_VER						(0x03000202)
#define CUR_ENGINE_VER_STR                  ("3.0.2.2")

// Data type

// Data environment type
#define	ET_NETWORK							(1)
#define	ET_DESKTOP							(2)

// Sub module names
#define SMN_FILE_FMT                        ("AIFilFmt")
#define SMN_SHELL_RECOGNIZE                 ("AIPack")
#define SMN_SFX_DETECT                      ("AISfxArc")
#define SMN_INFECTED                        ("ASVirus")
#define SMN_BOL_DETECT                      ("ASBOL")
#define SMN_COMMON_DETECT                   ("ASCommon")
#define SMN_MALWS_DETECT                    ("ASMalwS")
#define SMN_MALWE_DETECT                    ("ASMalwE")
#define SMN_MALWFH_DETECT                   ("ASMalwFH")
#define SMN_MALWHS_DETECT                   ("ASMalwHS")
#define SMN_KEXPLOIT_DETECT                 ("ASKExplt")
#define SMN_EXPLOIT_DETECT                  ("ASExplot")
#define SMN_SCRIPT_DETECT                   ("ASScript")
#define SMN_MALWB_DETECT                    ("ASMalwB")
#define SMN_VCS2_STATIC                     ("ASVCS2S")
#define SMN_SPLIT_SCRIPT                    ("APSScrpt")
#define SMN_UNIT_EOP                        ("APUnitEP")
#define SMN_SPLIT_PE                        ("APSPE")
#define SMN_ARCHIVE                         ("APUnArc")
#define SMN_UNPACK_STATIC                   ("APUnPack")
#define SMN_SPLIT_EML                       ("APSMail")
#define SMN_SUF                             ("AWSuf")
#define SMN_SUF_DETECT                      ("ASSuf")
#define SMN_CLOUD_DETECT                    ("ASCloud")

typedef struct
{
	unsigned long   obj_ver;       // version of this structure
	unsigned long   obj_type;      // data type
	unsigned long   evro_type;     // environment type
	unsigned char   *buf;          // pointer of data buffer
	unsigned long   size;          // size of valid data
	unsigned char   obj_des[4096]; // data description
} OBJ_PROVIDER, *P_OBJ_PROVIDER;

/*************************************************************************************************\
 * Function : Engine will call this callback function when it finishes scanning a object
 * Param    : p_op              the object provider pointer
 *          : p_rpt_handle      the handle of result report
 *            p_param           transfered to engine by OBJ_DISPOSER::p_rpt_param
 *
 * Return   : Undefined
 * Note     : This callback function will be called only if engine reports the scanning result
\*************************************************************************************************/
typedef	long	(*P_OBJ_DISPOSER_CALLBACK)(P_OBJ_PROVIDER p_op, void *p_rpt_handle, void *p_param);

/*************************************************************************************************\
 * Function : Engine will call this callback function when it needs to know continue or not
 * Param    : p_param           transfered to engine by OBJ_DISPOSER::p_rpt_param
 *
 * Return   : OD_CONTINUE or OD_ABORT
\*************************************************************************************************/
typedef	long	(*P_QUERY_CONTINUE_CALLBACK)(void *p_param);

typedef struct
{
	P_OBJ_DISPOSER_CALLBACK   rpt_callback;
	void                      *p_rpt_param;
	P_QUERY_CONTINUE_CALLBACK query_continue_callback;
	void                      *p_qc_param;
} OBJ_DISPOSER, *P_OBJ_DISPOSER;

typedef struct
{
	unsigned long	total_db_item_count;
	char			db_time_stamp[32];
} DB_INFO, *P_DB_INFO;

/*************************************************************************************************\
 * Function : Create a new instance of engine
 * Param    : ppEngine          the engine handle returned
 *
 * Return   : Error code
\*************************************************************************************************/
long	AVL_SDK_CreateInstance(void **ppEngine);

/*************************************************************************************************\
 * Function : Release handle of engine instance
 * Param    : pEngine           the engine handle
 *
 * Return   : Error code
\*************************************************************************************************/
long	AVL_SDK_Release(void *pEngine);

/*************************************************************************************************\
 * Function : Load a template file of configuration
 * Param    : pEngine           the engine handle
 *            szFilename        the full path of configuration file
 *
 * Return   : Error code
\*************************************************************************************************/
long	AVL_SDK_LoadConfigFile(void *pEngine, char *szFilename);

/*************************************************************************************************\
 * Function : Set an int value to engine
 * Param    : pEngine           the engine handle
 *            pCfgIdx           index of configuration
 *            lValue            the value to be set
 *
 * Return   : Error code
\*************************************************************************************************/
long	AVL_SDK_SetConfigInt(void *pEngine, long CfgIdx, long lValue);

/*************************************************************************************************\
 * Function : Set an string value to engine
 * Param    : pEngine           the engine handle
 *            pCfgIdx           index of configuration
 *            pValue            the value to be set
 *
 * Return   : Error code
\*************************************************************************************************/
long	AVL_SDK_SetConfigString(void *pEngine, long CfgIdx, const char *pValue);

/*************************************************************************************************\
 * Function : Get an int value from engine
 * Param    : pEngine           the engine handle
 *            pCfgIdx           index of configuration
 *            pValue            the buffer to store the value
 *
 * Return   : Error code
\*************************************************************************************************/
long	AVL_SDK_GetConfigInt(void *pEngine, long CfgIdx, long *pValue);

/*************************************************************************************************\
 * Function : Get an string value from engine
 * Param    : pEngine           the engine handle
 *            pCfgIdx           index of configuration
 *            Buf               the buffer to store the value
 *            BufLen            the length of the buffer
 *
 * Return   : Error code
\*************************************************************************************************/
long	AVL_SDK_GetConfigString(void *pEngine, long CfgIdx, char *Buf, long BufLen);

/*************************************************************************************************\
 * Function : Initialize the engine handle based on the configuration
 * Param    : pEngine           the engine handle
 *            pVerificationCode verification code
 *
 * Return   : Error code
 * Note     : If initialization failes, engine will release this instance automatically
\*************************************************************************************************/
long	AVL_SDK_InitInstance(void *pEngine, const void* pVerificationCode);

/*************************************************************************************************\
 * Function : Scan data
 * Param    : pEngine           the engine handle
 *            pObj              the pointer of OBJ_PROVIDER structure
 *            pObjDisposer      the pointer of OBJ_DISPOSER structure
 *
 * Return   : Error code
\*************************************************************************************************/
long	AVL_SDK_Scan(void *pEngine, P_OBJ_PROVIDER pObj, P_OBJ_DISPOSER pObjDisposer);

/*************************************************************************************************\
 * Function : Query the result report
 * Param    : pEngine           the engine handle
 *            pRptHandle        the handle of report
 *            key               the index of report
 *            value             the buffer to get report
 *
 * Return   : Error code
\*************************************************************************************************/
long	AVL_SDK_QueryReportInt(void *pEngine, void *pRptHandle, unsigned long key, long *value);

/*************************************************************************************************\
 * Function : Query the result report
 * Param    : pEngine           the engine handle
 *            pRptHandle        the handle of report
 *            key               the index of report
 *            value             the pointer to string report
 *
 * Return   : Error code
\*************************************************************************************************/
long	AVL_SDK_QueryReportStr(void *pEngine, void *pRptHandle, unsigned long key, unsigned char **value);

/*************************************************************************************************\
 * Function : Query the library info
 * Param    : pEngine           the engine handle
 *            pDBInfo           the pointer of DB_INFO structure
 *
 * Return   : Error code
\*************************************************************************************************/
long	AVL_SDK_QueryDBInfo(void *pEngine, P_DB_INFO pDBInfo);

#ifdef __cplusplus
}
#endif

#endif // __AVLSDK_INTERFACE_H__


