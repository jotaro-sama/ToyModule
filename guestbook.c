#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/moduleparam.h>
#include <linux/device.h>
#include "guestbook.h"

#define DEVICE_NAME "guestbook"
#define MESS_SIZE  1024
#define MAX_MESSAGES 512

static char *hotel_name = "La Volpe";
module_param(hotel_name, charp, S_IRUGO);

static int dev_open(struct inode*, struct file*);
static int dev_release(struct inode*, struct file*);
static ssize_t dev_read(struct file*, char*, size_t, loff_t*);
static ssize_t dev_write(struct file*, const char*, size_t, loff_t*);
static long dev_ioctl(struct file*, unsigned int, unsigned long);

static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
    .unlocked_ioctl = dev_ioctl,
};

static size_t current_messages;
static char** messages;
static int to_read;

struct class *dev_class;
struct device *dev;

static char *devnode_helper(struct device *dev, umode_t *mode) {
    if (!mode) {
        return NULL;
    }
    if (dev->devt == MKDEV(MAJOR_NUM, 0)) {
        *mode = 0666;
    }
    return NULL;
}

static int __init guestbook_init(void) {
    current_messages = 0;
    to_read = 0;
    messages = (char**) kmalloc(sizeof(char*)*MAX_MESSAGES, GFP_KERNEL);

    int status = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);
    if(status < 0) {
        printk(KERN_ALERT "Guestbook loading for hotel \"%s\" unsuccessful.\n", hotel_name);
        return MAJOR_NUM;
    }    

    dev_class = class_create(THIS_MODULE, "guestbook_class");
    dev_class->devnode = devnode_helper;
    dev = device_create(dev_class, NULL, MKDEV(MAJOR_NUM, 0), NULL, DEV_FILE_NAME);
    if (IS_ERR(dev)) {
        class_destroy(dev_class);
        unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
        printk(KERN_ALERT "Guestbook device creation for hotel \"%s\" unsuccessful.\n", hotel_name);
    }

    printk(KERN_INFO "Guestbook module for hotel \"%s\" successfully loaded. MAJOR_NUM number: %d\n", hotel_name, MAJOR_NUM);
    return 0;
}

static void __exit guestbook_exit(void) {
    device_destroy(dev_class, dev->devt);
    class_destroy(dev_class);
    unregister_chrdev(MAJOR_NUM, DEVICE_NAME);

    size_t i;
    for (i=0; i<current_messages; i++) {
        kfree(messages[i]);
    }
    kfree(messages);

    printk(KERN_INFO "Guestbook module for hotel \"%s\" unloaded.\n", hotel_name);
}

static int dev_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "Guestbook device opened.\n");
    return 0;
}

// Allows the user to leave his message on the guestbook
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
    if (current_messages == MAX_MESSAGES) {
        printk(KERN_ALERT "Sorry, the guestbook module is full! No more space left for messages.\n");
        return -ENOMEM;
    }
    else if (len > MESS_SIZE) {
        printk(KERN_ALERT "Message too long, write operation on guestbook failed!\n");
        return -EINVAL;
    }
    messages[current_messages] = (char*) kmalloc(sizeof(char)*len, GFP_KERNEL);
    copy_from_user(messages[current_messages], buffer, len);
    size_t last_mess = current_messages;
    current_messages += 1;
    

    printk(KERN_INFO "Guest message #%lu has been written. It says: %s.\n", last_mess, messages[last_mess]);
    return len;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "Guestbook device closed.\n");
    return 0;
}

// Output last message on the guestbook
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    unsigned long uncopied = 0;

    size_t mess_to_read = 0;
    if (to_read == 0) {
        mess_to_read = current_messages-1;
    }
    else {
        // to_read starts from 1 
        mess_to_read = to_read-1;
    }

    if(current_messages == 0) {
        printk(KERN_INFO "Guestbook still empty! Write something to it before.\n");
        return -ENOMSG;
    }
    else if (mess_to_read >= current_messages) {
        printk(KERN_INFO "The to_read param has been set to a position greater than the latest message. Could not read the message.\n");
        return -ENOMSG;
    }

    size_t output_mess_len = strlen(messages[mess_to_read]);

    if (output_mess_len > len) {
        output_mess_len = len;
    }

    printk(KERN_INFO "Copying to user...\n");
    uncopied = copy_to_user(buffer, messages[mess_to_read], output_mess_len);

    if (uncopied != 0) {
        printk(KERN_ALERT "The message could not be read correctly. %lu bytes not copied.\n", uncopied);
        return -EFAULT;
    }

    return output_mess_len; 
}

static long dev_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param) {
    switch (ioctl_num) {
        case IOCTL_CHANGE_MESS_TO_READ:
            to_read = (size_t) ioctl_param;
            return 0;

        case IOCTL_CLEAR_GUESTBOOK:
            *((size_t*)ioctl_param) = current_messages;
            current_messages = 0;
            return (long) *((size_t*)ioctl_param);
    }
    printk(KERN_ALERT "Unrecognized ioctl operation.\n");
    return -EFAULT;
}

module_init(guestbook_init);
module_exit(guestbook_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Giovanni De Luca <https://github.com/jotaro-sama>");
MODULE_DESCRIPTION("Guestbook module. Leave a message writing to the device, and read the latest message reading from the device.");
