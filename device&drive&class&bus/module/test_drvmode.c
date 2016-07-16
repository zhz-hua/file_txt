/*************************************************************************
	> File Name: test_drvmode.c
	> Author: zhz
	> Mail: zhz_aj@163.com 
	> Created Time: Tue 12 Apr 2016 04:01:51 PM CST
 ************************************************************************/

#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/init.h>
#define  DRVMODE_INFO(fmt,arg...)           printk("<<-DRVMODE-INFO->> "fmt"\n",##arg) 


static struct kobject *auto_link_kobj;

static int __init auto_link_drvmode_init(void)
{
	DRVMODE_INFO("auto_link drvmode_init");
auto_link_kobj = kobject_create_and_add("auto-link",NULL);
	if (!auto_link_kobj)
		DRVMODE_INFO("out of memory");
	return 0;
}

static void __exit  auto_link_drvmode_exit(void)
{
	DRVMODE_INFO("auto_link drvmode_exit");
	kobject_del(auto_link_kobj);
}
module_init(auto_link_drvmode_init);
module_exit(auto_link_drvmode_exit);

MODULE_AUTHOR("auto-link");
MODULE_DESCRIPTION("auto_link_drvmode");
MODULE_LICENSE("GPL");
