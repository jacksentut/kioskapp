#include "include.h"

void ethernet_status_check(void);
extern void *MsgTread(void * data);
extern void *MsgTread2(void * data);
extern void *MsgTread4(void * data);

struct {
    byte step;
} Ethernet_scrn;

struct {
    byte step;
} Else_scrn;

void linesta_chk_func(void)
{
    if(Region01.Comm.comm_line & ETHERNET_LINE)
    {
        switch(Ethernet_scrn.step)
        {
            case 0:
                api_set_apptimer(dtimer3, 100);
                Ethernet_scrn.step++;
                break;
				
            case 1:
                if(api_chk_apptimer(dtimer3)== FALSE )
                {
                    switch(Eth_info.step)
                    {
                        case 1:
                            if(ethernet_link_chk()==1)
                            {
                                if(Eth_info.IP_state == 0)
                                {
                                    if(dns_info_check())
                                        Eth_info.IP_state = 0;
                                    else
                                    {                  
                                        write_dns_info();
                                        Eth_info.IP_state=1;
                                    }
                                }
                            }
                            else
                            {
                                Eth_info.step = 2;
                            }
                            Ethernet_scrn.step = 0;
                            break;
                        case 2:
                        case 3:
                        case 4:
                            if(ethernet_link_chk()==1)
                            {
                                Eth_info.step++;
                                api_set_apptimer(dtimer3, 10);
                            }
                            else
                            {
                                Ethernet_scrn.step = 0;
                            }
                            break;
                            
                        case 5:
                        default:
                            aDebugMsg("  인터넷설정중  ");
                            aDebugMsg("잠시만기다리세요");
                            init_ethernet();
                            KIOSK_STEP = 0;
                            break;
                    }
            	}
        }
    }
    else
    {
        switch(Else_scrn.step)
        {
            case 0:
                api_set_apptimer(dtimer3, 100);
                Else_scrn.step++;
                break;
				
            case 1:
                if(api_chk_apptimer(dtimer3)== FALSE )
                {
                    memset(&Else_scrn, 0, sizeof(Else_scrn));
                }
                break;
				
            default:
                memset(&Else_scrn, 0, sizeof(Else_scrn));
                break;
        }
    }
}

