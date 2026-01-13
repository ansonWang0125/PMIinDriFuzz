import subprocess

class RunVM(object):

    def __init__(self):
        self.__parse_args()
        self.__process_args()

    def _parse_args(self, args):
        parser = argparse.ArgumentParser()
        group = parser.add_mutually_exclusive_group(required=True)
        group.add_argument('-k', '--kernel', type=str)
        group.add_argument('-m', '--mode', type=str, choices=['fuzz', 'fault'])
        group.add_argument('-a', '--arch', type=str, default='x86_64',
                choices=['x86_64', 'aarch64'])
        parser.add_argument('-d', '--debug', action='store_true')
        parser.add_argument('-g', '--gdb', action='store_true')
        parser.add_argument('-f', '--fault', action='store_true')
        parser.add_argument('-p', '--port', type=int, default=self._get_port())
        parser.add_argument('-e', '--extra', type=str)
        parser.add_argument('-o', '--output', type=Path)
        parser.add_argument('-s', '--share', type=Path)
        parser.add_argument('--host-cpu', action='store_true')
        parser.add_argument('--no-snapshot', action='store_true')

        parser.add_argument("-d", "--device", const="none")
        parser.add_argument("-t", "--type", const="none")
        self.extend_args = parser.parse_args(args)

    def _setup_args(self):
        if self.extend_args.arch == 'aarch64':
            kernel = self.env.build_linux_aarch64_dir
            image = self.env.out_buildroot_image
        else:
            if self.extend_args.mode == 'fault':
                kernel = self.env.build_linux_fault_dir
                image = self.env.out_build_image_fault_injection
            else:
                kernel = self.env.linux_build_fuzz_dir
                image = self.env.out_build_image_without_module

        self.args = []
        if self.extend_args.gdb:
            self.args.extend('gdb --args'.split(' '))
        self.args.append(f'{self.env.qemu_bin_dir}/qemu-system-{self.extend_args.arch}')
        self.args.extend('-m 4G'.split(' '))
        self.args.extend('-smp 8'.split(' '))
        machine = 'virt' if self.extend_args.arch == 'aarch64' else 'q35'
        self.args.extend(f'-M {machine}'.split(' '))
        if self.extend_args.host_cpu:
            self.args.extend(f'-cpu host'.split(' '))
        if self.extend_args.arch == 'aarch64':
            self.args.extend(f'-cpu cortex-a57'.split(' '))
            self.args.extend(f'-append:console=ttyAMA0 root=/dev/vda oops=panic panic_on_warn=1 panic=-1 ftrace_dump_on_oops=orig_cpu debug earlyprintk=serial slub_debug=UZ'.split(':'))
        if self.extend_args.arch == 'x86_64':
            kernel_image = f'{kernel}/arch/x86/boot/bzImage'
        else:
            kernel_image = f'{kernel}/arch/arm64/boot/Image'
        self.args.extend(f'-kernel {kernel_image}'.split(' '))
        if not self.extend_args.arch == 'aarch64':
            self.args.append('-enable-kvm')
        if self.extend_args.debug:
            self.args.append('-S')
        self.args.extend('-display none'.split(' '))
        self.args.append('-no-reboot')
        self.args.extend('-name VM-QEMU'.split(' '))
        self.args.extend('-device virtio-rng-pci'.split(' '))
        self.args.extend(f'-hda {image}'.split(' '))
        if not self.extend_args.no_snapshot:
            self.args.append('-snapshot')
        share_dir = self.extend_args.share if self.extend_args.share \
                else self.env.out_share_dir
        self.args.extend(
            f'-fsdev local,id=fsdev0,path={share_dir},'
            'security_model=none'.split(' '))
        self.args.extend(
            '-device virtio-9p-pci,id=fs0,fsdev=fsdev0,mount_tag=hostshare'.split(' '))
        if not self.extend_args.arch == 'aarch64':
            self.args.extend('-device qemu-xhci'.split(' '))
        if self.extend_args.extra:
            self.args.extend(f'-device {self.extend_args.extra}'.split(' '))
        self.args.extend('-device e1000,netdev=net0'.split(' '))
        self.args.extend(
            '-netdev user,id=net0,restrict=on,'
            f'hostfwd=tcp:127.0.0.1:{self.extend_args.port}-:22'
            .split(' '))
        if not self.extend_args.fault:
            self.args.extend(
                '-monitor telnet:127.0.0.1:4444,server,nowait'.split(' '))
            self.args.extend(
                '-chardev socket,id=SOCKSYZ,server,nowait,host=localhost,'
                f'port={self._get_port()}'.split(' '))
            self.args.extend('-mon chardev=SOCKSYZ,mode=control'.split(' '))
            self.args.append('-s')
        self.args.extend('-serial mon:stdio'.split(' '))
    
    def __process_args(self):
        self.kernel = self.args.kernel
        self.device = self.args.device
        self.device_type = args.type

    def _get_devices(self):

    def _generate_qemu_command(self):
        self.command = 'qemu-system-x86_64\n'
        self.command += ' -M q35 --enable-kvm \n -device pci-bridge,id=pcie.1,bus=pcie.0,chassis_nr=1\n '
        self.command += self._get_devices()

    def _run_qemu_command():

    def process():
        _get_devices()
        _generate_qemu_command()
        _run_qemu_command()

if __name__ == '__main__':
    runVM = RunVM()
    runVM.process()
    