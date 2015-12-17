/*----------------------*/
/* CAPTURE DEFINE       */
/************************/
#ifdef _APP_DEF_
#else
#define _APP_DEF_
//#define PRT_SPEED_TEST
 
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2014/11/13"	// v1.4 ��������
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2014/11/28" 	// v1.5 tms �ȵǴ� �� ����, ������ �ӵ����� �׽�Ʈ �߰�, �����ڵ� E000 -> 8000 �� ���� �߰�
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2014/12/09" 	// v1.6 ������ ����̹� ����, ȭ�� �Ͼ����� ����, �����ۺ���/�ŴϾ� ��� �÷ο� ���� �߰�
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2014/12/18" 	// �ϴ� �׸��μ� �߰�, �������˽� �� ���� ���԰�� ����� �� ���� �߰�, ����� �߰�
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2014/12/22" 	// ����� �� ����� ���� ���� ���� ����, ���� �ݾ� Ʋ������ ���� ����
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2014/12/23" 	// �߱ǽ� �ϴ� �׸��μ� �߰�
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2014/12/30" 	// �߱ǽ� �ϴ� �׸��μ� �ٸ� �׸����� ��ü
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2015/01/02" 	// �߱ǽ� �ϴ� �׸��μ� �ٸ� �׸����� ��ü, v2.0
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2015/01/06" 	// ��ġȮ�� ���� ��û�� 1~3540�ʸ� �����ϰ� ���� ������ ��û�ϵ��� ����, ��ġŸ�ӵ� ���ʸ� �����ϰ� �����ϵ��� ����, v2.1
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2015/01/08" 	// ������� ��ư Ŭ���� ȭ�� �Ͼ�� ������ ����/�ݾ� ���� ��ư Ŭ���� �Ͼ�� ������ ���� ����, v2.2
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2015/01/08" 	// ������ȣȭ ����, v2.3
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2015/02/10" 	// TMS�� �μ�� ���� �ʵǵ��� ����, v2.3
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2015/02/10" 	// ���׸Ӵ� �κ��� �����ؼ� ���� ��, �̺�Ʈ�� �����ؼ� ���ͼ�, UI�� hide�� �ι� ����Ǽ� ���ߴ� ���� ����, v2.4
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2015/03/09" 	// ��ȣȭ �������, v2.5
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2015/03/17" 	// usb�� �α� ��� ��� �߰�, v2.5
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2015/03/18" 	// Ǯ Ŀ�� ����, v2.5
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2015/03/20" 	// ���� �α� ��� �߰�, v2.5
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2015/03/23" 	// SSH 22�� ��Ʈ �����ִ� �� ���� �߰�, �� ����, v2.6

#define PROGRAM_VERSION "2.6"	// Ű����ũ ����

#define PROGRAM_DATE    "20150323" 
#define PRODUCT_CODE    "CA"			// �ܸ������� 

#define FL_REGIONMA_AREA         "/app/RegionMA.dat"
#define FL_REGION00_AREA         "/app/Region00.dat"
#define FL_REGION01_AREA         "/app/kRegion01.dat"
#define FL_REGION02_AREA         "/app/kRegion02.dat"
#define FL_REGION03_AREA         "/app/Region03.dat"
#define FL_REGION04_AREA         "/app/kRegion04.dat"
#define FL_REGION05_AREA         "/app/Region05.dat"
#define FL_REGION06_AREA         "/app/Region06.dat"
#define FL_REGION07_AREA         "/appt/Region07.dat"
#define FL_REGION08_AREA         "/app/Region08.dat"
#define FL_REGION09_AREA         "/app/Region09.dat"
#define FL_REGION10_AREA         "/app/Region10.dat"
 
#define TMS_APPIMAGE_AREA        "/tmp/TmsAppImage.dat"  
#define KWI_APPIMAGE_AREA        "/tmp/InfAppImage.tar.gz"
#define KWI_APPIMAGE_AREA2       "/app/InfAppImage.tar.gz"  
#define KWI_EXECPROG_NAME        "/app/kiosk_logic.bin" 
#define KWI_SENDPROG_NAME        "/tmp/kiosk_logic_new.bin"


