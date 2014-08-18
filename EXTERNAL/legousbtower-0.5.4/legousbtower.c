/*
 * LEGO USB Tower driver - 0.5
 *
 * Copyright (c) 2001 Juergen Stuber <stuber@loria.fr>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation; either version 2 of
 *	the License, or (at your option) any later version.
 *
 * derived from USB Skeleton driver - 0.5
 * Copyright (c) 2001 Greg Kroah-Hartman (greg@kroah.com)
 *
 * History:
 *
 * 2001-10-13 - 0.1 js
 *   - first version
 * 2001-11-03 - 0.2 js
 *   - simplified buffering, one-shot URBs for writing
 * 2001-11-10 - 0.3 js
 *   - removed IOCTL (setting power/mode is more complicated, postponed)
 * 2001-11-28 - 0.4 js
 *   - added vendor commands for mode of operation and power level in open
 * 2001-12-04 - 0.5 js
 *   - set IR mode by default (by oversight 0.4 set VLL mode)
 * 2002-01-11 - 0.5? pcchan
 *   - make read buffer reusable and work around bytes_to_write issue between
 *     uhci and legusbtower
 * 2002-09-23 - 0.52 david (david@csse.uwa.edu.au)
 *   - imported into lejos project
 *   - changed wake_up to wake_up_interruptible
 *   - changed to use lego0 rather than tower0
 *   - changed dbg() to use __func__ rather than deprecated __FUNCTION__
 * 2003-01-12 - 0.53 david (david@csse.uwa.edu.au)
 *   - changed read and write to write everything or timeout (from a patch by Chris Riesen and 
 *     Brett Thaeler driver)
 *   - added ioctl functionality to set timeouts
 * 2003-07-18 - 0.54 davidgsf (david@csse.uwa.edu.au) 
 *   - initial import into LegoUSB project
 *   - merge of existing LegoUSB.c driver
 */

#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/errno.h>
#include <linux/poll.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/fcntl.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/smp_lock.h>
#include <linux/devfs_fs_kernel.h>
#include <linux/usb.h>
#include "legousbtower.h"

#ifndef USB_DIR_MASK
#define USB_DIR_MASK 0x80       /* should be in usb.h */
#endif

#ifdef CONFIG_USB_DEBUG
	static int debug = 4;
#else
	static int debug = 4;
#endif

/* Use our own dbg macro */
#undef dbg
#define dbg(lvl, format, arg...) do { if (debug >= lvl) printk(KERN_DEBUG  __FILE__ " : " format " \n", ## arg); } while (0)


/* Version Information */
#define DRIVER_VERSION "v0.54"
#define DRIVER_AUTHOR ""
#define DRIVER_DESC "LEGO USB Tower Driver"

/* Module paramaters */
MODULE_PARM(debug, "i");
MODULE_PARM_DESC(debug, "Debug enabled or not");


/* Define these values to match your device */
#define LEGO_USB_TOWER_VENDOR_ID	0x0694
#define LEGO_USB_TOWER_PRODUCT_ID	0x0001

/* table of devices that work with this driver */
static struct usb_device_id tower_table [] = {
	{ USB_DEVICE(LEGO_USB_TOWER_VENDOR_ID, LEGO_USB_TOWER_PRODUCT_ID) },
	{ }					/* Terminating entry */
};

MODULE_DEVICE_TABLE (usb, tower_table);

/* FIXME: Get a minor range for your devices from the usb maintainer */
#define LEGO_USB_TOWER_MINOR_BASE	0xB3

/* we can have up to this number of device plugged in at once */
#define MAX_DEVICES		16

/* number of configurations */
#define MAX_CONFIGURATION	4

#define COMMAND_TIMEOUT         (2*HZ)  /* 60 second timeout for a command */
#define LEGO_CMD_COMPLETE  1
#define LEGO_CMD_FAILURE   2


