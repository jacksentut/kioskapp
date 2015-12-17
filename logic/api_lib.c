
#include "include.h"
#include "./lib/include/printer.h"
#include "./lib/include/timer.h"
#include <stddef.h>
//#include <linux/mtd/mtd.h>
#include <linux/sockios.h>
#include <net/if.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/time.h>


///////////////////////////////////////////////////////////
// timer
///////////////////////////////////////////////////////////
typedef struct{
    int process_cnt;
    char tm_id[MAXTIMER];
    int  tm_cnt[MAXTIMER];
} ST_TERM_TIMER;
ST_TERM_TIMER Ttimer;
#define MAXAPPTIMER    20

void api_release_alltimer(void)
{
    memset(&Ttimer, 0, sizeof(ST_TERM_TIMER));
    if (ioctl(timerfd, TIMER_INIT) < 0)
    {
        perror("ioctl TIMER_INIT error ");
        return;
    }
    return;
}

char api_set_apptimer(int id, int expires)
{
    signed char ret;
    struct timer_manager timer_list;
    int Tid;

    Tid = id+1;
    if (Tid > MAXAPPTIMER || Tid < 0)
    {
        printf("MAXTIMER = %d, id = %d\n",MAXAPPTIMER,Tid);
        return(FALSE);
    }
	if(id>0)
		api_release_apptimer(id);
	memset(&timer_list, 0, sizeof(timer_list)); 
    
    timer_list.index = Tid;
    timer_list.expires = expires;
	//printf("Tid = %d, id = %d, expires = %d\n",Tid,id,expires);

    if ((ret = ioctl(timerfd, TIMER_SET_TIME, &timer_list)) < 0)
    {
        perror("ioctl TIMER_SET_TIME error ");
        return(FALSE);
    }

    return(TRUE);
}

char api_reset_apptimer(int id, int expires)
{
    signed char ret;
    struct timer_manager timer_list;
    int Tid;

    Tid = id+1;
    if (Tid > MAXAPPTIMER || Tid < 0)
    {
        printf("MAXTIMER = %d, id = %d\n",MAXAPPTIMER,Tid);
        return(FALSE);
    }
    
    memset(&timer_list, 0, sizeof(timer_list));

    timer_list.index = Tid;
    timer_list.expires = expires;

    if ((ret = ioctl(timerfd, TIMER_RESET_TIME, &timer_list)) < 0)
    {
        perror("ioctl TIMER_RESET_TIME error ");
        return(FALSE);
    }
    return(TRUE);
}

char api_chk_apptimer(int id)
{
    int Tid;

    Tid = id+1;
    if (Tid > MAXAPPTIMER || Tid < 0)
    {
        printf("MAXTIMER = %d, id = %d\n",MAXAPPTIMER,Tid);
        return(FALSE);
    }
    
    if (ioctl(timerfd, TIMER_CHECK, &Tid) < 0)
    {
        perror("ioctl TIMER_CHECK error ");
        return(FALSE);
    }
    if (Tid) return(FALSE);
    else     return(TRUE);
}

char api_release_apptimer(int id)
{
    int Tid;

    Tid = id+1;
    if (Tid > MAXAPPTIMER || Tid < 0)
    {
        printf("MAXTIMER = %d, id = %d\n",MAXAPPTIMER, Tid);
        return(FALSE);
    }
    if (ioctl(timerfd, TIMER_RELEASE, &Tid) < 0)
    {
        perror("ioctl TIMER_RELEASE error ");
        return(FALSE);
    }

    return(TRUE);
}

// returns    :
//              TRUE  -> setting OK
//              FALSE -> setting FAIL
// parameters :
//              id_no -> 설정된 타이머Index를 저장할 변수
//              count -> 설정할 타이머 값
// purpose    : 타이머 구동을 위해 값을 설정한다.
//              사용후에는 반드시 타이머를 release 해주어야 한다.
unsigned char set_timer(unsigned char *id_no, int count)
{
    signed char ret;
    int cnt;
    struct timer_manager timer_list;

    for(cnt=MAXAPPTIMER+1; cnt<MAXTIMER; cnt++)
    {
        if (!Ttimer.tm_id[cnt])
        {
            Ttimer.tm_id[cnt]  = 1;
            Ttimer.tm_cnt[cnt] = count;
            Ttimer.process_cnt++;
            
            memset(&timer_list, 0, sizeof(timer_list));
            timer_list.index = cnt;
            timer_list.expires = count;
            if ((ret = ioctl(timerfd, TIMER_SET_TIME, &timer_list)) < 0)
            {
                perror("ioctl TIMER_SET_TIME error ");
                return(FALSE);
            }
            
            *id_no = cnt;
            return(TRUE);
        }
    }
    printf("\nset_timer fail [%d]:", cnt);    
    return(FALSE);
}

// returns    :
//              MONOK  -> setting OK
//              false -> setting FAIL
// parameters :
//              id_no -> 재설정할 타이머 ID
//              count -> 재설정할 타이머 값
// purpose    : 이미 설정되어 구동중인 타이머를 다시 설정하고 싶을때.
unsigned char reset_timer(unsigned char id_no, int count)
{
    signed char ret;
    struct timer_manager timer_list;
    
    if (id_no >= MAXTIMER) return(FALSE);
    Ttimer.tm_id[id_no]  = 1;
    Ttimer.tm_cnt[id_no] = count;
    
    memset(&timer_list, 0, sizeof(timer_list));
    timer_list.index = id_no;
    timer_list.expires = count;
    if ((ret = ioctl(timerfd, TIMER_RESET_TIME, &timer_list)) < 0)
    {
        perror("ioctl TIMER_RESET_TIME error ");
        return(FALSE);
    }
    return(TRUE);
}

// returns    :
//              true  -> TIMER processd
//              false -> TIMER stop
// parameters :
//              id_no -> 구동여부를 조사할 타이머 ID
// purpose    : 타이머 구동여부를 조사한다.
unsigned char chk_timer(unsigned char id_no)
{
    int id;
    
    if (id_no > MAXTIMER || !Ttimer.tm_id[id_no]) return(FALSE);
        
    id = id_no;
    if (ioctl(timerfd, TIMER_CHECK, &id) < 0)
    {
        perror("ioctl TIMER_CHECK error ");
        return(FALSE);
    }
    if (id) return(FALSE);
    else    return(TRUE);

}

// returns    :
//              true  -> relaese OK
//              false -> relaese FAIL
// parameters :
//              id_no -> 구동을 중시시킬 타이머 ID
// purpose    : 타이머의 구동을 중지시킨다.
unsigned char release_timer(unsigned char id_no)
{
    int id;
    
    if (id_no >= MAXTIMER) return(FALSE);
    Ttimer.tm_id[id_no]  = 0;
    Ttimer.tm_cnt[id_no] = 0;
    Ttimer.process_cnt--;
    
    id = id_no;
    if (ioctl(timerfd, TIMER_RELEASE, &id) < 0)
    {
        perror("ioctl TIMER_RELEASE error ");
        return(FALSE);
    }
    return(TRUE);
}

void Delay(word n)
{
    byte Id=0;
    
    set_timer(&Id, n);
    while(1)
    {
        //if (Com_Ready(PSTN_PORT))
        //{
        //    Com_Getc(PSTN_PORT);
        //}
        
        if (chk_timer(Id)==FALSE)
        {
            release_timer(Id);
            break;
        }
    }
}



///////////////////////////////////////////////////////////
// serial
///////////////////////////////////////////////////////////
#if 1
int sputs(int cport, char *msg)
{
    int sfd = 0;
    int ret;
    
    switch(cport)
    {
        case ICC_PORT:
            sfd = uart0_fd;
            break;
        case RF_PORT:
            sfd = uart1_fd;
            break;
        case BARCODE_PORT:
            sfd = uart2_fd;
            break;
        case PSTN_PORT:
            sfd = uart3_fd;
            break;
    }
    
    ret = write(sfd, msg, strlen(msg));
    return(ret);
}

int sputch(int cport, char chval)
{
    char msg[5];
    int sfd = 0;
    int ret;

    
    sprintf(msg, "%02X ", chval);
    switch(cport)
    {
        case ICC_PORT:
            sfd = uart0_fd;
            break;
        case RF_PORT:
            sfd = uart1_fd;
            break;
        case BARCODE_PORT:
            sfd = uart2_fd;
            break;
        case PSTN_PORT:
            sfd = uart3_fd;
            break;
    }
    
    ret = write(sfd, msg, strlen(msg));
    return(ret);
}

int Com_Ready(int cport)
{
	int i;
	i=com_ready(cport);
    return(i);
}

int Com_Getc(int cport)
{
    int i;

#ifdef MODEM_DEBUG
    int rch;
    
    rch = combuf_getc(cport);
    if (cport==PSTN_PORT) printf("%c", rch); 
    return(rch);

#else    
	i = combuf_getc(cport);
	return(i);
#endif    
}

int Com_Write(int cport, char *msg, int len)
{
    int i;

#ifdef MODEM_DEBUG
int siz;

for(siz=0; siz<len; siz++)   
    if (cport==PSTN_PORT) printf("%c", msg[siz]);
#endif

    i=DataTxl(cport, msg, len);
	return i;
}
#endif

#if 0
///////////////////////////////////////////////////////////
// msr
///////////////////////////////////////////////////////////
#define TRACK2      0x22
#define TRACK3      0x24
void mcr2_init(void)
{
    track2_size = 0;
    memset(track2_data, 0, sizeof(track2_data));
    msr_init(TRACK2);
}

byte mcr2_chk(byte *dest)
{
    track2_size = msr_chk(track2_data);

    if (track2_size > 1)
    {
        memcpy(dest, track2_data, track2_size);
        return(TRUE);
    }
    else return(FALSE);
}

void mcr3_init(void)
{
    track3_size = 0;
    memset(track3_data, 0, sizeof(track3_data));
    msr_init(TRACK3);
}

byte mcr3_chk(byte *dest)
{
    track3_size = msr_chk(track3_data);

    if (track3_size > 1)
    {
        memcpy(dest, track3_data, track3_size);
        return(TRUE);
    }
    else return(FALSE);
}

void mcr23_init(void)
{
    track2_size = 0;
    memset(track2_data, 0, sizeof(track2_data));
    track3_size = 0;
    memset(track3_data, 0, sizeof(track3_data));
    track23_size = 0;
    memset(track23_data, 0, sizeof(track23_data));
    msr_init(TRACK2|TRACK3);
}

byte mcr23_chk(byte *track2_dest, byte *track3_dest)
{
    byte ret, mret;
    int  ii;
    
    track23_size = msr_chk(track23_data);

    if (track23_size > 1)
    {
        ret = 0;
        for(ii=0; ii<track23_size;)
        {
            mret = track23_data[ii++];
            if (mret == '\t') break;
            track2_data[track2_size++] = mret; 
        }
        
        for(; ii<track23_size;)
        {
            mret = track23_data[ii++];
            track3_data[track3_size++] = mret; 
        }
        
        memcpy(track2_dest, track2_data, track2_size);
        memcpy(track3_dest, track3_data, track3_size);
        if (track2_size) ret = 1;
        if (track3_size) ret += 2;
        return(ret);
    }
    else return(0);
}
#endif
///////////////////////////////////////////////////////////
// RTC
///////////////////////////////////////////////////////////
#if 0
/**
 @brief     RTC의 시간을 읽어와서 시스템시간으로 설정
 */
void rtc2systemtime(void)
{
    struct rtc_time rtm;
    struct tm       stm;
    struct timeval  tv={0,0};


    if (ioctl(rtcfd,RTC_RD_TIME, &rtm) < 0)
    {
        perror("RTC_RD_TIME error");
        return;
    }

    stm.tm_year = rtm.tm_year + 100;
    stm.tm_mon  = rtm.tm_mon-1;
    stm.tm_mday = rtm.tm_mday;
    stm.tm_hour = rtm.tm_hour;
    stm.tm_min  = rtm.tm_min;
    stm.tm_sec  = rtm.tm_sec;

    switch(rtm.tm_wday)
    {
        case 0:     // saturday
            stm.tm_wday = 6;
            break;
        case 1:     // sunday
            stm.tm_wday = 0;
            break;
        case 2:     // monday
            stm.tm_wday = 1;
            break;
        case 3:     // tuesday
            stm.tm_wday = 2;
            break;
        case 4:     // wednesday
            stm.tm_wday = 3;
            break;
        case 5:     // thursday
            stm.tm_wday = 4;
            break;
        case 6:     // friday
            stm.tm_wday = 5;
            break;
    }

    tv.tv_sec = mktime(&stm);

    settimeofday(&tv, NULL);
}

/**
 @brief     시간을 인자로 받아서 RTC를 설정한다.
 @return    success(0), fail(1)
 */
unsigned char set_systemtime2rtcArg(struct rtc_time rtm)
{
    if (!rtm.tm_year)
    {
        printf("Argument Parse Error\n");
        return 1;
    }

    if (ioctl(rtcfd, RTC_SET_TIME, &rtm) < 0)
    {
        perror("RTC_SET_TIME error");
        return 1;
    }

    memset(&rtm, 0, sizeof(rtm));

    if (ioctl(rtcfd, RTC_RD_TIME, &rtm) < 0)
    {
        perror("RTC_RD_TIME error");
        return 1;
    }
    return 0;
}

byte rtc_change(void)
{
    time_t time_now;
    struct tm *tm;
    static int second;
    
    time(&time_now);
    tm = localtime(&time_now);
    if (second != tm->tm_sec) 
    {
        second = tm->tm_sec;
        return(1);
    }
    else return(0);
}
#endif
///////////////////////////////////////////////////////////
// CRC
///////////////////////////////////////////////////////////

