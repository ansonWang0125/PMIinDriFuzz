#!/bin/bash

KERNEL="/storage/PMIinDriFuzz/host-kernel"

pushd ${KERNEL}
git checkout v4.18.20
popd

sudo apt-get -y install libncurses-dev \
                        gawk \
                        flex \
                        bison \
                        openssl \
                        libelf-dev \
                        libssl-dev
