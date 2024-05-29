#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/io.h>


#define DATA_A 0x80
#define DATA_B 0x70
#define LW_BRIDGE_BASE 0xFF200000
#define LW_BRIDGE_SPAN 0x00005000
#define START 0xc0

volatile int *ptr_A; // virtual address pointer to red LEDs
volatile int *ptr_B;
volatile int *ptr_START;
void *HW_virtual; // physical addresses for light-weight bridge

static int __init iniciar(void) {  

    HW_virtual = ioremap_nocache (LW_BRIDGE_BASE, LW_BRIDGE_SPAN);

    ptr_A = (int *)(HW_virtual + DATA_A);
    ptr_B = (int *)(HW_virtual + DATA_B);
    ptr_START = (int *)(HW_virtual + START);
    
    *ptr_START = 0;
    *ptr_A = dataA;
    *ptr_B = dataB;
    *ptr_START = 1; // Libera

    return 0;
}

static void __exit sair(void) {
  *ptr_START = 0;
  iounmap (HW_virtual);
}

module_init(iniciar);
module_exit(sair);