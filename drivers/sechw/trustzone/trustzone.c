/*
 * Copyright (C) 2014 Javier González
 *
 * Device driver for ARM TrustZone.

 * TODO: Implement hooks to delegate functions to specific TrustZone
 * implementations.
 * TODO: All checkings
 * TODO: Return values
 */

#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>

#include "trustzone.h"
#include "otz_id.h" /* TODO: Should not be here */

static LIST_HEAD(trustzone_chip_list);
static DEFINE_SPINLOCK(driver_lock);
static DECLARE_BITMAP(dev_mask, TRUSTZONE_NUM_DEVICES);

static struct trustzone_chip *tz_chip_find_get(u32 chip_num)
{
	struct trustzone_chip *pos, *chip = NULL;

	rcu_read_lock();
	list_for_each_entry_rcu(pos, &trustzone_chip_list, list) {
		if (chip_num != TRUSTZONE_ANY_NUM && chip_num != pos->dev_num)
			continue;

		/* XXX: Look into this: struct platform_driver otz_driver */
		/* if (try_module_get(pos->dev->driver->owner)) { */
			chip = pos;
			break;
		/* } */
	}
	rcu_read_unlock();
	return chip;
}

int __tz_open_session(struct trustzone_chip *chip,
		struct trustzone_session *session, u8 primitive)
{
	int ret = 0;

	/* FIXME:IMPORTANT We need to create the primitives in TrustZone so that
	 * we can directly communicate with the kernel without having to
	 * translate here.. */

	/* primitive = OTZ_SVC_TCXO; */
	ret = chip->tz_ops.open(primitive, session);
	if (ret) {
		dev_err(chip->dev, "Open session failed in TrustZone chip (id:%d)\n",
				chip->dev_num);
		return ret;
	}

	/* XXX: We need to look into this: struct platform_driver otz_driver
	 * This problem occurs up too;
	 * */
	/* trustzone_chip_put(chip); */
	return ret;
}

int __tz_close_session(struct trustzone_chip *chip,
		struct trustzone_session *tz_session)
{
	int ret = 0;

	ret = chip->tz_ops.close(tz_session);
	if (ret) {
		dev_err(chip->dev, "Close session failed in TrustZone chip (id:%d)\n",
			chip->dev_num);
		return ret;
	}
	/* TODO: look at the trustzone_chip_put(chip) in TPM to see if it is
	 * necessary to take the chip out of a list.
	 */
	return ret;
}

static int __tz_transmit_cmd(struct trustzone_chip *chip,
		struct trustzone_session *session, struct trustzone_cmd *cmd,
		struct trustzone_parameter_list *params)
{
	int ret = 0;

	ret = chip->tz_ops.invoke_command(session, cmd, params);
	if (ret) {
		dev_err(chip->dev, "Transmit command failed in TrustZone chip (id:%d)\n",
				chip->dev_num);
		goto out;
	}
	TZ_DBG("Transmit command succeeded\n");

out:
	return ret;
}

int tz_open_session(u32 chip_num, struct trustzone_session *session,
		u8 primitive)
{
	struct trustzone_chip *chip;
	int ret = 0;

	chip = tz_chip_find_get(chip_num);
	if (chip == NULL) {
		dev_err(chip->dev, "Could not find TrustZone chip (id:%d) registered\n",
				chip_num);
		return -ENODEV;
	}
	mutex_lock(&chip->tz_mutex);
	ret = __tz_open_session(chip, session, primitive);
	mutex_unlock(&chip->tz_mutex);

	return ret;
}
EXPORT_SYMBOL_GPL(tz_open_session);

int tz_close_session(u32 chip_num, struct trustzone_session *tz_session)
{
	struct trustzone_chip *chip;
	int ret = 0;

	chip = tz_chip_find_get(chip_num);
	if (chip == NULL) {
		dev_err(chip->dev, "Could not find TrustZone chip (id:%d) registered\n",
				chip_num);
		return -ENODEV;
	}
	mutex_lock(&chip->tz_mutex);
	ret = __tz_close_session(chip, tz_session);
	mutex_unlock(&chip->tz_mutex);

	return ret;
}
EXPORT_SYMBOL_GPL(tz_close_session);


/* XXX: Maybe we should have a general mapping of sessions where each session
 * has a general ID independently from the chip. This is future work.
 */
