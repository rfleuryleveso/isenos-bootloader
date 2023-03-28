#!/bin/bash
uefi-run -b /usr/share/ovmf/OVMF.fd -f kernel.elf -d edk2/Build/MdeModule/DEBUG_GCC5/X64/IsenBootloaderApp/IsenBootloaderApp/OUTPUT/IsenBootloaderApp.efi -- -s -m 4096