byte prtpaper_chk(void)
{
	int status = 0;

	status = print_status();
	if(status == 0)
		return(FALSE);
	else
		return(TRUE);
}
//-----------------------------------------------------------------------------------------------------
void prt_test1(unsigned int i)
{
    byte temp[512]={0};

    if (print_start(SW_ON, 0)==0) return;
    sprintf(temp,"%cInfoCrypt PRTTEST%4d%c\n",0x0e,i+1,0x0f);
    prtqueue_insert(temp);
    sprintf(temp,"%c01234567890123456789%c\n",0x0e,0x0f);
    prtqueue_insert(temp);
    sprintf(temp,"%cABCDEFGHIJKLMNOPQRST%c\n",0x0e,0x0f);
    prtqueue_insert(temp);
    sprintf(temp,"%cUVWXYZabcdefghijklmn%c\n",0x0e,0x0f);
    prtqueue_insert(temp);
    sprintf(temp,"%copqrstuvwxyz~!@#$^&*%c\n",0x0e,0x0f);
    prtqueue_insert(temp);
    sprintf(temp,"%copqrstuvwxyz~!@#$^&*%c\n",0x0e,0x0f);
    prtqueue_insert(temp);
    prtqueue_insert("가나다라마바사아자차카타파하아야어여오요우유으이\n");
    prtqueue_insert("ABCDEFGHIJKLMNOPQRSTUVWXYZ~!@#$%^&*()_-+12345678\n");
    prtqueue_insert("abcdefghijklmnopqrstuvwxyz=|?<>,./~!@#$%12345678\n");
    // 40자로 지정
    sprintf(temp, "%c%c%c%c%c%c%c%c%c%c", 0x1b,0x57,0x30,0x00,0x00,0x00,0x10,0x02,0x00,0x00);
    prtqueue_insert(temp);

    sprintf(temp, "%c℃Å￠￡￥♂♀∠⊥⌒∂∇≡≒§※☆★○●◎◇◆□■△▲▽▼→←↑↓↔〓≪≫√∽∝∵∫∬∈∋⊆⊇⊂⊃∪∩∧∨￢⇒⇔∀∃ː∮∑∏¤℉‰◁◀▷▶♤♠♡♥♧♣◈▣◐◑▒▤▥▨▧▦▩♨☏☎☜☞¶†‡↕↗↙↖↘♭♩♪♬㉿㈜№㏇™㏂㏘℡®%c\n",SO,SI);
    prtqueue_insert(temp);

    sprintf(temp, "%c㉠㉡㉢㉣㉤㉥㉦㉧㉨㉩㉪㉫㉬㉭㉮㉯㉰㉱㉲㉳㉴㉵㉶㉷㉸㉹㉺㉻%c\n", SO, SI);
    prtqueue_insert(temp);
    
    sprintf(temp, "㉠㉡㉢㉣㉤㉥㉦㉧㉨㉩㉪㉫㉬\n");
    prtqueue_insert(temp);
    
    sprintf(temp, "ⓐⓑⓒⓓⓔⓕⓖⓗⓘⓙⓚⓛⓜ\n");
    prtqueue_insert(temp);

    prtqueue_insert("ABCDEFGHIJKLMNOPQRSTUVWXYZ~!@#$%^&*()_-+12345678\n");
    
    // 40자로 지정취소
    sprintf(temp, "%c%c%c%c%c%c%c%c%c%c", 0x1b,0x57,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);
    prtqueue_insert(temp);

    // 42자로 지정
    sprintf(temp, "%c%c%c%c%c%c%c%c%c%c", 0x1b,0x57,0x18,0x00,0x00,0x00,0x10,0x02,0x00,0x00);
    prtqueue_insert(temp);

    sprintf(temp, "%c%c%c", 0x1b, 0x21, (DOUBLE_HIGHT_MODE | DOUBLE_WIDTH_MODE));
    prtqueue_insert(temp);
    
    sprintf(temp, "℃Å￠￡￥♂♀∠⊥⌒∂∇≡≒§※☆★○●◎◇◆□■△▲▽▼→←↑↓↔〓≪≫√∽∝∵∫∬∈∋⊆⊇⊂⊃∪∩∧∨￢⇒⇔∀∃ː∮∑∏¤℉‰◁◀▷▶♤♠♡♥♧♣◈▣◐◑▒▤▥▨▧▦▩♨☏☎☜☞¶†‡↕↗↙↖↘♭♩♪♬㉿㈜№㏇™㏂㏘℡®\n");
    prtqueue_insert(temp);

    sprintf(temp, "㉠㉡㉢㉣㉤㉥㉦㉧㉨㉩㉪㉫㉬㉭㉮㉯㉰㉱㉲㉳㉴㉵㉶㉷㉸㉹㉺㉻\n");
    prtqueue_insert(temp);

    sprintf(temp, "㉠㉡㉢㉣㉤㉥㉦㉧㉨㉩㉪㉫㉬\n");
    prtqueue_insert(temp);
    
    sprintf(temp, "ⓐⓑⓒⓓⓔⓕⓖⓗⓘⓙⓚⓛⓜ\n");
    prtqueue_insert(temp);
    
    // 42자로 지정취소
    sprintf(temp, "%c%c%c%c%c%c%c%c%c%c", 0x1b,0x57,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);
    prtqueue_insert(temp);

    sprintf(temp, "%c%c%c", 0x1b, 0x21, 0x00);
    prtqueue_insert(temp);
    print_end();
}

