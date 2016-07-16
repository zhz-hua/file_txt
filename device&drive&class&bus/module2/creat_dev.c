#include	<linux/kernel.h>
#include	<linux/module.h>
#include	<linux/device.h>
#include	<linux/cdev.h>
#include 	<linux/uaccess.h>
#include	<linux/interrupt.h>
#include	<linux/wait.h>
#include	<linux/init.h>
#include	<linux/fs.h>
#include 	<linux/kthread.h>
#include <linux/stat.h>
#include <linux/slab.h>
#include <linux/kobject.h>
#include <linux/klist.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/major.h>
#include <linux/kprobes.h>

#define D(fmt,arg...)           printk("<<-auto-link-INFO->> "fmt"\n",##arg)

static int major;
static struct cdev led_cdev;
static int status;
static struct class *led_cls;

static ssize_t led_read(struct file *file,
			char __user *buf,
			size_t count,
			loff_t *oops)
{
	copy_to_user(buf,&status,4);
	return count;
}
static ssize_t led_write(struct file *file,
			char __user *buf,
			size_t count,
			loff_t *ppos)
{
	int val;
	copy_from_user(&val,buf,4);
	if (val == 1){
		//gpio_set_value(num,1);
	}
	else {
		//gpio_set_value(num,0);
	}
	status = val;
	return count;
}
static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.read = led_read,
	.write = led_write
};



static int __init led_init(void)
{
	dev_t dev_id;
	if (major) {
		dev_id = MKDEV(major, 0);
		register_chrdev_region(dev_id, 1, "led");
	} else {
		alloc_chrdev_region(&dev_id, 0, 1, "led");
		major = MAJOR(dev_id);//提取主设备号
	}
	/*2.分配初始化 cdev*/
	cdev_init(&led_cdev, &led_fops);
	/*3.注册 cdev*/
	cdev_add(&led_cdev, dev_id, 1);
	/*4 自动创建设备节点*/
	/*4.1 创建设备类*/
	//sys/class/led
	led_cls = class_create(THIS_MODULE,"led");
	/*4.2 创建设备节点*/
	device_create(led_cls,NULL,dev_id,NULL,"myled");
	/*4.申请 GPIO 资源*/
	return 0;	
}
static void __exit led_exit(void)
{

	/*2.删除设备节点*/
	device_destroy(led_cls, MKDEV(major, 0));
	class_destroy(led_cls);
	/*3.删除 cdev*/
	cdev_del(&led_cdev);
	/*4.释放设备号*/
	unregister_chrdev_region(MKDEV(major, 0), 1);
}
module_init(led_init);
module_exit(led_exit);
MODULE_AUTHOR("<sam@cubietech.com>");
MODULE_DESCRIPTION("a simple example how to use gpio");
MODULE_LICENSE("GPL");