const word crc_tbl[256] = {
    0x0000,0xc0c1,0xc181,0x0140,0xc301,0x03c0,0x0280,0xc241,
    0xc601,0x06c0,0x0780,0xc741,0x0500,0xc5c1,0xc481,0x0440,
    0xcc01,0x0cc0,0x0d80,0xcd41,0x0f00,0xcfc1,0xce81,0x0e40,
    0x0a00,0xcac1,0xcb81,0x0b40,0xc901,0x09c0,0x0880,0xc841,
    0xd801,0x18c0,0x1980,0xd941,0x1b00,0xdbc1,0xda81,0x1a40,
    0x1e00,0xdec1,0xdf81,0x1f40,0xdd01,0x1dc0,0x1c80,0xdc41,
    0x1400,0xd4c1,0xd581,0x1540,0xd701,0x17c0,0x1680,0xd641,
    0xd201,0x12c0,0x1380,0xd341,0x1100,0xd1c1,0xd081,0x1040,
    0xf001,0x30c0,0x3180,0xf141,0x3300,0xf3c1,0xf281,0x3240,
    0x3600,0xf6c1,0xf781,0x3740,0xf501,0x35c0,0x3480,0xf441,
    0x3c00,0xfcc1,0xfd81,0x3d40,0xff01,0x3fc0,0x3e80,0xfe41,
    0xfa01,0x3ac0,0x3b80,0xfb41,0x3900,0xf9c1,0xf881,0x3840,
    0x2800,0xe8c1,0xe981,0x2940,0xeb01,0x2bc0,0x2a80,0xea41,
    0xee01,0x2ec0,0x2f80,0xef41,0x2d00,0xedc1,0xec81,0x2c40,
    0xe401,0x24c0,0x2580,0xe541,0x2700,0xe7c1,0xe681,0x2640,
    0x2200,0xe2c1,0xe381,0x2340,0xe101,0x21c0,0x2080,0xe041,
    0xa001,0x60c0,0x6180,0xa141,0x6300,0xa3c1,0xa281,0x6240,
    0x6600,0xa6c1,0xa781,0x6740,0xa501,0x65c0,0x6480,0xa441,
    0x6c00,0xacc1,0xad81,0x6d40,0xaf01,0x6fc0,0x6e80,0xae41,
    0xaa01,0x6ac0,0x6b80,0xab41,0x6900,0xa9c1,0xa881,0x6840,
    0x7800,0xb8c1,0xb981,0x7940,0xbb01,0x7bc0,0x7a80,0xba41,
    0xbe01,0x7ec0,0x7f80,0xbf41,0x7d00,0xbdc1,0xbc81,0x7c40,
    0xb401,0x74c0,0x7580,0xb541,0x7700,0xb7c1,0xb681,0x7640,
    0x7200,0xb2c1,0xb381,0x7340,0xb101,0x71c0,0x7080,0xb041,
    0x5000,0x90c1,0x9181,0x5140,0x9301,0x53c0,0x5280,0x9241,
    0x9601,0x56c0,0x5780,0x9741,0x5500,0x95c1,0x9481,0x5440,
    0x9c01,0x5cc0,0x5d80,0x9d41,0x5f00,0x9fc1,0x9e81,0x5e40,
    0x5a00,0x9ac1,0x9b81,0x5b40,0x9901,0x59c0,0x5880,0x9841,
    0x8801,0x48c0,0x4980,0x8941,0x4b00,0x8bc1,0x8a81,0x4a40,
    0x4e00,0x8ec1,0x8f81,0x4f40,0x8d01,0x4dc0,0x4c80,0x8c41,
    0x4400,0x84c1,0x8581,0x4540,0x8701,0x47c0,0x4680,0x8641,
    0x8201,0x42c0,0x4380,0x8341,0x4100,0x81c1,0x8081,0x4040,
};


/******************************************************************************
 * CRC 값 생성
 *-----------------------------------------------------------------------------
 *  Input : uiSrc       : Data
 *-----------------------------------------------------------------------------
 *  Output: puiCRC      : CRC 값
 *-----------------------------------------------------------------------------
 *  return:  NONE
 ******************************************************************************/
void crcupdate_proc(unsigned short uiSrc, unsigned short *puiCRC) 
{ 
    unsigned short uiIndex=0; 

    uiSrc = uiSrc & 0x00FF; 
    uiIndex = (*puiCRC >> 8) ^ uiSrc; 
    *puiCRC = (*puiCRC << 8) ^ crc_tbl[uiIndex]; 
} 

/******************************************************************************
 * CRC 값 생성
 *-----------------------------------------------------------------------------
 *  Input : szBuff      : Data
 *          inPos       : ETX까지의 길이
 *-----------------------------------------------------------------------------
 *  Output: puiCRC      : CRC 값
 *-----------------------------------------------------------------------------
 *  return:  CRC Value
 ******************************************************************************/
int crc_sum_FDI(char *szBuff, int inPos) 
{ 
    unsigned short usCRC; 
    int i; 

    /* append CRC-16 2 bytes */ 
    usCRC=0; 
    
    for (i = 0; i < inPos; i++)     /* inPos : ETX까지 길이 */ 
    {
        crcupdate_proc(szBuff[i], &usCRC); 
    }
    return(usCRC); 
} 


///////////////////////////////////////////////////////////
// printer
///////////////////////////////////////////////////////////
void api_paper_set(void)
{
#if 0
    //byte ret;
	char ret;
	int iStep=0;

    
    while(1)
    {
    	switch(iStep)
    	{
    		case 0:
		        clrlcd();
		        lcdchs(" <전표용지조정> ", LINE1);
		        lcdchs("1.위  로 [   키]", LINE3);
		        lcdchs("2.AUTOCUT       ", LINE4);
				lcddir((byte*)updir, 32+11, L_NORMAL);

				iStep++;

			case 1:
		        ret = GetContinueKey(1);
		        if(ret==CANCEL || ret==BSPACE)
		        {
		        	beep(7,1);
		            JTNET_STEP = 0;
		            return;
		        }
		        else
		        if(ret=='1' || ret==UPKEY)
		        {
		        	prt_step_move();					
		        }
		        else
		        if(ret=='2')
		        {
		        	beep(7,1);
		            prtqueue_insert("\x1B\x4C");    // PAGE MODE
		            prtqueue_insert("\x0C");
		            auto_cutting(AC_FULL);
		            prt_step_move();
		            JTNET_STEP = 0;
		            return;
		        }
				else
				{
					beep(7,1);
				}
    	}
    }
#endif
}

// type=0 : no message, type=1 : send message
byte print_start(byte mode, int type)
{
	byte ret, status=0;
	t_qdata qdata;
    
#if 0
//----------------------------------------------------------------------------------------------
// 이더넷 DHCP 동작과 프린터 인쇄 충돌 문제를 해결하기 위한 추가. by khd, 2012.11.13
//----------------------------------------------------------------------------------------------
	if(system("pidof udhcpc_debug") == 0)
	{
		if(system("kill -STOP `pidof udhcpc_debug`") == 0)
			printf(">>>>>>>>>>>>>>>>> udhcpc_debug stopped!!!\n");
		else
		{
			printf(">>>>>>>>>>>>>>>>> udhcpc_debug stopping error!!!\n");
			system("kill -9 `pidof udhcpc_debug`"); // added by khd
			printf(">>>>>>>>>>>>>>>> killed udhcpc_debug in print_start()\n");
		}
	}
//----------------------------------------------------------------------------------------------
#endif
if (mode == SW_ON)
    {
        while(1)
        {
			usleep(1);
            if (overhit_check() == 0x34)
            {
            	if(type==1)
            	{
					qdata.command_type = qPRINTER;
					qdata.command_code = qPRINT_START;
					qdata.qprint_resp.response_code = 4;
					qdata.qprint_resp.t_field.progress = 0;
					send_msgq(&qdata, 1);
            	}
                aDebugMsg("프린터 장치 과열");
                aDebugMsg("     잠시후     ");
                aDebugMsg("다시 시도하세요 ");
				log_printf(0, "인쇄:프린터 장치과열!");
                return(0);
            }
            else break;
        }
        
        while(1)
        {
			usleep(1);
            if (ac_position_check() == 0x37)
            {
            	if(type==1)
            	{
					qdata.command_type = qPRINTER;
					qdata.command_code = qPRINT_START;
					qdata.qprint_resp.response_code = 3;
					qdata.qprint_resp.t_field.progress = 0;
					send_msgq(&qdata, 1);
            	}
                aDebugMsg(" *용지커터오류* ");
                aDebugMsg("커터버튼을 몇번 ");
                aDebugMsg("  누른 후 다시  ");
                aDebugMsg("   시도하세요   ");
				log_printf(0, "인쇄:프린터 커터오류!");
                return(0);
            }
            else break;
        }

        while(1)
        {
			usleep(1);
            if (cover_switch_check() == 0x38)
            {
            	if(type==1)
            	{
					qdata.command_type = qPRINTER;
					qdata.command_code = qPRINT_START;
					qdata.qprint_resp.response_code = 1;
					qdata.qprint_resp.t_field.progress = 0;
					send_msgq(&qdata, 1);
            	}
                aDebugMsg("*프린터커버열림*");
                aDebugMsg(" 커버를 닫은 후 ");
                aDebugMsg("다시 시도하세요.");
				log_printf(0, "인쇄:프린터 커버열림!");
                return(0);
            }
            else break;
        }

        while(1)
        {
			usleep(1);
            if (paper_chk() == 0x32)
            {   
            	if(type==1)
            	{
					qdata.command_type = qPRINTER;
					qdata.command_code = qPRINT_START;
					qdata.qprint_resp.response_code = 2;
					qdata.qprint_resp.t_field.progress = 0;
					send_msgq(&qdata, 1);
            	}
                aDebugMsg("  용지인식오류! ");
                aDebugMsg("   전표용지를   ");
                aDebugMsg("다시 끼워주세요 ");
				log_printf(0, "인쇄:프린터 용지인식오류!");
                return(0);
            }
            else break;
        }
    }

    return 1;
}

void line_feed(word cnt)
{
    int i;
    for(i=0; i<cnt; i++)
    {
        prtqueue_insert("\n");
    }
}

void PrintInt_Check(void)
{
}


int print_end(void)
{
    line_feed(2);
#if 1
    if (Region01.Terminfo.PrtInfo.cutting_on=='2')
        auto_cutting(AC_PARTIAL);
    else
#endif
        auto_cutting(AC_FULL);
    
    return(1);
}

int pos_print_end(void)
{  
//    prtqueue_insert("\x0C");

    if (Region01.Terminfo.PrtInfo.cutting_on=='2')
        auto_cutting(AC_PARTIAL);
    else
        auto_cutting(AC_FULL);
	
    prtqueue_insert("\x1B\x53");    // STANDARD MODE
//    prtqueue_insert("\x1B\x4C");    // PAGE MODE
    prtqueue_insert("\n");

    return(1);
}

int  print_status(void)
{
    int ret;
    ret = paper_chk();
    return(ret);
}

int print_cover_status(void)
{
    int ret;
    ret = cover_switch_check();
    return(ret);
}

const byte S_Box[8][64] =
{
    {
      14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
       0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
       4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
      15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
    },
    {
      15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
       3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
       0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
      13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
    },
    {
      10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
      13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
      13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
       1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
    },
    {
       7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
      13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
      10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
       3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
    },
    {
       2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
      14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
       4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
      11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
    },
    {
      12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
      10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
       9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
       4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
    },
    {
       4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
      13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
       1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
       6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
    },
    { 13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
       1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
       7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
       2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
    }
};

const byte LS[16] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };


byte Li[4], Ri[4], Ri_1[4], Ki[7];
byte Ki2[16][8];