/* Structure to hold all of our device specific stuff */
struct lego_usb_tower {
	struct semaphore	sem;		/* locks this structure */
	struct usb_device* 	udev;		/* save off the usb device pointer */
	devfs_handle_t		devfs;		/* devfs device node */
	unsigned char		minor;		/* the starting minor number for this device */

	int			open_count;	/* number of times this port has been opened */

	char*			read_buffer;
        int			read_buffer_length;

	wait_queue_head_t	read_wait;
	wait_queue_head_t	write_wait;

	char*			interrupt_in_buffer;
	struct usb_endpoint_descriptor* interrupt_in_endpoint;
	struct urb*		interrupt_in_urb;

	char*			interrupt_out_buffer;
	struct usb_endpoint_descriptor* interrupt_out_endpoint;
	struct urb*		interrupt_out_urb;

                                /* device specific: */

	int                     read_timeout;
	int                     write_timeout;
};

/* Note that no locking is needed:
 * read_buffer is arbitrated by read_buffer_length == 0
 * interrupt_out_buffer is arbitrated by interrupt_out_urb->status == -EINPROGRESS
 * interrupt_in_buffer belongs to urb alone and is overwritten on overflow
 */

/* the global usb devfs handle */
extern devfs_handle_t usb_devfs_handle;


/* local function prototypes */
static ssize_t tower_read	(struct file *file, char *buffer, size_t count, loff_t *ppos);
static ssize_t tower_write	(struct file *file, const char *buffer, size_t count, loff_t *ppos);
static int tower_ioctl		(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg);
static inline void tower_delete (struct lego_usb_tower *dev);
static int tower_open		(struct inode *inode, struct file *file);
static int tower_release	(struct inode *inode, struct file *file);
static int tower_release_internal (struct lego_usb_tower *dev);
static void tower_abort_transfers (struct lego_usb_tower *dev);
static void tower_interrupt_in_callback (struct urb *urb);
static void tower_interrupt_out_callback (struct urb *urb);

static void* tower_probe	(struct usb_device *dev, unsigned int ifnum, const struct usb_device_id *id);
static void tower_disconnect	(struct usb_device *dev, void *ptr);


/* array of pointers to our devices that are currently connected */
static struct lego_usb_tower		*minor_table[MAX_DEVICES];

/* lock to protect the minor_table structure */
static DECLARE_MUTEX (minor_table_mutex);

/* file operations needed when we register this driver */
static struct file_operations tower_fops = {
	owner:		THIS_MODULE,
	read:		tower_read,
	write:		tower_write,
	ioctl:		tower_ioctl,
	open:		tower_open,
	release:	tower_release,
};


/* usb specific object needed to register this driver with the usb subsystem */
static struct usb_driver tower_driver = {
	name:		"legousbtower",
	probe:		tower_probe,
	disconnect:	tower_disconnect,
	fops:		&tower_fops,
	minor:		LEGO_USB_TOWER_MINOR_BASE,
	id_table:	tower_table,
};


/**
 *	lego_usb_tower_debug_data
 */
static inline void lego_usb_tower_debug_data (int level, const char *function, int toggle, int size, const unsigned char *data)
{
	int i;

	if (debug < level)
		return; 
	
	printk (KERN_DEBUG __FILE__": %s - toggle = %d, length = %d, data = ", 
		function, toggle, size);
	for (i = 0; i < size; ++i) {
		printk ("%.2x ", data[i]);
	}
	printk ("\n");
}


/**
 *	tower_delete
 */
static inline void tower_delete (struct lego_usb_tower *dev)
{
	dbg(2, "%s enter", __func__);

	minor_table[dev->minor] = NULL;
        tower_abort_transfers (dev);

        /* free data structures */
	if (dev->interrupt_in_urb != NULL) {
		usb_free_urb (dev->interrupt_in_urb);
       }
	if (dev->interrupt_out_urb != NULL) {
		usb_free_urb (dev->interrupt_out_urb);
        }
	if (dev->read_buffer != NULL) {
		kfree (dev->read_buffer);
        }
	if (dev->interrupt_in_buffer != NULL) {
		kfree (dev->interrupt_in_buffer);
        }
	if (dev->interrupt_out_buffer != NULL) {
		kfree (dev->interrupt_out_buffer);
        }
	kfree (dev);

	dbg(2, "%s : leave", __func__);
}


