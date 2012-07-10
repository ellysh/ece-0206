/*
 * ECE0206-01 driver
 *
 * ELCUS, 2011
 *
 * This driver is based on USB Skeleton driver - 2.2
 *
ядро 2.6.29-1

    сборка драйвера
#make  -C /usr/src/linux SUBDIRS=$PWD modules

#rmmod usbtest
#insmod -f ./ECE0206lin.ko

*/

#include "ECE0206lin.h"

/* table of devices that work with this driver */
static struct usb_device_id ece0206_table [] =
    {
    { USB_DEVICE(ECE0206_VENDOR_ID, ECE0206_PRODUCT_ID) },
    { }                    /* Terminating entry */
    };
MODULE_DEVICE_TABLE(usb, ece0206_table);


#define to_ece0206_dev(d) container_of(d, struct ece0206_device, kref)

static struct usb_driver ece0206_driver;
static void ece0206_draw_down(struct ece0206_device *dev);


//...........................................................................
//            ece0206_delete
//...........................................................................
static void ece0206_delete(struct kref *kref)
{
    struct ece0206_device *dev = to_ece0206_dev(kref);

    usb_put_dev(dev->udev);
    if (dev->bulk_in4_buffer) kfree(dev->bulk_in4_buffer);
    if (dev->interr_in6_buffer) kfree(dev->interr_in6_buffer);
    kfree(dev);
}


