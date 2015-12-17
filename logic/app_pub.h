//    ȭ�ϸ� :    PUB.H
// DUMY
HNX720T_EXT byte dumy01[1024+1];                   // DUMY0A BUFFER
HNX720T_EXT byte dumy02[MAXMID*22];                // DUMY02 BUFFER 3(�ſ�)
HNX720T_EXT byte dumy03[MAXMID*22];                // DUMY03 BUFFER 4(BACKUP)


/////////////////////////////////////////////////////////////////////////////////
// ���ּ� ���� ������ ���ֹ߼� �������Դϴ�.
// �ŷ��� �������� �� �ֹ߼� �������� �������� ��� �ؾ��մϴ�.
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
HNX720T_EXT byte printsign[64/8][128];// �μ�� ���� data
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

HNX720T_EXT byte c_cap1[MAXCAP*CAP_RECORD_SIZE];    // ���迵��
HNX720T_EXT byte *capture;

//----------------------------------------------------------------------------------
// KIOSK Program varialbes, 20140701_KHD
// GENERAL BUFFER
//----------------------------------------------------------------------------------
HNX720T_EXT int  KIOSK_SERVKIND;		// SERVICEKIND
HNX720T_EXT byte KIOSK_STEP;          // 
HNX720T_EXT byte KIOSK_NEXTSTEP;     // 
//----------------------------------------------------------------------------------------------
typedef struct // TMS ���� ��û (UI->LOGIC)
{
	int command_type; // qTMS
	int command_code; // qTMS_FIRST
} __attribute__((packed)) t_tms_first_ask;
//----------------------------------------------------------------------------------------------
typedef struct // TMS ���� �̺�Ʈ ��û (LOGIC->UI)
{
	int command_type; // qTMS
	int command_code; // qTMS_CC
} __attribute__((packed)) t_tms_start_ask;
//----------------------------------------------------------------------------------------------
typedef struct // TMS ���� ui ���� (UI->LOGIC)
{
	int command_type; // qTMS
	int command_code; // qTMS_CC
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_tms_ui_response;
//----------------------------------------------------------------------------------------------
typedef struct // TMS logic ���� (LOGIC->UI)
{
	int command_type; // qTMS
	int command_code; // qTMS_UPDATED
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_tms_logic_updated_response;
//----------------------------------------------------------------------------------------------
typedef struct // TA ������ �׽�Ʈ ��û (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_TA_TEST
	struct
	{
		int command; // 0:start, 1:stop
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_test_ask_ta_test;
//----------------------------------------------------------------------------------------------
typedef struct // USB�� �α� ��� ��û(UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_USB_LOGOUT
} __attribute__((packed)) t_test_ask_usb_logout;
//----------------------------------------------------------------------------------------------
typedef struct // USB�� �α� ��� ����(LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_USB_LOGOUT
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_test_response_usb_logout;
//----------------------------------------------------------------------------------------------
typedef struct // Type Approval Modem �׽�Ʈ ��û (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_MODEM
	struct
	{
		int command; // off-hook:10, on-hook:20, data send:30, dtmf test:0~9, '*', '#'
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_test_ask_ta_modem;
//----------------------------------------------------------------------------------------------
typedef struct // Type Approval Modem �׽�Ʈ ���� (LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_MODEM
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_test_response_ta_modem;
//----------------------------------------------------------------------------------------------
typedef struct // �μ�� ���� �׽�Ʈ ��û (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_PRINT_SPEED
	struct
	{
		int speed; // 0~100, �Ĺ� ���� default 80, set_speed
		int speed2; // 0~100, ���� ���� default 40, set_step
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_test_ask_print_speed;
//----------------------------------------------------------------------------------------------
typedef struct // �μ�� ���� �׽�Ʈ ���� (LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_PRINT_SPEED
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_test_response_print_speed;
//----------------------------------------------------------------------------------------------
typedef struct // SOUND �׽�Ʈ ��û (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_SOUND
} __attribute__((packed)) t_test_ask_sound;
//----------------------------------------------------------------------------------------------
typedef struct // SOUND �׽�Ʈ ���� (LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_SOUND
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_test_response_sound;
//----------------------------------------------------------------------------------------------
typedef struct // TIME_SET ��û (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_TIME_SET
	struct
	{
		unsigned char yymmddhhmmss[12+1]; // yymmddhhmmss + null ����
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_test_ask_time_set;
//----------------------------------------------------------------------------------------------
typedef struct // TIME_SET ���� (LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_TIME_SET
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_test_response_time_set;
//----------------------------------------------------------------------------------------------
typedef struct // USB_TEST ��û (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_USB
} __attribute__((packed)) t_test_ask_usb_test;
//----------------------------------------------------------------------------------------------
typedef struct // USB_TEST ���� (LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_USB
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_test_response_usb_test;
//----------------------------------------------------------------------------------------------
typedef struct // MODEM_TEST ��û (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_MODEM
} __attribute__((packed)) t_test_ask_modem_test;
//----------------------------------------------------------------------------------------------
typedef struct // MODEM_TEST ���� (LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_MODEM
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_test_response_modem_test;
//----------------------------------------------------------------------------------------------
typedef struct // BILLACCEPT_TEST ��û (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_BILLACCEPT
	struct
	{
		int target_amount; // �Է� ��ǥ �ݾ�
		int wait_time; // �ݾ� �Է� ���ð�
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_test_ask_billaccept_test;
//----------------------------------------------------------------------------------------------
typedef struct // BILLACCEPT_TEST ���� (LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_BILLACCEPT
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_test_response_billaccept_test;
//----------------------------------------------------------------------------------------------
typedef struct // LAN_TEST ��û (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_LAN
} __attribute__((packed)) t_test_ask_lan_test;
//----------------------------------------------------------------------------------------------
typedef struct // LAN_TEST ���� (LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_LAN
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_test_response_lan_test;
//----------------------------------------------------------------------------------------------
typedef struct // PRINT_TEST ��û (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_PRINT
} __attribute__((packed)) t_test_ask_print_test;
//----------------------------------------------------------------------------------------------
typedef struct // PRINT_TEST ���� (LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_PRINT
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_test_response_print_test;
//----------------------------------------------------------------------------------------------
typedef struct // BARCORD ��û (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_BARCODE
} __attribute__((packed)) t_test_ask_barcode_test;
//----------------------------------------------------------------------------------------------
typedef struct // PRINT_TEST ���� (LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_BARCODE
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_test_response_barcode_test;
//----------------------------------------------------------------------------------------------
typedef struct // TMS ��û (UI->LOGIC)
{
	int command_type; // qTEST
	int command_code; // qTEST_TMS
} __attribute__((packed)) t_test_ask_tms_test;
//----------------------------------------------------------------------------------------------
typedef struct // PRINT_TEST ���� (LOGIC->UI)
{
	int command_type; // qTEST
	int command_code; // qTEST_TMS
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_test_response_tms_test;
//----------------------------------------------------------------------------------------------
typedef struct // Reboot �̺�Ʈ ������ (LOGIC->UI)
{
	int command_type; // ETC
	int command_code; // qREBOOT
} __attribute__((packed)) t_etc_ask_reboot;
//----------------------------------------------------------------------------------------------
typedef struct // Cancel ��û������ (UI->LOGIC)
{
	int command_type; // ETC
	int command_code; // qREBOOT
} __attribute__((packed)) t_etc_ask_cancel;
//----------------------------------------------------------------------------------------------
typedef struct // Cancel ���䵥���� (LOGIC->UI)
{
	int command_type; // ETC
	int command_code; // qREBOOT
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_etc_response_cancel;
//----------------------------------------------------------------------------------------------
typedef struct // �޸� �ʱ�ȭ ��û������ (UI->LOGIC)
{
	int command_type; // ETC
	int command_code; // qINIT_MEMORY
} __attribute__((packed)) t_etc_ask_init_memory;
//----------------------------------------------------------------------------------------------
typedef struct // �޸� �ʱ�ȭ ���䵥���� (LOGIC->UI)
{
	int command_type; // ETC
	int command_code; // qINIT_MEMORY
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_etc_response_init_memory;
//----------------------------------------------------------------------------------------------
typedef struct // �������� ��û������ (UI->LOGIC)
{
	int command_type; // ETC
	int command_code; // qVARIABLE
} __attribute__((packed)) t_etc_ask_variable;
//----------------------------------------------------------------------------------------------
typedef struct // �������� ���䵥���� (LOGIC->UI)
{
	int command_type; // ETC
	int command_code; // qVARIABLE
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		unsigned char registration; // �ܸ��� ��ġ�������� (1��ġ������, 0��ġ���� �ʵ�, ����Ʈ�� 0)
		unsigned char vaccount_enable; // ������� ��뿩�� (1���, 0�̻��, ����Ʈ�� 0)
		unsigned char background_sound; // ����� ��뿩�� (1���, 0�̻��, ����Ʈ�� 1)
		unsigned char touch_sound; // ��ġ�� ��뿩�� (1���, 0�̻��, ����Ʈ�� 1)
		unsigned char volume; // ���� ������ (1~127, ��������� 70~127, ����Ʈ�� 120)
		unsigned char dynamic_ip_use; // ����IP ��뿩�� (1���, 0�̻��, ����Ʈ�� 0)
		unsigned char kiosk_ip[16]; // Ű����ũ IP
		unsigned char kiosk_sm[16]; // Ű����ũ ����ݸ���ũ
		unsigned char kiosk_gw[16]; // Ű����ũ ����Ʈ����
		unsigned char server_ip[16]; // ���� ���� IP
		unsigned char server_port[10]; // ���� ���� ��Ʈ
		unsigned char kiosk_version[10]; // 1.0.0
		unsigned char kiosk_yymmddhhmmss[13]; // Ű����ũ �ý��۽ð�
		unsigned char kiosk_mac[18];
		unsigned char kiosk_id[11];
		unsigned char shop_id[11];
		int print_speed;
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_etc_response_variable;
//----------------------------------------------------------------------------------------------
typedef struct // �׽�Ʈ �߱� ��û������ (UI->LOGIC)
{
	int command_type; // ETC
	int command_code; // qTEST_ISSUE
} __attribute__((packed)) t_etc_ask_test_issue;
//----------------------------------------------------------------------------------------------
typedef struct // �׽�Ʈ �߱� ���䵥���� (LOGIC->UI)
{
	int command_type; // ETC
	int command_code; // qTEST_ISSUE
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_etc_response_test_issue;
//----------------------------------------------------------------------------------------------
typedef struct // �������� ��û������ (UI->LOGIC)
{
	int command_type; // ETC
	int command_code; // qVOLUME
	struct
	{
		unsigned char vol; // ������ (1~127, ��������� 70~127, ����Ʈ�� 120)
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_etc_ask_set_volume;
//----------------------------------------------------------------------------------------------
typedef struct // �������� ���䵥���� (LOGIC->UI)
{
	int command_type; // ETC
	int command_code; // qVOLUME
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_etc_response_set_volume;
//----------------------------------------------------------------------------------------------
typedef struct // ��ġ������ ��û������ (UI->LOGIC)
{
	int command_type; // ETC
	int command_code; // qTOUCH_SOUND
	struct
	{
		unsigned char tsound; // 1:���, 0:�̻��
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_etc_ask_set_tsound;
//----------------------------------------------------------------------------------------------
typedef struct // ��ġ������ ���䵥���� (LOGIC->UI)
{
	int command_type; // ETC
	int command_code; // qTOUCH_SOUND
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_etc_response_set_tsound;
//----------------------------------------------------------------------------------------------
typedef struct // ��������� ��û������ (UI->LOGIC)
{
	int command_type; // ETC
	int command_code; // qBACK_SOUND
	struct
	{
		unsigned char bsound; // 1:���, 0:�̻��
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_etc_ask_set_bsound;
//----------------------------------------------------------------------------------------------
typedef struct // ��������� ���䵥���� (LOGIC->UI)
{
	int command_type; // ETC
	int command_code; // qBACK_SOUND
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_etc_response_set_bsound;
//----------------------------------------------------------------------------------------------
typedef struct // ��Ÿ�� ���� ��û������ (UI->LOGIC)
{
	int command_type; // ETC
	int command_code; // qLAN_SELECT
	struct
	{
		unsigned char lantype; // 1:����, 0:����
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_etc_ask_set_lantype;
//----------------------------------------------------------------------------------------------
typedef struct // ��Ÿ�� ���� ���䵥���� (LOGIC->UI)
{
	int command_type; // ETC
	int command_code; // qLAN_SELECT
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_etc_response_set_lantype;
//----------------------------------------------------------------------------------------------
typedef struct // ����IP ���� ��û������ (UI->LOGIC)
{
	int command_type; // ETC
	int command_code; // qLAN_IP_SET
	struct
	{
		unsigned char kiosk_ip[16]; // Ű����ũ IP
		unsigned char kiosk_sm[16]; // Ű����ũ ����ݸ���ũ
		unsigned char kiosk_gw[16]; // Ű����ũ ����Ʈ����
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_etc_ask_set_lanip;
//----------------------------------------------------------------------------------------------
typedef struct // ����IP ���� ���䵥���� (LOGIC->UI)
{
	int command_type; // ETC
	int command_code; // qLAN_IP_SET
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_etc_response_set_lanip;
//----------------------------------------------------------------------------------------------
typedef struct // ����IP/PORT ���� ��û������ (UI->LOGIC)
{
	int command_type; // ETC
	int command_code; // qSERVER_IP_SET
	struct
	{
		unsigned char server_ip[16]; // ���� ���� IP
		unsigned char server_port[10]; // ���� ���� PORT
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_etc_ask_set_serverip;
//----------------------------------------------------------------------------------------------
typedef struct // ����IP/PORT ���� ���䵥���� (LOGIC->UI)
{
	int command_type; // ETC
	int command_code; // qSERVER_IP_SET
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
} __attribute__((packed)) t_etc_response_set_serverip;
//----------------------------------------------------------------------------------------------
typedef struct // S900 ��û������ (UI->LOGIC) : ����ť <-- ��ġ����
{
	int command_type; // LOGIC
	int command_code; // qS900
	struct
	{
		unsigned char shop_serial_num[10];
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s900_ask;

typedef struct // S900 ���䵥���� (LOGIC->UI) : �۽�ť <-- ��ġ����
{
	int command_type; // LOGIC
	int command_code; // qS900
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		unsigned char message[50]; // �����ڵ尡 ������ ���, ���� �� ����
		unsigned char shop_name[50]; // ������ �̸�
		unsigned char vaccount_enable; // ������� ��뿩��, �̻��: 0, ���: 1
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s900_response;

typedef struct // S901 ��û������ (UI->LOGIC) : ����ť <-- ��ǰ�� �ǸŰ��� ���� üũ
{
	int command_type; // LOGIC
	int command_code; // qS901
	struct
	{
		int cp_code; // �۴�ī��, ���ǸӴ�, ƾĳ�� ���
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s901_ask;

typedef struct // S901 ���䵥���� (LOGIC->UI) : �۽�ť <-- ��ǰ�� �ǸŰ��� ���� üũ
{
	int command_type; // LOGIC
	int command_code; // qS901
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		unsigned char message[50]; // �����ڵ尡 ������ ���, ���� �� ����
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s901_response;

typedef struct // S902 ��û������ (UI->LOGIC) : ����ť <-- ��ǰ ��� üũ
{
	int command_type; // LOGIC
	int command_code; // qS902
	struct
	{
		int cp_code; // �۴�ī��, ���ǸӴ�, ƾĳ�� ���
		int cp_count; // �ѹ߱� �ż�, 1~9
		int cp_amount; // �߱� ���� �ݾ�
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s902_ask;

typedef struct // S902 ���䵥���� (LOGIC->UI) : �۽�ť <-- ��ǰ ��� üũ
{
	int command_type; // LOGIC
	int command_code; // qS902
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		unsigned char message[50]; // �ӽ� �׷���ݹ�ȣ Ȥ�� �����޽���
		int enable_count; // �߱� ���� �ż� (�ִ� 9��), 1~9
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s902_response;

typedef struct // S903 ��û������ (UI->LOGIC) : ����ť <-- �߱� ��û
{
	int command_type; // LOGIC
	int command_code; // qS903
	struct
	{
		int approval_type; // ����=3, RF=4, �ſ�ī��=5
		int cp_code; // �۴�ī��, ���ǸӴ�, ƾĳ�� ���
		int cp_amount; // �߱� ���� �ݾ�
		int cp_count; // �ѹ߱� �ż�
		int amount; // �ŷ� �ѱݾ� = cp_amount * cp_count
		int ticket_type; // �߱�=��1��, SMS='2'(1���϶��� ����)
		byte ticket_user_number[16];
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s903_ask;

typedef struct // S903 ���䵥���� (LOGIC->UI) : �۽�ť <-- �߱� ��û
{
	int command_type; // LOGIC
	int command_code; // qS903
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		int cp_code; // �۴�ī��, ���ǸӴ�, ƾĳ�� ���
		int cp_count; // �ѹ߱� �ż� (�ִ� 9��)
		unsigned char event_msg[1000];
		unsigned char event_msg_position; // ��:'1', ��:��2��
		int vaccount_balance; // ������� �ܾ�
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s903_response;

typedef struct // S905 ��û������ (UI->LOGIC) : ����ť <-- �߱�/�ܰ� ���
{
	int command_type; // LOGIC
	int command_code; // qS905
	struct
	{
		unsigned char cancel_type; // �������, ������ �߱����=��T', ������ �ܰ����='P', �� �ܰ����=��C'
		int cp_code; // �۴�ī��, ���ǸӴ�, ƾĳ�� ���
		unsigned char issue_date[8]; // ��������, yyyyMMdd, �ܰ���ҿ���
		unsigned char manage_num[24]; // ������ȣ, ��� �����̽���
		unsigned char cancel_num[20]; // ��ҹ�ȣ, UI �Էµ� ��ȣ 8�ڸ�
		int cp_amount; // �׸鰡
		int cancel_amount; // �߱� ��� �ݾ�, ��ҷ� ������ ó���� �ݾ� (��T'�� ��ü�ݾ�, 'P'/'C'�� �׸�ݾ�)
		unsigned char client_phone[16]; // ������ó, �� �ܰ� ��ҽ� ȯ�ҿ�
		unsigned char client_bank_name[20]; // �����̸�
		unsigned char client_account[20]; // ������¹�ȣ, �� �ܰ� ��ҽ� ȯ�ҿ�
		unsigned char client_name[24]; // ���࿹���ָ�
		int error_code; // ���� �����ڵ�, 1:��������, 2:�����, 3:�����ɸ�, 4:��������, 7:�ɹ�ȣ ���Ž���, 20:��ŸH/W����
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s905_ask;

typedef struct // S905 ���䵥���� (LOGIC->UI) : �۽�ť <-- �߱�/�ܰ� ���
{
	int command_type; // LOGIC
	int command_code; // qS905
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		int cancel_ok_count; // �߱���� �����ż�
		int cancel_tcount; 	// �߱���� ��ü�ż�
		int vaccount_balance; // ������� �ܾ�
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s905_response;

typedef struct // S906 ��û������ (UI->LOGIC) : ����ť <-- �������Ȳ ���
{
	int command_type; // LOGIC
	int command_code; // qS906
	struct
	{
		unsigned char error_code; // 8:�� ���� ������û
		unsigned char client_num[21]; // ������ó
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s906_ask;

typedef struct // S906 ���䵥���� (LOGIC->UI) : �۽�ť <-- �������Ȳ ���
{
	int command_type; // LOGIC
	int command_code; // qS906
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		unsigned char message[50]; // �����ڵ尡 ������ ���, ���� �� ����
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s906_response;

typedef struct // S907 ��û������ (UI->LOGIC) : ����ť <-- ���긶�� �ݾ� Ȯ��
{
	int command_type; // LOGIC
	int command_code; // qS907
} __attribute__((packed)) t_s907_ask;

typedef struct // S907 ���䵥���� (LOGIC->UI) : �۽�ť <-- ���긶�� �ݾ� Ȯ��
{
	int command_type; // LOGIC
	int command_code; // qS907
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		unsigned char vaccount_inform[50]; // �ܸ��⿡ �ο��� ������¹�ȣ �Ǵ� �����޽���
		unsigned char reference_time[19]; // ���긶�� ���ؽð�, YYYY-MM-DDHH24:MI:SS ����
		int not_magam_total_amount; 		// ��⳻ �ѱݾ�
		int not_magam_sale_amount; 		// �����Ǹűݾ�
		int not_magam_part_amount;		// �κг��ݾ�
		int not_magam_approval_amount; 	// �� ���αݾ�
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s907_response;

typedef struct // S908 ��û������ (UI->LOGIC) : ����ť <-- ���긶�� ó��
{
	int command_type; // LOGIC
	int command_code; // qS908
} __attribute__((packed)) t_s908_ask;

typedef struct // S908 ���䵥���� (LOGIC->UI) : �۽�ť <-- ���긶�� ó��
{
	int command_type; // LOGIC
	int command_code; // qS908
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		unsigned char vaccount_inform[50]; // �ܸ��⿡ �ο��� ������¹�ȣ �Ǵ� �����޽���
		unsigned char reference_time[19]; // ���긶�� ���ؽð�, YYYY-MM-DDHH24:MI:SS ����
		unsigned char old_reference_time[19]; // ���� ���긶���ð�, YYYY-MM-DDHH24:MI:SS
		int magam_total_amount; 	// ������ ���긶�� ��û �ĺ��� ��������� �Աݵ�
									// �ݾ��� �հ�(�������)
		int magam_sale_amount; 	// �Ǹű��Ѿ�(����߱Ǳݾ�+�߱���ұݾ�)
		int magam_cancel_amount;	// ��ұݾ�
		int magam_part_cancel_amount;	// ���� �κг� ��ұݾ�
		int magam_after_cancel_amount;	// ���긶���� ��ұݾ�
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s908_response;

typedef struct // S909 ��û������ (UI->LOGIC) : ����ť <-- ������ �α��� ����
{
	int command_type; // LOGIC
	int command_code; // qS909
	struct
	{
		unsigned char admin_password[6];	// ������ ��й�ȣ (4�ڸ� �̻�)
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s909_ask;

typedef struct // S909 ���䵥���� (LOGIC->UI) : �۽�ť <-- ������ �α��� ����
{
	int command_type; // LOGIC
	int command_code; // qS909
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		unsigned char message[50]; // �����ڵ尡 ������ ���, ���� �� ����
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s909_response;

typedef struct // S910 ��û������ (UI->LOGIC) : ����ť <-- �κг� ��, �������� (�ӽð����ȣ ���)
{
	int command_type; // LOGIC
	int command_code; // qS910
	struct
	{
		int part_amount; // ��ҷ� ������ ó���ϴ� (�κг�)�ݾ�
		unsigned char user_information[100]; // ����ó,�����̸�,���¹�ȣ,������ ����
		int error_code; // ���������ڵ�, 5:�Ա� ���ð� �ʰ�, 6:����ɸ�, 7:��ŸH/W����
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s910_ask;

typedef struct // S910 ���䵥���� (LOGIC->UI) : �۽�ť <-- �κг� ��, �������� (�ӽð����ȣ ���)
{
	int command_type; // LOGIC
	int command_code; // qS910
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		unsigned char message[50]; // ����ϴ� �ӽñ׷���ݹ�ȣ �Ǵ� �����޼���
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s910_response;

typedef struct // S911 ��û������ (UI->LOGIC) : ����ť <-- Ű����ũ ON/OFF
{
	int command_type; // LOGIC
	int command_code; // qS911
	struct
	{
		unsigned char kiosk_power; // 1:����ON, 2:����OFF
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s911_ask;

typedef struct // S911 ���䵥���� (LOGIC->UI) : �۽�ť <-- Ű����ũ ON/OFF
{
	int command_type; // LOGIC
	int command_code; // qS911
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		unsigned char message[50]; // �����ڵ尡 ������ ���, ���� �� ����
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s911_response;

typedef struct // S912 ��û������ (UI->LOGIC) : ����ť <-- H/W ������Ȳ ���
{
	int command_type; // LOGIC
	int command_code; // qS912
	struct
	{
		int error_code; // ���������ڵ�, 1:�����ɸ�,2:��������,3:����ɸ�,4:��ŸH/W����
		unsigned char user_phone[50]; // ������ ������ȣ(�������� �ĸ�(,) �����ڷ� ����)
		unsigned char message[50]; // ��������(������ �����ɸ�,����,����ɸ�,������� ��)
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s912_ask;

typedef struct // S912 ���䵥���� (LOGIC->UI) : �۽�ť <-- H/W ������Ȳ ���
{
	int command_type; // LOGIC
	int command_code; // qS912
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		unsigned char message[50]; // �����ڵ尡 ������ ���, ���� �� ����
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s912_response;

typedef struct // S913 ��û������ (UI->LOGIC) : ����ť <-- ������������ ������
{
	int command_type; // LOGIC
	int command_code; // qS913
} __attribute__((packed)) t_s913_ask;

typedef struct // S913 ���䵥���� (LOGIC->UI) : �۽�ť <-- ������������ ������
{
	int command_type; // LOGIC
	int command_code; // qS913
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		unsigned char message[50]; // �����ڵ尡 ������ ���, ���� �� ����
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s913_response;

typedef struct // S914 ��û������ (UI->LOGIC) : ����ť <-- ������ ������� �ܾ���ȸ
{
	int command_type; // LOGIC
	int command_code; // qS914
} __attribute__((packed)) t_s914_ask;

typedef struct // S914 ���䵥���� (LOGIC->UI) : �۽�ť <-- ������ ������� �ܾ���ȸ
{
	int command_type; // LOGIC
	int command_code; // qS914
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		unsigned char message[50]; // ������¹�ȣ �Ǵ� �����޼���
		int balance; // ������ �ܾ�
		unsigned char bank_name[20]; // ������¹�ȣ �����
		unsigned char account_user[50]; // ������¹�ȣ �����ָ�
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s914_response;

typedef struct // S921 ��û������ (UI->LOGIC) : ����ť <-- ��ġȮ��
{
	int command_type; // LOGIC
	int command_code; // qS921
} __attribute__((packed)) t_s921_ask;

typedef struct // S921 ���䵥���� (LOGIC->UI) : �۽�ť <-- ��ġȮ��
{
	int command_type; // LOGIC
	int command_code; // qS921
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		unsigned char patch_enable; // ��ġ�ʿ俩��, 'Y':��ġ, ��N':�ʿ����
		int patch_time; // 0~24
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s921_response;

typedef struct // S922 ��û������ (UI->LOGIC) : ����ť <-- ��ġ��û
{
	int command_type; // LOGIC
	int command_code; // qS922
} __attribute__((packed)) t_s922_ask;

typedef struct // S922 ���䵥���� (LOGIC->UI) : �۽�ť <-- ��ġ��û
{
	int command_type; // LOGIC
	int command_code; // qS922
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		unsigned char patch_url[128]; // ��ġ URL
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s922_response;

typedef struct // S923 ��û������ (UI->LOGIC) : ����ť <-- ��ġ�Ϸ�
{
	int command_type; // LOGIC
	int command_code; // qS923
} __attribute__((packed)) t_s923_ask;

typedef struct // S923 ���䵥���� (LOGIC->UI) : �۽�ť <-- ��ġ�Ϸ�
{
	int command_code; // qS923
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		unsigned char kiosk_version[10]; // ��ġ ��, Ű����ũ ����(ex. 1.0)
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s923_response;

typedef struct // S941 ��û������ (UI->LOGIC) : ����ť <-- ��ǰ�� ��ȯ�˻�
{
	int command_type; // LOGIC
	int command_code; // qS941
	struct
	{
		int cp_code; // �۴�ī��, ���ǸӴ�, ƾĳ�� ���
		unsigned char issue_date[8]; // �ɹ�������, yyyyMMdd, ��ȯ����
		unsigned char pin_manage_num[20]; // �� ������ȣ - ��8�ڸ���, (��ҹ�ȣ, UI �Էµ� ��ȣ 8�ڸ�)
//		int cp_amount; // �� �׸鰡, (�׸鰡)
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s941_ask;

typedef struct // S941 ���䵥���� (LOGIC->UI) : �۽�ť <-- ��ǰ�� ��ȯ�˻�
{
	int command_type; // LOGIC
	int command_code; // qS941
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		unsigned char message[50]; // �׷���ݹ�ȣ, ��ȯ��� �׷���ݹ�ȣ (���� ��ҿ� ���ݹ�ȣ)
		unsigned char pin_num[24]; // �� ��ȣ(����Ű), ��ȯ��� �� ��ȣ (Full) - ��ҿ�
		unsigned char pin_manage_num[20]; // �� ��ȣ(����Ű), ��ȯ��� Ȯ�ο� �� ������ȣ (Full) - ��ҿ�
		unsigned char pin_amount[10]; // �� �׸鰡, Ȯ�ο� �� �׸鰡
		unsigned char change_enable; // ��ȯ���� ����, 'Y':��ȯ����, 'N':��ȯ�Ұ�
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s941_response;

typedef struct // S942 ��û������ (UI->LOGIC) : ����ť <-- ��ǰ�� ��ȯ��û
{
	int command_type; // LOGIC
	int command_code; // qS942
	struct
	{
		int cp_code; // �۴�ī��, ���ǸӴ�, ƾĳ�� ���
		int cp_count; // �߱� �ż�, 1~9
		int cp_amount; // �߱� �ݾ�
		int issue_type; // �߱� ���, 1:�߱�, 2:SMS (�߱Ǹż��� 1���϶��� ����), 3:��ȯ�߱�, 4:��ȯSMS�߱�(�߱Ǹż��� 1���϶��� ����)
		unsigned char client_phone[16]; // SMS�߱ǽ� �� ��ȭ��ȣ
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_s942_ask;

typedef struct // S942 ���䵥���� (LOGIC->UI) : �۽�ť <-- ��ǰ�� ��ȯ��û
{
	int command_type; // LOGIC
	int command_code; // qS942
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
							// 1001:�������� �ʴ� CP�ڵ� �Դϴ�, 1002:CP����� ��ҿ�û�� �����Ͽ����ϴ�, 1099:���� ���� ������ �߻��Ͽ����ϴ�
							// 2010:�������� �ʴ� �����ȣ, 2011:�̹� ��� �Ϸ�� ��ҳ��� ����, 2012:��ұݾ� ����ġ, 2001:������ �ܸ��� ��Ȱ��ȭ Ȥ�� �ش��ϴ� ���ڵ� ����
							// 2002:�ܸ��� ��� ���� �̻� Ȥ�� ���ų��� ��ȸ ����, 2003:���ų������̺� ������Ʈ ����, 2020:���ų������̺� ����/���� ����
							// 2030:DB ó�� Exception �߻�, 2098:�ܼ� �߱ǿ� ���� �߱���� ����, 2099:���� �߱ǿ� ���� �Ϻ�/��ü�߱���� ����
} __attribute__((packed)) t_s942_response;
//----------------------------------------------------------------------------------------------
typedef struct // qBILL_COIN_ACCEPT ��û������ (UI->LOGIC) : ����ť <-- ����/���� �Է� ��û
{
	int command_type; 
	int command_code; // qBILL_COIN_ACCEPT
	struct
	{
		int target_amount; // �Է� ��ǥ �ݾ�
		int wait_time; // �ݾ� �Է� ���ð�
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_billcoin_ask;

typedef struct // qBILL_COIN_ACCEPT ���䵥���� (LOGIC->UI) : �۽�ť <-- ����/���� �Է� ����
{
	int command_type; 
	int command_code; // qBILL_COIN_ACCEPT
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		unsigned char machine; // bill:1, coin:2
		int input_amount; // �Է� �ݾ�
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_billcoin_response;
//----------------------------------------------------------------------------------------------
typedef struct // PRINT ���� �̺�Ʈ ������ (LOGIC->UI) : �۽�ť <-- ����Ʈ �̺�Ʈ
{
	int command_type; 
	int command_code; // qPRINT_START, qPRINT_COMPLETE, qPRINT_ERROR
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		int progress; 		// 1~100% ������
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_print_response;
//----------------------------------------------------------------------------------------------
typedef struct // ETHERNET ���� �̺�Ʈ ������ (LOGIC->UI) : �۽�ť <-- �̴��� �̺�Ʈ
{
	int command_type; 
	int command_code; 	// qETHERNET_CONNECT, qETHERNET_SEND_RECV, qETHERNET_COMPLETE, // qETHERNET_ERROR
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		int progress; 		// 1~100% ������, ETHERNET_SEND_RECV �̺�Ʈ���� ������
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_ethernet_response;
//----------------------------------------------------------------------------------------------
typedef struct // MODEM ���� �̺�Ʈ ������ (LOGIC->UI) : �۽�ť <-- �� �̺�Ʈ
{
	int command_type; 
	int command_code; 	// qMODEM_CONNECT, qMODEM_SEND_RECV, qMODEM_COMPLETE, qMODEM_ERROR
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		int progress; // 1~100% ������, ETHERNET_SEND_RECV �̺�Ʈ���� ������
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_MODEM_response;
//----------------------------------------------------------------------------------------------
typedef struct // USB ��û������ (UI->LOGIC) : ����ť <-- üũ, �ٿ�ε�
{
	int command_type; 
	int command_code; 	// qUSB_CHECK, qUSB_DOWNLOAD
} __attribute__((packed)) t_usb_ask;

typedef struct // USB ���䵥���� (LOGIC->UI) : �۽�ť <-- üũ, �ٿ�ε�
{
	int command_type; 
	int command_code; // qUSB_CHECK, qUSB_DOWNLOAD
	int response_code; // 0:����, �ܴ̿� �������ڵ� ����
	struct
	{
		int progress; // 1~100% ������, ETHERNET_SEND_RECV �̺�Ʈ���� ������
	} __attribute__((packed)) t_field;
} __attribute__((packed)) t_usb_response;
//----------------------------------------------------------------------------------------------
typedef struct
{
	int msg_type; // 1 ���� ���´�
	union
	{
		struct
		{
			int command_type; // LCD, PRINTER, BILL, COIN, ETHERNET, MODEM, LOGIC, MSR, IC, USB...
			int command_code; // ������ ����ڵ� (1~99)
			int response_code; // 0:����, �ܴ̿� �������ڵ� ����
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
    
    struct { // ��û������ ���� �ʵ�
		byte transcode[4+1];					// �ŷ������ڵ�
        byte unique_id[20+1];					// Unique ID, 20
        byte ip_address[39+1];   				// KIOSK IP �ּ�
        byte cp_code[4+1];    					// CP �ڵ�, CP�ڵ� ���̺� ����
        byte issue_count[2+1];   				// �߱Ǹż�, "01"~"09"
        byte issue_amount[10+1]; 				// �߱Ǳݾ�
        byte temp_group_pay_num[50+1];  		// �ӽñ׷���ݹ�ȣ, ��ǰ��� üũ�������� ���� �ӽñ׷���ݹ�ȣ�� ����
        byte approval_code[2+1];   			// ��������ڵ�, ���� ������ ������� (���ݡ�03��, RF"04", �ſ�ī�塰05��)
        byte pg_tran_num[50+1]; 				// PG�� �ŷ���ȣ, ���ݰ��� ������ PG�� �ŷ���ȣ�� ���� ���� ��ü�� Space�� ����
        byte pg_approval_num[50+1];			// PG�� ���ι�ȣ, ���ݰ��� ������ PG�� ���ι�ȣ�� ���� ���� ��ü�� Space�� ����
        byte pg_tran_date[14+1];    			// PG�� �ŷ��Ͻ�, PG�� �ŷ��Ͻø� �ƴ°�� ����(YYYYMMDDHHMMSS), �𸣴°��, ���Ͻð� ����
        byte payed_tamount[10+1];    			// ���� ������ �ѱݿ�, 1,004������ �߱ǽ� �׽�Ʈ �߱�
        byte issue_type[1+1];    				// �߱ǹ��, 1:�߱�, 2:SMS(1���϶��� ����)
        byte client_phone[16+1]; 				// ������ó, SMS�߱ǽ� ����ȭ��ȣ
        byte client_phone2[20+1]; 			// ������ó
        byte group_pay_num[50+1];   			// �׷���ݹ�ȣ, �߱ǿ�û ���������� �׷���ݹ�ȣ
        byte cancel_type[1+1];    			// �������, ������ �߱����(��T'), ������ �ܰ����('P'), �� �ܰ����('C')
        byte pin_date[8+1];    				// �� ��������, YYYYMMDD - �ܰ���ҿ���
        byte pin_num[24+1];	   				// �� ��ȣ(����Ű), �ܰ���ҿ���
        byte pin_manage_num[20+1];   			// �� ������ȣ,��C'�ܰ���ҿ��� ��8�ڸ���
        byte pin_issue_amount[10+1];   		// �� �׸鰡
        byte can_amount[10+1];   				// �߱���ұݾ�
        byte client_bank_name[20+1];		    // �����̸�
        byte client_account[20+1];   			// ������¹�ȣ
        byte client_name[24+1];   			// ���࿹���ָ�
        byte error_code[2+1];    				// �����ڵ�
        byte cash_num[20+1];    				// ĳ�ù�ȣ
        byte error_type_code[2+1];  			// ���� �����ڵ�
        byte etc_information[100+1]; 		// ��Ÿ����
        byte adjust_ref_time[19+1]; 			// ����Ȯ�� ���ؽð�
        byte adjust_run_time[19+1];	 		// ���긶�� ���ؽð�
        byte temp_payed_amount[10+1]; 		// �κг��ݾ�
        byte kiosk_total_amount[10+1]; 		// Ű����ũ�� ������ �ִ� �ѱݾ�
        byte admin_pw[6+1];			 		// ������ ��й�ȣ
        byte kiosk_status[1+1]; 				// '1':����ON, '2':����OFF
        byte error_message[50];				// ��������, ������ �����ɸ�, ��������, ����ɸ�, ������� ��
        byte self_manage_seq_num[20+1];		// ���� �����Ϸù�ȣ
//        byte trans_code[4+1];					// �ŷ������ڵ�
        byte account_ref_time[19+1];			// ������ȸ ���ؽð�
        byte kiosk_version[10+1];				// Ű����ũ ����
    } Send;

    struct {
		// ���������� ���� �ʵ�
		byte resp_transcode[4+1];				// ���䱸���ڵ�
		byte resp_terminal[16+1];				// �͹̳��̸�
		byte resp_shop_serial[10+1];			// ���� ������ �ø����ȣ
		byte resp_kiosk_serial[20+1];		// ���� ���ΰ����Ϸù�ȣ
		byte resp_message[50+1];				// ����޽���
		byte shop_name[50+1];					// ��������
		byte vaccount_use[1+1];				// ������� ��뿩��, '1':���, '2':����
		byte temp_group_pay_num[50+1];		// �ӽ� �׷���ݹ�ȣ
		byte cash_num[20+1];					// ������� ��뿩�ο� ���� ������ �ݾ� ���п�
		byte fix_group_pay_num[50+1];		// Ȯ�� �׷���ݹ�ȣ
		byte group_pay_num[50+1];				// �׷���ݹ�ȣ
		byte issue_count[2+1];				// �߱Ǹż�
		//---------- �ݺ����� ----------------------------------------------------------------
		byte pin_num[9][24+1];					// �ɹ�ȣ(����Ű)
		byte pin_manage_num[9][20+1];		// �� ������ȣ
		byte pin_issue_amount[9][10+1];		// �� �׸鰡
		byte issue_date[9][10+1];				// ������, "20140222" (���ǸӴ� �ܿ��� �����)
		byte pin_manage_num2[9][20+1];       // ���ǸӴϸ� �ٸ�
		//------------------------------------------------------------------------------------
		byte event_message[1000+1];			// �̺�Ʈ�޽���, �Ǹ�����
		byte event_message_pos[1+1];			// �̺�Ʈ�޽��� ��ġ, "1':��, '2':��
		byte vaccount_balance[20+1];			// ��������ܾ�
		byte cancel_ok_count[2+1];			// �߱���� �����ż�
		byte cancel_tcount[2+1];				// �߱���� ��ü�ż�
		//---------- �ݺ����� ----------------------------------------------------------------
		byte cancel_unit_num[9][2+1];		// �߱���� ����, "01"~"09"
		byte cancel_success[9][1+1];			// ��� ���� ����, 'Y':��ҿϷ�, 'N':��ҽ���
		byte cancel_fail_code[9][4+1];		// ��� ���� �ڵ�, ��� ����ڵ� ����
		//---------- �ݺ����� ----------------------------------------------------------------
		byte vaccount[50+1];					// �����������, �ܸ��⿡ �ο��� ������¹�ȣ �Ǵ� �����޽���
		byte adjust_run_time[19+1];			// ���긶�� ���ؽð�, YYYY-MM-DDHH24:MI:SS ���·� ��ȯ
		byte not_adjust_tamount[10+1];		// ������ �Ѿ�, ������ ���긶�� ��û �ĺ��� ��������� �Աݵ� �ݾ��� �հ踦 ��ȯ(�������) -> ����߱Ǿ�+�κг� ���� ����� ������+�߱� ���е� ������
		byte not_adjust_sale_tamount[10+1];	// ������ �Ǹ��Ѿ�, �Ǹű��Ѿ�(����߱Ǳݾ�+�߱���ұݾ�)
		byte not_adjust_temp_payed_tamount[10+1];	// ������ �κг��Ѿ�
		byte old_adjust_time[19+1];			// ���� ������ �����ð�, YYYY-MM-DDHH24:MI:SS ���·� ��ȯ
		byte tamount_for_adjust[10+1];		// ������ ���긶�� ��û �ĺ��� ��������� �Աݵ� �ݾ��� �հ踦 ��ȯ(�������) -> �Ǹ��Ѿ�, ��ұݾ�, �κг� ��ұݾ�
		byte sale_tamount[10+1];				// �Ǹ��Ѿ�
		byte cancel_tamount[10+1];			// ��ұݾ�
		byte cancel_temp_payed_amount[10+1];	// �κг� ��ұݾ�
		byte cancel_in_adjust[10+1];			// ���긶���� ��ұݾ�, ���� ���긶���ð� �������� �̹� ������ ���� �� �� ��ҵ� �ݾ�
		byte vaccount_bank_name[20+1];		// ������¹�ȣ �����
		byte vaccount_bank_user[50+1];		// ������¹�ȣ �����ָ�
		byte reboot_enable[1+1];				// ������ ����, 'Y':������, 'N':������ ����
		byte reboot_start_time[3+1];			// ������ �ð�, "005":5����, "010":10����, "900":15�ð���
		byte patch_enable[1+1];				// ��ġ����, 'Y':��ġ, 'N':��ġ �ʿ� ����
		byte patch_start_time[2+1];			// ��ġ�ð�, "00"~"24"
		byte patch_url[128+1];				// ��ġURL, ex) http://patch.teencash.co.kr/patch.aspx
		byte kiosk_version[10+1];				// Ű����ũ ����
		byte change_enable;					// ��ȯ���� ����, 'Y':��ȯ����, 'N':��ȯ�Ұ�
    } Receive;

    byte ymdsms[12+1];                      // YYMMDDSSMMSS
    byte respcode[4+1];                     // �����ڵ� // ����
    ulong tamount;
    ulong amount;
    byte  Reprt;
    byte  BfrCancel;
    byte  netcan_flag;                     // ��������÷���(TRUE=�����)
    byte  MakeTimeStamp[16+1];             // ���� ���鶧 ���Ǵ� YYMMDD, �� ���� Working Key�� ���Ǿ����Ƿ� �ʱ⿡ �ѹ� �а�, ���������� �����.
    byte  register_no[10+1];              // ����ڹ�ȣ
    int   comm_line;                      // ��Ŷ���
    byte  dcat_id[10+1];                  // download �� cat-id
    byte RCV_MSG[32+3];                  	// FI �ٿ�ε�, TMS CHECK �ŷ��� ������ ����� ����޼���
    byte update_flag;						// TMS CHECK �ŷ��� ����.(1 �����ʿ�, 2 ������)
    byte SeqNo[4+1];                       // ��ǥ������ȣ
    byte  track2_data[64];
} KIOSK_ID_FIELD;
//----------------------------------------------------------------------------------------------

HNX720T_EXT KIOSK_ID_FIELD kField, kField2;
HNX720T_EXT KIOSK_ID_FIELD kBfrField;

typedef struct {
    byte changed;       
    byte machine_no[10+1];					// �ܸ��� ����ȣ
    byte identification_no[11+1];           // �ܸ��� ������ȣ 
    byte ymdsms[16+1];                       // ����
} FL_REGION_MA;
HNX720T_EXT FL_REGION_MA RegionMA;

typedef struct
{
    byte changed;
    
    byte mem_cnf[16];                      // �޸��ʱ�ȭ����

    // ����������
    struct {
        byte registration[32+1];                 // �ܸ��� ��Ͽ���
        byte cat_id[20+1];                       // �ܸ��� ID	(��ȣ)		// DownField.L002
        byte register_no[10+1];                  // ����ڹ�ȣ			// DownField.L003
        byte retailer_name[48+1];                // ������ ��ȣ			// DownField.L004
        byte retailer_address[300+1];           // ������ �ּ�			// DownField.L005
        byte represent_name[32+1];               // ��ǥ�ڸ�				// DownField.L006
        byte retailer_phone[32+1];               // ������ ��ȭ			// DownField.L007
        byte helpdesk_phone[32+1];               // ������ ��ȭ			// DownField.L008

        byte key_index[2+1];                     // Working Key Index		// DownField.L009
        byte working_key[16+1];                  // Working Key			// DownField.L010
        byte sub_register_no[10+1];              // ���� ����ڹ�ȣ		// DownField.L043
        byte campain_message[404+1];             // ����޼���			// DownField.L039
        byte shop_serial[10+1];					// ������ �ø����ȣ
        byte shop_name[50+1];						// ��������
        byte vaccount_enable;						// ������»�뿩��, 1���, 0����
    } Merchant;

    // ���� ��������
    struct {
        int tm01;                             // ���Ӵ��ð�(30��)
        int tm02;                             // �������ӽð�(20��)
        int enq_tm;                           // ������ð�(10��)
        int ack_tm;                           // ACK ���ð�(15��)
        int eot_tm;                           // ������ð�(05��)
        int resp_tm;                          // ��Ŵ��ð�(45��)
        int redial_tm;                        // ����̾��غ�ð�(3��)		

        // ���Ӽ���(PSTN_LINE, C2I_LINE, CDMA_LINE, CHECK_LINE, WIBRO_LINE, ETHERNET_LINE, ETHMODEM_LINE, WCDMA_LINE)        
        int comm_line; 
		int private_bps; 
		byte use_comm_type; 

        struct { 
            byte downtel[16+1];                // �ٿ�ε���ȭ��ȣ			// DownField.L011
            byte apptel_01[16+1];              // ������ȭ01				// DownField.L012
            byte apptel_02[16+1];              // ������ȭ02				// DownField.L013
            byte jasatel_01[16+1];             // �ڻ������ȭ01			// DownField.L014
            byte jasatel_02[16+1];             // �ڻ������ȭ02			// DownField.L015
            byte tmstel_01[16+1];              // TMS��ȭ01				// DownField.L016
            byte tmstel_02[16+1];              // TMS��ȭ02				// DownField.L017
            byte pabx_on;                      // ��ȭ������뿩��(SW_ON, SW_OFF, SW_OFF)
            byte dial_spd;                     // ���̾�ӵ�(������=1, ����=2, ������=3, 3)
            byte dialtone_on;                  // ���̾��� üũ(��=1,�ƴϿ�=2, 1) 
            byte pabx_no[3+1];                 // ����ȣ���ȣ(0~9,*,#, 9) 
            byte pabx_wait;                    // ����ȣ��ð�(1~6, 0)
            int  pstn_spd;                     // ��ȭ���ӵ�(1200=1, 2400=2, 1)
            byte predial_on;                   // PREDIAL ����(1:��, 2:�ƴϿ�)
        } Pstn;

        struct { 
            byte ipmethod;                     // (1.�ڵ�, 2.����, 1)
            byte ipkind;                       // ����IP����(1.�����θ���, 2.IP�������)
            byte address[16+1];                // host ���� IP address		// DownField.L033
            byte portno[10+1];                  // host ���� port no		// DownField.L034
            byte tmsportno[10+1];               // tms  ���� port no		// DownField.L035
            byte staticIP[16+1];               // ���� IP
            byte staticSM[16+1];               // ���� Subnet MASK
            byte staticGW[16+1];
            byte dns_server01[18+1];
            byte dns_server02[18+1];
            byte domain_name[64+1];
            byte domain_ip[16+1];
            byte phone_change;                 // ��ȭ�����ӿ���(1:��, 2:�ƴϿ�) 
        } Ethernet;

    } Comm;
 
    struct {
        struct {
            byte cutting_on;                    // ��������(1:����, 2:�κ�����, 2)
            byte print_speed;                   // �μ�ӵ�(1.�ſ����, 2:������, 3:����, 4:������)
            byte print_concentration;           // �μ��(1:����, 2:���ϰ�)
        } PrtInfo;
        struct {
            byte background_sound;   // ����� ���� (1���, 0�̻��)
            byte touch_sound;         // ��ġ�� ���� (1���, 0�̻��)
            byte volume;           	// ������ (����Ʈ120, 70~127 ����)
        } SoundInfo;
	} Terminfo; 

	struct { 
        byte logo_prt_type;					// ��ǥ��½� LOGO �μ⿩��(1.����, 2.Tpay, 1) 
    } Thermal_Info;

	struct {
	    byte  ymdsms[12+1];                     // �����ŷ��Ͻ�
		byte adjust_run_time[19+1];			// ���� ���ؽð� (�����κ��� ���� ��) YYMMDDHHMMSS
    } Jungsan_Info;

	struct {
        byte auto_renew_on;						// �ڵ�����ó��(1:��, 2:�ƴϿ�, 2)
        byte tms_comm_line;						// TMS��ż���(1:�������, 2:��ȭ��, 1)
    } Tms_Info;

	struct {
		byte temp_group_pay_num[50+1];
		byte temp_cp_code[4+1];
		byte temp_payed_amount[10+1];
	    ulong bill_target_amount; 			// �����νı� ��ǥ�ݾ� �޽����� ���� �� (�μ�� ���)
	    ulong bill_input_amount; 				// �����νı� ��ǥ�ݾ� �޽����� ���� �� (�μ�� ���)
	    byte temp_pin_num[24+1];
		byte temp_pin_manage_num[20+1];
	} Issue_Info;
	byte log_depth;
	byte debug_depth;
} FL_REGION_01;
HNX720T_EXT FL_REGION_01 Region01;
HNX720T_EXT FL_REGION_01 Region01_backup;

typedef struct {                            //����� ����(���곻���� ��������)
    byte changed;
    
    uint  c_qrear, c_qfront;                // CAPTURE qurue rear, front
    byte  ymdsms[12+1];                     // �����ŷ��Ͻ�
    struct {
       int sequence_num;                    // Sequence-number(��ǥ������ȣ)(0000 ~ 9999)
       byte yymmdd[6+1];					// ���ڰ� ����Ǿ����� ���θ� �˱����� �������.
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

	byte ymdsms[12+1];	 // ������ ������Ʈ�� �����ϴ� �ð�				
	byte last_tms_ymdsms[12+1]; // ������ ������Ʈ�� �� �ð�
	byte updateflag;					
	byte auto_request_flag;
	byte check_ymdsms1[12+1]; // patch_check ������ ������ �ð� ����
	byte check_ymdsms2[12+1]; // patch_check ������ ������ �ð� ����
} FL_REGION_04;
HNX720T_EXT FL_REGION_04 Region04;
HNX720T_EXT FL_REGION_04 Region04_backup;

typedef struct {                            //���� ������
    byte servkind; // 1

	byte trans_date[12]; // 12, �ŷ��Ͻ�
    int cp_code;	// 4
    byte approval_type; // 1, �������� (����3, RF4, �ſ�5
    byte cp_tcount; 	// 1, �ѹ߱Ǹż�
    int tamount; 		// 4, �ŷ� �ѱݾ�
    byte ticket_type;	// 1, �߱�1, sms2
    byte ticket_user_phone[16]; // 16, sms�϶� ��ȭ��ȣ
    byte fix_group_pay_num[50]; // 50, Ȯ���׷���ݹ�ȣ �Ǵ� �ӽñ׷���ݹ�ȣ(ȯ�ҿ�)
//    byte pin_num[9][24]; // 24, �ɹ�ȣ
//    byte pin_manage_num[9][20]; // 20, �ɰ�����ȣ
//    int pin_issue_amount[9]; // 36, �߱ǳ���ݾ�(�׸鰡)
//    byte pin_issue_date[9][6]; // 54, ������, 140222
//    byte pin_manage_num2[9][20]; // �ɰ�����ȣ�� ����(���ǸӴϸ� �ٸ�)

	byte cancel_type; // 1, �������, �����ڹ߱����T, �����ڴܰ����P, ���ܰ����C
	byte pin_issue_date[8]; //8, �ܰ���ҵ� ��ǰ���� ��������
    byte pin_num[24]; // 24, �ɹ�ȣ
    byte pin_manage_num[20]; // 20, �ɰ�����ȣ
    byte error_type_code; // 1, ���������ڵ�, �Աݴ��ð��ʰ�5, ����ɸ�6, ��ŸH/W����7
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

HNX720T_EXT int msg_cancel; //�޽���ť���� ��Ҹ�ɽ� ����� ������
HNX720T_EXT int now_idle; // ��Ҹ�ɽ� idle=1 ���¿����� msg_cancel�� ����� ������ �ʴ´�.
HNX720T_EXT byte field_not_erase; // 0����, 1������

HNX720T_EXT int tms_check_flag;
HNX720T_EXT int tms_update_flag;
HNX720T_EXT byte first_modem_test_flag;
HNX720T_EXT byte network_cancel_flag;
//----------------------------------------------------------------------------------------------

