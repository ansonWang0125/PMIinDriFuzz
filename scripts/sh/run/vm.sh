#! /bin/bash

usage() {
    U=""
    if [[ -n "$1" ]]; then
        U="${U}$1\n\n"
    fi
    U="${U}Usage: $0 [options]\n\n"
    U="${U}Options:\n"
    U="$U    -t | --target:         Target source path\n"
    U="$U    -h | --help:           Show this output\n"
    echo -e "$U" >&2
}

while :
do
    case "$1" in
        -t | --target)
            TARGET="$2"
            shift 2
            ;;
        -m | --mode)
            MODE="$2"
            shift 2
            ;;
        -h | --help)
            usage ""
            exit 1
            ;;
        --) # End of all options
            shift
            break
            ;;
        -*) # Unknown option
            echo "Error: Unknown option: $1" >&2
            exit 1
            ;;
        *)
            break
            ;;
    esac
done

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
    -hda "/storage/PMIinDriFuzz/build/${MODE}-image/stretch.img" \
    -device bochs-display\
    -device ivshmem-plain,memdev=perf_record \
    -object memory-backend-file,id=perf_record,share=on,mem-path=/dev/shm/perf_record,size="1M" \
    -virtfs local,path=/storage/PMIinDriFuzz/scripts/c/shared_fs/,mount_tag="shared",security_model="mapped" \
    -kernel /storage/PMIinDriFuzz/build/kernel/main/${TARGET}/arch/x86/boot/bzImage \
    -append "root=/dev/sda panic=1 ftrace_dump_on_oops=orig_cpu rodata=n vsyscall=native net.ifnames=0 biosdevname=0 console=ttyS0 kvm-intel.nested=1 kvm-intel.unrestricted_guest=1 kvm-intel.vmm_exclusive=1 kvm-intel.fasteoi=1 kvm-intel.ept=1 kvm-intel.flexpriority=1 kvm-intel.vpid=1 kvm-intel.emulate_invalid_guest_state=1 kvm-intel.eptad=1 kvm-intel.enable_shadow_vmcs=1 kvm-intel.pml=1 kvm-intel.enable_apicv=1 perf_v4_pmi=y"

