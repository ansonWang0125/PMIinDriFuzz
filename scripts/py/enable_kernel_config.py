import argparse
import json
import subprocess

source_json_dir = "/storage/PMIinDriFuzz/config/kernel/enable/"
target_config_dir = "/storage/PMIinDriFuzz/config/kernel/"
linux_config_tool = "/storage/PMIinDriFuzz/guest-kernel/scripts/config"

source_json = ""
base_config = target_config_dir + "template_defconfig"
target_config = ""

def _enable_config(mode, config_name, config_status=''):
    cmd = (f'{linux_config_tool} --file '
            f'{target_config} -{mode} {config_name} {config_status}')
    print(f"toggle {config_name}")
    subprocess.run(cmd, shell=True)

def enable_config():
    with open(source_json, 'r') as f:
        config = json.loads(f.read())
    for config_name, config_status in config.items():
        if 'd' == config_status or 'e' == config_status or 'm' == config_status:
            mode = config_status
            _enable_config(mode, config_name)
        elif 'CMDLINE' == config_name:
            _enable_config('-set-str', config_name, config_status)
        else:
            _enable_config('-set-val', config_name, config_status)

def parse_arg():
    parser = argparse.ArgumentParser(prog="enable_config", description="A simple program to enable driver config in kernel config.")
    parser.add_argument("-s", "--source")
    parser.add_argument("-t", "--target")
    return parser.parse_args()

if __name__ == '__main__':
    args = parse_arg()
    source_json = source_json_dir + args.source
    target_config = target_config_dir + args.target
    cmd = (f'cp {base_config} {target_config}')
    subprocess.run(cmd, shell=True)
    enable_config()
    

