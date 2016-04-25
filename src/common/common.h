#ifndef COMMON_H
#define	COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "os_defs.h"

#define CAFE_OS_SD_PATH             "/vol/external01"
#define SD_PATH                     "sd:"
#define WIIU_PATH                   "/wiiu"

/* Macros for libs */
#define LIB_CORE_INIT           0
#define LIB_NSYSNET             1
#define LIB_GX2                 2
#define LIB_AOC                 3
#define LIB_AX                  4
#define LIB_FS                  5
#define LIB_OS                  6
#define LIB_PADSCORE            7
#define LIB_SOCKET              8
#define LIB_SYS                 9
#define LIB_VPAD                10
#define LIB_NN_ACP              11
#define LIB_SYSHID              12
#define LIB_VPADBASE            13

// functions types
#define STATIC_FUNCTION         0
#define DYNAMIC_FUNCTION        1

// none dynamic libs
#define LIB_LOADER              0x1001

#ifndef MEM_BASE
#define MEM_BASE                    (0x00800000)
#endif

#define ELF_DATA_ADDR               (*(volatile unsigned int*)(MEM_BASE + 0x1300 + 0x00))
#define ELF_DATA_SIZE               (*(volatile unsigned int*)(MEM_BASE + 0x1300 + 0x04))
#define MAIN_ENTRY_ADDR             (*(volatile unsigned int*)(MEM_BASE + 0x1400 + 0x00))
#define OS_FIRMWARE                 (*(volatile unsigned int*)(MEM_BASE + 0x1400 + 0x04))

#define OS_SPECIFICS                ((OsSpecifics*)(MEM_BASE + 0x1500))

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS                0
#endif
#define EXIT_HBL_EXIT               0xFFFFFFFE
#define EXIT_RELAUNCH_ON_LOAD       0xFFFFFFFD

#define RESTORE_INSTR_MAGIC         0xC001C0DE
#define RESTORE_INSTR_ADDR          ((restore_instructions_t*)(MEM_BASE + 0x1600))

typedef struct _restore_instructions_t {
    unsigned int magic;
    unsigned int instr_count;
    struct {
        unsigned int addr;
        unsigned int instr;
    } data[0];
} restore_instructions_t;

#ifdef __cplusplus
}
#endif

#endif	/* COMMON_H */

