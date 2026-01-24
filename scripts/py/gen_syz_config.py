import sys
import argparse
import json
import os
from pathlib import Path
# from check_exist import CheckEnvExist
import env

class GenSyzConfig(object):

    def __init__(self, args):
        self.env = env.Environment().env
        self.__parse_args(args)
        self.__process_args() 

    def __parse_args(self, args):
        parser = argparse.ArgumentParser()
        parser.add_argument(
            "--branch", dest="branch",
            type=str,
        )
        parser.add_argument(
            "--driver", dest="driver",
            type=str,
        )
        parser.add_argument(
            "--driver_type", dest="driver_type",
            type=str,
        )
        self.args = parser.parse_args(args)

    def __process_args(self):
        with open(f"/storage/PMIinDriFuzz/config/fuzzer/syzkaller/{self.args.driver_type}/driver_setup.json", "r") as fd:
            driver_setup = json.loads(fd.read())
        for driver_path, driver_info in driver_setup.items():
            if self.args.driver in driver_path:
                self.syscall_list =  driver_info["syscalls"]
                self.devices = driver_info["devices"]
        self.config = {}
        print(self.devices[0])

    def __gen_config(self):
        self.config['target'] = 'linux/amd64'
        self.config['http'] = 'localhost:56841'
        self.config['workdir'] = str(self.env.out_work_dir / f"{self.args.branch}/{self.args.driver_type}/{self.args.driver}")
        self.config['kernel_obj'] = str(self.env.build_kernel_dir / f"{self.args.branch}/{self.args.driver_type}")
        self.config['kernel_src'] = str(self.env.guest_kernel_dir)
        self.config['image'] = str(self.env.build_image_dir / f"stretch.img")
        self.config['sshkey'] = str(self.env.build_image_dir / f"stretch.id_rsa")
        self.config['syzkaller'] = str(self.env.syzkaller)
        self.config['procs'] = 1
        self.config['type'] = 'qemu'
        self.config['sandbox'] = 'none'
        self.config['reproduce'] = True
        self.config['cover'] = True
        self.config['enable_syscalls'] = self.syscall_list
        self.config['vm'] = {}
        self.config['vm']['count'] = 1
        self.config['vm']['kernel'] = str(self.env.build_kernel_dir / f"{self.args.branch}/{self.args.driver_type}" / 'arch/x86/boot/bzImage')
        self.config['vm']['cpu'] = 1
        self.config['vm']['mem'] = 4096
        qemu_args = '--enable-kvm '
        qemu_args += f'-device {self.devices[0]} -cpu host'
        self.config['vm']['qemu_args'] = qemu_args
        self.config['vm']['qemu'] = str(self.env.build_qemu_dir / "install/bin/qemu-system-x86_64")
        with open(f"/storage/PMIinDriFuzz/config/fuzzer/syzkaller/{self.args.driver_type}/{self.args.driver}.cfg", "w") as fd:
            json.dump(self.config, fd, indent=4)

    def process(self):
        self.__gen_config()

if __name__ == '__main__':
    genSyzConfig = GenSyzConfig(sys.argv[1:])
    genSyzConfig.process()
