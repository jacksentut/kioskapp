
#ifdef HNX720T_GLOBALS
    #define HNX720T_EXT
#else
    #define HNX720T_EXT    extern
#endif


#ifdef HNX720T_GLOBALS
    #define HNX720T_EXT
#else
    #define HNX720T_EXT    extern
#endif

///////////////////////////////////////////////////////////
// general
///////////////////////////////////////////////////////////
#define TRUE            0x01
#define FALSE           0x00

///////////////////////////////////////////////////////////
// timer
///////////////////////////////////////////////////////////

//#define release_alltimer    api_release_alltimer
//#define set_apptimer        api_reset_apptimer
//#define reset_apptimer      api_reset_apptimer
//#define chk_apptimer        api_chk_apptimer
//#define release_apptimer    api_release_apptimer

void api_release_alltimer(void);
char api_set_apptimer(int id, int expires);
char api_reset_apptimer(int id, int expires);
char api_chk_apptimer(int id);
char api_release_apptimer(int id);

byte set_timer(byte *id_no, int count);
byte reset_timer(byte id_no, int count);
byte chk_timer(byte id_no);
byte release_timer(byte id_no);
void Delay(word n);

///////////////////////////////////////////////////////////
// key
///////////////////////////////////////////////////////////
/*----------------------*/
/* Key Redefines        */
#define CANCEL        0x15
#define TIMEOUT		  0x64

#define GetKey    GetAppKey
void cankey_set(byte value);
int Kbhit(void);
byte GetAppKey(void);
char GetLibKey(byte type);
byte Entch(word tdelay);
byte Entch1(word tdelay);

///////////////////////////////////////////////////////////
// serial
///////////////////////////////////////////////////////////
#ifndef SOH
    #define SOH    0x01
#endif
#ifndef CAN
    #define CAN    0x18
#endif

int sputs(int cport, char *msg);
int sputch(int cport, char chval);
int Com_Ready(int cport);
int Com_Getc(int cport);
int Com_Write(int cport, char *msg, int len);

///////////////////////////////////////////////////////////
// beep
///////////////////////////////////////////////////////////

void perr(byte cnt);

///////////////////////////////////////////////////////////
// lcd
///////////////////////////////////////////////////////////

void displogo(int pos, const uchar *logo);
void lcdchs_kssm(const void *lcdds, byte start, byte end, byte disptype);
//void lcd_brightness_set_func(void);
byte lcd_brightness_set_func(void);



///////////////////////////////////////////////////////////
// led
///////////////////////////////////////////////////////////

#define LED_ON    0x01
#define LED_OFF   0x00

void online_led(byte type);
void lightdial_led(byte type);

///////////////////////////////////////////////////////////
// msr
///////////////////////////////////////////////////////////
HNX720T_EXT int track2_size;
HNX720T_EXT byte track2_data[128];
HNX720T_EXT int track3_size;
HNX720T_EXT byte track3_data[256];
HNX720T_EXT int track23_size;
HNX720T_EXT byte track23_data[256];

void mcr2_init(void);
byte mcr2_chk(byte *dest);
void mcr3_init(void);
byte mcr3_chk(byte *dest);
void mcr23_init(void);
byte mcr23_chk(byte *track2_dest, byte *track3_dest);

///////////////////////////////////////////////////////////
// hanin(hangul)
///////////////////////////////////////////////////////////

#define  IFILL    0x01
#define  MFILL    0x10
#define  EFILL    0x01
#define  NUMBER   0
#define  HANGUL   1
#define  ENGLISH  2

// HANIN
HNX720T_EXT byte hcnt;
HNX720T_EXT byte han, hi, hm, he, b_he;
HNX720T_EXT byte hstep, nhstep, keycnt, orgcd;
HNX720T_EXT byte *hkp;
HNX720T_EXT byte curpos,curchar;

void init_han(byte hantype);
//byte HanIn(byte x, byte *rtnbuf, byte max);
byte HanIn(byte x, byte *rtnbuf, byte max, byte dispstart);
///////////////////////////////////////////////////////////
// RTC
///////////////////////////////////////////////////////////
void rtc2systemtime(void);
unsigned char set_systemtime2rtcArg(struct rtc_time rtm);
byte rtc_change(void);

///////////////////////////////////////////////////////////
// CRC
///////////////////////////////////////////////////////////
int crc_sum_FDI(char *szBuff, int inPos) ;