void kiosk_comm_test(void)
{
	byte chs[20];
	byte ret;
	int in;

	printf("\n고정IP 설정? ");
	printf("\n1.예     2:아니오\n");
	scanf("%s", chs);
	in = atoi(chs);
	if(in == 1)
	{
		printf("\n고정IP (112.217.109.92) : ");
		scanf("%s", Region01.Comm.Ethernet.staticIP);
		printf("\n고정SM (255.255.255.248): ");
		scanf("%s", Region01.Comm.Ethernet.staticSM);
		printf("\n서버GW (112.217.109.89) : ");
		scanf("%s", Region01.Comm.Ethernet.staticGW);
		ret = init_ethernet();
		if(ret==FALSE)
		{
		    aDebugMsg("네트워크설정실패");
		    aDebugMsg(" 재시도바랍니다 ");
		}
	}
//    sprintf(Region01.Comm.Ethernet.staticIP,    "%s", "112.217.109.92");
//    sprintf(Region01.Comm.Ethernet.staticSM,    "%s", "255.255.255.248");
//    sprintf(Region01.Comm.Ethernet.staticGW,    "%s", "112.217.109.89");
//    sprintf(Region01.Comm.Ethernet.dns_server01,"%s", "168.126.63.1");
//    sprintf(Region01.Comm.Ethernet.dns_server02,"%s", "168.126.63.2");

	printf("\n1.S900 설치인증");
	printf("\n2.S901 상품별 판매가능 여부 체크");
	printf("\n3.S902 상품 재고 체크");
	printf("\n4.S903 발권 요청");
	printf("\n5.S905 발권/단건 취소");
	printf("\n6.S906 운영오류상황 기록");
	printf("\n7.S907 정산마감 금액 확인");
	printf("\n8.S908 정산마감 처리");
	printf("\n9.S909 관리자 로그인 인증");
	printf("\n10.S910 부분납후,강제종료 취소");
	printf("\n11.S911 키오스크 On/Off");
	printf("\n//12.S912 H/W 오류상황 기록");
	printf("\n//13.S913 서버응답전문 재전송");
	printf("\n14.S914 가맹점 가상계좌 잔액조회");
	printf("\n15.S915 원격리부팅");
	printf("\n16.S921 패치확인");
	printf("\n17.S922 패치요청");
	printf("\n18.S923 패치완료");
	printf("\n99.메모리 초기화");
	printf("\n19.변수 요청");
	printf("\n20.테스트발권 요청");
	printf("\n//21.프로그램 USB 업데이트 요청");
	printf("\n//22.프로그램 TMS 업데이트 요청");
	printf("\n23.볼륨설정 요청");
	printf("\n24.터치음 설정 요청");
	printf("\n25.배경음 설정 요청");
	printf("\n26.랜(고정/유동)설정 요청");
	printf("\n27.고정IP 설정 요청");
	printf("\n28.서버IP 설정 요청");
	printf("\n29.USB 프로그램 업데이트");
	printf("\nQ=quit\n");

#ifdef REAL_LINE
	memcpy(msgq_buf.qs900_ask.t_field.shop_serial_num, "9191010008", 10);
#else
	memcpy(msgq_buf.qs900_ask.t_field.shop_serial_num, "9191010009", 10);
#endif
	sprintf(Region01.Merchant.shop_serial, "%10.10s", msgq_buf.qs900_ask.t_field.shop_serial_num);
	//sprintf(Region01.Merchant.cat_id, "%s", "1406180001");

	ret = init_ethernet();
	if(ret==FALSE)
	{
	    aDebugMsg("네트워크설정실패");
	    aDebugMsg(" 재시도바랍니다 ");
	}
	
	while(1)
	{
		if(field_not_erase==0)
			init_val();
		scanf("%s", chs);
		if(chs[0]=='q')
			in=9999;
		else
			in=atoi(chs);
		switch(in)
		{
			case 1:
				KIOSK_SERVKIND = REG_DOWNLOAD;
				reg_download();
				break;
			case 2:
				KIOSK_SERVKIND = SALE_ENABLE_CHECK;
				sale_enable_check();
				break;
			case 3:
				KIOSK_SERVKIND = STOCK_CHECK;
				stock_check(0);
				break;
			case 4:
				KIOSK_SERVKIND = ISSUE_APP;
				issue_app(0);
				break;
			case 5:
				KIOSK_SERVKIND = ISSUE_CAN;
				issue_can();
				break;
			case 6:
				KIOSK_SERVKIND = OPERATION_CHECK;
				operation_check();
				break;
			case 7:
				KIOSK_SERVKIND = ADJUST_CHECK;
				adjust_check();
				break;
			case 8:
				KIOSK_SERVKIND = ADJUST_APP;
				adjust_app();
				break;
			case 9:
				KIOSK_SERVKIND = ADMIN_LOGIN_AUTH;
				admin_login_auth();
				break;
			case 10:
				KIOSK_SERVKIND = TEMP_CANCEL;
				temp_cancel();
				break;
			case 11:
				KIOSK_SERVKIND = KIOSK_ONOFF;
				kiosk_onoff();
				break;
			case 14:
				KIOSK_SERVKIND = VACCOUNT_BALANCE;
				vaccount_balance();
				break;
			case 15:
				KIOSK_SERVKIND = REMOTE_REBOOT;
				remote_reboot();
				break;
			case 16:
				KIOSK_SERVKIND = PATCH_CHECK;
				patch_check();
				break;
			case 17:
				KIOSK_SERVKIND = PATCH_REQUEST;
				patch_request();
				break;
			case 18:
				KIOSK_SERVKIND = PATCH_COMPLETE;
				patch_complete();
				break;
			case 99:
				app_memory_init();
				break;
			case 19:
				variable_response();
				break;
			case 20:
				test_issue_response();
				break;
			case 23:
				volume_set();
				break;
			case 24:
				tsound_set();
				break;
			case 25:
				bsound_set();
				break;
			case 26:
				lantype_select();
				break;
			case 27:
				lan_static_ip_set();
				break;
			case 28:
				lan_server_ip_set();
				break;
			case 29:
				kiosk_usb_update();
				break;
			case 9999: case 'Q': 
				return;
		}
		printf("\n1.S900 설치인증");
		printf("\n2.S901 상품별 판매가능 여부 체크");
		printf("\n3.S902 상품 재고 체크");
		printf("\n4.S903 발권 요청");
		printf("\n5.S905 발권/단건 취소");
		printf("\n6.S906 운영오류상황 기록");
		printf("\n7.S907 정산마감 금액 확인");
		printf("\n8.S908 정산마감 처리");
		printf("\n9.S909 관리자 로그인 인증");
		printf("\n10.S910 부분납후,강제종료 취소");
		printf("\n11.S911 키오스크 On/Off");
		printf("\n//12.S912 H/W 오류상황 기록");
		printf("\n//13.S913 서버응답전문 재전송");
		printf("\n14.S914 가맹점 가상계좌 잔액조회");
		printf("\n15.S915 원격리부팅");
		printf("\n16.S921 패치확인");
		printf("\n17.S922 패치요청");
		printf("\n18.S923 패치완료");
		printf("\n99.메모리 초기화");
		printf("\n19.변수 요청");
		printf("\n20.테스트발권 요청");
		printf("\n//21.프로그램 USB 업데이트 요청");
		printf("\n//22.프로그램 TMS 업데이트 요청");
		printf("\n23.볼륨설정 요청");
		printf("\n24.터치음 설정 요청");
		printf("\n25.배경음 설정 요청");
		printf("\n26.랜(고정/유동)설정 요청");
		printf("\n27.고정IP 설정 요청");
		printf("\n28.서버IP 설정 요청");
		printf("\n29.USB 프로그램 업데이트");
		printf("\nQ=quit\n");
	}
}
void on_line(void)
{
	printf("\n*********** Welcome to ******************");
	printf("\n*      Kiosk System Test Program        *");
	printf("\n*      Provided by Infocrypt            *");
	printf("\n*      Type H(or h) for help            *");
	printf("\n*****************************************\n");
}

