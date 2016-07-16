
说到sysfs接口，就不得不提到函数宏 DEVICE_ATTR原型是#define DEVICE_ATTR(_name, _mode, _show, _store) 

struct device_attribute dev_attr_##_name = __ATTR(_name, _mode, _show, _store)

函数宏DEVICE_ATTR内封装的是__ATTR(_name,_mode,_show,_stroe)方法，_show表示的是读方法，_stroe表示的是写方法。

当然_ATTR不是独生子女，他还有一系列的姊妹__ATTR_RO宏只有读方法，__ATTR_NULL等等

如对设备的使用  DEVICE_ATTR  ，对总线使用  BUS_ATTR  ，对驱动使用 DRIVER_ATTR  ，对类别 (class) 使用  CLASS_ATTR,  这四个高级的宏来自于 

DEVICE_ATTR  宏声明有四个参数，分别是名称、权限位、读函数、写函数。其中读函数和写函数是读写功能函数的函数名。

1、如果你完成了DEVICE_ATTR函数宏的填充，下面就需要创建接口了

例如：

    static	DEVICE_ATTR(polling, S_IRUGO | S_IWUSR, show_polling, set_polling);
    static struct attribute *dev_attrs[] = {
            &dev_attr_polling.attr,
            NULL,
    };

当你想要实现的接口名字是polling的时候，需要实现结构体struct attribute *dev_attrs[]

其中成员变量的名字必须是&dev_attr_polling.attr

2、然后再封装

    static struct	attribute_group dev_attr_grp = {
            .attrs = dev_attrs,
    };

3、再利用sysfs_create_group(&pdev->dev.kobj, &dev_attr_grp);创建接口

通过以上简单的三个步骤，就可以在adb shell 终端查看到接口了。 当我们将数据 echo 到接口中时，在上层实际上完成了一次 write 操作，对应到 kernel ，调用了驱动中的 “store”。
同理，当我 们cat 一个 接口时则会调用 “show” 。到这里，只是简单的建立了 android 层到 kernel 的桥梁，真正实现对硬件操作的，还是 在 "show" 和 "store" 中完成的。
其实呢？！用个proc文件系统的就知道，这个就喝proc中的write和read一样的，以我的理解：proc有点老了，以后肯定会大量使用attribute，proc好比是Windows XP，attribute就像是Windows Seven。

以下来个例子：


/*
 * Sample kobject implementation
 *
 * Copyright (C) 2004-2007 Greg Kroah-Hartman
 * Copyright (C) 2007 Novell Inc.
 *
 * Released under the GPL version 2 only.
 *
 */
#include
#include
#include
#include
#include
#include
#include

/*
 * This module shows how to create a simple subdirectory in sysfs called
 * /sys/kernel/kobject-example  In that directory, 3 files are created:
 * "foo", "baz", and "bar".  If an integer is written to these files, it can be
 * later read out of it.
 */

static int foo;

/*
 * The "foo" file where a static variable is read from and written to.
 */

static struct msm_gpio qup_i2c_gpios_io[] = {
     { GPIO_CFG(60, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
           "qup_scl" },
     { GPIO_CFG(61, 0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
           "qup_sda" },
     { GPIO_CFG(131, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
           "qup_scl" },
     { GPIO_CFG(132, 0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
           "qup_sda" },
};

static struct msm_gpio qup_i2c_gpios_hw[] = {
     { GPIO_CFG(60, 1, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
           "qup_scl" },
     { GPIO_CFG(61, 1, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
           "qup_sda" },
     { GPIO_CFG(131, 2, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
           "qup_scl" },
     { GPIO_CFG(132, 2, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
           "qup_sda" },
};

static void gsbi_qup_i2c_gpio_config(int adap_id, int config_type)
{
     int rc;

     if (adap_id < 0 || adap_id > 1)
           return;

     /* Each adapter gets 2 lines from the table */
     if (config_type)
           rc = msm_gpios_enable(&qup_i2c_gpios_hw[adap_id*2], 2);
     else
           rc = msm_gpios_enable(&qup_i2c_gpios_io[adap_id*2], 2);
     if (rc < 0)
           pr_err("QUP GPIO request/enable failed: %d\n", rc);
}

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
    char *buf)
{
      return sprintf(buf, "%d\n", foo);
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
      const char *buf, size_t count)
{
     int sda,scl;
     int i;
     sscanf(buf, "%du", &foo);
     printk("foo = %d.\n",foo);

     foo = foo -1;

     if  (foo < 0 || foo > 1)
     {
           printk("input foo error. foo=%d.\n",foo);
           return 0;
     }
           
      sda = GPIO_PIN((&qup_i2c_gpios_hw[foo*2+1])->gpio_cfg);
      scl = GPIO_PIN((&qup_i2c_gpios_hw[foo*2])->gpio_cfg);
      
     printk("sda = %d.\n",sda);
     printk("scl = %d.\n",scl);
     
     gsbi_qup_i2c_gpio_config(foo,0);

     for(i=0;i<9;i++)
     {
           if(gpio_get_value(sda))
           {
                printk("sda of i2c%d is high when %d pulse is output on scl.\n",foo,i);
                break;
           }
           gpio_set_value(scl,0);
           udelay(5);
           gpio_set_value(scl,1);
           udelay(5);
     }
     
     gsbi_qup_i2c_gpio_config(foo,1);

     printk("finish.\n");
     return count;
}

static struct kobj_attribute foo_attribute =
      __ATTR(i2c_unlock, 0666, foo_show, foo_store);

/*
 * Create a group of attributes so that we can create and destroy them all
 * at once.
 */
static struct attribute *attrs[] = {
      &foo_attribute.attr,
      NULL,»  /* need to NULL terminate the list of attributes */
};

/*
 * An unnamed attribute group will put all of the attributes directly in
 * the kobject directory.  If we specify a name, a subdirectory will be
 * created for the attributes with the directory being the name of the
 * attribute group.
 */
static struct attribute_group attr_group = {
      .attrs = attrs,
};

static struct kobject *example_kobj;

static int __init example_init(void)
{
     int retval;

     /*
      * Create a simple kobject with the name of "kobject_example",
      * located under /sys/kernel/
      *
      * As this is a simple directory, no uevent will be sent to
      * userspace.  That is why this function should not be used for
      * any type of dynamic kobjects, where the name and number are
      * not known ahead of time.
      */
     example_kobj = kobject_create_and_add("i2c_recovery", kernel_kobj);
     if (!example_kobj)
          return -ENOMEM;

     /* Create the files associated with this kobject */
     retval = sysfs_create_group(example_kobj, &attr_group);
     if (retval)
          kobject_put(example_kobj);

     return retval;
}

static void __exit example_exit(void)
{
     kobject_put(example_kobj);
}

module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wupeng");
