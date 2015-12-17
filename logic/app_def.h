/*----------------------*/
/* CAPTURE DEFINE       */
/************************/
#ifdef _APP_DEF_
#else
#define _APP_DEF_
//#define PRT_SPEED_TEST
 
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2014/11/13"	// v1.4 배포버전
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2014/11/28" 	// v1.5 tms 안되는 것 수정, 프린터 속도조절 테스트 추가, 에러코드 E000 -> 8000 로 변경 추가
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2014/12/09" 	// v1.6 프린터 드라이버 변경, 화면 하얀증상 수정, 아이템베이/매니아 취소 플로우 변경 추가
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2014/12/18" 	// 하단 그림인쇄 추가, 서버점검시 돈 먼저 투입경우 생기는 것 방지 추가, 망취소 추가
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2014/12/22" 	// 망취소 후 정산시 로직 버퍼 오류 수정, 정산 금액 틀려지는 오류 수정
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2014/12/23" 	// 발권시 하단 그림인쇄 추가
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2014/12/30" 	// 발권시 하단 그림인쇄 다른 그림으로 교체
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2015/01/02" 	// 발권시 하단 그림인쇄 다른 그림으로 교체, v2.0
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2015/01/06" 	// 패치확인 전문 요청시 1~3540초를 랜덤하게 더한 값으로 요청하도록 변경, 패치타임도 분초를 랜덤하게 대입하도록 변경, v2.1
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2015/01/08" 	// 구매취소 버튼 클릭시 화면 하얗게 나오는 증상/금액 없음 버튼 클릭시 하얗게 나오는 증상 수정, v2.2
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2015/01/08" 	// 전문암호화 적용, v2.3
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2015/02/10" 	// TMS가 인쇄시 적용 않되도록 수정, v2.3
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2015/02/10" 	// 에그머니 부분을 연속해서 누를 때, 이벤트가 연속해서 들어와서, UI의 hide가 두번 실행되서 멈추는 증상 수정, v2.4
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2015/03/09" 	// 암호화 적용버전, v2.5
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2015/03/17" 	// usb로 로그 출력 기능 추가, v2.5
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2015/03/18" 	// 풀 커팅 버전, v2.5
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2015/03/20" 	// 정상 로그 기록 추가, v2.5
//#define FIRMWARE_VER    "KIOSK 1010010000  DATE 2015/03/23" 	// SSH 22번 포트 열려있는 것 막기 추가, 양 제거, v2.6

#define PROGRAM_VERSION "2.6"	// 키오스크 버전

#define PROGRAM_DATE    "20150323" 
#define PRODUCT_CODE    "CA"			// 단말기종류 

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


// 신용/직불/현금 등의 DATA CAPTURE AREA
#define MAXCAP                   4096                           	// Record Size(256 byte) * 4096 = 0x100000 bytes = 1MBytes
#define CAP_RECORD_SIZE          256                             // record size 
#define ONE_ERASE_BLOCK          4096                           	// 한번에 지울 수 있는 블럭 사이즈
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
#define CONCERT_CNT 31     // 제휴사 개수

//-----------------------
#define MAX_PDT    10     // 총 상품코드 등록가능  개수 
#define MAX_OIL_KIND	8 // 품목 주유소의 상품 갯수 
#define MAX_LPG_KIND	5 // 품목 충전소의 상품 갯수 
#define MAX_GENERAL_KIND	20 // 품목 일반상품의 상품 갯수 
#define MAX_FREEOIL_KIND	7 // 품목 일반상품의 상품 갯수 
// 20120529_PSH 면세유 추가
#define MAX_FREEOIL_NEW_KIND	8 // 면세유품목 상품 갯수 

/*----------------------*/
/* MAX HOT KEY LIST     */
#define MAX_HKEY    56     // 총 핫키의 개수
#define MAX_VHKEY   54     // 총 유효한 핫키의 개수

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
#define  CASH_CHAR	  4096 // 전자세금 계산시 부호를 표시할 수 있게 하기 위함. 
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

