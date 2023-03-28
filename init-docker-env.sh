#!/bin/bash

if [[ $_ == $0 ]] ; then
    echo "Usage: source ../init-docker-env.sh"
    exit
fi

source .venv/bin/activate
pip install -r pip-requirements.txt --upgrade
stuart_setup -c .pytool/CISettings.py
