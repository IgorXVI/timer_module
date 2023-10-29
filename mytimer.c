#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/jiffies.h>
#include <linux/timer.h>

/* Meta Information */
MODULE_LICENSE("GPL");

//timer
static struct timer_list my_timer;

void timer_callback(struct timer_list* data) {
	gpio_set_value(14, 0);
}

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init my_init(void) {
	printk("Hello, Kernel!\n");

	/* GPIO 14 init */
	if(gpio_request(14, "rpi-gpio-14")) {
		printk("Can not allocate GPIO 14\n");
		return -1;
	}

	/* Set GPIO 14 direction */
	if(gpio_direction_output(14, 0)) {
		printk("Can not set GPIO 14 to output!\n");
		gpio_free(14);
		return -1;
	}

	//turn LED on
	gpio_set_value(14, 1);

	//init timer
	timer_setup(&my_timer, timer_callback, 0);
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));

	return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit my_exit(void) {
	printk("Goodbye, Kernel\n");
	gpio_free(14);
	del_timer(&my_timer);
}

module_init(my_init);
module_exit(my_exit);