//...........................................................................
//            read_interr_callback
//...........................................................................
static void ece0206_read_interr_callback(struct urb *urb)
{
    struct ece0206_device *dev;
    ULONG  w;
    USHORT i;
    PUCHAR pWorkBuf;
    PUCHAR pBigBuffer;
    UCHAR w1;
    UCHAR id;
    int result;

    i =0;
    dev = urb->context;

    /* sync/async unlink faults aren't errors */
    if (urb->status)
    {
        if(!(urb->status == -ENOENT ||
            urb->status == -ECONNRESET ||
            urb->status == -ESHUTDOWN))
            printk("%s - nonzero write bulk status received: %d",
                __func__, urb->status);

        spin_lock(&dev->err_lock);
        dev->errors = urb->status;
        spin_unlock(&dev->err_lock);
        return;
    }



    w = urb->actual_length;
    pBigBuffer = dev->pGlobBuffer;
    pWorkBuf = dev->interr_in6_buffer;


    do
    {
        w1 = *(pWorkBuf+i+1);

        id = (w1) & 0xf0;

        switch (id)
        {
        case 0x0:
            #ifdef     SPINLOCK
            spin_lock(&dev->glob_buff_lock);
            #endif

            *(pBigBuffer+22547) = 0xff;
            *(pBigBuffer+22544) = *(pWorkBuf+i);
            *(pBigBuffer+22545) = *(pWorkBuf+i+3);
            *(pBigBuffer+22546) = *(pWorkBuf+i+2);
            *(pBigBuffer+22547) = 0;

            #ifdef     SPINLOCK
            spin_unlock(&dev->glob_buff_lock);
            #endif

            i+=4;
            break;

        case 0x10:
            if(((w1) & 0xf)!=0xf)
            {
                *(pBigBuffer+22532) = w1;
                *(pBigBuffer+22533) = *(pWorkBuf+i+2);
                *(pBigBuffer+22534) = *(pWorkBuf+i+3);
            }
            else
            {
                int indexAp1 = *(pBigBuffer+22534)*11;
                int indexCc1 = *(pBigBuffer+22528)*11+11264;

                #ifdef     SPINLOCK
                spin_lock(&dev->glob_buff_lock);
                #endif

                *(pBigBuffer+indexAp1+10)= 0xff;
                *(pBigBuffer+indexAp1)= *(pBigBuffer+22544);
                indexAp1++;
                *(pBigBuffer+indexAp1)= *(pBigBuffer+22545);
                indexAp1++;
                *(pBigBuffer+indexAp1)= *(pBigBuffer+22546);
                indexAp1++;

                *(pBigBuffer+indexAp1)= *(pWorkBuf+i);
                indexAp1++;
                *(pBigBuffer+indexAp1)= *(pBigBuffer+22532);
                indexAp1++;
                *(pBigBuffer+indexAp1)= *(pBigBuffer+22533);
                indexAp1++;
                *(pBigBuffer+indexAp1)= *(pBigBuffer+22534);
                indexAp1++;
                *(pBigBuffer+indexAp1)= w1;
                indexAp1++;
                *(pBigBuffer+indexAp1)= *(pWorkBuf+i+2);
                indexAp1++;
                *(pBigBuffer+indexAp1)= *(pWorkBuf+i+3);
                indexAp1++;
                *(pBigBuffer+indexAp1)= 0;


                *(pBigBuffer+indexCc1+10)= 0xff;
                *(pBigBuffer+indexCc1)= *(pBigBuffer+22544);
                indexCc1++;
                *(pBigBuffer+indexCc1)= *(pBigBuffer+22545);
                indexCc1++;
                *(pBigBuffer+indexCc1)= *(pBigBuffer+22546);
                indexCc1++;

                *(pBigBuffer+indexCc1)= *(pWorkBuf+i);
                indexCc1++;
                *(pBigBuffer+indexCc1)= *(pBigBuffer+22532);
                indexCc1++;
                *(pBigBuffer+indexCc1)= *(pBigBuffer+22533);
                indexCc1++;
                *(pBigBuffer+indexCc1)= *(pBigBuffer+22534);
                indexCc1++;
                *(pBigBuffer+indexCc1)= w1;
                indexCc1++;
                *(pBigBuffer+indexCc1)= *(pWorkBuf+i+2);
                indexCc1++;
                *(pBigBuffer+indexCc1)= *(pWorkBuf+i+3);
                indexCc1++;
                *(pBigBuffer+indexCc1)= 0;

                *(pBigBuffer+22528)+=1;

                #ifdef     SPINLOCK
                spin_unlock(&dev->glob_buff_lock);
                #endif
            }
            i+=4;
            break;

        case 0x20:
            if(((w1) & 0xf)!=0xf)
            {
                *(pBigBuffer+22535) = w1;
                *(pBigBuffer+22536) = *(pWorkBuf+i+2);
                *(pBigBuffer+22537) = *(pWorkBuf+i+3);
            }
            else
            {
                int indexAp2 = *(pBigBuffer+22537)*11+2816;
                int indexCc2 = *(pBigBuffer+22529)*11+14080;

                #ifdef     SPINLOCK
                spin_lock(&dev->glob_buff_lock);
                #endif

                *(pBigBuffer+indexAp2+10)= 0xff;
                *(pBigBuffer+indexAp2)= *(pBigBuffer+22544);
                indexAp2++;
                *(pBigBuffer+indexAp2)= *(pBigBuffer+22545);
                indexAp2++;
                *(pBigBuffer+indexAp2)= *(pBigBuffer+22546);
                indexAp2++;

                *(pBigBuffer+indexAp2)= *(pWorkBuf+i);
                indexAp2++;
                *(pBigBuffer+indexAp2)= *(pBigBuffer+22535);
                indexAp2++;
                *(pBigBuffer+indexAp2)= *(pBigBuffer+22536);
                indexAp2++;
                *(pBigBuffer+indexAp2)= *(pBigBuffer+22537);
                indexAp2++;
                *(pBigBuffer+indexAp2)= w1;
                indexAp2++;
                *(pBigBuffer+indexAp2)= *(pWorkBuf+i+2);
                indexAp2++;
                *(pBigBuffer+indexAp2)= *(pWorkBuf+i+3);
                indexAp2++;
                *(pBigBuffer+indexAp2)= 0;


                *(pBigBuffer+indexCc2+10)= 0xff;
                *(pBigBuffer+indexCc2)= *(pBigBuffer+22544);
                indexCc2++;
                *(pBigBuffer+indexCc2)= *(pBigBuffer+22545);
                indexCc2++;
                *(pBigBuffer+indexCc2)= *(pBigBuffer+22546);
                indexCc2++;

                *(pBigBuffer+indexCc2)= *(pWorkBuf+i);
                indexCc2++;
                *(pBigBuffer+indexCc2)= *(pBigBuffer+22535);
                indexCc2++;
                *(pBigBuffer+indexCc2)= *(pBigBuffer+22536);
                indexCc2++;
                *(pBigBuffer+indexCc2)= *(pBigBuffer+22537);
                indexCc2++;
                *(pBigBuffer+indexCc2)= w1;
                indexCc2++;
                *(pBigBuffer+indexCc2)= *(pWorkBuf+i+2);
                indexCc2++;
                *(pBigBuffer+indexCc2)= *(pWorkBuf+i+3);
                indexCc2++;
                *(pBigBuffer+indexCc2)= 0;

                *(pBigBuffer+22529)+=1;

                #ifdef     SPINLOCK
                spin_unlock(&dev->glob_buff_lock);
                #endif
            }
            i+=4;
            break;

        case 0x30:
            if(((w1) & 0xf)!=0xf)
            {
                *(pBigBuffer+22538) = w1;
                *(pBigBuffer+22539) = *(pWorkBuf+i+2);
                *(pBigBuffer+22540) = *(pWorkBuf+i+3);
            }
            else
            {
                int indexAp3 = *(pBigBuffer+22540)*11+5632;
                int indexCc3 = *(pBigBuffer+22530)*11+16896;

                #ifdef     SPINLOCK
                spin_lock(&dev->glob_buff_lock);
                #endif

                *(pBigBuffer+indexAp3+10)= 0xff;
                *(pBigBuffer+indexAp3)= *(pBigBuffer+22544);
                indexAp3++;
                *(pBigBuffer+indexAp3)= *(pBigBuffer+22545);
                indexAp3++;
                *(pBigBuffer+indexAp3)= *(pBigBuffer+22546);
                indexAp3++;

                *(pBigBuffer+indexAp3)= *(pWorkBuf+i);
                indexAp3++;
                *(pBigBuffer+indexAp3)= *(pBigBuffer+22538);
                indexAp3++;
                *(pBigBuffer+indexAp3)= *(pBigBuffer+22539);
                indexAp3++;
                *(pBigBuffer+indexAp3)= *(pBigBuffer+22540);
                indexAp3++;
                *(pBigBuffer+indexAp3)= w1;
                indexAp3++;
                *(pBigBuffer+indexAp3)= *(pWorkBuf+i+2);
                indexAp3++;
                *(pBigBuffer+indexAp3)= *(pWorkBuf+i+3);
                indexAp3++;
                *(pBigBuffer+indexAp3)= 0;


                *(pBigBuffer+indexCc3+10)= 0xff;
                *(pBigBuffer+indexCc3)= *(pBigBuffer+22544);
                indexCc3++;
                *(pBigBuffer+indexCc3)= *(pBigBuffer+22545);
                indexCc3++;
                *(pBigBuffer+indexCc3)= *(pBigBuffer+22546);
                indexCc3++;

                *(pBigBuffer+indexCc3)= *(pWorkBuf+i);
                indexCc3++;
                *(pBigBuffer+indexCc3)= *(pBigBuffer+22538);
                indexCc3++;
                *(pBigBuffer+indexCc3)= *(pBigBuffer+22539);
                indexCc3++;
                *(pBigBuffer+indexCc3)= *(pBigBuffer+22540);
                indexCc3++;
                *(pBigBuffer+indexCc3)= w1;
                indexCc3++;
                *(pBigBuffer+indexCc3)= *(pWorkBuf+i+2);
                indexCc3++;
                *(pBigBuffer+indexCc3)= *(pWorkBuf+i+3);
                indexCc3++;
                *(pBigBuffer+indexCc3)= 0;

                *(pBigBuffer+22530)+=1;

                #ifdef     SPINLOCK
                spin_unlock(&dev->glob_buff_lock);
                #endif
            }
            i+=4;
            break;

        case 0x40:
            if(((w1) & 0xf)!=0xf)
            {
                *(pBigBuffer+22541) = w1;
                *(pBigBuffer+22542) = *(pWorkBuf+i+2);
                *(pBigBuffer+22543) = *(pWorkBuf+i+3);
            }
            else
            {
                int indexAp4 = *(pBigBuffer+22543)*11+8448;
                int indexCc4 = *(pBigBuffer+22531)*11+19712;

                #ifdef     SPINLOCK
                spin_lock(&dev->glob_buff_lock);
                #endif

                *(pBigBuffer+indexAp4+10)= 0xff;
                *(pBigBuffer+indexAp4)= *(pBigBuffer+22544);
                indexAp4++;
                *(pBigBuffer+indexAp4)= *(pBigBuffer+22545);
                indexAp4++;
                *(pBigBuffer+indexAp4)= *(pBigBuffer+22546);
                indexAp4++;

                *(pBigBuffer+indexAp4)= *(pWorkBuf+i);
                indexAp4++;
                *(pBigBuffer+indexAp4)= *(pBigBuffer+22541);
                indexAp4++;
                *(pBigBuffer+indexAp4)= *(pBigBuffer+22542);
                indexAp4++;
                *(pBigBuffer+indexAp4)= *(pBigBuffer+22543);
                indexAp4++;
                *(pBigBuffer+indexAp4)= w1;
                indexAp4++;
                *(pBigBuffer+indexAp4)= *(pWorkBuf+i+2);
                indexAp4++;
                *(pBigBuffer+indexAp4)= *(pWorkBuf+i+3);
                indexAp4++;
                *(pBigBuffer+indexAp4)= 0;

                *(pBigBuffer+indexCc4+10)= 0xff;
                *(pBigBuffer+indexCc4)= *(pBigBuffer+22544);
                indexCc4++;
                *(pBigBuffer+indexCc4)= *(pBigBuffer+22545);
                indexCc4++;
                *(pBigBuffer+indexCc4)= *(pBigBuffer+22546);
                indexCc4++;

                *(pBigBuffer+indexCc4)= *(pWorkBuf+i);
                indexCc4++;
                *(pBigBuffer+indexCc4)= *(pBigBuffer+22541);
                indexCc4++;
                *(pBigBuffer+indexCc4)= *(pBigBuffer+22542);
                indexCc4++;
                *(pBigBuffer+indexCc4)= *(pBigBuffer+22543);
                indexCc4++;
                *(pBigBuffer+indexCc4)= w1;
                indexCc4++;
                *(pBigBuffer+indexCc4)= *(pWorkBuf+i+2);
                indexCc4++;
                *(pBigBuffer+indexCc4)= *(pWorkBuf+i+3);
                indexCc4++;
                *(pBigBuffer+indexCc4)= 0;

                *(pBigBuffer+22531)+=1;

                #ifdef     SPINLOCK
                spin_unlock(&dev->glob_buff_lock);
                #endif
            }
            i+=4;
            break;


        default:
            i+=4;

        }

    }while(i<w);

    result = usb_submit_urb(dev->urbINT, GFP_ATOMIC);
    if(result)
    {
        printk(    "%s - Error %d submitting control urb\n",
                            __func__, result);
    };

}