/**
 *	tower_open
 */
static int tower_open (struct inode *inode, struct file *file)
{
	struct lego_usb_tower *dev = NULL;
	int subminor;
	int retval = 0;
	
	dbg(2,"%s : enter", __func__);

	subminor = MINOR (inode->i_rdev) - LEGO_USB_TOWER_MINOR_BASE;
	if ((subminor < 0) ||
	    (subminor >= MAX_DEVICES)) {
		retval = -ENODEV;
                goto exit;
	}

	/* increment our usage count for the module */
	MOD_INC_USE_COUNT;

	/* lock our minor table and get our local data for this minor */
	down (&minor_table_mutex);
	dev = minor_table[subminor];
	if (dev == NULL) {
		retval = -ENODEV;
                goto unlock_minor_table_exit;
	}

	/* lock this device */
	down (&dev->sem);

	if (dev->open_count == 0) {
		dev->read_timeout = 0;
		dev->write_timeout = COMMAND_TIMEOUT;
	}
        
	/* increment our usage count for the device */
	++dev->open_count;

	/* check that nobody else is using the device */
        /* NOTE: the cleanup code will decrement the count to 1 */
        if (dev->open_count > 1) {
                retval = -EBUSY;
                goto error;
        }
	/* save device in the file's private structure */
	file->private_data = dev;


        /* initialize in direction */
        dev->read_buffer_length = 0;
        /* start reading */
        FILL_INT_URB(
                dev->interrupt_in_urb,
                dev->udev,
                usb_rcvintpipe(dev->udev, dev->interrupt_in_endpoint->bEndpointAddress),
                dev->interrupt_in_buffer,
                dev->interrupt_in_endpoint->wMaxPacketSize,
                tower_interrupt_in_callback,
                dev,
                dev->interrupt_in_endpoint->bInterval);

        retval = usb_submit_urb (dev->interrupt_in_urb);

        if (retval != 0) {
                err("Couldn't submit interrupt_in_urb");
                goto error;
        }

        /* initialize out direction */
        /* nothing to do */
        goto unlock_exit;

 error:
        tower_release_internal (dev);

 unlock_exit:
	/* unlock this device */
	up (&dev->sem);

 unlock_minor_table_exit:
	/* unlock the minor table */
	up (&minor_table_mutex);
        if (retval != 0) {
                MOD_DEC_USE_COUNT;
        }

 exit:
	dbg(2,"%s : leave, return value %d ", __func__, retval);

	return retval;
}

/**
 *	tower_release
 */
static int tower_release (struct inode *inode, struct file *file)
{
	struct lego_usb_tower *dev;
	int retval = 0;

	dbg(2," %s : enter", __func__);

	dev = (struct lego_usb_tower *)file->private_data;

	if (dev == NULL) {
	        dbg(1," %s : object is NULL", __func__);
		retval = -ENODEV;
                goto exit;
	}

	/* lock our minor table */
	down (&minor_table_mutex);

	/* lock our device */
	down (&dev->sem);

 	if (dev->open_count <= 0) {
        	dbg(1," %s : device not opened", __func__);
                up (&dev->sem);
                up (&minor_table_mutex);
		retval = -ENODEV;
		goto exit;
	}

        /* do the work */
        retval = tower_release_internal (dev);

	up (&dev->sem);
	up (&minor_table_mutex);

	/* decrement our usage count for the module */
	MOD_DEC_USE_COUNT;

 exit:
	dbg(2," %s : leave, return value %d", __func__, retval);
	return retval;
}


/**
 *	tower_release_internal
 *      assumes minor_table and device are locked
 */
