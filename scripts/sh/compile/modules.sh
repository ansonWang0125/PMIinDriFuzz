#! /bin/bash

set -x

IVSHMEM_PCI_DIR=/storage/PMIinDriFuzz/scripts/c/module/ivshmem_pci

usage() {
    U=""
    if [[ -n "$1" ]]; then
        U="${U}$1\n\n"
    fi
    U="${U}Usage: $0 [options]\n\n"
    U="${U}Options:\n"
    U="$U    -t | --target:         Target kernel\n"
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

pushd ${IVSHMEM_PCI_DIR}
make TARGET=${TARGET}
popd