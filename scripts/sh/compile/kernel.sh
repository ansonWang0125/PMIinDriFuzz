#!/bin/bash
set -x

usage() {
    U=""
    if [[ -n "$1" ]]; then
        U="${U}$1\n\n"
    fi
    U="${U}Usage: $0 [options]\n\n"
    U="${U}Options:\n"
    U="$U    -c | --config:         Kernel config path\n"
    U="$U    -k | --kernel:         Kernel source path\n"
    U="$U    -h | --help:           Show this output\n"
    echo -e "$U" >&2
}

KERNEL="/storage/PMIinDriFuzz/kernel"
CONFIG="/storage/PMIinDriFuzz/config/kernel/guest_defconfig"
BUILD_DIR_BASE="/storage/PMIinDriFuzz/build/kernel"
BRANCH="main"
DEVICE="pci"
BUILD_DIR=""

while :
do
    case "$1" in
        -k | --kernel)
            KERNEL="$2"
            shift 2
            ;;
        -B | --branch)
            BRANCH="$2"
            shift 2
            ;;
        -b | --build)
            BUILD_DIR="$2"
            shift 2
            ;;
        -c | --config)
            CONFIG="$2"
            shift 2
            ;;
        -d | --device)
            DEVICE="$2"
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

if [[ -z ${KERNEL} || -z ${CONFIG} ]]; then
    usage "Both kernel path and config path is needed!!"
    exit 1
fi;

if [[ ! -d ${KERNEL} || ! -f ${CONFIG} ]]; then
    usage "Either kernel path or config path is invalid!!"
    echo "Kernel: ${KERNEL} Config: ${CONFIG}"
    exit 1
fi;

if [ "$BUILD_DIR" = "n" ]; then
    BUILD_DIR=""
elif [ "$BUILD_DIR" = "" ]; then
    BUILD_DIR=$BUILD_DIR_BASE/$BRANCH/$DEVICE/
fi;

mkdir -p $BUILD_DIR

cp $CONFIG $BUILD_DIR/.config

pushd ${KERNEL}
# make kvm_guest.config O=$BUILD_DIR
make olddefconfig O=$BUILD_DIR
make -j40 O=$BUILD_DIR
popd
