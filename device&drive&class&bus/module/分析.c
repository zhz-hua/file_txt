个人理解

busybox find ./ -name "auto-link"
./sys/bus/autolink/devices/auto-link
./sys/bus/autolink/drivers/auto-link
./sys/bus/autolink/drivers/auto-link/auto-link
./sys/devices/virtual/auto-class/auto-device/auto-link
./dev/auto-link
./sys/devices/virtual/auto-class/auto-device
./sys/class/auto-class/auto-device
/sys/class/auto-class/auto-device/auto-link


分析  类 总线 设备 驱动

1：auto_link_class_register();
	busybox find ./ -name "auto-class"                            
		./sys/devices/virtual/auto-class		//auto_link_class=class_create(THIS_MODULE,"auto-class"); 
		./sys/class/auto-class     
2：auto_link_bus_register
	struct device auto_link_bus = {
		.init_name = "auto-device",
		.release = auto_link_release,
	};
	auto_link_bus.class = auto_link_class;         		//绑定在设备在auto-class目录下
	busybox find ./ -name "auto-device"                     //device_register(&auto_link_bus)     
		./sys/devices/virtual/auto-class/auto-device     
		./sys/class/auto-class/auto-device   
	struct bus_type auto_link_bus_type = {
		.name	=	"autolink",
		.match	=	auto_link_bus_match,
	};

	busybox find ./ -name "autolink"            	         //bus_register(&auto_link_bus_type);    autolink                 
		./sys/bus/autolink
	static struct device auto_link_dev = {
		.init_name = "auto-link",
		.release = auto_link_release,
	};
	dev->devt = MKDEV(AUTO_LINK_MAJOR,1);			//指定设备号
	dev->bus = &auto_link_bus_type;	                        //指定总线 autolink
	dev->parent = &auto_link_bus                            //指定父   auto-device  感觉可以无

3：auto_link_device_register(&auto_link_dev);			//device_register(dev); 
	 busybox find ./ -name "auto-link"
		./sys/bus/autolink/devices/auto-link
		./sys/devices/virtual/auto-class/auto-device/auto-link
		./dev/auto-link
	static struct device_driver  auto_link_drv = {
			.name = "auto-link",
	};
4： auto_link_driver_register(&auto_link_drv);
	drv->bus = &auto_link_bus_type;   	   		//指定总线autolink
	drv->owner = THIS_MODULE;		   		//设置驱动函数
	drv->probe = auto_link_probe;
	drv->remove = auto_link_remove

	 busybox find ./ -name "auto-link"
		./sys/bus/autolink/drivers/auto-link
		./sys/bus/autolink/drivers/auto-link/auto-link
		./sys/devices/virtual/auto-class/auto-device/auto-link




