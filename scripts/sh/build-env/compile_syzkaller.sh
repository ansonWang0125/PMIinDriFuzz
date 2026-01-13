#!/bin/bash
set -x

sudo apt install -y cmake \
                    golang-go \
                    binutils-dev

pushd ${SYZ_PATH}
git checkout main
popd