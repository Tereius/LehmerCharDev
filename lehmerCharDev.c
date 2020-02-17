#include <linux/errno.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/random.h>
#include "lehmer64.h"

#define NAME "lehmer_character_device"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Björn Stresing");
MODULE_DESCRIPTION("A character device that generates pseudo numbers using lehmer generator. This is very fast.");

static int major;

static ssize_t read(struct file *filp, char __user *buf, size_t len, loff_t *off) {
    
    size_t written = 0;
    
    while(written < len) {
        size_t ret = 0;
        char ring[2048];
        size_t size = min(sizeof(ring) - sizeof(uint64_t) + 1, len - written);
        
        while(ret < size) {
            uint64_t rng = lehmer64();
            memcpy(ring + ret, &rng, sizeof(rng));
            ret += sizeof(rng);
        }
        
        ret = min(ret, len - written);
        
        if (copy_to_user(buf + written, ring, ret)) {
            written = -EFAULT;
            break;
        } else {
            *off += ret;
        }
        written += ret;
    }

    return written;
}

static const struct file_operations fops = {
    
    .owner = THIS_MODULE,
    .read = read,
};

static int myinit(void) {
    
    lehmer64_seed(get_random_u64());
    major = register_chrdev(0, NAME, &fops);
    return 0;
}

static void myexit(void) {
    
    unregister_chrdev(major, NAME);
}

module_init(myinit);
module_exit(myexit);