///////////////////////////////////////////////////////////
// CALCULATOR
///////////////////////////////////////////////////////////
void Divide(u8 *data1, u8 *data2, u8 *data3);
void calculator(void);

///////////////////////////////////////////////////////////
// printer
///////////////////////////////////////////////////////////

void api_paper_set(void);
void line_feed(word cnt);
byte print_start(byte mode, int type);
void PrintInt_Check(void);
int  print_end(void);
int  print_status(void);
void prt_speed_set(void);
int print_cover_status(void);

///////////////////////////////////////////////////////////
// des
///////////////////////////////////////////////////////////
// DESLIB DEFINES
#define ENCRYPT   1
#define DECRYPT   2
void function_des(byte cr_decr, byte *Data, byte *Key, byte *Res);

///////////////////////////////////////////////////////////
// xmodem
///////////////////////////////////////////////////////////
#define SYNC    0x16
void send_jtic700l_func(void);
void rcv_jtic700l_func(void);
u16 calcrcX(u8 *ptr, int len);
int rcv_ramcode(byte C_PORT, byte *SaveFileName, byte *InitStr);
void rcv_xmodem_main(void);

///////////////////////////////////////////////////////////
// modem
///////////////////////////////////////////////////////////
byte api_send_wait(byte *tx, byte *rx1, byte *rx2, word second);
void api_md_disconnect(void);
void api_modem_initset(void);
void api_dial_speed(byte speed);
void api_modem_pbx(byte pabx);
void api_modem_spd(int speed);
void md_conn_check(void);
void md_start(void);

HNX720T_EXT byte telbuf[32];
HNX720T_EXT byte mdflag;


///////////////////////////////////////////////////////////
// hanconv
///////////////////////////////////////////////////////////
//void api_KS_to_KSSM(byte *KS_Str, word len);
//void api_KSSM_to_KS(byte *KSSM_Str, word len);

///////////////////////////////////////////////////////////
// iccard
///////////////////////////////////////////////////////////
//void icc_reset(void);


///////////////////////////////////////////////////////////
// general
///////////////////////////////////////////////////////////
void HWIni(void);
void swval_init(void);
void fi_region_4K_reload(byte *setdata, char *full_path, int len, byte cmp);
void fi_region_4K_write(byte *setdata, char *full_path, int len);
void fi_area_reload(byte *setdata, char *full_path, int len);
void fi_area_write(byte *setdata, char *full_path, int len);
void fl_CAPK_write(byte *setdata, char *full_path, int len);
ulong a2l(byte *msg, word len);
byte l2a(ulong val, byte *buf);
void htoa(byte x, byte *y);
byte IsDigit(byte tt, byte limit);
void comm_changeover(void);



///////////////////////////////////////////////////////////
// ethernet
///////////////////////////////////////////////////////////
HNX720T_EXT int link_fd;
HNX720T_EXT byte IP_state;
HNX720T_EXT int host_port_num;
//HNX720T_EXT WTRNS_CTX* pTermCtx; // DELETE_ETHERNET
// (110111, HKSHIN)
typedef struct {
    byte step;
    byte IP_state;
} ETH_INFO_CHK;
HNX720T_EXT ETH_INFO_CHK Eth_info;

int  ethernet_link_chk(void);
byte init_ethernet(void);
byte init_ethernet2(void);
byte dns_info_check(void);
void read_dns_info(void);
void write_dns_info(void);

///////////////////////////////////////////////////////////
// WCDMA
///////////////////////////////////////////////////////////
HNX720T_EXT int wcdma_fd;

byte wcdma_send_wait(byte *tx, byte *rx1, word second);
void wcdma_disconnect(void);
void wcdma_conn_check(void);
void wcdma_start(void);
byte init_wcdma(void);

HNX720T_EXT byte cdmadflag;

void cdma_signal_handler(void);


///////////////////////////////////////////////////////////
// else
///////////////////////////////////////////////////////////

#define SW_ON    0x31
#define SW_OFF   0x32

#define Strlen    strlen
#define Memcpy    memcpy
#define Memset    memset
#define Strncmp   strncmp
#define Memcmp    memcmp


#define LCD_XSIZE1       128
#define LCD_YSIZE1       64
#define LCD_PAGESIZE1    LCD_YSIZE / 8

HNX720T_EXT byte Image_Data[LCD_YSIZE1 / 8][LCD_XSIZE1];
HNX720T_EXT byte curpos,curchar;



