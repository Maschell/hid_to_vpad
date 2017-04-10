#ifndef _MAIN_H_
#define _MAIN_H_

#include "common/types.h"
#include "dynamic_libs/os_functions.h"

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
void SplashScreen(s32 time,const char * message);
#ifdef __cplusplus
}
#endif

#endif
