#! /bin/bash

qemu-system-x86_64 \
	-m 512 \
    -smp 1 \
    -net nic,model=e1000 \
    -net user,host=10.0.2.10,hostfwd=tcp::1569-:22 \
    -display none \
    -serial mon:stdio \
    -no-reboot \
    -enable-kvm \
    -cpu host,pmu=true \
    -s \
    -hda "/storage/PMIinDriFuzz/build/test_image/stretch.img" \
    -kernel /storage/PMIinDriFuzz/build/kernel/main/pci/fbdev/arch/x86/boot/bzImage \
    -append "root=/dev/sda panic=1 ftrace_dump_on_oops=orig_cpu rodata=n vsyscall=native net.ifnames=0 biosdevname=0 console=ttyS0 kvm-intel.nested=1 kvm-intel.unrestricted_guest=1 kvm-intel.vmm_exclusive=1 kvm-intel.fasteoi=1 kvm-intel.ept=1 kvm-intel.flexpriority=1 kvm-intel.vpid=1 kvm-intel.emulate_invalid_guest_state=1 kvm-intel.eptad=1 kvm-intel.enable_shadow_vmcs=1 kvm-intel.pml=1 kvm-intel.enable_apicv=1 perf_v4_pmi=y"

