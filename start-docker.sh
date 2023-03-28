#!/bin/bash
docker run -i -v $PWD:/usr/workdir --workdir=/usr/workdir --name=edk2-env ghcr.io/tianocore/containers/fedora-37-dev