static int tower_release_internal (struct lego_usb_tower *dev)
{
	int retval = 0;

	dbg(2," %s : enter", __func__);

	if (dev->udev == NULL) {
		/* the device was unplugged before the file was released */
		tower_delete (dev);
		goto exit;
	}

	/* decrement our usage count for the device */
	--dev->open_count;
	if (dev->open_count <= 0) {
                tower_abort_transfers (dev);
		dev->open_count = 0;
	}

 exit:
	dbg(2," %s : leave", __func__);

	return retval;
}


/**
 *	tower_abort_transfers
 *      aborts transfers and frees associated data structures
 */
static void tower_abort_transfers (struct lego_usb_tower *dev)
{
	dbg(2," %s : enter", __func__);

        if (dev == NULL) {
        	dbg(1," %s : dev is null", __func__);
                goto exit;
        }

        /* shutdown transfer */
	if (dev->interrupt_in_urb != NULL) {
                usb_unlink_urb (dev->interrupt_in_urb);
        }
	if (dev->interrupt_out_urb != NULL) {
                usb_unlink_urb (dev->interrupt_out_urb);
        }

 exit:
	dbg(2," %s : leave", __func__);
}


/**
 *	tower_read
 */
static ssize_t tower_read (struct file *file, char *buffer, size_t count, loff_t *ppos)
{
        struct lego_usb_tower *dev;
        size_t bytes_read = 0;
        size_t bytes_to_read;
        int i;
        int retval = 0;
	int timeout = 0;

	dbg(2," %s : enter, count = %d", __func__, count);

	dev = (struct lego_usb_tower *)file->private_data;
	
	/* lock this object */
	down (&dev->sem);

	/* verify that the device wasn't unplugged */
	if (dev->udev == NULL) {
		retval = -ENODEV;
		up (&dev->sem);
		err("No device or device unplugged %d", retval);
		return retval;
	}
	
	/* verify that we actually have some data to read */
	if (count == 0) {
          	dbg(1," %s : read request of 0 bytes", __func__);
        	goto exit;
	}

	timeout = dev->read_timeout;

	while (1) {
		if (dev->read_buffer_length == 0) {

			if (timeout <= 0) {
			        retval = -ETIMEDOUT;
			        goto exit;
			}

			if (signal_pending(current)) {

				retval = -EINTR;
				goto exit;
			}

			up (&dev->sem);
			timeout = interruptible_sleep_on_timeout (&dev->read_wait, timeout);
			down (&dev->sem);

                } else {
                        /* copy the data from read_buffer into userspace */
                        bytes_to_read = count > dev->read_buffer_length ? dev->read_buffer_length : count;
                        if (copy_to_user (buffer, dev->read_buffer, bytes_to_read) != 0) {
                                retval = -EFAULT;
                                goto exit;
                        }
                        dev->read_buffer_length -= bytes_to_read;
                        for (i=0; i<dev->read_buffer_length; i++) {
                                dev->read_buffer[i] = dev->read_buffer[i+bytes_to_read];
                        }

                        buffer += bytes_to_read;
                        count -= bytes_to_read;
                        bytes_read += bytes_to_read;
			if (count == 0) {
				break;
			}
                }


        }

        retval = bytes_read;

 exit:
	/* unlock the device */
	up (&dev->sem);

	dbg(2," %s : leave, return value %d", __func__, retval);
	return retval;
}


/**
 *	tower_write
 */
