/*
 * Copyright (C) 2014 Javier González <javier@javigon.com>
 *
 * Generic device driver for ARM TrustZone.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * TODO: Revise return values
 */

#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>

#include "trustzone.h"

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

static int __tz_open(struct trustzone_chip *chip,
		struct trustzone_session *session, u8 service_id)
{
	int ret = 0;

	ret = chip->tz_ops.open(service_id, session);
	if (ret) {
		dev_err(chip->dev, "Open session failed in TrustZone"
				" chip (id:%d)\n", chip->dev_num);
		return ret;
	}

	/* TODO: We need to look into this: struct platform_driver otz_driver
	 * This problem occurs up too;
	 * */
	/* trustzone_chip_put(chip); */
	dev_dbg(chip->dev, "TrustZone open session succeeded\n");
	return ret;
}

static int __tz_close(struct trustzone_chip *chip,
		struct trustzone_session *tz_session)
{
	int ret = 0;

	ret = chip->tz_ops.close(tz_session);
	if (ret) {
		dev_err(chip->dev, "Close session failed in TrustZone chip"
				" (id:%d)\n", chip->dev_num);
		return ret;
	}
	/* TODO: Look at the trustzone_chip_put(chip) to see if it is
	 * necessary to take the chip out of a list. As done in open
	 */

	dev_dbg(chip->dev, "TrustZone close session succeeded\n");
	return ret;
}

static int __tz_invoke(struct trustzone_chip *chip,
		struct trustzone_session *session, struct trustzone_cmd *cmd,
		struct trustzone_parameter_list *params)
{
	int ret = 0;

	ret = chip->tz_ops.invoke_command(session, cmd, params);
	if (ret) {
		dev_err(chip->dev, "Transmit command failed in TrustZone chip"
				" (id:%d)\n", chip->dev_num);
		return ret;
	}
	dev_dbg(chip->dev, "TrustZone transmit command succeeded\n");

	return ret;
}

/**
 * tz_open() - open a TEE session
 * @chip_num: TrustZone chip idenfier. If wanting default use TRUSTZONE_ANY_NUM
 * @session: TEE session. It is passed allocated and returned filled by the TEE.
 * TEE clients do not need the session, but they need to keep it to invoke
 * commands and close the session, so that the TEE can identify it.
 * @service_id: service to be opened by the session. Services accept then
 * commands that they can interpret.
 *
 * Open a TEE session. The TEE tries to to open a new session and allocates
 * resources in the TEE for it. Sessions are opened for a given, known trusted
 * service. Known trusted services for kernel submodules are in tz_services.
 * If the session is opened succesfully, a session is return to the TEE client.
 *
 * Locks are maintained by this function when communicating with the TEE. See
 * __tz_open() for a function that does not maintain locks.
 *
 * Return: return codes defined in TODO: Look common errors in OTZ and OP-TEE
 */
int tz_open(u32 chip_num, struct trustzone_session *session,
		u8 service_id)
{
	struct trustzone_chip *chip;
	int ret = 0;

	chip = tz_chip_find_get(chip_num);
	if (chip == NULL) {
		dev_err(chip->dev, "Could not find TrustZone chip (id:%d)"
				" registered\n", chip_num);
		return -ENODEV;
	}
	mutex_lock(&chip->tz_mutex);
	ret = __tz_open(chip, session, service_id);
	mutex_unlock(&chip->tz_mutex);

	return ret;
}
EXPORT_SYMBOL_GPL(tz_open);

/**
 * tz_close() - close a TEE session
 * @chip_num: TrustZone chip idenfier. If wanting default use TRUSTZONE_ANY_NUM
 * @session: Filled TEE session.
 *
 * Close a TEE session.
 *
 * Locks are maintained by this function when communicating with the TEE. See
 * __tz_close() for a function that does not maintain locks.
 *
 * Return: TODO
 */
int tz_close(u32 chip_num, struct trustzone_session *tz_session)
{
	struct trustzone_chip *chip;
	int ret = 0;

	chip = tz_chip_find_get(chip_num);
	if (chip == NULL) {
		dev_err(chip->dev, "Could not find TrustZone chip (id:%d)"
				" registered\n", chip_num);
		return -ENODEV;
	}
	mutex_lock(&chip->tz_mutex);
	ret = __tz_close(chip, tz_session);
	mutex_unlock(&chip->tz_mutex);
	
	return ret;
}
EXPORT_SYMBOL_GPL(tz_close);

