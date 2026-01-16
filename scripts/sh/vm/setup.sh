#! /bin/bash

# Execute this script in VM!

# TODO_L3: Find a way to execute command when kernel booting, VM_SETUP
apt-get -y install build-essential
apt-get install pciutils
apt-get install tree
mkdir -p /mnt/shared 
mount -t 9p -o trans=virtio,version=9p2000.L shared /mnt/shared