static ssize_t tower_write (struct file *file, const char *buffer, size_t count, loff_t *ppos)
{
	struct lego_usb_tower *dev;
	size_t bytes_written = 0;
	size_t bytes_to_write;
        size_t buffer_size;
	int retval = 0;
	int timeout = 0;

	dbg(2," %s : enter, count = %d", __func__, count);

	dev = (struct lego_usb_tower *)file->private_data;

	/* lock this object */
	down (&dev->sem);

	/* verify that the device wasn't unplugged */
	if (dev->udev == NULL) {
		retval = -ENODEV;
		up (&dev->sem);
		err("No device or device unplugged %d", retval);
		return retval;
	}

	/* verify that we actually have some data to write */
	if (count == 0) {
        	dbg(1," %s : write request of 0 bytes", __func__);
		goto exit;
	}


        while (count > 0) {
                if (dev->interrupt_out_urb->status == -EINPROGRESS) {
		        timeout = dev->write_timeout;
			dbg(1," %s : enter timeout: %d", __func__, timeout);
			while (timeout > 0) {
				if (signal_pending(current)) {
					dbg(1," %s : interrupted", __func__);
					return -EINTR;
				}
				up (&dev->sem);
				timeout = interruptible_sleep_on_timeout (&dev->write_wait, timeout);
				down (&dev->sem);
				if (timeout > 0) {
					break;
				}
				dbg(1," %s : interrupted timeout: %d", __func__, timeout);
			}

			dbg(1," %s : final timeout: %d", __func__, timeout);
			if (timeout == 0) {
				dbg(1, "%s - command timed out.", __func__);
				retval = -ETIMEDOUT;
				goto exit;
			}

                	dbg(4," %s : in progress, count = %d", __func__, count);

                        // retval = wait_event_interruptible (dev->write_wait, dev->interrupt_out_urb->status != -EINPROGRESS);

                        // if (retval == -ERESTARTSYS) {
			// dbg(4," %s : retval = -ERESTARTSYS", __func__);
			// goto exit;
			// }
                } else {
		        dbg(4," %s : sending, count = %d", __func__, count);

                        /* write the data into interrupt_out_buffer from userspace */
                        buffer_size = dev->interrupt_out_endpoint->wMaxPacketSize;
                        bytes_to_write = count > buffer_size ? buffer_size : count;
			dbg(4," %s : buffer_size = %d, count = %d, bytes_to_write = %d", __func__, buffer_size, count, bytes_to_write);

                        if (copy_from_user (dev->interrupt_out_buffer, buffer, bytes_to_write) != 0) {
                                retval = -EFAULT;
                                goto exit;
                        }

                        /* send off the urb */
                        FILL_INT_URB(
                                dev->interrupt_out_urb,
                                dev->udev, 
                                usb_sndintpipe(dev->udev, dev->interrupt_out_endpoint->bEndpointAddress),
                                dev->interrupt_out_buffer,
                                bytes_to_write,
                                tower_interrupt_out_callback,
                                dev,
                                0);

                        dev->interrupt_out_urb->actual_length = bytes_to_write;
                        retval = usb_submit_urb (dev->interrupt_out_urb);

                        if (retval != 0) {
                                err("Couldn't submit interrupt_out_urb");
                                goto exit;
                        }

                        buffer += bytes_to_write;
                        count -= bytes_to_write;

                        bytes_written += bytes_to_write;
                }
        }

        retval = bytes_written;

 exit:
	/* unlock the device */
	up (&dev->sem);

	dbg(2," %s : leave, return value %d", __func__, retval);

	return retval;
}


/**
 *	tower_ioctl
 */
