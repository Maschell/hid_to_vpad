#include <string.h>
#include "common/common.h"
#include "common/kernel_defs.h"
#include "kernel/kernel_functions.h"
#include "kernel/syscalls.h"

void SetupKernelCallback(void)
{
    KernelSetupSyscalls();
}

