from pathlib import Path
import types

class Environment(object):

    def __init__(self):
        self.__set_constant()
    
    def __set_constant(self):
        self.env = types.SimpleNamespace()
        self.env.home_dir = Path("/storage")
        self.env.project_name = 'PMIinDriFuzz'
        self.env.project_dir = self.env.home_dir / self.env.project_name

        self.env.guest_kernel = self.env.project_dir / "guest-kernel"
        self.env.host_kernel = self.env.project_dir / "host-kernel"
        self.env.out_dir = self.env.project_dir / 'out'
        self.env.build_dir = self.env.project_dir / 'build'
        self.env.bin_dir = self.env.project_dir / 'bin'
        self.env.config = self.env.project_dir / 'config'
        self.env.out_work_dir = self.env.out_dir / 'workdir'
        self.env.build_linux_dir = self.env.build_dir / 'linux'
        self.env.build_qemu_dir = self.env.build_dir / 'qemu'
        self.env.build_image_dir = self.env.build_dir / 'image'
        self.env.bin_qemu_dir = self.env.bin_dir / 'qemu'
        self.env.config_str = self.env.config / 'str_const.json'
        # TODO-L4: Find a way to avoid syzkaller place in a strange path.
        self.env.syzkaller = Path("/storage/PMIinDriFuzz/fuzzer/go/src/github.com/google/syzkaller")
