/*
    Kernel module to create a secret key and give the address to userspace
*/
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/stat.h>
#include<linux/proc_fs.h>
#include<linux/mm.h>
#include<linux/vmalloc.h>
#include<linux/uaccess.h>
#include<linux/version.h>
#include<linux/seq_file.h>
//#include<emmintrin.h>
//#include<x86intrin.h>

static struct proc_dir_entry *key_proc;
char *secret_buffer;
volatile char *key_val;


//Correct method of accessing file data from an inode, depending on kernel version
static int open_proc(struct inode *inode, struct file *fp){
    #if LINUX_VERSION_CODE<=KERNEL_VERSION(4, 0, 0)
        return single_open(fp, NULL, PDE(inode)->data);
    #else
        return single_open(fp, NULL, PDE_DATA(inode));
    #endif
}


//copy the address of the secret data into user space
static ssize_t read_secret(struct file *filp, char *buffer, 
                            size_t length, loff_t *offset)
{
    memcpy(secret_buffer, key_val, strlen(key_val));
    return strlen(key_val);
}

// define file operations on the procfs entry
static const struct file_operations fops = {
    .owner= THIS_MODULE,
    .read= read_secret,
    .open= open_proc,
    .llseek= seq_lseek,
};


//initialization function
//key value assigned
//buffer space allocated 
static __init int initialize_proc(void){
    char *key_val = (char*)vmalloc(4);
    strcpy(key_val, "abcd");
    // secret_buffer = (char*)vmalloc(strlen(key_val));
    
    printk(KERN_INFO "Secret data address = %p\n", key_val);
    unsigned long kernel_addr = key_val;
    __builtin_prefetch(key_val);
    //_mm_prefetch((char*)kernel_addr, 3);
    printk(KERN_INFO "Value at %p = %s\n", key_val, key_val);
    char data = ((char*) kernel_addr)[0];
    printk(KERN_INFO "Data[0] = %c\n", data);
    // return 0;

//    printk("Address of pointer = %p\n", &key_val);
//	printk("Value at %p = %s\n", &key_val, &key_val);
    

    key_proc = proc_create_data("my_secret_key", 0444, NULL, &fops, NULL);
    if(key_proc)
        return 0;
    return -ENOMEM;
}

//Cleanup the file
//Remove the entry from procfs
static __exit void proc_cleanup(void){
    remove_proc_entry("my_secret_key", NULL);
    printk("Exit\n\n\n");
}

MODULE_LICENSE("GPL");
module_init(initialize_proc);
module_exit(proc_cleanup);

