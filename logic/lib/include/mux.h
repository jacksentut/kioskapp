#ifndef _MUX_H_
    #define _MUX_H_

#define MUXDEVNAME          "/dev/mux"

#define MUX_IOCTL_MAGIC         'm'
#define MUX_BARCODE	     	    _IO(  MUX_IOCTL_MAGIC,  0)
#define MUX_MODEM 	     	    _IO(  MUX_IOCTL_MAGIC,  1)
#define MUX_VERSION             _IOR( MUX_IOCTL_MAGIC, 99, unsigned char *)

#endif



