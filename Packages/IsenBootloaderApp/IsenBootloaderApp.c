//
// Created by rfleuryleveso on 21/03/23.
//

#include "IsenBootloaderApp.h"
#include "Utils.h"
#include "IO.h"
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/CpuLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/FileHandleLib.h>
#include <Protocol/LoadedImage.h>
#include "elf/elf32.h"
#include "elf/elf64.h"
#include "ELFLoader.h"

extern EFI_STATUS EFIAPI DxeDebugLibConstructor(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable);

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable) {
    EFI_STATUS Status = EFI_SUCCESS;


    EFI_CHECK(DxeDebugLibConstructor(ImageHandle, SystemTable));

    EFI_CHECK(gST->BootServices->SetWatchdogTimer(0, 0, 0, NULL));
    EFI_CHECK(gST->ConOut->ClearScreen(gST->ConOut));

    EFI_LOADED_IMAGE_PROTOCOL* LoadedImage = NULL;
    EFI_DEVICE_PATH_PROTOCOL* BootDevicePath = NULL;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* BootFs = NULL;
    EFI_HANDLE FsHandle = NULL;

    // get the boot image device path
    EFI_CHECK(gBS->HandleProtocol(gImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&LoadedImage));
    EFI_CHECK(gBS->HandleProtocol(LoadedImage->DeviceHandle, &gEfiDevicePathProtocolGuid, (void**)&BootDevicePath));

    // locate the file system
    EFI_CHECK(gBS->LocateDevicePath(&gEfiSimpleFileSystemProtocolGuid, &BootDevicePath, &FsHandle));
    EFI_CHECK(gBS->HandleProtocol(FsHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&BootFs));

    TRACE("Hello World!");
    ELF_INFO elfInfo = {0};
    elfInfo.VirtualOffset = 0xffffffff80000000;
    LoadElf64(BootFs, L"kernel.elf", &elfInfo);
    TRACE("    ENTRY = %p, PHYSICAL_BASE = %p", elfInfo.Entry, elfInfo.PhysicalBase);

    typedef int kernel_entry(void);
    kernel_entry* entry_point = (kernel_entry*)(elfInfo.PhysicalBase + elfInfo.Entry);
    int out = entry_point();
    TRACE("    KERNEL RETURNED %d", out);
cleanup:
    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
    }

    return Status;
}