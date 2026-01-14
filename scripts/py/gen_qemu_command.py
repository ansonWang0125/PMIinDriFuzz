import json
import argparse

class GetQemuCommand(object):
    
    def __init__(self):
        self.__parse_args()
        self.__process_args()

    def __parse_args(self):
        parser = argparse.ArgumentParser(prog="get_qemu_command", description="A simple program to get qemu command \
                                                                                example usage: \
                                                                                python3 command.py -c /storage/PMIinDriFuzz/config/qemu/pci.json -t sound")
        parser.add_argument("-c", "--device_config")
        parser.add_argument("-t", "--type")
        self.args = parser.parse_args()

    def __process_args(self):
        self.device_config = self.args.device_config
        self.device_type = self.args.type

    def _get_devices(self):
        device_list = []
        cnt = 0
        with open(self.device_config, 'r') as f:
            devices = json.loads(f.read())
            for device in devices[self.device_type]:
                cnt += 1
                device_list.append(f"-device {device},bus=pcie.1,addr={hex(cnt)}\n")
        return " ".join(device_list)

    def _generate_command(self):
        self.command = 'qemu-system-x86_64\n'
        self.command += ' -M q35 --enable-kvm \n -device pci-bridge,id=pcie.1,bus=pcie.0,chassis_nr=1\n '
        self.command += self._get_devices()

    def process(self):
        self._generate_command()
        print(self.command)

if __name__ == '__main__':
    getQemuCommand = GetQemuCommand(sys.argv[1:])
    getQemuCommand.process()