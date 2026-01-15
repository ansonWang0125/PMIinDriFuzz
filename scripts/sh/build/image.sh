#! /bin/bash

BUILD_IMAGE_DIR="${BUILD_PATH}/image"
CREATE_IMG="/storage/PMIinDriFuzz/fuzzer/go/src/github.com/google/syzkaller/tools/create-image.sh"

mkdir -p ${BUILD_IMAGE_DIR}

pushd ${BUILD_IMAGE_DIR}
cp $CREATE_IMG $BUILD_IMAGE_DIR
./create-image.sh
popd
