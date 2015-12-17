
#define HNX720T_GLOBALS

#include "include.h"

extern int uart0_fd, uart1_fd, uart2_fd, uart3_fd;

extern u8 open_dev(void);
extern void kiosk_MainLp(void);
extern void wait_command(void);
extern void help_mode(void);
extern void put_prompt(void);
extern void on_line(void);
extern void kiosk_comm_test(void);
extern void prt_test1(unsigned int i);

// jump
jmp_buf jmpb;

void goto_mainstep(void)
{
    longjmp(jmpb, 1);
}

u8 open_dev(void)
{
    u8 ret=0;
    
    if ((rtcfd = open("/dev/rtc", O_RDWR)) < 0)
    {
        perror("RTC open error : ");
        close(rtcfd);
        return ret;
    }
    
    if ((timerfd = open("/dev/timer", O_RDONLY)) < 0)
    {
        perror("timer open error : ");
        close(timerfd);
        return ret;
    }
    
    if ((printfd = open("/dev/printer", O_RDONLY)) < 0)
    {
        perror("printer open error : ");
        close(printfd);
        return ret;
    }
	font_open();
    ret = 0x11;
    
    return ret;
}

int main(void)
{
#if 0
	system("rmmod /home/root/printer.ko");
	system("insmod /home/root/printer.ko");
#endif
//	system("rmmod /home/root/timer.ko");
//	system("rmmod /home/root/mux.ko");
//	system("insmod /home/root/timer.ko");
//	system("insmod /home/root/mux.ko");

#if 0
	system("rmmod /home/root/printer.ko");
	system("rmmod /home/root/timer.ko");
	system("rmmod /home/root/mux.ko");
	system("insmod /home/root/printer.ko");
	system("insmod /home/root/timer.ko");
	system("insmod /home/root/mux.ko");
#endif

    if (!open_dev())
    {
        perror("device open error :");
        return(0);
    }
	field_not_erase = 0;
	
	sio_init();
    app_init(0);
    swval_init();
	bill_init();
	// 메시지큐 삭제
	//msgctl(SENDQ_ID, IPC_RMID, 0);
	//msgctl(RECVQ_ID, IPC_RMID, 0);
	
    socketfd = -1;
    link_fd  = -1;
    wcdma_fd = -1;

    if(Region01.Comm.comm_line & ETHERNET_LINE)
    {
        if(Region01.Comm.Ethernet.ipmethod == '1')
        {
	        aDebugMsg("  네트웍  관리  ");
	        aDebugMsg(" 자동 IP 주소를 ");
			aDebugMsg("  설정중 입니다 ");
        }

        if(init_ethernet()==TRUE)
        {
    	    ;
        }
        else
        {
            if(ethernet_link_chk()!=1)
            {
                aDebugMsg("  네트웍  관리  ");
                aDebugMsg("네트웍 상태 오류");
            }
            else
            {
                if(init_ethernet()==TRUE)
                {
            	    ;
                }
                else
                {
                    aDebugMsg("  네트웍  관리  ");
                	aDebugMsg("네트웍 상태 오류");
                }
            }
        }
    }

    setjmp(jmpb);

#ifdef SELF_DEBUG	
	wait_command();
#else
	tms_check_flag = 0;
	tms_update_flag = 0;
	kiosk_MainLp();
#endif

}


