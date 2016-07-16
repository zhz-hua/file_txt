#
# Makefile for the testmode core.
#

#obj-y				+= test_drvmode.o
obj-m           +=test_drvmode.o #产生hello模块的目标文件  
CURRENT_PATH        :=$(shell pwd) #模块所在的当前路径  
#LINUX_KERNEL        :=$(shell uname -r) #Linux内核源代码的当前版本  
LINUX_KERNEL_PATH   :=/home/zhanhuazhong/workspace/ivi-android-4.4.3-eng/kernel_imx 				#Linux     内核源代码的绝对路径
KERNEL_CROSS_TOOLCHAIN := /home/zhanhuazhong/workspace/ivi-android-4.4.3-eng/prebuilts/gcc/linux-x86/arm/arm-eabi-4.6/bin/arm-eabi-                                                                                   
KERNEL_ENV := ARCH=arm CROSS_COMPILE=$(KERNEL_CROSS_TOOLCHAIN) LOADADDR=$(LOAD_KERNEL_ENTRY)
HOST_PROCESSOR := $(shell cat /proc/cpuinfo | grep processor | wc -l)
all:
	make -C $(LINUX_KERNEL_PATH) -j$(HOST_PROCESSOR) M=$(CURRENT_PATH)  $(KERNEL_ENV)  
clean:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean #清理  
         

