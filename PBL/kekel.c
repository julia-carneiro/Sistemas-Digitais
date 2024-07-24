#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/io.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define DATA_A 0x80
#define DATA_B 0x70
#define LW_BRIDGE_BASE 0xFF200000
#define LW_BRIDGE_SPAN 0x00005000
#define START 0xc0
#define WR_FULL 0xb0

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TESTE");

// Declara ponteiros para endereços virtuais dos registradores
volatile int *ptr_A; 
volatile int *ptr_B;
volatile int *ptr_FULL;
volatile int *ptr_START;

// Endereço virtual base para o light-weight bridge
void *HW_virtual; 

// Define uma estrutura para o dispositivo virtual
struct device
{
    char data[100];

} virtual_device;

struct cdev *mcdev; // Estrutura do dispositivo de caractere
int major_number; // Número major atribuído ao nosso dispositivo
ssize_t ret; // Valor de retorno usado nas funções

dev_t dev_num; // Número do dispositivo

#define DEVICE_NAME "GPU" // Nome do nosso dispositivo

// Função de abertura do dispositivo
int device_open(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "GPU: opened device");
    return 0;
}

// Função de escrita do dispositivo
static ssize_t device_write(struct file *filp, unsigned long long *bufSourceData, size_t bufCount, loff_t *curOffSet)
{
    printk(KERN_INFO "GPU: writing device");
    ret = copy_from_user(virtual_device.data, bufSourceData, bufCount);

    // Espera enquanto o buffer estiver cheio
     while (*ptr_FULL)
     {
         printk(KERN_INFO "fila cheia");
     }

    // Extrai dados para escrever nos registradores de hardware
    unsigned long int dataA;
    unsigned long int dataB;
    memcpy(&dataA, virtual_device.data, 4);
    memcpy(&dataB, virtual_device.data + 4, 4);

    // Escreve dados nos registradores
    *ptr_A = dataA;
    *ptr_B = dataB;
    *ptr_START = 1; 
    *ptr_START = 0;

    return ret;
}

// Função de leitura do dispositivo
ssize_t device_read(struct file *filp, char *bufStoreData, size_t bufCount, loff_t *curOffSet)
{
    printk(KERN_INFO "GPU: reading from device");
    ret = copy_to_user(bufStoreData, virtual_device.data, bufCount);
    return ret;
}

// Função de fechamento do dispositivo
static int device_close(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "GPU: unloaded module");
}

// Define as operações do dispositivo
struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_close,
    .write = device_write,
    .read = device_read
};

// Função de inicialização do módulo
static int __init iniciar(void)
{
    // Aloca número major para o dispositivo
    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret < 0)
    {
        printk(KERN_ALERT "GPU: failed to allocate major number");
        return ret;
    }

    // Mapeia a memória do hardware
    HW_virtual = ioremap_nocache(LW_BRIDGE_BASE, LW_BRIDGE_SPAN);

    // Define os ponteiros para os registradores de hardware
    ptr_A = (int *)(HW_virtual + DATA_A);
    ptr_B = (int *)(HW_virtual + DATA_B);
    ptr_START = (int *)(HW_virtual + START);
    ptr_FULL = (int *)(HW_virtual + WR_FULL);

    major_number = MAJOR(dev_num);
    printk(KERN_INFO "GPU: major number is %d", major_number);
    printk(KERN_INFO "\tuse \"mknod /dev/%s c %d 0\" for device file", DEVICE_NAME, major_number);

    mcdev = cdev_alloc();
    mcdev->ops = &fops;
    mcdev->owner = THIS_MODULE;

    // Adiciona o dispositivo de caractere ao kernel
    ret = cdev_add(mcdev, dev_num, 1);
    if (ret < 0)
    {
        printk(KERN_ALERT "GPU: unstruct file *filpable to add dev cdev to kernel");
        return ret;
    }
}

// Função de saída do módulo
static void __exit sair(void)
{
    iounmap(HW_virtual); // Desmapeia a memória
    cdev_del(mcdev); // Remove o dispositivo de caractere
    unregister_chrdev_region(dev_num, 1); // Desregistra o número do dispositivo
    printk(KERN_INFO "Fechando o modulo");
}

// Define as funções de inicialização e saída do módulo
module_init(iniciar);
module_exit(sair);
