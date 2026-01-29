#! /bin/bash

if [ $# -ne 2 ]; then
	echo "Usage: $0 <MOD_DIR> <IMG_NAME>" >&2
	exit 1
fi

MOD_DIR=$1
IMG_NAME=$2
IMG_PATH=/storage/PMIinDriFuzz/build/prod-image

if [ ! -f "${IMG_PATH}/${IMG_NAME}.img" ]; then
	echo "$IMG_NAME does not exist." >&2
	exit 1
fi

if [ ! -d $MOD_DIR ]; then
	echo "$MOD_DIR does not exist." >&2
	exit 1
fi

set -eux

MNT_DIR=/mnt/${IMG_NAME%.*}

echo "MOUNT_DIR $MNT_DIR"
sudo mkdir -p $MNT_DIR
sudo mount -o loop "${IMG_PATH}/${IMG_NAME}.img" $MNT_DIR
sudo rm -rf $MOD_DIR/lib/modules/5.5.0-rc3+/
sudo mkdir -p $MOD_DIR/lib/modules/5.5.0-rc3+/drivers/uio/
sudo cp /storage/PMIinDriFuzz/scripts/c/module/ivshmem_pci/ivshmem_pci.ko $MOD_DIR/lib/modules/5.5.0-rc3+/drivers/uio/ivshmem_pci.ko
sudo mkdir -p $MNT_DIR/lib/modules/
sudo cp -r $MOD_DIR/lib/modules/5.5.0-rc3+/ $MNT_DIR/lib/modules/5.5.0-rc3+/
sudo cp /storage/PMIinDriFuzz/scripts/sh/vm/rc.local $MNT_DIR/etc/rc.local
sudo umount $MNT_DIR
