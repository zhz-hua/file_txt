#define UART_NR 8
struct imx_port {
	struct uart_port	port;
	struct timer_list	timer;
	unsigned int		old_status;
	int			txirq, rxirq, rtsirq;
	...
};
struct uart_port {
	spinlock_t		lock;			/* port lock */
	unsigned long		iobase;			/* in/out[bwl] */
	unsigned char __iomem	*membase;		/* read/write[bwl] */
	unsigned int		(*serial_in)(struct uart_port *, int);
	void			(*serial_out)(struct uart_port *, int, int);
	void			(*set_termios)(struct uart_port *,
				               struct ktermios *new,
				               struct ktermios *old);
	int			(*handle_irq)(struct uart_port *);
	void			(*handle_break)(struct uart_port *);
	unsigned int		irq;			/* irq number */
	unsigned long		irqflags;		/* irq flags  */
	unsigned int		uartclk;		/* base uart clock */
	unsigned int		fifosize;		/* tx fifo size */
	unsigned char		x_char;			/* xon/xoff char */
	unsigned char		regshift;		/* reg offset shift */
	unsigned char		iotype;			/* io access style */
	unsigned char		unused1;

	const struct uart_ops	*ops;
	unsigned int		custom_divisor;
	unsigned int		line;			/* port index */
	resource_size_t		mapbase;		/* for ioremap */
	struct device		*dev;			/* parent device */
	unsigned char		hub6;			/* this should be in the 8250 driver */
	unsigned char		suspended;
	unsigned char		irq_wake;
	unsigned char		unused[2];
	void			*private_data;		/* generic platform data pointer */
	...
};

static struct imx_port *imx_ports[UART_NR];

static struct uart_driver imx_reg = {
	.owner          = THIS_MODULE,
	.driver_name    = "IMX-uart",
	.dev_name       = "ttymxc",
	.major          = 207,
	.minor          = 16,
	.nr             = ARRAY_SIZE(imx_ports),
	.cons           = IMX_CONSOLE,
};

uart_add_one_port(&imx_reg, &sport->port);

int uart_add_one_port(struct uart_driver *drv, struct uart_port *uport)
{
	struct uart_state *state;
	struct tty_port *port;
	int ret = 0;
	struct device *tty_dev;

	BUG_ON(in_interrupt());

	if (uport->line >= drv->nr)
		return -EINVAL;

	state = drv->state + uport->line;
	port = &state->port;

	state->uart_port = uport;
	state->pm_state = UART_PM_STATE_UNDEFINED;

	uport->cons = drv->cons;
	uport->state = state;

	
	uart_configure_port(drv, state, uport);

	tty_dev = tty_port_register_device_attr(port, drv->tty_driver,
			uport->line, uport->dev, port, tty_dev_attr_groups);

	uport->flags &= ~UPF_DEAD;

	return ret;
}

static void
uart_configure_port(struct uart_driver *drv, struct uart_state *state,
		    struct uart_port *port)
{
	unsigned int flags;

	if (!port->iobase && !port->mapbase && !port->membase)
		return;

	flags = 0;
	if (port->flags & UPF_AUTO_IRQ)
		flags |= UART_CONFIG_IRQ;
	if (port->flags & UPF_BOOT_AUTOCONF) {
		if (!(port->flags & UPF_FIXED_TYPE)) {
			port->type = PORT_UNKNOWN;
			flags |= UART_CONFIG_TYPE;
		}
		port->ops->config_port(port, flags);
	}

	if (port->type != PORT_UNKNOWN) {
		unsigned long flags;

		uart_report_port(drv, port);

		uart_change_pm(state, UART_PM_STATE_ON);
	}
}
struct device *tty_register_device_attr(struct tty_driver *driver,
				   unsigned index, struct device *device,
				   void *drvdata,
				   const struct attribute_group **attr_grp)
{
	tty_line_name(driver, index, name);
	retval = tty_cdev_add(driver, devt, index, 1);
	dev->devt = devt;				//tty设备能注册
	dev->class = tty_class;
	dev->parent = device;
	dev->release = tty_device_create_release;
	dev_set_name(dev, "%s", name);
	dev->groups = attr_grp;
	dev_set_drvdata(dev, drvdata);

	retval = device_register(dev)
}
static ssize_t tty_line_name(struct tty_driver *driver, int index, char *p)
{
	if (driver->flags & TTY_DRIVER_UNNUMBERED_NODE)
		return sprintf(p, "%s", driver->name);
	else
		return sprintf(p, "%s%d", driver->name,
			       index + driver->name_base);
}
static int tty_cdev_add(struct tty_driver *driver, dev_t dev,
		unsigned int index, unsigned int count)
{
	cdev_init(&driver->cdevs[index], &tty_fops);
	driver->cdevs[index].owner = driver->owner;
	return cdev_add(&driver->cdevs[index], dev, count);
}
uart_report_port(struct uart_driver *drv, struct uart_port *port)
{
	char address[64];

	switch (port->iotype) {
	case UPIO_TSI:
		snprintf(address, sizeof(address),
			 "MMIO 0x%llx", (unsigned long long)port->mapbase);
		break;
	}

	printk(KERN_INFO "%s%s%s%d at %s (irq = %d) is a %s\n",
	       port->dev ? dev_name(port->dev) : "",
	       port->dev ? ": " : "",
	       drv->dev_name,
	       drv->tty_driver->name_base + port->line,
	       address, port->irq, uart_type(port));
}





