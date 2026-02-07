## PrIntFuzz
Record every bugs when reproduce PrIntFuzz, use patch/printfuzz.patch to fix it.

1. update cmake version (ref to common.md)
2. update git version (ref to common.md)
3. "fatal error: 'filesystem' file not found" llvm version is 14.0.0, [solution](https://blog.csdn.net/BIT_HXZ/article/details/127297351)

## just
1. sudo apt install just:
    err-msg: "E: Unable to locate package just"
    solution 1: install snap, then use snap install just
    result 1: Work. But I'm not familiar with snap.
    solution 2: set up [Prebuilt-MPR set up](https://docs.makedeb.org/prebuilt-mpr/getting-started/#setting-up-the-repository), then use apt install just
    result 2: makedeb unmaintained
    solution 3: use makedeb repo
    result 3: makedeb repo unmaintained
    
    Conclusion: Don't use just on Ubuntu if Ubuntu Distro not 24.04, or be familiar to snap.

# Qemu
1. /dev/vda:
    the vda device in vm-template.sh seems like it should run on a kernel with virtio config enable. TODO_L4: Check this statement.
2. /sys/fuze/connection not exist:
    My origin guest config is the def config in kernel source, but for building a guest vm, it should execute:
        make defconfig
        make kvm_guest.config
    TODO_L3: What config included in kvm_guest.config but excluded in defconfig
3. Segmentation fault at init:
    I replace the system installed qemu executable to my compiled qemu, then run the original guest kernel image / fs image, It crash at init. I rebuild the qemu from official stable commit (original run on my own commit) and rebuild the fs image, this bug fix.
    TODO_L3: I guess the problem is fs image isn't formatted (image is modified so that my qemu command can't recognize it.).
4. Segmentation fault again:
    I use the scripts/sh/run/qemu_test.sh to run the VM, it can successfully start, and also it can repeat run it, but when I run the "task run-syzkaller", it immediately segmentation failed, even I remove the image file, it can't use it again. Then I reboot the machine, it works fine again. Even I build a new image to run, I got the same error, I guess the problem is related to syzkaller, this is the error message:
    failed to create instance: failed to read from qemu: EOF
    The problem is related to ivshmem not enable in guest, so qemu can't successfully init PCI device in guest kernel for ivshmem.
    sol: the problem is I forget to revert last qemu commit in remote, so I still use the buggy qeemu code to run, and the host kernel seems like using wrong config to compile. I update the host kernel config and recompile the qemu, it works again.


# Syzkaller
    1. open(proc/self/ns/net) error number 2
        The enable_syscall list (config/linux/enable_config.json) in printfuzz for some reason not enable CONFIG_NET_NS, simply add to enable config list in pci
    2. machine check: got no fallback coverage:
        The syzkaller not support CONFIG_KCOV_INSTRUMENT_ALL disable fuzzing, so agamotto and printfuzz choose to close the error when syzkaller generate test syscall to test executor.
    3. syscall ioctl$DRM_IOCTL_MODE_CURSOR is unsupported on all arches (typo?)
        I paste the dev_dri.txt from printfuzz syzkaller, but I got this error. [syzkaller_mail_list](https://groups.google.com/g/syzkaller/c/Q5K3tS6dcDU) has similar problem.
        sol: I forget to make bin/syz-extract, so ./bin/extract doesn't create const file, so it can't resolve some ioctl?
# fcntl
1. error: `F_SETSIG' undeclared (first use in this function)
    sol: [debian report](https://lists.debian.org/debian-amd64/2005/02/msg00765.html) define _GNU_SOURCE  TODO_L3: what is _GNU_SOURCE
# gcc
1. undefine reference to static link library.
    I try to link my perf agent library(generic c code) to syz-executor in host, but I got undefined reference, the problem is executor.cc is a cpp code, and it can't directly link a c library because it need more information than function name (see [stack_overflow](https://stackoverflow.com/questions/1068129/c-library-not-linking-using-gcc-g)).

# modprobe
1. modprobe: FATAL: Module ivshmem_pci not found in directory /lib/modules/5.5.0-rc3+
    I copy all files from /build/image/pci/lib/modules to /lib/modules but we can't load it.
    sol: I directly rm the /lib/modules and copy /build/image/pci/lib/modules to /lib/modules again. I guess the problem is cp -rd will leave some file exist in old folder but not in new folder.


