/*************************************************************************
	> File Name: auto_link.h
	> Author: zhz
	> Mail: zhz_aj@163.com 
	> Created Time: Wed 13 Apr 2016 11:57:24 AM CST
 ************************************************************************/

#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/slab.h>
#include <linux/kobject.h>
#include <linux/klist.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/major.h>
#include <linux/kprobes.h>




#define D(fmt,arg...)           printk("<<-auto-link-INFO->> "fmt"\n",##arg)
