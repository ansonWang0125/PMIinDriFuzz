#!/bin/bash
set -x

SOURCE_DIR=$PROJ_PATH/qemu
BUID_DIR=$BUILD_PATH/qemu

sudo apt install -y libcap-dev \
                    libattr1-dev \
                    libglib2.0-dev \
                    libpixman-1-dev
mkdir -p ${BUID_DIR}

pushd ${SOURCE_DIR}
git checkout main
popd

pushd ${BUID_DIR}
$SOURCE_DIR/configure --prefix=${BUID_DIR}/install \
                          --target-list=x86_64-softmmu \
                          --enable-debug \
                          --enable-virtfs
popd