//...........................................................................
//            open
//...........................................................................
static int ece0206_open(struct inode *inode, struct file *file)
{
    struct ece0206_device *dev;
    struct usb_interface *interface;
    int subminor;
    int retval , i ;

    subminor = iminor(inode);
    retval = 0;

    interface = usb_find_interface(&ece0206_driver, subminor);
    if (!interface) {
        err ("%s - error, can't find device for minor %d",
             __func__, subminor);
        retval = -ENODEV;
        goto exit;
    }

    dev = usb_get_intfdata(interface);
    if (!dev) {
        retval = -ENODEV;
        goto exit;
    }

    /* increment our usage count for the device */
    kref_get(&dev->kref);

    /* lock the device to allow correctly handling errors
     * in resumption */
    mutex_lock(&dev->io_mutex);

    dev->open_count++;
    retval = usb_autopm_get_interface(interface);
    if (retval)
    {
        dev->open_count--;
        mutex_unlock(&dev->io_mutex);
        kref_put(&dev->kref, ece0206_delete);
        goto exit;
    }

    /* save our object in the file's private structure */
    file->private_data = dev;

    for(i=22528; i<22548; i++)
        *(dev->pGlobBuffer+i) = 0;

    dev->urbINT = usb_alloc_urb(0, GFP_ATOMIC);
    if (!dev->urbINT) {
        retval = -ENOMEM;
        dev->open_count--;
        mutex_unlock(&dev->io_mutex);
        kref_put(&dev->kref, ece0206_delete);
        goto exit;
    }


    usb_fill_int_urb(dev->urbINT
            ,dev->udev
            ,usb_rcvintpipe(dev->udev, dev->interr_in_endpointAddr)
            ,dev->interr_in6_buffer
            ,1024
            ,ece0206_read_interr_callback
            ,dev
            ,dev->interval);


    usb_anchor_urb(dev->urbINT, &dev->submitted);


    retval = usb_submit_urb(dev->urbINT, GFP_ATOMIC);

    if (retval)
    {
        usb_unanchor_urb(dev->urbINT);
        usb_free_urb(dev->urbINT);
        dev->open_count--;
        mutex_unlock(&dev->io_mutex);
        kref_put(&dev->kref, ece0206_delete);
        goto exit;
    }


    mutex_unlock(&dev->io_mutex);
    printk("ECE0206 device now open  \n");
exit:
    return retval;
}


//...........................................................................
//            release
//...........................................................................
static int ece0206_release(struct inode *inode, struct file *file)
{
    struct ece0206_device *dev;

    dev = (struct ece0206_device *)file->private_data;
    if (dev == NULL)
        return -ENODEV;

    mutex_lock(&dev->io_mutex);

    /* allow the device to be autosuspended */
    if (!--dev->open_count && dev->interface)
        usb_autopm_put_interface(dev->interface);
    mutex_unlock(&dev->io_mutex);

    //usb_free_urb(dev->urbINT);
     usb_kill_urb(dev->urbINT);

    /* decrement the count on our device */
    kref_put(&dev->kref, ece0206_delete);
    printk("ECE0206 device now release  \n");

    return 0;
}


