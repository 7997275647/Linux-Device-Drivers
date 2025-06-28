#include <linux/module.h>
#include<linux/init.h>

static int __init hello_init(void){
	printk("Hello Kernal\n");
	return 0;
}

static void __exit hello_exit(void){
	printk("Good bye! Kernal");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sandeep");
MODULE_DESCRIPTION("Simple uesr written module");