static int tower_ioctl (struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	struct lego_usb_tower *dev;
        int retval =  -ENOTTY;  /* default: we don't understand ioctl */

	dbg(2," %s : enter, cmd 0x%.4x, arg %ld", __func__, cmd, arg);

	dev = (struct lego_usb_tower *)file->private_data;

	/* lock this object */
	down (&dev->sem);

	/* verify that the device wasn't unplugged */
	if (dev->udev == NULL) {
		retval = -ENODEV;
                goto unlock_exit;
	}

        switch (cmd) {

	case LEGO_TOWER_SET_PARAM: {
		struct request_reply b;

          	dbg(2, "%s : SET_PARAM  0x%04lx", __func__, arg);

		retval = usb_control_msg (dev->udev,
					  usb_rcvctrlpipe(dev->udev, 0),
					  LEGO_USB_TOWER_REQUEST_SET,
					  USB_TYPE_VENDOR | USB_DIR_IN | USB_RECIP_DEVICE,
					  ((int) arg),
					  0,
					  &b,
					  sizeof(b),
					  COMMAND_TIMEOUT);

		if (retval >= 0) {
			if (b.error_code != 0) {
				err("%s : LEGO_TOWER_SET_PARAM - error returned  0x%02x", __func__, 
				    b.error_code);
				retval = -b.error_code;
			}
		}
	}
		break;


	case LEGO_TOWER_RESET: {
		struct request_reply b;

          	dbg(2, "%s : RESET", __func__);

		retval = usb_control_msg (dev->udev,
					  usb_rcvctrlpipe(dev->udev, 0),
					  LEGO_USB_TOWER_REQUEST_RESET,
					  USB_TYPE_VENDOR | USB_DIR_IN | USB_RECIP_DEVICE,
					  0,
					  0,
					  &b,
					  sizeof(b),
					  COMMAND_TIMEOUT);

		if (retval >= 0) {
			if (b.error_code != 0) {
				err("%s : LEGO_TOWER_RESET - error 0x%02x", __func__, b.error_code);
				retval = -b.error_code;
			}
		}
	}
		break;

	case LEGO_TOWER_SET_READ_TIMEOUT:
		retval = 0;
		dev->read_timeout = ((int) arg);
		break;
	case LEGO_TOWER_SET_WRITE_TIMEOUT:
		retval = 0;
		dev->write_timeout = ((int) arg);
		break;
	default:
		retval = -ENOTTY;
		break;

        }
	
 unlock_exit:
	/* unlock the device */
	up (&dev->sem);

	dbg(2," %s : leave, return value %d", __func__, retval);

	return retval;
}


/**
 *	tower_interrupt_in_callback
 */
static void tower_interrupt_in_callback (struct urb *urb)
{
	struct lego_usb_tower *dev = (struct lego_usb_tower *)urb->context;

	dbg(4," %s : enter, status %d", __func__, urb->status);

        lego_usb_tower_debug_data(5,__func__, usb_pipedata(urb->pipe), urb->actual_length, urb->transfer_buffer);

        if (urb->status != 0) {
                if ((urb->status != -ENOENT) && (urb->status != -ECONNRESET)) {
		        dbg(1," %s : nonzero status received: %d", __func__, urb->status);
                }
                goto exit;
        }

        down (&dev->sem);

        if (urb->actual_length > 0) {
                if (dev->read_buffer_length <
                    (4 * dev->interrupt_in_endpoint->wMaxPacketSize) - (urb->actual_length)) {
                        memcpy (dev->read_buffer+dev->read_buffer_length, dev->interrupt_in_buffer, urb->actual_length);
                        dev->read_buffer_length += urb->actual_length;
                        wake_up_interruptible (&dev->read_wait);
                } else {
		        dbg(1," %s : read_buffer overflow", __func__);
                }
        }

        up (&dev->sem);

 exit:
        lego_usb_tower_debug_data(5,__func__, usb_pipedata(urb->pipe), urb->actual_length, urb->transfer_buffer);
	dbg(4," %s : leave, status %d", __func__, urb->status);
}


/**
 *	tower_interrupt_out_callback
 */
static void tower_interrupt_out_callback (struct urb *urb)
{
	struct lego_usb_tower *dev = (struct lego_usb_tower *)urb->context;

	dbg(4," %s : enter, status %d", __func__, urb->status);

        lego_usb_tower_debug_data(5,__func__, usb_pipedata(urb->pipe), urb->actual_length, urb->transfer_buffer);

        if (urb->status != 0) {
                if ((urb->status != -ENOENT) && 
                    (urb->status != -ECONNRESET)) {
                        dbg(1, " %s :nonzero status received: %d", __func__, urb->status);
                }
                goto exit;
        }                        
        wake_up_interruptible(&dev->write_wait);
 exit:
        lego_usb_tower_debug_data(5,__func__, usb_pipedata(urb->pipe), urb->actual_length, urb->transfer_buffer);

	dbg(4," %s : leave, status %d", __func__, urb->status);
}


