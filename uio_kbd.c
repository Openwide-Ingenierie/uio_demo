#include <linux/kernel.h>	/* printk() */
#include <linux/module.h>	/* modules */
#include <linux/init.h>		/* module_{init,exit}() */
#include <linux/uio_driver.h>
#include <linux/device.h> 
#include <linux/slab.h> /* kzalloc */

MODULE_LICENSE("GPL");


/*
 * Global variables
 */
static const int irq = 1;
static struct uio_info *info;
static struct device *dev;
char *mem_area;

/*
 * Handlers
 */

static irqreturn_t uio_handler(int irq, struct uio_info *dev_info)
{
  static unsigned int irq_count = 0;
	pr_info("UIO handler");
  
  irq_count++;
  memcpy(mem_area, &irq_count, sizeof(irq_count));
  
	return IRQ_HANDLED;
}

static void uio_release(struct device *dev)
{
	pr_info("UIO release\n");
}

/*
 * Init and Exit
 */
static int __init uio_kbd_init(void)
{
  int ret;
  
  dev = kzalloc(sizeof(struct device), GFP_KERNEL);
  dev_set_name(dev, "uio_kbd_device");
  dev->release = uio_release;
  ret = device_register(dev);
  if (ret < 0){
    kfree(dev);
    pr_warn("Failing to register uio_kbd device\n");
    return ret;
  }



  mem_area = kzalloc(PAGE_SIZE, GFP_KERNEL);

  info = kzalloc(sizeof(struct uio_info), GFP_KERNEL);
  info->name = "uio_kbd_device";
  info->version = "0.0.1";
  info->irq = irq;
  info->irq_flags = IRQF_SHARED;
  info->handler = uio_handler;
  
  info->mem[0].name = "basic_mem_map";
  info->mem[0].memtype = UIO_MEM_LOGICAL;
  info->mem[0].addr = (phys_addr_t) mem_area;
  info->mem[0].size = sizeof(mem_area);
  
  
  ret = uio_register_device(dev, info);
  if (ret < 0) {
    device_unregister(dev);
    kfree(dev);
    kfree(info);
    kfree(mem_area);
    pr_warn("Failing to register uio_kbd UIO device\n");
    return ret;
  }
  
  pr_info( "Registered UIO handler for IRQ=%d\n", irq);
  return 0;
  
}

static void __exit uio_kbd_exit(void)
{
  uio_unregister_device(info);
  device_unregister(dev);
  pr_info("Un-Registered UIO handler for IRQ=%d\n", irq);
  kfree(info);
  kfree(dev);
  kfree(mem_area);
}

module_init(uio_kbd_init);
module_exit(uio_kbd_exit);

