Probe方式（new style）
需要构建 i2c_driver
一 数据
#define GTP_I2C_NAME                "Goodix-TS"
 
	static const struct of_device_id goodix_match_table[] = {                                                                                                                
		 {.compatible = "goodix,gt9xx",},                                                                                                                                 
		 { },                                                                                                                                                             
	 };
	static const struct i2c_device_id goodix_ts_id[] = {
	    { GTP_I2C_NAME, 0 },
	    { }
	};

	static struct i2c_driver goodix_ts_driver = {   
	    .probe      = goodix_ts_probe,              
	    .remove     = goodix_ts_remove,             
	    .id_table   = goodix_ts_id,                 
	    .driver = {                                 
		.name     = GTP_I2C_NAME,               
		.owner    = THIS_MODULE,                
		.of_match_table = goodix_match_table,   
	    },                                          
	};                                              
                                                                                                                                                                      
二：注册i2c_driver
	static int __init goodix_ts_init(void)                          
	{                                                                                                                           
	    return i2c_add_driver(&goodix_ts_driver);                                              
	}                                                               

 	module_init(goodix_ts_init);  
	在测试i2c_driver过程中是 将driver注册到i2c_bus_typ上
	static const struct i2c_device_id *i2c_match_id(const struct i2c_device_id *id,
		                const struct i2c_client *client)                       
	{                                                                              
	    while (id->name[0]) {                                                      
		if (strcmp(client->name, id->name) == 0)                               
		    return id;                                                         
		id++;                                                                  
	    }                                                                          
	    return NULL;                                                               
	}  
  	可以看是是利用i2c_client的名字和额id_table中的名称做配备。本驱动是 
	static const struct i2c_device_id goodix_ts_id[] = {
	    { GTP_I2C_NAME, 0 },
	    { }
	};  
	在Adapter模式中.i2c_client是我们增加够造出来的，而现在的i2c_client是那来的 

	i2c_register_board_info(注册i2_client)

然后 配备match(device device_drive) 调用probe                                                                       

	static int goodix_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)     
	{                                                                                         
	}                                                                                         
	#define i2c_add_driver(driver)	i2c_register_driver(THIS_MODULE, driver)  
		                                        
	int i2c_register_driver(struct module *owner, struct i2c_driver *driver)                  
	{                                                                                         
	    int res;                                                                              
		                                                                                  
		               
	    if (unlikely(WARN_ON(!i2c_bus_type.p)))                                               
		return -EAGAIN;                                                                   
		                                                                                                      
	    driver->driver.owner = owner;                                                         
	    driver->driver.bus = &i2c_bus_type;                                                   
		                                                                                                                                                           
	    res = driver_register(&driver->driver);                                               
	    if (res)                                                                              
		return res;                                                                       
		                                                                                                                    
	    if (driver->suspend)                                                                  
		pr_warn("i2c-core: driver [%s] using legacy suspend method\n",                    
		    driver->driver.name);                                                         
	    if (driver->resume)                                                                   
		pr_warn("i2c-core: driver [%s] using legacy resume method\n",                     
		    driver->driver.name);                                                         
		                                                                                  
	    pr_debug("i2c-core: driver [%s] registered\n", driver->driver.name);                  
		                                                                                                                    
	    i2c_for_each_dev(driver, __process_new_driver);                                       
		                                                                                  
	    return 0;                                                                             
	}                                                                                         
	EXPORT_SYMBOL(i2c_register_driver);                                                       



下面讲解下newstyle方式的i2c设备驱动。



<一>定义并填充i2c_driver:

    staticconst struct i2c_device_id at24c08b_id[] = { 
        {"at24c08b", 0 }, //该i2c_driver所支持的i2c_client
        {} 
    }; 
    MODULE_DEVICE_TABLE(i2c,at24c08b_id); 
    /*定义并填充i2c_driver:
    *probe设备探测函数,i2c_add_driver()时会被调用 
    *remove设备卸载函数； 
    */
    staticstruct i2c_driver at24c08b_driver = { 
        .driver= { 
            .name= "at24c08b", 
           .owner = THIS_MODULE, 
        },
        .probe= at24c08b_probe, 
            .remove= __devexit_p(at24c08b_remove), 
            .id_table= at24c08b_id, 
    };




<二>模块初始化函数

    staticint __init at24c08b_init(void) 
    {  
        returni2c_add_driver(&at24c08b_driver); 
    }



分析i2c_add_driver():

        i2c_register_driver()
            driver->driver.bus= &i2c_bus_type;//设置i2c_driver的总线类型
            driver_register()//这个函数结束后就会调用probe()函数                                   
              i2c_for_each_dev(driver,__process_new_driver);//对每一个存在的i2c_adapter,调用__process_new_driver()函数                                    
                  i2c_do_add_adapter()
                    i2c_detect(adap,driver);
                        //我们的i2c_driver没设置address_list和detect()函数，所以到这里就返回了。
                        address_list= driver->address_list;
                        if(!driver->detect || !address_list)
                            return0;


