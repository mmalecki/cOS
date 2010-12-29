#!/bin/sh

./mkflop.sh
qemu -m 256 -fda floppy.img $*
