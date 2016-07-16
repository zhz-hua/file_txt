/* action_list列表用来保存从启动脚本/init.rc解析得到的一系列action，以及一系列内建的action。当这些action需要执行的时候，它们就会被添加到action_queue列表中去，以便init进程可以执行它们。       回到init进程的入口函数main中，最后init进程会进入到一个无限循环中去。在这个无限循环中，init进程会做以下五个事情：

       A. 调用函数execute_one_command来检查action_queue列表是否为空。如果不为空的话，那么init进程就会将保存在列表头中的action移除，并且执行这个被移除的action。由于前面我们将一个名称为“console_init”的action添加到了action_queue列表中，因此，在这个无限循环中，这个action就会被执行，即函数console_init_action会被调用。
       B. 调用函数restart_processes来检查系统中是否有进程需要重启。在启动脚本/init.rc中，我们可以指定一个进程在退出之后会自动重新启动。在这种情况下，函数restart_processes就会检查是否存在需要重新启动的进程，如果存在的话，那么就会将它重新启动起来。
       C. 处理系统属性变化事件。当我们调用函数property_set来改变一个系统属性值时，系统就会通过一个socket（通过调用函数get_property_set_fd可以获得它的文件描述符）来向init进程发送一个属性值改变事件通知。init进程接收到这个属性值改变事件之后，就会调用函数handle_property_set_fd来进行相应的处理。后面在分析第三个开机画面的显示过程时，我们就会看到，SurfaceFlinger服务就是通过修改“ctl.start”和“ctl.stop”属性值来启动和停止第三个开机画面的。
       D. 处理一种称为“chorded keyboard”的键盘输入事件。这种类型为chorded keyboard”的键盘设备通过不同的铵键组合来描述不同的命令或者操作，它对应的设备文件为/dev/keychord。我们可以通过调用函数get_keychord_fd来获得这个设备的文件描述符，以便可以监控它的输入事件，并且调用函数handle_keychord来对这些输入事件进行处理。
       E. 回收僵尸进程。我们知道，在Linux内核中，如果父进程不等待子进程结束就退出，那么当子进程结束的时候，就会变成一个僵尸进程，从而占用系统的资源。为了回收这些僵尸进程，init进程会安装一个SIGCHLD信号接收器。当那些父进程已经退出了的子进程退出的时候，内核就会发出一个SIGCHLD信号给init进程。init进程可以通过一个socket（通过调用函数get_signal_fd可以获得它的文件描述符）来将接收到的SIGCHLD信号读取回来，并且调用函数handle_signal来对接收到的SIGCHLD信号进行处理，即回收那些已经变成了僵尸的子进程。
      注意，由于后面三个事件都是可以通过文件描述符来描述的，因此，init进程的入口函数main使用poll机制来同时轮询它们，以便可以提高效率。
      接下来我们就重点分析函数console_init_action的实现，以便可以了解第二个开机画面的显示过程：


系统开机第一个程序的进程是init，它的源码位于system/core/init/init.c，其中函数load_565rle_image负责logo的显示，如果读取成功，就在/dev/graphics/fb0显示图片，如果读取失败，则将/dev/tty0设为文本模式，并打开/dev/tty0并输出android字样。之后会显示android开机动画(其实两张图片做成的效果)，logo图片由INIT_IMAGE_FILE(在init.h中)指定。*/
/*ndroid开机的第一个进程为init，在它的实现文件，即init.c中，会通过调用函数load_565rle_image(INIT_IMAGE_FILE)来实现开机启动显示logo的。而上述这个函数是在同目录下的logo.c中实现的。

在load_565rle_image()这个主函数中先调用vt_set_mode()来设定终端的显示方式，默认是使用终端的graphics显示方式，通过对开/dev/tty0设备文件调用ioctl系统调用来设定，但是如果不能打开load_565rle_image(INIT_IMAGE_FILE)中指定的INIT_IMAGE_FILE图片文件时，就直接将/dev/tty0设置为Text显示模式，然后直接返回，执行init.c中后面的代码（直接向/dev/tty0设备写入"Android"字样的logo，这也就是默认的启动logo，很没有创意，哈哈！）

但是如果存在INIT_IMAGE_FILE这个图片文件的话，就会在load_565rle_image中通过调用mmap，将图片文件映射到当前进程的存储空间中，接着就又调用fb_open()函数来打开linux framebuufer对应的设备文件 /dev/graphics/fb0 ,并把framebuffer也通过mmap映射到存储空间，于是就在后面调用android_memset16()来写入图片，从android_memset16()这样函数的参数就可以看出，程序的目的就是要把ptr对应的图片存储空间地址所对应的字节，写到bits对应的framebuffer对应的存储空间中。写Framebuffer具体细节就不追究了，可能牵涉到Framebuffer原理的一些知识*/

