/**
 * Seconds.c
 *
 * 
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/hash.h>
#include <linux/gcd.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/delay.h>

#define BUFFER_SIZE 128
#define PROC_NAME "seconds"
#define MESSAGE "Hello World\n"
/**
 * Function prototypes
 */
ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
        .owner = THIS_MODULE,
        .read = proc_read,
};

unsigned long timeInit, timeExit, timeTot; //Used to determine seconds since kernel was loaded

/* This function is called when the module is loaded. */
int proc_init(void) {
        // creates the /proc/seconds entry
        proc_create(PROC_NAME, 0, NULL, &proc_ops);
        printk( KERN_INFO "/proc/%s created\n", PROC_NAME);
        timeInit = jiffies/HZ;
        printk( KERN_INFO "Jiffies/HZ at start: %lu\n", timeInit);
        mdelay(1000); //Delay program by one second to demonstrate time difference
	return 0;
}

/* This function is called when the module is removed. */
void proc_exit(void) {
        
        remove_proc_entry(PROC_NAME, NULL);
        printk( KERN_INFO "/proc/%s removed\n", PROC_NAME);
        timeExit = jiffies/HZ;
        printk( KERN_INFO "Jiffies/HZ at removal: %lu\n", timeExit);
        timeTot = timeExit - timeInit;
        printk( KERN_INFO "Seconds elapsed between intervals: %lu\n", timeTot);
}

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos) {
        int rv = 0;
        char buffer[BUFFER_SIZE];
        static int completed = 0;
        
        if (completed) {
                completed = 0;
                return 0;
        }
        
        completed = 1;
        rv = sprintf(buffer, "Hello World\n");
        // copies the contents of buffer to userspace usr_buf
        copy_to_user(usr_buf, buffer, rv);

        return rv;
}


/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello Module");
MODULE_AUTHOR("SGG");