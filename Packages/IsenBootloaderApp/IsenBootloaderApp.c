//
// Created by rfleuryleveso on 21/03/23.
//

#include "IsenBootloaderApp.h"
#include "util.h"
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/CpuLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include "elf/elf32.h"
#include "elf/elf64.h"

extern EFI_STATUS EFIAPI DxeDebugLibConstructor(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable);

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable) {
    EFI_STATUS Status = EFI_SUCCESS;


    EFI_CHECK(DxeDebugLibConstructor(ImageHandle, SystemTable));

    EFI_CHECK(gST->BootServices->SetWatchdogTimer(0, 0, 0, NULL));
    EFI_CHECK(gST->ConOut->ClearScreen(gST->ConOut));
    TRACE("Hello World!");


cleanup:
    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
    }

    return Status;
}