//...........................................................................
//            ioctl
//...........................................................................
static int ece0206_ioctl (struct inode *inode, struct file *filp,
         unsigned int cmd, POINTER arg)
{
    struct ece0206_device *dev;
    unsigned short i;
    int bytes_read;
    unsigned char temp,temp1;
    unsigned int tempInt;

    int retval = 0;

    dev = (struct ece0206_device *)filp->private_data;

    mutex_lock(&dev->io_mutex);
    if (!dev->interface) {        /* disconnect() was called */
        retval = -ENODEV;
        goto exit;
    }

    switch(cmd)
     {
    case ECE02061_WRITE_ARRAY_1:

        tempInt = *(unsigned char *)(arg+1);
        if((tempInt<1)||(tempInt>127)) tempInt =1;

        tempInt = tempInt*4+2;

        __get_user(dev->data[0], (unsigned char *)(arg));
        dev->data[1] = 0x80;

        for(i = 2; i<tempInt; i++)
            __get_user(dev->data[i], (unsigned char *)(arg+i));


        retval = usb_bulk_msg(dev->udev,
            usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr),
            &dev->data,
            tempInt,
            &bytes_read,
            500);
        break;

    case ECE02061_CLEAR_ARRAY_1:

        tempInt = *(unsigned char *)(arg+1);
        if((tempInt<1)||(tempInt>127)) tempInt =1;

        tempInt = tempInt*4+2;

        __get_user(dev->data[0], (unsigned char *)(arg));
        dev->data[1] = 0x80;

        for(i = 2; i<tempInt; i++)
            dev->data[i] = 0;


        retval = usb_bulk_msg(dev->udev,
            usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr),
            &dev->data,
            tempInt,
            &bytes_read,
            500);
        break;

    case ECE02061_SO_PUSK1:

        dev->SOmode.SOmodeCommand0 = 0x0;
        dev->SOmode.SOmodeCommand1 = 0x20;
        __get_user ( temp,(unsigned char *)(arg+4));
        dev->SOmode.SOmodeDelay = temp;
        __get_user ( temp,(unsigned char *)(arg+5));
        dev->SOmode.SOmodeFrameNumber = temp;
        __get_user ( temp,(unsigned char *)(arg+3));
        dev->SOmode.SOmodeParamNumber = temp;
        dev->SOmode.SOmodeMode &= 0x30;
        __get_user ( temp,(unsigned char *)(arg+1));
        __get_user ( temp1,(unsigned char *)(arg+2));
        dev->SOmode.SOmodeMode = dev->SOmode.SOmodeMode
                        |0x80
                        |((temp&1)<<3)
                        |(temp1&3);
        __get_user ( temp,(unsigned char *)arg);

         switch(temp)
         {
         case 0:
            break;
         case 1:
            dev->SOmode.SOmodeMode|=0x44;
            break;
         case 255:
            dev->SOmode.SOmodeMode|=0x40;
            break;
         default:
             break;
         }

        retval = usb_bulk_msg(dev->udev,
            usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr),
            &dev->SOmode,
            6,
            &bytes_read,
            100);

        break;

    case ECE02061_SO_STOP1:

        dev->SOmode.SOmodeCommand1 = 0x20;
        dev->SOmode.SOmodeMode &= 0x30;

        retval = usb_bulk_msg(dev->udev,
            usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr),
            &dev->SOmode,
            6,
            &bytes_read,
            100);
        break;

    case ECE02061_SO_STATE1:

        dev->SOmode.SOmodeCommand1 = 0x60;

        retval = usb_bulk_msg(dev->udev,
            usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr),
            &dev->SOmode,
            6,
            &bytes_read,
            100);

        retval = usb_bulk_msg(dev->udev,
            usb_rcvbulkpipe(dev->udev, dev->bulk_in_endpointAddr),
            &dev->data,
            4,
            &bytes_read,
            100);
        if(!retval)
        {
            if(dev->data[3]&0x80)
            {
                __put_user ( 1,(unsigned char*)(arg));

                temp = dev->data[3];

                switch(temp&0x44)
                {
                case 0x44:
                    __put_user ( 1,(unsigned char*)(arg+1));
                    break;
                case 0x40:
                    __put_user ( 255,(unsigned char*)(arg+1));
                    break;
                default:
                    __put_user ( 0,(unsigned char*)(arg+1));
                    break;
                }

                __put_user ( (temp&0x8)>>3,(unsigned char*)(arg+2));
                __put_user ( temp&0x3,(unsigned char*)(arg+3));
                __put_user ( dev->data[2],(unsigned char*)(arg+4));
                __put_user ( dev->data[0],(unsigned char*)(arg+5));
                __put_user ( dev->data[1],(unsigned char*)(arg+6));
            }
            else
            {
            for(i=0; i<7; i++)
                __put_user ( 0,(unsigned char*)(arg+i));
            }

        }
    else
            __put_user ( 0xff,(unsigned char*)(arg));


        break;

    case ECE02061_WRITE_PARAM_1:

        for(i = 0; i<6; i++)
        {    retval = __get_user(dev->data[i], (unsigned char *)(arg+i));
        }

        retval = usb_bulk_msg(dev->udev,
            usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr),
            &dev->data,
            6,
            &bytes_read,
            100);
        break;

    case ECE02061_READ_PARAM_1:

        __get_user(dev->data[0], (unsigned char *)(arg));
        dev->data[1] = 0x40;

        retval = usb_bulk_msg(dev->udev,
            usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr),
            &dev->data,
            2,
            &bytes_read,
            100);

        retval = usb_bulk_msg(dev->udev,
            usb_rcvbulkpipe(dev->udev, dev->bulk_in_endpointAddr),
            &dev->data,
            4,
            &bytes_read,
            100);

        __put_user ( dev->data[0],(unsigned char*)(arg));
        __put_user ( dev->data[1],(unsigned char*)(arg+1));
        __put_user ( dev->data[2],(unsigned char*)(arg+2));
        __put_user ( dev->data[3],(unsigned char*)(arg+3));

        break;

    case ECE02061_GET_SERIAL_NUMBER:
        i=0;
        while(*(dev->udev->product+i))
        {
            __put_user ( *(dev->udev->serial+i),(unsigned char*)(arg+i));
            i++;
        }
        break;

    case ECE02061_GET_PRODUCT_NAME:
        i=0;
        while(*(dev->udev->product+i))
        {
            __put_user ( *(dev->udev->product+i),(unsigned char*)(arg+i));
            i++;
        }

        break;

    case ECE02061_GET_MANUFACTURER_NAME:
        i=0;
        while(*(dev->udev->product+i))
        {
            __put_user ( *(dev->udev->manufacturer+i),(unsigned char*)(arg+i));
            i++;
        }

        break;


    case ECE02061_SI_PUSK:

        __get_user ( dev->numberCH,(unsigned char *)arg);

        if((!dev->numberCH)||(dev->numberCH > SI_CHANNAL_NUMBER))
        {
            __put_user ( 1,(unsigned char*)arg);
            break;
        }
        dev->numberCH-=1;


         if(dev->numberCH==3)
            dev->SImode.SImode[3] &= 2;
         else
             dev->SImode.SImode[dev->numberCH] = 0;

         dev->SImode.SImode[dev->numberCH] = dev->SImode.SImode[dev->numberCH]
                        |0x80
                        |((*(unsigned char*)(arg+1)&1)<<4)
                        |((*(unsigned char*)(arg+2)&1)<<3)
                        |(*(unsigned char*)(arg+3)&1);

         dev->SImode.SImodeCommand1 = 0x24;                //00100100

        retval = usb_bulk_msg(dev->udev,
            usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr),
            &dev->SImode,
            6,
            &bytes_read,
            100);

        __put_user ( 0,(unsigned char*)arg);
        break;

    case ECE02061_SI_STOP:

        __get_user ( dev->numberCH,(unsigned char *)arg);

        if((!dev->numberCH)||(dev->numberCH > SI_CHANNAL_NUMBER))
        {
            __put_user ( 1,(unsigned char*)arg);
            break;
        }
        dev->numberCH-=1;


         if(dev->numberCH==3)
            dev->SImode.SImode[3] &= 2;
         else
             dev->SImode.SImode[dev->numberCH] = 0;


         dev->SImode.SImodeCommand1 = 0x24;

        retval = usb_bulk_msg(dev->udev,
            usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr),
            &dev->SImode,
            6,
            &bytes_read,
            100);
        __put_user ( 0,(unsigned char*)arg);
        break;

    case ECE02061_READ_ARRAY_AP1:

        for(i =0; i<256; i++)
        {
            #ifdef     SPINLOCK
            spin_lock(&dev->glob_buff_lock);
            #endif

            __put_user ( *(dev->pGlobBuffer+i*11+6),(PUCHAR)arg+i*10);
            __put_user ( *(dev->pGlobBuffer+i*11+5),(PUCHAR)arg+i*10+1);
            __put_user ( *(dev->pGlobBuffer+i*11+9),(PUCHAR)arg+i*10+2);
            __put_user ( *(dev->pGlobBuffer+i*11+8),(PUCHAR)arg+i*10+3);
            __put_user ( *(dev->pGlobBuffer+i*11+3),(PUCHAR)arg+i*10+4);
            __put_user ( *(dev->pGlobBuffer+i*11+2),(PUCHAR)arg+i*10+5);
            __put_user ( *(dev->pGlobBuffer+i*11+1),(PUCHAR)arg+i*10+6);
            __put_user ( *(dev->pGlobBuffer+i*11),(PUCHAR)arg+i*10+7);
            temp = ((*(dev->pGlobBuffer+i*11+4))<<4)+ ((*(dev->pGlobBuffer+i*11+7))&0xf);
            __put_user (temp, (PUCHAR)arg+i*10+8);
            __put_user ( *(dev->pGlobBuffer+i*11+10),(PUCHAR)arg+i*10+9);

            #ifdef     SPINLOCK
            spin_unlock(&dev->glob_buff_lock);
            #endif
        }

        break;

    case ECE02061_READ_ARRAY_CC1:

        for(i =0; i<256; i++)
        {
            #ifdef     SPINLOCK
            spin_lock(&dev->glob_buff_lock);
            #endif

            __put_user ( *(dev->pGlobBuffer+11264+i*11+6),(PUCHAR)arg+i*10);
            __put_user ( *(dev->pGlobBuffer+11264+i*11+5),(PUCHAR)arg+i*10+1);
            __put_user ( *(dev->pGlobBuffer+11264+i*11+9),(PUCHAR)arg+i*10+2);
            __put_user ( *(dev->pGlobBuffer+11264+i*11+8),(PUCHAR)arg+i*10+3);
            __put_user ( *(dev->pGlobBuffer+11264+i*11+3),(PUCHAR)arg+i*10+4);
            __put_user ( *(dev->pGlobBuffer+11264+i*11+2),(PUCHAR)arg+i*10+5);
            __put_user ( *(dev->pGlobBuffer+11264+i*11+1),(PUCHAR)arg+i*10+6);
            __put_user ( *(dev->pGlobBuffer+11264+i*11),(PUCHAR)arg+i*10+7);
            temp = ((*(dev->pGlobBuffer+11264+i*11+4))<<4)+ ((*(dev->pGlobBuffer+11264+i*11+7))&0xf);
            __put_user (temp, (PUCHAR)arg+i*10+8);
            __put_user ( *(dev->pGlobBuffer+11264+i*11+10),(PUCHAR)arg+i*10+9);

            #ifdef     SPINLOCK
            spin_unlock(&dev->glob_buff_lock);
            #endif
        }

        break;

    case ECE02061_READ_ARRAY_AP2:

        for(i =0; i<256; i++)
        {
            #ifdef     SPINLOCK
            spin_lock(&dev->glob_buff_lock);
            #endif

            __put_user ( *(dev->pGlobBuffer+2816+i*11+6),(PUCHAR)arg+i*10);
            __put_user ( *(dev->pGlobBuffer+2816+i*11+5),(PUCHAR)arg+i*10+1);
            __put_user ( *(dev->pGlobBuffer+2816+i*11+9),(PUCHAR)arg+i*10+2);
            __put_user ( *(dev->pGlobBuffer+2816+i*11+8),(PUCHAR)arg+i*10+3);
            __put_user ( *(dev->pGlobBuffer+2816+i*11+3),(PUCHAR)arg+i*10+4);
            __put_user ( *(dev->pGlobBuffer+2816+i*11+2),(PUCHAR)arg+i*10+5);
            __put_user ( *(dev->pGlobBuffer+2816+i*11+1),(PUCHAR)arg+i*10+6);
            __put_user ( *(dev->pGlobBuffer+2816+i*11),(PUCHAR)arg+i*10+7);
            temp = ((*(dev->pGlobBuffer+2816+i*11+4))<<4)+ ((*(dev->pGlobBuffer+2816+i*11+7))&0xf);
            __put_user (temp, (PUCHAR)arg+i*10+8);
            __put_user ( *(dev->pGlobBuffer+2816+i*11+10),(PUCHAR)arg+i*10+9);

            #ifdef     SPINLOCK
            spin_unlock(&dev->glob_buff_lock);
            #endif
        }

        break;

    case ECE02061_READ_ARRAY_CC2:

        for(i =0; i<256; i++)
        {
            #ifdef     SPINLOCK
            spin_lock(&dev->glob_buff_lock);
            #endif

            __put_user ( *(dev->pGlobBuffer+14080+i*11+6),(PUCHAR)arg+i*10);
            __put_user ( *(dev->pGlobBuffer+14080+i*11+5),(PUCHAR)arg+i*10+1);
            __put_user ( *(dev->pGlobBuffer+14080+i*11+9),(PUCHAR)arg+i*10+2);
            __put_user ( *(dev->pGlobBuffer+14080+i*11+8),(PUCHAR)arg+i*10+3);
            __put_user ( *(dev->pGlobBuffer+14080+i*11+3),(PUCHAR)arg+i*10+4);
            __put_user ( *(dev->pGlobBuffer+14080+i*11+2),(PUCHAR)arg+i*10+5);
            __put_user ( *(dev->pGlobBuffer+14080+i*11+1),(PUCHAR)arg+i*10+6);
            __put_user ( *(dev->pGlobBuffer+14080+i*11),(PUCHAR)arg+i*10+7);
            temp = ((*(dev->pGlobBuffer+14080+i*11+4))<<4)+ ((*(dev->pGlobBuffer+14080+i*11+7))&0xf);
            __put_user (temp, (PUCHAR)arg+i*10+8);
            __put_user ( *(dev->pGlobBuffer+14080+i*11+10),(PUCHAR)arg+i*10+9);

            #ifdef     SPINLOCK
            spin_unlock(&dev->glob_buff_lock);
            #endif
        }

        break;

    case ECE02061_READ_ARRAY_AP3:

        for(i =0; i<256; i++)
        {
            #ifdef     SPINLOCK
            spin_lock(&dev->glob_buff_lock);
            #endif

            __put_user ( *(dev->pGlobBuffer+5632+i*11+6),(PUCHAR)arg+i*10);
            __put_user ( *(dev->pGlobBuffer+5632+i*11+5),(PUCHAR)arg+i*10+1);
            __put_user ( *(dev->pGlobBuffer+5632+i*11+9),(PUCHAR)arg+i*10+2);
            __put_user ( *(dev->pGlobBuffer+5632+i*11+8),(PUCHAR)arg+i*10+3);
            __put_user ( *(dev->pGlobBuffer+5632+i*11+3),(PUCHAR)arg+i*10+4);
            __put_user ( *(dev->pGlobBuffer+5632+i*11+2),(PUCHAR)arg+i*10+5);
            __put_user ( *(dev->pGlobBuffer+5632+i*11+1),(PUCHAR)arg+i*10+6);
            __put_user ( *(dev->pGlobBuffer+5632+i*11),(PUCHAR)arg+i*10+7);
            temp = ((*(dev->pGlobBuffer+5632+i*11+4))<<4)+ ((*(dev->pGlobBuffer+5632+i*11+7))&0xf);
            __put_user (temp, (PUCHAR)arg+i*10+8);
            __put_user ( *(dev->pGlobBuffer+5632+i*11+10),(PUCHAR)arg+i*10+9);

            #ifdef     SPINLOCK
            spin_unlock(&dev->glob_buff_lock);
            #endif
        }

        break;

    case ECE02061_READ_ARRAY_CC3:

        for(i =0; i<256; i++)
        {
            #ifdef     SPINLOCK
            spin_lock(&dev->glob_buff_lock);
            #endif

            __put_user ( *(dev->pGlobBuffer+16896+i*11+6),(PUCHAR)arg+i*10);
            __put_user ( *(dev->pGlobBuffer+16896+i*11+5),(PUCHAR)arg+i*10+1);
            __put_user ( *(dev->pGlobBuffer+16896+i*11+9),(PUCHAR)arg+i*10+2);
            __put_user ( *(dev->pGlobBuffer+16896+i*11+8),(PUCHAR)arg+i*10+3);
            __put_user ( *(dev->pGlobBuffer+16896+i*11+3),(PUCHAR)arg+i*10+4);
            __put_user ( *(dev->pGlobBuffer+16896+i*11+2),(PUCHAR)arg+i*10+5);
            __put_user ( *(dev->pGlobBuffer+16896+i*11+1),(PUCHAR)arg+i*10+6);
            __put_user ( *(dev->pGlobBuffer+16896+i*11),(PUCHAR)arg+i*10+7);
            temp = ((*(dev->pGlobBuffer+16896+i*11+4))<<4)+ ((*(dev->pGlobBuffer+16896+i*11+7))&0xf);
            __put_user (temp, (PUCHAR)arg+i*10+8);
            __put_user ( *(dev->pGlobBuffer+16896+i*11+10),(PUCHAR)arg+i*10+9);

            #ifdef     SPINLOCK
            spin_unlock(&dev->glob_buff_lock);
            #endif
        }

        break;

    case ECE02061_READ_ARRAY_AP4:


        for(i =0; i<256; i++)
        {
            #ifdef     SPINLOCK
            spin_lock(&dev->glob_buff_lock);
            #endif

            __put_user ( *(dev->pGlobBuffer+8448+i*11+6),(PUCHAR)arg+i*10);
            __put_user ( *(dev->pGlobBuffer+8448+i*11+5),(PUCHAR)arg+i*10+1);
            __put_user ( *(dev->pGlobBuffer+8448+i*11+9),(PUCHAR)arg+i*10+2);
            __put_user ( *(dev->pGlobBuffer+8448+i*11+8),(PUCHAR)arg+i*10+3);
            __put_user ( *(dev->pGlobBuffer+8448+i*11+3),(PUCHAR)arg+i*10+4);
            __put_user ( *(dev->pGlobBuffer+8448+i*11+2),(PUCHAR)arg+i*10+5);
            __put_user ( *(dev->pGlobBuffer+8448+i*11+1),(PUCHAR)arg+i*10+6);
            __put_user ( *(dev->pGlobBuffer+8448+i*11),(PUCHAR)arg+i*10+7);
            temp = ((*(dev->pGlobBuffer+8448+i*11+4))<<4)+ ((*(dev->pGlobBuffer+8448+i*11+7))&0xf);
            __put_user (temp, (PUCHAR)arg+i*10+8);
            __put_user ( *(dev->pGlobBuffer+8448+i*11+10),(PUCHAR)arg+i*10+9);

            #ifdef     SPINLOCK
            spin_unlock(&dev->glob_buff_lock);
            #endif
        }

        break;

    case ECE02061_READ_ARRAY_CC4:

        for(i =0; i<256; i++)
        {
            #ifdef     SPINLOCK
            spin_lock(&dev->glob_buff_lock);
            #endif

            __put_user ( *(dev->pGlobBuffer+19712+i*11+6),(PUCHAR)arg+i*10);
            __put_user ( *(dev->pGlobBuffer+19712+i*11+5),(PUCHAR)arg+i*10+1);
            __put_user ( *(dev->pGlobBuffer+19712+i*11+9),(PUCHAR)arg+i*10+2);
            __put_user ( *(dev->pGlobBuffer+19712+i*11+8),(PUCHAR)arg+i*10+3);
            __put_user ( *(dev->pGlobBuffer+19712+i*11+3),(PUCHAR)arg+i*10+4);
            __put_user ( *(dev->pGlobBuffer+19712+i*11+2),(PUCHAR)arg+i*10+5);
            __put_user ( *(dev->pGlobBuffer+19712+i*11+1),(PUCHAR)arg+i*10+6);
            __put_user ( *(dev->pGlobBuffer+19712+i*11),(PUCHAR)arg+i*10+7);
            temp = ((*(dev->pGlobBuffer+19712+i*11+4))<<4)+ ((*(dev->pGlobBuffer+19712+i*11+7))&0xf);
            __put_user (temp, (PUCHAR)arg+i*10+8);
            __put_user ( *(dev->pGlobBuffer+19712+i*11+10),(PUCHAR)arg+i*10+9);

            #ifdef     SPINLOCK
            spin_unlock(&dev->glob_buff_lock);
            #endif
        }

        break;

    case ECE02061_READ_PARAM_AP1:

            i = *(PUCHAR)arg;

            #ifdef     SPINLOCK
            spin_lock(&dev->glob_buff_lock);
            #endif

            __put_user ( *(dev->pGlobBuffer+i*11+6),(PUCHAR)arg);
            __put_user ( *(dev->pGlobBuffer+i*11+5),(PUCHAR)arg+1);
            __put_user ( *(dev->pGlobBuffer+i*11+9),(PUCHAR)arg+2);
            __put_user ( *(dev->pGlobBuffer+i*11+8),(PUCHAR)arg+3);
            __put_user ( *(dev->pGlobBuffer+i*11+3),(PUCHAR)arg+4);
            __put_user ( *(dev->pGlobBuffer+i*11+2),(PUCHAR)arg+5);
            __put_user ( *(dev->pGlobBuffer+i*11+1),(PUCHAR)arg+6);
            __put_user ( *(dev->pGlobBuffer+i*11),(PUCHAR)arg+i+7);
            temp = ((*(dev->pGlobBuffer+i*11+4))<<4)+ ((*(dev->pGlobBuffer+i*11+7))&0xf);
            __put_user (temp, (PUCHAR)arg+8);
            __put_user ( *(dev->pGlobBuffer+i*11+10),(PUCHAR)arg+9);

            #ifdef     SPINLOCK
            spin_unlock(&dev->glob_buff_lock);
            #endif

        break;

    case ECE02061_READ_PARAM_CC1:

            i = *(PUCHAR)arg;

            #ifdef     SPINLOCK
            spin_lock(&dev->glob_buff_lock);
            #endif

            __put_user ( *(dev->pGlobBuffer+11264+i*11+6),(PUCHAR)arg);
            __put_user ( *(dev->pGlobBuffer+11264+i*11+5),(PUCHAR)arg +1);
            __put_user ( *(dev->pGlobBuffer+11264+i*11+9),(PUCHAR)arg +2);
            __put_user ( *(dev->pGlobBuffer+11264+i*11+8),(PUCHAR)arg +3);
            __put_user ( *(dev->pGlobBuffer+11264+i*11+3),(PUCHAR)arg +4);
            __put_user ( *(dev->pGlobBuffer+11264+i*11+2),(PUCHAR)arg +5);
            __put_user ( *(dev->pGlobBuffer+11264+i*11+1),(PUCHAR)arg +6);
            __put_user ( *(dev->pGlobBuffer+11264+i*11),(PUCHAR)arg +7);
            temp = ((*(dev->pGlobBuffer+11264+i*11+4))<<4)+ ((*(dev->pGlobBuffer+11264+i*11+7))&0xf);
            __put_user (temp, (PUCHAR)arg +8);
            __put_user ( *(dev->pGlobBuffer+11264+i*11+10),(PUCHAR)arg +9);

            #ifdef     SPINLOCK
            spin_unlock(&dev->glob_buff_lock);
            #endif

        break;

    case ECE02061_READ_PARAM_AP2:

            i = *(PUCHAR)arg;

            #ifdef     SPINLOCK
            spin_lock(&dev->glob_buff_lock);
            #endif

            __put_user ( *(dev->pGlobBuffer+2816+i*11+6),(PUCHAR)arg );
            __put_user ( *(dev->pGlobBuffer+2816+i*11+5),(PUCHAR)arg +1);
            __put_user ( *(dev->pGlobBuffer+2816+i*11+9),(PUCHAR)arg +2);
            __put_user ( *(dev->pGlobBuffer+2816+i*11+8),(PUCHAR)arg +3);
            __put_user ( *(dev->pGlobBuffer+2816+i*11+3),(PUCHAR)arg +4);
            __put_user ( *(dev->pGlobBuffer+2816+i*11+2),(PUCHAR)arg +5);
            __put_user ( *(dev->pGlobBuffer+2816+i*11+1),(PUCHAR)arg +6);
            __put_user ( *(dev->pGlobBuffer+2816+i*11),(PUCHAR)arg +7);
            temp = ((*(dev->pGlobBuffer+2816+i*11+4))<<4)+ ((*(dev->pGlobBuffer+2816+i*11+7))&0xf);
            __put_user (temp, (PUCHAR)arg +8);
            __put_user ( *(dev->pGlobBuffer+2816+i*11+10),(PUCHAR)arg +9);

            #ifdef     SPINLOCK
            spin_unlock(&dev->glob_buff_lock);
            #endif
        break;

    case ECE02061_READ_PARAM_CC2:

            i = *(PUCHAR)arg;

            #ifdef     SPINLOCK
            spin_lock(&dev->glob_buff_lock);
            #endif

            __put_user ( *(dev->pGlobBuffer+14080+i*11+6),(PUCHAR)arg );
            __put_user ( *(dev->pGlobBuffer+14080+i*11+5),(PUCHAR)arg +1);
            __put_user ( *(dev->pGlobBuffer+14080+i*11+9),(PUCHAR)arg +2);
            __put_user ( *(dev->pGlobBuffer+14080+i*11+8),(PUCHAR)arg +3);
            __put_user ( *(dev->pGlobBuffer+14080+i*11+3),(PUCHAR)arg +4);
            __put_user ( *(dev->pGlobBuffer+14080+i*11+2),(PUCHAR)arg +5);
            __put_user ( *(dev->pGlobBuffer+14080+i*11+1),(PUCHAR)arg +6);
            __put_user ( *(dev->pGlobBuffer+14080+i*11),(PUCHAR)arg +7);
            temp = ((*(dev->pGlobBuffer+14080+i*11+4))<<4)+ ((*(dev->pGlobBuffer+14080+i*11+7))&0xf);
            __put_user (temp, (PUCHAR)arg +8);
            __put_user ( *(dev->pGlobBuffer+14080+i*11+10),(PUCHAR)arg +9);

            #ifdef     SPINLOCK
            spin_unlock(&dev->glob_buff_lock);
            #endif

        break;

    case ECE02061_READ_PARAM_AP3:

            i = *(PUCHAR)arg;

            #ifdef     SPINLOCK
            spin_lock(&dev->glob_buff_lock);
            #endif

            __put_user ( *(dev->pGlobBuffer+5632+i*11+6),(PUCHAR)arg );
            __put_user ( *(dev->pGlobBuffer+5632+i*11+5),(PUCHAR)arg +1);
            __put_user ( *(dev->pGlobBuffer+5632+i*11+9),(PUCHAR)arg +2);
            __put_user ( *(dev->pGlobBuffer+5632+i*11+8),(PUCHAR)arg +3);
            __put_user ( *(dev->pGlobBuffer+5632+i*11+3),(PUCHAR)arg +4);
            __put_user ( *(dev->pGlobBuffer+5632+i*11+2),(PUCHAR)arg +5);
            __put_user ( *(dev->pGlobBuffer+5632+i*11+1),(PUCHAR)arg +6);
            __put_user ( *(dev->pGlobBuffer+5632+i*11),(PUCHAR)arg +7);
            temp = ((*(dev->pGlobBuffer+5632+i*11+4))<<4)+ ((*(dev->pGlobBuffer+5632+i*11+7))&0xf);
            __put_user (temp, (PUCHAR)arg +8);
            __put_user ( *(dev->pGlobBuffer+5632+i*11+10),(PUCHAR)arg +9);

            #ifdef     SPINLOCK
            spin_unlock(&dev->glob_buff_lock);
            #endif

        break;

    case ECE02061_READ_PARAM_CC3:

            i = *(PUCHAR)arg;

            #ifdef     SPINLOCK
            spin_lock(&dev->glob_buff_lock);
            #endif

            __put_user ( *(dev->pGlobBuffer+16896+i*11+6),(PUCHAR)arg );
            __put_user ( *(dev->pGlobBuffer+16896+i*11+5),(PUCHAR)arg +1);
            __put_user ( *(dev->pGlobBuffer+16896+i*11+9),(PUCHAR)arg +2);
            __put_user ( *(dev->pGlobBuffer+16896+i*11+8),(PUCHAR)arg +3);
            __put_user ( *(dev->pGlobBuffer+16896+i*11+3),(PUCHAR)arg +4);
            __put_user ( *(dev->pGlobBuffer+16896+i*11+2),(PUCHAR)arg +5);
            __put_user ( *(dev->pGlobBuffer+16896+i*11+1),(PUCHAR)arg +6);
            __put_user ( *(dev->pGlobBuffer+16896+i*11),(PUCHAR)arg +7);
            temp = ((*(dev->pGlobBuffer+16896+i*11+4))<<4)+ ((*(dev->pGlobBuffer+16896+i*11+7))&0xf);
            __put_user (temp, (PUCHAR)arg +8);
            __put_user ( *(dev->pGlobBuffer+16896+i*11+10),(PUCHAR)arg +9);

            #ifdef     SPINLOCK
            spin_unlock(&dev->glob_buff_lock);
            #endif

        break;

    case ECE02061_READ_PARAM_AP4:

            i = *(PUCHAR)arg;

            #ifdef     SPINLOCK
            spin_lock(&dev->glob_buff_lock);
            #endif

            __put_user ( *(dev->pGlobBuffer+8448+i*11+6),(PUCHAR)arg );
            __put_user ( *(dev->pGlobBuffer+8448+i*11+5),(PUCHAR)arg +1);
            __put_user ( *(dev->pGlobBuffer+8448+i*11+9),(PUCHAR)arg +2);
            __put_user ( *(dev->pGlobBuffer+8448+i*11+8),(PUCHAR)arg +3);
            __put_user ( *(dev->pGlobBuffer+8448+i*11+3),(PUCHAR)arg +4);
            __put_user ( *(dev->pGlobBuffer+8448+i*11+2),(PUCHAR)arg +5);
            __put_user ( *(dev->pGlobBuffer+8448+i*11+1),(PUCHAR)arg +6);
            __put_user ( *(dev->pGlobBuffer+8448+i*11),(PUCHAR)arg +7);
            temp = ((*(dev->pGlobBuffer+8448+i*11+4))<<4)+ ((*(dev->pGlobBuffer+8448+i*11+7))&0xf);
            __put_user (temp, (PUCHAR)arg +8);
            __put_user ( *(dev->pGlobBuffer+8448+i*11+10),(PUCHAR)arg +9);

            #ifdef     SPINLOCK
            spin_unlock(&dev->glob_buff_lock);
            #endif

        break;

    case ECE02061_READ_PARAM_CC4:

            i = *(PUCHAR)arg;

            #ifdef     SPINLOCK
            spin_lock(&dev->glob_buff_lock);
            #endif

            __put_user ( *(dev->pGlobBuffer+19712+i*11+6),(PUCHAR)arg );
            __put_user ( *(dev->pGlobBuffer+19712+i*11+5),(PUCHAR)arg +1);
            __put_user ( *(dev->pGlobBuffer+19712+i*11+9),(PUCHAR)arg +2);
            __put_user ( *(dev->pGlobBuffer+19712+i*11+8),(PUCHAR)arg +3);
            __put_user ( *(dev->pGlobBuffer+19712+i*11+3),(PUCHAR)arg +4);
            __put_user ( *(dev->pGlobBuffer+19712+i*11+2),(PUCHAR)arg +5);
            __put_user ( *(dev->pGlobBuffer+19712+i*11+1),(PUCHAR)arg +6);
            __put_user ( *(dev->pGlobBuffer+19712+i*11),(PUCHAR)arg +7);
            temp = ((*(dev->pGlobBuffer+19712+i*11+4))<<4)+ ((*(dev->pGlobBuffer+19712+i*11+7))&0xf);
            __put_user (temp, (PUCHAR)arg +8);
            __put_user ( *(dev->pGlobBuffer+19712+i*11+10),(PUCHAR)arg +9);

            #ifdef     SPINLOCK
            spin_unlock(&dev->glob_buff_lock);
            #endif

        break;

    case ECE02061_CLEAR_SI_ARRAY:

        __get_user ( dev->numberCH,(unsigned char *)arg);

        if((!dev->numberCH)||(dev->numberCH > SI_CHANNAL_NUMBER))
        {
            __put_user ( 1,(unsigned char*)arg);
            break;
        }



        switch (dev->numberCH)
        {
        case 1:
            #ifdef     SPINLOCK
            spin_lock(&dev->glob_buff_lock);
            #endif

            *(dev->pGlobBuffer + 22528) = 0;

            for ( i=0; i<2816; i++)
            {
                *(dev->pGlobBuffer + i) = 0;
                *(dev->pGlobBuffer + 11264+i) = 0;
            }

            #ifdef     SPINLOCK
            spin_unlock(&dev->glob_buff_lock);
            #endif
            break;

        case 2:
            #ifdef     SPINLOCK
            spin_lock(&dev->glob_buff_lock);
            #endif

            *(dev->pGlobBuffer + 22529) = 0;

            for ( i=0; i<2816; i++)
            {
                *(dev->pGlobBuffer + 2816+i) = 0;
                *(dev->pGlobBuffer + 14080+i) = 0;
            }

            #ifdef     SPINLOCK
            spin_unlock(&dev->glob_buff_lock);
            #endif
            break;

        case 3:
            #ifdef     SPINLOCK
            spin_lock(&dev->glob_buff_lock);
            #endif

            *(dev->pGlobBuffer + 22530) = 0;

            for ( i=0; i<2816; i++)
            {
                *(dev->pGlobBuffer + 5632+i) = 0;
                *(dev->pGlobBuffer + 16896+i) = 0;
            }

            #ifdef     SPINLOCK
            spin_unlock(&dev->glob_buff_lock);
            #endif
            break;

        case 4:
            #ifdef     SPINLOCK
            spin_lock(&dev->glob_buff_lock);
            #endif

            *(dev->pGlobBuffer + 22531) = 0;

            for ( i=0; i<2816; i++)
            {
                *(dev->pGlobBuffer + 8448+i) = 0;
                *(dev->pGlobBuffer + 19712+i) = 0;
            }

            #ifdef     SPINLOCK
            spin_unlock(&dev->glob_buff_lock);
            #endif
            break;
        }


        __put_user ( 0,(unsigned char*)arg);
        break;

    case ECE02061_GET_TIMER:

            #ifdef     SPINLOCK
            spin_lock(&dev->glob_buff_lock);
            #endif

            __put_user ( *(dev->pGlobBuffer+22546),(PUCHAR)(arg));
            __put_user ( *(dev->pGlobBuffer+22545),(PUCHAR)arg+1);
            __put_user ( *(dev->pGlobBuffer+22544),(PUCHAR)arg+2);
            __put_user ( *(dev->pGlobBuffer+22547),(PUCHAR)(arg+3));

            #ifdef     SPINLOCK
            spin_unlock(&dev->glob_buff_lock);
            #endif



    break;

    case ECE02061_SET_SHORT_MODE:

            dev->SImode.SImode[3] |= 2;

        break;

    case ECE02061_SET_LONG_MODE:

        dev->SImode.SImode[3] &= 0xfd;

        break;

    case ECE02061_DO_CLR:

        dev->SOmode.SOmodeCommand1 = 0x20;
        dev->SOmode.SOmodeMode = dev->SOmode.SOmodeMode & 0xcf;


        retval = usb_bulk_msg(dev->udev,
            usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr),
            &dev->SOmode,
            6,
            &bytes_read,
            100);


        break;

    case ECE02061_DO_SET:
        dev->SOmode.SOmodeCommand1 = 0x20;
        dev->SOmode.SOmodeMode = (dev->SOmode.SOmodeMode & 0xcf)|0x20;


        retval = usb_bulk_msg(dev->udev,
            usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr),
            &dev->SOmode,
            6,
            &bytes_read,
            100);

        break;

    case ECE02061_DO_SPRT:

         dev->SOmode.SOmodeMode = (dev->SOmode.SOmodeMode & 0xcf)|0x10;

        break;
    }
