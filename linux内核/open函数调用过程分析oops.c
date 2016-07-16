[   17.288318] the imx startup:imx_startup
<4>[   17.288354] CPU: 0 PID: 519 Comm: k.mcuservicelib Tainted: G           O 3.10.53 #0
<4>[   17.288369] Backtrace: 
<4>[   17.288384] Function entered at [<800120cc>] from [<8001225c>]
<4>[   17.288395]  r6:00000000 r5:9c06c0c4 r4:9c332810 r3:00000000
<4>[   17.288425] Function entered at [<80012244>] from [<805c5358>]
<4>[   17.288436] Function entered at [<805c5338>] from [<802bf90c>]
<4>[   17.288446] Function entered at [<802bf8e8>] from [<802bc150>]
<4>[   17.288456]  r7:9d310400 r6:00000000 r5:9c06c0c4 r4:9c332810
<4>[   17.288485] Function entered at [<802bc108>] from [<802bcf78>]
<4>[   17.288495]  r8:9d310524 r7:9d1d3240 r6:9c06c130 r5:9d310400 r4:9c06c0c4
<4>[   17.288495] r3:00000002
<4>[   17.288534] Function entered at [<802bce74>] from [<802a2c2c>]
<4>[   17.288545] Function entered at [<802a29d8>] from [<800e35f0>]
<4>[   17.288556] Function entered at [<800e3554>] from [<800ddd8c>]
<4>[   17.288565]  r8:800e3554 r7:00020002 r6:9d1d3248 r5:9c83c958 r4:9d1d3240
<4>[   17.288600] Function entered at [<800ddba8>] from [<800dde20>]
<4>[   17.288612] Function entered at [<800dde04>] from [<800ec868>]
<4>[   17.288622]  r4:9db61ee0 r3:9db61e94
<4>[   17.288640] Function entered at [<800ec3c0>] from [<800ed144>]
<4>[   17.288650] Function entered at [<800ed08c>] from [<800ed7bc>]
<4>[   17.288661] Function entered at [<800ed788>] from [<800dee74>]
<4>[   17.288670]  r7:00000001 r6:0000002f r5:00020002 r4:9cce5000
<4>[   17.288769] Function entered at [<800ded84>] from [<800def24>]
<4>[   17.288782] Function entered at [<800def00>] from [<8000e440>]

./arm-eabi-objdump -D vmlinux > vmlinux.dis

[   17.288318] the imx startup:imx_startup
<4>[   17.288354] CPU: 0 PID: 519 Comm: k.mcuservicelib Tainted: G           O 3.10.53 #0
<4>[   17.288369] Backtrace: 
<4>[   17.288384] Function entered at [dump_backtrace] from [show_stack]
<4>[   17.288395]  r6:00000000 r5:9c06c0c4 r4:9c332810 r3:00000000
<4>[   17.288425] Function entered at [show_stack] from [dump_stack]
<4>[   17.288436] Function entered at [dump_stack] from [imx_startup]
<4>[   17.288446] Function entered at [imx_startup] from [uart_port_startup]
<4>[   17.288456]  r7:9d310400 r6:00000000 r5:9c06c0c4 r4:9c332810
<4>[   17.288485] Function entered at [<uart_port_startup>] from [uart_open]
<4>[   17.288495]  r8:9d310524 r7:9d1d3240 r6:9c06c130 r5:9d310400 r4:9c06c0c4
<4>[   17.288495] r3:00000002
<4>[   17.288534] Function entered at [<uart_open>] from [tty_open>]
<4>[   17.288545] Function entered at [<tty_open>] from [<chrdev_open>]
<4>[   17.288556] Function entered at [<chrdev_open>] from [do_dentry_open.isra.13]
<4>[   17.288565]  r8:800e3554 r7:00020002 r6:9d1d3248 r5:9c83c958 r4:9d1d3240
<4>[   17.288600] Function entered at [do_dentry_open.isra.13] from [finish_open]
<4>[   17.288612] Function entered at [finish_open] from [<do_last>]
<4>[   17.288622]  r4:9db61ee0 r3:9db61e94
<4>[   17.288640] Function entered at [<do_last>] from [<path_openat]
<4>[   17.288650] Function entered at [<path_openat>] from [<do_filp_open>]
<4>[   17.288661] Function entered at [<do_filp_open>] from [<do_sys_open>]
<4>[   17.288670]  r7:00000001 r6:0000002f r5:00020002 r4:9cce5000
<4>[   17.288769] Function entered at [<do_sys_open>] from [<SyS_open>]
<4>[   17.288782] Function entered at [<SyS_open>] from [<<ret_fast_syscall>]


