#!/bin/bash
uefi-run -b /usr/share/ovmf/OVMF.fd -f kernel.bin -d edk2/Build/MdeModule/DEBUG_GCC5/X64/IsenBootloaderApp/IsenBootloaderApp/OUTPUT/IsenBootloaderApp.efi -- --serial tcp::6666,server -s