// 수정요망 펌웨어와 변수 겹침
#define dtimer1     1   // 일반 delay
#define dtimer2     2   // HOST로 data send receive 시 사용
#define dtimer3     3   // KeyInRes에서 사용
#define dtimer4     4   // Signpad 에서 사용
#define dtimer5     5   // CDMA
#define dtimer6     6   // EMV 전용
#define dtimer7     7   // 초기상태 화면용 timer
#define dtimer8     8    
#define dtimer9     9   // 포스관련 전용
#define dtimer10    10  // LED 끄기기능
#define dtimer11    11  // Serial POS 스레드 타이머 전용으로 추가, 20140226_KHD
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------
// kiosk define
//----------------------------------------------------------------------------------------------------------------------------------------------
#define SENDQ_ID 1000
#define RECVQ_ID (1000+1)

#define MSGQ_BUFF_SIZE	4096

typedef enum // command_type 정의
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
	qCOMMAND_TYPE_CNT // real count = qCOMMAND_TYPE_CNT - 1 // 시작을 1부터 했기 때문
} MsgQueueCommandType;

typedef enum // TMS command_code 정의
{
	qTMS_CC=0, 		// TMS command_code
	qTMS_UPDATED, 	// TMS command_code
	qTMS_FIRST, 		// TMS FIRST
	qTMS_CODE_CNT 	// real count = qTMS_CODE_CNT
} MsgQueueTMSCode;

typedef enum // TEST command_code 정의
{
	qTEST_SOUND=0, 		// 사운드 테스트
	qTEST_TIME_SET,		// 시간 설정
	qTEST_USB, 			// USB 테스트
	qTEST_MODEM,  			// 모뎀 테스트
	qTEST_BILLACCEPT,		// 지폐인식기 테스트
	qTEST_LAN,				// 랜 테스트
	qTEST_PRINT,			// 인쇄 테스트
	qTEST_BARCODE,		// 제품번호 바코드 입력
	qTEST_TMS,				// TMS Download
	qTEST_TA_MODEM,		// 인증용 모뎀 테스트
	qTEST_PRINT_SPEED,	// 인쇄농도 및 인쇄 속도 테스트
	qTEST_TA_TEST,		// 형식승인 테스트용
	qTEST_USB_LOGOUT,		// USB메모리로 로그 출력
	qTEST_CODE_CNT 		// real count = qETC_CODE_CNT
} MsgQueueTESTCode;

typedef enum // ETC command_code 정의
{
	qREBOOT=0, // LOGIC -> UI
	qCANCEL,
	qINIT_MEMORY, // 기기 초기화
	qVARIABLE,   	// 변수 요청
	qTEST_ISSUE,	// 테스트 발권
	qVOLUME,		// 볼륨 조정
	qTOUCH_SOUND,	// 터치음 설정
	qBACK_SOUND,	// 배경음 설정
	qLAN_SELECT,	// 랜 설정
	qLAN_IP_SET,	// 랜 고정 IP 설정
	qSERVER_IP_SET,	// 서버 IP/PORT 설정
	qUSB_UPDATE,	// USB 업데이트
	qETC_CODE_CNT // real count = qETC_CODE_CNT
} MsgQueueETCCode;

typedef enum // LOGIC command_code 정의
{
	qS900 = 0,		// 설치인증
	qS901,			// 상품별 판매가능 여부 체크
	qS902,			// 상품 재고 체크
	qS903,			// 발권 요청
	qS905,			// 발권/단건 취소
	qS906,			// 운영오류상황 기록, 로직 프로세스 단독으로 실행
	qS907,			// 정산마감 금액 확인
	qS908,			// 정산마감 처리
	qS909,			// 관리자 로그인 인증
	qS910,			// 부분납 후, 강제종료(임시결재번호 취소)
	qS911,			// Kiosk ON/OFF
	qS912,			// H/W 오류상황 기록
	qS913,			// 서버응답전문 재전송
	qS914,			// 가맹점 가상계좌 잔액조회
	//qS915,		// 원격리부팅, 로직 프로세스 단독으로 실행 후, ETC 이벤트로 UI에 전달
	qS921,			// 패치확인
	qS922,			// 패치요청
	qS923,			// 패치완료 (패치시작)
	qS941,			// 상품권 교환검사
	qS942,			// 상품권 교환요청
	qLOGIC_CODE_CNT // real count = qLOGIC_CODE_CNT 
} MsgQueueLogicCode;

