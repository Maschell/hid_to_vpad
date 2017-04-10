#ifndef __FS_UTILS_H_
#define __FS_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <gctypes.h>

s32 MountFS(void *pClient, void *pCmd, char **mount_path);
s32 UmountFS(void *pClient, void *pCmd, const char *mountPath);

s32 LoadFileToMem(const char *filepath, u8 **inbuffer, u32 *size);

//! todo: C++ class
s32 CreateSubfolder(const char * fullpath);
s32 CheckFile(const char * filepath);

#ifdef __cplusplus
}
#endif

#endif // __FS_UTILS_H_