static int tz_transmit_cmd(u32 chip_num, struct trustzone_session *session,
		struct trustzone_cmd *cmd,
		struct trustzone_parameter_list *params)
{
	struct trustzone_chip *chip;
	int ret = 0;

	chip = tz_chip_find_get(chip_num);
	if (chip == NULL) {
		dev_err(chip->dev, "Could not find TrustZone chip (id:%d) registered\n",
				chip_num);
		ret = -ENODEV;
		goto out;
	}
	mutex_lock(&chip->tz_mutex);
	ret = __tz_transmit_cmd(chip, session, cmd, params);
	mutex_unlock(&chip->tz_mutex);
	TZ_DBG("Transmit command succeeded\n");

out:
	return ret;
}
EXPORT_SYMBOL_GPL(tz_transmit_cmd);

/* FIXME: Secure system primitives should not be hear since they are system
 * defined
 */
int tz_monitor_syscall(u32 chip_num, struct trustzone_session *tz_session,
		unsigned long sig, siginfo_t *sig_info)
{
	struct trustzone_cmd cmd;
	struct trustzone_session my_tz_session;
	struct trustzone_chip *chip;
	int ret = 0;

	/* TODO: This needs to be sent as a parameter, or made generic */
	cmd.cmd = OTZ_SYSCALL_MONITOR;
	chip = tz_chip_find_get(chip_num);
	if (chip == NULL) {
		dev_err(chip->dev, "Could not find TrustZone chip (id:%d) registered\n",
				chip_num);
		return -ENODEV;
	}
	mutex_lock(&chip->tz_mutex);
	ret = __tz_open_session(chip, &my_tz_session,
		TZ_SECURE_PRIMITIVE_SVC);

	if (ret) {
		dev_err(chip->dev, "Open session failed for SYSCALL_MONITOR");
		return ret;
	}
	ret = __tz_transmit_cmd(chip, &my_tz_session, &cmd, NULL);

	if (ret) {
		dev_err(chip->dev, "Send SYSCALL_MONITOR to SW failed\n");
		goto out;
	}
	ret = __tz_close_session(chip, &my_tz_session);

	if (ret) {
		dev_err(chip->dev, "Close session failed during test\n");
		return ret;
	}
	mutex_unlock(&chip->tz_mutex);

out:
	return ret;
}

/*
 * Perform an operation in the TEE.
 * Performing an operation entails opening a TEE session, sending a single task
 * and closing the TEE session.
 * This function is though when sending a single task to the TEE. FOr sending a
 * series of tasks is better to explicetly opening and closing a session,
 * sending the desired tasks to the TEE in the middle
 */
int tz_send_operation(u32 chip_num, struct trustzone_session *session,
		struct trustzone_cmd *cmd,
		struct trustzone_parameter_list *params)
{
	return tz_transmit_cmd(chip_num, session, cmd, params);
}

int tz_shared_memory_allocate(void)
{
	return 0;
}

int tz_shared_memory_register(void)
{
	return 0;
}

int tz_shared_memory_free(void)
{
	return 0;
}

/**
 * TEST - TODO: Think where this should go
 */

/* TODO: This should be under debug flag. Here we should not know how trustzone
 * session is packed
 * TODO: Use kernel list
 * TODO: Free tz_param_list->value memory that has been allocated
 */

/*
 * Clean up tz_param_list.
 *
 * Clean the list. If list->value has allocated memory, it is up to the user of
 * the list to release the memory and point it to NULL.
 *
 */
static int _clean_test(struct trustzone_parameter_list *tz_param_list)
{
	struct trustzone_parameter *p, *c;

	p = tz_param_list->params;
	while (p != NULL) {
		c = p->nxt;
		if (p->value != NULL)
			kfree(p->value);
		kfree(p);
		p = c;
	}
	p = c = NULL;
	tz_param_list = NULL;

	TZ_DBG("Clean up finished successfully\n");
	return 0;
}

/**
 * Open and close session;
 */
static int _test1(u32 chip_num, u8 target, u8 test_command)
{
	struct trustzone_session tz_session;
	struct trustzone_cmd cmd;
	struct trustzone_chip *chip;
	int ret = 0;

	chip = tz_chip_find_get(chip_num);
	/* Open session */
	ret = tz_open_session(chip_num, &tz_session, target);
	if (ret) {
		dev_err(chip->dev, "Open Session failed during test\n");
		goto out;
	}

	cmd.cmd = test_command;

	/* Send test command without parameters (NULL) */
	ret = tz_transmit_cmd(chip_num, &tz_session, &cmd, NULL);
	if (ret) {
		dev_err(chip->dev, "Send command 1 failed during test\n");
		goto out;
	}

	ret = tz_close_session(chip_num, &tz_session);
	if (ret) {
		dev_err(chip->dev, "Close session failed during test\n");
		goto out;
	}

	TZ_DBG("Test 1 succeeded\n");

out:
	return ret;
}

