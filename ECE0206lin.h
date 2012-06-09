
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kref.h>
#include <asm/uaccess.h>
#include <linux/usb.h>
#include <linux/mutex.h>
#include "ECE0206lin_ioctl.h"

#define ECE0206_VENDOR_ID	0x04b4
#define ECE0206_PRODUCT_ID	0x8613

#define	UCHAR	 unsigned char
#define	PUCHAR	 unsigned char *
#define	USHORT	 unsigned short int
#define	ULONG	 unsigned long int

#define	SI_CHANNAL_NUMBER	4

//#define	SPINLOCK	1
/*
extern 	UCHAR BuffAP_1[2816];	0		
extern 	UCHAR BuffAP_2[2816];	2816		
extern 	UCHAR BuffAP_3[2816];	5632		
extern 	UCHAR BuffAP_4[2816];	8448		
extern 	UCHAR BuffCC_1[2816];	11264		
extern 	UCHAR BuffCC_2[2816];	14080		
extern 	UCHAR BuffCC_3[2816];	16896		
extern 	UCHAR BuffCC_4[2816];	19712		
extern	UCHAR countCC_1;	22528		
extern	UCHAR countCC_2;	22529		
extern	UCHAR countCC_3;	22530		
extern	UCHAR countCC_4;	22531		

extern	UCHAR lowW_10,lowW_11,lowW_12;	22532		
extern	UCHAR lowW_20,lowW_21,lowW_22;	22535		
extern	UCHAR lowW_30,lowW_31,lowW_32;	22538		
extern	UCHAR lowW_40,lowW_41,lowW_42;	22541		


extern	UCHAR ourTimer1;	22544		
extern	UCHAR ourTimer2;	22545		
extern	UCHAR ourTimer3;	22546		
extern	UCHAR ourTimer;	22547		
*/

struct _SOmode1
{
	UCHAR	SOmodeCommand0;
	UCHAR	SOmodeCommand1;
	UCHAR   SOmodeDelay;
	UCHAR   SOmodeFrameNumber;	
	UCHAR   SOmodeParamNumber;  
	UCHAR   SOmodeMode;				
};

struct _SImode
{
	UCHAR	SImodeCommand0;
	UCHAR	SImodeCommand1;
	UCHAR   SImode[SI_CHANNAL_NUMBER];
};


struct ece0206_device 
{
	struct usb_device	*udev;			/* the usb device for this device */
	struct usb_interface	*interface;		/* the interface for this device */
	struct usb_anchor	submitted;		/* in case we need to retract our submissions */


	size_t			bulk_out2_size;		/* the size of the send buffer */
	unsigned char           *bulk_in4_buffer;	/* the buffer to receive data */
	size_t			bulk_in4_size;		/* the size of the receive buffer */
	unsigned char           *interr_in6_buffer;	/* the buffer to receive data */
	size_t			interr_in6_size;		/* the size of the receive buffer */

	int			errors;			/* the last request tanked */
	int			open_count;		/* count the number of openers */
	spinlock_t		err_lock;		/* lock for errors */
	#ifdef     SPINLOCK
	spinlock_t		glob_buff_lock;		/* lock */
	#endif
	struct kref		kref;
	struct mutex		io_mutex;		/* synchronize I/O with disconnect */
	__u8			bulk_in_endpointAddr;	/* the address of the bulk in endpoint */
	__u8			bulk_out_endpointAddr;	/* the address of the bulk out endpoint */
	__u8			interr_in_endpointAddr;	/* the address of the interr in endpoint */
	int			interval;

	struct urb *		urbINT;
	struct _SOmode1		SOmode;
	struct _SImode		SImode;


	UCHAR   		numberCH;
	UCHAR			data[510];				
	UCHAR 			GlobBuffer[22548];  
	PUCHAR 			pGlobBuffer;

};

	

