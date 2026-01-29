// Reference to lsuio: https://www.osadl.org/UIO-Archives.uio-archives.0.html
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include "ivshmem_driver.h"

int line_from_file(char *filename, char *linebuf)
{
	char *s;
	int i;
	memset(linebuf, 0, UIO_MAX_NAME_SIZE);
	FILE* file = fopen(filename,"r");
	if (!file) return -1;
	s = fgets(linebuf,UIO_MAX_NAME_SIZE,file);
	if (!s) return -2;
	for (i=0; (*s)&&(i<UIO_MAX_NAME_SIZE); i++) {
		if (*s == '\n') *s = 0;
		s++;
	}
	return 0;
}

int uio_get_mem_size(struct uio_info_t* info, int map_num)
{
	int ret;
	char filename[64];
	info->maps[map_num].size = UIO_INVALID_SIZE;
	sprintf(filename, "/sys/class/uio/uio%d/maps/map%d/size",
		info->uio_num, map_num);
	FILE* file = fopen(filename,"r");
	if (!file) return -1;
	ret = fscanf(file,"0x%x",&info->maps[map_num].size);
	fclose(file);
	if (ret<0) return -2;
	return 0;
}

int uio_get_mem_addr(struct uio_info_t* info, int map_num)
{
	int ret;
	char filename[64];
	info->maps[map_num].addr = UIO_INVALID_ADDR;
	sprintf(filename, "/sys/class/uio/uio%d/maps/map%d/addr",
		info->uio_num, map_num);
	FILE* file = fopen(filename,"r");
	if (!file) return -1;
	ret = fscanf(file,"0x%lx",&info->maps[map_num].addr);
	fclose(file);
	if (ret<0) return -2;
	return 0;
}

int uio_get_event_count(struct uio_info_t* info)
{
	int ret;
	char filename[64];
	info->event_count = 0;
	sprintf(filename, "/sys/class/uio/uio%d/event", info->uio_num);
	FILE* file = fopen(filename,"r");
	if (!file) return -1;
	ret = fscanf(file,"%ld",&info->event_count);
	fclose(file);
	if (ret<0) return -2;
	return 0;
}

int uio_get_name(struct uio_info_t* info)
{
	char filename[64];
	sprintf(filename, "/sys/class/uio/uio%d/name", info->uio_num);

	return line_from_file(filename, info->name);
}

int uio_get_version(struct uio_info_t* info)
{
	char filename[64];
	sprintf(filename, "/sys/class/uio/uio%d/version", info->uio_num);

	return line_from_file(filename, info->version);
}

int uio_get_all_info(struct uio_info_t* info)
{
	int i;
	if (!info)
		return -1;
	if ((info->uio_num < 0)||(info->uio_num > UIO_MAX_NUM))
		return -1;
	for (i = 0; i < MAX_UIO_MAPS; i++) {
		uio_get_mem_size(info, i);
		uio_get_mem_addr(info, i);
	}
	uio_get_event_count(info);
	uio_get_name(info);
	uio_get_version(info);
	return 0;
}

void uio_mmap_test(struct uio_info_t* info, int map_num){
	info->maps[map_num].mmap_result = UIO_MMAP_NOT_DONE;
	if (info->maps[map_num].size <= 0)
		return;
	info->maps[map_num].mmap_result = UIO_MMAP_FAILED;
	char dev_name[16];
	sprintf(dev_name,"/dev/uio%d",info->uio_num);
	int fd = open(dev_name,O_RDONLY);
	if (fd < 0)
		return;
	void* map_addr = mmap( NULL,
			       info->maps[map_num].size,
			       PROT_READ,
			       MAP_SHARED,
			       fd,
			       map_num*getpagesize());
	if (map_addr != MAP_FAILED) {
		info->maps[map_num].mmap_result = UIO_MMAP_OK;
		munmap(map_addr, info->maps[map_num].size);
	}
	close(fd);
}

// This driver build the info constrain according to this driver "/storage/PMIinDriFuzz/scripts/c/module/ivshmem_pci/ivshmem_pci.c"
void* uio_mmap(struct uio_info_t* info){
	int ret = 0, uiofd = 0, map_num = 1;
	char dev_name[16];
	void* map_addr = NULL;

    ret = uio_get_all_info(info);
	if (ret < 0){
		printf("uio get information failed\n");
		return NULL;
	}
	if (strcmp(info->name, "ivshmem") || strcmp(info->version, "0.0.1")) {
		printf("uio information not match, name: %s(expect: ivshmem), version: %s(expect: 0.0.1)\n", info->name, info->version);
		return NULL;
	}

	info->maps[map_num].mmap_result = UIO_MMAP_NOT_DONE;
	if (info->maps[map_num].size <= 0) {
		printf("map size is negative");
		return NULL;
	}
	info->maps[map_num].mmap_result = UIO_MMAP_FAILED;
	sprintf(dev_name,"/dev/uio%d",info->uio_num);
	uiofd = open(dev_name, O_RDWR);
	if (uiofd < 0) {
		printf("uio device open failed\n");
		return NULL;
	}
	map_addr = mmap(NULL,
				info->maps[map_num].size,
				PROT_WRITE,
				MAP_SHARED,
				uiofd,
				map_num * getpagesize());
	if (map_addr != MAP_FAILED)
		info->maps[map_num].mmap_result = UIO_MMAP_OK;
	close(uiofd);
	memset(map_addr, 0, info->maps[map_num].size);
	return map_addr;
}