/**
 *	tower_probe
 *
 *	Called by the usb core when a new device is connected that it thinks
 *	this driver might be interested in.
 */
static void * tower_probe (struct usb_device *udev, unsigned int ifnum, const struct usb_device_id *id)
{
	struct lego_usb_tower *dev = NULL;
	int minor;
	struct usb_interface* interface;
	struct usb_interface_descriptor *iface_desc;
        struct usb_endpoint_descriptor* endpoint;
	int i;
	char name[32];
        void *retval = NULL;

	dbg(2," %s : enter", __func__);

        if (udev == NULL) {
		info ("udev is NULL.");
        }
	
	/* See if the device offered us matches what we can accept */
	if ((udev->descriptor.idVendor != LEGO_USB_TOWER_VENDOR_ID) ||
	    (udev->descriptor.idProduct != LEGO_USB_TOWER_PRODUCT_ID)) {
		goto exit;
	}

        if( ifnum != 0 ) {
		info ("Strange interface number %d.", ifnum);
		goto exit;
        }

	/* select a "subminor" number (part of a minor number) */
	down (&minor_table_mutex);
	for (minor = 0; minor < MAX_DEVICES; ++minor) {
		if (minor_table[minor] == NULL)
			break;
	}
	if (minor >= MAX_DEVICES) {
		info ("Too many devices plugged in, can not handle this device.");
		goto unlock_exit;
	}

	/* allocate memory for our device state and intialize it */
	dev = kmalloc (sizeof(struct lego_usb_tower), GFP_KERNEL);
	if (dev == NULL) {
		err ("Out of memory");
		goto unlock_minor_exit;
	}
	init_MUTEX (&dev->sem);
        down (&dev->sem);
	dev->udev = udev;
	dev->minor = minor;
        dev->open_count = 0;

        dev->read_buffer = NULL;
        dev->read_buffer_length = 0;

        init_waitqueue_head (&dev->read_wait);
        init_waitqueue_head (&dev->write_wait);

        dev->interrupt_in_buffer = NULL;
        dev->interrupt_in_endpoint = NULL;
        dev->interrupt_in_urb = NULL;

        dev->interrupt_out_buffer = NULL;
        dev->interrupt_out_endpoint = NULL;
        dev->interrupt_out_urb = NULL;


        /* look for the endpoints */
        /* It seems slightly dubious to set up endpoints here,
           as we may change the configuration before calling open.
           But the endpoints should be the same in all configurations. */
	interface = &dev->udev->actconfig->interface[0];
	iface_desc = &interface->altsetting[0];

	/* set up the endpoint information */
	for (i = 0; i < iface_desc->bNumEndpoints; ++i) {
		endpoint = &iface_desc->endpoint[i];

		if (((endpoint->bEndpointAddress & USB_DIR_MASK) == USB_DIR_IN) &&
		    ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_INT)) {
			/* we found an interrupt in endpoint */
                        dev->interrupt_in_endpoint = endpoint;
		}
		
		if (((endpoint->bEndpointAddress & USB_DIR_MASK) == USB_DIR_OUT) &&
		    ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_INT)) {
			/* we found an interrupt out endpoint */
                        dev->interrupt_out_endpoint = endpoint;
		}
	}
        if(dev->interrupt_in_endpoint == NULL) {
                err("interrupt in endpoint not found");
                retval = NULL;
                goto unlock_exit;
        }
        if (dev->interrupt_out_endpoint == NULL) {
                err("interrupt out endpoint not found");
                retval = NULL;
                goto unlock_exit;
        }

        dev->read_buffer = kmalloc ((4*dev->interrupt_in_endpoint->wMaxPacketSize), GFP_KERNEL);
        if (!dev->read_buffer) {
                err("Couldn't allocate read_buffer");
                retval = NULL;
                goto unlock_exit;
        }
        dev->interrupt_in_buffer = kmalloc (dev->interrupt_in_endpoint->wMaxPacketSize, GFP_KERNEL);
        if (!dev->interrupt_in_buffer) {
                err("Couldn't allocate interrupt_in_buffer");
                retval = NULL;
                goto unlock_exit;
        }
        dev->interrupt_in_urb = usb_alloc_urb(0);
        if (!dev->interrupt_in_urb) {
                err("Couldn't allocate interrupt_in_urb");
                retval = NULL;
                goto unlock_exit;
        }
        dev->interrupt_out_buffer = kmalloc (dev->interrupt_out_endpoint->wMaxPacketSize, GFP_KERNEL);
        if (!dev->interrupt_out_buffer) {
                err("Couldn't allocate interrupt_out_buffer");
                retval = NULL;
                goto unlock_exit;
        }
        dev->interrupt_out_urb = usb_alloc_urb(0);
        if (!dev->interrupt_out_urb) {
                err("Couldn't allocate interrupt_out_urb");
                retval = NULL;
                goto unlock_exit;
        }                

        /* put dev in interface->private_data for disconnect */
	udev->actconfig->interface[0].private_data = dev;

        /* publish it */
	minor_table[minor] = dev;

	/* initialize the devfs node for this device and register it */
	sprintf(name, "usb/lego%d", dev->minor);
	
	dev->devfs = devfs_register (usb_devfs_handle, name,
				     DEVFS_FL_DEFAULT, USB_MAJOR,
				     LEGO_USB_TOWER_MINOR_BASE + dev->minor,
				     S_IFCHR | S_IRUSR | S_IWUSR | 
				     S_IRGRP | S_IWGRP | S_IROTH, 
				     &tower_fops, NULL);

	/* let the user know what node this device is now attached to */
	info ("LEGO USB Tower device now attached to /dev/usb/lego%d", dev->minor);

        retval = dev;

 unlock_exit:
        /* unlock device */
        up (&dev->sem);

 unlock_minor_exit:
	up (&minor_table_mutex);

 exit:
	dbg(2," %s : leave, return value 0x%.8lx (dev)", __func__, (long) dev);

	return retval;
}


