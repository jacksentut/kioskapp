#include "include.h"
#define COMMAND_SIZE    128

//#define DEBUG_MSG

extern const CP_CODE_LIST cp_code_list[];
extern const TRANS_CODE_LIST trans_code_list[];
int make_header(int ServKind, byte *Tbuf)
{
    int len = 0;
	byte tmp_buf[50];
    
    Tbuf[len++] = 'P';
    Tbuf[len++] = '#';
    Tbuf[len++] = 'Y'; // 전문형식, 'N':평문, 'Y':암호화
    memcpy(&Tbuf[len], "0000", 4); // 전문길이, 거래구분코드부터의 전체길이
    len += 4;

	// 거래구분코드
    switch(ServKind)
    {
        case REG_DOWNLOAD: // 설치인증 (S900 <-> R900)
            memcpy(&Tbuf[len], "S900", 4); 
            len += 4;
            break;
    	case SALE_ENABLE_CHECK: // 상품별 판매 가능여부 체크 (S901 <-> R901)
            memcpy(&Tbuf[len], "S901", 4);
            len += 4;
            break;
        case STOCK_CHECK: // 상품 재고 체크 (S902 <-> R902)
            memcpy(&Tbuf[len], "S902", 4);
            len += 4;
            break;
    	case ISSUE_APP: // 발권 요청 (S903 <-> R903)
            memcpy(&Tbuf[len], "S903", 4);
            len += 4;
            break;
        case ISSUE_CAN: // 발권/단건 취소 (S905 <-> R905)
            memcpy(&Tbuf[len], "S905", 4);
            len += 4;
            break;
        case OPERATION_CHECK: // 운영오류 상황기록 (S906 <-> R906)
            memcpy(&Tbuf[len], "S906", 4);
            len += 4;
            break;
        case ADJUST_CHECK: // 정산마감 금액 확인 (S907 <-> R907)
            memcpy(&Tbuf[len], "S907", 4);
            len += 4;
            break;
        case ADJUST_APP: // 정산마감 처리 (S908 <-> R908)
            memcpy(&Tbuf[len], "S908", 4);
            len += 4;
            break;
        case ADMIN_LOGIN_AUTH: // 관리자 로그인 인증 (S909 <-> R909)
            memcpy(&Tbuf[len], "S909", 4);
            len += 4;
            break;
        case TEMP_CANCEL: // 부분납 후, 강제종료 (임시결재번호 취소) (S910 <-> R910)
            memcpy(&Tbuf[len], "S910", 4);
            len += 4;
            break;
        case KIOSK_ONOFF: // 키오스크 온/오프 (S911 <-> R911)
            memcpy(&Tbuf[len], "S911", 4);
            len += 4;
            break;
        case HW_ERROR_CHECK: // H/W 오류상황 기록 (S912 <-> R912)
            memcpy(&Tbuf[len], "S912", 4);
            len += 4;
            break;
        case RESPONSE_RESEND: // 서버응답전문 재전송 (S913 <-> R913)
            memcpy(&Tbuf[len], "S913", 4);
            len += 4;
            break;
        case VACCOUNT_BALANCE: // 가맹점 가상계좌 잔액조회 (S914 <-> R914)
            memcpy(&Tbuf[len], "S914", 4);
            len += 4;
            break;
        case REMOTE_REBOOT: // 원격리부팅 (S915 <-> R915)
            memcpy(&Tbuf[len], "S915", 4);
            len += 4;
            break;
        case PATCH_CHECK: // 패치확인 (S921 <-> R921)
            memcpy(&Tbuf[len], "S921", 4);
            len += 4;
            break;
        case PATCH_REQUEST: // 패치요청 (S922 <-> R922)
            memcpy(&Tbuf[len], "S922", 4);
            len += 4;
            break;
        case PATCH_COMPLETE: // 패치완료 (S923 <-> R923)
            memcpy(&Tbuf[len], "S923", 4);
            len += 4;
            break;
        case CHANGE_CHECK: // 상품권 교환검사 (S941 <-> R941)
            memcpy(&Tbuf[len], "S941", 4);
            len += 4;
            break;
        case CHANGE_APP: // 상품권 교환요청 (S942 <-> R942)
            memcpy(&Tbuf[len], "S942", 4);
            len += 4;
            break;
        default:
            aDebugMsg("service kind error [%d]\n", ServKind);
            break;
    }

    memcpy(&Tbuf[len], "0000", 4); // 요청유형코드, "0000" 고정, 변동시 별도 협의
    len += 4;
	
	sprintf(tmp_buf, "20%12.12s", kField.MakeTimeStamp); // 요청일시, 전문 요청 시간 (YYYYMMDDHHMMSS)
	memset(&Tbuf[len], 0x20, 15);
	memcpy(&Tbuf[len], tmp_buf, 14);
    len += 15;

	sprintf(tmp_buf, "%-16.16s", "KIOSK");
//	memcpy(&Tbuf[len], "KIOSK", 5); // 터미널 이름
	memcpy(&Tbuf[len], tmp_buf, 16);
	len += 16;
//	sprintf(tmp_buf, "%-10.10s", msgq_buf.qs900_ask.t_field.shop_serial_num); // 가맹점 시리얼번호, I&Play에서 오프라인 발급
#ifndef SELF_DEBUG
    if(memcmp(Region01.Merchant.registration, "[Register OK :", 14)!=0)
	{
		sprintf(tmp_buf, "%-10.10s", msgq_buf.qs900_ask.t_field.shop_serial_num); // 가맹점 시리얼번호, I&Play에서 오프라인 발급
	}
	else
		sprintf(tmp_buf, "%-10.10s", Region01.Merchant.shop_serial); // 가맹점 시리얼번호, I&Play에서 오프라인 발급
#else
		sprintf(tmp_buf, "%-10.10s", Region01.Merchant.shop_serial); // 가맹점 시리얼번호, I&Play에서 오프라인 발급
#endif
	memcpy(&Tbuf[len], tmp_buf, 10);
	len += 10;

	sprintf(tmp_buf, "%-20.20s", Region01.Merchant.cat_id); // KIOSK 내부에서 관리하는 일련번호
	memcpy(&Tbuf[len], tmp_buf, 20);
	len += 20;

    return(len);
}

