#!/bin/bash

BUID_DIR=$BUILD_PATH/qemu

pushd ${BUID_DIR}
make -j8 install
popd