exit:
    mutex_unlock(&dev->io_mutex);
    return retval;
}


//...........................................................................
//            flush
//...........................................................................
static int ece0206_flush(struct file *file, fl_owner_t id)
{
    struct ece0206_device *dev;
    int res;
    int bytes_read;

    dev = (struct ece0206_device *)file->private_data;
    if (dev == NULL)
        return -ENODEV;

    /* wait for io to stop */
    mutex_lock(&dev->io_mutex);

    //SI stop
     dev->SImode.SImode[0] = 0;
     dev->SImode.SImode[1] = 0;
     dev->SImode.SImode[2] = 0;
     dev->SImode.SImode[3] = 0;
     dev->SImode.SImodeCommand1 = 0x24;
    usb_bulk_msg(dev->udev,
            usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr),
            &dev->SImode,
            6,
            &bytes_read,
            100);
    //SO stop
    dev->SOmode.SOmodeCommand1 = 0x20;
    dev->SOmode.SOmodeMode = 0;
    usb_bulk_msg(dev->udev,
            usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr),
            &dev->SOmode,
            6,
            &bytes_read,
            100);


    ece0206_draw_down(dev);

    spin_lock_irq(&dev->err_lock);
    res = dev->errors ? (dev->errors == -EPIPE ? -EPIPE : -EIO) : 0;
    dev->errors = 0;
    spin_unlock_irq(&dev->err_lock);

    mutex_unlock(&dev->io_mutex);

    printk("ECE0206lin driver: flush \n");
    return res;
}