void make_packet(int ServKind)
{
	byte Ltemp[10];	
	byte ucCommandBuf[COMMAND_SIZE]={0};
	
    union crc_def
    {
        u16 rcrc;
        byte crccode[2];
    } crc;

	// 헤더
    slen = 0;
    memset(sendbuf, 0x00, sizeof(sendbuf));
    slen = make_header(ServKind, sendbuf);

	// Field
	switch(ServKind)
    {
    	case REG_DOWNLOAD:
			memcpy(&sendbuf[slen], kField.Send.unique_id, 20);
			slen += 20;
			memcpy(&sendbuf[slen], kField.Send.ip_address, 39);
			slen += 39;
			break;
    	case SALE_ENABLE_CHECK:
			memcpy(&sendbuf[slen], kField.Send.cp_code, 4);
			slen += 4;
			break;
    	case STOCK_CHECK:
			memcpy(&sendbuf[slen], kField.Send.cp_code, 4);
			slen += 4;
			memcpy(&sendbuf[slen], kField.Send.issue_count, 2);
			slen += 2;
			memcpy(&sendbuf[slen], kField.Send.issue_amount, 10);
			slen += 10;
			break;
    	case ISSUE_APP:
			memcpy(&sendbuf[slen], kField.Send.temp_group_pay_num, 50);
			slen += 50;
			memcpy(&sendbuf[slen], kField.Send.approval_code, 2);
			slen += 2;
			memcpy(&sendbuf[slen], kField.Send.pg_tran_num, 50);
			slen += 50;
			memcpy(&sendbuf[slen], kField.Send.pg_approval_num, 50);
			slen += 50;
			memcpy(&sendbuf[slen], kField.Send.pg_tran_date, 14);
			slen += 14;
			memcpy(&sendbuf[slen], kField.Send.payed_tamount, 10);
			slen += 10;
			memcpy(&sendbuf[slen], kField.Send.issue_type, 1);
			slen += 1;
			memcpy(&sendbuf[slen], kField.Send.client_phone, 16);
			slen += 16;
			break;
    	case ISSUE_CAN:
#if 0
			if(kField.Send.cancel_type[0] == 'T') // 발권 취소
			{
				memcpy(&sendbuf[slen], kField.Send.group_pay_num, 50);
				slen += 50;
				memcpy(&sendbuf[slen], kField.Send.cancel_type, 1);
				slen += 1;
				memcpy(&sendbuf[slen], kField.Send.can_amount, 10);
				slen += 10;
				memcpy(&sendbuf[slen], kField.Send.error_code, 2);
				slen += 2;
			}			
			else if(kField.Send.cancel_type[0] == 'P') // 관리자 단건 취소
			{
				memcpy(&sendbuf[slen], kField.Send.cancel_type, 1);
				slen += 1;
				memcpy(&sendbuf[slen], kField.Send.cp_code, 4);
				slen += 4;
				memcpy(&sendbuf[slen], kField.Send.pin_date, 8);
				slen += 8;
				memcpy(&sendbuf[slen], kField.Send.pin_num, 24);
				slen += 24;
				memcpy(&sendbuf[slen], kField.Send.pin_manage_num, 20);
				slen += 20;
				memcpy(&sendbuf[slen], kField.Send.pin_issue_amount, 10);
				slen += 10;
				memcpy(&sendbuf[slen], kField.Send.can_amount, 10);
				slen += 10;
			}			
			else if(kField.Send.cancel_type[0] == 'C') // 고객 단건 취소
			{
				memcpy(&sendbuf[slen], kField.Send.cancel_type, 1);
				slen += 1;
				memcpy(&sendbuf[slen], kField.Send.cp_code, 4);
				slen += 4;
				memcpy(&sendbuf[slen], kField.Send.pin_date, 8);
				slen += 8;
				memcpy(&sendbuf[slen], kField.Send.pin_manage_num, 20);
				slen += 20;
				memcpy(&sendbuf[slen], kField.Send.pin_issue_amount, 10);
				slen += 10;
				memcpy(&sendbuf[slen], kField.Send.can_amount, 10);
				slen += 10;
				memcpy(&sendbuf[slen], kField.Send.client_phone, 16);
				slen += 16;
				memcpy(&sendbuf[slen], kField.Send.client_bank_name, 20);
				slen += 20;
				memcpy(&sendbuf[slen], kField.Send.client_account, 20);
				slen += 20;
				memcpy(&sendbuf[slen], kField.Send.client_name, 24);
				slen += 24;
				memcpy(&sendbuf[slen], kField.Send.error_code, 2);
				slen += 2;
			}			
#else
			memcpy(&sendbuf[slen], kField.Send.group_pay_num, 50);
			slen += 50;
			memcpy(&sendbuf[slen], kField.Send.cancel_type, 1);
			slen += 1;
			memcpy(&sendbuf[slen], kField.Send.cp_code, 4);
			slen += 4;
			memcpy(&sendbuf[slen], kField.Send.pin_date, 8);
			slen += 8;
			memcpy(&sendbuf[slen], kField.Send.pin_num, 24);
			slen += 24;
			memcpy(&sendbuf[slen], kField.Send.pin_manage_num, 20);
			slen += 20;
			memcpy(&sendbuf[slen], kField.Send.pin_issue_amount, 10);
			slen += 10;
			memcpy(&sendbuf[slen], kField.Send.can_amount, 10);
			slen += 10;
			memcpy(&sendbuf[slen], kField.Send.client_phone, 16);
			slen += 16;
			memcpy(&sendbuf[slen], kField.Send.client_bank_name, 20);
			slen += 20;
			memcpy(&sendbuf[slen], kField.Send.client_account, 20);
			slen += 20;
			memcpy(&sendbuf[slen], kField.Send.client_name, 24);
			slen += 24;
			memcpy(&sendbuf[slen], kField.Send.error_code, 2);
			slen += 2;
#endif
			break;
    	case OPERATION_CHECK:
			memcpy(&sendbuf[slen], kField.Send.group_pay_num, 50);
			slen += 50;
			memcpy(&sendbuf[slen], kField.Send.cash_num, 20);
			slen += 20;
			memcpy(&sendbuf[slen], kField.Send.error_type_code, 2);
			slen += 2;
			memcpy(&sendbuf[slen], kField.Send.client_phone2, 20);
			slen += 20;
			memcpy(&sendbuf[slen], kField.Send.etc_information, 100);
			slen += 100;
			break;
    	case ADJUST_CHECK:
			memcpy(&sendbuf[slen], kField.Send.adjust_ref_time, 19);
			slen += 19;
			break;
    	case ADJUST_APP:
			memcpy(&sendbuf[slen], kField.Send.adjust_ref_time, 19);
			slen += 19;
			memcpy(&sendbuf[slen], kField.Send.issue_amount, 10);
			slen += 10;
			memcpy(&sendbuf[slen], kField.Send.can_amount, 10);
			slen += 10;
			memcpy(&sendbuf[slen], kField.Send.temp_payed_amount, 10);
			slen += 10;
			memcpy(&sendbuf[slen], kField.Send.kiosk_total_amount, 10);
			slen += 10;
			break;
    	case ADMIN_LOGIN_AUTH:
			memcpy(&sendbuf[slen], kField.Send.admin_pw, 6);
			slen += 6;
			break;
    	case TEMP_CANCEL:
			memcpy(&sendbuf[slen], kField.Send.temp_group_pay_num, 50);
			slen += 50;
			memcpy(&sendbuf[slen], kField.Send.temp_payed_amount, 10);
			slen += 10;
			memcpy(&sendbuf[slen], kField.Send.etc_information, 100);
			slen += 100;
			memcpy(&sendbuf[slen], kField.Send.error_type_code, 2);
			slen += 2;
			break;
    	case KIOSK_ONOFF:
			memcpy(&sendbuf[slen], kField.Send.kiosk_status, 1);
			slen += 1;
			break;
    	case HW_ERROR_CHECK:
			break;
    	case RESPONSE_RESEND:
			break;
    	case VACCOUNT_BALANCE:
			memcpy(&sendbuf[slen], kField.Send.account_ref_time, 19);
			slen += 19;
			break;
    	case REMOTE_REBOOT:
			break;
    	case PATCH_CHECK:
			memcpy(&sendbuf[slen], kField.Send.kiosk_version, 10);
			slen += 10;
			break;
    	case PATCH_REQUEST:
			break;
    	case PATCH_COMPLETE:
			break;
    	case CHANGE_CHECK:
			memcpy(&sendbuf[slen], kField.Send.cp_code, 4);
			slen += 4;
			memcpy(&sendbuf[slen], kField.Send.pin_date, 8);
			slen += 8;
			memcpy(&sendbuf[slen], kField.Send.pin_num, 24);
			slen += 24;
			memcpy(&sendbuf[slen], kField.Send.pin_manage_num, 20);
			slen += 20;
			memcpy(&sendbuf[slen], kField.Send.pin_issue_amount, 10);
			slen += 10;
			break;
    	case CHANGE_APP:
			memcpy(&sendbuf[slen], kField.Send.group_pay_num, 50);
			slen += 50;
			memcpy(&sendbuf[slen], kField.Send.pin_num, 24);
			slen += 24;
			memcpy(&sendbuf[slen], kField.Send.pin_manage_num, 20);
			slen += 20;
			memcpy(&sendbuf[slen], kField.Send.cp_code, 4);
			slen += 4;
			memcpy(&sendbuf[slen], kField.Send.issue_count, 2);
			slen += 2;
			memcpy(&sendbuf[slen], kField.Send.issue_amount, 10);
			slen += 10;
			memcpy(&sendbuf[slen], kField.Send.issue_type, 1);
			slen += 1;
			memcpy(&sendbuf[slen], kField.Send.client_phone, 16);
			slen += 16;
			break;
		default:
			break;
    }

	sprintf(Ltemp, "%04d", slen-7);
    memcpy(&sendbuf[3], Ltemp, 4); // 전문길이, 거래구분코드부터의 전체길이
	
	if(sendbuf[2] == 'Y') // 전문 암호화 여부
	{
		int msize=0;
		int ret=0;

		// 암호화된 바이트수=(x/16+2)*32, x:암호화할 바이트수
		msize = ((((slen-7)/16)+2)*32)+(7+1);
		enc_send_len = msize - 1;
#if 0
	    if((p_enc_buf = (unsigned char *)malloc(msize)) == NULL)
	    {
	        printf("allocation error!!\n");
	    }
	    else
	    {
			printf("[%d]Bytes allocation success!!\n", msize);
		}
		
		memset(p_enc_buf, 0, msize);
#endif
		msize = INP_EncData(&sendbuf[7], slen-7, &cyper_tmp_buf[7]);
		aDebugMsg(">>>>>>> INP_EncData():ret=%d", msize);
		memcpy(cyper_tmp_buf, "P#Y", 3);
		sprintf(Ltemp, "%04d", enc_send_len-7);
		memcpy(&cyper_tmp_buf[3], Ltemp, 4);
	}

#ifdef DEBUG_MSG
	printf("요청전문\n");
	HexDump(sendbuf,slen);
#endif

}

