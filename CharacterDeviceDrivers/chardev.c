#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>         // file_operations
#include <linux/cdev.h>       // cdev utilities
#include <linux/uaccess.h>    // copy_to_user, copy_from_user
#include <linux/device.h>     // device_create

#define DEVICE_NAME "mychardev"
#define CLASS_NAME "mycharclass"
#define BUFFER_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ChatGPT");
MODULE_DESCRIPTION("A Simple Linux Character Device Driver");
MODULE_VERSION("0.1");

static int majorNumber;
static char kernelBuffer[BUFFER_SIZE] = {0};
static int deviceOpenCount = 0;

static struct class* charClass = NULL;
static struct device* charDevice = NULL;
static struct cdev my_cdev;

static int dev_open(struct inode* inodep, struct file* filep) {
    deviceOpenCount++;
    printk(KERN_INFO "mychardev: Device opened %d time(s)\n", deviceOpenCount);
    return 0;
}

static int dev_release(struct inode* inodep, struct file* filep) {
    printk(KERN_INFO "mychardev: Device successfully closed\n");
    return 0;
}

static ssize_t dev_read(struct file* filep, char __user* buffer, size_t len, loff_t* offset) {
    int bytes_to_read;
    if (*offset >= BUFFER_SIZE)
        return 0;  // EOF

    bytes_to_read = min((size_t)(BUFFER_SIZE - *offset), len);

    if (copy_to_user(buffer, kernelBuffer + *offset, bytes_to_read) != 0) {
        printk(KERN_WARNING "mychardev: Failed to send data to user\n");
        return -EFAULT;
    }

    *offset += bytes_to_read;

    printk(KERN_INFO "mychardev: Sent %d bytes to the user\n", bytes_to_read);
    return bytes_to_read;
}

static ssize_t dev_write(struct file* filep, const char __user* buffer, size_t len, loff_t* offset) {
    int bytes_to_write = min(len, (size_t)(BUFFER_SIZE - 1));

    if (copy_from_user(kernelBuffer, buffer, bytes_to_write) != 0) {
        printk(KERN_WARNING "mychardev: Failed to receive data from user\n");
        return -EFAULT;
    }

    kernelBuffer[bytes_to_write] = '\0';  // Null terminate
    printk(KERN_INFO "mychardev: Received %d bytes from the user\n", bytes_to_write);

    return bytes_to_write;
}

// File operations structure
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

static int __init mychardev_init(void) {
    dev_t dev_no;
    int ret;

    // Allocate a major number dynamically
    ret = alloc_chrdev_region(&dev_no, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        printk(KERN_ALERT "mychardev: Failed to allocate a major number\n");
        return ret;
    }

    majorNumber = MAJOR(dev_no);
    printk(KERN_INFO "mychardev: Registered correctly with major number %d\n", majorNumber);

    // Initialize cdev structure and add it to kernel
    cdev_init(&my_cdev, &fops);
    my_cdev.owner = THIS_MODULE;

    ret = cdev_add(&my_cdev, dev_no, 1);
    if (ret < 0) {
        unregister_chrdev_region(dev_no, 1);
        printk(KERN_ALERT "mychardev: Failed to add cdev\n");
        return ret;
    }

    // Register device class
    charClass = class_create(CLASS_NAME);
    if (IS_ERR(charClass)) {
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_no, 1);
        printk(KERN_ALERT "mychardev: Failed to register device class\n");
        return PTR_ERR(charClass);
    }

    // Create the device node in /dev
    charDevice = device_create(charClass, NULL, dev_no, NULL, DEVICE_NAME);
    if (IS_ERR(charDevice)) {
        class_destroy(charClass);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_no, 1);
        printk(KERN_ALERT "mychardev: Failed to create device\n");
        return PTR_ERR(charDevice);
    }

    printk(KERN_INFO "mychardev: Device class created correctly\n");
    return 0;
}

static void __exit mychardev_exit(void) {
    device_destroy(charClass, MKDEV(majorNumber, 0));
    class_unregister(charClass);
    class_destroy(charClass);
    cdev_del(&my_cdev);
    unregister_chrdev_region(MKDEV(majorNumber, 0), 1);
    printk(KERN_INFO "mychardev: Goodbye from the LKM!\n");
}

module_init(mychardev_init);
module_exit(mychardev_exit);
