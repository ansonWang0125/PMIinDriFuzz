import json

driver_source = "/storage/PrIntFuzz/out/json/config.json"
general_source = "/storage/PrIntFuzz/config/linux/enable_config.json"
json_target = "/storage/PrIntFuzz/wkdir/enable.json"

target_config_type = "fuzz"

enable_config_dic = {}

def append_enable_config(mode, config_name, config_status=''):
    print("mode: %s, config: %s, config status: %s" % (mode, config_name, config_status))
    if not config_status:
        enable_config_dic[config_name] = mode
    else:
        enable_config_dic[config_name] = config_status

def write_enable_config():
    with open(json_target, "w") as f:
        json.dump(enable_config_dic, f, indent=True)

def get_enable_driver():
    with open(driver_source, 'r') as f:
        config = json.loads(f.read())
    config_list = []
    for bus, bus_config in config.items():
        for value in bus_config.values():
            if 'Config' not in value:
                continue
            for dep in value['Dep']:
                if dep in config_list:
                    continue
                config_list.append(dep)
                append_enable_config("e", dep)
            append_enable_config("e", value['Config'])
def get_enable_general():
        with open(general_source, 'r') as f:
            content = json.loads(f.read())
        for config_type, config in content.items():
            if config_type != target_config_type:
                continue
            for config_name, config_status in config.items():
                if 'd' == config_status or 'e' == config_status or 'm' == config_status:
                    mode = config_status
                    append_enable_config(mode, config_name)
                elif 'CMDLINE' == config_name:
                    append_enable_config('-set-str', config_name, config_status)
                else:
                    append_enable_config('-set-val', config_name, config_status)
if __name__ == '__main__':
    get_enable_driver()
    get_enable_general()
    write_enable_config()