/* returns :                       */
/*           0 : success, 1 : fail */
byte packet_anal(int ServKind, int type)
{
    byte ret=0;
	int iLen=0;
	unsigned char WorkBuf[200+1];
	t_qdata qdata;
	
// 20120524_PSH
	if(recvbuf[2]=='Y')
	{
		printf("\n암호화된 응답전문 : \n");
		//HexDump(recvbuf,rlen);
		
		// 복호화된 바이트수=(y-64)/2, y:암호화된 바이트수
		iLen = INP_DecData(&recvbuf[7], rlen-7, cyper_tmp_buf);
		aDebugMsg(">>>>>>> INP_DecData():ret=%d", iLen);
		memset(&recvbuf[7], 0, sizeof(recvbuf)-7);
//		iLen = (rlen-7-64)/2;
		iLen = strlen(cyper_tmp_buf);
		aDebugMsg(">>>>>>> strlen(cyper_tmp_buf):ret=%d", iLen);
		memcpy(&recvbuf[7], cyper_tmp_buf, iLen);
		rlen = iLen+7;
		sprintf(WorkBuf, "%04d", iLen);
		memcpy(&recvbuf[3], WorkBuf, 4); // 복호화된 길이로 recvbuf의 전문길이를 바꾼다.
	}
	printf("\n응답전문 : \n");
	HexDump(recvbuf,rlen);

	memcpy(kField.Receive.resp_transcode, &recvbuf[7], 4);
   	memcpy(kField.respcode, &recvbuf[11], 4); // 응답결과코드
   	memcpy(kField.Receive.resp_terminal, &recvbuf[30], 16); // 응답 터미널이름
   	memcpy(kField.Receive.resp_shop_serial, &recvbuf[46], 10); // 응답 가맹점 시리얼번호
   	memcpy(kField.Receive.resp_kiosk_serial, &recvbuf[56], 20); // 응답 키오스크 내부관리일련번호

	if( ((memcmp(kField.Receive.resp_terminal, "I&Play", 6) != 0) && (memcmp(kField.Receive.resp_terminal, "I&PLAY", 6) != 0)) || (memcmp(kField.Receive.resp_kiosk_serial, Region01.Merchant.cat_id, 10) != 0))
	{
        aDebugMsg("  수신전문오류1  ");
		if(type)
		{
			qdata.command_type = qETHERNET;
			qdata.command_code = qETHERNET_CONNECT;
			qdata.qlan_resp.response_code = 4; // 수신전문오류
			qdata.qlan_resp.t_field.progress = 0;
			send_msgq(&qdata, 1);
		}
		log_printf(0, "통신:수신전문 오류1!");
		return 1;
	}
	
    if(memcmp(Region01.Merchant.registration, "[Register OK :", 14)==0)
   	{
		if(memcmp(kField.Receive.resp_shop_serial, Region01.Merchant.shop_serial, 10) != 0 )
		{
	        aDebugMsg("  수신전문오류2  ");
			if(type)
			{
				qdata.command_type = qETHERNET;
				qdata.command_code = qETHERNET_CONNECT;
				qdata.qlan_resp.response_code = 4; // 수신전문오류
				qdata.qlan_resp.t_field.progress = 0;
				send_msgq(&qdata, 1);
			}
			log_printf(0, "통신:수신전문 오류2!");
			return 1;
		}
    }
	ret = 0;
	if(memcmp(kField.Receive.resp_transcode, trans_code_list[ServKind].str_trans_rcode, 4) != 0)
	{
        aDebugMsg("  수신전문오류3  ");
		ret = 1;
	}

	switch(ServKind)
	{
		case REG_DOWNLOAD:
		case SALE_ENABLE_CHECK:
		case STOCK_CHECK:
		case ISSUE_APP:
		case ISSUE_CAN:
		case ADJUST_CHECK:
		case ADJUST_APP:
		case ADMIN_LOGIN_AUTH:
		case TEMP_CANCEL:
		case KIOSK_ONOFF:
		case VACCOUNT_BALANCE:
		case OPERATION_CHECK:
		case CHANGE_CHECK:
		case CHANGE_APP:
			memcpy(kField.Receive.resp_message, &recvbuf[76], 50); // 응답코드가 오류인 경우, 오류 상세 설명
			break;
		case HW_ERROR_CHECK:
		case RESPONSE_RESEND:
		case REMOTE_REBOOT:
		case PATCH_CHECK:
		case PATCH_REQUEST:
		case PATCH_COMPLETE:
			break;
	}

	if(ret)
	{
		if(type)
		{
			qdata.command_type = qETHERNET;
			qdata.command_code = qETHERNET_CONNECT;
			qdata.qlan_resp.response_code = 4; // 수신전문오류
			qdata.qlan_resp.t_field.progress = 0;
			send_msgq(&qdata, 1);
		}
		log_printf(0, "통신:수신전문 오류3!");
		return ret;
	}
	memset(TimeStamp, 0, sizeof(TimeStamp));
	memcpy(TimeStamp, &recvbuf[17], 12);
	memcpy(kField.ymdsms, &recvbuf[17], 12);
	set_dtm(TimeStamp);
	aDebugMsg("시간설정됨!!!!!!!!! : %12.12s", TimeStamp);
    
    return ret;
}



