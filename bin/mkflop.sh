#!/bin/sh

sudo mkdir /mnt/floppy/
sudo losetup /dev/loop1 floppy.img
sudo mount -o loop /dev/loop1 /mnt/floppy
sudo cp -f cOS /mnt/floppy
sudo umount /mnt/floppy
sudo losetup -d /dev/loop1
sudo rm -rf /mnt/floppy
