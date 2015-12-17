#include "include.h"

const byte TERM_MASTER_KEY_pdevice[16]= {'3','D','E','2','0','1','A','3','A','0','5','2','A','1','3','0'};
const byte TERM_MASTER_KEY[16]= {'3','D','E','2','0','1','A','3','A','0','5','2','A','1','3','0'};
extern  unsigned char set_sound_table[];
void capture_delete(int type)
{
    int cnt;
    FILE *fp;
    
    capture = c_cap1;        // 집계영역시작번지
    for(cnt=0; cnt<(MAXCAP*CAP_RECORD_SIZE)/ONE_ERASE_BLOCK; cnt++)
    {        
        memset(capture+cnt*ONE_ERASE_BLOCK, 0x00, ONE_ERASE_BLOCK);
    }
    
    if ((fp = fopen(FL_CDATACAP_AREA, "w+")) == NULL)
    {
        printf("region read(%s) open error :", FL_CDATACAP_AREA);
    }
    else fclose(fp);

    if (!type)
    {
        memset(&Region02, 0, sizeof(FL_REGION_02));
        Region02.Seqno.sequence_num = 0;
    }
    else
    {
        Region02.c_qrear = 0;
        Region02.c_qfront = 0;
    }
    memset(&Region02.TempClose, 0, sizeof(Region02.TempClose));
    fi_region_4K_write((byte *)&Region02, FL_REGION02_AREA, sizeof(FL_REGION_02));
    if (type!=2)
    {
        // 직전취소 및 재인쇄영역
        memset(&kBfrField, 0, sizeof(KIOSK_ID_FIELD));
        fi_area_write((byte *)&kBfrField, BFR_CAPTURE_AREA, sizeof(KIOSK_ID_FIELD));
    }

}

void dns_server_init(void)
{
    FILE *fp;
    
    if ((fp = fopen("/app/resolv.conf", "w+")) == NULL)
    {
        printf("/app/resolv.conf open error :");
        return;
    }
    
    fprintf(fp, "%s\n", DNS_Server_01);
    fprintf(fp, "%s\n", DNS_Server_02);
    fclose(fp);
}

