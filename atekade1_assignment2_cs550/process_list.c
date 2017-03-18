/***************************************************************************
 * This file contains the code for generating kernel module called
 * "process_list". This module gets automatically compiled using the provide
 * Makefile. The 'make' command should be able to generate the "process_list.ko"
 * kernel module that can be inserted at rutime and dynamically using "sudo
 * insmod process_list.ko" command.
 *
 * This module can be run and tested from user space since the user_space.c
 * program; which can be compiled and run using the same Makefile provided for
 * kernel module, has ability to make open(), read() and close() call on the
 * module. 
 *
 * These functions calls are limited to the user space and does not enter into
 * the kernel space. instead, the kernel module returns its data into the user
 * space.
 *
 * Author : Aniruddha Tekade <atekade1@binghamton.edu>
 * Data of Creation : March 16, 2017.
 ****************************************************************************
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/tty.h>
#include <linux/version.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

static int times = 0;

struct task_struct *iter;
int cnt = 0;
int subscript = 0;
/**
 * Structure for storing the data of every process
 */ 
struct process
{
	int pid;
	int ppid;
	int cpu;
	int state; 
};

struct process data;

/**
 * Function declarations to be used later for opening and using the device
 */
static int dev_Open(struct inode *, struct file *);
static ssize_t dev_Read(struct file *, char *, size_t, loff_t *);
static int dev_Close(struct inode *, struct file *);

/**
 * File operation structure
 */
static struct file_operations fops = {
    .read = dev_Read,
    .open = dev_Open,
    .release = dev_Close,
};

/**
 * process_list device structure that defines characteristics of the device
 */ 
static struct miscdevice process_list = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "process_list",
	.fops = &fops,
};

/**
 * Array to be used for storing processe structures dynamically
 */
struct process *procArray;                                                         

/**
 * Definition of Open call 
 */
static int dev_Open(struct inode *i, struct file *fp)
{
 
    /**
     * Notifies to the user program about device started or not
     */
	printk(KERN_ALERT"Device opened %d times\n", times);      

    /**
     * Gets total number of processes that are running at present
     */
	for_each_process(iter)                                                          
	{
		cnt++;
	}

	/**
	 * Allocating memory for array of structures
	 */
	procArray = kmalloc(sizeof(struct process)*cnt, GFP_KERNEL);
	for_each_process(iter)
	{
		/**
		 * Allocate new process structure into the array & set all its fields
		 */
		procArray[subscript].pid = iter->pid;
		procArray[subscript].ppid = iter->parent->pid;
		procArray[subscript].cpu = task_cpu(iter);
		procArray[subscript].state = iter->state;
		printk("Subscript = %d", subscript);
		subscript++;
	}
	return 0;
}

int ret = 0;
int i = 0;

/**
 * Definition of Read call
 */
static ssize_t dev_Read(struct file *fp, char *out, size_t size, loff_t *off)
{
	if(i < cnt)
	{
		data.pid = procArray[i].pid;
		data.ppid = procArray[i].ppid;
		data.cpu = procArray[i].cpu;
		data.state = procArray[i].state;

		ret = copy_to_user(out, &data, sizeof(struct process));
		if(ret == 0)
		{
			ret = 0;
			i++;
		} else ret = EFAULT;
		
	} else ret = -1;
	return ret;
}

/**
 * Definition of close call
 */
static int dev_Close(struct inode *inod, struct file *fp)
{
	printk(KERN_ALERT"\nDevice closed successfully!");
	return 0;
}

/**
 * Module init() function definition
 */
static int __init init_func(void)
{
	misc_register(&process_list);
	return 0;
}

/**
 * Module exit() function definition
 */
static void __exit exit_func(void)
{
	misc_deregister(&process_list);
}

module_init(init_func);
module_exit(exit_func);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("anirtek");
