#include "ELFLoader.h"

EFI_STATUS
EFIAPI
LoadElf(
    IN EFI_FILE_HANDLE *OsFileHandle,
    IN VOID *HeaderPtr,
    IN VOID **EntryPointPtr)
{
    EFI_STATUS Status;

    ElfMinHdr *min_header = (ElfMinHdr *)HeaderPtr;
    Print(L"ELF Header: \n");
    for (int i = 0; i < EI_NIDENT; i++)
    {
        Print(L"%d > %x\n", i, min_header->e_ident[i]);
    }

    if (
        !(min_header->e_ident[0] == 0x7F &&
          min_header->e_ident[1] == 0x45 &&
          min_header->e_ident[2] == 0x4C &&
          min_header->e_ident[3] == 0x46))
    {
        Print(L"Wrong ELF Header magic bytes\n");
        return EFI_UNSUPPORTED;
    }

    if (
        !(min_header->e_ident[4] == 0x2))
    {
        Print(L"Only x64 files are supported\n");
        return EFI_UNSUPPORTED;
    }

    Print(L"Reading the full ELF64 header \n");
    Elf64_Ehdr *ElfHeader = AllocatePool(sizeof(Elf64_Ehdr));
    UINTN ElfHeaderSize = sizeof(Elf64_Ehdr);

    (*OsFileHandle)->SetPosition(*OsFileHandle, 0);
    Status = (*OsFileHandle)->Read(*OsFileHandle, &ElfHeaderSize, (VOID *)ElfHeader);
    if (EFI_ERROR(Status))
    {
        Print(L"ERROR: File->Read.\n");
        Print(L"Status: %x\n", (int)Status);
        return Status;
    }
    if (ElfHeaderSize != sizeof(Elf64_Ehdr))
    {
        Print(L"ERROR: Wrong Read.\n");
        Print(L"Read %d instead of %d\n", ElfHeaderSize, sizeof(Elf64_Ehdr));
        return EFI_UNSUPPORTED;
    }

    for (int i = 0; i < ElfHeaderSize; i++)
    {
        Print(L"%02x  ", ((UINT8 *)ElfHeader)[i]);
        if ((i > 0 && i % 8 == 0) || i == ElfHeaderSize - 1)
        {
            Print(L"\n");
        }
    }

    Print(L"EFL: %d program headers (0x%x) and %d sections (0x%x) \n", ElfHeader->e_phnum, ElfHeader->e_phoff, ElfHeader->e_shnum, ElfHeader->e_shoff);
    for (int i = 0; i < ElfHeader->e_phnum; ++i)
    {
        UINTN ProgramHeaderSize = sizeof(Elf64_Phdr);
        Elf64_Phdr ProgramHeader;
        (*OsFileHandle)->SetPosition(*OsFileHandle, ElfHeader->e_phoff + ElfHeader->e_phentsize * i);
        Status = (*OsFileHandle)->Read(*OsFileHandle, &ProgramHeaderSize, (VOID *)&ProgramHeader);
        if (EFI_ERROR(Status))
        {
            Print(L"ERROR: OsFileHandle->Read.\n");
            Print(L"Status: %x\n", (int)Status);
            return Status;
        }
        Print(L"Program: %d, type: %x, offset: %x, virtual address: %x, size %d \n", i, ProgramHeader.p_type, ProgramHeader.p_offset, ProgramHeader.p_vaddr, ProgramHeader.p_filesz);
        if(ProgramHeader.p_type != 1) {
          continue;
        }
        // get the type and pages to allocate
        UINTN nPages = EFI_SIZE_TO_PAGES(ALIGN_VALUE(ProgramHeader.p_memsz, EFI_PAGE_SIZE));

        // allocate the address
        EFI_PHYSICAL_ADDRESS base = ProgramHeader.p_paddr;
        Print(L"Allocating Program %d to %x (%d pages)\n", i, base, nPages);
        Status = gBS->AllocatePages(AllocateAddress, EfiLoaderData, nPages, &base);

        if (EFI_ERROR(Status))
        {
            Print(L"ERROR: gBS->AllocatePages.\n");
            Print(L"Status: %x\n", (int)Status);
            return Status;
        }
        (*OsFileHandle)->SetPosition(*OsFileHandle, ProgramHeader.p_offset);
        Status = (*OsFileHandle)->Read(*OsFileHandle, (UINTN *)&ProgramHeader.p_filesz, (VOID *)base);
        if (EFI_ERROR(Status))
        {
            Print(L"ERROR: OsFileHandle->Read.\n");
            Print(L"Status: %x\n", (int)Status);
            return Status;
        }
        Print(L"Allocating Program %d to Virtual 0x%x\n", i, base);

        // ZeroMem((void *)(base + phdr.p_filesz), phdr.p_memsz - phdr.p_filesz);
    }
    for (int i = 0; i < ElfHeader->e_shnum; ++i)
    {
        UINTN SectionHeaderSize = sizeof(Elf64_Shdr);
        Elf64_Shdr SectionHeader;
        (*OsFileHandle)->SetPosition(*OsFileHandle, ElfHeader->e_shoff + ElfHeader->e_shentsize * i);
        Status = (*OsFileHandle)->Read(*OsFileHandle, &SectionHeaderSize, (VOID *)&SectionHeader);
        if (EFI_ERROR(Status))
        {
            Print(L"ERROR: OsFileHandle->Read.\n");
            Print(L"Status: %x\n", (int)Status);
            return Status;
        }
        Print(L"Section %d, type %x \n", i, SectionHeader.sh_type);
    }
    Print(L"EntryPoint: %x.\n", ElfHeader->e_entry);
    *EntryPointPtr = (VOID*)ElfHeader->e_entry;
    return EFI_SUCCESS;
}