void function_des(byte cr_decr, byte *Data, byte *Key, byte *Res)
{
    byte buff[16]={0};
    byte i, k, Datan;
    byte s1, s2;

    Li[0] = Li[1] = Li[2] = Li[3] = 0;
    Ri_1[0] = Ri_1[1] = Ri_1[2] = Ri_1[3] = 0;

    for( i = 0; i < 8; i++) buff[i] = Key[i];

    for(i = 0; i < 8; i++){
        k = (1 << i);
        Datan = Data[i];
        if(Datan & 0x40) Li[0] += k;
        if(Datan & 0x10) Li[1] += k;
        if(Datan & 0x04) Li[2] += k;
        if(Datan & 0x01) Li[3] += k;
        if(Datan & 0x80) Ri_1[0] += k;
        if(Datan & 0x20) Ri_1[1] += k;
        if(Datan & 0x08) Ri_1[2] += k;
        if(Datan & 0x02) Ri_1[3] += k;
        Data[i] = Datan;
    }

    Ki[0] = buff[7] & 0x80;
    Ki[0] += (buff[0] >> 7);
    if(buff[6] & (0x80)) Ki[0] += 0x40;
    if(buff[5] & (0x80)) Ki[0] += 0x20;
    if(buff[4] & (0x80)) Ki[0] += 0x10;
    if(buff[3] & (0x80)) Ki[0] += 0x08;
    if(buff[2] & (0x80)) Ki[0] += 0x04;
    if(buff[1] & (0x80)) Ki[0] += 0x02;

    Ki[1] = buff[6] & 0x40;
    if(buff[7] & (0x40)) Ki[1] += 0x80;
    if(buff[5] & (0x40)) Ki[1] += 0x20;
    if(buff[4] & (0x40)) Ki[1] += 0x10;
    if(buff[3] & (0x40)) Ki[1] += 0x08;
    if(buff[2] & (0x40)) Ki[1] += 0x04;
    if(buff[1] & (0x40)) Ki[1] += 0x02;
    if(buff[0] & (0x40)) Ki[1] += 0x01;

    Ki[2] = buff[5] & 0x20;
    if(buff[7] & (0x20)) Ki[2] += 0x80;
    if(buff[6] & (0x20)) Ki[2] += 0x40;
    if(buff[4] & (0x20)) Ki[2] += 0x10;
    if(buff[3] & (0x20)) Ki[2] += 0x08;
    if(buff[2] & (0x20)) Ki[2] += 0x04;
    if(buff[1] & (0x20)) Ki[2] += 0x02;
    if(buff[0] & (0x20)) Ki[2] += 0x01;

    Ki[3] = (buff[4] & 0x10);
    Ki[3] += (buff[5] & 0x02);
    if(buff[7] & (0x10)) Ki[3] += 0x80;
    if(buff[6] & (0x10)) Ki[3] += 0x40;
    if(buff[5] & (0x10)) Ki[3] += 0x20;
    if(buff[7] & (0x02)) Ki[3] += 0x08;
    if(buff[6] & (0x02)) Ki[3] += 0x04;
    if(buff[4] & (0x02)) Ki[3] += 0x01;

    Ki[4] = buff[6] & 0x04;
    if(buff[3] & (0x02)) Ki[4] += 0x80;
    if(buff[2] & (0x02)) Ki[4] += 0x40;
    if(buff[1] & (0x02)) Ki[4] += 0x20;
    if(buff[0] & (0x02)) Ki[4] += 0x10;
    if(buff[7] & (0x04)) Ki[4] += 0x08;
    if(buff[5] & (0x04)) Ki[4] += 0x02;
    if(buff[4] & (0x04)) Ki[4] += 0x01;

    Ki[5] = buff[7] & 0x08;
    if(buff[3] & (0x04)) Ki[5] += 0x80;
    if(buff[2] & (0x04)) Ki[5] += 0x40;
    if(buff[1] & (0x04)) Ki[5] += 0x20;
    if(buff[0] & (0x04)) Ki[5] += 0x10;
    if(buff[6] & (0x08)) Ki[5] += 0x04;
    if(buff[5] & (0x08)) Ki[5] += 0x02;
    if(buff[4] & (0x08)) Ki[5] += 0x01;

    Ki[6] = (buff[3] & 0x08) << 4;
    if(buff[2] & (0x08)) Ki[6] += 0x40;
    if(buff[1] & (0x08)) Ki[6] += 0x20;
    if(buff[0] & (0x08)) Ki[6] += 0x10;
    if(buff[3] & (0x10)) Ki[6] += 0x08;
    if(buff[2] & (0x10)) Ki[6] += 0x04;
    if(buff[1] & (0x10)) Ki[6] += 0x02;
    if(buff[0] & (0x10)) Ki[6] += 0x01;

    for(i = 0; i < 16; i++){
        s1 = ((Ki[0] & 0x80) >> 3);
        s2 = ((Ki[3] & 0x08) >> 3);
        Ki[0] <<= 1;
        Ki[0] += Ki[1] >> 7;
        Ki[1] <<= 1;
        Ki[1] += Ki[2] >> 7;
        Ki[2] <<= 1;
        Ki[2] += Ki[3] >> 7;
        Ki[3] <<= 1;
        Ki[3] += Ki[4] >> 7;
        Ki[4] <<= 1;
        Ki[4] += Ki[5] >> 7;
        Ki[5] <<= 1;
        Ki[5] += Ki[6] >> 7;
        Ki[6] <<= 1;
        Ki[3] &= 0xEF;
        Ki[3] |= s1;
        Ki[6] += s2;

        if(LS[i] == 2){
            s1 = ((Ki[0] & 0x80) >> 3);
            s2 = ((Ki[3] & 0x08) >> 3);
            Ki[0] <<= 1;
            Ki[0] += (Ki[1]) >> 7;
            Ki[1] <<= 1;
            Ki[1] += (Ki[2]) >> 7;
            Ki[2] <<= 1;
            Ki[2] += (Ki[3]) >> 7;
            Ki[3] <<= 1;
            Ki[3] += (Ki[4]) >> 7;
            Ki[4] <<= 1;
            Ki[4] += (Ki[5]) >> 7;
            Ki[5] <<= 1;
            Ki[5] += (Ki[6]) >> 7;
            Ki[6] <<= 1;
            Ki[3] &= 0xEF;
            Ki[3] |= s1;
            Ki[6] += s2;
        }

        Ki2[i][0] = (Ki[1] & 0x04) << 3;
        if(Ki[2] & 0x80) Ki2[i][0] += 0x10;
        if(Ki[1] & 0x20) Ki2[i][0] += 0x08;
        if(Ki[2] & 0x01) Ki2[i][0] += 0x04;
        if(Ki[0] & 0x80) Ki2[i][0] += 0x02;
        if(Ki[0] & 0x08) Ki2[i][0] += 0x01;

        Ki2[i][1] = (Ki[0] & 0x20);
        Ki2[i][1] += (Ki[3] & 0x10);
        Ki2[i][1] += (Ki[0] & 0x04);
        if(Ki[1] & 0x02) Ki2[i][1] += 0x08;
        if(Ki[2] & 0x08) Ki2[i][1] += 0x02;
        if(Ki[1] & 0x40) Ki2[i][1] += 0x01;

        Ki2[i][2] = Ki[0] & 0x01;
        if(Ki[2] & 0x02) Ki2[i][2] += 0x20;
        if(Ki[2] & 0x20) Ki2[i][2] += 0x10;
        if(Ki[1] & 0x10) Ki2[i][2] += 0x08;
        if(Ki[0] & 0x10) Ki2[i][2] += 0x04;
        if(Ki[3] & 0x40) Ki2[i][2] += 0x02;

        Ki2[i][3] = (Ki[1] & 0x01) << 5;
        if(Ki[0] & 0x02) Ki2[i][3] += 0x10;
        if(Ki[3] & 0x20) Ki2[i][3] += 0x08;
        if(Ki[2] & 0x10) Ki2[i][3] += 0x04;
        if(Ki[1] & 0x08) Ki2[i][3] += 0x02;
        if(Ki[0] & 0x40) Ki2[i][3] += 0x01;

        Ki2[i][4] = (Ki[6] & 0x10);
        Ki2[i][4] += (Ki[5] & 0x02);
        if(Ki[5] & 0x80) Ki2[i][4] += 0x20;
        if(Ki[3] & 0x02) Ki2[i][4] += 0x08;
        if(Ki[4] & 0x08) Ki2[i][4] += 0x04;
        if(Ki[6] & 0x02) Ki2[i][4] += 0x01;

        Ki2[i][5] = Ki[5] & 0x01;
        if(Ki[3] & 0x04) Ki2[i][5] += 0x20;
        if(Ki[4] & 0x01) Ki2[i][5] += 0x10;
        if(Ki[6] & 0x20) Ki2[i][5] += 0x08;
        if(Ki[5] & 0x08) Ki2[i][5] += 0x04;
        if(Ki[4] & 0x80) Ki2[i][5] += 0x02;

        Ki2[i][6] = (Ki[5] & 0x10) << 1;
        if(Ki[6] & 0x80) Ki2[i][6] += 0x10;
        if(Ki[4] & 0x02) Ki2[i][6] += 0x08;
        if(Ki[6] & 0x01) Ki2[i][6] += 0x04;
        if(Ki[4] & 0x40) Ki2[i][6] += 0x02;
        if(Ki[6] & 0x08) Ki2[i][6] += 0x01;

        Ki2[i][7] = Ki[3] & 0x01;
        if(Ki[5] & 0x04) Ki2[i][7] += 0x20;
        if(Ki[5] & 0x40) Ki2[i][7] += 0x10;
        if(Ki[6] & 0x40) Ki2[i][7] += 0x08;
        if(Ki[4] & 0x10) Ki2[i][7] += 0x04;
        if(Ki[3] & 0x08) Ki2[i][7] += 0x02;
    }

    for(i = 0; i < 16; i++)
    {
        buff[0] = ((Ri_1[3] & 0x01) << 5) | ((Ri_1[0]) >> 3);
        buff[1] = ((Ri_1[0] & 0x1F) << 1) | ((Ri_1[1]) >> 7);
        buff[2] = ((Ri_1[0] & 0x01) << 5) | ((Ri_1[1]) >> 3);
        buff[3] = ((Ri_1[1] & 0x1F) << 1) | ((Ri_1[2]) >> 7);
        buff[4] = ((Ri_1[1] & 0x01) << 5) | ((Ri_1[2]) >> 3);
        buff[5] = ((Ri_1[2] & 0x1F) << 1) | ((Ri_1[3]) >> 7);
        buff[6] = ((Ri_1[2] & 0x01) << 5) | ((Ri_1[3]) >> 3);
        buff[7] = ((Ri_1[3] & 0x1F) << 1) | ((Ri_1[0]) >> 7);

        if(cr_decr == ENCRYPT)
        {
            buff[0] ^= Ki2[i][0];
            s1 = (buff[0] & 0x20) + ((buff[0] & 0x01) << 4);
            s2 = (buff[0] & 0x1E) >> 1;
            buff[0] = S_Box[0][s1 + s2];

            buff[1] ^= Ki2[i][1];
            s1 = (buff[1] & 0x20) + ((buff[1] & 0x01) << 4);
            s2 = (buff[1] & 0x1E) >> 1;
            buff[1] = S_Box[1][s1 + s2];

            buff[2] ^= Ki2[i][2];
            s1 = (buff[2] & 0x20) + ((buff[2] & 0x01) << 4);
            s2 = (buff[2] & 0x1E) >> 1;
            buff[2] = S_Box[2][s1 + s2];

            buff[3] ^= Ki2[i][3];
            s1 = (buff[3] & 0x20) + ((buff[3] & 0x01) << 4);
            s2 = (buff[3] & 0x1E) >> 1;
            buff[3] = S_Box[3][s1 + s2];

            buff[4] ^= Ki2[i][4];
            s1 = (buff[4] & 0x20) + ((buff[4] & 0x01) << 4);
            s2 = (buff[4] & 0x1E) >> 1;
            buff[4] = S_Box[4][s1 + s2];

            buff[5] ^= Ki2[i][5];
            s1 = (buff[5] & 0x20) + ((buff[5] & 0x01) << 4);
            s2 = (buff[5] & 0x1E) >> 1;
            buff[5] = S_Box[5][s1 + s2];

            buff[6] ^= Ki2[i][6];
            s1 = (buff[6] & 0x20) + ((buff[6] & 0x01) << 4);
            s2 = (buff[6] & 0x1E) >> 1;
            buff[6] = S_Box[6][s1 + s2];

            buff[7] ^= Ki2[i][7];
            s1 = (buff[7] & 0x20) + ((buff[7] & 0x01) << 4);
            s2 = (buff[7] & 0x1E) >> 1;
            buff[7] = S_Box[7][s1 + s2];
        }
        else
        {
            buff[0] ^= Ki2[15-i][0];
            s1 = (buff[0] & 0x20) + ((buff[0] & 0x01) << 4);
            s2 = (buff[0] & 0x1E) >> 1;
            buff[0] = S_Box[0][s1 + s2];

            buff[1] ^= Ki2[15-i][1];
            s1 = (buff[1] & 0x20) + ((buff[1] & 0x01) << 4);
            s2 = (buff[1] & 0x1E) >> 1;
            buff[1] = S_Box[1][s1 + s2];

            buff[2] ^= Ki2[15-i][2];
            s1 = (buff[2] & 0x20) + ((buff[2] & 0x01) << 4);
            s2 = (buff[2] & 0x1E) >> 1;
            buff[2] = S_Box[2][s1 + s2];

            buff[3] ^= Ki2[15-i][3];
            s1 = (buff[3] & 0x20) + ((buff[3] & 0x01) << 4);
            s2 = (buff[3] & 0x1E) >> 1;
            buff[3] = S_Box[3][s1 + s2];

            buff[4] ^= Ki2[15-i][4];
            s1 = (buff[4] & 0x20) + ((buff[4] & 0x01) << 4);
            s2 = (buff[4] & 0x1E) >> 1;
            buff[4] = S_Box[4][s1 + s2];

            buff[5] ^= Ki2[15-i][5];
            s1 = (buff[5] & 0x20) + ((buff[5] & 0x01) << 4);
            s2 = (buff[5] & 0x1E) >> 1;
            buff[5] = S_Box[5][s1 + s2];

            buff[6] ^= Ki2[15-i][6];
            s1 = (buff[6] & 0x20) + ((buff[6] & 0x01) << 4);
            s2 = (buff[6] & 0x1E) >> 1;
            buff[6] = S_Box[6][s1 + s2];

            buff[7] ^= Ki2[15-i][7];
            s1 = (buff[7] & 0x20) + ((buff[7] & 0x01) << 4);
            s2 = (buff[7] & 0x1E) >> 1;
            buff[7] = S_Box[7][s1 + s2];
        }

        buff[0] = (buff[0] << 4) + buff[1];
        buff[1] = (buff[2] << 4) + buff[3];
        buff[2] = (buff[4] << 4) + buff[5];
        buff[3] = (buff[6] << 4) + buff[7];

        Ri[0] = (buff[3] & 0x08);
        Ri[0] += (buff[2] & 0x18) << 1;
        Ri[0] += buff[1] << 7;
        Ri[0] += buff[2] >> 7;
        if(buff[0] & (0x02)) Ri[0] += 0x40;
        if(buff[1] & (0x10)) Ri[0] += 0x04;
        if(buff[3] & (0x10)) Ri[0] += 0x02;

        Ri[1] = (buff[0] & 0x80);
        Ri[1] += (buff[0] & 0x08);
        Ri[1] += (buff[3] & 0x02);
        if(buff[1] & (0x02)) Ri[1] += 0x40;
        if(buff[2] & (0x02)) Ri[1] += 0x20;
        if(buff[3] & (0x40)) Ri[1] += 0x10;
        if(buff[2] & (0x40)) Ri[1] += 0x04;
        if(buff[1] & (0x40)) Ri[1] += 0x01;

        Ri[2] = buff[1] >> 7;
        if(buff[0] & (0x40)) Ri[2] += 0x80;
        if(buff[0] & (0x01)) Ri[2] += 0x40;
        if(buff[2] & (0x01)) Ri[2] += 0x20;
        if(buff[1] & (0x04)) Ri[2] += 0x10;
        if(buff[3] & (0x01)) Ri[2] += 0x08;
        if(buff[3] & (0x20)) Ri[2] += 0x04;
        if(buff[0] & (0x20)) Ri[2] += 0x02;

        Ri[3] = buff[3] >> 7;
        if(buff[2] & (0x20)) Ri[3] += 0x80;
        if(buff[1] & (0x08)) Ri[3] += 0x40;
        if(buff[3] & (0x04)) Ri[3] += 0x20;
        if(buff[0] & (0x04)) Ri[3] += 0x10;
        if(buff[2] & (0x04)) Ri[3] += 0x08;
        if(buff[1] & (0x20)) Ri[3] += 0x04;
        if(buff[0] & (0x10)) Ri[3] += 0x02;

        Ri[0] ^= Li[0];         Ri[1] ^= Li[1];
        Ri[2] ^= Li[2];         Ri[3] ^= Li[3];

        Li[0] = Ri_1[0];        Li[1] = Ri_1[1];
        Li[2] = Ri_1[2];        Li[3] = Ri_1[3];

        Ri_1[0] = Ri[0];        Ri_1[1] = Ri[1];
        Ri_1[2] = Ri[2];        Ri_1[3] = Ri[3];
    }

    for(i = 0; i < 8; i++)
    {
        k = (1 << i);
        Datan = 0;
        if(Li[0] & k) Datan += 0x80;
        if(Li[1] & k) Datan += 0x20;
        if(Li[2] & k) Datan += 0x08;
        if(Li[3] & k) Datan += 0x02;
        if(Ri[0] & k) Datan += 0x40;
        if(Ri[1] & k) Datan += 0x10;
        if(Ri[2] & k) Datan += 0x04;
        if(Ri[3] & k) Datan += 0x01;
        Res[i] = Datan;
    }
}

