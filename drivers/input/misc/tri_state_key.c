/*
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/gpio_keys.h>
#include <linux/of_platform.h>
#include <linux/of_gpio.h>

#include <linux/switch.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>

#include <linux/regulator/consumer.h>

#include <linux/timer.h>

#define DRV_NAME	"tri-state-key"

<<<<<<< HEAD
/*
	        	KEY1(GPIO1)	KEY2(GPIO92)
pin1 connect to pin4	0	            1         | MUTE
pin2 connect to pin5	1	            1         | Do Not Disturb
pin4 connect to pin3	1	            0         | Normal

*/
typedef enum
{
	MODE_UNKNOWN,
	MODE_MUTE,
	MODE_DO_NOT_DISTURB,
	MODE_NORMAL,
	MODE_MAX_NUM
} tri_mode_t;

<<<<<<< HEAD
/*
 * Target key codes sending to OPPO's Keyhandler
 * see KeyHandler.java in device/oppo/common/keyhandler
 */

#define KEY_SLIDER_TOP      601
#define KEY_SLIDER_MIDDLE   602
#define KEY_SLIDER_BOTTOM   603
=======
//static struct platform_device *tri_state_key_dev;
>>>>>>> parent of 2b88a468aa68... input: tri-state: rewritten tri-state-key driver

=======
#define KEYCODE_BASE 600
#define TOTAL_KEYCODES 6

/*
 * Default mapping between OP's sti-state switch and OPPO's key codes
 * see Constants.java in device/oppo/common
 */
static int current_mode = 0;
static int keyCode_slider_top = KEYCODE_BASE + 1;
static int keyCode_slider_middle = KEYCODE_BASE + 2;
static int keyCode_slider_bottom = KEYCODE_BASE + 3;
>>>>>>> d25c06f1f573... op5: tri_state_key: allow for additional settings

<<<<<<< HEAD
struct switch_dev_data {
	int irq_key1;
	int irq_key2;
=======
struct switch_dev_data
{
	//tri_mode_t last_type;
	//tri_mode_t mode_type;
	//int switch_enable;
>>>>>>> parent of 2b88a468aa68... input: tri-state: rewritten tri-state-key driver
	int irq_key3;
	int irq_key2;
	int irq_key1;
	int key1_gpio;//key1 gpio34
	int key2_gpio;//key2 gpio77
	int key3_gpio;

<<<<<<< HEAD
	struct regulator *vdd_io;
	//bool power_enabled;
=======
	int key1_gpio;
	int key2_gpio;
	int key3_gpio;
>>>>>>> d25c06f1f573... op5: tri_state_key: allow for additional settings

	struct work_struct work;
	struct switch_dev sdev;
	struct device *dev;
	//struct input_dev *input;

