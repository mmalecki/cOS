#!/bin/sh

./mkflop.sh
~/Programowanie/Sources/bochs-2.4.5/bochs -q -f bochsrc-debug.txt
