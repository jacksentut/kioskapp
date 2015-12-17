#ifndef _TIMER_H_
#define _TIMER_H_

#define TIMERDEVNAME    "/dev/timer"

#define MAXTIMER        40          /**< \C3ִ\EB Ÿ\C0̸\D3 \B0\B9\BC\F6*/

struct timer_manager {
	int index;
	int expires;
};

#define TIMER_IOCTL_MAGIC           't'
#define TIMER_INIT                  _IO(  TIMER_IOCTL_MAGIC, 0)
#define TIMER_SET_TIME              _IOW( TIMER_IOCTL_MAGIC, 1,               struct timer_manager)
#define TIMER_RESET_TIME            _IOW( TIMER_IOCTL_MAGIC, 2,  struct timer_manager)
#define TIMER_CHECK                 _IOWR(TIMER_IOCTL_MAGIC, 3,  int)
#define TIMER_RELEASE               _IOW( TIMER_IOCTL_MAGIC, 4,  int)
#define TIMER_TEST                  _IO(  TIMER_IOCTL_MAGIC, 5)
#define TIMER_USEFUL                _IOR( TIMER_IOCTL_MAGIC, 7,  int)
#define TIMER_VERSION               _IOR( TIMER_IOCTL_MAGIC, 99, unsigned char *)



#endif
