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
        self.config = env.Config().config
        self.__set_constant()
        self.__parse_args(args)
        # self.__process_args() 

    def __set_constant(self):
        self.kernel_src = self.env.guest_kernel
        self.image = self.config.image[0]
        self.ssh_key = self.config.pub_key[0]
        self.syzkaller = self.env.syzkaller

    def __parse_args(self, args):
        parser = argparse.ArgumentParser()
        parser.add_argument(
            "-d", "--device", dest="device",
            help="Fuzzing device",
            default=self.dev_list[0],
        )
        parser.add_argument(
            "-host", "--host", dest="host",
            help="Host IP syzkaller dashboard listening.",
            default="localhost",
        )
        parser.add_argument(
            "-b", "--branch", dest="branch",
            help="Choose a branch in kernel source.",
            default="localhost",
        )
        parser.add_argument(
            "-o", "--output", dest="output",
            help="",
            required=True,
        )
        parser.add_argument(
            "-p", "--port", dest="port",
            help="Port syzkaller dashboard listening.",
            default=56841,
        )
        parser.add_argument(
            "-t", "--type", dest="type",
            help="Fuzzing type.",
            default=1,
        )
        parser.add_argument(
            "-c", "--vmcnt", dest="vmcnt",
            help="Number of VM used.",
            default=1,
        )
        self.args = parser.parse_args(args)

    def __process_args(self):
        self.host = self.args.host
        self.port = self.args.port
        self. = self.args.branch
        gopath = os.environ["GOPATH"]

        if not self.checkExist.process("GOPATH", "env"):
            sys.exit()
        gopath = os.environ["GOPATH"]
        if not self.checkExist.process(gopath, "path"):
            sys.exit()

        cfg_name = os.path.basename(args.input)
        cfg = re.compile("^[^-]*-([^-]*)[-.].*$").search(cfg_name)
        cfg = cfg.group(1)

        cwd = os.getcwd()
        os.chdir(os.path.dirname(__file__))
        agents = fuzz.enumerate_guest_agents("linux")
        os.chdir(cwd)
        agent_id = agents.index("agent-%s-prog98" % (cfg))

        root_scripts = os.path.dirname(os.path.abspath(__file__))
        root = os.path.dirname(root_scripts)

    def __replace_config(self):
        with open(args.input, "r") as infd:
            with open(args.output, "w") as outfd:
                outfd.truncate()
                for line in infd.readlines():
                    line = line.replace("$HOST", self.host)
                    line = line.replace("$PORT", self.port)
                    line = line.replace("$WORKDIR", self.workdir)
                    line = line.replace("$KERNEL_BUILD", self.kernel_build)
                    line = line.replace("$KERNEL_SRC", self.kernel_src)
                    line = line.replace("$IMAGE", self.image)
                    line = line.replace("$SSH_KEY", self.ssh_key)
                    line = line.replace("$SYZ_PATH", self.syzkaller)
                    line = line.replace("$ENABLE_SYSCALLS", self.enable_syscalls)
                    line = line.replace("$VMCNT", self.vmcnt)
                    line = line.replace("$KERNEL_IMAGE", self.kernel_image)
                    line = line.replace("$QEMU_ARGS", self.qemu_args)
                    outfd.write(line)

    def process(self):
        print("Start process!")
        # __replace_config()

if __name__ == '__main__':
    genSyzConfig = GenSyzConfig(sys.argv[1:])
    genSyzConfig.process()
