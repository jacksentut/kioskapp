
#define _OS_BASE_PROGRAM_

#ifdef _OS_BASE_PROGRAM_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <time.h>
    #include <sys/ioctl.h>
    #include <poll.h>
    #include <errno.h>
    #include <termios.h>
    #include <sys/time.h>
    #include <linux/rtc.h>
    #include <sys/ipc.h>
    #include <sys/shm.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <sys/stat.h>
    #include <setjmp.h>
    #include <gdbm.h>
    #include <pthread.h>
    #include <sys/socket.h> 
    #include <netinet/in.h> 
    #include <arpa/inet.h> 
    #include <netdb.h> 
    #include <assert.h>
    #include <stdarg.h>
	#include <sys/ipc.h>
	#include <sys/msg.h>
	#include <dirent.h>
    
    typedef unsigned char   uchar;
    //typedef unsigned int    uint;
    //typedef unsigned long   ulong;
    typedef unsigned char   byte;
    typedef unsigned int    word;
    typedef unsigned long   dword;
    typedef unsigned char   BYTE;
    typedef unsigned int    WORD;
    typedef volatile unsigned long  vu_long;
    typedef volatile unsigned short vu_short;
    typedef volatile unsigned char  vu_char;
    typedef unsigned int   u32;
    typedef unsigned short u16;
    typedef int            s32;
    typedef short int      s16;
    typedef unsigned char  u8; 
    typedef signed char    s8; 
    typedef unsigned    char    BOOLEAN;
    typedef unsigned    char    INT8U;      /* Unsigned 8  bit value            */
    typedef signed      char    INT8S;      /* Signed   8  bit value            */
    typedef unsigned    short   INT16U;     /* Unsigned 16 bit value            */
    typedef signed      short   INT16S;     /* Signed   16 bit value            */
    typedef unsigned    int     INT32U;     /* Unsigned 32 bit value            */
    typedef signed      int     INT32S;     /* Signed   32 bit value            */
    
    #include "./lib/libdef.h"
//    #include "./lib/libpub.h"
    #include "./lib/libfunc.h"
//    #include "./lib/libflash.h"

    #ifdef HNX720T_GLOBALS
        #define HNX720T_EXT
    #else
        #define HNX720T_EXT    extern
    #endif

	#include "app_def.h"
	#include "app_func.h"
	#include "api_lib.h"
	#include "app_pub.h"
	#include "const.h"
	#include "INPCipherSVC.h"
#endif


