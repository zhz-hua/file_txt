http://blog.csdn.net/angle_birds/article/details/16802099
 1 class结构体介绍
		内核中定义了struct class结构体，顾名思义，一个struct class结构体类型变量对应一个类，内核同时提供了class_create(…)函数，
	可以用它来创建一个类，这个类存放于sysfs下面，一旦创建好了这个类，再调用device_create(…)函数来在/dev目录下创建相应的设备节点。
	这样，加载模块的时候，用户空间中的udev会自动响应device_create(…)函数，去/sysfs下寻找对应的类从而创建设备节点。
 实际上，class_destroy()是通过调用class_unregister()实现的。
 关于struct device *device_create(struct class *class, struct device *parent,
                          dev_t devt, void *drvdata, const char*fmt, ...)

class_register(calss)   //调用前 必须手动分配额class的内存 。调用后必须设置class的name等参数
class_creat(owner,name)
void class_unregister(struct class *cls)
void class_destory(struct class *cls)
这个函数用于在一个类中创建一个虚拟设备，如果parent不为空，那么新创建的设备会是这个parent的字设备.创建的这个设备可以用来创建在sysfs中创建设备文件.

int device_create_file(struct device *dev,

                  const struct device_attribute *attr)

为dev在sysfs中创建一个设备文件

    #include<linux/module.h>  
    #include<linux/types.h>  
    #include<linux/device.h>  
    #include<linux/fs.h>  
    #include<linux/err.h>  
       
    #include"timed_output.h"  
       
    static structclass *timed_output_class;  
    static atomic_tdevice_count;  
       
    static ssize_t enable_show(structdevice *dev, struct device_attribute *attr,  
                 char *buf)  
    {  
        struct timed_output_dev *tdev =dev_get_drvdata(dev);  
        int remaining = tdev->get_time(tdev);  
       
        return sprintf(buf, "%d\n",remaining);  
    }  
       
    static ssize_t enable_store(  
                 struct device *dev, structdevice_attribute *attr,  
                 const char *buf, size_t size)  
    {  
        struct timed_output_dev *tdev =dev_get_drvdata(dev);  
        int value;  
       
        if (sscanf(buf, "%d", &value)!= 1)  
                 return -EINVAL;  
       
        tdev->enable(tdev, value);  
       
        return size;  
    }  
       
    static DEVICE_ATTR(enable,S_IRUGO | S_IWUSR, enable_show, enable_store);  
       
    static int create_timed_output_class(void)  
    {  
        if (!timed_output_class) {  
                 timed_output_class =class_create(THIS_MODULE, "timed_output");  
                 if (IS_ERR(timed_output_class))  
                           return PTR_ERR(timed_output_class);  
                 atomic_set(&device_count, 0);  
        }  
       
        return 0;  
    }  
       
    int timed_output_dev_register(struct timed_output_dev *tdev)  
    {  
        int ret;  
       
        if (!tdev || !tdev->name ||!tdev->enable || !tdev->get_time)  
                 return -EINVAL;  
       
        ret = create_timed_output_class();  
        if (ret < 0)  
                 return ret;  
       
        tdev->index =atomic_inc_return(&device_count);  
        tdev->dev =device_create(timed_output_class, NULL,  
                 MKDEV(0, tdev->index), NULL,tdev->name);  
        if (IS_ERR(tdev->dev))  
                 return PTR_ERR(tdev->dev);  
       
        ret = device_create_file(tdev->dev,&dev_attr_enable);   // 
        if (ret < 0)  
                 goto err_create_file;  
       
        dev_set_drvdata(tdev->dev, tdev);  
        tdev->state = 0;  
        return 0;  
       
    err_create_file:  
        device_destroy(timed_output_class, MKDEV(0,tdev->index));  
        printk(KERN_ERR "timed_output: Failedto register driver %s\n",  
                           tdev->name);  
       
        return ret;  
    }  
    EXPORT_SYMBOL_GPL(timed_output_dev_register);  
       
    voidtimed_output_dev_unregister(struct timed_output_dev *tdev)  
    {  
        device_remove_file(tdev->dev,&dev_attr_enable);  
        device_destroy(timed_output_class, MKDEV(0,tdev->index));  
        dev_set_drvdata(tdev->dev, NULL);  
    }  
    EXPORT_SYMBOL_GPL(timed_output_dev_unregister);  
       
    static int __init timed_output_init(void)  
    {  
        return create_timed_output_class();  
    }  
       
    static void __exittimed_output_exit(void)  
    {  
        class_destroy(timed_output_class);  
    }  
       
    module_init(timed_output_init);  
    module_exit(timed_output_exit);  
       
    MODULE_AUTHOR("MikeLockwood <lockwood@android.com>");  
    MODULE_DESCRIPTION("timedoutput class driver");  
    MODULE_LICENSE("GPL");  

\kernel\drivers\staging\android\timed_output.h
    #ifndef _LINUX_TIMED_OUTPUT_H  
    #define _LINUX_TIMED_OUTPUT_H  
       
    struct timed_output_dev {  
             constchar  *name;  
       
             /* enablethe output and set the timer */  
             void   (*enable)(struct timed_output_dev *sdev, inttimeout);  
       
             /*returns the current number of milliseconds remaining on the timer */  
             int              (*get_time)(struct timed_output_dev *sdev);        
             /*private data */  
             struct device       *dev;  
             int              index;  
             int              state;  
    };  
       
    extern int timed_output_dev_register(struct timed_output_dev *dev);  
    extern void timed_output_dev_unregister(struct timed_output_dev *dev);  
       
    #endif  
       

 其二调试口中 向设备文件中写数据.列如:

echo "1000">>/sys/class/timed_output/vibrator/enable          //震动1S中
