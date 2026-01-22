import json

def get_pci_id():
    pci_ids = set()
    id_counter = 0
    with open("/storage/PMIinDriFuzz/out/json/final.json", "r") as fd:
        final_dict = json.loads(fd.read())
    for driver_info in final_dict.values():
        if "PCI Device ID" in driver_info.keys() and driver_info["PCI Device ID"] is not None:
            for ids_list in driver_info["PCI Device ID"].values():
                for pci_id in ids_list:
                    pci_ids.add(pci_id)
                break
            if ids_list is not None:
                id_counter += 1
    print(len(pci_ids))
    print(pci_ids)
    print(id_counter)

def get_driver_setup():
    driver_setup = {}
    with open("/storage/PMIinDriFuzz/out/json/final.json", "r") as fd:
        final_dict = json.loads(fd.read())
    with open("/storage/PMIinDriFuzz/config/fuzzer/syzkaller/pci/printfuzz_enable_syscalls.json", "r") as fd:
        enablesys_dic = json.loads(fd.read())
    for driver_info in final_dict.values():
        if "QEMU Devices" in driver_info.keys() and driver_info["QEMU Devices"] is not None:
            driver_source_rel = driver_info["Source File"].replace("/storage/PrIntFuzz/third_party/linux/", "")
            if driver_source_rel not in driver_setup.keys():
                driver_setup[driver_source_rel] = {}
            driver_setup[driver_source_rel]["devices"] = driver_info["QEMU Devices"]
    for value in enablesys_dic.values():
        for dri_path in driver_setup.keys():
            for subsystem in value["drivers"]:
                if subsystem in dri_path:
                    driver_setup[dri_path]["syscalls"] = value["syscalls"]
    with open("/storage/PMIinDriFuzz/config/fuzzer/syzkaller/pci/driver_setup.json", "w") as fd:
        json.dump(driver_setup, fd, indent=4)

if __name__ == '__main__':
    get_driver_setup()