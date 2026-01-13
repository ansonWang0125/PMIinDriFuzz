#!/bin/bash

if [ -f ${SYZ_PATH} ]; then
    exit 0
fi

go get -u -d github.com/google/syzkaller

rm ${SYZ_PATH}

pushd ${PROJ_PATH}
git submodule add git@github.com:ansonWang0125/PMIinDriFuzz-syzkaller.git $SYZ_PATH
popd