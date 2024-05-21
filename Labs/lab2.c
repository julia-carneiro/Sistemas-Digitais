#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>

#define HEX3_HEX0_BASE 0x00000020
#define HEX5_HEX4_BASE 0x00000030
#define LW_BRIDGE_BASE 0xFF200000
#define LW_BRIDGE_SPAN 0x00005000



/* Prototypes for functions used to access physical memory addresses */
int open_physical (int);
void * map_physical (int, unsigned int, unsigned int);
void close_physical (int);
int unmap_physical (void *, unsigned int);


int main()
{

 	volatile int * HEX_ptr; // virtual address pointer to red LEDs
    int fd = -1; // used to open /dev/mem
    void *LW_virtual; // physical addresses for light-weight bridge

	// Create virtual memory access to the FPGA light-weight bridge
    if ((fd = open_physical (fd)) == -1)
    return (-1);
    if (!(LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)))
    return (-1);


	long long int frase = 0b111001111111000111000010000000110110101011110000000001111000011100110011111100000110;

	while(1){
		HEX_ptr = (int *) (LW_virtual + HEX3_HEX0_BASE);
		for(int i = 0; i<4; i++){
			*HEX_ptr = frase >> (i*8);
			sleep(1);
		}

	}

	unmap_physical (LW_virtual, LW_BRIDGE_SPAN);
    close_physical (fd);
    
    return 0;

}


/* Open /dev/mem to give access to physical addresses */
int open_physical (int fd) {
    if (fd == -1) // check if already open
    if ((fd = open( "/dev/mem", (O_RDWR | O_SYNC))) == -1) {
    printf ("ERROR: could not open \"/dev/mem\"...\n");
    return (-1);
}
    return fd;

}

/* Close /dev/mem to give access to physical addresses */
void close_physical (int fd) {
    close (fd);
}

/* Establish a virtual address mapping for the physical addresses starting
* at base and extending by span bytes */
void* map_physical(int fd, unsigned int base, unsigned int span) {
    void *virtual_base;
    // Get a mapping from physical addresses to virtual addresses
    virtual_base = mmap (NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED,
    fd, base);
    if (virtual_base == MAP_FAILED) {
    printf ("ERROR: mmap() failed...\n");
    close (fd);
    return (NULL);
}
    return virtual_base;
}

/* Close the previously-opened virtual address mapping */
int unmap_physical(void * virtual_base, unsigned int span) {
    if (munmap (virtual_base, span) != 0) {
    printf ("ERROR: munmap() failed...\n");
    return (-1);
}
return 0;
}
