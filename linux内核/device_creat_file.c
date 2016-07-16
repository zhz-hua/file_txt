一：
	一般步  		//				/sys/class/timed_output/vibrator/enable
	creat_class		//timed_output   		创建timed_output
	device_create		//timed_output/vibrator		创建vibrator	
	device_creat_file	//timed_output/vibrator/enable	创建enable属性文件
二： int device_create_file(struct device *, struct device_attribute *);
	在/sys/devices/xxx/目录下创建device属性文件

	struct attribute {
		char                    * name;
		struct module  *owner;
		mode_t                  mode;
	};
	属性
    为了使对属性的读写变得有意义，一般将attribute结构嵌入到其他数据结构中。子系统通常都会定义自己的属性结构，并且提供添加和删除属性文件的包裹函数。
    例如，设备驱动模型为device子系统定义了相应的属性结构device_attribute：
	struct device_attribute {
	 struct attribute attr;
	 ssize_t (*show)(struct device *dev, struct device_attribute *attr,char *buf);
	 ssize_t (*store)(struct device *dev, struct device_attribute *attr,const char *buf, size_t count);
	};

    系统提供了一个宏方便定义device属性:

    #define DEVICE_ATTR(_name, _mode, _show, _store) /
        struct device_attribute dev_attr_##_name = __ATTR(_name, _mode, _show, _store)

    其中，__ATTR定义如下：

    #define __ATTR(_name,_mode,_show,_store) { /
     .attr = {.name = __stringify(_name), .mode = _mode }, /
     .show = _show,     /
     .store = _store,     /
    }
    例如，定义一个device属性，名为foo，读写该文件的方法分别为show_foo和store_foo：

    static DEVICE_ATTR(foo, S_IWUSR | S_IRUGO, show_foo, store_foo);

			    将宏展开为：   
			    static struct device_attribute dev_attr_foo = {
				    .attr = {
						  .name = "foo",
						  .mode = S_IWUSR | S_IRUGO,

					       },
				    .show = show_foo,
				    .store = store_foo,
			    };
ret = device_create_file(tdev->dev,&dev_attr_foo); 

 

子系统特定的回调函数

    当子系统定义一个新的属性类型时，必须实现一组sysfs操作，从而将文件的读写调用(read/write调用)重定向到属性拥有者的show和store方法。

 

    struct sysfs_ops {
        ssize_t (*show)(struct kobject *, struct attribute *, char *);
        ssize_t (*store)(struct kobject *, struct attribute *, const char *);
    };

 

    当读写一个文件时，sysfs将为此类型调用合适的方法，这些方法会将kobject结构和attribute结构转换为合适的指针类型，然后调用与之关 联的相关的方法。注意，子系统必须已经为此类型定义好了kobj_type作为此类型的描述符，因为sysfs_ops指针存储在kobj_type中。

    举个例子：  

    #define to_dev_attr(_attr) container_of(_attr, struct device_attribute, attr)
    #define to_dev(d) container_of(d, struct device, kobj)

    static ssize_t dev_attr_show(struct kobject * kobj, struct attribute * attr, char * buf)
    {
        struct device_attribute * dev_attr = to_dev_attr(attr);
        struct device * dev = to_dev(kobj);
        ssize_t ret = 0;

        if (dev_attr->show)
                ret = dev_attr->show(dev, buf);
        return ret;
    }

 

属性的读写

    为了读写属性，当定义属性时，必须指定show和/或者store方法：

ssize_t (*show)(struct device * dev, struct device_attribute * attr, char * buf);
ssize_t (*store)(struct device * dev, struct device_attribute * attr, const char * buf);



- 缓冲区的大小应总是为PAGE_SIZE个字节。在i386上，PAGE_SIZE=4096

- show方法应该返回放入缓冲区的字节数，即snprintf的返回值

- show方法应该总是使用snprintf

- store方法应该返回实际使用的字节数，可以使用strlen来得到

- show和/或者store方法可能会出错，所以当失败时，记得返回错误值

- The object passed to the methods will be pinned in memory via sysfs
  referencing counting its embedded object. However, the physical
  entity (e.g. device) the object represents may not be present. Be
  sure to have a way to check this, if necessary. （？？？）
    下面的代码展示了device属性的一个简单的实现：

    static ssize_t show_name(struct device *dev, struct device_attribute *attr, char *buf)
    {
         return snprintf(buf, PAGE_SIZE, "%s/n", dev->name);
    }

    static ssize_t store_name(struct device * dev, const char * buf)
    {
         sscanf(buf, "%20s", dev->name);
         return strnlen(buf, PAGE_SIZE);
    }

    static DEVICE_ATTR(name, S_IRUGO, show_name, store_name);

    注意，实际应用时，并不允许从用户空间设置设备的名字，这里仅举个例子。

 

    下面捡几个重要的目录进行说明：

    devices目录展现了系统中的设备树，它直接对应于内核中的设备树，即device的层次结构 ；

    bus目录下包含了若干目录，每个目录表示系统中的一个总线，且每个目录下又包含两个子目录：devices、drivers。其中devices子目录 下包含系统中发现的device的符号链接，这些符号链接分别指向/sys/devices/xxx目录下对应的目录；drivers子目录下包含特定总 线上的那些用于驱动每个设备的驱动程序的目录（可见，一个驱动程序只会出现在某一个特定的总线上）；

   

当前接口

    目前sysfs中存在以下接口：
- devices (include/linux/device.h)
    device属性：

    struct device_attribute {
     struct attribute attr;
     ssize_t (*show)(struct device *dev, struct device_attribute *attr, char *buf);
     ssize_t (*store)(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
    };

 

    属性的声明：

    DEVICE_ATTR(_name, _mode, _show, _store);

    属性的创建和移除：

    int device_create_file(struct device *device, struct device_attribute * attr);
    void device_remove_file(struct device * dev, struct device_attribute * attr);


- bus drivers (include/linux/device.h)
    bus属性：

    struct bus_attribute {
        struct attribute        attr;
        ssize_t (*show)(struct bus_type *, char * buf);
        ssize_t (*store)(struct bus_type *, const char * buf);
    };

    属性的声明：

    BUS_ATTR(_name, _mode, _show, _store)

    属性的创建和移除：

    int bus_create_file(struct bus_type *, struct bus_attribute *);
    void bus_remove_file(struct bus_type *, struct bus_attribute *);


- device drivers (include/linux/device.h)
    driver属性：

    struct driver_attribute {
        struct attribute        attr;
        ssize_t (*show)(struct device_driver *, char * buf);
        ssize_t (*store)(struct device_driver *, const char * buf,
                         size_t count);
    };

    属性的声明：

    DRIVER_ATTR(_name, _mode, _show, _store)

    属性的创建和移除：

    int driver_create_file(struct device_driver *, struct driver_attribute *);
    void driver_remove_file(struct device_driver *, struct driver_attribute *);

