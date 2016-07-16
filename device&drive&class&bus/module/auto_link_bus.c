/*************************************************************************
	> File Name: auto_link_bus.c
	> Author: zhz
	> Mail: zhz_aj@163.com 
	> Created Time: Wed 13 Apr 2016 12:01:55 PM CST
 ************************************************************************/

#include "auto_link.h"
#define AUTO_LINK_MAJOR 8
static int auto_link_bus_match(struct device *dev,struct device_driver *drv);
/*	
typedef struct __auto_link_device_driver{
		char * name;
		struct device_driver driver;
}auto_link_device_driver;
static int auto_link_driver_register(auto_link_device_driver *auto_link_driver)
{
	auto_link_driver->driver.name = auto_link_driver->name;
	auto_link_driver->driver.bus =&auto_link_bus_type;
	return driver_register(&auto_link_driver->driver);
}


static void auto_link_driver_unregister(auto_link_device_driver *auto_link_driver)  
{
	driver_unregister(&auto_link_driver->driver);
}
*/
void auto_link_release(struct device *dev)
{
	D("atuo-link:dev:%s is release",dev->kobj.name);
}


struct bus_type auto_link_bus_type = {
	.name	=	"autolink",
	.match	=	auto_link_bus_match,
};

struct device auto_link_bus = {
	.init_name = "auto-device",
	.release = auto_link_release,
};


static struct class *auto_link_class;
static int auto_link_class_register(void)
{
	auto_link_class = class_create(THIS_MODULE,"auto-class");
	if(IS_ERR(auto_link_class)){
		D("auto-link:class creat error");
		return PTR_ERR(auto_link_class ); 
	}
		D("auto-link :class creat success");
	return 0;
}
static void auto_link_class_unregister(void)
{
	class_destroy(auto_link_class);
	D("auto-link clsaa destroy");
}
static int auto_link_bus_register(void)
{
	int ret = -1;
	auto_link_bus.class = auto_link_class;
	ret = device_register(&auto_link_bus);
	D("auto-link_bus :device creat success");
	if (ret) goto device_error;
	ret = bus_register(&auto_link_bus_type);
	D("auto-link :bus creat success");
	if(ret) goto bus_error;
	return ret;
bus_error:
	bus_unregister(&auto_link_bus_type);
device_error:
	device_unregister(&auto_link_bus);
	return ret;
}
static void auto_link_bus_unregister(void)
{
	bus_unregister(&auto_link_bus_type);
	D("auto-link_bus :bus uninstall");
	device_unregister(&auto_link_bus);
	D("auto-link device uninstall");
}



static struct device auto_link_dev = {
	.init_name = "auto-link",
	.release = auto_link_release,
};

static struct device_driver  auto_link_drv = {
		.name = "auto-link",
};

static int auto_link_bus_match(struct device *dev,struct device_driver *drv)
{
	int ret = -1;
	ret = strncmp(dev->kobj.name,drv->name,strlen(drv->name));
	D("auto-link:dev-kobj.name:%s drv-name:%s ret:%d",dev->kobj.name,drv->name,ret);
	return !ret;
}


static int auto_link_device_register(struct device *dev)
{
	dev->devt = MKDEV(AUTO_LINK_MAJOR,1);
	dev->bus = &auto_link_bus_type;
	dev->parent = &auto_link_bus;
	return device_register(dev);

}
static void auto_link_device_unregister(struct device *dev) 
{
	 D("auto-link:dev-kobj.name:%s unstall ",dev->kobj.name);
	device_unregister(dev);
}

static int auto_link_probe(struct device *dev)
{
	D("device and dirver match success and install");
	return 0;
}

static int auto_link_remove(struct device *dev)
{	
	D( "deivce and dirvier uninstall");
	return 0;
}

static int auto_link_driver_register(struct device_driver *drv)
{
	drv->bus = &auto_link_bus_type;
	drv->owner = THIS_MODULE;
	drv->probe = auto_link_probe;
	drv->remove = auto_link_remove;
	return driver_register(drv);
}

static void auto_link_driver_unregister(struct device_driver *drv)
{
	D("auto-link:drv->name:%s unstall" ,drv->name);
	driver_unregister(drv);
}
static int __init auto_link_bus_init(void)
{
		int ret = -1;
		ret = auto_link_class_register();
		if(ret) goto class_error;
		ret = auto_link_bus_register();
		if(ret) goto bus_error;
		ret = auto_link_device_register(&auto_link_dev);
		if(ret) goto device_error;
		D("auto-link:device install"); 
		ret = auto_link_driver_register(&auto_link_drv);
		if(ret) goto driver_error;
		D("auto-link:driver install "); 
		return ret;
driver_error:
		auto_link_driver_unregister(&auto_link_drv);
device_error:
		auto_link_device_unregister(&auto_link_dev);
bus_error:
		auto_link_bus_unregister();
class_error:
		auto_link_class_unregister();
		return ret;

}

static void __exit auto_link_bus_exit(void)
{
		D("auto-link driver unregister");
		auto_link_driver_unregister(&auto_link_drv);
		auto_link_device_unregister(&auto_link_dev);
		auto_link_bus_unregister();
		auto_link_class_unregister();
		D("auto-link bus unregister");
}


module_init(auto_link_bus_init);
module_exit(auto_link_bus_exit);
MODULE_AUTHOR("zhanhuazhong");
MODULE_DESCRIPTION("auto-link bus test");
MODULE_LICENSE("GPL");