void put_prompt(void)
{
	printf("\nKiosk>");
}

void help_mode(void)
{
	printf("\nH:shows you this Help screen\n");
//	printf("A:Execute App_main()\n");
	printf("P:프린터 테스트\n");
	printf("T:랜통신 테스트\n");
	printf("M:모뎀 테스트\n");
	printf("B:지폐인식기 테스트\n");
	printf("U:USB 테스트\n");
	printf("S:사운드 테스트\n");
	printf("R:시간 설정\n");
	printf("Q:Quit Test\n");
	on_line();
}

void bill_test1(void)
{
	byte chs[20];
	
	if(bill_connect())
		aDebugMsg("지폐인식기 인식 성공!");
	else
		aDebugMsg("지폐인식기 인식 실패!");
	if(bill_sendrcv("GV", '?', chs))	// 버전 확인
		aDebugMsg("Version:%d.%d", chs[2], chs[3]);
	if(bill_sendrcv("SC", 0x07, chs))
	{
		aDebugMsg("CONFIG 쓰기 성공!");
	}
	if(bill_sendrcv("GC", '?', chs)) // CONFIG 읽기
	{
		aDebugMsg("Current Config:0x%02X", chs[3]);
		if(chs[3] & 0x01) aDebugMsg("1,000원 인식여부 : ON"); else aDebugMsg("1,000원 인식여부 : OFF");
		if(chs[3] & 0x02) aDebugMsg("5,000원 인식여부 : ON"); else aDebugMsg("5,000원 인식여부 : OFF");
		if(chs[3] & 0x04) aDebugMsg("10,000원 인식여부 : ON"); else aDebugMsg("10,000원 인식여부 : OFF");
		if(chs[3] & 0x08) aDebugMsg("50,000원 인식여부 : ON"); else aDebugMsg("50,000원 인식여부 : OFF");
		if(chs[3] & 0x10) aDebugMsg("지폐인식후 자동으로 STACK"); else aDebugMsg("지폐인식후 입수명령 대기");
		if(chs[3] & 0x20) aDebugMsg("동작상태 변경시 내용 자동송출"); else aDebugMsg("요청시에만 Data 송출");
	}
	billcoin_accept();
}

