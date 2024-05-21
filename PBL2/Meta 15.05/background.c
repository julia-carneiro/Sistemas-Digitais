#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>

#define DATA_A 0x80
#define DATA_B 0x70
#define HW_REGS_BASE 0xfc000000
#define HW_REGS_SPAN 0x04000000
#define LW_BRIDGE_BASE 0xFF200000
#define LW_BRIDGE_SPAN 0x00005000
#define START 0xc0

/* Prototypes for functions used to access physical memory addresses */
int open_physical(int);
void *map_physical(int, unsigned int, unsigned int);
void close_physical(int);
int unmap_physical(void *, unsigned int);

int main() {
  volatile int *ptr_A; // virtual address pointer to red LEDs
  volatile int *ptr_B;
  volatile int *ptr_START;
  int fd = -1;      // used to open /dev/mem
  void *HW_virtual; // physical addresses for light-weight bridge

  // Create virtual memory access to the FPGA light-weight bridge
  if ((fd = open_physical(fd)) == -1)
    return (-1);
  //if (!(HW_virtual = map_physical(fd, HW_REGS_BASE, HW_REGS_SPAN)))
    //return (-1);
  if (!(HW_virtual = map_physical(fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)))
    return (-1);

  ptr_A = (int *)(HW_virtual + DATA_A);
  ptr_B = (int *)(HW_virtual + DATA_B);
  ptr_START = (int *)(HW_virtual + START);

  *ptr_A = 0;
  *ptr_B = 40;
  *ptr_START = 1; // Libera
  *ptr_START = 0;

  unmap_physical(HW_virtual, HW_REGS_SPAN);
  close_physical(fd);

  return 0;
}

// FUNÇÕES DE MAPEAMENTO DA MEMORIA
/* Open /dev/mem to give access to physical addresses */
int open_physical(int fd) {
  if (fd == -1) // check if already open
    if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
      printf("ERROR: could not open \"/dev/mem\"...\n");
      return (-1);
    }
  return fd;
}

/* Close /dev/mem to give access to physical addresses */
void close_physical(int fd) { close(fd); }

/* Establish a virtual address mapping for the physical addresses starting
 * at base and extending by span bytes */
void *map_physical(int fd, unsigned int base, unsigned int span) {
  void *virtual_base;
  // Get a mapping from physical addresses to virtual addresses
  virtual_base =
      mmap(NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, base);
  if (virtual_base == MAP_FAILED) {
    printf("ERROR: mmap() failed...\n");
    close(fd);
    return (NULL);
  }
  return virtual_base;
}

/* Close the previously-opened virtual address mapping */
int unmap_physical(void *virtual_base, unsigned int span) {
  if (munmap(virtual_base, span) != 0) {
    printf("ERROR: munmap() failed...\n");
    return (-1);
  }
  return 0;
}