/**
 * Open and close session with:
 *	- INPUT uint32_t
 *	- OUTPUT uint32_t
 */
static int _test2(u32 chip_num, u8 target, u8 test_command)
{
	struct trustzone_session tz_session;
	struct trustzone_cmd cmd;
	struct trustzone_parameter_list tz_param_list = {
		.n_params = 0,
	};
	struct trustzone_parameter *last_tz_param;
	struct trustzone_chip *chip;
	int ret = 0;

	chip = tz_chip_find_get(chip_num);
	/* Open session */
	ret = tz_open_session(chip_num, &tz_session, target);
	if (ret) {
		dev_err(chip->dev, "Open Session failed during test\n");
		goto out;
	}

	cmd.cmd = test_command;

	/* Parameter1 - uint32 (in) */
	last_tz_param->type = TZ_UINT32;
	last_tz_param->inout = TZ_PARAM_IN;
	last_tz_param->size = sizeof(uint32_t);
	last_tz_param->value = kmalloc(sizeof(uint32_t), GFP_KERNEL);
	if (!last_tz_param->value) {
		dev_err(chip->dev, "kmalloc failed for input parameter\n");
		ret = -ENOMEM;
		goto free;
	}
	*(int *)last_tz_param->value = 33;
	last_tz_param->nxt = NULL;
	tz_param_list.n_params++;

	/* Parameter2 - uint32 (out) */
	last_tz_param->nxt = kmalloc(sizeof(struct trustzone_parameter),
			GFP_KERNEL);
	if (!last_tz_param->nxt) {
		dev_err(chip->dev, "kmalloc failed for input parameter\n");
		ret = -ENOMEM;
		goto free;
	 }
	last_tz_param = last_tz_param->nxt;

	last_tz_param->type = TZ_UINT32;
	last_tz_param->inout = TZ_PARAM_OUT;
	last_tz_param->size = sizeof(uint32_t);
	if (!last_tz_param->value) {
		dev_err(chip->dev, "kmalloc failed for input parameter\n");
		ret = -ENOMEM;
		goto free;
	}
	*(int *)last_tz_param->value = 33;
	last_tz_param->nxt = NULL;
	tz_param_list.n_params++;

	/* Send command */
	ret = tz_transmit_cmd(chip_num, &tz_session, &cmd, &tz_param_list);
	if (ret) {
		dev_err(chip->dev, "Send command 2 failed during test. Ret=%d\n",
				ret);
		goto free;
	}

	/* Output parameter1 (uint32_t) */
	TZ_DBG("Received from SW: %d\n", *(uint32_t *)last_tz_param->value);

	ret = tz_close_session(chip_num, &tz_session);
	if (ret) {
		dev_err(chip->dev, "Close session failed during test\n");
		goto free;
	}

	TZ_DBG("Test 2 succeeded\n");

free:
	/* Local cleanup: allocated values */
	_clean_test(&tz_param_list);
out:
	return ret;
}

/**
 * Open and close session with:
 *	- 2 X INPUT uint32_t
 *	- OUTPUT uint32_t
 *	- INPUT generic
 *	- OUTPUT generic
 */