int kmodem_test(void)
{
	byte ret;
//	byte chs[20];
//	int in;
//#define ICC_PORT			UART0_PORT		// IC 모듈
//#define RF_PORT			UART1_PORT		// RF_MODULE
//#define BARCODE_PORT		UART2_PORT		// 바코드
//#define PSTN_PORT			UART3_PORT		// 모뎀
//#define ETHERNET_PORT   	LAN_PORT
#if 0
	printf("\n1:1200BPS Test");
	printf("\n2:2400BPS Test");
	printf("\n3:4800BPS Test");
	printf("\n4:9600BPS Test");
	printf("\n5:19200BPS Test");
	printf("\n6:38400BPS Test");
	printf("\n7:57600BPS Test");
	printf("\n8:115200BPS Test\n");
	while(1) 
	{
		scanf("%s", chs);
		switch(chs[0]) 
		{
			case '1': printf("\n1200BPS"); sio_set(PSTN_PORT, B1200);	break;
			case '2': printf("\n2400BPS"); sio_set(PSTN_PORT, B2400);	break;
			case '3': printf("\n4800BPS"); sio_set(PSTN_PORT, B4800);	break;
			case '4': printf("\n9600BPS"); sio_set(PSTN_PORT, B9600);	break;
			case '5': printf("\n19200BPS"); sio_set(PSTN_PORT, B19200);	break;
			case '6': printf("\n38400BPS"); sio_set(PSTN_PORT, B38400);	break;
			case '7': printf("\n57600BPS"); sio_set(PSTN_PORT, B57600);	break;
			case '8': printf("\n115200BPS"); sio_set(PSTN_PORT, B115200);	break;
			case 'q': case 'Q': return;
		}
		Delay(20);
	    ret = api_send_wait((byte *)"ATZ\r", (byte *)"OK", (byte *)"\0", 2);
		if(ret==MD_OK)
			printf("\n모뎀 OK...\n");
		else
			printf("\n모뎀 Fail...\n");
	}
#endif
	sio_set(PSTN_PORT, B9600);
	Delay(20);
    ret = api_send_wait((byte *)"ATZ\r", (byte *)"OK", (byte *)"\0", 2);
	if(ret==MD_OK)
	{
		printf("\n모뎀 OK...\n");
		return 0;
	}
	else
	{
		printf("\n모뎀 Fail...\n");
		return 1;
	}
}