//...........................................................................
//            file_operations
//...........................................................................
static const struct file_operations ece0206_fops = {
    .owner =    THIS_MODULE,
     .ioctl =     ece0206_ioctl,
    .open =        ece0206_open,
    .release =    ece0206_release,
    .flush =    ece0206_flush,
};

/*
 * usb class driver info in order to get a minor number from the usb core,
 * and to have the device registered with the driver core
 */
static struct usb_class_driver ece0206_class = {
    .name = "ece0206_%d",
    .fops = &ece0206_fops,
    //.mode = S_IFCHR | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH,
    //.minor_base =    ece0206_device_MINOR_BASE,
};


//...........................................................................
//            probe
//...........................................................................
static int ece0206_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    struct ece0206_device *dev;
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint;
    size_t buffer_size;
    int i;
    int retval = -ENOMEM;

    /* allocate memory for our device state and initialize it */
    dev = kzalloc(sizeof(*dev), GFP_ATOMIC);
    if (!dev) {
        err("kzalloc(sizeof(*dev), GFP_KERNEL): Out of memory \n");
        goto error;
    }

    kref_init(&dev->kref);
    mutex_init(&dev->io_mutex);
    spin_lock_init(&dev->err_lock);
    #ifdef     SPINLOCK
    spin_lock_init(&dev->glob_buff_lock);
    #endif
    init_usb_anchor(&dev->submitted);

    dev->udev = usb_get_dev(interface_to_usbdev(interface));
    dev->interface = interface;

    /* set up the endpoint information */
    iface_desc = interface->cur_altsetting;
    for (i = 0; i < iface_desc->desc.bNumEndpoints; ++i)
    {
        endpoint = &iface_desc->endpoint[i].desc;


        if((!dev->bulk_in_endpointAddr)
               && usb_endpoint_is_bulk_in(endpoint))
        {

              /* we found a bulk in endpoint */
            buffer_size = le16_to_cpu(endpoint->wMaxPacketSize);
            dev->bulk_in4_size = buffer_size;
            dev->bulk_in_endpointAddr = endpoint->bEndpointAddress;
            dev->bulk_in4_buffer = kmalloc(buffer_size, GFP_ATOMIC);
            if (!dev->bulk_in4_buffer) {
                err("Could not allocate bulk_in_buffer");
                goto error;
            }
        }

        if (!dev->bulk_out_endpointAddr
               && usb_endpoint_is_bulk_out(endpoint))
        {
            /* we found a bulk out endpoint */
            buffer_size = le16_to_cpu(endpoint->wMaxPacketSize);
            dev->bulk_out2_size = buffer_size;
            dev->bulk_out_endpointAddr = endpoint->bEndpointAddress;
        }

        if (!dev->interr_in_endpointAddr
                && usb_endpoint_is_int_in(endpoint))
        {
              /* we found a interrupt in endpoint */
            buffer_size = le16_to_cpu(endpoint->wMaxPacketSize);
            dev->interr_in6_size = buffer_size;
            dev->interr_in_endpointAddr = endpoint->bEndpointAddress;
            dev->interval = endpoint-> bInterval;
            dev->interr_in6_buffer = kmalloc(buffer_size, GFP_ATOMIC);
            if (!dev->interr_in6_buffer) {
                err("Could not allocate int_in_buffer");
                goto error;
            }
            dev->pGlobBuffer = &dev->GlobBuffer[0] ;

        }
    }
        if ((dev->interr_in6_size!=1024)
            || (dev->bulk_out2_size!=512)
            || (dev->bulk_in4_size!=512))
        {
            printk("ECE0206lin driver: size error %d   %d   %d \n"
                ,dev->interr_in6_size
                ,dev->bulk_out2_size
                ,dev->bulk_in4_size);
            goto error;
        }

    /* save our data pointer in this interface device */
    usb_set_intfdata(interface, dev);

    /* we can register the device now, as it is ready */
    retval = usb_register_dev(interface, &ece0206_class);
    if (retval) {
        /* something prevented us from registering this driver */
        err("Not able to get a minor for this device.");
        usb_set_intfdata(interface, NULL);
        goto error;
    }

    /* let the user know what node this device is now attached to */
    printk("ECE0206 device now attached to ECE0206lin-%d  \n", interface->minor);
    return 0;