u16 calcrcX(u8 *ptr, int len)
{
    u16 crc, i;
    u16 j;

    crc = 0;
    for( j=0; j<len; j++ )
    {
        crc = crc ^ (int)ptr[j] << 8;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 0x8000)
                crc = crc << 1 ^ 0x1021;
            else
                crc = crc << 1;
        }
    }
    return(crc & 0xFFFF);
}

///////////////////////////////////////////////////////////
// modem
///////////////////////////////////////////////////////////
#if 0
byte api_mchkbuf[32];

void MODEM_Delay(word n)
{
    byte Id=0;
    
    set_timer(&Id, n);
    while(1)
    {
        Kbhit();
        if (Com_Ready(PSTN_PORT))
        {
            Com_Getc(PSTN_PORT);
        }
        
        if (chk_timer(Id)==FALSE)
        {
            release_timer(Id);
            break;
        }
    }
}

byte api_send_wait(byte *tx, byte *rx1, byte *rx2, word second)
{
    byte Id = 0;
    word len, len1, len2, i;

    while(Com_Ready(PSTN_PORT))
    {
        Com_Getc(PSTN_PORT);
    }

    if (tx[0])
    {
        len = Strlen(tx);
        for(i=0; i<len; i++)
        {
            if (tx[i] == '.') tx[i] = ',';
        }
        Com_Write(PSTN_PORT, tx, strlen(tx));
    }

    if (rx1[0] || rx2[0])
    {
        len1 = strlen(rx1);
        len2 = strlen(rx2);
        Memset(api_mchkbuf, 0x00, sizeof(api_mchkbuf));
        set_timer(&Id, second*100);
        while(1)
        {
            Kbhit();
            if (chk_timer(Id)==FALSE)
            {
                release_timer(Id);
                return(MD_TIMEOUT);
            }

            if (Com_Ready(PSTN_PORT))
            {
                memcpy(api_mchkbuf, api_mchkbuf+1, sizeof(api_mchkbuf)-1);
                api_mchkbuf[sizeof(api_mchkbuf)-1] = Com_Getc(PSTN_PORT);
                if (len1)
                {
                    if (!memcmp(&api_mchkbuf[sizeof(api_mchkbuf)-len1], rx1, len1))
                    {
                        release_timer(Id);
                        return(MD_OK);
                    }
                }
                if (len2)
                {
                    if (!memcmp(&api_mchkbuf[sizeof(api_mchkbuf)-len2], rx2, len2))
                    {
                        release_timer(Id);
                        return(MD_OK);
                    }
                }
            }
        }
    }
    else return(MD_OK);
}

void api_md_disconnect(void)
{
    api_send_wait((byte *)"+++", (byte *)"OK", (byte *)"NO CARRIER", 1);
    MODEM_Delay(20);
    api_send_wait((byte *)"ATH0\r", (byte *)"OK", (byte *)"NO CARRIER", 1);
}

void api_modem_initset(void)
{
    byte spd[5]={0};
    byte biostemp[128]={0};

	clrlcd();
    lcdchs("  시스템  관리  ", RLINE1);
    lcdchs("  PSTN 모듈을   ", LINE3);
	lcdchs("  설정중 입니다 ", LINE4);
    
    sio_set(PSTN_PORT, Region01.Comm.Pstn.pstn_spd);
    if (Region01.Comm.Pstn.pstn_spd==B2400)
    {
        api_send_wait((byte *)"ATZ\r", (byte *)"OK", (byte *)"\0", 2);
        MODEM_Delay(20);

        api_send_wait((byte *)"AT+GCI=61\r", (byte *)"OK", (byte *)"\0", 1);
        MODEM_Delay(20);

        api_send_wait((byte *)"AT\\N\\V2&D0&G4%C0\\Q0\r", (byte *)"OK", (byte *)"\0", 1);
        MODEM_Delay(20);

        api_send_wait((byte *)"AT+IFC=0,0\r", (byte *)"OK", (byte *)"\0", 1);
        MODEM_Delay(20);

        api_send_wait((byte *)"AT+MS=V22B\r", (byte *)"OK", (byte *)"\0", 1);
        MODEM_Delay(20);

        
    }
    else
    {
        api_send_wait((byte *)"ATZ\r", (byte *)"OK", (byte *)"\0", 2);
        MODEM_Delay(20);
    
        //sprintf(biostemp, "ath0e0+gci=61\r");
        sprintf(biostemp, "ath0e0+gci=61\\T2\r"); // 20120405_PSTN_TJH
        api_send_wait(biostemp, (byte *)"OK", (byte *)"\0", 1);
        MODEM_Delay(20);
    
        sprintf(biostemp, "at:u52,0000\r");
        api_send_wait(biostemp, (byte *)"OK", (byte *)"\0", 1);
        MODEM_Delay(20);
    
        sprintf(biostemp, "at:u67,0006\r");
        api_send_wait(biostemp, (byte *)"OK", (byte *)"\0", 1);
        MODEM_Delay(20);
    
        sprintf(biostemp, "at:u70,2700\r");
        api_send_wait(biostemp, (byte *)"OK", (byte *)"\0", 1);
        MODEM_Delay(20);
    
        sprintf(biostemp, "at:u7a,0001\r");
        api_send_wait(biostemp, (byte *)"OK", (byte *)"\0", 1);
        MODEM_Delay(20);
    
        sprintf(biostemp, "at:u46,0790\r");
        api_send_wait(biostemp, (byte *)"OK", (byte *)"\0", 1);
        MODEM_Delay(20);
    
        sprintf(biostemp, "at:u84,0004\r");
        api_send_wait(biostemp, (byte *)"OK", (byte *)"\0", 1);
        MODEM_Delay(20);
    
        sprintf(biostemp, "at:u35,1000\r");
        api_send_wait(biostemp, (byte *)"OK", (byte *)"\0", 1);
        MODEM_Delay(20);
    
        sprintf(biostemp, "at&g3&h7\\n0\\v2h0%%v1\r");
        api_send_wait(biostemp, (byte *)"OK", (byte *)"\0", 1);
        MODEM_Delay(20);
    }

    if (Region01.Comm.Pstn.pabx_on=='1')
        sprintf(biostemp, "atx3s8=2s6=1\r");
    else
    {
        if (Region01.Comm.Pstn.dialtone_on == '1')
            sprintf(biostemp, "atx4s8=2s6=1\r");
        else
            sprintf(biostemp, "atx3s8=2s6=1\r");
    }
    api_send_wait(biostemp, (byte *)"OK", (byte *)"\0", 1);
    MODEM_Delay(20);

    if (Region01.Comm.Pstn.dial_spd=='3') Memcpy(spd, "0046", 4);
    else
    if (Region01.Comm.Pstn.dial_spd=='2') Memcpy(spd, "0055", 4);
    else                                  Memcpy(spd, "0064", 4);

    sprintf(biostemp, "at:u47,%s,%s\r", spd, spd);
    api_send_wait(biostemp, (byte *)"OK", (byte *)"\0", 1);
    MODEM_Delay(20);
}

void api_dial_speed(byte speed)
{
    byte spd[5]={0};
    byte biostemp[128]={0};

    if (speed=='3') Memcpy(spd, "0046", 4);
    else
    if (speed=='2') Memcpy(spd, "0055", 4);
    else            Memcpy(spd, "0064", 4);

    // Dial Speed ///////////////////////////////////////////
    sprintf(biostemp, "at:u47,%s, %s\r", spd, spd); // Dial On time 
    api_send_wait(biostemp, (byte *)"OK", (byte *)"\0", 2);
    MODEM_Delay(20);
}

void api_modem_pbx(byte pabx)
{
    byte biostemp[128]={0};

    if (pabx==SW_ON)
        sprintf(biostemp, "atx3s8=2s6=1\r");
    else
    {
        if (Region01.Comm.Pstn.dialtone_on == '1')
            sprintf(biostemp, "atx4s8=2s6=1\r");
        else
            sprintf(biostemp, "atx3s8=2s6=1\r");
    }
    api_send_wait(biostemp, (byte *)"OK", (byte *)"\0", 1);
    MODEM_Delay(20);
}

void api_modem_spd(int speed)
{
    clrlcd();
    lcdchs("      전화      ", 0,15,L_INVERSE);
    lcdchs("   모뎀설정중   ", 32,  47, L_NORMAL);
    lcdchs("잠시만기다리세요", 48, 63, L_NORMAL);
    api_modem_initset();
}

void md_conn_check(void)
{
    if (Com_Ready(PSTN_PORT))
    {
        memcpy(api_mchkbuf, api_mchkbuf+1, sizeof(api_mchkbuf)-1);
        api_mchkbuf[sizeof(api_mchkbuf)-1] = Com_Getc(PSTN_PORT);
        if (!memcmp(&api_mchkbuf[sizeof(api_mchkbuf)-7],  "CONNECT", 7))
            mdflag = MD_CONNECT;
        else
        if (!memcmp(&api_mchkbuf[sizeof(api_mchkbuf)-4],  "BUSY", 4))
            mdflag = MD_BUSY;
        else
        if (!memcmp(&api_mchkbuf[sizeof(api_mchkbuf)-11],  "LINE IN USE", 11))
            mdflag = MD_BUSY;
        else
        if (!memcmp(&api_mchkbuf[sizeof(api_mchkbuf)-10], "NO CARRIER", 10))
            mdflag = MD_NOCARRIER;
        else
        if (!memcmp(&api_mchkbuf[sizeof(api_mchkbuf)-11], "NO DIALTONE", 11))
            mdflag = MD_NODIALTONE;

///////////////////////////////////////////////////////////////////////////////////////////////
//        if (mdflag) 
//        {
//            while(1)
//            {  // 접속된지 오래되었다면 이미 라인이 단절되었을 수 있기 때문에 한번더 확인한다.
//                if (Com_Ready(PSTN_PORT))
//                {
//                    memcpy(api_mchkbuf, api_mchkbuf+1, sizeof(api_mchkbuf)-1);
//                    api_mchkbuf[sizeof(api_mchkbuf)-1] = Com_Getc(PSTN_PORT);
//                    if (!memcmp(&api_mchkbuf[sizeof(api_mchkbuf)-4],  "BUSY", 4))
//                        mdflag = MD_BUSY;
//                    else
//                    if (!memcmp(&api_mchkbuf[sizeof(api_mchkbuf)-10], "NO CARRIER", 10))
//                        mdflag = MD_NOCARRIER;
//                    else
//                    if (!memcmp(&api_mchkbuf[sizeof(api_mchkbuf)-11], "NO DIALTONE", 11))
//                        mdflag = MD_NODIALTONE;
//                }
//                else break;
//            }
//            //if (mdflag != MD_CONNECT) MODEM_Delay(20);          
//        }
///////////////////////////////////////////////////////////////////////////////////////////////
    }
}

void md_start(void)
{
    byte i;
    byte api_biostemp[128]={0};

    memset(api_biostemp, 0, sizeof(api_biostemp));
    memcpy(api_biostemp, "ATDT", 4);
    memcpy(&api_biostemp[4], telbuf, Strlen(telbuf));
    api_biostemp[Strlen(api_biostemp)] = '\r';
    for (i = 0; i < Strlen(api_biostemp); i++)
    {
        if (api_biostemp[i] == '.') api_biostemp[i] = ',';
    }
    Com_Write(PSTN_PORT, api_biostemp, Strlen(api_biostemp));
    
    Memset(api_mchkbuf, 0x00, sizeof(api_mchkbuf));
    mdflag  = 0;
}