// �ſ�/����/���� ���� DATA CAPTURE AREA
#define MAXCAP                   4096                           	// Record Size(256 byte) * 4096 = 0x100000 bytes = 1MBytes
#define CAP_RECORD_SIZE          256                             // record size 
#define ONE_ERASE_BLOCK          4096                           	// �ѹ��� ���� �� �ִ� �� ������
#define FL_CDATACAP_AREA         "/app/kdatacap.dat"

// Error Log
#define FL_CERRORLOG_AREA        "/app/kCapErrLog.dat"

// Before Data Capture
#define BFR_CAPTURE_AREA         "/app/kBfrDataCap.dat"

#define MAXMID      200
#define MAXCID      200
#define MAXPUM      32

/************************/
#define MAX_BANK_LIST   32
#define MAX_BANK_CODE   64

/************************/
#define MAX_DONGLE_LEN  255
#define CONCERT_CNT 31     // ���޻� ����

//-----------------------
#define MAX_PDT    10     // �� ��ǰ�ڵ� ��ϰ���  ���� 
#define MAX_OIL_KIND	8 // ǰ�� �������� ��ǰ ���� 
#define MAX_LPG_KIND	5 // ǰ�� �������� ��ǰ ���� 
#define MAX_GENERAL_KIND	20 // ǰ�� �Ϲݻ�ǰ�� ��ǰ ���� 
#define MAX_FREEOIL_KIND	7 // ǰ�� �Ϲݻ�ǰ�� ��ǰ ���� 
// 20120529_PSH �鼼�� �߰�
#define MAX_FREEOIL_NEW_KIND	8 // �鼼��ǰ�� ��ǰ ���� 

/*----------------------*/
/* MAX HOT KEY LIST     */
#define MAX_HKEY    56     // �� ��Ű�� ����
#define MAX_VHKEY   54     // �� ��ȿ�� ��Ű�� ����

#define MAX_PNTPREFIX       10

/*----------------------*/
/* COMMLINE DEFINE      */
#define PSTN_LINE        0x01
#define C2I_LINE         0x02
#define CDMA_LINE        0x04
#define CHECK_LINE       0x08
#define WIBRO_LINE       0x10
#define ETHERNET_LINE    0x20
#define ETHMODEM_LINE    0x40
//#define WCDMA_LINE       0x80 

// Ethernet
#define DNS_Server_01        "168.126.63.1"
#define DNS_Server_02        "168.126.63.2"

/*----------------------*/
/* keyin function defines */
//#define  MAX_COLUMN   64
#define  MAX_COLUMN   96 
#define  MS_CARD      1
#define  KEY_BOARD    2
#define  FORWARD_DIRECT  4
#define  BACKWARD_DIRECT  8
#define  ACT_FLAG     16
#define  S_FLAG       32
#define  ALL_CHAR     64
#define  DOT_CHAR     128
#define  PULL_DOWN    256
#define  STARTED      512
#define  ICC_CARD     1024
#define  SHARP_CHAR   2048
#define  CASH_CHAR	  4096 // ���ڼ��� ���� ��ȣ�� ǥ���� �� �ְ� �ϱ� ����. 
#define  KEY_HIT_DISP	8192


#define  STRIPE          0x80
#define  F_CONTINUE      0x81
#define  REJECT          0x82
#define  DONGLE_READ     0x83
#define  ICC_INSERTED    0x84

#define  CLEAR_PIN   0x00
#define  DES_PIN     0x01
#define  SEED_PIN    0x02

/*----------------------*/
/* check reader         */
#define  CHECKREAD_ERR   0x85
#define  CHECKREAD_OK    0x86

