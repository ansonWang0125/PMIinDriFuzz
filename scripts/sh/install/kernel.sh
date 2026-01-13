#!/bin/bash

usage() {
    U=""
    if [[ -n "$1" ]]; then
        U="${U}$1\n\n"
    fi
    U="${U}Usage: $0 [options]\n\n"
    U="${U}Options:\n"
    U="$U    -k | --kernel:         Kernel source path\n"
    U="$U    -h | --help:           Show this output\n"
    echo -e "$U" >&2
}

KERNEL="/storage/PMIinDriFuzz/kernel"

while :
do
    case "$1" in
        -k | --kernel)
            KERNEL="$2"
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

if [[ -z ${KERNEL} ]]; then
    usage "Kernel path is needed!!"
    exit 1
fi;

if [[ ! -d ${KERNEL} ]]; then
    usage "Kernel path is invalid!!"
    echo "Kernel: ${KERNEL}"
    exit 1
fi;

echo "Installing \"${KERNEL}\"."

pushd ${KERNEL}
sudo make modules_install
sudo make install
popd
