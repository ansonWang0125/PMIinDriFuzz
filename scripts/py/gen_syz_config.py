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
        with open(f"/storage/PMIinDriFuzz/config/fuzzer/syzkaller/{self.args.driver_type}/dritype2syscall.json", "w") as fd:
            dritype2syscall = json.loads(fd.read())
        with open(f"/storage/PMIinDriFuzz/config/fuzzer/syzkaller/{self.args.driver_type}/dri2dev.json", "w") as fd:
            dri2dev = json.loads(fd.read())
        for driver_type, syscall in dritype2syscall.item():
            if self.args.driver_type == driver_type:
                self.syscall_list =  self.args.syscall_list
        for driver_path, device_list in dri2dev.item():
            if self.args.driver_type in driver:
                self.device = device_list[0]

    def __gen_config(self):
        self.config['target'] = 'linux/amd64'
        self.config['http'] = 'localhost:amd64'
        self.config['workdir'] = str(self.env.out_work_dir / f"{self.args.branch}/{self.args.driver_type}/{self.args.driver}")
        self.config['kernel_obj'] = str(self.env.build_kernel_dir / f"{self.args.branch}/{self.args.driver_type}")
        self.config['kernel_src'] = str(self.env.proj_name / f"/{self.args.branch}")
        self.config['image'] = str(self.env.build_image_dir / f"/stretch.img")
        self.config['sshkey'] = str(self.env.build_image_dir / f"/stretch.id_rsa")
        self.config['syzkaller'] = str(self.env.syzkaller)
        self.config['procs'] = 1
        self.config['type'] = 'qemu'
        self.config['sandbox'] = 'none'
        self.config['reproduce'] = True
        self.config['cover'] = True
        self.config['enable_syscalls'] = self.syscall_list
        self.config['vm'] = {}
        self.config['vm']['count'] = 1
        self.config['vm']['kernel'] = str(self.env.build_kernel_dir / f"{self.args.branch}/{self.args.device_type}" / 'arch/x86/boot/bzImage')
        self.config['vm']['cpu'] = 1
        self.config['vm']['mem'] = 512
        qemu_args = '--enable-kvm '
        qemu_args += f'-device {self.device} '
        qemu_args += (f"-fsdev local,id=fsdev0,path={self.syz_device_dir},security_model=none "
                "-device virtio-9p-pci,id=fs0,fsdev=fsdev0,mount_tag=hostshare ")
        self.config['vm']['qemu_args'] = qemu_args
        self.config['vm']['qemu'] = str(self.env.build_qemu_dir / f'bin/qemu-system-x86_64')
        print(self.config)

    def process(self):
        self.__gen_config()

if __name__ == '__main__':
    genSyzConfig = GenSyzConfig(sys.argv[1:])
    genSyzConfig.process()
