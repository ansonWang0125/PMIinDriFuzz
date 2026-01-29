#define UIO_INVALID_SIZE	-1
#define UIO_INVALID_ADDR	(~0)

#define UIO_MAX_NAME_SIZE	64
#define MAX_UIO_MAPS 	5
#define UIO_MAX_NUM		255

#define UIO_MMAP_NOT_DONE	0
#define UIO_MMAP_OK		1
#define UIO_MMAP_FAILED		2

struct uio_map_t {
	unsigned long addr;
	int size;
	int mmap_result;
};

struct uio_info_t {
	int uio_num;
	struct uio_map_t maps[ MAX_UIO_MAPS ];
	unsigned long event_count;
	char name[ UIO_MAX_NAME_SIZE ];
	char version[ UIO_MAX_NAME_SIZE ];
	struct uio_dev_attr_t *dev_attrs;
	struct uio_info_t* next;  /* for linked list */
};

void* uio_mmap(struct uio_info_t*);