	struct timer_list s_timer;
	struct pinctrl * key_pinctrl;
	struct pinctrl_state * set_state;

};

static struct switch_dev_data *switch_data;
static DEFINE_MUTEX(sem);
//#if 0
static int set_gpio_by_pinctrl(void)
{
	printk(KERN_ERR "tristate_key set_gpio_by_pinctrl. \n");
	return pinctrl_select_state(switch_data->key_pinctrl, switch_data->set_state);
}
//#endif
static void switch_dev_work(struct work_struct *work)
{
<<<<<<< HEAD

	int key1,key2,key3;
	//pr_err("%s  gpio_get_value(%d)=%d\n",__func__,switch_data->key1_gpio,gpio_get_value(switch_data->key1_gpio));
	//pr_err("%s  gpio_get_value(%d)=%d\n",__func__,switch_data->key2_gpio,gpio_get_value(switch_data->key2_gpio));
	//pr_err("%s  gpio_get_value(%d)=%d\n",__func__,switch_data->key3_gpio,gpio_get_value(switch_data->key3_gpio));
	//pr_err("entering tristate:switch_dev_work\n");

<<<<<<< HEAD
	key1 = gpio_get_value(switch_data->key1_gpio);
	key2 = gpio_get_value(switch_data->key2_gpio);
	key3 = gpio_get_value(switch_data->key3_gpio);

	if (key1 == 0) {
		mode = MODE_MUTE;
		keyCode = KEY_SLIDER_TOP;
	} else if (key2 == 0) {
		mode = MODE_DO_NOT_DISTURB;
		keyCode = KEY_SLIDER_MIDDLE;
	} else if (key3 == 0) {
		mode = MODE_NORMAL;
		keyCode = KEY_SLIDER_BOTTOM;
=======
	int keyCode;
	int mode;
	mutex_lock(&sem);

	if(!gpio_get_value(switch_data->key3_gpio))
	{
		mode = 3;
		keyCode = keyCode_slider_bottom;
>>>>>>> d25c06f1f573... op5: tri_state_key: allow for additional settings
	}
	else if(!gpio_get_value(switch_data->key2_gpio))
	{
		mode = 2;
		keyCode = keyCode_slider_middle;
	}
	else if(!gpio_get_value(switch_data->key1_gpio))
	{
		mode = 1;
		keyCode = keyCode_slider_top;
	}
        if (current_mode != mode) {
		current_mode = mode;
		switch_set_state(&switch_data->sdev, current_mode);
		send_input(keyCode);
<<<<<<< HEAD
		printk(DRV_NAME " changed to mode: %d\n", switch_data->sdev.state);
=======
	mutex_lock(&sem);
	key1=gpio_get_value(switch_data->key1_gpio);
	key2=gpio_get_value(switch_data->key2_gpio);
	key3=gpio_get_value(switch_data->key3_gpio);
	//pr_err("tristate %s,key1=%d,key2=%d,key3=%d\n",__func__,key1,key2,key3);
	if(((key1==0)&&(key2==1)&&(key3==1))||((key1==1)&&(key2==0)&&(key3==1))||((key1==1)&&(key2==1)&&(key3==0)))
	{
		//gpio_set_value(switch_data->key3_gpio,0);
		printk("tristate_key set_gpio_by_pinctrl. \n");
		if(!key2)
		{
			switch_set_state(&switch_data->sdev, MODE_DO_NOT_DISTURB);
			//pr_err("%s MODE_DO_NOT_DISTURB\n",__func__);
		}

		if(!key3)
		{
			switch_set_state(&switch_data->sdev, MODE_NORMAL);
			//pr_err("%s MODE_NORMAL\n",__func__);

		}

		if(!key1)
		{
			switch_set_state(&switch_data->sdev, MODE_MUTE);
			//pr_err("%s MODE_MUTE\n",__func__);

		}

		printk(KERN_ERR "%s ,tristatekey set to state(%d) \n",__func__,switch_data->sdev.state);
>>>>>>> parent of 2b88a468aa68... input: tri-state: rewritten tri-state-key driver
=======
		printk("%s ,tristate set to state(%d) \n", __func__, switch_data->sdev.state);
>>>>>>> d25c06f1f573... op5: tri_state_key: allow for additional settings
	}
	mutex_unlock(&sem);
}
irqreturn_t switch_dev_interrupt(int irq, void *_dev)
{
//printk("%s\n",__func__);
	schedule_work(&switch_data->work);

	return IRQ_HANDLED;
}

static void timer_handle(unsigned long arg)
{
	//mod_timer(&s_timer, jiffies + HZ);
	//  if(set_gpio_by_pinctrl() < 0)
	//      printk(KERN_ERR "tristate_key set_gpio_by_pinctrl FAILD!!!. \n");
	schedule_work(&switch_data->work);
	//del_timer(&switch_data->s_timer);

	//printk(KERN_ERR "tristate_key set gpio77 timer. \n");
}

/* //no need cause switch_class.c state_show()
static ssize_t switch_dev_print_state(struct switch_dev *sdev, char *buf)
{
	tri_mode_t state;
		state = switch_data->mode_type;

	if (state)
		return sprintf(buf, "%d\n", state);
	return -1;
}
*/

#ifdef CONFIG_OF
static int switch_dev_get_devtree_pdata(struct device *dev)
{
	struct device_node *node;

	node = dev->of_node;
	if (!node){
		//printk("<0>""no node was found!!!!,%s\n",__func__);
		return -EINVAL;
	}
	switch_data->key3_gpio= of_get_named_gpio(node, "tristate,gpio_key3", 0);
	if ((!gpio_is_valid(switch_data->key3_gpio)))
		return -EINVAL;
	pr_err("switch_data->key3_gpio=%d \n", switch_data->key3_gpio);

	switch_data->key2_gpio= of_get_named_gpio(node, "tristate,gpio_key2", 0);
	if ((!gpio_is_valid(switch_data->key2_gpio)))
		return -EINVAL;
	pr_err("switch_data->key2_gpio=%d \n", switch_data->key2_gpio);
//printk("%s, key2 gpio:%d \n", __func__, switch_data->key2_gpio);

	switch_data->key1_gpio= of_get_named_gpio(node, "tristate,gpio_key1", 0);
	if ((!gpio_is_valid(switch_data->key1_gpio)))
		return -EINVAL;
	pr_err("switch_data->key1_gpio=%d \n", switch_data->key1_gpio);
//printk("%s, key1 gpio:%d \n", __func__, switch_data->key1_gpio);
	return 0;
}

<<<<<<< HEAD
=======
static struct of_device_id tristate_dev_of_match[] = {
	{ .compatible = "oneplus,tri-state-key", },
	{ },
};
MODULE_DEVICE_TABLE(of, tristate_dev_of_match);
>>>>>>> d25c06f1f573... op5: tri_state_key: allow for additional settings
#else

static inline int
switch_dev_get_devtree_pdata(struct device *dev)
{
	printk("<0>""%s inline function",__func__);
	return 0;
}
#endif

<<<<<<< HEAD
<<<<<<< HEAD
#define REGISTER_IRQ_FOR(KEY)\
	switch_data->irq_##KEY = gpio_to_irq(switch_data->KEY##_gpio);\
	if (switch_data->irq_##KEY <= 0) {\
		dev_err(dev, "%s: irq number is not specified, irq #= %d, int pin=%d\n",\
			__func__, switch_data->irq_##KEY, switch_data->KEY##_gpio);\
		goto err_detect_irq_num_failed;\
	} else {\
		error = gpio_request(switch_data->KEY##_gpio, "tristate_" #KEY "-int");\
		if (error < 0) {\
			dev_err(dev, "%s: gpio_request, err=%d\n", __func__, error);\
			goto err_request_gpio;\
		}\
		error = gpio_direction_input(switch_data->KEY##_gpio);\
		if (error < 0) {\
			dev_err(dev, "%s: gpio_direction_input, err=%d\n", __func__, error);\
			goto err_set_gpio_input;\
		}\
		error = request_irq(switch_data->irq_##KEY, switch_dev_interrupt,\
			IRQF_TRIGGER_FALLING, "tristate_" #KEY, switch_data);\
		if (error) {\
			dev_err(dev, "%s: request_irq, err=%d\n", __func__, error);\
			switch_data->irq_##KEY = -EINVAL;\
			goto err_request_irq;\
		}\
=======
	// Keycode top
static int keyCode_top_show(struct seq_file *seq, void *offset)
{
    seq_printf(seq, "%d\n", keyCode_slider_top);
    return 0;
}
static ssize_t keyCode_top_write(struct file *file, const char __user *page, size_t t, loff_t *lo)
{
	int data;
	char buf[10];

	if (copy_from_user(buf, page, t))
	{
		dev_err(switch_data->dev, "read proc input error.\n");
		return t;
	}

	if (sscanf(buf, "%d", &data) != 1)
		return t;
	if (data < KEYCODE_BASE || data >= (KEYCODE_BASE + TOTAL_KEYCODES))
		return t;

	keyCode_slider_top = data;
	if (current_mode == 1)
		send_input(keyCode_slider_top);

	return t;
}
static int keyCode_top_open(struct inode *inode, struct file *file)
{
	return single_open(file, keyCode_top_show, inode->i_private);
}
const struct file_operations proc_keyCode_top =
{
	.owner		= THIS_MODULE,
	.open		= keyCode_top_open,
	.read		= seq_read,
	.write		= keyCode_top_write,
	.llseek 	= seq_lseek,
	.release	= single_release,
};

	// Keycode middle
static int keyCode_middle_show(struct seq_file *seq, void *offset)
{
    seq_printf(seq, "%d\n", keyCode_slider_middle);
    return 0;
}
static ssize_t keyCode_middle_write(struct file *file, const char __user *page, size_t t, loff_t *lo)
{
	int data;
	char buf[10];

	if (copy_from_user(buf, page, t))
	{
		dev_err(switch_data->dev, "read proc input error.\n");
		return t;
>>>>>>> d25c06f1f573... op5: tri_state_key: allow for additional settings
	}

	if (sscanf(buf, "%d", &data) != 1)
		return t;
	if (data < KEYCODE_BASE || data >= (KEYCODE_BASE + TOTAL_KEYCODES))
		return t;

	keyCode_slider_middle = data;
	if (current_mode == 2)
		send_input(keyCode_slider_middle);

	return t;
}
static int keyCode_middle_open(struct inode *inode, struct file *file)
{
	return single_open(file, keyCode_middle_show, inode->i_private);
}
const struct file_operations proc_keyCode_middle =
{
	.owner		= THIS_MODULE,
	.open		= keyCode_middle_open,
	.read		= seq_read,
	.write		= keyCode_middle_write,
	.llseek 	= seq_lseek,
	.release	= single_release,
};

	// Keycode bottom
static int keyCode_bottom_show(struct seq_file *seq, void *offset)
{
    seq_printf(seq, "%d\n", keyCode_slider_bottom);
    return 0;
}
static ssize_t keyCode_bottom_write(struct file *file, const char __user *page, size_t t, loff_t *lo)
{
	int data;
	char buf[10];

	if (copy_from_user(buf, page, t))
	{
		dev_err(switch_data->dev, "read proc input error.\n");
		return t;
	}

	if (sscanf(buf, "%d", &data) != 1)
		return t;
	if (data < KEYCODE_BASE || data >= (KEYCODE_BASE + TOTAL_KEYCODES))
		return t;

	keyCode_slider_bottom = data;
	if (current_mode == 3)
		send_input(keyCode_slider_bottom);

	return t;
}
static int keyCode_bottom_open(struct inode *inode, struct file *file)
{
	return single_open(file, keyCode_bottom_show, inode->i_private);
}
const struct file_operations proc_keyCode_bottom =
{
	.owner		= THIS_MODULE,
	.open		= keyCode_bottom_open,
	.read		= seq_read,
	.write		= keyCode_bottom_write,
	.llseek 	= seq_lseek,
	.release	= single_release,
};

static int tristate_dev_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	int error = 0;
	int i;

	switch_data = kzalloc(sizeof(struct switch_dev_data), GFP_KERNEL);
	switch_data->dev = dev;

	switch_data->input = input_allocate_device();

	// init input device
=======
static int tristate_dev_probe(struct platform_device *pdev)
{
	struct device *dev;
	int error;
>>>>>>> parent of 2b88a468aa68... input: tri-state: rewritten tri-state-key driver

	dev= &pdev->dev;
	error = 0;

<<<<<<< HEAD
=======
	for (i = KEYCODE_BASE; i < KEYCODE_BASE + TOTAL_KEYCODES; i++)
	    set_bit(i, switch_data->input->keybit);
>>>>>>> d25c06f1f573... op5: tri_state_key: allow for additional settings

<<<<<<< HEAD
	set_bit(KEY_SLIDER_TOP, switch_data->input->keybit);
	set_bit(KEY_SLIDER_MIDDLE, switch_data->input->keybit);
	set_bit(KEY_SLIDER_BOTTOM, switch_data->input->keybit);
=======
	//pr_err("tristate_key set_gpio_by_pinctrl. \n");
	//void __iomem *cfg_reg;
	//void __iomem *cfg_reg;
>>>>>>> parent of 2b88a468aa68... input: tri-state: rewritten tri-state-key driver


	switch_data = kzalloc(sizeof(struct switch_dev_data), GFP_KERNEL);
	switch_data->dev = dev;
	//pr_err("%s  init platform_device.probe\n",__func__);

//#if 0
	switch_data->key_pinctrl = devm_pinctrl_get(switch_data->dev);
	if (IS_ERR_OR_NULL(switch_data->key_pinctrl))
	{
		dev_err(switch_data->dev, "Failed to get pinctrl \n");
		goto err_switch_dev_register;
	}
	switch_data->set_state =pinctrl_lookup_state(switch_data->key_pinctrl,"pmx_tri_state_key_active");
	if (IS_ERR_OR_NULL(switch_data->set_state))
	{
		dev_err(switch_data->dev, "Failed to lookup_state \n");
		goto err_switch_dev_register;
	}

	set_gpio_by_pinctrl();
//#endif
	//switch_data->last_type = MODE_UNKNOWN;

	//tristate_supply_init();
	error = switch_dev_get_devtree_pdata(dev);
	if (error)
	{
		dev_err(dev, "parse device tree fail!!!\n");
		goto err_switch_dev_register;
	}

<<<<<<< HEAD
	//config irq gpio and request irq
	switch_data->irq_key1 = gpio_to_irq(switch_data->key1_gpio);
	if (switch_data->irq_key1 <= 0)
	{
		printk("%s, irq number is not specified, irq #= %d, int pin=%d\n\n", __func__, switch_data->irq_key1, switch_data->key1_gpio);
		goto err_detect_irq_num_failed;
	}
	else
	{
		error = gpio_request(switch_data->key1_gpio,"tristate_key1-int");
		if(error < 0)
		{
			printk(KERN_ERR "%s: gpio_request, err=%d", __func__, error);
			goto err_request_gpio;
		}
		error = gpio_direction_input(switch_data->key1_gpio);
		if(error < 0)
		{
			printk(KERN_ERR "%s: gpio_direction_input, err=%d", __func__, error);
			goto err_set_gpio_input;
		}

		error = request_irq(switch_data->irq_key1, switch_dev_interrupt,
		                    IRQF_TRIGGER_FALLING, "tristate_key1", switch_data);

		if (error)
		{
			dev_err(dev,
			        "request_irq %i failed.\n",
			        switch_data->irq_key1);

			switch_data->irq_key1 = -EINVAL;
			goto err_request_irq;
		}
	}
	//config irq gpio and request irq
	switch_data->irq_key2 = gpio_to_irq(switch_data->key2_gpio);
	if (switch_data->irq_key2 <= 0)
	{
		printk("%s, irq number is not specified, irq #= %d, int pin=%d\n\n", __func__, switch_data->irq_key2, switch_data->key2_gpio);
		goto err_detect_irq_num_failed;
	}
	else
	{
		error = gpio_request(switch_data->key2_gpio,"tristate_key2-int");
		if(error < 0)
		{
			printk(KERN_ERR "%s: gpio_request, err=%d", __func__, error);
			goto err_request_gpio;
		}
		error = gpio_direction_input(switch_data->key2_gpio);
		if(error < 0)
		{
			printk(KERN_ERR "%s: gpio_direction_input, err=%d", __func__, error);
			goto err_set_gpio_input;
		}

		error = request_irq(switch_data->irq_key2, switch_dev_interrupt,
		                    IRQF_TRIGGER_FALLING, "tristate_key2", switch_data);

		if (error)
		{
			dev_err(dev,
			        "request_irq %i failed.\n",
			        switch_data->irq_key2);

			switch_data->irq_key2 = -EINVAL;
			goto err_request_irq;
		}

	}

	switch_data->irq_key3 = gpio_to_irq(switch_data->key3_gpio);
	if (switch_data->irq_key3 <= 0)
	{
		printk("%s, irq number is not specified, irq #= %d, int pin=%d\n\n", __func__, \
		       switch_data->irq_key3, switch_data->key3_gpio);
		goto err_detect_irq_num_failed;
	}
	else
	{
		error = gpio_request(switch_data->key3_gpio,"tristate_key3-int");
		if(error < 0)
		{
			printk(KERN_ERR "%s: gpio_request, err=%d", __func__, error);
			goto err_request_gpio;
		}
		error = gpio_direction_input(switch_data->key3_gpio);
		if(error < 0)
		{
			printk(KERN_ERR "%s: gpio_direction_input, err=%d", __func__, error);
			goto err_set_gpio_input;
		}


		error = request_irq(switch_data->irq_key3, switch_dev_interrupt,
		                    IRQF_TRIGGER_FALLING, "tristate_key3", switch_data);

		if (error)
		{
			dev_err(dev,
			        "request_irq %i failed.\n",
			        switch_data->irq_key3);

			switch_data->irq_key3 = -EINVAL;
			goto err_request_irq;
		}

	}

=======
	// irqs and work, timer stuffs
	//config irq gpio and request irq
	switch_data->irq_key1 = gpio_to_irq(switch_data->key1_gpio);
       if (switch_data->irq_key1 <= 0)
       {
            printk("%s, irq number is not specified, irq #= %d, int pin=%d\n\n", __func__, switch_data->irq_key1, switch_data->key1_gpio);
            goto err_detect_irq_num_failed;
       }
       else
       {
        	error = gpio_request(switch_data->key1_gpio,"tristate_key1-int");
        	if(error < 0)
        	{
        		printk(KERN_ERR "%s: gpio_request, err=%d", __func__, error);
        		goto err_request_gpio;
        	}
        	error = gpio_direction_input(switch_data->key1_gpio);
        	if(error < 0)
        	{
        		printk(KERN_ERR "%s: gpio_direction_input, err=%d", __func__, error);
        		goto err_set_gpio_input;
        	}

			error = request_irq(switch_data->irq_key1, switch_dev_interrupt,
			    IRQF_TRIGGER_FALLING, "tristate_key1", switch_data);

        	if (error) {
        		dev_err(dev,
        			"request_irq %i failed.\n",
        			switch_data->irq_key1);

        		switch_data->irq_key1 = -EINVAL;
        		goto err_request_irq;
            }
       }

	 switch_data->irq_key2 = gpio_to_irq(switch_data->key2_gpio);
       if (switch_data->irq_key2 <= 0)
       {
            printk("%s, irq number is not specified, irq #= %d, int pin=%d\n\n", __func__, switch_data->irq_key2, switch_data->key2_gpio);
            goto err_detect_irq_num_failed;
       }
       else
       {
        	error = gpio_request(switch_data->key2_gpio,"tristate_key2-int");
        	if(error < 0)
        	{
        		printk(KERN_ERR "%s: gpio_request, err=%d", __func__, error);
        		goto err_request_gpio;
        	}
        	error = gpio_direction_input(switch_data->key2_gpio);
        	if(error < 0)
        	{
        		printk(KERN_ERR "%s: gpio_direction_input, err=%d", __func__, error);
        		goto err_set_gpio_input;
        	}

			error = request_irq(switch_data->irq_key2, switch_dev_interrupt,
			    IRQF_TRIGGER_FALLING, "tristate_key2", switch_data);

        	if (error) {
        		dev_err(dev,
        			"request_irq %i failed.\n",
        			switch_data->irq_key2);

        		switch_data->irq_key2 = -EINVAL;
        		goto err_request_irq;
            }

       }

	   switch_data->irq_key3 = gpio_to_irq(switch_data->key3_gpio);
	   if (switch_data->irq_key3 <= 0)
	   {
	            printk("%s, irq number is not specified, irq #= %d, int pin=%d\n\n", __func__, \
	            switch_data->irq_key3, switch_data->key3_gpio);
	            goto err_detect_irq_num_failed;
       }
       else
       {
        	error = gpio_request(switch_data->key3_gpio,"tristate_key3-int");
        	if(error < 0)
        	{
        		printk(KERN_ERR "%s: gpio_request, err=%d", __func__, error);
        		goto err_request_gpio;
        	}
        	error = gpio_direction_input(switch_data->key3_gpio);
        	if(error < 0)
        	{
        		printk(KERN_ERR "%s: gpio_direction_input, err=%d", __func__, error);
        		goto err_set_gpio_input;
        	}

			error = request_irq(switch_data->irq_key3, switch_dev_interrupt,
			    IRQF_TRIGGER_FALLING, "tristate_key3", switch_data);

        	if (error) {
        		dev_err(dev,
        			"request_irq %i failed.\n",
        			switch_data->irq_key3);

        		switch_data->irq_key3 = -EINVAL;
        		goto err_request_irq;
            }

       }
>>>>>>> d25c06f1f573... op5: tri_state_key: allow for additional settings

	INIT_WORK(&switch_data->work, switch_dev_work);

	init_timer(&switch_data->s_timer);
	switch_data->s_timer.function = &timer_handle;
	switch_data->s_timer.expires = jiffies + 5*HZ;

	add_timer(&switch_data->s_timer);

	enable_irq_wake(switch_data->irq_key1);
	enable_irq_wake(switch_data->irq_key2);
	enable_irq_wake(switch_data->irq_key3);


	switch_data->sdev.name = DRV_NAME;
	error = switch_dev_register(&switch_data->sdev);
	if (error < 0)
		goto err_request_gpio;
<<<<<<< HEAD
	}

=======
	//set_gpio_by_pinctrl();
	//report the first switch
	//switch_dev_work(&switch_data->work);
>>>>>>> parent of 2b88a468aa68... input: tri-state: rewritten tri-state-key driver
	return 0;


err_request_gpio:
	switch_dev_unregister(&switch_data->sdev);
err_request_irq:
err_detect_irq_num_failed:
err_set_gpio_input:
	gpio_free(switch_data->key2_gpio);
	gpio_free(switch_data->key1_gpio);
	gpio_free(switch_data->key3_gpio);
err_switch_dev_register:
	kfree(switch_data);
        printk("%s, meet_error\n\n", __func__);

	return error;
}

static int tristate_dev_remove(struct platform_device *pdev)
{
	printk("%s\n",__func__);
	cancel_work_sync(&switch_data->work);
	gpio_free(switch_data->key1_gpio);
	gpio_free(switch_data->key2_gpio);
	gpio_free(switch_data->key3_gpio);
	switch_dev_unregister(&switch_data->sdev);
	kfree(switch_data);

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_OF
static struct of_device_id tristate_dev_of_match[] =
{
	{ .compatible = "oneplus,tri-state-key", },
	{ },
};
MODULE_DEVICE_TABLE(of, tristate_dev_of_match);
#endif

static struct platform_driver tristate_dev_driver =
{
=======
static struct platform_driver tristate_dev_driver = {
>>>>>>> d25c06f1f573... op5: tri_state_key: allow for additional settings
	.probe	= tristate_dev_probe,
	.remove	= tristate_dev_remove,
	.driver	= {
		.name	= DRV_NAME,
		.owner	= THIS_MODULE,
		.of_match_table = tristate_dev_of_match,
	},
};
module_platform_driver(tristate_dev_driver);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("switch Profiles by this triple key driver");

