#ifndef _MAIN_H_
#define _MAIN_H_

#include <gctypes.h>

/* Main */
#ifdef __cplusplus
extern "C" {
#endif

//! C wrapper for our C++ functions
s32 Menu_Main(void);
void deInit(void);
void ApplyPatches(void);
void RestorePatches(void);

s32 isInMiiMakerHBL();
bool SplashScreen(s32 time,const char * message,u8 pos, u32 button);
#ifdef __cplusplus
}
#endif

#endif