static int _test3(u32 chip_num, u8 target, u8 test_command)
{
	struct trustzone_session tz_session;
	struct trustzone_cmd cmd;
	struct trustzone_parameter_list tz_param_list = {
		.n_params = 0,
	};
	struct trustzone_parameter *last_tz_param;
	struct trustzone_chip *chip;
	int ret = 0;

	chip = tz_chip_find_get(chip_num);
	/* Open session */
	ret = tz_open_session(chip_num, &tz_session, target);
	if (ret) {
		dev_err(chip->dev, "Open Session failed during test\n");
		goto out;
	}

	cmd.cmd = test_command;
	tz_param_list.params = kmalloc(sizeof(struct trustzone_parameter),
			GFP_KERNEL);
	if (!tz_param_list.params) {
		dev_err(chip->dev, "kmalloc failed for input parameter\n");
		ret = -ENOMEM;
		goto free;
	 }
	last_tz_param = tz_param_list.params;

	/* Parameter1 - uint32 (in) */
	last_tz_param->type = TZ_UINT32;
	last_tz_param->inout = TZ_PARAM_IN;
	last_tz_param->size = sizeof(uint32_t);
	last_tz_param->value = kmalloc(sizeof(uint32_t), GFP_KERNEL);
	if (!last_tz_param->value) {
		dev_err(chip->dev, "kmalloc failed for input parameter\n");
		ret = -ENOMEM;
		goto free;
	}
	*(int *)last_tz_param->value = sizeof("Hola Mundo\n");
	last_tz_param->nxt = NULL;
	tz_param_list.n_params++;

	/* Parameter2 - uint32 (in) */
	last_tz_param->nxt = kmalloc(sizeof(struct trustzone_parameter),
			GFP_KERNEL);
	if (!last_tz_param->nxt) {
		dev_err(chip->dev, "kmalloc failed for input parameter\n");
		ret = -ENOMEM;
		goto free;
	}
	last_tz_param = last_tz_param->nxt;

	last_tz_param->type = TZ_UINT32;
	last_tz_param->inout = TZ_PARAM_IN;
	last_tz_param->size = sizeof(uint32_t);
	last_tz_param->value = kmalloc(sizeof(uint32_t), GFP_KERNEL);
	if (!last_tz_param->value) {
		dev_err(chip->dev, "kmalloc failed for input parameter\n");
		ret = -ENOMEM;
		goto free;
	}
	*(int *)last_tz_param->value = 3;
	last_tz_param->nxt = NULL;
	tz_param_list.n_params++;

	/* Parameter3 - Generic (in) */
	last_tz_param->nxt = kmalloc(sizeof(struct trustzone_parameter),
			GFP_KERNEL);
	if (!last_tz_param->nxt) {
		dev_err(chip->dev, "kmalloc failed for input parameter\n");
		ret = -ENOMEM;
		goto free;
	}
	last_tz_param = last_tz_param->nxt;

	last_tz_param->type = TZ_GENERIC;
	last_tz_param->inout = TZ_PARAM_IN;
	last_tz_param->size = strlen("Hola mundo\n") + 1;
	last_tz_param->value = kmalloc(sizeof("Hola mundo\n"), GFP_KERNEL);
	if (!last_tz_param->value) {
		dev_err(chip->dev, "kmalloc failed for input parameter\n");
		ret = -ENOMEM;
		goto free;
	}
	strcpy(last_tz_param->value, "Hola mundo\n");
	last_tz_param->nxt = NULL;
	tz_param_list.n_params++;

	/* Parameter4 - Generic (out) */
	last_tz_param->nxt = kmalloc(sizeof(struct trustzone_parameter),
			GFP_KERNEL);
	if (!last_tz_param->nxt) {
		dev_err(chip->dev, "kmalloc failed for input parameter\n");
		ret = -ENOMEM;
		goto free;
	}
	last_tz_param = last_tz_param->nxt;
	last_tz_param->type = TZ_GENERIC;
	last_tz_param->inout = TZ_PARAM_OUT;
	last_tz_param->size = 512;
	last_tz_param->value = kmalloc(512, GFP_KERNEL);
	if (!last_tz_param->value) {
		dev_err(chip->dev, "kmalloc failed for input parameter\n");
		ret = -ENOMEM;
		goto free;
	}
	last_tz_param->nxt = NULL;
	tz_param_list.n_params++;

	/* Send command */
	ret = tz_transmit_cmd(chip_num, &tz_session, &cmd, &tz_param_list);
	if (ret) {
		dev_err(chip->dev, "Send command 2 failed during test. Ret=%d\n",
				ret);
		goto free;
	}

	/* Output parameter1 (generic) */
	TZ_DBG("Received from SW: %s\n", (char *)last_tz_param->value);

	ret = tz_close_session(chip_num, &tz_session);
	if (ret) {
		dev_err(chip->dev, "Close session failed during test\n");
		goto free;
	}

	TZ_DBG("Test 3 succeeded\n");

free:
	_clean_test(&tz_param_list);
out:
	return ret;
}