/////////////////////////////////////////////////////////////
//// hanconv
/////////////////////////////////////////////////////////////
//
//const word api_i_h_buffer[] = {
//    0x8861,0x8862,0x8865,0x8868,0x8869,0x886A,0x886B,
//    0x8871,0x8873,0x8874,0x8875,0x8876,0x8877,0x8878,
//    0x8879,0x887B,0x887C,0x887D,0x8881,0x8882,0x8885,
//    0x8889,0x8891,0x8893,0x8895,0x8896,0x8897,0x88A1,
//    0x88A2,0x88A5,0x88A9,0x88B5,0x88B7,0x88C1,0x88C5,
//    0x88C9,0x88E1,0x88E2,0x88E5,0x88E8,0x88E9,0x88EB,
//    0x88F1,0x88F3,0x88F5,0x88F6,0x88F7,0x88F8,0x88FB,
//    0x88FC,0x88FD,0x8941,0x8945,0x8949,0x8951,0x8953,
//    0x8955,0x8956,0x8957,0x8961,0x8962,0x8963,0x8965,
//    0x8968,0x8969,0x8971,0x8973,0x8975,0x8976,0x8977,
//    0x897B,0x8981,0x8985,0x8989,0x8993,0x8995,0x89A1,
//    0x89A2,0x89A5,0x89A8,0x89A9,0x89AB,0x89AD,0x89B0,
//    0x89B1,0x89B3,0x89B5,0x89B7,0x89B8,0x89C1,0x89C2,
//    0x89C5,0x89C9,0x89CB,0x89D1,0x89D3,0x89D5,0x89D7,
//    0x89E1,0x89E5,0x89E9,0x89F1,0x89F6,0x89F7,0x8A41,
//    0x8A42,0x8A45,0x8A49,0x8A51,0x8A53,0x8A55,0x8A57,
//    0x8A61,0x8A65,0x8A69,0x8A73,0x8A75,0x8A81,0x8A82,
//    0x8A85,0x8A88,0x8A89,0x8A8A,0x8A8B,0x8A90,0x8A91,
//    0x8A93,0x8A95,0x8A97,0x8A98,0x8AA1,0x8AA2,0x8AA5,
//    0x8AA9,0x8AB6,0x8AB7,0x8AC1,0x8AD5,0x8AE1,0x8AE2,
//    0x8AE5,0x8AE9,0x8AF1,0x8AF3,0x8AF5,0x8B41,0x8B45,
//    0x8B49,0x8B61,0x8B62,0x8B65,0x8B68,0x8B69,0x8B6A,
//    0x8B71,0x8B73,0x8B75,0x8B77,0x8B81,0x8BA1,0x8BA2,
//    0x8BA5,0x8BA8,0x8BA9,0x8BAB,0x8BB1,0x8BB3,0x8BB5,
//    0x8BB7,0x8BB8,0x8BBC,0x8C61,0x8C62,0x8C63,0x8C65,
//    0x8C69,0x8C6B,0x8C71,0x8C73,0x8C75,0x8C76,0x8C77,
//    0x8C7B,0x8C81,0x8C82,0x8C85,0x8C89,0x8C91,0x8C93,
//    0x8C95,0x8C96,0x8C97,0x8CA1,0x8CA2,0x8CA9,0x8CE1,
//    0x8CE2,0x8CE3,0x8CE5,0x8CE9,0x8CF1,0x8CF3,0x8CF5,
//    0x8CF6,0x8CF7,0x8D41,0x8D42,0x8D45,0x8D51,0x8D55,
//    0x8D57,0x8D61,0x8D65,0x8D69,0x8D75,0x8D76,0x8D7B,
//    0x8D81,0x8DA1,0x8DA2,0x8DA5,0x8DA7,0x8DA9,0x8DB1,
//    0x8DB3,0x8DB5,0x8DB7,0x8DB8,0x8DB9,0x8DC1,0x8DC2,
//    0x8DC9,0x8DD6,0x8DD7,0x8DE1,0x8DE2,0x8DF7,0x8E41,
//    0x8E45,0x8E49,0x8E51,0x8E53,0x8E57,0x8E61,0x8E81,
//    0x8E82,0x8E85,0x8E89,0x8E90,0x8E91,0x8E93,0x8E95,
//    0x8E97,0x8E98,0x8EA1,0x8EA9,0x8EB6,0x8EB7,0x8EC1,
//    0x8EC2,0x8EC5,0x8EC9,0x8ED1,0x8ED3,0x8ED6,0x8EE1,
//    0x8EE5,0x8EE9,0x8EF1,0x8EF3,0x8F41,0x8F61,0x8F62,
//    0x8F65,0x8F67,0x8F69,0x8F6B,0x8F70,0x8F71,0x8F73,
//    0x8F75,0x8F77,0x8F7B,0x8FA1,0x8FA2,0x8FA5,0x8FA9,
//    0x8FB1,0x8FB3,0x8FB5,0x8FB7,0x9061,0x9062,0x9063,
//    0x9065,0x9068,0x9069,0x906A,0x906B,0x9071,0x9073,
//    0x9075,0x9076,0x9077,0x9078,0x9079,0x907B,0x907D,
//    0x9081,0x9082,0x9085,0x9089,0x9091,0x9093,0x9095,
//    0x9096,0x9097,0x90A1,0x90A2,0x90A5,0x90A9,0x90B1,
//    0x90B7,0x90E1,0x90E2,0x90E4,0x90E5,0x90E9,0x90EB,
//    0x90EC,0x90F1,0x90F3,0x90F5,0x90F6,0x90F7,0x90FD,
//    0x9141,0x9142,0x9145,0x9149,0x9151,0x9153,0x9155,
//    0x9156,0x9157,0x9161,0x9162,0x9165,0x9169,0x9171,
//    0x9173,0x9176,0x9177,0x917A,0x9181,0x9185,0x91A1,
//    0x91A2,0x91A5,0x91A9,0x91AB,0x91B1,0x91B3,0x91B5,
//    0x91B7,0x91BC,0x91BD,0x91C1,0x91C5,0x91C9,0x91D6,
//    0x9241,0x9245,0x9249,0x9251,0x9253,0x9255,0x9261,
//    0x9262,0x9265,0x9269,0x9273,0x9275,0x9277,0x9281,
//    0x9282,0x9285,0x9288,0x9289,0x9291,0x9293,0x9295,
//    0x9297,0x92A1,0x92B6,0x92C1,0x92E1,0x92E5,0x92E9,
//    0x92F1,0x92F3,0x9341,0x9342,0x9349,0x9351,0x9353,
//    0x9357,0x9361,0x9362,0x9365,0x9369,0x936A,0x936B,
//    0x9371,0x9373,0x9375,0x9377,0x9378,0x937C,0x9381,
//    0x9385,0x9389,0x93A1,0x93A2,0x93A5,0x93A9,0x93AF,
//    0x93B1,0x93B3,0x93B5,0x93B7,0x93BC,0x9461,0x9462,
//    0x9463,0x9465,0x9468,0x9469,0x946A,0x946B,0x946C,
//    0x9470,0x9471,0x9473,0x9475,0x9476,0x9477,0x9478,
//    0x9479,0x947D,0x9481,0x9482,0x9485,0x9489,0x9491,
//    0x9493,0x9495,0x9496,0x9497,0x94A1,0x94E1,0x94E2,
//    0x94E3,0x94E5,0x94E8,0x94E9,0x94EB,0x94EC,0x94F1,
//    0x94F3,0x94F5,0x94F7,0x94F9,0x94FC,0x9541,0x9542,
//    0x9545,0x9549,0x9551,0x9553,0x9555,0x9556,0x9557,
//    0x9561,0x9565,0x9569,0x9576,0x9577,0x9581,0x9585,
//    0x95A1,0x95A2,0x95A5,0x95A8,0x95A9,0x95AB,0x95AD,
//    0x95B1,0x95B3,0x95B5,0x95B7,0x95B9,0x95BB,0x95C1,
//    0x95C5,0x95C9,0x95E1,0x95F6,0x9641,0x9645,0x9649,
//    0x9651,0x9653,0x9655,0x9661,0x9681,0x9682,0x9685,
//    0x9689,0x9691,0x9693,0x9695,0x9697,0x96A1,0x96B6,
//    0x96C1,0x96D7,0x96E1,0x96E5,0x96E9,0x96F3,0x96F5,
//    0x96F7,0x9741,0x9745,0x9749,0x9751,0x9757,0x9761,
//    0x9762,0x9765,0x9768,0x9769,0x976B,0x9771,0x9773,
//    0x9775,0x9777,0x9781,0x97A1,0x97A2,0x97A5,0x97A8,
//    0x97A9,0x97B1,0x97B3,0x97B5,0x97B6,0x97B7,0x97B8,
//    0x9861,0x9862,0x9865,0x9869,0x9871,0x9873,0x9875,
//    0x9876,0x9877,0x987D,0x9881,0x9882,0x9885,0x9889,
//    0x9891,0x9893,0x9895,0x9896,0x9897,0x98E1,0x98E2,
//    0x98E5,0x98E9,0x98EB,0x98EC,0x98F1,0x98F3,0x98F5,
//    0x98F6,0x98F7,0x98FD,0x9941,0x9942,0x9945,0x9949,
//    0x9951,0x9953,0x9955,0x9956,0x9957,0x9961,0x9976,
//    0x99A1,0x99A2,0x99A5,0x99A9,0x99B7,0x99C1,0x99C9,
//    0x99E1,0x9A41,0x9A45,0x9A81,0x9A82,0x9A85,0x9A89,
//    0x9A90,0x9A91,0x9A97,0x9AC1,0x9AE1,0x9AE5,0x9AE9,
//    0x9AF1,0x9AF3,0x9AF7,0x9B61,0x9B62,0x9B65,0x9B68,
//    0x9B69,0x9B71,0x9B73,0x9B75,0x9B81,0x9B85,0x9B89,
//    0x9B91,0x9B93,0x9BA1,0x9BA5,0x9BA9,0x9BB1,0x9BB3,
//    0x9BB5,0x9BB7,0x9C61,0x9C62,0x9C65,0x9C69,0x9C71,
//    0x9C73,0x9C75,0x9C76,0x9C77,0x9C78,0x9C7C,0x9C7D,
//    0x9C81,0x9C82,0x9C85,0x9C89,0x9C91,0x9C93,0x9C95,
//    0x9C96,0x9C97,0x9CA1,0x9CA2,0x9CA5,0x9CB5,0x9CB7,
//    0x9CE1,0x9CE2,0x9CE5,0x9CE9,0x9CF1,0x9CF3,0x9CF5,
//    0x9CF6,0x9CF7,0x9CFD,0x9D41,0x9D42,0x9D45,0x9D49,
//    0x9D51,0x9D53,0x9D55,0x9D57,0x9D61,0x9D62,0x9D65,
//    0x9D69,0x9D71,0x9D73,0x9D75,0x9D76,0x9D77,0x9D81,
//    0x9D85,0x9D93,0x9D95,0x9DA1,0x9DA2,0x9DA5,0x9DA9,
//    0x9DB1,0x9DB3,0x9DB5,0x9DB7,0x9DC1,0x9DC5,0x9DD7,
//    0x9DF6,0x9E41,0x9E45,0x9E49,0x9E51,0x9E53,0x9E55,
//    0x9E57,0x9E61,0x9E65,0x9E69,0x9E73,0x9E75,0x9E77,
//    0x9E81,0x9E82,0x9E85,0x9E89,0x9E91,0x9E93,0x9E95,
//    0x9E97,0x9EA1,0x9EB6,0x9EC1,0x9EE1,0x9EE2,0x9EE5,
//    0x9EE9,0x9EF1,0x9EF5,0x9EF7,0x9F41,0x9F42,0x9F45,
//    0x9F49,0x9F51,0x9F53,0x9F55,0x9F57,0x9F61,0x9F62,
//    0x9F65,0x9F69,0x9F71,0x9F73,0x9F75,0x9F77,0x9F78,
//    0x9F7B,0x9F7C,0x9FA1,0x9FA2,0x9FA5,0x9FA9,0x9FB1,
//    0x9FB3,0x9FB5,0x9FB7,0xA061,0xA062,0xA065,0xA067,
//    0xA068,0xA069,0xA06A,0xA06B,0xA071,0xA073,0xA075,
//    0xA077,0xA078,0xA07B,0xA07D,0xA081,0xA082,0xA085,
//    0xA089,0xA091,0xA093,0xA095,0xA096,0xA097,0xA098,
//    0xA0A1,0xA0A2,0xA0A9,0xA0B7,0xA0E1,0xA0E2,0xA0E5,
//    0xA0E9,0xA0EB,0xA0F1,0xA0F3,0xA0F5,0xA0F7,0xA0F8,
//    0xA0FD,0xA141,0xA142,0xA145,0xA149,0xA151,0xA153,
//    0xA155,0xA156,0xA157,0xA161,0xA162,0xA165,0xA169,
//    0xA175,0xA176,0xA177,0xA179,0xA181,0xA1A1,0xA1A2,
//    0xA1A4,0xA1A5,0xA1A9,0xA1AB,0xA1B1,0xA1B3,0xA1B5,
//    0xA1B7,0xA1C1,0xA1C5,0xA1D6,0xA1D7,0xA241,0xA245,
//    0xA249,0xA253,0xA255,0xA257,0xA261,0xA265,0xA269,
//    0xA273,0xA275,0xA281,0xA282,0xA283,0xA285,0xA288,
//    0xA289,0xA28A,0xA28B,0xA291,0xA293,0xA295,0xA297,
//    0xA29B,0xA29D,0xA2A1,0xA2A5,0xA2A9,0xA2B3,0xA2B5,
//    0xA2C1,0xA2E1,0xA2E5,0xA2E9,0xA341,0xA345,0xA349,
//    0xA351,0xA355,0xA361,0xA365,0xA369,0xA371,0xA375,
//    0xA3A1,0xA3A2,0xA3A5,0xA3A8,0xA3A9,0xA3AB,0xA3B1,
//    0xA3B3,0xA3B5,0xA3B6,0xA3B7,0xA3B9,0xA3BB,0xA461,
//    0xA462,0xA463,0xA464,0xA465,0xA468,0xA469,0xA46A,
//    0xA46B,0xA46C,0xA471,0xA473,0xA475,0xA477,0xA47B,
//    0xA481,0xA482,0xA485,0xA489,0xA491,0xA493,0xA495,
//    0xA496,0xA497,0xA49B,0xA4A1,0xA4A2,0xA4A5,0xA4B3,
//    0xA4E1,0xA4E2,0xA4E5,0xA4E8,0xA4E9,0xA4EB,0xA4F1,
//    0xA4F3,0xA4F5,0xA4F7,0xA4F8,0xA541,0xA542,0xA545,
//    0xA548,0xA549,0xA551,0xA553,0xA555,0xA556,0xA557,
//    0xA561,0xA562,0xA565,0xA569,0xA573,0xA575,0xA576,
//    0xA577,0xA57B,0xA581,0xA585,0xA5A1,0xA5A2,0xA5A3,
//    0xA5A5,0xA5A9,0xA5B1,0xA5B3,0xA5B5,0xA5B7,0xA5C1,
//    0xA5C5,0xA5D6,0xA5E1,0xA5F6,0xA641,0xA642,0xA645,
//    0xA649,0xA651,0xA653,0xA661,0xA665,0xA681,0xA682,
//    0xA685,0xA688,0xA689,0xA68A,0xA68B,0xA691,0xA693,
//    0xA695,0xA697,0xA69B,0xA69C,0xA6A1,0xA6A9,0xA6B6,
//    0xA6C1,0xA6E1,0xA6E2,0xA6E5,0xA6E9,0xA6F7,0xA741,
//    0xA745,0xA749,0xA751,0xA755,0xA757,0xA761,0xA762,
//    0xA765,0xA769,0xA771,0xA773,0xA775,0xA7A1,0xA7A2,
//    0xA7A5,0xA7A9,0xA7AB,0xA7B1,0xA7B3,0xA7B5,0xA7B7,
//    0xA7B8,0xA7B9,0xA861,0xA862,0xA865,0xA869,0xA86B,
//    0xA871,0xA873,0xA875,0xA876,0xA877,0xA87D,0xA881,
//    0xA882,0xA885,0xA889,0xA891,0xA893,0xA895,0xA896,
//    0xA897,0xA8A1,0xA8A2,0xA8B1,0xA8E1,0xA8E2,0xA8E5,
//    0xA8E8,0xA8E9,0xA8F1,0xA8F5,0xA8F6,0xA8F7,0xA941,
//    0xA957,0xA961,0xA962,0xA971,0xA973,0xA975,0xA976,
//    0xA977,0xA9A1,0xA9A2,0xA9A5,0xA9A9,0xA9B1,0xA9B3,
//    0xA9B7,0xAA41,0xAA61,0xAA77,0xAA81,0xAA82,0xAA85,
//    0xAA89,0xAA91,0xAA95,0xAA97,0xAB41,0xAB57,0xAB61,
//    0xAB65,0xAB69,0xAB71,0xAB73,0xABA1,0xABA2,0xABA5,
//    0xABA9,0xABB1,0xABB3,0xABB5,0xABB7,0xAC61,0xAC62,
//    0xAC64,0xAC65,0xAC68,0xAC69,0xAC6A,0xAC6B,0xAC71,
//    0xAC73,0xAC75,0xAC76,0xAC77,0xAC7B,0xAC81,0xAC82,
//    0xAC85,0xAC89,0xAC91,0xAC93,0xAC95,0xAC96,0xAC97,
//    0xACA1,0xACA2,0xACA5,0xACA9,0xACB1,0xACB3,0xACB5,
//    0xACB7,0xACC1,0xACC5,0xACC9,0xACD1,0xACD7,0xACE1,
//    0xACE2,0xACE3,0xACE4,0xACE5,0xACE8,0xACE9,0xACEB,
//    0xACEC,0xACF1,0xACF3,0xACF5,0xACF6,0xACF7,0xACFC,
//    0xAD41,0xAD42,0xAD45,0xAD49,0xAD51,0xAD53,0xAD55,
//    0xAD56,0xAD57,0xAD61,0xAD62,0xAD65,0xAD69,0xAD71,
//    0xAD73,0xAD75,0xAD76,0xAD77,0xAD81,0xAD85,0xAD89,
//    0xAD97,0xADA1,0xADA2,0xADA3,0xADA5,0xADA9,0xADAB,
//    0xADB1,0xADB3,0xADB5,0xADB7,0xADBB,0xADC1,0xADC2,
//    0xADC5,0xADC9,0xADD7,0xADE1,0xADE5,0xADE9,0xADF1,
//    0xADF5,0xADF6,0xAE41,0xAE45,0xAE49,0xAE51,0xAE53,
//    0xAE55,0xAE61,0xAE62,0xAE65,0xAE69,0xAE71,0xAE73,
//    0xAE75,0xAE77,0xAE81,0xAE82,0xAE85,0xAE88,0xAE89,
//    0xAE91,0xAE93,0xAE95,0xAE97,0xAE99,0xAE9B,0xAE9C,
//    0xAEA1,0xAEB6,0xAEC1,0xAEC2,0xAEC5,0xAEC9,0xAED1,
//    0xAED7,0xAEE1,0xAEE2,0xAEE5,0xAEE9,0xAEF1,0xAEF3,
//    0xAEF5,0xAEF7,0xAF41,0xAF42,0xAF49,0xAF51,0xAF55,
//    0xAF57,0xAF61,0xAF62,0xAF65,0xAF69,0xAF6A,0xAF71,
//    0xAF73,0xAF75,0xAF77,0xAFA1,0xAFA2,0xAFA5,0xAFA8,
//    0xAFA9,0xAFB0,0xAFB1,0xAFB3,0xAFB5,0xAFB7,0xAFBC,
//    0xB061,0xB062,0xB064,0xB065,0xB069,0xB071,0xB073,
//    0xB076,0xB077,0xB07D,0xB081,0xB082,0xB085,0xB089,
//    0xB091,0xB093,0xB096,0xB097,0xB0B7,0xB0E1,0xB0E2,
//    0xB0E5,0xB0E9,0xB0EB,0xB0F1,0xB0F3,0xB0F6,0xB0F7,
//    0xB141,0xB145,0xB149,0xB157,0xB1A1,0xB1A2,0xB1A5,
//    0xB1A8,0xB1A9,0xB1AB,0xB1B1,0xB1B3,0xB1B7,0xB1C1,
//    0xB1C2,0xB1C5,0xB1D6,0xB1E1,0xB1F6,0xB241,0xB245,
//    0xB249,0xB251,0xB253,0xB261,0xB281,0xB282,0xB285,
//    0xB289,0xB291,0xB293,0xB297,0xB2A1,0xB2B6,0xB2C1,
//    0xB2E1,0xB2E5,0xB357,0xB361,0xB362,0xB365,0xB369,
//    0xB36B,0xB370,0xB371,0xB373,0xB381,0xB385,0xB389,
//    0xB391,0xB3A1,0xB3A2,0xB3A5,0xB3A9,0xB3B1,0xB3B3,
//    0xB3B5,0xB3B7,0xB461,0xB462,0xB465,0xB466,0xB467,
//    0xB469,0xB46A,0xB46B,0xB470,0xB471,0xB473,0xB475,
//    0xB476,0xB477,0xB47B,0xB47C,0xB481,0xB482,0xB485,
//    0xB489,0xB491,0xB493,0xB495,0xB496,0xB497,0xB4A1,
//    0xB4A2,0xB4A5,0xB4A9,0xB4AC,0xB4B1,0xB4B3,0xB4B5,
//    0xB4B7,0xB4BB,0xB4BD,0xB4C1,0xB4C5,0xB4C9,0xB4D3,
//    0xB4E1,0xB4E2,0xB4E5,0xB4E6,0xB4E8,0xB4E9,0xB4EA,
//    0xB4EB,0xB4F1,0xB4F3,0xB4F4,0xB4F5,0xB4F6,0xB4F7,
//    0xB4F8,0xB4FA,0xB4FC,0xB541,0xB542,0xB545,0xB549,
//    0xB551,0xB553,0xB555,0xB557,0xB561,0xB562,0xB563,
//    0xB565,0xB569,0xB56B,0xB56C,0xB571,0xB573,0xB574,
//    0xB575,0xB576,0xB577,0xB57B,0xB57C,0xB57D,0xB581,
//    0xB585,0xB589,0xB591,0xB593,0xB595,0xB596,0xB5A1,
//    0xB5A2,0xB5A5,0xB5A9,0xB5AA,0xB5AB,0xB5AD,0xB5B0,
//    0xB5B1,0xB5B3,0xB5B5,0xB5B7,0xB5B9,0xB5C1,0xB5C2,
//    0xB5C5,0xB5C9,0xB5D1,0xB5D3,0xB5D5,0xB5D6,0xB5D7,
//    0xB5E1,0xB5E2,0xB5E5,0xB5F1,0xB5F5,0xB5F7,0xB641,
//    0xB642,0xB645,0xB649,0xB651,0xB653,0xB655,0xB657,
//    0xB661,0xB662,0xB665,0xB669,0xB671,0xB673,0xB675,
//    0xB677,0xB681,0xB682,0xB685,0xB689,0xB68A,0xB68B,
//    0xB691,0xB693,0xB695,0xB697,0xB6A1,0xB6A2,0xB6A5,
//    0xB6A9,0xB6B1,0xB6B3,0xB6B6,0xB6B7,0xB6C1,0xB6C2,
//    0xB6C5,0xB6C9,0xB6D1,0xB6D3,0xB6D7,0xB6E1,0xB6E2,
//    0xB6E5,0xB6E9,0xB6F1,0xB6F3,0xB6F5,0xB6F7,0xB741,
//    0xB742,0xB745,0xB749,0xB751,0xB753,0xB755,0xB757,
//    0xB759,0xB761,0xB762,0xB765,0xB769,0xB76F,0xB771,
//    0xB773,0xB775,0xB777,0xB778,0xB779,0xB77A,0xB77B,
//    0xB77C,0xB77D,0xB781,0xB785,0xB789,0xB791,0xB795,
//    0xB7A1,0xB7A2,0xB7A5,0xB7A9,0xB7AA,0xB7AB,0xB7B0,
//    0xB7B1,0xB7B3,0xB7B5,0xB7B6,0xB7B7,0xB7B8,0xB7BC,
//    0xB861,0xB862,0xB865,0xB867,0xB868,0xB869,0xB86B,
//    0xB871,0xB873,0xB875,0xB876,0xB877,0xB878,0xB881,
//    0xB882,0xB885,0xB889,0xB891,0xB893,0xB895,0xB896,
//    0xB897,0xB8A1,0xB8A2,0xB8A5,0xB8A7,0xB8A9,0xB8B1,
//    0xB8B7,0xB8C1,0xB8C5,0xB8C9,0xB8E1,0xB8E2,0xB8E5,
//    0xB8E9,0xB8EB,0xB8F1,0xB8F3,0xB8F5,0xB8F7,0xB8F8,
//    0xB941,0xB942,0xB945,0xB949,0xB951,0xB953,0xB955,
//    0xB957,0xB961,0xB965,0xB969,0xB971,0xB973,0xB976,
//    0xB977,0xB981,0xB9A1,0xB9A2,0xB9A5,0xB9A9,0xB9AB,
//    0xB9B1,0xB9B3,0xB9B5,0xB9B7,0xB9B8,0xB9B9,0xB9BD,
//    0xB9C1,0xB9C2,0xB9C9,0xB9D3,0xB9D5,0xB9D7,0xB9E1,
//    0xB9F6,0xB9F7,0xBA41,0xBA45,0xBA49,0xBA51,0xBA53,
//    0xBA55,0xBA57,0xBA61,0xBA62,0xBA65,0xBA77,0xBA81,
//    0xBA82,0xBA85,0xBA89,0xBA8A,0xBA8B,0xBA91,0xBA93,
//    0xBA95,0xBA97,0xBAA1,0xBAB6,0xBAC1,0xBAE1,0xBAE2,
//    0xBAE5,0xBAE9,0xBAF1,0xBAF3,0xBAF5,0xBB41,0xBB45,
//    0xBB49,0xBB51,0xBB61,0xBB62,0xBB65,0xBB69,0xBB71,
//    0xBB73,0xBB75,0xBB77,0xBBA1,0xBBA2,0xBBA5,0xBBA8,
//    0xBBA9,0xBBAB,0xBBB1,0xBBB3,0xBBB5,0xBBB7,0xBBB8,
//    0xBBBB,0xBBBC,0xBC61,0xBC62,0xBC65,0xBC67,0xBC69,
//    0xBC6C,0xBC71,0xBC73,0xBC75,0xBC76,0xBC77,0xBC81,
//    0xBC82,0xBC85,0xBC89,0xBC91,0xBC93,0xBC95,0xBC96,
//    0xBC97,0xBCA1,0xBCA5,0xBCB7,0xBCE1,0xBCE2,0xBCE5,
//    0xBCE9,0xBCF1,0xBCF3,0xBCF5,0xBCF6,0xBCF7,0xBD41,
//    0xBD57,0xBD61,0xBD76,0xBDA1,0xBDA2,0xBDA5,0xBDA9,
//    0xBDB1,0xBDB3,0xBDB5,0xBDB7,0xBDB9,0xBDC1,0xBDC2,
//    0xBDC9,0xBDD6,0xBDE1,0xBDF6,0xBE41,0xBE45,0xBE49,
//    0xBE51,0xBE53,0xBE77,0xBE81,0xBE82,0xBE85,0xBE89,
//    0xBE91,0xBE93,0xBE97,0xBEA1,0xBEB6,0xBEB7,0xBEE1,
//    0xBF41,0xBF61,0xBF71,0xBF75,0xBF77,0xBFA1,0xBFA2,
//    0xBFA5,0xBFA9,0xBFB1,0xBFB3,0xBFB7,0xBFB8,0xBFBD,
//    0xC061,0xC062,0xC065,0xC067,0xC069,0xC071,0xC073,
//    0xC075,0xC076,0xC077,0xC078,0xC081,0xC082,0xC085,
//    0xC089,0xC091,0xC093,0xC095,0xC096,0xC097,0xC0A1,
//    0xC0A5,0xC0A7,0xC0A9,0xC0B1,0xC0B7,0xC0E1,0xC0E2,
//    0xC0E5,0xC0E9,0xC0F1,0xC0F3,0xC0F5,0xC0F6,0xC0F7,
//    0xC141,0xC142,0xC145,0xC149,0xC151,0xC153,0xC155,
//    0xC157,0xC161,0xC165,0xC176,0xC181,0xC185,0xC197,
//    0xC1A1,0xC1A2,0xC1A5,0xC1A9,0xC1B1,0xC1B3,0xC1B5,
//    0xC1B7,0xC1C1,0xC1C5,0xC1C9,0xC1D7,0xC241,0xC245,
//    0xC249,0xC251,0xC253,0xC255,0xC257,0xC261,0xC271,
//    0xC281,0xC282,0xC285,0xC289,0xC291,0xC293,0xC295,
//    0xC297,0xC2A1,0xC2B6,0xC2C1,0xC2C5,0xC2E1,0xC2E5,
//    0xC2E9,0xC2F1,0xC2F3,0xC2F5,0xC2F7,0xC341,0xC345,
//    0xC349,0xC351,0xC357,0xC361,0xC362,0xC365,0xC369,
//    0xC371,0xC373,0xC375,0xC377,0xC3A1,0xC3A2,0xC3A5,
//    0xC3A8,0xC3A9,0xC3AA,0xC3B1,0xC3B3,0xC3B5,0xC3B7,
//    0xC461,0xC462,0xC465,0xC469,0xC471,0xC473,0xC475,
//    0xC477,0xC481,0xC482,0xC485,0xC489,0xC491,0xC493,
//    0xC495,0xC496,0xC497,0xC4A1,0xC4A2,0xC4B7,0xC4E1,
//    0xC4E2,0xC4E5,0xC4E8,0xC4E9,0xC4F1,0xC4F3,0xC4F5,
//    0xC4F6,0xC4F7,0xC541,0xC542,0xC545,0xC549,0xC551,
//    0xC553,0xC555,0xC557,0xC561,0xC565,0xC569,0xC571,
//    0xC573,0xC575,0xC576,0xC577,0xC581,0xC5A1,0xC5A2,
//    0xC5A5,0xC5A9,0xC5B1,0xC5B3,0xC5B5,0xC5B7,0xC5C1,
//    0xC5C2,0xC5C5,0xC5C9,0xC5D1,0xC5D7,0xC5E1,0xC5F7,
//    0xC641,0xC649,0xC661,0xC681,0xC682,0xC685,0xC689,
//    0xC691,0xC693,0xC695,0xC697,0xC6A1,0xC6A5,0xC6A9,
//    0xC6B7,0xC6C1,0xC6D7,0xC6E1,0xC6E2,0xC6E5,0xC6E9,
//    0xC6F1,0xC6F3,0xC6F5,0xC6F7,0xC741,0xC745,0xC749,
//    0xC751,0xC761,0xC762,0xC765,0xC769,0xC771,0xC773,
//    0xC777,0xC7A1,0xC7A2,0xC7A5,0xC7A9,0xC7B1,0xC7B3,
//    0xC7B5,0xC7B7,0xC861,0xC862,0xC865,0xC869,0xC86A,
//    0xC871,0xC873,0xC875,0xC876,0xC877,0xC881,0xC882,
//    0xC885,0xC889,0xC891,0xC893,0xC895,0xC896,0xC897,
//    0xC8A1,0xC8B7,0xC8E1,0xC8E2,0xC8E5,0xC8E9,0xC8EB,
//    0xC8F1,0xC8F3,0xC8F5,0xC8F6,0xC8F7,0xC941,0xC942,
//    0xC945,0xC949,0xC951,0xC953,0xC955,0xC957,0xC961,
//    0xC965,0xC976,0xC981,0xC985,0xC9A1,0xC9A2,0xC9A5,
//    0xC9A9,0xC9B1,0xC9B3,0xC9B5,0xC9B7,0xC9BC,0xC9C1,
//    0xC9C5,0xC9E1,0xCA41,0xCA45,0xCA55,0xCA57,0xCA61,
//    0xCA81,0xCA82,0xCA85,0xCA89,0xCA91,0xCA93,0xCA95,
//    0xCA97,0xCAA1,0xCAB6,0xCAC1,0xCAE1,0xCAE2,0xCAE5,
//    0xCAE9,0xCAF1,0xCAF3,0xCAF7,0xCB41,0xCB45,0xCB49,
//    0xCB51,0xCB57,0xCB61,0xCB62,0xCB65,0xCB68,0xCB69,
//    0xCB6B,0xCB71,0xCB73,0xCB75,0xCB81,0xCB85,0xCB89,
//    0xCB91,0xCB93,0xCBA1,0xCBA2,0xCBA5,0xCBA9,0xCBB1,
//    0xCBB3,0xCBB5,0xCBB7,0xCC61,0xCC62,0xCC63,0xCC65,
//    0xCC69,0xCC6B,0xCC71,0xCC73,0xCC75,0xCC76,0xCC77,
//    0xCC7B,0xCC81,0xCC82,0xCC85,0xCC89,0xCC91,0xCC93,
//    0xCC95,0xCC96,0xCC97,0xCCA1,0xCCA2,0xCCE1,0xCCE2,
//    0xCCE5,0xCCE9,0xCCF1,0xCCF3,0xCCF5,0xCCF6,0xCCF7,
//    0xCD41,0xCD42,0xCD45,0xCD49,0xCD51,0xCD53,0xCD55,
//    0xCD57,0xCD61,0xCD65,0xCD69,0xCD71,0xCD73,0xCD76,
//    0xCD77,0xCD81,0xCD89,0xCD93,0xCD95,0xCDA1,0xCDA2,
//    0xCDA5,0xCDA9,0xCDB1,0xCDB3,0xCDB5,0xCDB7,0xCDC1,
//    0xCDD7,0xCE41,0xCE45,0xCE61,0xCE65,0xCE69,0xCE73,
//    0xCE75,0xCE81,0xCE82,0xCE85,0xCE88,0xCE89,0xCE8B,
//    0xCE91,0xCE93,0xCE95,0xCE97,0xCEA1,0xCEB7,0xCEE1,
//    0xCEE5,0xCEE9,0xCEF1,0xCEF5,0xCF41,0xCF45,0xCF49,
//    0xCF51,0xCF55,0xCF57,0xCF61,0xCF65,0xCF69,0xCF71,
//    0xCF73,0xCF75,0xCFA1,0xCFA2,0xCFA5,0xCFA9,0xCFB1,
//    0xCFB3,0xCFB5,0xCFB7,0xD061,0xD062,0xD065,0xD069,
//    0xD06E,0xD071,0xD073,0xD075,0xD077,0xD081,0xD082,
//    0xD085,0xD089,0xD091,0xD093,0xD095,0xD096,0xD097,
//    0xD0A1,0xD0B7,0xD0E1,0xD0E2,0xD0E5,0xD0E9,0xD0EB,
//    0xD0F1,0xD0F3,0xD0F5,0xD0F7,0xD141,0xD142,0xD145,
//    0xD149,0xD151,0xD153,0xD155,0xD157,0xD161,0xD162,
//    0xD165,0xD169,0xD171,0xD173,0xD175,0xD176,0xD177,
//    0xD181,0xD185,0xD189,0xD193,0xD1A1,0xD1A2,0xD1A5,
//    0xD1A9,0xD1AE,0xD1B1,0xD1B3,0xD1B5,0xD1B7,0xD1BB,
//    0xD1C1,0xD1C2,0xD1C5,0xD1C9,0xD1D5,0xD1D7,0xD1E1,
//    0xD1E2,0xD1E5,0xD1F5,0xD1F7,0xD241,0xD242,0xD245,
//    0xD249,0xD253,0xD255,0xD257,0xD261,0xD265,0xD269,
//    0xD273,0xD275,0xD281,0xD282,0xD285,0xD289,0xD28E,
//    0xD291,0xD295,0xD297,0xD2A1,0xD2A5,0xD2A9,0xD2B1,
//    0xD2B7,0xD2C1,0xD2C2,0xD2C5,0xD2C9,0xD2D7,0xD2E1,
//    0xD2E2,0xD2E5,0xD2E9,0xD2F1,0xD2F3,0xD2F5,0xD2F7,
//    0xD341,0xD342,0xD345,0xD349,0xD351,0xD355,0xD357,
//    0xD361,0xD362,0xD365,0xD367,0xD368,0xD369,0xD36A,
//    0xD371,0xD373,0xD375,0xD377,0xD37B,0xD381,0xD385,
//    0xD389,0xD391,0xD393,0xD397,0xD3A1,0xD3A2,0xD3A5,
//    0xD3A9,0xD3B1,0xD3B3,0xD3B5,0xD3B7
//};
//
//void api_KS_to_KSSM(byte *KS_Str, word len)
//{
//    int loc, i;
//
//    if (!len) return;
//    for(i=0;i<len;)
//    {
//        if (KS_Str[i] & 0x80)
//        {
//            loc = ((word)KS_Str[i] - 176) * 94 + (word)KS_Str[i+1] - 161;
//            if (loc >= 2350)
//            {
//                KS_Str[i] = ' ';
//                KS_Str[i+1] = ' ';
//            }
//            else
//            {
//                if (loc >= 0)
//                {
//                    KS_Str[i]   = (byte)(api_i_h_buffer[loc] / 256);
//                    KS_Str[i+1] = (byte)(api_i_h_buffer[loc] % 256);
//                }
//            }
//            i+=2;
//        } else i++;
//    }
//}
//
//int BinarySearch(word key)
//{
//    int high, mid, low;                     // 이진 탐색시 사용하는 변수들
//    int flag;                               // 루프를 빠져 나갈 조건 검사 변수
//    unsigned temp;                          // 임시 저장용 변수
//
//    high = 2349;                            // high의 초기값을 IndexTbl의 배열 크기로 함
//    low  = 0;                               // low의 초기값을 0으로 함
//    flag = 0;                               // flag의 초기값을 0으로 함
//    do
//    {
//        mid = (int)(high + low) / 2;        // 가운데 값을 계산
//        temp = api_i_h_buffer[mid];             // IndexTbl[mid]의  값을 temp에 저장
//        if (temp == key) return(mid);       // key에 해당값이 IndexTbl에 있는지 검사
//        else
//        {
//            if (temp > key)                 // key보다 temp값이 크면 mid 아래에 key에
//                high = mid - 1;             // 해당하는 값이 있으므로 high를 수정
//            else if (temp < key)            // key보다 temp값이 작으면 mid 위에 key에
//                low  = mid + 1;             // 해당하는 값이 있으므로 low를 수정
//            if (high < low) flag = 1;       // key가 IndexTbl에 없을 경우 flag를 1로 하여
//        }                                   // 루프를 빠져 나간다.
//    } while(!flag);
//    return(-1);                             // key가 IndexTbl에 없을 경우 -1을 돌려 준다.
//}
//
//void api_KSSM_to_KS(byte *KSSM_Str, word len)
//{
//    int i, posion;
//
//    if (!len) return;
//    for(i=0;i<len;)
//    {
//        if (KSSM_Str[i] & 0x80)
//        {
//            posion = BinarySearch(KSSM_Str[i] * 256 + (word)KSSM_Str[i+1]);
//            if (posion >= 0)
//            {
//               KSSM_Str[i]   = (byte)(176 + posion / 94);
//               KSSM_Str[i+1] = (byte)(161 + posion % 94);
//            }
//            i += 2;
//        } else i++;
//    }
//}
//