//-----------------------------------------------------------------------------------------------------------------------------
// type 1: force memory init, 0:don't care
//-----------------------------------------------------------------------------------------------------------------------------
void app_init(byte type)
{
    u32 cnt;

    FILE *fp;
    struct stat statbuf;
    size_t w_size;
#if 1
	fi_region_4K_reload((byte *)&RegionMA, FL_REGIONMA_AREA, sizeof(FL_REGION_MA), 0x00);
//    if(alpha_numeric_check(RegionMA.machine_no, 10)==FALSE)
    {
    	memset(tempbuf, 0, sizeof(tempbuf));
    	file2mem(tempbuf, "/media/mmcblk0p1/product-info", sizeof(tempbuf)-1);
	    memset(RegionMA.machine_no, 0x00, sizeof(RegionMA.machine_no));
	    memcpy(RegionMA.machine_no, tempbuf, 10);
        fi_region_4K_write((byte *)&RegionMA, FL_REGIONMA_AREA, sizeof(RegionMA));
    }
#endif
	fi_region_4K_reload((byte *)&Region01, FL_REGION01_AREA, sizeof(FL_REGION_01), 0x00);
	sprintf(Region01.Merchant.cat_id, "%-10.10s", RegionMA.machine_no);
    aDebugMsg("단말기번호:%s", Region01.Merchant.cat_id);
	
	if(type)
		memcpy(Region01.mem_cnf, "               ", 14);
	
	if(Memcmp(Region01.mem_cnf, "MEMORY INITED!", 14) || memcmp(Region01.Merchant.registration, "[Register OK :", 14) != 0)
    {
    	aDebugMsg("app init... ");
        aDebugMsg("데이타초기화중! ");
        aDebugMsg("잠시만기다리세요");

        dns_server_init();

		memcpy((byte *)&Region01_backup, (byte *)&Region01, sizeof(FL_REGION_01));
        // REGION 01 AREA
        Memset(&Region01, 0x00, sizeof(FL_REGION_01));  // 초기화

		memcpy(Region01.Merchant.cat_id, RegionMA.machine_no, 10);
        Region01.Comm.tm01 = 30;					// 접속대기시간(30초)
        Region01.Comm.tm02 = 20;                    // 국선접속시간(20초)
        Region01.Comm.enq_tm = 10;                  // 연결대기시간(10초)
        Region01.Comm.ack_tm = 15;                  // ACK 대기시간(15초)
        Region01.Comm.eot_tm = 5;                   // 종료대기시간(05초)
        Region01.Comm.resp_tm = 45;                 // 통신대기시간(45초)
        Region01.Comm.redial_tm = 3;                // 재다이얼준비시간(3초)
        
        // 접속선로(PSTN_LINE, C2I_LINE, CDMA_LINE, CHECK_LINE, WIBRO_LINE, ETHERNET_LINE, ETHMODEM_LINE, WCDMA_LINE)        
        Region01.Comm.comm_line = ETHERNET_LINE;
		Region01.Comm.private_bps = B9600;
		Region01.Comm.use_comm_type = '2';

		// PSTN
        sprintf(Region01.Comm.Pstn.downtel, "16883340"); 
        Region01.Comm.Pstn.dial_spd = '2';          // 다이얼속도(느리게=1, 보통=2, 빠르게=3, 2)
        Region01.Comm.Pstn.dialtone_on = '1';       // 다이얼톤 확인(예=1, 아니오=2, 1)
        Region01.Comm.Pstn.pabx_on  = '2';          // 전화내선사용여부(예=1, 아니오=2)
        Region01.Comm.Pstn.pstn_spd = B1200;        // 전화선속도(1200=0, 2400=1, 1)
        Region01.Comm.Pstn.predial_on = '1';        // PREDIAL(예=1, 아니오=2, 1)

		// Debug & Log
		Region01.log_depth = 0;
		Region01.debug_depth = 0;

		// ETHERNET
		if(type==0 && Memcmp(Region01_backup.mem_cnf, "MEMORY INITED!", 14))
		{
	    	aDebugMsg("app IP init... ");
	        Region01.Comm.Ethernet.ipmethod = '2';          // (유동 IP = 1, 고정 IP = 2, 1)
	        Region01.Comm.Ethernet.ipkind = '1';            // 서버IP설정(1.도메인명사용, 2.IP설정사용)
#ifdef REAL_LINE
	        sprintf(Region01.Comm.Ethernet.address,     "%s", "112.175.103.105"); // 테스트 서버 IP : 112.175.103.106, 73, 실서버IP : 112.175.103.105
#else
	        sprintf(Region01.Comm.Ethernet.address,     "%s", "112.175.103.73"); // 테스트 서버 IP : 112.175.103.106, 73, 실서버IP : 112.175.103.105
#endif
	        sprintf(Region01.Comm.Ethernet.portno,      "%s", "11901"); // 테스트 Port : 11901
	        sprintf(Region01.Comm.Ethernet.tmsportno,   "%s", "29998");
	        sprintf(Region01.Comm.Ethernet.domain_name, "%s", "www.jtnet.co.kr");
	        sprintf(Region01.Comm.Ethernet.domain_ip,   "%s", "210.48.96.28");

#ifdef LAN_SET_93
	        sprintf(Region01.Comm.Ethernet.staticIP,    "%s", "112.217.109.93");
#else
	        sprintf(Region01.Comm.Ethernet.staticIP,    "%s", "112.217.109.94");
#endif
	        sprintf(Region01.Comm.Ethernet.staticSM,    "%s", "255.255.255.248");
	        sprintf(Region01.Comm.Ethernet.staticGW,    "%s", "112.217.109.89");
	        sprintf(Region01.Comm.Ethernet.dns_server01,"%s", "168.126.63.1");
	        sprintf(Region01.Comm.Ethernet.dns_server02,"%s", "168.126.63.2");
		}
		else
		{
	    	aDebugMsg("app IP No init... ");
	        Region01.Comm.Ethernet.ipmethod = Region01_backup.Comm.Ethernet.ipmethod;          // (유동 IP = 1, 고정 IP = 2, 1)
	        Region01.Comm.Ethernet.ipkind = Region01_backup.Comm.Ethernet.ipkind;            // 서버IP설정(1.도메인명사용, 2.IP설정사용)
#ifdef REAL_LINE
	        sprintf(Region01.Comm.Ethernet.address,     "%s", Region01_backup.Comm.Ethernet.address); // 테스트 서버 IP : 112.175.103.106, 73, 실서버IP : 112.175.103.105
#else
	        sprintf(Region01.Comm.Ethernet.address,     "%s", Region01_backup.Comm.Ethernet.address); // 테스트 서버 IP : 112.175.103.106, 73, 실서버IP : 112.175.103.105
#endif
	        sprintf(Region01.Comm.Ethernet.portno,      "%s", Region01_backup.Comm.Ethernet.portno); // 테스트 Port : 11901
	        sprintf(Region01.Comm.Ethernet.tmsportno,   "%s", Region01_backup.Comm.Ethernet.tmsportno);
	        sprintf(Region01.Comm.Ethernet.domain_name, "%s", Region01_backup.Comm.Ethernet.domain_name);
	        sprintf(Region01.Comm.Ethernet.domain_ip,   "%s", Region01_backup.Comm.Ethernet.domain_ip);

#ifdef LAN_SET_93
	        sprintf(Region01.Comm.Ethernet.staticIP,    "%s", Region01_backup.Comm.Ethernet.staticIP);
#else
	        sprintf(Region01.Comm.Ethernet.staticIP,    "%s", Region01_backup.Comm.Ethernet.staticIP);
#endif
	        sprintf(Region01.Comm.Ethernet.staticSM,    "%s", Region01_backup.Comm.Ethernet.staticSM);
	        sprintf(Region01.Comm.Ethernet.staticGW,    "%s", Region01_backup.Comm.Ethernet.staticGW);
	        sprintf(Region01.Comm.Ethernet.dns_server01,"%s", Region01_backup.Comm.Ethernet.dns_server01);
	        sprintf(Region01.Comm.Ethernet.dns_server02,"%s", Region01_backup.Comm.Ethernet.dns_server02);
		}

		Region01.Merchant.vaccount_enable = 0; // 가상계좌 사용여부 (1:사용, 0:비사용)
		Region01.Terminfo.SoundInfo.background_sound = 0; // 배경음 사용여부 (1:사용, 0:비사용)
		Region01.Terminfo.SoundInfo.touch_sound = 1; // 터치음 사용여부 (1:사용, 0:비사용)
		Region01.Terminfo.SoundInfo.volume = 2; // 볼륨 인덱스 (70~127), 70(0), 90(1), 100(2), 110(3), 120(4), 122(5), 124(6), 127(7)
		
        Region01.Comm.Ethernet.phone_change = '2';     // 전화선접속여부(1:예, 2:아니오)
        Region01.Terminfo.PrtInfo.cutting_on = '2'; // 용지절단(1:절단, 2:부분절단, 2)

        Memcpy(Region01.mem_cnf, "MEMORY INITED!", 14);
		fi_region_4K_write((byte *)&Region01, FL_REGION01_AREA, sizeof(FL_REGION_01));

	    // REGION 04 TMS AREA
	    memset(&Region04, 0, sizeof(FL_REGION_04));
		sprintf(Region04.check_ymdsms1, "%s", "141026040000");
		sprintf(Region04.check_ymdsms2, "%s", "141026050000");
	    fi_region_4K_write((byte *)&Region04, FL_REGION04_AREA, sizeof(FL_REGION_04));
    }

	if(Region01.log_depth > 1) Region01.log_depth = 0;
	//if(Region01.debug_depth > 2) Region01.debug_depth = 0;

	if( !(Region01.Terminfo.SoundInfo.volume >= 0 && Region01.Terminfo.SoundInfo.volume <= 7) )
	{
		Region01.Terminfo.SoundInfo.volume = 2;
	}
	sprintf(tempbuf, "amixer set Speaker %d", set_sound_table[Region01.Terminfo.SoundInfo.volume]);
	system(tempbuf);

	if(!(Region01.Terminfo.PrtInfo.print_speed >= 0 && Region01.Terminfo.PrtInfo.print_speed <= 60))
		Region01.Terminfo.PrtInfo.print_speed = 0;

#ifndef PRT_SPEED_TEST
	set_bright(Region01.Terminfo.PrtInfo.print_speed);
#endif
	
	fi_region_4K_reload((byte *)&Region02, FL_REGION02_AREA, sizeof(FL_REGION_02), 0x00);
	fi_region_4K_reload((byte *)&Region04, FL_REGION04_AREA, sizeof(FL_REGION_04), 0x00);

    capture = c_cap1;        // 신용/직불 집계영역시작번지
    for(cnt=0; cnt<((MAXCAP*CAP_RECORD_SIZE)/ONE_ERASE_BLOCK); cnt++)
    {        
        memset(capture+cnt*ONE_ERASE_BLOCK, 0x00, ONE_ERASE_BLOCK);
    }

    if ((fp = fopen(FL_CDATACAP_AREA, "r+")) == NULL)
    {
        if ((fp = fopen(FL_CDATACAP_AREA, "w+")) == NULL)
        {
            printf("region read(%s) open error :", FL_CDATACAP_AREA);
        }
        else fclose(fp);
    }
    else
    {
        fstat(fileno(fp), &statbuf);
        w_size = statbuf.st_size;
        if (w_size) fread(capture, 1, w_size, fp);
        fclose(fp);
    }
}

