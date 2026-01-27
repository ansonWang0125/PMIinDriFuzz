obj-m += ivshmem_pci.o   
   
PWD := "/storage/PMIinDriFuzz/scripts/c/module/ivshmem_pci"
   
all:  
	$(MAKE) -C /storage/PMIinDriFuzz/build/kernel/main/def M=$(PWD) modules
	
clean: 
	$(MAKE) -C /storage/PMIinDriFuzz/build/kernel/main/def M=$(PWD) clean