///////////////////////////////////////////////////////////
// iccard
///////////////////////////////////////////////////////////
//void icc_reset(void)
//{
//    ioctl(iccdevfd, ICC_RESET);
//}
#endif

///////////////////////////////////////////////////////////
// general
///////////////////////////////////////////////////////////

void HWIni(void)
{
    byte clevel;

    api_release_alltimer();
#ifdef MODEM_DEBUG
    sio_set(AUX_PORT, B115200);
#endif
#if 0
    fi_region_4K_reload((byte *)&Region00, FL_REGION00_AREA, sizeof(FL_REGION_00), 0x00);
    clevel = Region00.lcdbr;
    if(clevel<0x0f || clevel>0x28) clevel = 0x1c;
    clevel = lcd_contrast((byte *)"SET", clevel);
    font_open();
#endif
}

void swval_init(void)
{
#if 0
    fi_region_4K_reload((byte *)&Region00, FL_REGION00_AREA, sizeof(FL_REGION_00), 0x00);
    api_modem_initset();
    keyled_off();keyled2_off();lcd_backlight_on();
    if (Region00.buzer_on == SW_ON) beep_on();
    else                            beep_off();
#endif
}


void fi_region_4K_reload(byte *setdata, char *full_path, int len, byte cmp)
{
    FILE *fp;
    
    cmp = cmp;
    if ((fp = fopen(full_path, "r+")) == NULL)
    {
        if ((fp = fopen(full_path, "w+")) == NULL)
        {
            printf("region reload(%s) open error :", full_path);
            return;
        }
        memset(setdata, 0, len);
        fwrite(setdata, 1, len, fp);
        fclose(fp);
        return;
    }
    
    fread(setdata, 1, len, fp);
    fclose(fp);
}