int kiosk_usb_test(void)
{
	FILE *fp;
	byte tt_buf[10];
	byte usb_step = 0;
	DIR *dir;
	int ret[10];

	aDebugMsg("   USB 테스트   ");
	aDebugMsg("  USB 메모리를  ");
	aDebugMsg(" 삽입하여주세요 ");

    api_set_apptimer(6, 3*100);
	while(1)
	{
		switch(usb_step)
		{
			case 0:
				ret[0]=access("/sys/block/sda", F_OK);
				ret[1]=access("/sys/block/sdb", F_OK);
				ret[2]=access("/sys/block/sdc", F_OK);
				ret[3]=access("/sys/block/sdd", F_OK);
				if(ret[0]==0 || ret[1]==0 || ret[2]==0 || ret[3]==0)
				{
					aDebugMsg("access=[%d]", ret);
					if((fp = fopen("/media/sda1/infocrypt_usb_test.dat", "w+")) == 0 )
					{
						//printf("usb not mounted...\n");
					}
					else
					{
						aDebugMsg(">>> USB writing...\n");
						fwrite("1234", 1, 4, fp);
						fclose(fp);
						usb_step++;
					    api_set_apptimer(6, 10*1000);
						break;
					}
				}
				if(api_chk_apptimer(6)==FALSE)
				{
					printf("\nusb mount timeout..\n");
					aDebugMsg("   USB 테스트   ");
					aDebugMsg("  테스트 실패!  ");
					aDebugMsg("종료키를누르세요");
					return 1;
				}
				break;
			case 1:
				if((fp = fopen("/media/sda1/infocrypt_usb_test.dat", "r+")) == 0 )
				{
					//printf("usb file read failed...\n");
				}
				else
				{
					aDebugMsg(">>> USB reading...\n");
					memset(tt_buf, 0x00, sizeof(tt_buf));
					fread(tt_buf, 1, 4, fp);
					fclose(fp);
					if(memcmp(tt_buf, "1234", 4)==0) // success
					{
						aDebugMsg("   USB 테스트   ");
						aDebugMsg("  테스트 성공!  ");
						aDebugMsg("종료키를누르세요");
						system("rm /mnt/usb/infocrypt_usb_test.dat");
						return 0;
					}
					else
					{
						aDebugMsg("   USB 테스트   ");
						aDebugMsg("  테스트 실패!  ");
						aDebugMsg("종료키를누르세요");
						system("rm /media/sda1/infocrypt_usb_test.dat");
						return 2;
					}
				}
				if(api_chk_apptimer(6)==FALSE)
				{
					printf("usb file read timeout..\n");
					aDebugMsg("   USB 테스트   ");
					aDebugMsg("  테스트 실패!  ");
					aDebugMsg("종료키를누르세요");
					return 3;
				}
				break;
		}
	}
}

