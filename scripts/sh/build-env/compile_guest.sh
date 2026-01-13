#!/bin/bash

KERNEL="/storage/PMIinDriFuzz/guest-kernel"

pushd ${KERNEL}
git checkout main
popd

sudo apt-get -y install libncurses-dev \
                        gawk \
                        flex \
                        bison \
                        openssl \
                        libelf-dev \
                        libssl-dev
