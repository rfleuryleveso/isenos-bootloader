#ifndef ISENLOADERPKG_DISKUTILS
#define ISENLOADERPKG_DISKUTILS

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


EFI_STATUS
EFIAPI
FindOSBin(
        IN EFI_FILE_HANDLE Dir,
        OUT EFI_FILE_HANDLE *OsBinFile,
        OUT VOID **OsBinPtr,
        OUT UINTN *OsBinFileSize);

#endif