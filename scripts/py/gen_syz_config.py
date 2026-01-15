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
            default="main",
        )
        parser.add_argument(
            "--device", dest="device",
            type=str,
            default="pci",
        )
        parser.add_argument(
            "--host", dest="host",
            type=str,
            default="localhost",
        )
        parser.add_argument(
            "--image", dest="image",
            type=str,
            default="stretch.img",
        )
        parser.add_argument(
            "--output", dest="output",
            type=str,
            required=True,
        )
        parser.add_argument(
            "--port", dest="port",
            type=str,
            default="56841",
        )
        parser.add_argument(
            "--proc_num", dest="proc_num",
            type=str,
            default="1",
        )
        parser.add_argument(
            "--public_key", dest="public_key",
            type=str,
            default="stretch.id_rsa",
        )
        parser.add_argument(
            "--qemu", action="store_true",
        )
        parser.add_argument(
            "--syscalls", action="store_true",
        )
        parser.add_argument(
            "--type", dest="type",
            type=str,
            default="fbdev",
        )
        parser.add_argument(
            "--vmcnt", dest="vmcnt",
            type=str,
            default="1",
        )
        self.args = parser.parse_args(args)

    def __process_args(self):
        self.input = "/storage/PMIinDriFuzz/config/fuzzer/syzkaller/template.cfg"
        self.qemu_args = "qemu-system-x86_64 -m 512 -smp"
        self.branch = self.args.branch
        self.device = self.args.device
        self.host = self.args.host
        self.image = self.args.image
        self.output = self.args.output
        self.port = self.args.port
        self.proc_num = self.args.proc_num
        self.public_key = self.args.public_key
        self.qemu = self.args.qemu
        self.type = self.args.type
        self.vmcnt = self.args.vmcnt
        self.syscalls = self.args.syscalls
        self.enable_syscalls = ""

    def __replace_config(self):
        with open(self.input, "r") as infd:
            with open(self.output, "w") as outfd:
                outfd.truncate()
                for line in infd.readlines():
                    line = line.replace("$HOST", self.host)
                    line = line.replace("$PORT", self.port)
                    line = line.replace("$OUT_WORK_DIR", str(self.env.out_work_dir))
                    line = line.replace("$BRANCH", self.branch)
                    line = line.replace("$DEVICE", self.device)
                    line = line.replace("$TYPE", self.type)
                    line = line.replace("$BUILD_KERNEL_DIR", str(self.env.build_kernel_dir))
                    line = line.replace("$PROC_NUM", self.proc_num)
                    line = line.replace("$PROJ_DIR", str(self.env.proj_dir))
                    line = line.replace("$BUILD_DIR", str(self.env.build_dir))
                    line = line.replace("$IMAGE", self.image)
                    line = line.replace("$BUILD_IMAGE_DIR", str(self.env.build_image_dir))
                    line = line.replace("$PUBLIC_KEY", self.public_key)
                    line = line.replace("$SYZKALLER", str(self.env.syzkaller))
                    line = line.replace("$ENABLE_SYSCALLS", self.enable_syscalls)
                    line = line.replace("$VMCNT", self.vmcnt)
                    line = line.replace("$BIN_QEMU_DIR", str(self.env.bin_qemu_dir))
                    line = line.replace("$QEMU_ARGS", self.qemu_args)
                    if not self.qemu and ( "qemu_args" in line or "qemu-system-x86_64" in line ):
                        continue
                    if not self.syscalls and "enable_syscalls" in line:
                        continue
                    outfd.write(line)

    def process(self):
        print("Start process!")
        self.__replace_config()

if __name__ == '__main__':
    genSyzConfig = GenSyzConfig(sys.argv[1:])
    genSyzConfig.process()
