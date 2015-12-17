//    화일명 :    PUB.H
// DUMY
HNX720T_EXT byte dumy01[1024+1];                   // DUMY0A BUFFER
HNX720T_EXT byte dumy02[MAXMID*22];                // DUMY02 BUFFER 3(신용)
HNX720T_EXT byte dumy03[MAXMID*22];                // DUMY03 BUFFER 4(BACKUP)


/////////////////////////////////////////////////////////////////////////////////
// 이주석 위의 변수는 비휘발성 변수들입니다.
// 거래시 지워져도 될 휘발성 변수들은 다음부터 기술 해야합니다.
/////////////////////////////////////////////////////////////////////////////////

HNX720T_EXT word slen, rlen;                       // SEND, RECEIVE COUNTER
HNX720T_EXT byte sendbuf[4096];                    // SEND DATA BUFFER
HNX720T_EXT byte recvbuf[((4096/16)+2)*32+1];     // RECEIVE DATA BUFFER
HNX720T_EXT byte cyper_tmp_buf[((4096/16)+2)*32+1];     		// cyper temp buffer
HNX720T_EXT byte tempbuf[1024+1];                     // gloval temp bufferbyte
HNX720T_EXT byte tmbuf[512];                       // gloval temp bufferbyte
HNX720T_EXT byte *p_enc_buf;                       // encrypt buffer pointer
HNX720T_EXT byte *p_dec_buf;                       // decrypt buffer pointer
HNX720T_EXT int enc_send_len;                       // encrypt send length
HNX720T_EXT int enc_recv_len;                       // encrypt recv length

// GENERAL BUFFER
HNX720T_EXT byte TimeStamp[16+1];
HNX720T_EXT byte inbuf[128];                       // gloval input buffer
HNX720T_EXT byte intemp[128];                      // gloval input buffer
HNX720T_EXT byte scenbuf[512];                     // gloval scenario buffer
HNX720T_EXT byte mscenbuf[512];                    // gloval MESSAGE scenario buffer
HNX720T_EXT byte Mask[MAX_COLUMN+1];               // gloval masking buffer
HNX720T_EXT byte kinbuf1[MAX_COLUMN+1];            // buffer01 for KeyIn Function
HNX720T_EXT byte kinbuf2[MAX_COLUMN+1];            // buffer02 for KeyIn Function
HNX720T_EXT int  SERVKIND;                         // SERVICEKIND
HNX720T_EXT byte Hret, Iret;                       // 
HNX720T_EXT byte dispcnt;
HNX720T_EXT byte formstr1[64+1], formstr2[64+1];
HNX720T_EXT byte encryptdata[32+1];
HNX720T_EXT byte printsign[64/8][128];// 인쇄용 서명 data
HNX720T_EXT byte buf2K[2048+3];                    
HNX720T_EXT byte LOGO_DISP_ON;						
HNX720T_EXT byte SIGN_TYPE;							

// MSGHNDLR VARIBALES
HNX720T_EXT u8 dialcnt, Predial, HostConnected;
HNX720T_EXT int linetype;
HNX720T_EXT int socketfd;
HNX720T_EXT int link_fd;

HNX720T_EXT byte debug_mod;
HNX720T_EXT int  debuglen;
HNX720T_EXT byte debugbuf[512+1];

//HNX720T_EXT int lcdfd, msrfd, buzzerfd, keyfd, printfd, timerfd, ledfd, rtcfd, cashfd, iccdevfd, modemdevfd, muxfd;
HNX720T_EXT int lcdfd, msrfd, buzzerfd, keyfd, ledfd, rtcfd, cashfd, iccdevfd, modemdevfd, muxfd;

extern const byte TERM_MASTER_KEY_pdevice[16];
extern const byte TERM_MASTER_KEY[16];

HNX720T_EXT byte c_cap1[MAXCAP*CAP_RECORD_SIZE];    // 집계영역
HNX720T_EXT byte *capture;