/**
 * tz_invoke() - invoke a TEE command
 * @chip_num: TrustZone chip idenfier. If wanting default use TRUSTZONE_ANY_NUM
 * @session: filled TEE session.
 * @cmd: TEE command that is interpreted by the TEE service that opened the
 * session.
 * @params: list of parameters to be sent together with the command.
 *
 * Invoke a TEE command. Commands are supposed to be interpreted by the TEE
 * service that opened the session. It is the client TEE's responsability to
 * known the commands that are supported by the TEE service.
 *
 * There are no restrictions regarding number of  parameters, and can contain as
 * many parameters as wanted (or supported by each specific TEE framework).
 *
 * Locks are maintained by this function when communicating with the TEE. See
 * __tz_invoke() for a function that does not maintain locks.
 *
 * Return: TODO
 */
int tz_invoke(u32 chip_num, struct trustzone_session *session,
		struct trustzone_cmd *cmd,
		struct trustzone_parameter_list *params)
{
	struct trustzone_chip *chip;
	int ret = 0;

	chip = tz_chip_find_get(chip_num);
	if (chip == NULL) {
		dev_err(chip->dev, "Could not find TrustZone chip (id:%d)"
				" registered\n", chip_num);
		return -ENODEV;
	}
	mutex_lock(&chip->tz_mutex);
	ret = __tz_invoke(chip, session, cmd, params);
	mutex_unlock(&chip->tz_mutex);

	return ret;
}
EXPORT_SYMBOL_GPL(tz_invoke);

/**
 * tz_atomic_operation() - send a command atomically to the TEE
 * @chip_num: TrustZone chip idenfier. If wanting default use TRUSTZONE_ANY_NUM
 * @service_id: service to be opened by the session. Services accept then
 * commands that they can interpret.
 * @cmd: TEE command that is interpreted by the TEE service that opened the
 * session.
 * @params: list of parameters to be sent together with the command.
 *
 * Performing an atomic operation entails opening a TEE session to a service,
 * sending a single command to the TEE, and closing the TEE session. No session
 * has then to be maintained by the TEE client.
 *
 * This operation is recommended when sending a single task to the TEE. For
 * sending a series of tasks is better to explicetly opening and closing a
 * session, sending the desired tasks to the TEE in the middle.
 *
 * Return: TODO:
 */
int tz_atomic_operation(u32 chip_num, u8 service_id,
		struct trustzone_cmd *cmd,
		struct trustzone_parameter_list *params)
{
	struct trustzone_chip *chip;
	struct trustzone_session session;
	int ret = 0;

	chip = tz_chip_find_get(chip_num);
	if (chip == NULL) {
		dev_err(chip->dev, "Could not find TrustZone chip (id:%d)"
				" registered\n", chip_num);
		ret = -ENODEV;
		goto out;
	}

	mutex_lock(&chip->tz_mutex);
	ret = __tz_open(chip, &session, service_id);
	if (ret)
		goto out;

	ret = __tz_invoke(chip, &session, cmd, params);
	if (ret)
		goto out_close;

out_close:
	ret = __tz_close(chip, &session);
out:
	mutex_unlock(&chip->tz_mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(tz_atomic_operation);

/*
 * If the vendor provides a release function, call it too
 */
static void trustzone_vendor_release(struct trustzone_chip *chip)
{
	if (!chip)
		return;

	if (chip->tz_ops.release)
		chip->tz_ops.release(chip->dev);

	kfree(chip->tz_ops.miscdev.name);
}

static void trustzone_dev_release(struct device *dev)
{
	struct trustzone_chip *chip = dev_get_drvdata(dev);

	if (!chip)
		return;

	trustzone_vendor_release(chip);

	chip->release(dev);
	kfree(chip);
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

	/* TODO: Add sysfs interface
	if (sysfs_create_group(&dev->kobj, chip->tz_ops.attr_group)) {
		misc_deregister(&chip->tz_ops.miscdev);
		goto put_device;
	}
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

MODULE_AUTHOR("Javier González (javier@javigon.com)");
MODULE_DESCRIPTION("TrustZone Generic Driver");
MODULE_VERSION("1.0");
MODULE_LICENSE("GPL");