error:
    if (dev)
        /* this frees allocated memory */
        kref_put(&dev->kref, ece0206_delete);
    printk("ECE0206lin driver: probe error \n");
    return retval;
}

//...........................................................................
//            disconnect
//...........................................................................
static void ece0206_disconnect(struct usb_interface *interface)
{
    struct ece0206_device *dev;
    int minor = interface->minor;

    dev = usb_get_intfdata(interface);
    usb_set_intfdata(interface, NULL);

    /* give back our minor */
    usb_deregister_dev(interface, &ece0206_class);

    /* prevent more I/O from starting */
    mutex_lock(&dev->io_mutex);
    dev->interface = NULL;
    mutex_unlock(&dev->io_mutex);

    usb_kill_anchored_urbs(&dev->submitted);

    /* decrement our usage count */
    kref_put(&dev->kref, ece0206_delete);

    printk("ECE0206 #%d now disconnected  \n", minor);
}


//...........................................................................
//            draw_down
//...........................................................................
static void ece0206_draw_down(struct ece0206_device *dev)
{
    int time;

    time = usb_wait_anchor_empty_timeout(&dev->submitted, 1000);
    if (!time)
        usb_kill_anchored_urbs(&dev->submitted);
    printk("ECE0206lin driver: draw_down \n");
}