void kiosk_MainLp(void);
void wait_command(void)
{
	byte chs[20];
	byte ret;

	help_mode();
	put_prompt();
	
	while(1) 
	{
		scanf("%s", chs);
		switch(chs[0]) 
		{
			case 'h' 	: 	case 'H' 	: 	help_mode();	break;
			case '\r' 	: 	case '\n' 	: 	put_prompt();  	break;
//			case 'a' 	: 	case 'A' 	: 	kiosk_MainLp();  	break;
			case 'p' 	: 	case 'P' 	: 	prt_test1(1);  	break;
			case 't' 	: 	case 'T' 	: 	kiosk_comm_test(); 	break;
			case 'm' 	: 	case 'M' 	: 	kmodem_test(); 	break;
			case 'b' 	: 	case 'B' 	: 	bill_test1(); break;
			case 'u' 	: 	case 'U' 	: 	kiosk_usb_test(); break;
			case 'a' 	: 	case 'A' 	: 	barcode_read(); break;
			case 's' 	: 	case 'S' 	: 	
				//sound_init(100);
				//sound_play("/app/girls_hoot.wav"); break;
				play_sound("/app/kiosksound/TC_UI_Button.wav"); break;
			case 'r' 	: 	case 'R' 	:
				get_dtm(TimeStamp);
				printf("\nCurrent ==> 20%2.2s년 %2.2s월 %2.2s일 %2.2s시 %2.2s분 %2.2s초", TimeStamp, &TimeStamp[2], &TimeStamp[4], &TimeStamp[6], &TimeStamp[8], &TimeStamp[10]);
				printf("\nEnter YYMMDDHHMMSS ==> ");
				scanf("%s", chs);
				set_dtm(chs); 
				break;
			case 'q' 	: 	case 'Q' 	: 	exit(0);
			default  : printf("\nInvalid command, try again.\n"); break;
		}
		put_prompt();
  	}
}
//-----------------------------------------------------------------------------------------------------
extern pthread_cond_t  qmsgq_ok;
extern pthread_cond_t  qmsgq_act_on;
extern pthread_mutex_t qmsgq_mutex;
void kiosk_MainLp(void)
{
    byte ret=0, cover_changed=0;
	pthread_t ptest, ptest2, ptest4;
	int thread_status=0, thread_status2=0, thread_status4=0;
	int aa;
	byte boot_time[16+1]={0};

#if 0
	char strSrc[20];
	char strEnc[500];
	char strDec[500];
	int  result;

	strcpy(strSrc, "1234567890");
	printf("Src = [%s]\n", strSrc);
	
	result = INP_EncData(strSrc, strlen(strSrc), strEnc);
	
	printf("return=%d, Enc = [%s]\n", result, strEnc);
	result = INP_DecData(strEnc, strlen(strEnc), strDec);
	printf("return=%d, Dec = [%s]\n", result, strDec);

	while(1);
#endif	

    KIOSK_STEP = 0;
		
	msg_thread_state=0;
	msg_thread_exit=0;
	msg_thread_stop=0;
	first_modem_test_flag=0;

	Region01.log_depth = 0;
	log_printf(0, "<<<키오스크 로직 프로그램 실행됨>>>");
	while(1)
    {
        switch(KIOSK_STEP)
        {
            case 0:                  // start step
            	if(msg_cancel)
					send_msg_cancel();
            	msg_cancel = 0;
				now_idle = 1;
				network_cancel_flag = 0;
            case 1:
                release_alltimer();
                disconnect();
                init_val();             
//                mcr2_init();

				memset(&msgq_buf, 0, sizeof(t_qdata));
				pthread_cond_signal(&qmsgq_act_on);

				if(msg_thread_state==0) // 메시지 수신 스레드가 없으면 생성
				{
					memset(&msgq_buf, 0, sizeof(t_qdata));
					msg_thread_exit = 0;
					if(pthread_create(&ptest, NULL, MsgTread, (void*)&aa)<0)
					{
						log_printf(0, "MsgThread 쓰레드 생성 실패");
						aDebugMsg("MsgThread 쓰레드 생성 실패 ");
					}
					else
					{
						aDebugMsg("MsgThread 쓰레드 생성 성공");
					}
				}
				if(msg_thread_state2==0) // 메시지 수신 스레드가 없으면 생성
				{
					msg_thread_exit2 = 0;
					if(pthread_create(&ptest2, NULL, MsgTread2, (void*)&aa)<0)
					{
						log_printf(0, "MsgThread 쓰레드 생성 실패 ");
						aDebugMsg("MsgThread 쓰레드 생성 실패 ");
					}
					else
					{
						aDebugMsg("MsgThread 쓰레드 생성 성공");
					}
				}
				if(msg_thread_state4==0) // 메시지 수신 스레드가 없으면 생성
				{
					msg_thread_exit4 = 0;
					if(pthread_create(&ptest4, NULL, MsgTread4, (void*)&aa)<0)
					{
						log_printf(0, "TMS 쓰레드 생성 실패 ");
						aDebugMsg("TMS 쓰레드 생성 실패 ");
					}
					else
					{
						aDebugMsg("TMS 쓰레드 생성 성공");
					}
				}
//----------------------------------------------------------------------------------------------
// 이더넷 DHCP 동작과 프린터 인쇄 충돌 문제를 해결하기 위한 추가. by khd, 2012.11.13
// 20121218_NTI 랜선 빠져 있을경우 지연발생 증상 수정
// 단말기 통신설정이 랜통신일 경우 dhcp 할당 받도록
//----------------------------------------------------------------------------------------------
				if (Region01.Comm.comm_line & ETHERNET_LINE)
				{
					if(ethernet_link_chk() == 0)
					{
						Eth_info.step = 2;
						Eth_info.IP_state = 0;
					}

					if(Region01.Comm.Ethernet.ipmethod == '1')	// 자동IP
					{
						if(system("pidof udhcpc") != 0)
						{
							system("udhcpc -b --retries 10");
							aDebugMsg(">>>>>>>>>>>>>>>>> udhcpc_debug reloaded!!!");
						}
						else
						{
							if(system("kill -CONT `pidof udhcpc                                                 `") == 0)
							{
								aDebugMsg(">>>>>>>>>>>>>>>>> udhcpc_debug continued!!!");
							}
							else
							{
								aDebugMsg(">>>>>>>>>>>>>>>>> udhcpc_debug aleady continued!!!");
							}
						}
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
					}
				}
//----------------------------------------------------------------------------------------------
                if(memcmp(Region01.Merchant.registration, "[Register OK :", 14)!=0)
                {
                    aDebugMsg(" 가맹점 다운로드요망 ");
                    KIOSK_STEP = 2;
                    break;
                }
				aDebugMsg("TMS update check time => %s", Region04.check_ymdsms1);
				aDebugMsg("TMS update time => %s", Region04.ymdsms);
                KIOSK_STEP = 2;
                break;
            case 2: // event check
#if 1
                if ((ret=print_cover_status()) == 56)
            	{
                    aDebugMsg("*프린터커버열림1*");
					api_set_apptimer(dtimer4, 100);
					cover_changed = 0;
                    KIOSK_STEP = 5;
                    break;
            	}
                if ((ret=print_status()) == 50)
                {
                    aDebugMsg("*프린터용지확인1*");
					api_set_apptimer(dtimer4, 100);
                    KIOSK_STEP = 4;
                    break;
                }
				//aDebugMsg("print_status() : %d", ret);
#endif
                linesta_chk_func(); 

				if(msgq_buf.command_type != 0) // 메시지큐로부터 메시지 수신됨
				{
					msgq_process();
                    KIOSK_STEP = 0;
				}
#if 0
                ret=mcr2_chk(kField.track2_data);
                if(ret)
                {
					aDebugMsg("MSR2 읽음=%s", kField.track2_data);
	                mcr2_init();
                }
#endif				
				usleep(1);
                break;
            case 4: // 프린터용지가 없는 경우
//				ret=cover_switch_check();
//				aDebugMsg("cover=%d", ret);
                if ((ret=print_cover_status()) == 56)
            	{
                    aDebugMsg("*프린터커버열림1*");
					api_set_apptimer(dtimer4, 100);
					cover_changed = 0;
                    KIOSK_STEP = 5;
                    break;
            	}
				
            	if(api_chk_apptimer(dtimer4)==FALSE)
            	{
	                if(print_status() == 50)
	                {
	                    aDebugMsg("*프린터용지확인2*");
						api_set_apptimer(dtimer4, 100);
	                    break;
	                }
					else
						KIOSK_STEP = 2;
            	}
				
                linesta_chk_func(); 
#if 1
				if(msgq_buf.command_type != 0) // 메시지큐로부터 메시지 수신됨
				{
					msgq_process();
                    KIOSK_STEP = 0;
				}
#endif
				usleep(1);
                break;
            case 5: // 프린터 커버가 열린 경우
            	if(api_chk_apptimer(dtimer4)==FALSE)
            	{
	                if(print_cover_status() == 56)
	                {
	                    aDebugMsg("*프린터커버열림2*");
						cover_changed = 0;
						api_set_apptimer(dtimer4, 50);
	                    break;
	                }
					else // 커버가 닫힌 경우
					{
						if(cover_changed == 0)
						{
							cover_changed = 1;
		                    aDebugMsg("*프린터커버닫힘2*");
							api_set_apptimer(dtimer4, 50);
						}
						else if(cover_changed == 1)
						{
							cover_changed = 0;

			                if ((ret=print_status()) == 50)
			                {
			                    aDebugMsg("*프린터용지확인1*");
								api_set_apptimer(dtimer4, 50);
			                    KIOSK_STEP = 4;
			                }
							else
							{
			                    aDebugMsg("*프린터커버닫힘3*");
								prt_test1(0);
								prt_test1(1);
								prt_test1(2);
								KIOSK_STEP = 2;
							}
						}
	                    break;
					}
            	}
				
                linesta_chk_func(); 
#if 1
				if(msgq_buf.command_type != 0) // 메시지큐로부터 메시지 수신됨
				{
					msgq_process();
                    KIOSK_STEP = 0;
				}
#endif
				usleep(1);
                break;
        }
    }
    pthread_join(ptest, (void*) &thread_status);
    pthread_join(ptest2, (void*) &thread_status2);
    pthread_join(ptest4, (void*) &thread_status4);
}