//----------------------------------------------------------------------------------
// KIOSK Program varialbes, 20140701_KHD
// GENERAL BUFFER
//----------------------------------------------------------------------------------
HNX720T_EXT int  KIOSK_SERVKIND;		// SERVICEKIND
HNX720T_EXT byte KIOSK_STEP;          // 
HNX720T_EXT byte KIOSK_NEXTSTEP;     // 
//----------------------------------------------------------------------------------------------
typedef struct // TMS 시작 요청 (UI->LOGIC)
{
	int command_type; // qTMS
	int command_code; // qTMS_FIRST
} __attribute__((packed)) t_tms_first_ask;
//----------------------------------------------------------------------------------------------
typedef struct // TMS 시작 이벤트 요청 (LOGIC->UI)
{
	int command_type; // qTMS
	int command_code; // qTMS_CC
} __attribute__((packed)) t_tms_start_ask;
//----------------------------------------------------------------------------------------------
typedef struct // TMS 시작 ui 응답 (UI->LOGIC)
{
	int command_type; // qTMS
	int command_code; // qTMS_CC
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_tms_ui_response;
//----------------------------------------------------------------------------------------------
typedef struct // TMS logic 응답 (LOGIC->UI)
{
	int command_type; // qTMS
	int command_code; // qTMS_UPDATED
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_tms_logic_updated_response;
//----------------------------------------------------------------------------------------------
typedef struct // TA 인증용 테스트 요청 (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_TA_TEST
	struct
	{
		int command; // 0:start, 1:stop
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_test_ask_ta_test;
//----------------------------------------------------------------------------------------------
typedef struct // USB로 로그 출력 요청(UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_USB_LOGOUT
} __attribute__((packed)) t_test_ask_usb_logout;
//----------------------------------------------------------------------------------------------
typedef struct // USB로 로그 출력 응답(LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_USB_LOGOUT
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_test_response_usb_logout;
//----------------------------------------------------------------------------------------------
typedef struct // Type Approval Modem 테스트 요청 (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_MODEM
	struct
	{
		int command; // off-hook:10, on-hook:20, data send:30, dtmf test:0~9, '*', '#'
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_test_ask_ta_modem;
//----------------------------------------------------------------------------------------------
typedef struct // Type Approval Modem 테스트 응답 (LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_MODEM
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_test_response_ta_modem;
//----------------------------------------------------------------------------------------------
typedef struct // 인쇄농도 설정 테스트 요청 (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_PRINT_SPEED
	struct
	{
		int speed; // 0~100, 후반 가속 default 80, set_speed
		int speed2; // 0~100, 전반 가속 default 40, set_step
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_test_ask_print_speed;
//----------------------------------------------------------------------------------------------
typedef struct // 인쇄농도 설정 테스트 응답 (LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_PRINT_SPEED
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_test_response_print_speed;
//----------------------------------------------------------------------------------------------
typedef struct // SOUND 테스트 요청 (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_SOUND
} __attribute__((packed)) t_test_ask_sound;
//----------------------------------------------------------------------------------------------
typedef struct // SOUND 테스트 응답 (LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_SOUND
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_test_response_sound;
//----------------------------------------------------------------------------------------------
typedef struct // TIME_SET 요청 (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_TIME_SET
	struct
	{
		unsigned char yymmddhhmmss[12+1]; // yymmddhhmmss + null 형식
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_test_ask_time_set;
//----------------------------------------------------------------------------------------------
typedef struct // TIME_SET 응답 (LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_TIME_SET
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_test_response_time_set;
//----------------------------------------------------------------------------------------------
typedef struct // USB_TEST 요청 (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_USB
} __attribute__((packed)) t_test_ask_usb_test;
//----------------------------------------------------------------------------------------------
typedef struct // USB_TEST 응답 (LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_USB
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_test_response_usb_test;
//----------------------------------------------------------------------------------------------
typedef struct // MODEM_TEST 요청 (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_MODEM
} __attribute__((packed)) t_test_ask_modem_test;
//----------------------------------------------------------------------------------------------
typedef struct // MODEM_TEST 응답 (LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_MODEM
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_test_response_modem_test;
//----------------------------------------------------------------------------------------------
typedef struct // BILLACCEPT_TEST 요청 (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_BILLACCEPT
	struct
	{
		int target_amount; // 입력 목표 금액
		int wait_time; // 금액 입력 대기시간
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_test_ask_billaccept_test;
//----------------------------------------------------------------------------------------------
typedef struct // BILLACCEPT_TEST 응답 (LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_BILLACCEPT
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_test_response_billaccept_test;
//----------------------------------------------------------------------------------------------
typedef struct // LAN_TEST 요청 (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_LAN
} __attribute__((packed)) t_test_ask_lan_test;
//----------------------------------------------------------------------------------------------
typedef struct // LAN_TEST 응답 (LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_LAN
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_test_response_lan_test;
//----------------------------------------------------------------------------------------------
typedef struct // PRINT_TEST 요청 (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_PRINT
} __attribute__((packed)) t_test_ask_print_test;
//----------------------------------------------------------------------------------------------
typedef struct // PRINT_TEST 응답 (LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_PRINT
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_test_response_print_test;
//----------------------------------------------------------------------------------------------
typedef struct // BARCORD 요청 (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_BARCODE
} __attribute__((packed)) t_test_ask_barcode_test;
//----------------------------------------------------------------------------------------------
typedef struct // PRINT_TEST 응답 (LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_BARCODE
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_test_response_barcode_test;
//----------------------------------------------------------------------------------------------
typedef struct // TMS 요청 (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_TMS
} __attribute__((packed)) t_test_ask_tms_test;
//----------------------------------------------------------------------------------------------
typedef struct // PRINT_TEST 응답 (LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_TMS
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_test_response_tms_test;
//----------------------------------------------------------------------------------------------
typedef struct // Reboot 이벤트 데이터 (LOGIC->UI)
{
	int command_type; // ETC
	int command_code; // qREBOOT
} __attribute__((packed)) t_etc_ask_reboot;
//----------------------------------------------------------------------------------------------
typedef struct // Cancel 요청데이터 (UI->LOGIC)
{
	int command_type; // ETC
	int command_code; // qREBOOT
} __attribute__((packed)) t_etc_ask_cancel;
//----------------------------------------------------------------------------------------------
typedef struct // Cancel 응답데이터 (LOGIC->UI)
{
	int command_type; // ETC
	int command_code; // qREBOOT
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_etc_response_cancel;
//----------------------------------------------------------------------------------------------
typedef struct // 메모리 초기화 요청데이터 (UI->LOGIC)
{
	int command_type; // ETC
	int command_code; // qINIT_MEMORY
} __attribute__((packed)) t_etc_ask_init_memory;
//----------------------------------------------------------------------------------------------
typedef struct // 메모리 초기화 응답데이터 (LOGIC->UI)
{
	int command_type; // ETC
	int command_code; // qINIT_MEMORY
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_etc_response_init_memory;
//----------------------------------------------------------------------------------------------
typedef struct // 설정변수 요청데이터 (UI->LOGIC)
{
	int command_type; // ETC
	int command_code; // qVARIABLE
} __attribute__((packed)) t_etc_ask_variable;
//----------------------------------------------------------------------------------------------
typedef struct // 설정변수 응답데이터 (LOGIC->UI)
{
	int command_type; // ETC
	int command_code; // qVARIABLE
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		unsigned char registration; // 단말기 설치인증여부 (1설치인증됨, 0설치인증 않됨, 디폴트는 0)
		unsigned char vaccount_enable; // 가상계좌 사용여부 (1사용, 0미사용, 디폴트는 0)
		unsigned char background_sound; // 배경음 사용여부 (1사용, 0미사용, 디폴트는 1)
		unsigned char touch_sound; // 터치음 사용여부 (1사용, 0미사용, 디폴트는 1)
		unsigned char volume; // 현재 볼륨값 (1~127, 가용범위는 70~127, 디폴트는 120)
		unsigned char dynamic_ip_use; // 유동IP 사용여부 (1사용, 0미사용, 디폴트는 0)
		unsigned char kiosk_ip[16]; // 키오스크 IP
		unsigned char kiosk_sm[16]; // 키오스크 서브넷마스크
		unsigned char kiosk_gw[16]; // 키오스크 게이트웨이
		unsigned char server_ip[16]; // 접속 서버 IP
		unsigned char server_port[10]; // 접속 서버 포트
		unsigned char kiosk_version[10]; // 1.0.0
		unsigned char kiosk_yymmddhhmmss[13]; // 키오스크 시스템시간
		unsigned char kiosk_mac[18];
		unsigned char kiosk_id[11];
		unsigned char shop_id[11];
		int print_speed;
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_etc_response_variable;
//----------------------------------------------------------------------------------------------
typedef struct // 테스트 발권 요청데이터 (UI->LOGIC)
{
	int command_type; // ETC
	int command_code; // qTEST_ISSUE
} __attribute__((packed)) t_etc_ask_test_issue;
//----------------------------------------------------------------------------------------------
typedef struct // 테스트 발권 응답데이터 (LOGIC->UI)
{
	int command_type; // ETC
	int command_code; // qTEST_ISSUE
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_etc_response_test_issue;
//----------------------------------------------------------------------------------------------
typedef struct // 볼륨조정 요청데이터 (UI->LOGIC)
{
	int command_type; // ETC
	int command_code; // qVOLUME
	struct
	{
		unsigned char vol; // 볼륨값 (1~127, 가용범위는 70~127, 디폴트는 120)
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_etc_ask_set_volume;
//----------------------------------------------------------------------------------------------
typedef struct // 볼륨조정 응답데이터 (LOGIC->UI)
{
	int command_type; // ETC
	int command_code; // qVOLUME
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_etc_response_set_volume;
//----------------------------------------------------------------------------------------------
typedef struct // 터치음설정 요청데이터 (UI->LOGIC)
{
	int command_type; // ETC
	int command_code; // qTOUCH_SOUND
	struct
	{
		unsigned char tsound; // 1:사용, 0:미사용
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_etc_ask_set_tsound;
//----------------------------------------------------------------------------------------------
typedef struct // 터치음설정 응답데이터 (LOGIC->UI)
{
	int command_type; // ETC
	int command_code; // qTOUCH_SOUND
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_etc_response_set_tsound;
//----------------------------------------------------------------------------------------------
typedef struct // 배경음설정 요청데이터 (UI->LOGIC)
{
	int command_type; // ETC
	int command_code; // qBACK_SOUND
	struct
	{
		unsigned char bsound; // 1:사용, 0:미사용
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_etc_ask_set_bsound;
//----------------------------------------------------------------------------------------------
typedef struct // 배경음설정 응답데이터 (LOGIC->UI)
{
	int command_type; // ETC
	int command_code; // qBACK_SOUND
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_etc_response_set_bsound;
//----------------------------------------------------------------------------------------------
typedef struct // 랜타입 설정 요청데이터 (UI->LOGIC)
{
	int command_type; // ETC
	int command_code; // qLAN_SELECT
	struct
	{
		unsigned char lantype; // 1:유동, 0:고정
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_etc_ask_set_lantype;
//----------------------------------------------------------------------------------------------
typedef struct // 랜타입 설정 응답데이터 (LOGIC->UI)
{
	int command_type; // ETC
	int command_code; // qLAN_SELECT
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_etc_response_set_lantype;
//----------------------------------------------------------------------------------------------
typedef struct // 고정IP 설정 요청데이터 (UI->LOGIC)
{
	int command_type; // ETC
	int command_code; // qLAN_IP_SET
	struct
	{
		unsigned char kiosk_ip[16]; // 키오스크 IP
		unsigned char kiosk_sm[16]; // 키오스크 서브넷마스크
		unsigned char kiosk_gw[16]; // 키오스크 게이트웨이
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_etc_ask_set_lanip;
//----------------------------------------------------------------------------------------------
typedef struct // 고정IP 설정 응답데이터 (LOGIC->UI)
{
	int command_type; // ETC
	int command_code; // qLAN_IP_SET
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_etc_response_set_lanip;
//----------------------------------------------------------------------------------------------
typedef struct // 서버IP/PORT 설정 요청데이터 (UI->LOGIC)
{
	int command_type; // ETC
	int command_code; // qSERVER_IP_SET
	struct
	{
		unsigned char server_ip[16]; // 서버 접속 IP
		unsigned char server_port[10]; // 서버 접속 PORT
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_etc_ask_set_serverip;
//----------------------------------------------------------------------------------------------
typedef struct // 서버IP/PORT 설정 응답데이터 (LOGIC->UI)
{
	int command_type; // ETC
	int command_code; // qSERVER_IP_SET
	int response_code; // 0:정상, 이외는 응답결과코드 참조
} __attribute__((packed)) t_etc_response_set_serverip;
//----------------------------------------------------------------------------------------------
typedef struct // S900 요청데이터 (UI->LOGIC) : 수신큐 <-- 설치인증
{
	int command_type; // LOGIC
	int command_code; // qS900
	struct
	{
		unsigned char shop_serial_num[10];
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s900_ask;

typedef struct // S900 응답데이터 (LOGIC->UI) : 송신큐 <-- 설치인증
{
	int command_type; // LOGIC
	int command_code; // qS900
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		unsigned char message[50]; // 응답코드가 오류인 경우, 오류 상세 설명
		unsigned char shop_name[50]; // 가맹점 이름
		unsigned char vaccount_enable; // 가상계좌 사용여부, 미사용: 0, 사용: 1
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s900_response;

typedef struct // S901 요청데이터 (UI->LOGIC) : 수신큐 <-- 상품별 판매가능 여부 체크
{
	int command_type; // LOGIC
	int command_code; // qS901
	struct
	{
		int cp_code; // 퍼니카드, 해피머니, 틴캐시 등등
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s901_ask;

typedef struct // S901 응답데이터 (LOGIC->UI) : 송신큐 <-- 상품별 판매가능 여부 체크
{
	int command_type; // LOGIC
	int command_code; // qS901
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		unsigned char message[50]; // 응답코드가 오류인 경우, 오류 상세 설명
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s901_response;

typedef struct // S902 요청데이터 (UI->LOGIC) : 수신큐 <-- 상품 재고 체크
{
	int command_type; // LOGIC
	int command_code; // qS902
	struct
	{
		int cp_code; // 퍼니카드, 해피머니, 틴캐시 등등
		int cp_count; // 총발권 매수, 1~9
		int cp_amount; // 발권 낱장 금액
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s902_ask;

typedef struct // S902 응답데이터 (LOGIC->UI) : 송신큐 <-- 상품 재고 체크
{
	int command_type; // LOGIC
	int command_code; // qS902
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		unsigned char message[50]; // 임시 그룹과금번호 혹은 오류메시지
		int enable_count; // 발권 가능 매수 (최대 9매), 1~9
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s902_response;

typedef struct // S903 요청데이터 (UI->LOGIC) : 수신큐 <-- 발권 요청
{
	int command_type; // LOGIC
	int command_code; // qS903
	struct
	{
		int approval_type; // 현금=3, RF=4, 신용카드=5
		int cp_code; // 퍼니카드, 해피머니, 틴캐시 등등
		int cp_amount; // 발권 낱장 금액
		int cp_count; // 총발권 매수
		int amount; // 거래 총금액 = cp_amount * cp_count
		int ticket_type; // 발권=‘1’, SMS='2'(1장일때만 가능)
		byte ticket_user_number[16];
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s903_ask;

typedef struct // S903 응답데이터 (LOGIC->UI) : 송신큐 <-- 발권 요청
{
	int command_type; // LOGIC
	int command_code; // qS903
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		int cp_code; // 퍼니카드, 해피머니, 틴캐시 등등
		int cp_count; // 총발권 매수 (최대 9매)
		unsigned char event_msg[1000];
		unsigned char event_msg_position; // 상:'1', 하:‘2’
		int vaccount_balance; // 가상계좌 잔액
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s903_response;

typedef struct // S905 요청데이터 (UI->LOGIC) : 수신큐 <-- 발권/단건 취소
{
	int command_type; // LOGIC
	int command_code; // qS905
	struct
	{
		unsigned char cancel_type; // 취소유형, 관리자 발권취소=‘T', 관리자 단건취소='P', 고객 단건취소=‘C'
		int cp_code; // 퍼니카드, 해피머니, 틴캐시 등등
		unsigned char issue_date[8]; // 발행일자, yyyyMMdd, 단건취소에만
		unsigned char manage_num[24]; // 관리번호, 모두 스페이스로
		unsigned char cancel_num[20]; // 취소번호, UI 입력된 번호 8자리
		int cp_amount; // 액면가
		int cancel_amount; // 발권 취소 금액, 취소로 선수금 처리된 금액 (‘T'는 전체금액, 'P'/'C'는 액면금액)
		unsigned char client_phone[16]; // 고객연락처, 고객 단건 취소시 환불용
		unsigned char client_bank_name[20]; // 은행이름
		unsigned char client_account[20]; // 은행계좌번호, 고객 단건 취소시 환불용
		unsigned char client_name[24]; // 은행예금주명
		int error_code; // 오류 구분코드, 1:사용자취소, 2:망취소, 3:용지걸림, 4:용지부족, 7:핀번호 수신실패, 20:기타H/W오류
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s905_ask;

typedef struct // S905 응답데이터 (LOGIC->UI) : 송신큐 <-- 발권/단건 취소
{
	int command_type; // LOGIC
	int command_code; // qS905
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		int cancel_ok_count; // 발권취소 성공매수
		int cancel_tcount; 	// 발권취소 전체매수
		int vaccount_balance; // 가상계좌 잔액
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s905_response;

typedef struct // S906 요청데이터 (UI->LOGIC) : 수신큐 <-- 운영오류상황 기록
{
	int command_type; // LOGIC
	int command_code; // qS906
	struct
	{
		unsigned char error_code; // 8:고객 임의 지원요청
		unsigned char client_num[21]; // 고객연락처
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s906_ask;

typedef struct // S906 응답데이터 (LOGIC->UI) : 송신큐 <-- 운영오류상황 기록
{
	int command_type; // LOGIC
	int command_code; // qS906
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		unsigned char message[50]; // 응답코드가 오류인 경우, 오류 상세 설명
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s906_response;

typedef struct // S907 요청데이터 (UI->LOGIC) : 수신큐 <-- 정산마감 금액 확인
{
	int command_type; // LOGIC
	int command_code; // qS907
} __attribute__((packed)) t_s907_ask;

typedef struct // S907 응답데이터 (LOGIC->UI) : 송신큐 <-- 정산마감 금액 확인
{
	int command_type; // LOGIC
	int command_code; // qS907
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		unsigned char vaccount_inform[50]; // 단말기에 부여된 가상계좌번호 또는 오류메시지
		unsigned char reference_time[19]; // 정산마감 기준시간, YYYY-MM-DDHH24:MI:SS 형태
		int not_magam_total_amount; 		// 기기내 총금액
		int not_magam_sale_amount; 		// 정상판매금액
		int not_magam_part_amount;		// 부분납금액
		int not_magam_approval_amount; 	// 총 승인금액
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s907_response;

typedef struct // S908 요청데이터 (UI->LOGIC) : 수신큐 <-- 정산마감 처리
{
	int command_type; // LOGIC
	int command_code; // qS908
} __attribute__((packed)) t_s908_ask;

typedef struct // S908 응답데이터 (LOGIC->UI) : 송신큐 <-- 정산마감 처리
{
	int command_type; // LOGIC
	int command_code; // qS908
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		unsigned char vaccount_inform[50]; // 단말기에 부여된 가상계좌번호 또는 오류메시지
		unsigned char reference_time[19]; // 정산마감 기준시간, YYYY-MM-DDHH24:MI:SS 형태
		unsigned char old_reference_time[19]; // 이전 정산마감시간, YYYY-MM-DDHH24:MI:SS
		int magam_total_amount; 	// 마지막 정산마감 요청 후부터 현재까지의 입금된
									// 금액의 합계(미정산액)
		int magam_sale_amount; 	// 판매금총액(정상발권금액+발권취소금액)
		int magam_cancel_amount;	// 취소금액
		int magam_part_cancel_amount;	// 정산 부분납 취소금액
		int magam_after_cancel_amount;	// 정산마감후 취소금액
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s908_response;

typedef struct // S909 요청데이터 (UI->LOGIC) : 수신큐 <-- 관리자 로그인 인증
{
	int command_type; // LOGIC
	int command_code; // qS909
	struct
	{
		unsigned char admin_password[6];	// 관리자 비밀번호 (4자리 이상)
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s909_ask;

typedef struct // S909 응답데이터 (LOGIC->UI) : 송신큐 <-- 관리자 로그인 인증
{
	int command_type; // LOGIC
	int command_code; // qS909
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		unsigned char message[50]; // 응답코드가 오류인 경우, 오류 상세 설명
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s909_response;

typedef struct // S910 요청데이터 (UI->LOGIC) : 수신큐 <-- 부분납 후, 강제종료 (임시결재번호 취소)
{
	int command_type; // LOGIC
	int command_code; // qS910
	struct
	{
		int part_amount; // 취소로 선수금 처리하는 (부분납)금액
		unsigned char user_information[100]; // 연락처,은행이름,계좌번호,예금주 정보
		int error_code; // 오류구분코드, 5:입금 대기시간 초과, 6:지폐걸림, 7:기타H/W오류
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s910_ask;

typedef struct // S910 응답데이터 (LOGIC->UI) : 송신큐 <-- 부분납 후, 강제종료 (임시결재번호 취소)
{
	int command_type; // LOGIC
	int command_code; // qS910
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		unsigned char message[50]; // 취소하는 임시그룹과금번호 또는 오류메세지
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s910_response;

typedef struct // S911 요청데이터 (UI->LOGIC) : 수신큐 <-- 키오스크 ON/OFF
{
	int command_type; // LOGIC
	int command_code; // qS911
	struct
	{
		unsigned char kiosk_power; // 1:전원ON, 2:전원OFF
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s911_ask;

typedef struct // S911 응답데이터 (LOGIC->UI) : 송신큐 <-- 키오스크 ON/OFF
{
	int command_type; // LOGIC
	int command_code; // qS911
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		unsigned char message[50]; // 응답코드가 오류인 경우, 오류 상세 설명
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s911_response;

typedef struct // S912 요청데이터 (UI->LOGIC) : 수신큐 <-- H/W 오류상황 기록
{
	int command_type; // LOGIC
	int command_code; // qS912
	struct
	{
		int error_code; // 오류구분코드, 1:용지걸림,2:용지부족,3:지폐걸림,4:기타H/W오류
		unsigned char user_phone[50]; // 관리자 연락번호(복수개는 컴마(,) 구분자로 구분)
		unsigned char message[50]; // 오류내용(영수증 용지걸림,부족,지폐걸림,돈통오류 등)
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s912_ask;

typedef struct // S912 응답데이터 (LOGIC->UI) : 송신큐 <-- H/W 오류상황 기록
{
	int command_type; // LOGIC
	int command_code; // qS912
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		unsigned char message[50]; // 응답코드가 오류인 경우, 오류 상세 설명
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s912_response;

typedef struct // S913 요청데이터 (UI->LOGIC) : 수신큐 <-- 서버응답전문 재전송
{
	int command_type; // LOGIC
	int command_code; // qS913
} __attribute__((packed)) t_s913_ask;

typedef struct // S913 응답데이터 (LOGIC->UI) : 송신큐 <-- 서버응답전문 재전송
{
	int command_type; // LOGIC
	int command_code; // qS913
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		unsigned char message[50]; // 응답코드가 오류인 경우, 오류 상세 설명
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s913_response;

typedef struct // S914 요청데이터 (UI->LOGIC) : 수신큐 <-- 가맹점 가상계좌 잔액조회
{
	int command_type; // LOGIC
	int command_code; // qS914
} __attribute__((packed)) t_s914_ask;

typedef struct // S914 응답데이터 (LOGIC->UI) : 송신큐 <-- 가맹점 가상계좌 잔액조회
{
	int command_type; // LOGIC
	int command_code; // qS914
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		unsigned char message[50]; // 가상계좌번호 또는 오류메세지
		int balance; // 가맹점 잔액
		unsigned char bank_name[20]; // 가상계좌번호 은행명
		unsigned char account_user[50]; // 가상계좌번호 계좌주명
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s914_response;

typedef struct // S921 요청데이터 (UI->LOGIC) : 수신큐 <-- 패치확인
{
	int command_type; // LOGIC
	int command_code; // qS921
} __attribute__((packed)) t_s921_ask;

typedef struct // S921 응답데이터 (LOGIC->UI) : 송신큐 <-- 패치확인
{
	int command_type; // LOGIC
	int command_code; // qS921
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		unsigned char patch_enable; // 패치필요여부, 'Y':패치, ‘N':필요없음
		int patch_time; // 0~24
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s921_response;

typedef struct // S922 요청데이터 (UI->LOGIC) : 수신큐 <-- 패치요청
{
	int command_type; // LOGIC
	int command_code; // qS922
} __attribute__((packed)) t_s922_ask;

typedef struct // S922 응답데이터 (LOGIC->UI) : 송신큐 <-- 패치요청
{
	int command_type; // LOGIC
	int command_code; // qS922
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		unsigned char patch_url[128]; // 패치 URL
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s922_response;

typedef struct // S923 요청데이터 (UI->LOGIC) : 수신큐 <-- 패치완료
{
	int command_type; // LOGIC
	int command_code; // qS923
} __attribute__((packed)) t_s923_ask;

typedef struct // S923 응답데이터 (LOGIC->UI) : 송신큐 <-- 패치완료
{
	int command_code; // qS923
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		unsigned char kiosk_version[10]; // 패치 후, 키오스크 버전(ex. 1.0)
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s923_response;

typedef struct // S941 요청데이터 (UI->LOGIC) : 수신큐 <-- 상품권 교환검사
{
	int command_type; // LOGIC
	int command_code; // qS941
	struct
	{
		int cp_code; // 퍼니카드, 해피머니, 틴캐시 등등
		unsigned char issue_date[8]; // 핀발행일자, yyyyMMdd, 교환여부
		unsigned char pin_manage_num[20]; // 핀 관리번호 - 뒤8자리만, (취소번호, UI 입력된 번호 8자리)
//		int cp_amount; // 핀 액면가, (액면가)
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s941_ask;

typedef struct // S941 응답데이터 (LOGIC->UI) : 송신큐 <-- 상품권 교환검사
{
	int command_type; // LOGIC
	int command_code; // qS941
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		unsigned char message[50]; // 그룹과금번호, 교환대상 그룹과금번호 (이후 취소용 과금번호)
		unsigned char pin_num[24]; // 핀 번호(인증키), 교환대상 핀 번호 (Full) - 취소용
		unsigned char pin_manage_num[20]; // 핀 번호(인증키), 교환대상 확인용 핀 관리번호 (Full) - 취소용
		unsigned char pin_amount[10]; // 핀 액면가, 확인용 핀 액면가
		unsigned char change_enable; // 교환가능 여부, 'Y':교환가능, 'N':교환불가
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s941_response;

typedef struct // S942 요청데이터 (UI->LOGIC) : 수신큐 <-- 상품권 교환요청
{
	int command_type; // LOGIC
	int command_code; // qS942
	struct
	{
		int cp_code; // 퍼니카드, 해피머니, 틴캐시 등등
		int cp_count; // 발권 매수, 1~9
		int cp_amount; // 발권 금액
		int issue_type; // 발권 방식, 1:발권, 2:SMS (발권매수가 1장일때만 가능), 3:교환발권, 4:교환SMS발권(발권매수가 1장일때만 가능)
		unsigned char client_phone[16]; // SMS발권시 고객 전화번호
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s942_ask;

typedef struct // S942 응답데이터 (LOGIC->UI) : 송신큐 <-- 상품권 교환요청
{
	int command_type; // LOGIC
	int command_code; // qS942
	int response_code; // 0:정상, 이외는 응답결과코드 참조
							// 1001:존재하지 않는 CP코드 입니다, 1002:CP사로의 취소요청이 실패하였습니다, 1099:서버 내부 오류가 발생하였습니다
							// 2010:존재하지 않는 결재번호, 2011:이미 취소 완료된 취소내역 존재, 2012:취소금액 불일치, 2001:가맹점 단말기 미활성화 혹은 해당하는 레코드 부재
							// 2002:단말기 계약 상태 이상 혹은 구매내역 조회 실패, 2003:구매내역테이블 업데이트 에러, 2020:구매내역테이블 수정/삽입 실패
							// 2030:DB 처리 Exception 발생, 2098:단수 발권에 대한 발권취소 실패, 2099:복수 발권에 대한 일부/전체발권취소 실패
} __attribute__((packed)) t_s942_response;
//----------------------------------------------------------------------------------------------
typedef struct // qBILL_COIN_ACCEPT 요청데이터 (UI->LOGIC) : 수신큐 <-- 지폐/동전 입력 요청
{
	int command_type; 
	int command_code; // qBILL_COIN_ACCEPT
	struct
	{
		int target_amount; // 입력 목표 금액
		int wait_time; // 금액 입력 대기시간
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_billcoin_ask;

typedef struct // qBILL_COIN_ACCEPT 응답데이터 (LOGIC->UI) : 송신큐 <-- 지폐/동전 입력 응답
{
	int command_type; 
	int command_code; // qBILL_COIN_ACCEPT
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		unsigned char machine; // bill:1, coin:2
		int input_amount; // 입력 금액
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_billcoin_response;
//----------------------------------------------------------------------------------------------
typedef struct // PRINT 응답 이벤트 데이터 (LOGIC->UI) : 송신큐 <-- 프린트 이벤트
{
	int command_type; 
	int command_code; // qPRINT_START, qPRINT_COMPLETE, qPRINT_ERROR
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		int progress; 		// 1~100% 진행율
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_print_response;
//----------------------------------------------------------------------------------------------
typedef struct // ETHERNET 응답 이벤트 데이터 (LOGIC->UI) : 송신큐 <-- 이더넷 이벤트
{
	int command_type; 
	int command_code; 	// qETHERNET_CONNECT, qETHERNET_SEND_RECV, qETHERNET_COMPLETE, // qETHERNET_ERROR
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		int progress; 		// 1~100% 진행율, ETHERNET_SEND_RECV 이벤트에서 설정됨
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_ethernet_response;
//----------------------------------------------------------------------------------------------
typedef struct // MODEM 응답 이벤트 데이터 (LOGIC->UI) : 송신큐 <-- 모뎀 이벤트
{
	int command_type; 
	int command_code; 	// qMODEM_CONNECT, qMODEM_SEND_RECV, qMODEM_COMPLETE, qMODEM_ERROR
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		int progress; // 1~100% 진행율, ETHERNET_SEND_RECV 이벤트에서 설정됨
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_MODEM_response;
//----------------------------------------------------------------------------------------------
typedef struct // USB 요청데이터 (UI->LOGIC) : 수신큐 <-- 체크, 다운로드
{
	int command_type; 
	int command_code; 	// qUSB_CHECK, qUSB_DOWNLOAD
} __attribute__((packed)) t_usb_ask;

typedef struct // USB 응답데이터 (LOGIC->UI) : 송신큐 <-- 체크, 다운로드
{
	int command_type; 
	int command_code; // qUSB_CHECK, qUSB_DOWNLOAD
	int response_code; // 0:정상, 이외는 응답결과코드 참조
	struct
	{
		int progress; // 1~100% 진행율, ETHERNET_SEND_RECV 이벤트에서 설정됨
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_usb_response;
//----------------------------------------------------------------------------------------------
typedef struct
{
	int msg_type; // 1 으로 놓는다
	union
	{
		struct
		{
			int command_type; // LCD, PRINTER, BILL, COIN, ETHERNET, MODEM, LOGIC, MSR, IC, USB...
			int command_code; // 수행할 기능코드 (1~99)
			int response_code; // 0:정상, 이외는 응답결과코드 참조
		};
		t_etc_ask_reboot reboot_ask;
		t_etc_ask_cancel cancel_ask;
		t_etc_response_cancel cancel_resp;

		t_etc_ask_init_memory init_memory_ask;
		t_etc_response_init_memory init_memory_resp;
		t_etc_ask_variable variable_ask;
		t_etc_response_variable variable_resp;

		t_etc_ask_test_issue test_issue_ask;
		t_etc_response_test_issue test_issue_resp;
		t_etc_ask_set_volume set_volume_ask;
		t_etc_response_set_volume set_volume_resp;
		t_etc_ask_set_tsound set_tsound_ask;
		t_etc_response_set_tsound set_tsound_resp;
		t_etc_ask_set_bsound set_bsound_ask;
		t_etc_response_set_bsound set_bsound_resp;
		t_etc_ask_set_lantype set_lantype_ask;
		t_etc_response_set_lantype set_lantype_resp;
		t_etc_ask_set_lanip set_lanip_ask; 
		t_etc_response_set_lanip set_lanip_resp;
		t_etc_ask_set_serverip set_serverip_ask;
		t_etc_response_set_serverip set_serverip_resp;

		t_s900_ask qs900_ask;
		t_s900_response	qs900_resp;
		t_s901_ask qs901_ask;
		t_s901_response	qs901_resp;
		t_s902_ask qs902_ask;
		t_s902_response	qs902_resp;
		t_s903_ask qs903_ask;
		t_s903_response	qs903_resp;
		t_s905_ask qs905_ask;
		t_s905_response	qs905_resp;
		t_s906_ask qs906_ask;
		t_s906_response	qs906_resp;
		t_s907_ask qs907_ask;
		t_s907_response	qs907_resp;
		t_s908_ask qs908_ask;
		t_s908_response	qs908_resp;
		t_s909_ask qs909_ask;
		t_s909_response	qs909_resp;
		t_s910_ask qs910_ask;
		t_s910_response	qs910_resp;
		t_s911_ask qs911_ask;
		t_s911_response	qs911_resp;
		t_s912_ask qs912_ask;
		t_s912_response	qs912_resp;
		t_s913_ask qs913_ask;
		t_s913_response	qs913_resp;
		t_s914_ask qs914_ask;
		t_s914_response	qs914_resp;
		t_s921_ask qs921_ask;
		t_s921_response	qs921_resp;
		t_s922_ask qs922_ask;
		t_s922_response	qs922_resp;
		t_s923_ask qs923_ask;
		t_s923_response	qs923_resp;
		t_s941_ask qs941_ask;
		t_s941_response qs941_resp;
		t_s942_ask qs942_ask;
		t_s942_response qs942_resp;

		t_billcoin_ask	qbillcoin_ask;
		t_billcoin_response qbillcoin_resp;
		t_print_response qprint_resp;
		t_ethernet_response qlan_resp;
		t_MODEM_response qmodem_resp;
		t_usb_ask qusb_ask;
		t_usb_response qusb_resp;

		t_test_ask_sound qtest_sound_ask;
		t_test_response_sound qtest_sound_resp;
		t_test_ask_time_set qtest_time_set_ask;
		t_test_response_time_set qtest_time_set_resp;
		t_test_ask_usb_test qtest_usb_test_ask;
		t_test_response_usb_test qtest_usb_test_resp;
		t_test_ask_modem_test qtest_modem_test_ask;
		t_test_response_modem_test qtest_modem_test_resp;
		t_test_ask_billaccept_test qtest_billaccept_test_ask;
		t_test_response_billaccept_test qtest_billaccept_test_resp;
		t_test_ask_lan_test qtest_lan_test_ask;
		t_test_response_lan_test qtest_lan_test_resp;
		t_test_ask_print_test qtest_print_test_ask;
		t_test_response_print_test qtest_print_test_resp;
		t_test_ask_barcode_test qtest_barcode_ask;
		t_test_response_barcode_test qtest_barcode_resp;
		t_test_ask_tms_test qtest_tms_ask;
		t_test_response_tms_test qtest_tms_resp;
		t_test_ask_ta_modem qtest_ta_modem_ask;
		t_test_response_ta_modem qtest_ta_modem_resp;
		t_test_ask_print_speed qtest_print_speed_ask;
		t_test_response_print_speed qtest_print_speed_resp;
		t_test_ask_ta_test qtest_ta_test_ask;
		t_test_ask_usb_logout qtest_usb_logout_ask;
		t_test_response_usb_logout qtest_usb_logout_resp;

		t_tms_first_ask qtms_first_ask;
		t_tms_start_ask qtms_start_logic_ask;
		t_tms_ui_response qtms_ui_resp;
		t_tms_logic_updated_response qtms_logic_updated_resp;

		char data_buff[MSGQ_BUFF_SIZE];
	};
} __attribute__((packed)) t_qdata;
//----------------------------------------------------------------------------------------
typedef struct {
    int  servkind;                          	// SERVKIND...
    
    struct { // 요청전문에 들어가는 필드
		byte transcode[4+1];					// 거래구분코드
        byte unique_id[20+1];					// Unique ID, 20
        byte ip_address[39+1];   				// KIOSK IP 주소
        byte cp_code[4+1];    					// CP 코드, CP코드 테이블 참조
        byte issue_count[2+1];   				// 발권매수, "01"~"09"
        byte issue_amount[10+1]; 				// 발권금액
        byte temp_group_pay_num[50+1];  		// 임시그룹과금번호, 상품재고 체크응답으로 받은 임시그룹과금번호를 설정
        byte approval_code[2+1];   			// 결재수단코드, 고객이 지불한 결재수단 (현금“03”, RF"04", 신용카드“05”)
        byte pg_tran_num[50+1]; 				// PG사 거래번호, 현금결재 등으로 PG사 거래번호가 없는 경우는 전체를 Space로 설정
        byte pg_approval_num[50+1];			// PG사 승인번호, 현금결재 등으로 PG사 승인번호가 없는 경우는 전체를 Space로 설정
        byte pg_tran_date[14+1];    			// PG사 거래일시, PG사 거래일시를 아는경우 설정(YYYYMMDDHHMMSS), 모르는경우, 당일시간 설정
        byte payed_tamount[10+1];    			// 고객이 지불한 총금엑, 1,004원으로 발권시 테스트 발권
        byte issue_type[1+1];    				// 발권방식, 1:발권, 2:SMS(1장일때만 가능)
        byte client_phone[16+1]; 				// 고객연락처, SMS발권시 고객전화번호
        byte client_phone2[20+1]; 			// 고객연락처
        byte group_pay_num[50+1];   			// 그룹과금번호, 발권요청 응답전문의 그룹과금번호
        byte cancel_type[1+1];    			// 취소유형, 관리자 발권취소(‘T'), 관리자 단건취소('P'), 고객 단건취소('C')
        byte pin_date[8+1];    				// 핀 발행일자, YYYYMMDD - 단건취소에만
        byte pin_num[24+1];	   				// 핀 번호(인증키), 단건취소에만
        byte pin_manage_num[20+1];   			// 핀 관리번호,‘C'단건취소에는 뒤8자리만
        byte pin_issue_amount[10+1];   		// 핀 액면가
        byte can_amount[10+1];   				// 발권취소금액
        byte client_bank_name[20+1];		    // 은행이름
        byte client_account[20+1];   			// 은행계좌번호
        byte client_name[24+1];   			// 은행예금주명
        byte error_code[2+1];    				// 오류코드
        byte cash_num[20+1];    				// 캐시번호
        byte error_type_code[2+1];  			// 오류 구분코드
        byte etc_information[100+1]; 		// 기타정보
        byte adjust_ref_time[19+1]; 			// 정산확인 기준시간
        byte adjust_run_time[19+1];	 		// 정산마감 기준시간
        byte temp_payed_amount[10+1]; 		// 부분납금액
        byte kiosk_total_amount[10+1]; 		// 키오스크가 가지고 있는 총금액
        byte admin_pw[6+1];			 		// 관리자 비밀번호
        byte kiosk_status[1+1]; 				// '1':전원ON, '2':전원OFF
        byte error_message[50];				// 오류내용, 영수증 용지걸림, 용지부족, 지폐걸림, 돈통오류 등
        byte self_manage_seq_num[20+1];		// 내부 관리일련번호
//        byte trans_code[4+1];					// 거래구분코드
        byte account_ref_time[19+1];			// 계좌조회 기준시간
        byte kiosk_version[10+1];				// 키오스크 버전
    } Send;

    struct {
		// 응답전문에 들어가는 필드
		byte resp_transcode[4+1];				// 응답구분코드
		byte resp_terminal[16+1];				// 터미널이름
		byte resp_shop_serial[10+1];			// 응답 가맹점 시리얼번호
		byte resp_kiosk_serial[20+1];		// 응답 내부관리일련번호
		byte resp_message[50+1];				// 응답메시지
		byte shop_name[50+1];					// 가맹점명
		byte vaccount_use[1+1];				// 가상계좌 사용여부, '1':사용, '2':비사용
		byte temp_group_pay_num[50+1];		// 임시 그룹과금번호
		byte cash_num[20+1];					// 가상계좌 사용여부에 따른 지폐함 금액 구분용
		byte fix_group_pay_num[50+1];		// 확정 그룹과금번호
		byte group_pay_num[50+1];				// 그룹과금번호
		byte issue_count[2+1];				// 발권매수
		//---------- 반복구간 ----------------------------------------------------------------
		byte pin_num[9][24+1];					// 핀번호(인증키)
		byte pin_manage_num[9][20+1];		// 핀 관리번호
		byte pin_issue_amount[9][10+1];		// 핀 액면가
		byte issue_date[9][10+1];				// 발행일, "20140222" (해피머니 외에는 빈공간)
		byte pin_manage_num2[9][20+1];       // 해피머니만 다름
		//------------------------------------------------------------------------------------
		byte event_message[1000+1];			// 이벤트메시지, 판매점명
		byte event_message_pos[1+1];			// 이벤트메시지 위치, "1':상, '2':하
		byte vaccount_balance[20+1];			// 가상계좌잔액
		byte cancel_ok_count[2+1];			// 발권취소 성공매수
		byte cancel_tcount[2+1];				// 발권취소 전체매수
		//---------- 반복구간 ----------------------------------------------------------------
		byte cancel_unit_num[9][2+1];		// 발권취소 순번, "01"~"09"
		byte cancel_success[9][1+1];			// 취소 성공 여부, 'Y':취소완료, 'N':취소실패
		byte cancel_fail_code[9][4+1];		// 취소 실패 코드, 취소 결과코드 참조
		//---------- 반복구간 ----------------------------------------------------------------
		byte vaccount[50+1];					// 가상계좌정보, 단말기에 부여된 가상계좌번호 또는 오류메시지
		byte adjust_run_time[19+1];			// 정산마감 기준시간, YYYY-MM-DDHH24:MI:SS 형태로 변환
		byte not_adjust_tamount[10+1];		// 미정산 총액, 마지막 정산마감 요청 후부터 현재까지의 입금된 금액의 합계를 반환(미정산액) -> 정상발권액+부분납 강제 종료된 선수금+발권 실패된 선수금
		byte not_adjust_sale_tamount[10+1];	// 미정산 판매총액, 판매금총액(정상발권금액+발권취소금액)
		byte not_adjust_temp_payed_tamount[10+1];	// 미정산 부분납총액
		byte old_adjust_time[19+1];			// 이전 정산의 마감시간, YYYY-MM-DDHH24:MI:SS 형태로 변환
		byte tamount_for_adjust[10+1];		// 마지막 정산마감 요청 후부터 현재까지의 입금된 금액의 합계를 반환(미정산액) -> 판매총액, 취소금액, 부분납 취소금액
		byte sale_tamount[10+1];				// 판매총액
		byte cancel_tamount[10+1];			// 취소금액
		byte cancel_temp_payed_amount[10+1];	// 부분납 취소금액
		byte cancel_in_adjust[10+1];			// 정산마감후 취소금액, 현재 정산마감시간 기준으로 이미 정산이 끝난 건 중 취소된 금액
		byte vaccount_bank_name[20+1];		// 가상계좌번호 은행명
		byte vaccount_bank_user[50+1];		// 가상계좌번호 계좌주명
		byte reboot_enable[1+1];				// 리부팅 여부, 'Y':리부팅, 'N':리부팅 않함
		byte reboot_start_time[3+1];			// 리부팅 시간, "005":5분후, "010":10분후, "900":15시간후
		byte patch_enable[1+1];				// 패치여부, 'Y':패치, 'N':패치 필요 없음
		byte patch_start_time[2+1];			// 패치시간, "00"~"24"
		byte patch_url[128+1];				// 패치URL, ex) http://patch.teencash.co.kr/patch.aspx
		byte kiosk_version[10+1];				// 키오스크 버전
		byte change_enable;					// 교환가능 여부, 'Y':교환가능, 'N':교환불가
    } Receive;

    byte ymdsms[12+1];                      // YYMMDDSSMMSS
    byte respcode[4+1];                     // 응답코드 // 지훈
    ulong tamount;
    ulong amount;
    byte  Reprt;
    byte  BfrCancel;
    byte  netcan_flag;                     // 망상취소플래그(TRUE=망취소)
    byte  MakeTimeStamp[16+1];             // 전문 만들때 사용되는 YYMMDD, 이 값이 Working Key로 사용되어지므로 초기에 한번 읽고, 끝날때까지 사용함.
    byte  register_no[10+1];              // 사업자번호
    int   comm_line;                      // 통신라인
    byte  dcat_id[10+1];                  // download 시 cat-id
    byte RCV_MSG[32+3];                  	// FI 다운로드, TMS CHECK 거래시 오류인 경우의 응답메세지
    byte update_flag;						// TMS CHECK 거래시 사용됨.(1 갱신필요, 2 현버전)
    byte SeqNo[4+1];                       // 전표검증번호
    byte  track2_data[64];
} KIOSK_ID_FIELD;
//----------------------------------------------------------------------------------------------

HNX720T_EXT KIOSK_ID_FIELD kField, kField2;
HNX720T_EXT KIOSK_ID_FIELD kBfrField;

typedef struct {
    byte changed;       
    byte machine_no[10+1];					// 단말기 기기번호
    byte identification_no[11+1];           // 단말기 고유번호 
    byte ymdsms[16+1];                       // 날자
} FL_REGION_MA;
HNX720T_EXT FL_REGION_MA RegionMA;

typedef struct
{
    byte changed;
    
    byte mem_cnf[16];                      // 메모리초기화여부

    // 가맹점정보
    struct {
        byte registration[32+1];                 // 단말기 등록여부
        byte cat_id[20+1];                       // 단말기 ID	(번호)		// DownField.L002
        byte register_no[10+1];                  // 사업자번호			// DownField.L003
        byte retailer_name[48+1];                // 가맹점 상호			// DownField.L004
        byte retailer_address[300+1];           // 가맹점 주소			// DownField.L005
        byte represent_name[32+1];               // 대표자명				// DownField.L006
        byte retailer_phone[32+1];               // 가맹점 전화			// DownField.L007
        byte helpdesk_phone[32+1];               // 고객문의 전화			// DownField.L008

        byte key_index[2+1];                     // Working Key Index		// DownField.L009
        byte working_key[16+1];                  // Working Key			// DownField.L010
        byte sub_register_no[10+1];              // 하위 사업자번호		// DownField.L043
        byte campain_message[404+1];             // 광고메세지			// DownField.L039
        byte shop_serial[10+1];					// 가맹점 시리얼번호
        byte shop_name[50+1];						// 가맹점명
        byte vaccount_enable;						// 가상계좌사용여부, 1사용, 0비사용
    } Merchant;

    // 서버 접속정보
    struct {
        int tm01;                             // 접속대기시간(30초)
        int tm02;                             // 국선접속시간(20초)
        int enq_tm;                           // 연결대기시간(10초)
        int ack_tm;                           // ACK 대기시간(15초)
        int eot_tm;                           // 종료대기시간(05초)
        int resp_tm;                          // 통신대기시간(45초)
        int redial_tm;                        // 재다이얼준비시간(3초)		

        // 접속선로(PSTN_LINE, C2I_LINE, CDMA_LINE, CHECK_LINE, WIBRO_LINE, ETHERNET_LINE, ETHMODEM_LINE, WCDMA_LINE)        
        int comm_line; 
		int private_bps; 
		byte use_comm_type; 

        struct { 
            byte downtel[16+1];                // 다운로드전화번호			// DownField.L011
            byte apptel_01[16+1];              // 승인전화01				// DownField.L012
            byte apptel_02[16+1];              // 승인전화02				// DownField.L013
            byte jasatel_01[16+1];             // 자사승인전화01			// DownField.L014
            byte jasatel_02[16+1];             // 자사승인전화02			// DownField.L015
            byte tmstel_01[16+1];              // TMS전화01				// DownField.L016
            byte tmstel_02[16+1];              // TMS전화02				// DownField.L017
            byte pabx_on;                      // 전화내선사용여부(SW_ON, SW_OFF, SW_OFF)
            byte dial_spd;                     // 다이얼속도(느리게=1, 보통=2, 빠르게=3, 3)
            byte dialtone_on;                  // 다이얼톤 체크(예=1,아니오=2, 1) 
            byte pabx_no[3+1];                 // 국선호출번호(0~9,*,#, 9) 
            byte pabx_wait;                    // 국선호출시간(1~6, 0)
            int  pstn_spd;                     // 전화선속도(1200=1, 2400=2, 1)
            byte predial_on;                   // PREDIAL 여부(1:예, 2:아니오)
        } Pstn;

        struct { 
            byte ipmethod;                     // (1.자동, 2.수동, 1)
            byte ipkind;                       // 서버IP설정(1.도메인명사용, 2.IP설정사용)
            byte address[16+1];                // host 접속 IP address		// DownField.L033
            byte portno[10+1];                  // host 접속 port no		// DownField.L034
            byte tmsportno[10+1];               // tms  접속 port no		// DownField.L035
            byte staticIP[16+1];               // 고정 IP
            byte staticSM[16+1];               // 고정 Subnet MASK
            byte staticGW[16+1];
            byte dns_server01[18+1];
            byte dns_server02[18+1];
            byte domain_name[64+1];
            byte domain_ip[16+1];
            byte phone_change;                 // 전화선접속여부(1:예, 2:아니오) 
        } Ethernet;

    } Comm;
 
    struct {
        struct {
            byte cutting_on;                    // 용지절단(1:절단, 2:부분절단, 2)
            byte print_speed;                   // 인쇄속도(1.매우빠름, 2:빠르게, 3:보통, 4:느리게)
            byte print_concentration;           // 인쇄농도(1:보통, 2:진하게)
        } PrtInfo;
        struct {
            byte background_sound;   // 배경음 설정 (1사용, 0미사용)
            byte touch_sound;         // 터치음 설정 (1사용, 0미사용)
            byte volume;           	// 볼륨값 (디폴트120, 70~127 범위)
        } SoundInfo;
	} Terminfo; 

	struct { 
        byte logo_prt_type;					// 전표출력시 LOGO 인쇄여부(1.비사용, 2.Tpay, 1) 
    } Thermal_Info;

	struct {
	    byte  ymdsms[12+1];                     // 최종거래일시
		byte adjust_run_time[19+1];			// 정산 기준시간 (서버로부터 받은 것) YYMMDDHHMMSS
    } Jungsan_Info;

	struct {
        byte auto_renew_on;						// 자동갱신처리(1:예, 2:아니오, 2)
        byte tms_comm_line;						// TMS통신설정(1:메인통신, 2:전화선, 1)
    } Tms_Info;

	struct {
		byte temp_group_pay_num[50+1];
		byte temp_cp_code[4+1];
		byte temp_payed_amount[10+1];
	    ulong bill_target_amount; 			// 지폐인식기 목표금액 메시지로 받은 것 (인쇄시 사용)
	    ulong bill_input_amount; 				// 지폐인식기 목표금액 메시지로 받은 것 (인쇄시 사용)
	    byte temp_pin_num[24+1];
		byte temp_pin_manage_num[20+1];
	} Issue_Info;
	byte log_depth;
	byte debug_depth;
} FL_REGION_01;
HNX720T_EXT FL_REGION_01 Region01;
HNX720T_EXT FL_REGION_01 Region01_backup;

typedef struct {                            //정산용 정보(정산내역은 별도저장)
    byte changed;
    
    uint  c_qrear, c_qfront;                // CAPTURE qurue rear, front
    byte  ymdsms[12+1];                     // 최종거래일시
    struct {
       int sequence_num;                    // Sequence-number(전표검증번호)(0000 ~ 9999)
       byte yymmdd[6+1];					// 일자가 변경되었는지 여부를 알기위해 만든것임.
    } Seqno;

    struct {
    } Cp_code;
    
    struct {
        byte ymdsms[12+1];				
    } TempClose;

	struct {
        byte ymdsms[12+1];				
    } DailyClose;
    
} FL_REGION_02;
HNX720T_EXT FL_REGION_02 Region02;
HNX720T_EXT FL_REGION_02 Region02_backup;

typedef struct {                        
    byte changed;

	byte ymdsms[12+1];	 // 무조건 업데이트를 수행하는 시간				
	byte last_tms_ymdsms[12+1]; // 마지막 업데이트를 한 시간
	byte updateflag;					
	byte auto_request_flag;
	byte check_ymdsms1[12+1]; // patch_check 전문을 날리는 시간 저장
	byte check_ymdsms2[12+1]; // patch_check 전문을 날리는 시간 저장
} FL_REGION_04;
HNX720T_EXT FL_REGION_04 Region04;
HNX720T_EXT FL_REGION_04 Region04_backup;

typedef struct {                            //정산 데이터
    byte servkind; // 1

	byte trans_date[12]; // 12, 거래일시
    int cp_code;	// 4
    byte approval_type; // 1, 승인유형 (현금3, RF4, 신용5
    byte cp_tcount; 	// 1, 총발권매수
    int tamount; 		// 4, 거래 총금액
    byte ticket_type;	// 1, 발권1, sms2
    byte ticket_user_phone[16]; // 16, sms일때 전화번호
    byte fix_group_pay_num[50]; // 50, 확정그룹과금번호 또는 임시그룹과금번호(환불용)
//    byte pin_num[9][24]; // 24, 핀번호
//    byte pin_manage_num[9][20]; // 20, 핀관리번호
//    int pin_issue_amount[9]; // 36, 발권낱장금액(액면가)
//    byte pin_issue_date[9][6]; // 54, 발행일, 140222
//    byte pin_manage_num2[9][20]; // 핀관리번호와 동일(해피머니만 다름)

	byte cancel_type; // 1, 취소유형, 관리자발권취소T, 관리자단건취소P, 고객단건취소C
	byte pin_issue_date[8]; //8, 단건취소된 상품권의 발행일자
    byte pin_num[24]; // 24, 핀번호
    byte pin_manage_num[20]; // 20, 핀관리번호
    byte error_type_code; // 1, 오류구분코드, 입금대기시간초과5, 지폐걸림6, 기타H/W오류7
} CAP_DATA;
HNX720T_EXT CAP_DATA kcap_data;


HNX720T_EXT t_qdata msgq_buf;
HNX720T_EXT int msg_thread_state;
HNX720T_EXT int msg_thread_exit;
HNX720T_EXT int msg_thread_stop;

HNX720T_EXT int msg_thread_state2;
HNX720T_EXT int msg_thread_exit2;
HNX720T_EXT int msg_thread_stop2;

HNX720T_EXT int msg_thread_state4;
HNX720T_EXT int msg_thread_exit4;
HNX720T_EXT int msg_thread_stop4;

HNX720T_EXT int msg_cancel; //메시지큐에서 취소명령시 양수로 설정됨
HNX720T_EXT int now_idle; // 취소명령시 idle=1 상태에서는 msg_cancel을 양수로 만들지 않는다.
HNX720T_EXT byte field_not_erase; // 0지움, 1않지움

HNX720T_EXT int tms_check_flag;
HNX720T_EXT int tms_update_flag;
HNX720T_EXT byte first_modem_test_flag;
HNX720T_EXT byte network_cancel_flag;
//----------------------------------------------------------------------------------------------

