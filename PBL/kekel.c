#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/io.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <linux/uaccess.h>



#define DATA_A 0x80
#define DATA_B 0x70
#define LW_BRIDGE_BASE 0xFF200000
#define LW_BRIDGE_SPAN 0x00005000
#define START 0xc0
#define WR_FULL 0xb0

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TESTE");

volatile int *ptr_A; // virtual address pointer to red LEDs
volatile int *ptr_B;
volatile int *ptr_FULL;
volatile int *ptr_START;
void *HW_virtual; // physical addresses for light-weight bridge
struct device
{
    char data[100];
    struct semaphore sem;

} virtual_device;


struct cdev *mcdev;
int major_number;
ssize_t ret;
    

dev_t dev_num;

#define DEVICE_NAME "GPU"

int device_open(struct inode *inode, struct file *filp){
    printk(KERN_INFO "GPU: opened device"); 
    return 0;   
}

static ssize_t device_write(struct file *filp, unsigned long long* bufSourceData, size_t bufCount, loff_t* curOffSet){
    printk(KERN_INFO "GPU: writing device");
    ret = copy_from_user(virtual_device.data, bufSourceData, bufCount);
    
    while(*ptr_FULL)
    {
        printk(KERN_INFO "fila cheia");
    }

    unsigned long int dataA;
    unsigned long int dataB;
    memcpy(&dataA,virtual_device.data,4);
    memcpy(&dataB,virtual_device.data+4,4);

    *ptr_A = dataA;
    // sem_close(virtual_device.sem);
    *ptr_B = dataB;
    *ptr_START = 1; // Libera
    *ptr_START = 0;

    return ret;
}

ssize_t device_read(struct file *filp, char* bufStoreData, size_t bufCount, loff_t* curOffSet){
    printk(KERN_INFO "GPU: reading from device");
    ret = copy_to_user(bufStoreData, virtual_device.data, bufCount);
    return ret;
}


static int device_close(struct inode *inode, struct file *filp){
    // sem_close(virtual_device.sem);
    printk(KERN_ALERT "GPU: unloaded module");
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_close,
    .write = device_write,
    .read = device_read
};  

static int __init iniciar(void) {  
    ret = alloc_chrdev_region(&dev_num,0,1,DEVICE_NAME);
    if(ret < 0){
        printk(KERN_ALERT "GPU: failed to allocate major number");
        return ret;
    }
    
    HW_virtual = ioremap_nocache (LW_BRIDGE_BASE, LW_BRIDGE_SPAN);
    
    ptr_A = (int *)(HW_virtual + DATA_A);
    ptr_B = (int *)(HW_virtual + DATA_B);
    ptr_START = (int *)(HW_virtual + START);
    ptr_FULL = (int *)(HW_virtual + WR_FULL);
    
    major_number = MAJOR(dev_num);
    printk(KERN_INFO "GPU: major number is %d", major_number);
    printk(KERN_INFO "\tuse \"mknod /dev/%s c %d 0\" for device file", DEVICE_NAME,major_number);

    mcdev= cdev_alloc();    
    mcdev->ops = &fops;
    mcdev->owner = THIS_MODULE;

    ret = cdev_add(mcdev, dev_num, 1);
    if(ret < 0){
        printk(KERN_ALERT "GPU: unstruct file *filpable to add dev cdev to kernel");
        return ret; 
    }
    // sem_init(&virtual_device.sem,1);
}

static void __exit sair(void) {
  iounmap (HW_virtual);
//   *ptr_START = 0; // colar no write
  cdev_del(mcdev);
  unregister_chrdev_region(dev_num, 1);
  printk(KERN_INFO "Fechando o modulo");
}

module_init(iniciar);
module_exit(sair);
    