/*
在rle565格式，前面2个字节中用来描述序列的个数，而后面2个字节用来描述一个具体的颜色，其中，颜色的RGB值分别占5位、6位和5位。理解了565rle图像格式之后，我们就可以理解函数load_565rle_image中的while循环的实现逻辑了。在每一次循环中，都会依次从文件initlogo.rle中读出4个字节，其中，前两个字节的内容保存在变量n中，而后面2个字节的内容用来写入到帧缓冲区硬件设备中去。由于2个字节刚好就可以使用一个无符号短整数来描述，因此，函数load_565rle_image通过调用函数android_memset16来将从文件initlogo.rle中读取出来的颜色值写入到帧缓冲区硬件设备中去，
  参数ptr指向被写入的地址，在我们这个场景中，这个地址即为帧缓冲区硬件设备映射到init进程中的虚拟地址值。       参数val用来描述被写入的值，在我们这个场景中，这个值即为从文件initlogo.rle中读取出来的颜色值。
       参数count用来描述被写入的地址的长度，它是以字节为单位的。由于在将参数val的值写入到参数ptr所描述的地址中去时，是以无符号短整数为单位的，即是以2个字节为单位的，因此，函数android_memset16在将参数val写入到地址ptr中去之前，首先会将参数count的值除以2。相应的地，在函数load_565rle_image中，需要将具有相同颜色值的序列的个数乘以2之后，再调用函数android_memset16。
*/
void android_memset16(void *_ptr, unsigned short val, unsigned count)
{
    unsigned short *ptr = _ptr;
    count >>= 1;
    while(count--)
        *ptr++ = val;
}
#endif

struct FB {
    unsigned short *bits;
    unsigned size;
    int fd;
    struct fb_fix_screeninfo fi;
    struct fb_var_screeninfo vi;
};

#define fb_width(fb) ((fb)->vi.xres)
#define fb_height(fb) ((fb)->vi.yres)
#define fb_bpp(fb) ((fb)->vi.bits_per_pixel)
#define fb_size(fb) ((fb)->vi.xres * (fb)->vi.yres * \
	((fb)->vi.bits_per_pixel/8))
