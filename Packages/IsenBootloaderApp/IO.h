#ifndef __IO_H__
#define __IO_H__

#include <Uefi.h>
#include <Protocol/SimpleFileSystem.h>

EFI_STATUS FileRead(EFI_FILE_HANDLE Handle, void* Buffer, UINTN Size, UINTN Offset);

#endif //__IO_H__