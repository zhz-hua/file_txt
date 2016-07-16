一 ：建立一个  /sys/class/timed_output/vibrator/enable  的节点


	
	#include <linux/hrtimer.h>  
	#include <linux/err.h>  
	#include <linux/gpio.h>  
	#include <linux/wakelock.h>  
	#include <linux/mutex.h>  
	#include <linux/clk.h>  
	#include <linux/workqueue.h>  
	#include <asm/mach-types.h>  
	#include <linux/kernel.h>  
	#include <linux/module.h>  
	#include<../drivers/staging/android/timed_output.h>  
	#define THE_DEVICE"/sys/class/timed_output/vibrator/enable" 

	#define IMX_GPIO_NR(bank, nr)             (((bank) - 1) * 32 + (nr))        //IO定义  
	#define SABRESD_VIBRATOR_CTL                   IMX_GPIO_NR(4, 17)   //电机通过P4.17脚控制 高打开 低关闭  
	#define MAX_TIMEOUT        10000/* 10s */  //最长可打开10s  

	static struct	vibrator {  
	     struct	wake_lock wklock;      		//wake_lock 防止震动过程中系统休眠,线程不释放此设备,造成不必要错误  
	     struct	hrtimer timer;    		//高精度定时器  
	     struct	mutex lock;       		//互斥锁,防止多线程同时访问这个设备.  
	     struct	work_struct work; 		//设备操作队列,用于一次操作完成和下一次开始同步用 (三星这么用的，具体为什么不直接用回调函数，我也不懂,还望大神们私信给个说明 感激不尽)  
	} vibdata;  

	static void mx6_vibrator_off(void)  
	{  

	     gpio_direction_output(SABRESD_VIBRATOR_CTL,0);         
	     wake_unlock(&vibdata.wklock);              //震动关闭就可以释放 wake_lock锁  
	      
	}  
	void mx6_motor_enable(struct timed_output_dev *sdev,int value)  
	{  
	     mutex_lock(&vibdata.lock);                     //关键代码段,同一时间只允许一个线程执行  
	      
	     /* cancelprevious timer and set GPIO according to value */  
	     hrtimer_cancel(&vibdata.timer);            //当先前定时器完成后 关闭这个定时器  
	     cancel_work_sync(&vibdata.work);         //当上次震动完成后 关闭这次动作  
	     if(value)  
	     {  
		       wake_lock(&vibdata.wklock);         //开始震动打开wake lock锁不允许休眠  
		       gpio_direction_output(SABRESD_VIBRATOR_CTL,1);  
	      
		       if(value > 0)  
		       {  
		                if(value > MAX_TIMEOUT)  
		                         value= MAX_TIMEOUT;  
		                value+= 45;                                    //为了使震动变得明显,固定增加一个时间.跟硬件有关系  
		                hrtimer_start(&vibdata.timer,                 //开始定时器  
		                         ns_to_ktime((u64)value* NSEC_PER_MSEC),  
		                         HRTIMER_MODE_REL);  
		       }  
	     }  
	     else  
		       mx6_vibrator_off();  

	     mutex_unlock(&vibdata.lock);                 //关键代码段执行完成,释放互斥锁  


	}  
	int     mx6_get_time(structtimed_output_dev *sdev)  
	{  
	     if(hrtimer_active(&vibdata.timer))  
	     {  
		       ktime_tr = hrtimer_get_remaining(&vibdata.timer);                 //读取剩余时间按并返回  
		       returnktime_to_ms(r);  
	     }  
	      
	     return 0;  
	}  
	struct timed_output_dev mx6_motot_driver={  
			.name ="vibrator",                                  	//注意这个名字,由于HAL层里面的设备为//"/sys/class/timed_output/vibrator/enable"  
		                                                     		//因此这个名字必须为"vibrator"  
		       .enable= mx6_motor_enable,  
		       .get_time= mx6_get_time,  
	};  

	static enum hrtimer_restart mx6_vibrator_timer_func(struct hrtimer * timer) //定时器结束时候的回调函数  
	{  
	     schedule_work(&vibdata.work);              //定时器完成了 执行work队列回调函数来关闭电机  
	     returnHRTIMER_NORESTART;  
	}  
	static void mx6_vibrator_work(struct work_struct *work)//工作队列处理函数,当工作队列执行 当  
	//schedule_work时候执行  
	{  
	     mx6_vibrator_off();  
	}  


	void __init mx6_motor_init()  
	{  
	     int ret =0;  
	     hrtimer_init(&vibdata.timer,CLOCK_MONOTONIC, HRTIMER_MODE_REL);//初始化定时器  
	     vibdata.timer.function= mx6_vibrator_timer_func;           //设置回调函数  
	     INIT_WORK(&vibdata.work,mx6_vibrator_work);    //初始化工作队列  
	     ret =gpio_request(SABRESD_VIBRATOR_CTL, "vibrator-en");    //申请IO  
	     if (ret< 0)  
	     {  
		       printk("vibratorrequest IO err!:%d\n",ret);  
		       returnret;  
	     }  
	     wake_lock_init(&vibdata.wklock,WAKE_LOCK_SUSPEND, "vibrator"); //初始化 wake_lock  
	     mutex_init(&vibdata.lock);             //初始化 互斥锁  
	     ret=timed_output_dev_register(&mx6_motot_driver);		//注册timed_output 设备  
	     if (ret< 0)  
		       gotoerr_to_dev_reg;  
	     return 0;  
	err_to_dev_reg:          					 //错误了 就释放所有资源  
	     mutex_destroy(&vibdata.lock);  
	     wake_lock_destroy(&vibdata.wklock);  

	     gpio_free(+_VIBRATOR_CTL);  
	     printk("vibrator   err!:%d\n",ret);  
	     returnret;  
	      
	}  
	void mx6_motor_exit()  
	{  
	     mutex_destroy(&vibdata.lock);  
	     wake_lock_destroy(&vibdata.wklock);  
	     gpio_free(SABRESD_VIBRATOR_CTL);  
	     printk("vibrator  exit!\n");  
	     timed_output_dev_register(&mx6_motot_driver);  
	}  
	module_init(mx6_motor_init);  
	module_exit(mx6_motor_exit);  

	MODULE_AUTHOR("<lijianzhang>");  
	MODULE_DESCRIPTION("Motor Vibrator driver");  
	MODULE_LICENSE("GPL");  

