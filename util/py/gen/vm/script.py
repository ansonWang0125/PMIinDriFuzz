import subprocess



def _run_cmd(cmd):
    print('\n[CMD]:', cmd)
    subprocess.run(cmd, shell=True, check=True)

def parse_arg():
    parser = argparse.ArgumentParser(prog="get_qemu_command", description="A simple program to get qemu command \
                                                                            example usage: \
                                                                            python3 command.py -d /storage/PMIinDriFuzz/config/qemu/pci.json -t sound")
    parser.add_argument("-d", "--device")
    parser.add_argument("-t", "--type")
    return parser.parse_args()

if __name__ == '__main__':
    args = parse_arg()
    if not args.device or not args.type:
        print("Invalid device path or type, try --help")
    device = args.device
    device_type = args.type
    