void AllRegion_Update(void)
{
	fi_region_4K_reload((byte *)&Region01_backup, FL_REGION01_AREA, sizeof(FL_REGION_01), 0x00);
	fi_region_4K_reload((byte *)&Region02_backup, FL_REGION02_AREA, sizeof(FL_REGION_02), 0x00);
	fi_region_4K_reload((byte *)&Region04_backup, FL_REGION04_AREA, sizeof(FL_REGION_04), 0x00);
	
	if(memcmp((byte *)&Region01, (byte *)&Region01_backup, sizeof(FL_REGION_01)))
	{
	    fi_region_4K_write((byte *)&Region01, FL_REGION01_AREA, sizeof(FL_REGION_01));
	}
	if(memcmp((byte *)&Region02, (byte *)&Region02_backup, sizeof(FL_REGION_02)))
	{
	    fi_region_4K_write((byte *)&Region02, FL_REGION02_AREA, sizeof(FL_REGION_02));
	}
	if(memcmp((byte *)&Region04, (byte *)&Region04_backup, sizeof(FL_REGION_04)))
	{
	    fi_region_4K_write((byte *)&Region04, FL_REGION04_AREA, sizeof(FL_REGION_04));
	}
}

void init_val(void)
{
    AllRegion_Update();
    SERVKIND = 0;
    Predial = 0;
    linetype = 0;
    if(field_not_erase==0)
	    memset(&kField, 0, sizeof(KIOSK_ID_FIELD));
}