void fi_region_4K_write(byte *setdata, char *full_path, int len)
{
    FILE *fp;
    byte ttu[4096+1]={0};    // region으로 설정되는 데이타는 4K이내로 설정함, 이유없슴...

    if ((fp = fopen(full_path, "r")) == NULL)
    {
        //fclose(fp);
       ;
    }
    else
    {
        if (len > 4096) len = 4096;
        fread(ttu, 1, len, fp);
        fclose(fp);

        // 값이 동일하다면 write할 필요가 없음.
        if (!memcmp(ttu, setdata, len)) return; 
    }
    
    if ((fp = fopen(full_path, "w+")) == NULL)
    {
        printf("region write(%s) open error :", full_path);
        return;
    }

    fwrite(setdata, 1, len, fp);
    fclose(fp);
}

void fi_area_reload(byte *setdata, char *full_path, int len)
{
    FILE *fp;
    
    if ((fp = fopen(full_path, "r+")) == NULL)
    {
        //fclose(fp);
        return;
    }
    
    fread(setdata, 1, len, fp);
    fclose(fp);
}

void fi_area_write(byte *setdata, char *full_path, int len)
{
    FILE *fp;

    if ((fp = fopen(full_path, "w+")) == NULL)
    {
        printf("region write(%s) open error :", full_path);
        return;
    }

    fwrite(setdata, 1, len, fp);
    fclose(fp);
}