/**
 *	tower_disconnect
 *
 *	Called by the usb core when the device is removed from the system.
 */
static void tower_disconnect (struct usb_device *udev, void *ptr)
{
	struct lego_usb_tower *dev;
	int minor;

	dbg(2," %s : enter", __func__);

	dev = (struct lego_usb_tower *)ptr;
	
	down (&minor_table_mutex);
	down (&dev->sem);

	minor = dev->minor;

	/* remove our devfs node */
	devfs_unregister(dev->devfs);

	/* if the device is not opened, then we clean up right now */
	if (!dev->open_count) {
		up (&dev->sem);
		tower_delete (dev);
	} else {
		dev->udev = NULL;
		up (&dev->sem);
	}

	info("LEGO USB Tower #%d now disconnected", minor);
	up (&minor_table_mutex);

	dbg(2," %s : leave", __func__);
}



/**
 *	lego_usb_tower_init
 */
static int __init lego_usb_tower_init(void)
{
	int result;
        int retval = 0;

	dbg(2," %s : enter", __func__);

	/* register this driver with the USB subsystem */
	result = usb_register(&tower_driver);
	if (result < 0) {
		err("usb_register failed for the "__FILE__" driver. Error number %d", result);
		retval = -1;
                goto exit;
	}

	info(DRIVER_DESC " " DRIVER_VERSION);

 exit:
	dbg(2," %s : leave, return value %d", __func__, retval);

	return retval;
}


/**
 *	lego_usb_tower_exit
 */
static void __exit lego_usb_tower_exit(void)
{
	dbg(2," %s : enter", __func__);

	/* deregister this driver with the USB subsystem */
	usb_deregister (&tower_driver);

	dbg(2," %s : leave", __func__);
}

module_init (lego_usb_tower_init);
module_exit (lego_usb_tower_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
#ifdef MODULE_LICENSE
MODULE_LICENSE("GPL");
#endif
