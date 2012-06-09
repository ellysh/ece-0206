
#include <linux/ioctl.h>
#ifndef __ECE02061_ioctl__h_
#define __ECE02061_ioctl__h_

#define ece0206_IOC_MAGIC 'k'

#define ECE02061_GET_SERIAL_NUMBER	_IOWR(ece0206_IOC_MAGIC,131,unsigned int)

#define ECE02061_WRITE_PARAM_1	_IOWR(ece0206_IOC_MAGIC,100, unsigned int)
#define ECE02061_READ_PARAM_1	_IOWR(ece0206_IOC_MAGIC,101, unsigned int)
#define ECE02061_SO_PUSK1	_IOWR(ece0206_IOC_MAGIC,102, unsigned int)
#define ECE02061_SO_STOP1	_IO(ece0206_IOC_MAGIC,103)
#define ECE02061_SO_STATE1	_IOWR(ece0206_IOC_MAGIC,104, unsigned int)
#define ECE02061_WRITE_ARRAY_1	_IOWR(ece0206_IOC_MAGIC,105, unsigned int)
#define ECE02061_CLEAR_ARRAY_1	_IOWR(ece0206_IOC_MAGIC,106, unsigned int)

#define ECE02061_SI_PUSK	_IOWR(ece0206_IOC_MAGIC,107, unsigned int)
#define ECE02061_SI_STOP	_IOWR(ece0206_IOC_MAGIC,108, unsigned int)

#define ECE02061_CLEAR_SI_ARRAY	_IOWR(ece0206_IOC_MAGIC,109, unsigned int)

#define ECE02061_READ_ARRAY_AP1	_IOR(ece0206_IOC_MAGIC,110, unsigned int)
#define ECE02061_READ_ARRAY_CC1	_IOR(ece0206_IOC_MAGIC,111, unsigned int)
#define ECE02061_READ_ARRAY_AP2	_IOR(ece0206_IOC_MAGIC,112, unsigned int)
#define ECE02061_READ_ARRAY_CC2	_IOR(ece0206_IOC_MAGIC,113, unsigned int)
#define ECE02061_READ_ARRAY_AP3	_IOR(ece0206_IOC_MAGIC,114, unsigned int)
#define ECE02061_READ_ARRAY_CC3	_IOR(ece0206_IOC_MAGIC,115, unsigned int)
#define ECE02061_READ_ARRAY_AP4	_IOR(ece0206_IOC_MAGIC,116, unsigned int)
#define ECE02061_READ_ARRAY_CC4	_IOR(ece0206_IOC_MAGIC,117, unsigned int)
#define ECE02061_READ_PARAM_AP1	_IOR(ece0206_IOC_MAGIC,118, unsigned int)
#define ECE02061_READ_PARAM_AP2	_IOR(ece0206_IOC_MAGIC,119, unsigned int)
#define ECE02061_READ_PARAM_AP3	_IOR(ece0206_IOC_MAGIC,120, unsigned int)
#define ECE02061_READ_PARAM_AP4	_IOR(ece0206_IOC_MAGIC,121, unsigned int)
#define ECE02061_READ_PARAM_CC1	_IOR(ece0206_IOC_MAGIC,122, unsigned int)
#define ECE02061_READ_PARAM_CC2	_IOR(ece0206_IOC_MAGIC,123, unsigned int)
#define ECE02061_READ_PARAM_CC3	_IOR(ece0206_IOC_MAGIC,124, unsigned int)
#define ECE02061_READ_PARAM_CC4	_IOR(ece0206_IOC_MAGIC,125, unsigned int)

#define ECE02061_SET_SHORT_MODE	_IO(ece0206_IOC_MAGIC,126)
#define ECE02061_SET_LONG_MODE	_IO(ece0206_IOC_MAGIC,127)


#define ECE02061_GET_TIMER	_IOWR(ece0206_IOC_MAGIC,128, unsigned int)


#define ECE02061_DO_CLR		_IO(ece0206_IOC_MAGIC,129)
#define ECE02061_DO_SET		_IO(ece0206_IOC_MAGIC,130)
#define ECE02061_DO_SPRT	_IO(ece0206_IOC_MAGIC,131)

#define ECE02061_GET_PRODUCT_NAME	_IOR(ece0206_IOC_MAGIC,132, unsigned int)
#define ECE02061_GET_MANUFACTURER_NAME	_IOR(ece0206_IOC_MAGIC,133, unsigned int)
#endif
