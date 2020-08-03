#ifndef	__AVLSDK30_S_INTERFACE_H__
#define	__AVLSDK30_S_INTERFACE_H__

#include "engine.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct	_rslt_info
{
	unsigned char	*desc;
	unsigned char	*malware_name;
	unsigned char	*ff_name;
	unsigned char	*shell_comp_name;
	unsigned char	*detector;
	long			malware_id;
	long			ff_id;
	long			shell_id;
	unsigned char	reserved[64];
} RSLT_INFO, *P_RSLT_INFO;

typedef struct	_AVLSDK3s_engine
{
	void	*engine;
	void	*nt_handle;
} AVLSDK3S_ENGINE, *P_AVLSDK3S_ENGINE;

long	AVL_SDKs_InitNewInstance(char *szCfgFile, void *pVerificationCode, const char *szWorkPath, P_AVLSDK3S_ENGINE pEngine);

long	AVL_SDKs_ReleaseInstance(P_AVLSDK3S_ENGINE pEngine);

long	AVL_SDKs_Scan(P_AVLSDK3S_ENGINE pEngine, P_OBJ_PROVIDER pObj, P_OBJ_DISPOSER pObjDisposer);

long	AVL_SDKs_SetConfigInt(P_AVLSDK3S_ENGINE pEngine, long CfgIdx, long lValue);

long	AVL_SDKs_SetConfigString(P_AVLSDK3S_ENGINE pEngine, long CfgIdx, const char *pValue);

long	AVL_SDKs_GetConfigInt(P_AVLSDK3S_ENGINE pEngine, long CfgIdx, long *pValue);

long	AVL_SDKs_GetConfigString(P_AVLSDK3S_ENGINE pEngine, long CfgIdx, char *Buf, long BufLen);

long	AVL_SDKs_QueryDBInfo(P_AVLSDK3S_ENGINE pEngine, P_DB_INFO pDBInfo);

#ifdef __cplusplus
}
#endif

#endif // __AVLSDK30_S_INTERFACE_H__