//...........................................................................
//            suspend
//...........................................................................
static int ece0206_suspend(struct usb_interface *intf, pm_message_t message)
{
    struct ece0206_device *dev = usb_get_intfdata(intf);

    if (!dev)
        return 0;
    ece0206_draw_down(dev);
    printk("ECE0206lin driver: suspend \n");
    return 0;
}


//...........................................................................
//            resume
//...........................................................................
static int ece0206_resume (struct usb_interface *intf)
{
    printk("ECE0206lin driver: resume \n");
    return 0;
}


//...........................................................................
//        struct usb_driver
//...........................................................................
static struct usb_driver ece0206_driver = {
    .name =        "ECE0206lin",
    .probe =    ece0206_probe,
    .disconnect =    ece0206_disconnect,
    .suspend =    ece0206_suspend,
    .resume =    ece0206_resume,
    .id_table =    ece0206_table,
    .supports_autosuspend = 1,
};


//...........................................................................
//            init
//...........................................................................
static int __init ece0206_init(void)
{
    int result;

    /* register this driver with the USB subsystem */
    result = usb_register(&ece0206_driver);
    if (result)
        err("ECE0206lin driver: usb_register failed. Error number %d", result);
    else
        printk("ECE0206lin driver: register \n");

    return result;
}


//...........................................................................
//            exit
//...........................................................................
static void __exit ece0206_exit(void)
{
    /* deregister this driver with the USB subsystem */
    usb_deregister(&ece0206_driver);

    printk("ECE0206lin driver: deregister \n");
}

module_init(ece0206_init);
module_exit(ece0206_exit);

MODULE_LICENSE("GPL");