/*
 将文件/initlogo.rle映射到init进程的地址空间之后，接下来再调用函数fb_open来打开设备文件/dev/graphics/fb0。
前面在介绍第一个开机画面的显示过程中提到，设备文件/dev/graphics/fb0是用来访问系统的帧缓冲区硬件设备的，
因此，打开了设备文件/dev/graphics/fb0之后，我们就可以将文件/initlogo.rle的内容输出到帧缓冲区硬件设备中去了
*/
static int fb_open(struct FB *fb)	//打开framebuffer设备 
{
	//fd对应设备文件为 /dev/graphics/fb0
    fb->fd = open("/dev/graphics/fb0", O_RDWR);
    if (fb->fd < 0)
        return -1;
/*
打开了设备文件/dev/graphics/fb0之后，接着再分别通过IO控制命令FBIOGET_FSCREENINFO和FBIOGET_VSCREENINFO来获得帧缓冲硬件设备的固定信息和可变信息。固定信息使用一个fb_fix_screeninfo结构体来描述，它保存的是帧缓冲区硬件设备固有的特性，这些特性在帧缓冲区硬件设备被初始化了之后，就不会发生改变，例如屏幕大小以及物理地址等信息。可变信息使用一个fb_var_screeninfo结构体来描述，它保存的是帧缓冲区硬件设备可变的特性，这些特性在系统运行的期间是可以改变的，例如屏幕所使用的分辨率、颜色深度以及颜色格式等。        除了获得帧缓冲区硬件设备的固定信息和可变信息之外，函数fb_open还会将设备文件/dev/graphics/fb0的内容映射到init进程的地址空间来，这样init进程就可以通过映射得到的虚拟地址来访问帧缓冲区硬件设备的内容了。
       回到函数load_565rle_image中，接下来分别使用宏fb_width和fb_height来获得屏幕所使用的的分辨率，即屏幕的宽度和高度。宏fb_width和fb_height的定义如下所示：
*/
    if (ioctl(fb->fd, FBIOGET_FSCREENINFO, &fb->fi) < 0)
        goto fail;
    if (ioctl(fb->fd, FBIOGET_VSCREENINFO, &fb->vi) < 0)
        goto fail;
	//对fd对应的Framebuffer大小进行mmap
    fb->bits = mmap(0, fb_size(fb), PROT_READ | PROT_WRITE, 
                    MAP_SHARED, fb->fd, 0);
    if (fb->bits == MAP_FAILED)
        goto fail;

    return 0;

fail:
    close(fb->fd);
    return -1;
}
//关闭framebuffer
static void fb_close(struct FB *fb)
{
    munmap(fb->bits, fb_size(fb));
    close(fb->fd);
}

/* there's got to be a more portable way to do this ... */
static void fb_update(struct FB *fb)
{
    fb->vi.yoffset = 1;
    ioctl(fb->fd, FBIOPAN_DISPLAY, &fb->vi);
    fb->vi.yoffset = 0;
    ioctl(fb->fd, FBIOPAN_DISPLAY, &fb->vi);
}
//设置终端显示模式：Graphics or Text?  
static int vt_set_mode(int graphics)
{
    int fd, r;
    fd = open("/dev/tty0", O_RDWR | O_SYNC);
    if (fd < 0)
        return -1;
//设置图片还是文字  
    r = ioctl(fd, KDSETMODE, (void*) (graphics ? KD_GRAPHICS : KD_TEXT));
    close(fd);
    return r;
}

/* 565RLE image format: [count(2 bytes), rle(2 bytes)] */

