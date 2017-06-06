#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/tty.h>
#include <linux/version.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/stat.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define MAX_SIZE 100
/**
 * define semaphores
 */
static DEFINE_SEMAPHORE(semVarRead); 
static DEFINE_SEMAPHORE(semVarWrite); 

/**
 * Defining mutex varibale to beused for locking
 */
static DEFINE_MUTEX(mutVar);

/**************************************************************************
 * Module parameters declared globally
 **************************************************************************
 * Module paramter : should be inserted at the time of inserting the module
 * into the system dynamically. And it must be an integer. 
 * It produces an error on non-integer input.
 */
static int N = 256;                                                                     

/**
 * including mechanism that accpets the module paramter N
 */
module_param(N, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(N, "This defines the size of the buffer used by linepipe device.");

int cnt = 0;
int subscript = 0;

int writeCount = 0;
int readCount = 0;

/**
 * defining structure that is used for storing the buffer
 */
typedef struct
{
    char arr[MAX_SIZE]; 
} CharBuff;

/**
 * Structure object as pointer to the structure
 */
CharBuff *queue; 

/**
 * Defining device functions 
 */
static int dev_Open(struct inode *, struct file *);
static ssize_t dev_Read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_Write(struct file *, const char *, size_t, loff_t *);
static int dev_Close(struct inode *, struct file *);

static struct file_operations fops = {
    .read = dev_Read,
    .write = dev_Write,
    .open = dev_Open,
    .release = dev_Close,
};

static struct miscdevice linepipe = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "linepipe",
    .fops = &fops,
};

/**
 * dev_Open is used to open the function with the valid mode.
 */
static int dev_Open(struct inode *i, struct file *fp)
{
    return 0;
}

int ret = 0;
int i = 0;

/**
 * dev_Read() function is used for reading from the device. 
 * This function will be accessed by the consumer via the system call called
 * copy_to_user() which helps to copy data from kernel space into the user
 * program space
 */
static ssize_t dev_Read(struct file *fp, char *out, size_t size, loff_t *off)
{
    size_t msgLength;
    /**
     * blocking the read operation by making semaphore down
     */
    down_interruptible(&semVarWrite);
    /**
     * acquiring the lock
     */
    mutex_lock_interruptible(&mutVar);

    /**
     * Entering into the critical section
     */
    ret = copy_to_user(out, queue[readCount].arr, MAX_SIZE);
    msgLength = strlen(queue[readCount].arr);
    if(readCount < N)
    {
        readCount++;
        if(readCount == N)
        {
            readCount = 0;
        } 
    }
    /**
     * exiting the critical section
     */

    /**
     * Realising the lock
     */
    mutex_unlock(&mutVar);
    /**
     * Making the semaphore up
     */
    up(&semVarRead);
    return msgLength; 
}

/**
 * dev_Write() fuction is used for writing into the device.
 * This function will be used by producers via the system call
 * copy_from_user which enables the user application to send data or
 * write data into the kernel space from user space.
 */
static ssize_t dev_Write(struct file *fp, const char *out, size_t size, loff_t *off)
{
    char data[MAX_SIZE];

    /**
     * Making sepahore down for write operation
     */
    down_interruptible(&semVarRead);

    /**
     * Acquiring the lock varible of mutex
     */
    mutex_lock_interruptible(&mutVar);

    /**
     * Entering the crtical section
     */
    ret = 0;
    ret = copy_from_user(data, out, MAX_SIZE); 
    if(writeCount < N)
    {	
        strcpy(queue[writeCount].arr, data);
        writeCount++;
        if(writeCount == N)
        {
            writeCount = 0;	
        }
    }
    /**
     * Exiting the critical section
     */

    /**
     * Realising the lock variable
     */
    mutex_unlock(&mutVar);
    
    /**
     * Making the semaphore variable up
     */
    up(&semVarWrite);
    return strlen(out);
}

/**
 * Close the device after the use is done
 */
static int dev_Close(struct inode *inod, struct file *fp)
{
    printk(KERN_ALERT"\nDevice closed successfully!");
    return 0;
}

/**
 * Initializing the device
 */
static int __init init_func(void)
{
    printk(KERN_ALERT "You entered buffer size = %i an integer.\n", N);    
    sema_init(&semVarRead, N);
    sema_init(&semVarWrite, 0);
    mutex_init(&mutVar);
    queue = kmalloc(sizeof(CharBuff *) * N, GFP_KERNEL);
    misc_register(&linepipe);
    return 0;
}

/**
 * Exit the kernle module
 */
static void __exit exit_func(void)
{
    kfree(queue);
    misc_deregister(&linepipe);
}

module_init(init_func);
module_exit(exit_func);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("anirtek");
