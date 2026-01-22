from pathlib import Path
import types

class Environment(object):

    def __init__(self):
        self.__set_env()
    
    def __set_env(self):
        self.env = types.SimpleNamespace()
        self.env.home_dir = Path("/storage")
        self.env.proj_name = 'PMIinDriFuzz'
        self.env.proj_dir = self.env.home_dir / self.env.proj_name
        self.env.guest_kernel_dir = self.env.proj_dir / "guest-kernel"
        self.env.host_kernel_dir = self.env.proj_dir / "host-kernel"
        self.env.out_dir = self.env.proj_dir / 'out'
        self.env.build_dir = self.env.proj_dir / 'build'
        self.env.bin_dir = self.env.proj_dir / 'bin'
        self.env.out_work_dir = self.env.out_dir / 'workdir'
        self.env.build_image_dir = self.env.build_dir / 'image'
        self.env.build_kernel_dir = self.env.build_dir / 'kernel'
        self.env.build_qemu_dir = self.env.build_dir / 'qemu'
        self.env.bin_qemu_dir = self.env.bin_dir / 'qemu'
        # TODO-L4: Find a way to avoid syzkaller place in a strange path.
        self.env.syzkaller = Path("/storage/PMIinDriFuzz/fuzzer/go/src/github.com/google/syzkaller")