int tz_send_test_operation(u32 chip_num, u8 tz_impl)
{
	struct trustzone_chip *chip;
	u8 target, test_command;
	int ret = 0;

	/* TODO: Make test generic, not driver-specific */
	chip = tz_chip_find_get(chip_num);
	if (tz_impl == TZ_OPEN_VIRTUALIZATION) {
		target = OTZ_SVC_TCXO;
		test_command = OTZ_TCXO_TEST_CMD;
	} else {
		dev_err(chip->dev, "Unknown test implementation\n");
		goto out;
	}

	/*
	 * Test 1: Open and close session;
	 */

	/*
	 * Test 2: Open and close session with:
	 *	- INPUT uint32_t
	 *	- OUTPUT uint32_t
	 */

	/*
	 * Test3: Open and clase session with:
	 *	- 2 X INPUT uint32_t
	 *	- OUTPUT uint32_t
	 *	- INPUT generic
	 *	- OUTPUT generic
	 */
	ret = _test3(chip_num, target, test_command);

out:
	return ret;
}

/*
 * If the vendor provides a release function, call it too
 */
void trustzone_vendor_release(struct trustzone_chip *chip)
{
	if (!chip)
		return;

	if (chip->tz_ops.release)
		chip->tz_ops.release(chip->dev);

	kfree(chip->tz_ops.miscdev.name);
}

static void trustzone_dev_release(struct device *dev)
{
	/* FIXME: You need to fix all this crap... */
	/* struct trustzone_chip *chip = dev_get_drvdata(dev); */
	/* struct trustzone_chip *chip; */

	/* if (!chip) */
		/* return; */

	/* trustzone_vendor_release(chip); */

	/* chip->release(dev); */
	/* kfree(chip); */
}
EXPORT_SYMBOL_GPL(trustzone_dev_release);

struct trustzone_chip *trustzone_register_hardware(struct device *dev,
		const struct trustzone_operations *entry)
{
	char *devname;
	struct trustzone_chip *chip;

	chip = kzalloc(sizeof(*chip), GFP_KERNEL);
	devname = kmalloc(TRUSTZONE_NAME_MAX, GFP_KERNEL);

	if (chip == NULL || devname == NULL)
		goto out_free;

	/* TODO: All mutexes and timers, as they do in the TPM module */
	mutex_init(&chip->tz_mutex);
	INIT_LIST_HEAD(&chip->list);
	memcpy(&chip->tz_ops, entry, sizeof(struct trustzone_operations));
	chip->dev_num = find_first_zero_bit(dev_mask, TRUSTZONE_NUM_DEVICES);

	if (chip->dev_num >= TRUSTZONE_NUM_DEVICES) {
		dev_err(dev, "No available trustzone device numbers\n");
		goto out_free;
	} else if (chip->dev_num == 0)
		chip->tz_ops.miscdev.minor = MISC_DYNAMIC_MINOR;

	set_bit(chip->dev_num, dev_mask);
	scnprintf(devname, TRUSTZONE_NAME_MAX, "%s%d", "tz", chip->dev_num);
	chip->tz_ops.miscdev.name = devname;
	chip->tz_ops.miscdev.parent = dev;
	chip->dev = get_device(dev);
	chip->release = dev->release;
	dev->release = trustzone_dev_release;
	dev_set_drvdata(dev, chip);

	if (misc_register(&chip->tz_ops.miscdev)) {
		dev_err(chip->dev,
				"unable to misc_register %s, minor %d\n",
				chip->tz_ops.miscdev.name,
				chip->tz_ops.miscdev.minor);
		goto put_device;
	}
	/* TODO: This needs to be added when we want to have an interface to
	 * sysfs directly to the trustzone generic interface
	if (sysfs_create_group(&dev->kobj, chip->tz_ops.attr_group)) {
		misc_deregister(&chip->tz_ops.miscdev);
		goto put_device;
	}
	*/

	/* XXX: We are missing a tpm_add_ppi here. Seems to be TPM specific but
	 * we need to figure out how it works and why it is there.
	 */

	/* Make chip available */
	spin_lock(&driver_lock);
	list_add_rcu(&chip->list, &trustzone_chip_list);
	spin_unlock(&driver_lock);

	return chip;

put_device:
	put_device(chip->dev);
out_free:
	if (chip != NULL)
		kfree(chip);
	if (devname != NULL)
		kfree(devname);
	return NULL;
}
EXPORT_SYMBOL_GPL(trustzone_register_hardware);

MODULE_AUTHOR("Javier González (jgon@itu.dk)");
MODULE_DESCRIPTION("TrustZone Driver");
MODULE_VERSION("1.0");
MODULE_LICENSE("GPL");
