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

BUILD_DIR=""
INSTALL_MOD_PATH=/storage/PMIinDriFuzz/build/kernel/image

while :
do
    case "$1" in
        -b | --build)
            BUILD_DIR="$2"
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

if [[ -z ${BUILD_DIR} ]]; then
    usage "Build path is needed!!"
    exit 1
fi;

mkdir -p $BUILD_DIR

pushd ${BUILD_DIR}
make modules_install INSTALL_MOD_PATH=$INSTALL_MOD_PATH
popd