#define  HNODIALTONE_STEP  1
#define  HRECONNECT_STEP   2
#define  HREDIAL_STEP      5
#define  HDIAL_READY       8
#define  HDIAL_STEP        10
#define  HENQ_STEP         15
#define  HNSEND_STEP       20
#define  HNEXT_STEP        21
#define  HSTX_STEP         25
#define  HSOH_STEP         26
#define  HETX_STEP         30
#define  HCRC_STEP         40
#define  HEOT_STEP         50
#define  HCONNECT_FAIL     96
#define  HUBREAK_STEP      97
#define  HEOT_RECEIVED     100
#define  HEOT_FAIL         101

#define OK_BEEP			    beep(7,2)
#define FAIL_BEEP			beep(7,5)
#define ERR_BEEP    		beep(7,7)
#define MODEM_LOFF          onlineled1_off()
#define MODEM_LON           onlineled1_on()
#define COM_LOFF            onlineled2_off()
#define COM_LON             onlineled2_on()
#define KEYLEDON_1          keyled_on()
#define KEYLEDOFF_1         keyled_off()
#define KEYLEDON_2          keyled2_on()
#define KEYLEDOFF_2         keyled2_off()
#define LCDLAMPON           lcd_backlight_on()
#define LCDLAMPOFF          lcd_backlight_off()
#define ICLEDON             icled_on()
#define ICLEDOFF            icled_off()

#define ALLON        1
#define ALLOFF       2
#define LEDON        3
#define LEDOFF       4
#define LCDON        5
#define LCDOFF       6
#define ICON         7
#define ICOFF        8
#define KEYON        9

// ������� �߿���� ���� ��ħ
#define dtimer1     1   // �Ϲ� delay
#define dtimer2     2   // HOST�� data send receive �� ���
#define dtimer3     3   // KeyInRes���� ���
#define dtimer4     4   // Signpad ���� ���
#define dtimer5     5   // CDMA
#define dtimer6     6   // EMV ����
#define dtimer7     7   // �ʱ���� ȭ��� timer
#define dtimer8     8    
#define dtimer9     9   // �������� ����
#define dtimer10    10  // LED ������
#define dtimer11    11  // Serial POS ������ Ÿ�̸� �������� �߰�, 20140226_KHD
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------
// kiosk define
//----------------------------------------------------------------------------------------------------------------------------------------------
#define SENDQ_ID 1000
#define RECVQ_ID (1000+1)

#define MSGQ_BUFF_SIZE	4096

typedef enum // command_type ����
{
	qLCD=1,
	qPRINTER,
	qAMOUNT_INPUT,
	qETHERNET,
	qMODEM,
	qMSR,
	qIC,
	qUSB,
	qLOGIC,
	qETC,
	qTEST,
	qTMS,
	qCOMMAND_TYPE_CNT // real count = qCOMMAND_TYPE_CNT - 1 // ������ 1���� �߱� ����
} MsgQueueCommandType;

typedef enum // TMS command_code ����
{
	qTMS_CC=0, 		// TMS command_code
	qTMS_UPDATED, 	// TMS command_code
	qTMS_FIRST, 		// TMS FIRST
	qTMS_CODE_CNT 	// real count = qTMS_CODE_CNT
} MsgQueueTMSCode;

typedef enum // TEST command_code ����
{
	qTEST_SOUND=0, 		// ���� �׽�Ʈ
	qTEST_TIME_SET,		// �ð� ����
	qTEST_USB, 			// USB �׽�Ʈ
	qTEST_MODEM,  			// �� �׽�Ʈ
	qTEST_BILLACCEPT,		// �����νı� �׽�Ʈ
	qTEST_LAN,				// �� �׽�Ʈ
	qTEST_PRINT,			// �μ� �׽�Ʈ
	qTEST_BARCODE,		// ��ǰ��ȣ ���ڵ� �Է�
	qTEST_TMS,				// TMS Download
	qTEST_TA_MODEM,		// ������ �� �׽�Ʈ
	qTEST_PRINT_SPEED,	// �μ�� �� �μ� �ӵ� �׽�Ʈ
	qTEST_TA_TEST,		// ���Ľ��� �׽�Ʈ��
	qTEST_USB_LOGOUT,		// USB�޸𸮷� �α� ���
	qTEST_CODE_CNT 		// real count = qETC_CODE_CNT
} MsgQueueTESTCode;

