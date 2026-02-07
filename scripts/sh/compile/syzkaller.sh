#!/bin/bash
set -x

pushd $SYZ_PATH
make bin/syz-extract 
./bin/syz-extract -os=linux -builddir=/storage/PMIinDriFuzz/build/kernel/main/pci -sourcedir=/storage/PMIinDriFuzz/guest-kernel -arch=amd64 dev_dri.txt
make generate
make
popd