/*
1：open的过程
	根据以上可以确认 open("/dev/ttymxc2",O_RDWR | O_NOCTTY | O_NONBLOCK)
	行数调用关系如下
	open("/dev/ttymxc2",O_RDWR | O_NOCTTY | O_NONBLOCK)
	{
		SyS_open-->do_sys_open-->do_filp_open-->path_openat-->do_last-->finish_open-->do_dentry_open.isra.13-->chrdev_open  (系统调用)
		-->tty_open-->uart_open-->uart_port_startup-->imx_startup 初始化开启中断等
	}
2: read的过程
3：write的过程
*/
关于open函数
 1： open 的过程
   首先 打开设备文件fd = open("/dev/userdevice,0_RDWR);  用户态进入内核态  
   其次 系统 调用 sys_open  
   最后 以之匹 file_operations 的open函数
当应用程序调用open系统调用后，sys_open就会调用字符驱动的file_operations中的open函数 
这个open最终会调用驱动中的open函数(代码流程是这样的open()->sys_open()->filp_open()->dentry_open()->驱动open)*/

通过open系统调用先去打开这个设备，不管是设备还是文件，我们要访问它都要称通过open函数来先打开， 这样才能调用其它的函数如read、write来操作它,即通知内核新建一个代表该文件的结构，并且返回该文件的描述符(一个整数)，该描述符在进程内唯一。

在linux系统进程中，分为内核空间和用户空间，当一个任务（进程）执行系统调用而陷入内核代码中执行时，我们就称进程处于内核运行态（内核态）。在内核态下，CPU可执行任何指令。当进程在执行用户自己的代码时，则称其处于用户运行态（用户态）。用户态不能访问内核空间，包括代码和数据。所有进程的内核空间（3G－4G）都是共享的。当我们在用户空间调用open之后，会产生一个软中断，然后通过系统调用进入内核空间。通过系统调用号，我们就可以跳转到该中断例程的入口地址。

串口
/* 
 * 当应用程序调用open系统调用后，sys_open就会调用字符驱动的file_operations中的open函数 
 * 也就是tty_fops中的open函数 
 * */ 
open
static const struct file_operations tty_fops = {
	.llseek		= no_llseek,
	.read		= tty_read,
	.write		= tty_write,
	.poll		= tty_poll,
	.unlocked_ioctl	= tty_ioctl,
	.compat_ioctl	= tty_compat_ioctl,
	.open		= tty_open,
	.release	= tty_release,
	.fasync		= tty_fasync,
};
/*
Tty_io.c
*/
tty_open

if (tty->ops->open)
retval = tty->ops->open(tty, filp);

static const struct tty_operations uart_ops = {
	.open		= uart_open,
	.close		= uart_close,
	.write		= uart_write,
	.put_char	= uart_put_char,
	.flush_chars	= uart_flush_chars,
	.write_room	= uart_write_room,
	.chars_in_buffer= uart_chars_in_buffer,
	.flush_buffer	= uart_flush_buffer,
	.ioctl		= uart_ioctl,
	.throttle	= uart_throttle,
	.unthrottle	= uart_unthrottle,
	.send_xchar	= uart_send_xchar,
	.set_termios	= uart_set_termios,
	.set_ldisc	= uart_set_ldisc,
	.stop		= uart_stop,
	.start		= uart_start,
	.hangup		= uart_hangup,
}
/*Serial_core.c*/
uart_open

/*
	 * Start up the serial port.
	 */
	retval = uart_startup(tty, state, 0);
uart_port_startup

retval = uport->ops->startup(uport);

static struct uart_ops imx_pops = {
	.tx_empty	= imx_tx_empty,
	.set_mctrl	= imx_set_mctrl,
	.get_mctrl	= imx_get_mctrl,
	.stop_tx	= imx_stop_tx,
	.start_tx	= imx_start_tx,
	.stop_rx	= imx_stop_rx,
	.enable_ms	= imx_enable_ms,
	.break_ctl	= imx_break_ctl,
	.startup	= imx_startup,
	.shutdown	= imx_shutdown,
	.flush_buffer	= imx_flush_buffer,
	.set_termios	= imx_set_termios,
	.type		= imx_type,
	.config_port	= imx_config_port,
	.verify_port	= imx_verify_port,
}
/*Imx.c*/
imx_startup