void fl_CAPK_write(byte *setdata, char *full_path, int len)
{
    FILE *fp;


    if ((fp = fopen(full_path, "w+")) == NULL)
    {
        printf("region write(%s) open error :", full_path);
        return;
    }

    fwrite(setdata, 1, len, fp);

    fclose(fp);
}


ulong a2l(byte *msg, word len)
{
    byte i;
    ulong val=0, val1;

    val = 0l;
    for(i = 0; i < len; i++)
    {
        val1 = (msg[i]>='0' && msg[i]<='9') ? (msg[i] - 0x30) : (0);
        val  = val*10 + val1;
    }
    return(val);
}

byte l2a(ulong val, byte *buf)
{
    byte li, lj;
    byte ltmp;
    byte ret;

    li = lj = 0;

    do{
        buf[li++] = (byte)((val % 10)+0x30);
    }while((val /= 10) != 0L);

    buf[li] = '\0';
    ret = li;

    li -= 1;

    while(lj < li)
    {
        ltmp = buf[li];
        buf[li] = buf[lj];
        buf[lj] = ltmp;
        --li; ++lj;
    }
    return(ret);
}

void htoa(byte x, byte *y)
{
    byte   a,b;

    a = x & 0x0f;
    b = (x & 0xf0) >> 4;
    y[1] = (a < 10) ? (a + 0x30) : (a + 0x37);
    y[0] = (b < 10) ? (b + 0x30) : (b + 0x37);
}

byte IsDigit(byte tt, byte limit)
{
    if((tt >= '0') && (tt <= limit + '0')) return(1);
    else return(0);
}

// 1:success 0:fail
int ethernet_link_chk(void)
{
	FILE *read_fp;
	char buffer[128 + 1];
	int chars_read, i;
	memset(buffer, 0, sizeof(buffer));

	read_fp = popen("cat /sys/class/net/eth0/operstate", "r");
	if (read_fp != NULL)
	{
		chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
		if (chars_read > 0)
		{
			if (strcmp(buffer, "down\n") == 0)
			{
				pclose(read_fp);
				return 0;
			}
			else
			{
				pclose(read_fp);
				return 1;
			}
		}
	} 
}

// returns : 0 -> success, 1 -> fail
// purpose : udhcpc 로 유동 IP 설정 후, /etc/resolv.conf 값을 읽어서 "nameserver"라는 문구가 없다면
//           유동 IP 설정 오류이다.
byte dns_info_check(void)
{
    FILE *fp;
    byte buf[64];
    
    if ((fp = fopen("/etc/resolv.conf", "r")) == NULL)
    {
        printf("/etc/resolv.conf read open error :");
        return(1);
    }

    for(;;)
    {
        memset(buf, 0, sizeof(buf));
        if (fgets(buf, sizeof(buf)-1, fp)==NULL) break;
        
        if (memcmp(buf, "nameserver", 10)==0)
        {
            fclose(fp);
            return(0);
        }
    }
    fclose(fp);
    return(1);
}

byte init_ethernet(void)
{
    int  error;
    byte buf2[256]={0};
    byte IP[32]={0};
    byte GW[32]={0};
    byte SM[32]={0};

    memset(&Eth_info, 0, sizeof(ETH_INFO_CHK));
    if(Region01.Comm.comm_line & ETHERNET_LINE)
    {
        if(ethernet_link_chk() == 0)
        {
            Eth_info.step = 2;
            Eth_info.IP_state = 0;
            return FALSE;
        }
#if 1
        if(Region01.Comm.Ethernet.ipmethod == '1')	// 자동IP
        {
            system("rm /etc/resolv.conf");
            system("udhcpc -b -retries=5");
            if(dns_info_check())
            {
                Eth_info.step = 1;
                Eth_info.IP_state = 0;
            }
            else
            { 
                write_dns_info();
                Eth_info.step = 1;
                Eth_info.IP_state = 1;
            }
// 20120723_PSH LAN UDHC_DEBUG KILL 안함            
//			system("kill -9 `pidof udhcpc_debug`"); // 20120201_TJH // 자동으로 IP가 잡히고 난 뒤에는 udhcp를 죽이도록 했음. 이것땜시 프린터 출력시 이상이 생기는 경우가 있는것 같아서.
        }
        else										// 고정IP
#endif
        {
            ip_format_copy(IP, Region01.Comm.Ethernet.staticIP);
            ip_format_copy(SM, Region01.Comm.Ethernet.staticSM);
            ip_format_copy(GW, Region01.Comm.Ethernet.staticGW);

            memset(buf2, 0x00, sizeof(buf2));
            sprintf(buf2, "/app/static.sh %s %s %s", IP, SM, GW);
            error = system(buf2);
            if (error != 0)
            {
                Eth_info.step = 1;
                Eth_info.IP_state = 0;
            }
            else
            {
                write_dns_info();
                Eth_info.step = 1;
                Eth_info.IP_state = 1;
            }
        }
    }
    
    if (Eth_info.IP_state == 1) return(TRUE);
    else                        return(FALSE);
}

//-----------------------------------------------------------------------------
// Region01.Comm.Ethernet.ipmethod='1':자동 IP 설정으로 초기화, Region01.Comm.Ethernet.ipmethod='2':고정 IP 설정으로 초기화
//-----------------------------------------------------------------------------
byte init_ethernet2(void)
{
    int  error, pstat;
    byte buf2[256]={0};
    byte IP[32]={0};
    byte GW[32]={0};
    byte SM[32]={0};

    memset(&Eth_info, 0, sizeof(ETH_INFO_CHK));
    if(ethernet_link_chk() == 0)
    {
        Eth_info.step = 2;
        Eth_info.IP_state = 0;
        return FALSE;
    }

    if(Region01.Comm.Ethernet.ipmethod == '1')	// 자동IP
    {
        system("rm /etc/resolv.conf");
// 20120723_PSH LAN UDHC_DEBUG KILL 안함            
		system("kill -9 `pidof udhcpc`");
        system("udhcpc_debug -b -retries=5");
        if(dns_info_check())
        {
            Eth_info.step = 1;
            Eth_info.IP_state = 0;
        }
        else
        { 
            write_dns_info();
            Eth_info.step = 1;
            Eth_info.IP_state = 1;
        }
// 20120723_PSH LAN UDHC_DEBUG KILL 안함            
//			system("kill -9 `pidof udhcpc_debug`"); // 20120201_TJH // 자동으로 IP가 잡히고 난 뒤에는 udhcp를 죽이도록 했음. 이것땜시 프린터 출력시 이상이 생기는 경우가 있는것 같아서.
    }
    else										// 고정IP
    {
        pstat = system("pidof udhcpc");
        if (!pstat) system("kill -9 `pidof udhcpc`");

        ip_format_copy(IP, Region01.Comm.Ethernet.staticIP);
        ip_format_copy(SM, Region01.Comm.Ethernet.staticSM);
        ip_format_copy(GW, Region01.Comm.Ethernet.staticGW);

        memset(buf2, 0x00, sizeof(buf2));
        sprintf(buf2, "/app/static.sh %s %s %s", IP, SM, GW);
        error = system(buf2);
        if (error != 0)
        {
//printf("ckdns003-[%s]\n",Region01.Comm.Lan.domain_ip);
            Eth_info.step = 1;
            Eth_info.IP_state = 0;
        }
        else
        {
//printf("ckdns004-[%s]\n", Region01.Comm.Lan.domain_ip);
            write_dns_info();
            Eth_info.step = 1;
            Eth_info.IP_state = 1;
        }
    }
    
    if (Eth_info.IP_state == 1) return(TRUE);
    else                        return(FALSE);
}

void write_dns_info(void)
{
    FILE *fp;
    
    if ((fp = fopen("/etc/resolv.conf", "w+")) == NULL)
    {
        printf("/etc/resolv.conf write open error :");
        return;
    }
//printf("ckdns005-write dns\n");
    fprintf(fp, "nameserver %s\n", Region01.Comm.Ethernet.dns_server01);
    fprintf(fp, "nameserver %s\n", Region01.Comm.Ethernet.dns_server02);
    fclose(fp);
}

unsigned long a2ll(byte *msg, word len)
{
    byte i;
    ulong val=0, val1;

    val = 0l;
    for(i = 0; i < len; i++)
    {
        val1 = (msg[i]>='0' && msg[i]<='9') ? (msg[i] - 0x30) : (0);
        val  = val*10 + val1;
    }
    return(val);
}

unsigned char set_systemtime2rtcArg(struct rtc_time rtm)
{
    if (!rtm.tm_year)
    {
        printf("Argument Parse Error\n");
        return 1;
    }

    if (ioctl(rtcfd, RTC_SET_TIME, &rtm) < 0)
    {
        perror("RTC_SET_TIME error");
        return 1;
    }

    memset(&rtm, 0, sizeof(rtm));

    if (ioctl(rtcfd, RTC_RD_TIME, &rtm) < 0)
    {
        perror("RTC_RD_TIME error");
        return 1;
    }
    return 0;
}

void get_dtm(byte *dtm)
{
    time_t time_now;
    struct tm *tm;
    char years[5];

    time(&time_now);
    tm = localtime(&time_now);
    sprintf(years, "%04d", tm->tm_year + 1900);

    sprintf(dtm, "%2.2s%02d%02d%02d%02d%02d%d",
            &years[2], tm->tm_mon+1, tm->tm_mday,
            tm->tm_hour, tm->tm_min, tm->tm_sec,tm->tm_wday);
}

byte set_dtm(byte *time_stamp)
{
    byte ret=0;
    byte yearb[4+1],monb[2+1],dayb[2+1],hourb[2+1],minb[2+1],secb[2+1];
    struct rtc_time rtm;
	byte set_temp[100];
	
    sprintf(yearb,"20%2.2s", time_stamp); // 년
    memcpy(monb,  &time_stamp[2],  2); // 월
    memcpy(dayb,  &time_stamp[4],  2); // 일
    memcpy(hourb, &time_stamp[6],  2); // 시
    memcpy(minb,  &time_stamp[8],  2); // 분
    memcpy(secb,  &time_stamp[10], 2); // 초
    memset(set_temp, 0x00, sizeof(set_temp));
	sprintf(set_temp, "date %2.2s%2.2s%2.2s%2.2s%4.4s.%2.2s", monb, dayb, hourb, minb, yearb, secb);
	system(set_temp);

    rtm.tm_year  = (word)a2ll(yearb, 4) - 1900;
    rtm.tm_mon   = (word)a2ll(monb,  2) - 1;
    rtm.tm_mday  = (word)a2ll(dayb,  2);
    rtm.tm_hour  = (word)a2ll(hourb, 2);
    rtm.tm_min   = (word)a2ll(minb,  2);
    rtm.tm_sec   = (word)a2ll(secb,  2);
    set_systemtime2rtcArg(rtm);

    return(0);
}

byte api_mchkbuf[32];
byte api_send_wait(byte *tx, byte *rx1, byte *rx2, word second)
{
    byte Id = 0;
    word len, len1, len2, i;

	aDebugMsg("버려지는 데이터:");
    while(Com_Ready(PSTN_PORT))
    {
        printf("%c", Com_Getc(PSTN_PORT));
    }
	printf("\n");

	aDebugMsg("Logic -> 모뎀:");
    if (tx[0])
    {
        len = Strlen(tx);
        for(i=0; i<len; i++)
        {
            if (tx[i] == '.') tx[i] = ',';
        }
        Com_Write(PSTN_PORT, tx, strlen(tx));
		aDebugMsg(tx);
    }

	aDebugMsg("모뎀 -> Logic:");
    if (rx1[0] || rx2[0])
    {
        len1 = strlen(rx1);
        len2 = strlen(rx2);
        Memset(api_mchkbuf, 0x00, sizeof(api_mchkbuf));
        set_timer(&Id, second*100);
        while(1)
        {
			usleep(1);
            if (chk_timer(Id)==FALSE)
            {
                release_timer(Id);
				printf("\n");
                return(MD_TIMEOUT);
            }

            if (Com_Ready(PSTN_PORT))
            {
                memcpy(api_mchkbuf, api_mchkbuf+1, sizeof(api_mchkbuf)-1);
                api_mchkbuf[sizeof(api_mchkbuf)-1] = Com_Getc(PSTN_PORT);
				printf("%c", api_mchkbuf[sizeof(api_mchkbuf)-1]);
                if (len1)
                {
                    if (!memcmp(&api_mchkbuf[sizeof(api_mchkbuf)-len1], rx1, len1))
                    {
                        release_timer(Id);
						printf("\n");
                        return(MD_OK);
                    }
                }
                if (len2)
                {
                    if (!memcmp(&api_mchkbuf[sizeof(api_mchkbuf)-len2], rx2, len2))
                    {
                        release_timer(Id);
						printf("\n");
                        return(MD_OK);
                    }
                }
            }
        }
    }
    else return(MD_OK);
}

