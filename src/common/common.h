#ifndef COMMON_H
#define	COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <dynamic_libs/os_defs.h>

#define CAFE_OS_SD_PATH             "/vol/external01"
#define SD_PATH                     "sd:"
#define WIIU_PATH                   "/wiiu"

// none dynamic libs
#define LIB_LOADER              0x1001

#ifndef MEM_BASE
#define MEM_BASE                    (0x00800000)
#endif

#define ELF_DATA_ADDR               (*(volatile u32*)(MEM_BASE + 0x1300 + 0x00))
#define ELF_DATA_SIZE               (*(volatile u32*)(MEM_BASE + 0x1300 + 0x04))
#define MAIN_ENTRY_ADDR             (*(volatile u32*)(MEM_BASE + 0x1400 + 0x00))
#define OS_FIRMWARE                 (*(volatile u32*)(MEM_BASE + 0x1400 + 0x04))

#define OS_SPECIFICS                ((OsSpecifics*)(MEM_BASE + 0x1500))

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS                0
#endif
#define EXIT_HBL_EXIT               0xFFFFFFFE
#define EXIT_RELAUNCH_ON_LOAD       0xFFFFFFFD

#define DEFAULT_HID_TO_VPAD_PATH    SD_PATH WIIU_PATH "/apps/hidtovpad"
#define DEFAULT_LANG_PATH           DEFAULT_HID_TO_VPAD_PATH "/languages"
#define LANGUAGE_FILE_EXT           ".lang"

#ifdef __cplusplus
}
#endif

#endif	/* COMMON_H */

