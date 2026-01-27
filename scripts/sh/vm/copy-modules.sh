#! /bin/bash

if [ $# -ne 2 ]; then
	echo "Usage: $0 <MOD_DIR> <IMG_NAME>" >&2
	exit 1
fi

MOD_DIR=$1
IMG_NAME=$2

if [ ! -f $IMG_NAME ]; then
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
sudo mount -o loop $IMG_NAME $MNT_DIR
sudo mkdir -p $MNT_DIR/lib/modules/
sudo cp -rd $MOD_DIR/lib/modules/5.5.0-rc3+/ $MNT_DIR/lib/modules/5.5.0-rc3+/
sudo umount $MNT_DIR
