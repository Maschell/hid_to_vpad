#include <string.h>
#include "common/common.h"
#include "common/kernel_defs.h"
#include "kernel/kernel_functions.h"
#include "kernel/syscalls.h"

/* our retain data */
ReducedCosAppXmlInfo cosAppXmlInfoStruct __attribute__((section(".data")));
/*
 *  This function is a kernel hook function. It is called directly from kernel code at position 0xFFF18558.
 */
void my_PrepareTitle(CosAppXmlInfo *xmlKernelInfo)
{
    /**
     *  DBAT for access to our data region is setup at this point for the 0xC0000000 area.
     */

    //! Copy all data from the XML info
    strncpy(cosAppXmlInfoStruct.rpx_name, xmlKernelInfo->rpx_name, FS_MAX_ENTNAME_SIZE);

    cosAppXmlInfoStruct.version_cos_xml = xmlKernelInfo->version_cos_xml;
    cosAppXmlInfoStruct.os_version = xmlKernelInfo->os_version;
    cosAppXmlInfoStruct.title_id = xmlKernelInfo->title_id;
    cosAppXmlInfoStruct.app_type = xmlKernelInfo->app_type;
    cosAppXmlInfoStruct.cmdFlags = xmlKernelInfo->cmdFlags;
    cosAppXmlInfoStruct.max_size = xmlKernelInfo->max_size;
    cosAppXmlInfoStruct.avail_size = xmlKernelInfo->avail_size;
    cosAppXmlInfoStruct.codegen_size = xmlKernelInfo->codegen_size;
    cosAppXmlInfoStruct.codegen_core = xmlKernelInfo->codegen_core;
    cosAppXmlInfoStruct.max_codesize = xmlKernelInfo->max_codesize;
    cosAppXmlInfoStruct.overlay_arena = xmlKernelInfo->overlay_arena;
    cosAppXmlInfoStruct.default_stack0_size = xmlKernelInfo->default_stack0_size;
    cosAppXmlInfoStruct.default_stack1_size = xmlKernelInfo->default_stack1_size;
    cosAppXmlInfoStruct.default_stack2_size = xmlKernelInfo->default_stack2_size;
    cosAppXmlInfoStruct.default_redzone0_size = xmlKernelInfo->default_redzone0_size;
    cosAppXmlInfoStruct.default_redzone1_size = xmlKernelInfo->default_redzone1_size;
    cosAppXmlInfoStruct.default_redzone2_size = xmlKernelInfo->default_redzone2_size;
    cosAppXmlInfoStruct.exception_stack0_size = xmlKernelInfo->exception_stack0_size;
    cosAppXmlInfoStruct.exception_stack1_size = xmlKernelInfo->exception_stack1_size;
    cosAppXmlInfoStruct.exception_stack2_size = xmlKernelInfo->exception_stack2_size;
    cosAppXmlInfoStruct.sdk_version = xmlKernelInfo->sdk_version;
    cosAppXmlInfoStruct.title_version = xmlKernelInfo->title_version;
}

void SetupKernelCallback(void)
{
    KernelSetupSyscalls();
}