typedef enum // AMOUNT command_code 정의
{
	qBILL_COIN_ACCEPT=0, // UI -> LOGIC
	qBILLCOIN_CODE_CNT // real count = qBILLCOIN_CODE_CNT 
} MsgQueueBillCode;

typedef enum // PRINTER command_code 정의
{
	qPRINT_START=0, 	// LOGIC -> UI
	qPRINT_CODE_CNT 	// real count = qPRINT_CODE_CNT 
} MsgQueuePrintCode;

typedef enum // ETHERNET command_code 정의
{
	qETHERNET_CONNECT=0, 	// LOGIC -> UI
	qETHERNET_CODE_CNT 		// real count = qETHERNET_CODE_CNT 
} MsgQueueEthernetCode;

typedef enum // MODEM command_code 정의
{
	qMODEM_CONNECT=0, 	// LOGIC -> UI
	qMODEM_CODE_CNT 		// real count = qMODEM_CODE_CNT 
} MsgQueueModemCode;

typedef enum // USB command_code 정의
{
	qUSB_CHECK=0, 		// UI -> LOGIC
	qUSB_CODE_CNT 	// real count = qUSB_CODE_CNT 
} MsgQueueUsbCode;

typedef enum {
    REG_DOWNLOAD=0, 			// 설치인증 (S900 <-> R900)
    SALE_ENABLE_CHECK,		// 상품별 판매 가능여부 체크 (S901 <-> R901)
    STOCK_CHECK,				// 상품 재고 체크 (S902 <-> R902)
    ISSUE_APP,					// 발권 요청 (S903 <-> R903)
    ISSUE_CAN,					// 발권/단건 취소 (S905 <-> R905)
    OPERATION_CHECK,			// 운영오류 상황기록 (S906 <-> R906)
    ADJUST_CHECK,				// 정산마감 금액 확인 (S907 <-> R907)
    ADJUST_APP,				// 정산마감 처리 (S908 <-> R908)
    ADMIN_LOGIN_AUTH,			// 관리자 로그인 인증 (S909 <-> R909)
    TEMP_CANCEL,				// 부분납 후, 강제종료 (임시결재번호 취소) (S910 <-> R910)
    KIOSK_ONOFF,				// 키오스크 온/오프 (S911 <-> R911)
    HW_ERROR_CHECK,			// H/W 오류상황 기록 (S912 <-> R912)
    RESPONSE_RESEND,			// 서버응답전문 재전송 (S913 <-> R913)
    VACCOUNT_BALANCE,			// 가맹점 가상계좌 잔액조회 (S914 <-> R914)
    REMOTE_REBOOT,			// 원격리부팅 (S915 <-> R915)
    PATCH_CHECK,				// 패치확인 (S921 <-> R921)
    PATCH_REQUEST,			// 패치요청 (S922 <-> R922)
    PATCH_COMPLETE,			// 패치완료 (S923 <-> R923)
    CHANGE_CHECK,				// 상품권 교환검사 (S941 <-> R941)
    CHANGE_APP,				// 상품권 교환요청 (S942 <-> R942)
    KIOSK_TOT_SERVICE_CNT	// 키오스크 거래의 총갯수
} KioskServiceKind;

typedef byte (*KioskServiceFunc)(void);
//------------------------------------------------------------------------------------------------
// library에서 정의된 것들
//------------------------------------------------------------------------------------------------
#define UART0_PORT		0
#define UART1_PORT		1
#define UART2_PORT		2
#define UART3_PORT		3
#define LAN_PORT      	4


#define ICC_PORT			UART0_PORT		// IC 모듈
#define RF_PORT			UART1_PORT		// RF_MODULE
#define BARCODE_PORT		UART2_PORT		// 바코드
#define PSTN_PORT			UART3_PORT		// 모뎀
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


