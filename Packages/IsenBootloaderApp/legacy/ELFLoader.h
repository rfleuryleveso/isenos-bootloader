#ifndef ISENLOADERPKG_ELFLOADER
#define ISENLOADERPKG_ELFLOADER


#define OS_AND_KERNEL_MEMORY_TYPE 0x80000001
#include <Uefi.h>
#include <Guid/FileInfo.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include "DiskUtils.h"
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/LoadFile.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include "elf.h"

EFI_STATUS
EFIAPI
LoadElf(
    IN EFI_FILE_HANDLE* OsFileHandle,
    IN VOID *HeaderPtr,
        IN VOID **EntryPointPtr);

#endif