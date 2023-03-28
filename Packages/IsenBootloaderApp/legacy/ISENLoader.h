#ifndef ISENLOADER_ISENLOADER
#define ISENLOADER_ISENLOADER


#include "ELFLoader.h"
#include "DiskUtils.h"
#include <Uefi.h>
#include <Guid/FileInfo.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/LoadFile.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include "elf.h"


EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable);

#endif