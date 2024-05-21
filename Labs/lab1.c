#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>


#define LEDR_BASE 0x00000000
#define LW_BRIDGE_BASE 0xFF200000
#define LW_BRIDGE_SPAN 0x00005000

/* Prototypes for functions used to access physical memory addresses */
int open_physical (int);
void * map_physical (int, unsigned int, unsigned int);
void close_physical (int);
int unmap_physical (void *, unsigned int);

/* This program increments the contents of the red LED parallel port */
int main(void)
{   
    volatile int * LEDR_ptr; // virtual address pointer to red LEDs
    int fd = -1; // used to open /dev/mem
    void *LW_virtual; // physical addresses for light-weight bridge

    // Create virtual memory access to the FPGA light-weight bridge
    if ((fd = open_physical (fd)) == -1)
    return (-1);
    if (!(LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)))
    return (-1);
    
    while(1){
        LEDR_ptr = (int *) (LW_virtual + LEDR_BASE);
        
        *LEDR_ptr = 0x00000001;   
        for(int i =0; i<9;i++){
            sleep(1);
            *LEDR_ptr = *LEDR_ptr << 1; // Add 1 to the I/O register
        }
        
        
        for(int i =0; i<9;i++){
            sleep(1);
            *LEDR_ptr = *LEDR_ptr >> 1; // Add 1 to the I/O register
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