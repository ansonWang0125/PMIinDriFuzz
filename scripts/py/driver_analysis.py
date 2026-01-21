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

def get_qemu_id():
    qemu_dev_ids = {}
    with open("/storage/PMIinDriFuzz/out/json/final.json", "r") as fd:
        final_dict = json.loads(fd.read())
    for driver_source_abs, driver_info in final_dict.items():
        if "QEMU Devices" in driver_info.keys() and driver_info["QEMU Devices"] is not None:
            driver_source_rel = driver_source_abs.replace("/storage/PrIntFuzz/build/linux/linux_allmodconfig/", "")
            qemu_dev_ids[driver_source_rel] = driver_info["QEMU Devices"]
    with open("/storage/PMIinDriFuzz/config/fuzzer/syzkaller/pci/device.json", "w") as fd:
        json.dump(qemu_dev_ids, fd, indent=4)

def driverType2syscall():
    sys2dev_data = {}
    tmp_dict = {}
    with open("/storage/PMIinDriFuzz/config/fuzzer/syzkaller/pci/dri2dev.json", "r") as fd:
        dri2dev_dic = json.loads(fd.read())
    with open("/storage/PMIinDriFuzz/config/fuzzer/syzkaller/pci/printfuzz_enable_syscalls.json", "r") as fd:
        enablesys_dic = json.loads(fd.read())
    for device_sys in enablesys_dic.values():
        for dri_path in dri2dev_dic.keys():
            print(device_sys["drivers"][0])
            if device_sys["drivers"][0] in dri_path:
                tmp_dict["syscalls"] = device_sys["syscalls"]
                sys2dev_data[device_sys["drivers"][0]] = tmp_dict["syscalls"]
    with open("/storage/PMIinDriFuzz/config/fuzzer/syzkaller/pci/dritype2syscall.json", "w") as fd:
        json.dump(sys2dev_data, fd, indent=4)

if __name__ == '__main__':
    driverType2syscall()