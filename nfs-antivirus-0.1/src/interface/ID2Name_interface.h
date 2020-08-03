#ifndef	__ID_NAME_INTERFACE_H__
#define	__ID_NAME_INTERFACE_H__

long AVL_NTranser_Init(const char *path, void **handle);

long AVL_NTranser_QueryNameByID(void *handle, const char *mod_name, long id, unsigned char *buf, unsigned long size);

void AVL_NTranser_Release(void *handle);

#endif // __ID_NAME_INTERFACE_H__