二：设置节点  timed_output_dev_register(&mx6_motot_driver);

	struct timed_output_dev {  
	     constchar  *name;  

	     /* enablethe output and set the timer */  
	     void   (*enable)(struct timed_output_dev *sdev, inttimeout);  

	     /*returns the current number of milliseconds remaining on the timer */  
	     int              (*get_time)(struct	timed_output_dev *sdev);  

	     /*private data */  
	     struct device       *dev;  
	     int              index;  
	     int              state;  
	};  

	extern int timed_output_dev_register(struct timed_output_dev*dev);  
	extern void timed_output_dev_unregister(structtimed_output_dev *dev);  

	#include<linux/module.h>  
	#include<linux/types.h>  
	#include<linux/device.h>  
	#include<linux/fs.h>  
	#include<linux/err.h>  

	#include"timed_output.h"  

	static struct class *timed_output_class;  
	static atomic_t device_count;  

	static ssize_t enable_show(structdevice *dev, struct device_attribute *attr,  
		 char *buf)  
	{  
	struct timed_output_dev *tdev =dev_get_drvdata(dev);  
	int remaining = tdev->get_time(tdev);  

	return sprintf(buf, "%d\n",remaining);  
	}  

	static ssize_tenable_store(  
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

	static intcreate_timed_output_class(void)  
	{  
	if (!timed_output_class) {  
		 timed_output_class =class_create(THIS_MODULE, "timed_output");  
		 if (IS_ERR(timed_output_class))  
		           return PTR_ERR(timed_output_class);  
		 atomic_set(&device_count, 0);  
	}  

	return 0;  
	}  

	int	timed_output_dev_register(struct timed_output_dev *tdev)  
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

	ret = device_create_file(tdev->dev,&dev_attr_enable);  //dev_attr_enable  static DEVICE_ATTR(enable,S_IRUGO | S_IWUSR, enable_show, enable_store);
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

	void timed_output_dev_unregister(struct timed_output_dev *tdev)  
	{  
		device_remove_file(tdev->dev,&dev_attr_enable);  
		device_destroy(timed_output_class, MKDEV(0,tdev->index));  
		dev_set_drvdata(tdev->dev, NULL);  
	}  
	EXPORT_SYMBOL_GPL(timed_output_dev_unregister);  

	static int __inittimed_output_init(void)  
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