分析driver_register()：

        driver_find()//i2c_driver是否已经被注册
        bus_add_driver()//将i2c_driver挂接到i2c总线i2c_bus_type上
            driver_attach()
                //对i2c总线上的每一个i2c设备i2c_client都会调用__driver_attach，这里的dev即i2c_client，drv即i2c_driver
                bus_for_each_dev(drv->bus,NULL, drv, __driver_attach);
                    driver_match_device(drv,dev)
                        //调用i2c总线i2c_bus_type的match函数
                        returndrv->bus->match ? drv->bus->match(dev, drv) : 1;
                            i2c_device_match()
                                //若i2c_client的名字和i2c_device_id的中名字相同，则匹配成功，才会调用后面的probe()
                                i2c_match_id(driver->id_table,client)
                    driver_probe_device()
                        really_probe()
                            //调用i2c总线i2c_bus_type的probe函数
                            dev->bus->probe(dev);
                                i2c_device_probe()
                                    //调用到i2c_driver的probe()函数
                                    driver->probe(client, i2c_match_id(driver->id_table, client))     



i2c总线i2c_bus_type的定义如下：

    structbus_type i2c_bus_type = {
        .name        ="i2c",
        .match        =i2c_device_match,
        .probe        =i2c_device_probe,
        .remove        =i2c_device_remove,
        .shutdown    =i2c_device_shutdown,
        ...
    };



<三>注册i2c设备相关信息
对于newstyle方式，需要通过i2c_register_board_info()函数注册i2c_board_info，向内核提供i2c设备的相关信息。
在arch/arm/mach-s3c2440/mach-mini2440.c添加如下代码：

    /*I2C设备at24c08b的相关信息*/
    staticstruct i2c_board_info i2c_devices[] __initdata = { 
        {I2C_BOARD_INFO("at24c08b", 0x50), }, //0x50是at24c08b的设备地址 
    };
    staticvoid __init mini2440_machine_init(void)
    {
        …
        i2c_register_board_info(0,i2c_devices,ARRAY_SIZE(i2c_devices));
    }



分析i2c_register_board_info()：

        structi2c_devinfo    *devinfo;//定义一个i2c_devinfo
            devinfo->board_info= *info;//保存i2c_board_info
                //将i2c_devinfo挂在链表__i2c_board_list上
                list_add_tail(&devinfo->list,&__i2c_board_list);



搜索__i2c_board_list可知：

    i2c_add_numbered_adapter()//i2c-s3c2410.c中调用该函数来注册一个i2c_adapter
        i2c_add_adapter()
            i2c_register_adapter()
                i2c_scan_static_board_info()
                    list_for_each_entry(devinfo,&__i2c_board_list, list) 
                        //利用i2c_adapter和i2c_board_info构造i2c_client
                        if(devinfo->busnum == adapter->nr
                        &&!i2c_new_device(adapter,&devinfo->board_info))
                            structi2c_client *client;
                            client->adapter= adap;//设置i2c_client的adapter
                            client->addr= info->addr;//设置设备地址
                            …//继续设置i2c_client
                            device_register()//将i2c设备i2c_client挂接到i2c总线上



分析device_register()：

        device_add()
            bus_add_device()
                //将设备挂接在总线上，对于i2c而言，即把i2c_client挂接到i2c_bus_type
                klist_add_tail(&dev->p->knode_bus,&bus->p->klist_devices);  


ps:上述这一套分析适用于所有符合总线设备驱动模型的驱动,如usb总线，平台总线，pci总线，i2c总线等

 1、如果是news style形式的,在注册adapter的时候,将它上面的i2c设备转换成了struct client。struct client->dev->bus又指定了和i2c driver同一个bus，因此它们可以发生probe.

 1、I2C协议的普通数据传输

        I2C协议普通数据传输的接口函数基本为i2c_master_send和i2c_master_recv，查看其函数发现，最后都是调用i2c_transfer函数实现传输的，i2c_transfer函数如下：

    int i2c_transfer(struct i2c_adapter * adap, struct i2c_msg *msgs, int num)
    {
        int ret;
        if (adap->algo->master_xfer) {
            if (in_atomic() || irqs_disabled()) {
                ret = mutex_trylock(&adap->bus_lock);
                if (!ret)
                    /* I2C activity is ongoing. */
                    return -EAGAIN;
            } else {
                mutex_lock_nested(&adap->bus_lock, adap->level);
            }
            ret = adap->algo->master_xfer(adap,msgs,num);		//传输工作交给了adap->algo->master_xfer()完成
            mutex_unlock(&adap->bus_lock);
            return ret;
        } else {
            dev_dbg(&adap->dev, "I2C level transfers not supported/n");
            return -ENOSYS;
        }
    }

        因为在这里的同步用的是mutex。首先判断是否允许睡眠，如果不允许，尝试获锁，如果获锁失败，则返回。这样的操作是避免进入睡眠，我们在后面也可以看到，实际的传输工作交给了adap->algo->master_xfer()完成，也就是我们在（一）中注册的algorithm中的i2c_gsc_func函数。 