int load_565rle_image(char *fn)
{
    struct FB fb;
    struct stat s;
    unsigned short *data, *bits, *ptr;
    uint32_t rgb32, red, green, blue, alpha;
    unsigned count, max;
    int fd;

    if (vt_set_mode(1)) //打开Graphics显示模式
        return -1;

    fd = open(fn, O_RDONLY);	//判断是否能够打开INIT_IMAGE_FILE  
    if (fd < 0) {
        ERROR("cannot open '%s'\n", fn);
        goto fail_restore_text;	 //如果不行就恢复Text显示模式  
    }
 /* 取得文件大小 */  fstat(fd, &sb);
    if (fstat(fd, &s) < 0) {
        goto fail_close_file;
    }
/*在用户空间调用mmap，相当于在进程中开辟了文件的存储IO映射 
  data为返回的映射地址，把图片文件内容映射到存储区 
*/
    data = mmap(0, s.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED)
        goto fail_close_file;

    if (fb_open(&fb))
        goto fail_unmap_data;

    max = fb_width(&fb) * fb_height(&fb);
    ptr = data;
    count = s.st_size;
    bits = fb.bits;
    while (count > 3) {
        unsigned n = ptr[0];
        if (n > max)
            break;
        if (fb_bpp(&fb) == 16) {
//将ptr对应的图片写到bits对应的framebuffer中,细节不追究了
            android_memset16(bits, ptr[1], n << 1);
            bits += n;
        } else {
            /* convert 16 bits to 32 bits */
            red = ((ptr[1] >> 11) & 0x1F);
            red = (red << 3) | (red >> 2);
            green = ((ptr[1] >> 5) & 0x3F);
            green = (green << 2) | (green >> 4);
            blue = ((ptr[1]) & 0x1F);
            blue = (blue << 3) | (blue >> 2);
            alpha = 0xff;
            rgb32 = (alpha << 24) | (red << 16)
                    | (green << 8) | (blue << 0);
            android_memset32((uint32_t *)bits, rgb32, n << 2);
            bits += (n * 2);
        }

        max -= n;
        ptr += 2;
        count -= 4;
    }
/*
调用函数munmap来注销文件/initlogo.rle在init进程中的映射，并且调用函数close来关闭文件/initlogo.rle。关闭了文件/initlogo.rle之后，还会调用函数unlink来删除目标设备上的/initlogo.rle文件。注意，这只是删除了目标设备上的/initlogo.rle文件，而不是删除ramdisk映像中的initlogo.rle文件，因此，每次关机启动之后，系统都会重新将ramdisk映像中的initlogo.rle文件安装到目标设备上的根目录来，这样就可以在每次开机的时候都能将它显示出来
调用fb_close函数之前，函数load_565rle_image还会调用另外一个函数fb_update来更新屏幕上的第二个开机画面
*/
    munmap(data, s.st_size);
    fb_update(&fb);
    fb_close(&fb);
    close(fd);
    unlink(fn);//删除fn ?  
    return 0;

fail_unmap_data:
//munmap释放申请的内存映射IO 
    munmap(data, s.st_size);    
fail_close_file:
    close(fd);
fail_restore_text:
    vt_set_mode(0);
//返回-1，于是就直接显示"Android"字样！  
    return -1;
}
/*总结：显示开机logo，如果使用图片的话就是通过mmap，将图片内容写到Framebuffer存储映射空间来完成的；而如果只是显示Text的话，就是直接和/dev/tty0打交道了。分析完毕*/

int ioctl(int fd, ind cmd, …)； 
    其中fd是用户程序打开设备时使用open函数返回的文件标示符，cmd是用户程序对设备的控制命令，至于后面的省略号，那是一些补充参数，一般最多一个，这个参数的有无和cmd的意义相关。 

一、fstat 函数

功能：由文件描述符取得文件状态。

相关函数：stat 、lstat 、chmod 、chown 、readlink 、utime。

头文件：  #include <sys/stat.h>
          #include <unistd.h>

函数声明： int fstat （int filedes，struct ＊buf）；

描述： fstat（）用来将参数filedes 所指向的文件状态复制到参数buf 所指向的结构中（struct stat）。fstat（）与stat（）作用完全相同，不同之处在于传入的参数为已打开的文件描述符。

返回值：执行成功返回0，失败返回－1，错误代码保存在errno。

int _fstat( int handle, struct _stat *buffer );
获取文件状态和信息。返回结构在 &st 中。fd  -- 文件句柄。
头文件 <sys/stat.h> and <sys/types.h>
MS VC++ 编译器

 mmap系统调用使得进程之间通过映射同一个普通文件实现共享内存。普通文件被映射到进程地址空间后，进程可以像访问普通内存一样对文件进行访问，不必再调用read()，write（）等操作。

          我们的程序中大量运用了mmap，用到的正是mmap的这种“像访问普通内存一样对文件进行访问”的功能。实践证明，当要对一个文件频繁的进行访问，并且指针来回移动时，调用mmap比用常规的方法快很多。
          来看看mmap的定义：
void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offse

若映射成功则返回映射区的内存起始地址，否则返回MAP_FAILED(－1)，错误原因存于errno 中

