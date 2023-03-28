#include "ISENLoader.h"

/**
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/

EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable)
{
  Print(L"ISEN Bootloader, loading OS 28_02_2023_12_53\n");

  EFI_STATUS Status;
  EFI_HANDLE *HandleBuffer = NULL;
  UINTN HandleCount = 0;

  Status = gBS->LocateHandleBuffer(ByProtocol,
                                   &gEfiSimpleFileSystemProtocolGuid,
                                   NULL,
                                   &HandleCount,
                                   &HandleBuffer);

  Print(L"HandleCount:    %x\n", HandleCount);
  Print(L"HandleBuffer:   %x\n", HandleBuffer);
  if (EFI_ERROR(Status))
  {
    Print(L"ERROR: LocateHandleBuffer.\n");
    Print(L"Status: %x\n", (int)Status);
    Print(L"HandleCount:    %x\n", HandleCount);
    Print(L"HandleBuffer:   %x\n", HandleBuffer);
    return Status;
  }

  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *efiSFSProtocol;
  EFI_DEVICE_PATH *Dp;

  for (UINT8 Index = 0; Index < HandleCount; Index++)
  {
    Status = gBS->HandleProtocol(HandleBuffer[Index],
                                 &gEfiSimpleFileSystemProtocolGuid,
                                 (VOID **)&efiSFSProtocol);
    if (EFI_ERROR(Status))
    {
      Print(L"ERROR: HandleProtocol.\n");
      Print(L"Status: %x\n", (int)Status);
      Print(L"HandleCount:    %x\n", HandleCount);
      Print(L"HandleBuffer:   %x\n", HandleBuffer);

      return Status;
    }

    Status = gBS->OpenProtocol(
        HandleBuffer[Index],
        &gEfiDevicePathProtocolGuid,
        (VOID **)&Dp,
        NULL,
        ImageHandle,
        EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if (EFI_ERROR(Status))
    {
      Print(L"ERROR: HandleProtocol.\n");
      Print(L"Status: %x\n", (int)Status);
      Print(L"HandleCount:    %x\n", HandleCount);
      Print(L"HandleBuffer:   %x\n", HandleBuffer);

      return Status;
    }

    EFI_FILE_HANDLE Root;

    Status = efiSFSProtocol->OpenVolume(efiSFSProtocol, &Root);
    if (EFI_ERROR(Status))
    {
      Print(L"ERROR: OpenVolume.\n");
      Print(L"Status: %x\n", (int)Status);
      Print(L"HandleCount:    %x\n", HandleCount);
      Print(L"HandleBuffer:   %x\n", HandleBuffer);

      return Status;
    }
    EFI_FILE_HANDLE OsFileBin;
    UINTN OsFileBinSize;
    VOID* OsBinPtr;

    Status = FindOSBin(Root, &OsFileBin, &OsBinPtr, &OsFileBinSize);
    if (EFI_ERROR(Status))
    {
      Print(L"ERROR: FindOSBin.\n");
      Print(L"Status: %x\n", (int)Status);
      Print(L"HandleCount:    %x\n", HandleCount);
      Print(L"HandleBuffer:   %x\n", HandleBuffer);

      return Status;
    }
    for (int i = 0; i < OsFileBinSize; i++)
    {
        Print(L"%02x  ", ((UINT8 *)OsBinPtr)[i]);
        if ((i > 0 && i % 8 == 0) || i == OsFileBinSize - 1)
        {
            Print(L"\n");
        }
    }

    Print(L"Read file successfuly 0x%x (%d bytes)\n", (UINTN)OsBinPtr, OsFileBinSize);
    Print(L"Entrypoint 0x%x\n", (UINTN)OsBinPtr + 0x13);
    Root->Close(OsFileBin);
    typedef int(__attribute__((sysv_abi)) * EntryPointFunction)(); // Placeholder names for jump
    EntryPointFunction EntryPointPlaceholder = (EntryPointFunction)(OsBinPtr + 0x13);
    // gBS->ExitBootServices(ImageHandle);
    int OSResult = EntryPointPlaceholder();
    Print(L"OS has returned %d\n", OSResult);
  }
  Print(L"ISEN Bootloader, loaded OS into memory\n");
  return EFI_SUCCESS;
}