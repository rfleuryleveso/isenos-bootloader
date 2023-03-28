#!/bin/bash
ln -s /usr/workdir/Packages/IsenBootloaderApp /usr/workdir/edk2/IsenBootloaderApp
stuart_update -c .pytool/CISettings.py
stuart_ci_build -c .pytool/CISettings.py -p MdeModulePkg -t DEBUG -a X64 -v