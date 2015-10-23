#!/bin/sh
if [ -z "$HOME" ]; then
    echo "error: need to set HOME environment variable to the path of your home directory (ie. /home/username)"
    exit 1
fi
export LD_LIBRARY_PATH=./fmodex/lib
./OpenRPGMaker