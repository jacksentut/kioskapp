#ifndef _PRINTER_H_
    #define _PRINTER_H_

#define PRINTERDEVNAME          "/dev/printer"

#define PRINTTING               0x30
#define NO_PRINTTING            0x31

#define NO_PAPER_EXIST          0x32
#define PAPER_EXIST             0x33

#define OVER_HIT                0x34
#define NO_OVER_HIT             0x35

#define NO_CUTTER_EXIST			0x36
#define CUTTER_EXIST			0x37

#define COVER_OPEN				0x38
#define COVER_CLOSE				0x39

typedef struct {
    unsigned int dots;
    unsigned char **pdata;
}__attribute__((packed))Prt_Info;

#define PRN_IOCTL_MAGIC             'p'
#define PRN_PAPER_PRINT             _IOW(PRN_IOCTL_MAGIC, 0,  Prt_Info)
#define PRN_PRINT_INIT              _IO( PRN_IOCTL_MAGIC, 1)
#define PRN_STEPMOVE                _IO( PRN_IOCTL_MAGIC, 2)
#define PRN_MOVE_PAPER              _IOW(PRN_IOCTL_MAGIC, 3,  int)
#define PRN_BRIGHT                  _IOW(PRN_IOCTL_MAGIC, 4,  int)
#define PRN_AC_PARTIAL              _IO( PRN_IOCTL_MAGIC, 5)
#define PRN_AC_FULL                 _IO( PRN_IOCTL_MAGIC, 6)
#define PRN_PAPER_CHECK             _IOR(PRN_IOCTL_MAGIC, 7,  int)
#define PRN_OVERHIT_CHECK           _IOR(PRN_IOCTL_MAGIC, 8,  int)
#define PRN_COVER_CHECK           	_IOR(PRN_IOCTL_MAGIC, 9,  int)
#define PRN_AC_CHECK        	   	_IOR(PRN_IOCTL_MAGIC, 10, int)
#define PRN_PRINT_STATUS            _IOR(PRN_IOCTL_MAGIC, 11, int)
#define PRN_STEP_STOP               _IO( PRN_IOCTL_MAGIC, 12)
#define PRN_AC_CUTINIT				_IO( PRN_IOCTL_MAGIC, 13)
#define PRN_SPEED                   _IOW(PRN_IOCTL_MAGIC, 14, int)
#define PRN_VERSION                 _IOR(PRN_IOCTL_MAGIC, 99, unsigned char *)

#endif



