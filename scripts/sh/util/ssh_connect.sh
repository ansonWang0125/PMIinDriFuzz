#! /bin/bash

scp -P 1569 \
    -F /dev/null \
    -o UserKnownHostsFile=/dev/null \
    -o BatchMode=yes \
    -o IdentitiesOnly=yes \
    -o StrictHostKeyChecking=no \
    -o ConnectTimeout=10 \
    -i /storage/PMIinDriFuzz/build/image/stretch.id_rsa \
    -v /storage/PMIinDriFuzz/fuzzer/go/src/github.com/google/syzkaller/bin/linux_amd64/syz-fuzzer root@localhost:/syz-fuzzer

scp -P 1569 \
    -F /dev/null \
    -o UserKnownHostsFile=/dev/null \
    -o BatchMode=yes \
    -o IdentitiesOnly=yes \
    -o StrictHostKeyChecking=no \
    -o ConnectTimeout=10 \
    -i /storage/PMIinDriFuzz/build/image/stretch.id_rsa \
    -v /storage/PMIinDriFuzz/fuzzer/go/src/github.com/google/syzkaller/bin/linux_amd64/syz-executor root@localhost:/syz-executor

ssh -p 1569 \
    -F /dev/null \
    -o UserKnownHostsFile=/dev/null \
    -o BatchMode=yes \
    -o IdentitiesOnly=yes \
    -o StrictHostKeyChecking=no \
    -o ConnectTimeout=10 \
    -i /storage/PMIinDriFuzz/build/image/stretch.id_rsa \
    -v root@localhost

ssh -p 1569 -i /storage/PMIinDriFuzz/build/test-image/stretch.id_rsa root@localhost