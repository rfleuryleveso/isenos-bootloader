#include "DiskUtils.h"

#define MAX_FILE_INFO_SIZE 1024

EFI_STATUS
EFIAPI
FindOSBin(
    IN EFI_FILE_HANDLE Dir,
    OUT EFI_FILE_HANDLE *OsBinFile,
    OUT VOID **OsBinPtr,
    OUT UINTN *OsBinFileSize)
{
  EFI_STATUS Status;
  EFI_FILE_INFO *FileInfo;
  CHAR16 *FileName;
  UINTN FileInfoSize;

  // big enough to hold EFI_FILE_INFO struct and
  // the whole file path
  FileInfo = AllocatePool(MAX_FILE_INFO_SIZE);
  if (FileInfo == NULL)
  {
    return EFI_OUT_OF_RESOURCES;
  }

  for (;;)
  {
    // get the next file's info. there's an internal position
    // that gets incremented when you read from a directory
    // so that subsequent reads gets the next file's info
    FileInfoSize = MAX_FILE_INFO_SIZE;
    Status = Dir->Read(Dir, &FileInfoSize, (VOID *)FileInfo);
    if (EFI_ERROR(Status) || FileInfoSize == 0)
    {
      FreePool(FileInfo);
      return Status;
    }

    FileName = FileInfo->FileName;
    Print(L"Reading file %s \n", FileName);
    // skip files named . or ..
    if (StrCmp(FileName, L"kernel.bin") != 0)
    {
      continue;
    }
    Print(L"Found OS file");
     Print(L"Found kernel.bin (%d bytes) %s \n", FileInfo->FileSize, FileName);
    // read the file into a buffer
    Status = Dir->Open(Dir, OsBinFile, FileName, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(Status))
    {
      FreePool(FileInfo);
      return Status;
    }
    // reset position just in case
     (*OsBinFile)->SetPosition(*OsBinFile, 0);
    void *BinFile = AllocatePool(FileInfo->FileSize);
    if (BinFile == NULL)
    {
      Print(L"AllocatePool failed for file \n");
      FreePool(FileInfo);
      return EFI_OUT_OF_RESOURCES;
    }

     UINTN FileSize = FileInfo->FileSize,
          Status = (*OsBinFile)->Read(*OsBinFile, &FileSize, BinFile);
    if (EFI_ERROR(Status))
    {
      FreePool(FileInfo);
      Print(L"ERROR: File->Read.\n");
      Print(L"Status: %x\n", (int)Status);
      return Status;
    }
    Print(L"Read file successfuly (%d bytes)\n", FileSize);
    *OsBinFileSize = FileSize;
    *OsBinPtr = BinFile;
    Dir->Close(*OsBinFile);

    FreePool(FileInfo);

    return EFI_SUCCESS;
  }
}
