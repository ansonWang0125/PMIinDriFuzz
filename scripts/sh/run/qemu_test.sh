#! /bin/bash

qemu-system-x86_64 \
	-m 2G \
	-smp 2 \
    -cpu host \
	-kernel /storage/PMIinDriFuzz/build/kernel/main/def/arch/x86/boot/bzImage \
	-append "console=ttyS0 root=/dev/sda earlyprintk=serial net.ifnames=0" \
	-hda "/storage/PMIinDriFuzz/build/test-image/stretch.img" \
	-enable-kvm \
	-nographic