typedef enum // ETC command_code ����
{
	qREBOOT=0, // LOGIC -> UI
	qCANCEL,
	qINIT_MEMORY, // ��� �ʱ�ȭ
	qVARIABLE,   	// ���� ��û
	qTEST_ISSUE,	// �׽�Ʈ �߱�
	qVOLUME,		// ���� ����
	qTOUCH_SOUND,	// ��ġ�� ����
	qBACK_SOUND,	// ����� ����
	qLAN_SELECT,	// �� ����
	qLAN_IP_SET,	// �� ���� IP ����
	qSERVER_IP_SET,	// ���� IP/PORT ����
	qUSB_UPDATE,	// USB ������Ʈ
	qETC_CODE_CNT // real count = qETC_CODE_CNT
} MsgQueueETCCode;

typedef enum // LOGIC command_code ����
{
	qS900 = 0,		// ��ġ����
	qS901,			// ��ǰ�� �ǸŰ��� ���� üũ
	qS902,			// ��ǰ ��� üũ
	qS903,			// �߱� ��û
	qS905,			// �߱�/�ܰ� ���
	qS906,			// �������Ȳ ���, ���� ���μ��� �ܵ����� ����
	qS907,			// ���긶�� �ݾ� Ȯ��
	qS908,			// ���긶�� ó��
	qS909,			// ������ �α��� ����
	qS910,			// �κг� ��, ��������(�ӽð����ȣ ���)
	qS911,			// Kiosk ON/OFF
	qS912,			// H/W ������Ȳ ���
	qS913,			// ������������ ������
	qS914,			// ������ ������� �ܾ���ȸ
	//qS915,		// ���ݸ�����, ���� ���μ��� �ܵ����� ���� ��, ETC �̺�Ʈ�� UI�� ����
	qS921,			// ��ġȮ��
	qS922,			// ��ġ��û
	qS923,			// ��ġ�Ϸ� (��ġ����)
	qS941,			// ��ǰ�� ��ȯ�˻�
	qS942,			// ��ǰ�� ��ȯ��û
	qLOGIC_CODE_CNT // real count = qLOGIC_CODE_CNT 
} MsgQueueLogicCode;

typedef enum // AMOUNT command_code ����
{
	qBILL_COIN_ACCEPT=0, // UI -> LOGIC
	qBILLCOIN_CODE_CNT // real count = qBILLCOIN_CODE_CNT 
} MsgQueueBillCode;

typedef enum // PRINTER command_code ����
{
	qPRINT_START=0, 	// LOGIC -> UI
	qPRINT_CODE_CNT 	// real count = qPRINT_CODE_CNT 
} MsgQueuePrintCode;

typedef enum // ETHERNET command_code ����
{
	qETHERNET_CONNECT=0, 	// LOGIC -> UI
	qETHERNET_CODE_CNT 		// real count = qETHERNET_CODE_CNT 
} MsgQueueEthernetCode;

typedef enum // MODEM command_code ����
{
	qMODEM_CONNECT=0, 	// LOGIC -> UI
	qMODEM_CODE_CNT 		// real count = qMODEM_CODE_CNT 
} MsgQueueModemCode;

typedef enum // USB command_code ����
{
	qUSB_CHECK=0, 		// UI -> LOGIC
	qUSB_CODE_CNT 	// real count = qUSB_CODE_CNT 
} MsgQueueUsbCode;

