#!/bin/sh
rm OpenRPGMaker
qmake-qt4
make $*
notify-send "Open RPG Maker finished compiling"