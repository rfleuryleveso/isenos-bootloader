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
  Print(L"ISEN Bootloader, loading OS 28_02_2023_11_08\n");

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
  }
  Print(L"ISEN Bootloader, gEfiSimpleFileSystemProtocol loaded\n");
  return EFI_SUCCESS;
}