typedef enum {
    REG_DOWNLOAD=0, 			// ��ġ���� (S900 <-> R900)
    SALE_ENABLE_CHECK,		// ��ǰ�� �Ǹ� ���ɿ��� üũ (S901 <-> R901)
    STOCK_CHECK,				// ��ǰ ��� üũ (S902 <-> R902)
    ISSUE_APP,					// �߱� ��û (S903 <-> R903)
    ISSUE_CAN,					// �߱�/�ܰ� ��� (S905 <-> R905)
    OPERATION_CHECK,			// ����� ��Ȳ��� (S906 <-> R906)
    ADJUST_CHECK,				// ���긶�� �ݾ� Ȯ�� (S907 <-> R907)
    ADJUST_APP,				// ���긶�� ó�� (S908 <-> R908)
    ADMIN_LOGIN_AUTH,			// ������ �α��� ���� (S909 <-> R909)
    TEMP_CANCEL,				// �κг� ��, �������� (�ӽð����ȣ ���) (S910 <-> R910)
    KIOSK_ONOFF,				// Ű����ũ ��/���� (S911 <-> R911)
    HW_ERROR_CHECK,			// H/W ������Ȳ ��� (S912 <-> R912)
    RESPONSE_RESEND,			// ������������ ������ (S913 <-> R913)
    VACCOUNT_BALANCE,			// ������ ������� �ܾ���ȸ (S914 <-> R914)
    REMOTE_REBOOT,			// ���ݸ����� (S915 <-> R915)
    PATCH_CHECK,				// ��ġȮ�� (S921 <-> R921)
    PATCH_REQUEST,			// ��ġ��û (S922 <-> R922)
    PATCH_COMPLETE,			// ��ġ�Ϸ� (S923 <-> R923)
    CHANGE_CHECK,				// ��ǰ�� ��ȯ�˻� (S941 <-> R941)
    CHANGE_APP,				// ��ǰ�� ��ȯ��û (S942 <-> R942)
    KIOSK_TOT_SERVICE_CNT	// Ű����ũ �ŷ��� �Ѱ���
} KioskServiceKind;

typedef byte (*KioskServiceFunc)(void);
//------------------------------------------------------------------------------------------------
// library���� ���ǵ� �͵�
//------------------------------------------------------------------------------------------------
#define UART0_PORT		0
#define UART1_PORT		1
#define UART2_PORT		2
#define UART3_PORT		3
#define LAN_PORT      	4


#define ICC_PORT			UART0_PORT		// IC ���
#define RF_PORT			UART1_PORT		// RF_MODULE
#define BARCODE_PORT		UART2_PORT		// ���ڵ�
#define PSTN_PORT			UART3_PORT		// ��
#define ETHERNET_PORT   	LAN_PORT

#if 0
#define CANCEL 		0x15
#else
#define STX             0x02
#define ETX             0x03
#define EOT             0x04
#define ENQ             0x05
#define EOM             0x19

#define ACK             0x06
#define CR              0x0d
#define LF              0x0a
#define TAB             0x09
#define FF              0x0c
#define SO              0x0e
#define SI              0x0f
#define DLE             0x10
#define NAK             0x15
#define ESC             0x1b
#define FS              0x1c
#define GS              0x1d
#define RS              0x1e
#define US              0x1f
//20120518_RDK
#define EM              0x19
/* End */
#endif
/////////////////////////////////////////////////////////
// libmodem.c
/////////////////////////////////////////////////////////
#define MD_OK           0x30
#define MD_CONNECT      0x31
#define MD_BUSY         0x32
#define MD_NOCARRIER    0x33
#define MD_NODIALTONE   0x34
#define MD_TIMEOUT      0x35
#define MD_CONN01       0x37

//20120518_RDK
#define MD_NOLINE       0x38

typedef struct
{
	int     int_cp_code;
    byte 	*str_cp_code;
	byte    *cp_code_msg;
} CP_CODE_LIST;

typedef struct
{
	int     int_trans_code;
    byte 	*str_trans_scode;
	byte    *str_trans_rcode;
} TRANS_CODE_LIST;
//------------------------------------------------------------------------------------------------
//#define SELF_DEBUG
#define REAL_LINE
#define LAN_SET_93
//------------------------------------------------------------------------------------------------


