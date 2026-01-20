#! /bin/bash

# Use this script to copy necessary file to guest VM for developing.

sudo mkdir -p /mnt/stretch
sudo mount /storage/PMIinDriFuzz/build/test-image/stretch.img /mnt/stretch
sudo cp /storage/PMIinDriFuzz/scripts/sh/vm/rc.local /mnt/stretch/etc/rc.local
sudo umount /mnt/stretch
