#! /usr/bin/env bash

CONSOLE=mon:stdio
SMP=1
MEMSIZE=$((4096))
KERNEL="/storage/build/guest/arch/x86/boot/bzImage"
FS="/storage/agamotto-guest-linux/scripts/focal.img"
CMDLINE=""
PORT=1569
VCPUPIN=1
MONPORT=55555

usage() {
        U=""
        if [[ -n "$1" ]]; then
                U="${U}$1\n\n"
        fi
        U="${U}Usage: $0 [options]\n\n"
        U="${U}Options:\n"
        U="$U    -c | --CPU <nr>:       Number of cores (default ${SMP})\n"
        U="$U    -v | --vcpupin <list>: Pin vcpu to <list>\n"
        U="$U    -m | --mem <MB>:       Memory size (default ${MEMSIZE})\n"
        U="$U    -k | --kernel <Image>: Use kernel image (default ${KERNEL})\n"
        U="$U    -s | --serial <file>:  Output console to <file>\n"
        U="$U    -i | --image <image>:  Use <image> as block device (default $FS)\n"
        U="$U    -a | --append <snip>:  Add <snip> to the kernel cmdline\n"
        U="$U    -p | --port <port>:    Host port (default $PORT)\n"
        U="$U    -mp | --monitor-port <port>:   QMon port (default $MONPORT)\n"
        U="$U    -h | --help:           Show this output\n"
        U="${U}\n"
        echo -e "$U" >&2
}

while :
do
        case "$1" in
          -c | --cpu)
                SMP="$2"
                shift 2
                ;;
          -v | --vcpupin)
                VCPUPIN="$2"
                shift 2
                ;;
          -m | --mem)
                MEMSIZE="$2"
                shift 2
                ;;
          -k | --kernel)
                KERNEL="$2"
                shift 2
                ;;
          -s | --serial)
                CONSOLE="file:$2"
                shift 2
                ;;
          -i | --image)
                FS="$2"
                shift 2
                ;;
          -a | --append)
                CMDLINE="$2"
                shift 2
                ;;
          -h | --help)
                usage ""
                exit 1
                ;;
          -p | --port)
                PORT=$2
                shift 2
                ;;
          -mp | --monitor-port)
                MONPORT=$2
                shift 2
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

# QEMU COMMAND:
qemu-system-x86_64 \
    -m ${MEMSIZE} \
    -smp ${SMP} \
    -net nic,model=e1000 -net user,host=10.0.2.10,hostfwd=tcp::${PORT}-:22 \
    -display none \
    -serial ${CONSOLE} \
    -no-reboot \
    -enable-kvm \
    -cpu host,lbr-fmt=5,pmu=true \
    -s \
    -drive if=none,file=${FS},id=vda,cache=none,format=raw \
    -device virtio-blk-pci,drive=vda \
    -monitor telnet:127.0.0.1:${MONPORT},server,nowait \
    -kernel ${KERNEL} \
    -append "earlyprintk=serial oops=panic nowatchdog panic_on_warn=1 ftrace_dump_on_oops=orig_cpu rodata=n vsyscall=native net.ifnames=0 biosdevname=0 root=/dev/vda rw console=ttyS0 kvm-intel.nested=1 kvm-intel.unrestricted_guest=1 kvm-intel.vmm_exclusive=1 kvm-intel.fasteoi=1 kvm-intel.ept=1 kvm-intel.flexpriority=1 kvm-intel.vpid=1 kvm-intel.emulate_invalid_guest_state=1 kvm-intel.eptad=1 kvm-intel.enable_shadow_vmcs=1 kvm-intel.pml=1 kvm-intel.enable_apicv=1 dummy_hcd.num=8 nokaslr perf_v4_pmi=y ${CMDLINE}"
