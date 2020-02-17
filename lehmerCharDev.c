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
    
    size_t ret = 0;
    uint64_t rng = lehmer64();

    if (copy_to_user(buf, &rng, sizeof(&rng))) {
        ret = -EFAULT;
    } else {
        ret = sizeof(&rng);
        *off += ret;
    }
    return ret;
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
