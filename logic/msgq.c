#include "include.h"


extern int msg_thread_state;
extern int msg_thread_exit;
extern int msg_thread_stop;

extern pthread_cond_t  qmsgq_ok;
extern pthread_cond_t  qmsgq_act_on;
extern pthread_mutex_t qmsgq_mutex;
extern const CP_CODE_LIST cp_code_list[];
extern const CP_CODE_LIST cp_code_list2[];
extern  unsigned char set_sound_table[];
//------------------------------------------------------------------------------------------------------
int reg_download(void)
{
	int servkind = KIOSK_SERVKIND;
	byte ret;
	t_qdata qdata;
	u8 ip[16+1], subnet_mask[16+1], gateway[16+1], dns1[18+1], dns2[18+1];

#ifdef SELF_DEBUG
	//memcpy(msgq_buf.qs900_ask.t_field.shop_serial_num, "9191010009", 10);
#ifdef REAL_LINE
	memcpy(msgq_buf.qs900_ask.t_field.shop_serial_num, "9191010008", 10);
#else
	memcpy(msgq_buf.qs900_ask.t_field.shop_serial_num, "9191010009", 10);
#endif
#endif
	//sprintf(Region01.Merchant.cat_id, "%s", "1406180001");
	sprintf(Region01.Merchant.shop_serial, "%s", msgq_buf.qs900_ask.t_field.shop_serial_num);
	aDebugMsg("[msg recv]--> %-10.10s", msgq_buf.qs900_ask.t_field.shop_serial_num);
	sprintf(kField.Send.unique_id, "%-20.20s", Region01.Merchant.cat_id); // Unique ID (CPU-ID 혹은 HDD-ID 혹은 기타)
	if(ifconfig_read(ip, subnet_mask, gateway, dns1, dns2)==0)
	{
		aDebugMsg("IP정보 Ok!");
	}
	else
	{
		aDebugMsg("IP정보를확인하지 못하였습니다.");
	}
	sprintf(kField.Send.ip_address, "%-39.39s", ip);
	
    get_dtm(kField.MakeTimeStamp);
	aDebugMsg("reg_download() started...");
	log_printf(1, "설치인증(S900) 요청:가맹점번호=%s, IP=%s", Region01.Merchant.shop_serial, ip);
	make_packet(servkind);
    ret = host_msganal(servkind, 1);
    disconnect();
    if(ret) return ret;
#if 1
	qdata.command_type = qETHERNET;
	qdata.command_code = qETHERNET_CONNECT;
	qdata.qlan_resp.response_code = 0; // 통신정상완료
	qdata.qlan_resp.t_field.progress = 100;
	send_msgq(&qdata, 1);
#endif

    if (memcmp(kField.respcode, "0000", 4)==0)
    {
		memcpy(kField.Receive.shop_name, &recvbuf[126], 50); // 가맹점 이름
		memcpy(kField.Receive.vaccount_use, &recvbuf[176], 1); // 가상계좌 사용여부
		if(recvbuf[176] == '1' || recvbuf[176] == '0')
			Region01.Merchant.vaccount_enable = 1;
		else
			Region01.Merchant.vaccount_enable = 0;
		sprintf(Region01.Merchant.shop_name, "%-50.50s", kField.Receive.shop_name);
		erase_space(Region01.Merchant.shop_name, 3);
        aDebugMsg("  설치인증 성공!  ");
		sprintf(Region01.Merchant.shop_serial, "%10.10s", msgq_buf.qs900_ask.t_field.shop_serial_num);
		sprintf(Region01.Merchant.registration, "%s", "[Register OK :");
        //trans_print(SERVKIND);
        disconnect();
		capture_delete(0); // 설치인증 성공시 키오스크 집계데이터를 초기화 한다.
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS900;
		qdata.qs900_resp.response_code = 0;
		del_space(qdata.qs900_resp.t_field.shop_name, kField.Receive.shop_name, sizeof(kField.Receive.shop_name));
		qdata.qs900_resp.t_field.vaccount_enable = Region01.Merchant.vaccount_enable;
		send_msgq(&qdata, 1);
		log_printf(1, "설치인증(S900) 성공");
#endif
    }
    else
    {
        disconnect();
        aDebugMsg("설치인증 실패:%s", kField.Receive.resp_message);
		log_printf(1, "설치인증(S900) 실패:%4.4s", kField.respcode);
#if 1
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS900;
		if(kField.respcode[0] == 'E' || kField.respcode[0] == 'e')
			kField.respcode[0] = '8';
		qdata.qs900_resp.response_code = (int)a2l(kField.respcode, 4);
		memset(qdata.qs900_resp.t_field.message, 0, sizeof(qdata.qs900_resp.t_field.message));
		send_msgq(&qdata, 1);
#endif
        return(2);
    }

    return(0);
}
//------------------------------------------------------------------------------------------------------
int sale_enable_check(void)
{
	int servkind = KIOSK_SERVKIND;
	byte ret;
	t_qdata qdata;
	#ifdef SELF_DEBUG
	msgq_buf.qs901_ask.t_field.cp_code = 43; // 넥슨카드
	sprintf(kField.Send.cp_code, "%04d", msgq_buf.qs901_ask.t_field.cp_code);
	#else
	sprintf(kField.Send.cp_code, "%04d", msgq_buf.qs901_ask.t_field.cp_code);
	#endif

    get_dtm(kField.MakeTimeStamp);
	aDebugMsg("sale_enable_check() started...");
	log_printf(1, "판매가능체크(S901) 요청:CP Code=%4.4s", kField.Send.cp_code);
	make_packet(servkind);
    ret = host_msganal(servkind, 1);
    disconnect();
    if(ret) return ret;
#if 1
	qdata.command_type = qETHERNET;
	qdata.command_code = qETHERNET_CONNECT;
	qdata.qlan_resp.response_code = 0; // 통신정상완료
	qdata.qlan_resp.t_field.progress = 100;
	send_msgq(&qdata, 1);
#endif

    if (memcmp(kField.respcode, "0000", 4)==0)
    {
		memset(&qdata, 0, sizeof(t_qdata));
        aDebugMsg("  상품별 판매가능 여부 체크 성공!  ");
        //trans_print(SERVKIND);
        disconnect();
#if 1
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS901;
		if(kField.respcode[0] == 'E' || kField.respcode[0] == 'e')
			kField.respcode[0] = '8';
		qdata.qs901_resp.response_code = (int)a2l(kField.respcode, 4);
		memset(qdata.qs901_resp.t_field.message, 0, sizeof(qdata.qs901_resp.t_field.message));
		send_msgq(&qdata, 1);
		log_printf(1, "판매가능체크(S901) 성공");
#endif
    }
    else
    {
        disconnect();
        aDebugMsg("상품별 판매가능 여부 체크 실패:%s", kField.Receive.resp_message);
		log_printf(1, "판매가능체크(S901) 실패:%4.4s", kField.respcode);
        return(2);
    }

    return(0);
}
//------------------------------------------------------------------------------------------------------
// type=0 정상발권, type=1 테스트발권
//------------------------------------------------------------------------------------------------------
int stock_check(byte type)
{
	int servkind = KIOSK_SERVKIND;
	byte ret;
	t_qdata qdata;

#ifdef SELF_DEBUG
	msgq_buf.qs902_ask.t_field.cp_code = 43; // 넥슨카드
	msgq_buf.qs902_ask.t_field.cp_count = 1; // 총발권 매수
	msgq_buf.qs902_ask.t_field.cp_amount = 5000; // 발권 낱장 금액
	sprintf(kField.Send.cp_code, "%04d", msgq_buf.qs902_ask.t_field.cp_code);
	sprintf(kField.Send.issue_count, "%02d", msgq_buf.qs902_ask.t_field.cp_count);
	sprintf(kField.Send.issue_amount, "%-10d", msgq_buf.qs902_ask.t_field.cp_amount);
#else
	sprintf(kField.Send.cp_code, "%04d", msgq_buf.qs902_ask.t_field.cp_code);
	sprintf(kField.Send.issue_count, "%02d", msgq_buf.qs902_ask.t_field.cp_count);
	sprintf(kField.Send.issue_amount, "%-10d", msgq_buf.qs902_ask.t_field.cp_amount);
#endif

	if(type==1)
	{
		msgq_buf.qs902_ask.t_field.cp_code = 17; // 틴캐시
		msgq_buf.qs902_ask.t_field.cp_count = 1; // 총발권 매수
		msgq_buf.qs902_ask.t_field.cp_amount = 1004; // 발권 낱장 금액
		sprintf(kField.Send.cp_code, "%04d", msgq_buf.qs902_ask.t_field.cp_code);
		sprintf(kField.Send.issue_count, "%02d", msgq_buf.qs902_ask.t_field.cp_count);
		sprintf(kField.Send.issue_amount, "%-10d", msgq_buf.qs902_ask.t_field.cp_amount);
	}

    get_dtm(kField.MakeTimeStamp);
	aDebugMsg("stock_check() started...");
	log_printf(1, "재고체크(S902) 요청:CP Code=%4.4s, 매수=%2.2s, 액면가=%10.10s", kField.Send.cp_code, kField.Send.issue_count, kField.Send.issue_amount);
	make_packet(servkind);
    ret = host_msganal(servkind, 1);
    disconnect();
    if(ret) return ret;
#if 1
	qdata.command_type = qETHERNET;
	qdata.command_code = qETHERNET_CONNECT;
	qdata.qlan_resp.response_code = 0; // 통신정상완료
	qdata.qlan_resp.t_field.progress = 100;
	send_msgq(&qdata, 1);
#endif

    if (memcmp(kField.respcode, "0000", 4)==0)
    {
		sprintf(Region01.Issue_Info.temp_group_pay_num,  "%-50.50s", kField.Receive.resp_message);
		sprintf(Region01.Issue_Info.temp_cp_code,  "%4.4s", kField.Send.cp_code);
		memcpy(kField.Receive.issue_count, &recvbuf[126], 2); // 발권 매수
		memcpy(&kField2, &kField, sizeof(KIOSK_ID_FIELD));
//		field_not_erase = 1;
        aDebugMsg("  상품 재고 체크 성공! 임시그룹과금번호=%s, 발권매수=%2.2s", Region01.Issue_Info.temp_group_pay_num, kField.Receive.issue_count);
        //trans_print(SERVKIND);
        disconnect();
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS902;
		qdata.qs902_resp.response_code = 0;
		del_space(qdata.qs902_resp.t_field.message, kField.Receive.resp_message, sizeof(kField.Receive.resp_message));
		qdata.qs902_resp.t_field.enable_count = (int)a2l(kField.Receive.issue_count, 1);
		send_msgq(&qdata, 1);
		log_printf(1, "재고체크(S902) 성공");
#endif
    }
    else // 오류메시지 처리
    {
        disconnect();
        aDebugMsg("상품 재고 체크 실패:%s", kField.Receive.resp_message);
		log_printf(1, "재고체크(S902) 실패:%4.4s", kField.respcode);
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS902;
		if(kField.respcode[0] == 'E' || kField.respcode[0] == 'e')
			kField.respcode[0] = '8';
		qdata.qs902_resp.response_code = (int)a2l(kField.respcode, 4);
		del_space(qdata.qs902_resp.t_field.message, kField.Receive.resp_message, sizeof(kField.Receive.resp_message));
		qdata.qs902_resp.t_field.enable_count = (int)a2l(kField.Receive.issue_count, 1);
		send_msgq(&qdata, 1);
#endif
        return(2);
    }

    return(0);
}
//------------------------------------------------------------------------------------------------------
// type=0 정상발권, type=1 테스트발권
//------------------------------------------------------------------------------------------------------
int issue_app(byte type)
{
	int servkind = KIOSK_SERVKIND;
	byte ret;
	t_qdata qdata;
	byte tbuf[20], tbuf2[20];
	int issue_cnt=0;

	memcpy(&kField, &kField2, sizeof(KIOSK_ID_FIELD));
	
    get_dtm(kField.MakeTimeStamp);
	memset(tbuf, 0, sizeof(tbuf));
	memcpy(tbuf, kField.MakeTimeStamp, 12);
#ifdef SELF_DEBUG
	msgq_buf.qs903_ask.t_field.approval_type = 3; // 현금:3, RF:4, 신용카드:5
	msgq_buf.qs903_ask.t_field.cp_code = 43; // 넥슨카드
	msgq_buf.qs903_ask.t_field.cp_amount = 5000; // 발권 낱장 금액
	msgq_buf.qs903_ask.t_field.cp_count = 1; // 총발권 매수
	msgq_buf.qs903_ask.t_field.amount = 5000; //msgq_buf.qs903_ask.t_field.cp_amount * msgq_buf.qs903_ask.t_field.cp_count; // 거래총금액
	printf("티켓 발권 유형? (1:발권, 2:SMS) \n");
	scanf("%s", tbuf2);
	ret = (byte)atoi(tbuf2);
	if(ret != 2)
		msgq_buf.qs903_ask.t_field.ticket_type = 1;// 티켓 발권 유형, 발권:1, SMS:2
	else
		msgq_buf.qs903_ask.t_field.ticket_type = 2;// 티켓 발권 유형, 발권:1, SMS:2
		
	sprintf(msgq_buf.qs903_ask.t_field.ticket_user_number, "%-16.16s", "01053616681"); // 고객 연락처, SMS 발권시 고객 전화번호
	
	sprintf(kField.Send.temp_group_pay_num, "%-50.50s", Region01.Issue_Info.temp_group_pay_num);
	sprintf(kField.Send.approval_code, "%02d", msgq_buf.qs903_ask.t_field.approval_type);
	sprintf(kField.Send.pg_tran_num, "%-50.50s", ""); // PG사 거래번호
	sprintf(kField.Send.pg_approval_num, "%-50.50s", ""); // PG사 승인번호
	sprintf(kField.Send.pg_tran_date, "20%-12.12s", tbuf); // PG사 거래일시
	sprintf(kField.Send.payed_tamount, "%-10d", msgq_buf.qs903_ask.t_field.amount); // 고객이 지불한 총금액
	sprintf(kField.Send.issue_type, "%1d", msgq_buf.qs903_ask.t_field.ticket_type); // 발권 방식
	sprintf(kField.Send.client_phone, "%16.16s", msgq_buf.qs903_ask.t_field.ticket_user_number); // SMS 발권시 고객 전화번호
#else
	sprintf(kField.Send.temp_group_pay_num, "%-50.50s", Region01.Issue_Info.temp_group_pay_num);
	sprintf(kField.Send.approval_code, "%02d", msgq_buf.qs903_ask.t_field.approval_type);
	sprintf(kField.Send.pg_tran_num, "%-50.50s", ""); // PG사 거래번호
	sprintf(kField.Send.pg_approval_num, "%-50.50s", ""); // PG사 승인번호
	sprintf(kField.Send.pg_tran_date, "20%-12.12s", tbuf); // PG사 거래일시
	sprintf(kField.Send.payed_tamount, "%10d", msgq_buf.qs903_ask.t_field.amount); // 고객이 지불한 총금액
	sprintf(kField.Send.issue_type, "%1d", msgq_buf.qs903_ask.t_field.ticket_type); // 발권 방식
//	sprintf(kField.Send.client_phone, "%16.16s", msgq_buf.qs903_ask.t_field.ticket_user_number); // SMS 발권시 고객 전화번호
	sprintf(kField.Send.client_phone, "%16.16s", msgq_buf.qs903_ask.t_field.ticket_user_number); // SMS 발권시 고객 전화번호
#endif
	if(type==1)
	{
		msgq_buf.qs903_ask.t_field.approval_type = 3; // 현금:3, RF:4, 신용카드:5
		msgq_buf.qs903_ask.t_field.cp_code = 17; // 틴캐시
		msgq_buf.qs903_ask.t_field.cp_amount = 1004; // 발권 낱장 금액
		msgq_buf.qs903_ask.t_field.cp_count = 1; // 총발권 매수
		msgq_buf.qs903_ask.t_field.amount = 1004; //msgq_buf.qs903_ask.t_field.cp_amount * msgq_buf.qs903_ask.t_field.cp_count; // 거래총금액
		printf("티켓 발권 유형? (1:발권, 2:SMS) \n");
		msgq_buf.qs903_ask.t_field.ticket_type = 1;// 티켓 발권 유형, 발권:1, SMS:2
		sprintf(msgq_buf.qs903_ask.t_field.ticket_user_number, "%-16.16s", "01053616681"); // 고객 연락처, SMS 발권시 고객 전화번호
		
		sprintf(kField.Send.temp_group_pay_num, "%-50.50s", Region01.Issue_Info.temp_group_pay_num);
		sprintf(kField.Send.approval_code, "%02d", msgq_buf.qs903_ask.t_field.approval_type);
		sprintf(kField.Send.pg_tran_num, "%-50.50s", ""); // PG사 거래번호
		sprintf(kField.Send.pg_approval_num, "%-50.50s", ""); // PG사 승인번호
		sprintf(kField.Send.pg_tran_date, "20%-12.12s", tbuf); // PG사 거래일시
		sprintf(kField.Send.payed_tamount, "%-10d", msgq_buf.qs903_ask.t_field.amount); // 고객이 지불한 총금액
		sprintf(kField.Send.issue_type, "%1d", msgq_buf.qs903_ask.t_field.ticket_type); // 발권 방식
		sprintf(kField.Send.client_phone, "%16.16s", msgq_buf.qs903_ask.t_field.ticket_user_number); // SMS 발권시 고객 전화번호
	}

	aDebugMsg("issue_app() started...");
	del_space(tempbuf, Region01.Issue_Info.temp_group_pay_num, sizeof(Region01.Issue_Info.temp_group_pay_num));
	log_printf(1, "발권(S903) 요청:그룹과금번호=%s, 금액=%d, 발권방식=%s", tempbuf, msgq_buf.qs903_ask.t_field.amount, kField.Send.issue_type);
	make_packet(servkind);
    ret = host_msganal(servkind, 1);
    disconnect();
    if(ret) return ret;
#if 1
	qdata.command_type = qETHERNET;
	qdata.command_code = qETHERNET_CONNECT;
	qdata.qlan_resp.response_code = 0; // 통신정상완료
	qdata.qlan_resp.t_field.progress = 100;
	send_msgq(&qdata, 1);
#endif

//	kField.respcode[0] = 'E';
		
    if (memcmp(kField.respcode, "0000", 4)==0)
    {
		sprintf(kField.Receive.fix_group_pay_num,  "%-50.50s", kField.Receive.resp_message);
		memcpy(kField.Receive.issue_count, &recvbuf[126], 2); // 발권 매수
		{
			int i=0;
			issue_cnt = (int)a2l(kField.Receive.issue_count, 2);
			for(i=0; i<issue_cnt; i++)
			{
				sprintf(kField.Receive.pin_num[i], "%-24.24s", &recvbuf[128+(i*84)]); // 핀번호(인증키), 핀번호
				sprintf(kField.Receive.pin_manage_num[i], "%-20.20s", &recvbuf[152+(i*84)]); // 핀 관리번호
				sprintf(kField.Receive.pin_issue_amount[i], "%-10.10s", &recvbuf[172+(i*84)]); // 핀 액면가
				sprintf(kField.Receive.issue_date[i], "%-10.10s", &recvbuf[182+(i*84)]); // 발행일, 해피머니 외에는 빈공간
				sprintf(kField.Receive.pin_manage_num2[i], "%-20.20s", &recvbuf[192+(i*84)]); // 핀 관리번호2, 핀 관리번호와 동일 (해피머니만 다름)
			}
		}
		sprintf(kField.Receive.event_message, "%-1000.1000s", &recvbuf[212+((issue_cnt-1)*84)]); // 이벤트 메시지
		sprintf(kField.Receive.event_message_pos, "%1.1s", &recvbuf[1212+((issue_cnt-1)*84)]); // 이벤트 메시지 위치, 상:1, 하:2
		sprintf(kField.Receive.vaccount_balance, "%-20.20s", &recvbuf[1213+((issue_cnt-1)*84)]); // 가상 계좌 잔액
//		field_not_erase = 0;
		//---------------------------------- 여기서부터 발권 매수별 반복 ------------------------------------------------------------------
//		sprintf(kField.Receive.pin_num, "%-24.24s", &recvbuf[128]); // 핀번호(인증키), 핀번호
//		sprintf(kField.Receive.pin_manage_num, "%-20.20s", &recvbuf[152]); // 핀 관리번호
//		sprintf(kField.Receive.pin_issue_amount, "%-10.10s", &recvbuf[172]); // 핀 액면가
//		sprintf(kField.Receive.issue_date, "%-10.10s", &recvbuf[182]); // 발행일, 해피머니 외에는 빈공간
//		sprintf(kField.Receive.pin_manage_num2, "%-20.20s", &recvbuf[192]); // 핀 관리번호2, 핀 관리번호와 동일 (해피머니만 다름)
		//---------------------------------------------------------------------------------------------------------------------------------
        aDebugMsg("  발권 요청 성공! 확정그룹과금번호=%s, 발권매수=%s", kField.Receive.fix_group_pay_num, kField.Receive.issue_count);
        aDebugMsg("  이벤트 메시지=%s, 이벤트 메시지 위치=%s, 가상계좌잔액=%s", kField.Receive.event_message, kField.Receive.event_message_pos, kField.Receive.vaccount_balance);
		trans_capture(servkind);
        trans_print(servkind);
        disconnect();
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS903;
		qdata.qs903_resp.response_code = 0;
		qdata.qs903_resp.t_field.cp_count = (int)a2l(kField.Receive.issue_count, 2);
		del_space(qdata.qs903_resp.t_field.event_msg, kField.Receive.event_message, sizeof(kField.Receive.event_message));
		qdata.qs903_resp.t_field.event_msg_position = kField.Receive.event_message_pos[0];
		del_space(tempbuf, kField.Receive.vaccount_balance, sizeof(kField.Receive.vaccount_balance));
		qdata.qs903_resp.t_field.vaccount_balance = atoi(tempbuf);
		send_msgq(&qdata, 1);
		log_printf(1, "발권(S903) 요청 성공");
#endif
    }
    else // 오류메시지 처리
    {
        disconnect();
        aDebugMsg("발권 요청 실패:%s", kField.Receive.resp_message);
		log_printf(1, "발권(S903) 요청 실패:%4.4s", kField.respcode);
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS903;
		if(kField.respcode[0] == 'E' || kField.respcode[0] == 'e')
			kField.respcode[0] = '8';
		qdata.qs903_resp.response_code = (int)a2l(kField.respcode, 4);
		aDebugMsg(">> response_code = %d", qdata.qs903_resp.response_code);
		qdata.qs903_resp.t_field.cp_count = (int)a2l(kField.Receive.issue_count, 2);
		del_space(qdata.qs903_resp.t_field.event_msg, kField.Receive.event_message, sizeof(kField.Receive.event_message));
		qdata.qs903_resp.t_field.event_msg_position = kField.Receive.event_message_pos[0];
		del_space(tempbuf, kField.Receive.vaccount_balance, sizeof(kField.Receive.vaccount_balance));
		qdata.qs903_resp.t_field.vaccount_balance = atoi(tempbuf);
		send_msgq(&qdata, 1);
#endif
        return(2);
    }

    return(0);
}
//------------------------------------------------------------------------------------------------------
int issue_can(void)
{
	int servkind = KIOSK_SERVKIND;
	byte ret;
	t_qdata qdata;
	int issue_cnt=0;

#ifdef SELF_DEBUG	
	msgq_buf.qs905_ask.t_field.cancel_type = 'C'; // 취소유형, 관리자 발권취소=‘T', 관리자 단건취소='P', 고객 단건취소=‘C'
	msgq_buf.qs905_ask.t_field.cp_code = 43; // 넥슨카드
	printf("\n발행일자를 입력해 주세요:예=20140816> ");
	scanf("%s", msgq_buf.qs905_ask.t_field.issue_date); // 발행일자 (UI 입력된 8자리)
	printf("\n취소번호를 입력해 주세요:예=12345678> ");
	scanf("%s", msgq_buf.qs905_ask.t_field.cancel_num); // 취소번호 (UI 입력된 8자리)
	sprintf(msgq_buf.qs905_ask.t_field.manage_num, "%24.24", ""); // 관리번호, 모두 스페이스
	msgq_buf.qs905_ask.t_field.cp_amount = 0; // 액면가
	msgq_buf.qs905_ask.t_field.cancel_amount = 0; // 발권 취소금액
#if 0
	sprintf(msgq_buf.qs905_ask.t_field.client_phone, "%-16.16s", "01053616681"); // 고객연락처, 고객 단건 취소시 환불용
	sprintf(msgq_buf.qs905_ask.t_field.client_bank_name, "%-20.20s", "기업은행"); // 고객 은행이름
	sprintf(msgq_buf.qs905_ask.t_field.client_account, "%-20.20s", "1234567890123"); // 고객 은행계좌번호, 고객 단건 취소시 환불용
	sprintf(msgq_buf.qs905_ask.t_field.client_name, "%-24.24s", "김현동"); // 은행예금주명
#endif
	msgq_buf.qs905_ask.t_field.error_code = 1;// 오류 구분코드, 1:사용자취소, 2:망취소, 3:용지걸림, 4:용지부족, 7:핀번호 수신실패, 20:기타H/W오류

	sprintf(kField.Send.cancel_type, "%c", msgq_buf.qs905_ask.t_field.cancel_type); // 취소유형
	if(kField.Send.cancel_type[0] == 'T') // 발권 취소
	{
		sprintf(kField.Send.group_pay_num, "%-50.50s", "");
		sprintf(kField.Send.can_amount, "%10d", msgq_buf.qs905_ask.t_field.cancel_amount); 

		sprintf(kField.Send.cp_code, "%04s", "");
		sprintf(kField.Send.pin_date, "%-8.8s", "");
		sprintf(kField.Send.pin_num, "%-24.24s", "");
		sprintf(kField.Send.pin_manage_num, "%-20.20s", "");
		sprintf(kField.Send.pin_issue_amount, "%10s", ""); 
//		sprintf(kField.Send.can_amount, "%-10s", ""); 
		sprintf(kField.Send.client_phone, "%-16.16s", ""); 
		sprintf(kField.Send.client_bank_name, "%-20.20s", ""); 
		sprintf(kField.Send.client_account, "%-20.20s", ""); 
		sprintf(kField.Send.client_name, "%-24.24s", ""); 
		sprintf(kField.Send.error_code, "%02d", msgq_buf.qs905_ask.t_field.error_code); 
	}
	else if(kField.Send.cancel_type[0] == 'P') // 관리자 단건 취소
	{
		sprintf(kField.Send.cp_code, "%04d", msgq_buf.qs905_ask.t_field.cp_code);
		sprintf(kField.Send.pin_date, "%-8.8s", msgq_buf.qs905_ask.t_field.issue_date);
		sprintf(kField.Send.pin_num, "%-24.24s", msgq_buf.qs905_ask.t_field.manage_num);
		sprintf(kField.Send.pin_manage_num, "%%-20.20s", msgq_buf.qs905_ask.t_field.cancel_num);
		sprintf(kField.Send.pin_issue_amount, "%-10d", msgq_buf.qs905_ask.t_field.cp_amount); 
		sprintf(kField.Send.can_amount, "%-10d", msgq_buf.qs905_ask.t_field.cancel_amount); 

		sprintf(kField.Send.group_pay_num, "%-50.50s", "");
		sprintf(kField.Send.client_phone, "%-16.16s", "");
		sprintf(kField.Send.client_bank_name, "%-20.20s", "");
		sprintf(kField.Send.client_account, "%-20.20s", "");
		sprintf(kField.Send.client_name, "%-24.24s", "");
		sprintf(kField.Send.error_code, "%02d", msgq_buf.qs905_ask.t_field.error_code); 
	}
	else if(kField.Send.cancel_type[0] == 'C') // 고객 단건 취소
	{
		sprintf(kField.Send.cp_code, "%04d", msgq_buf.qs905_ask.t_field.cp_code);
		sprintf(kField.Send.pin_date, "%-8.8s", msgq_buf.qs905_ask.t_field.issue_date);
		sprintf(kField.Send.pin_manage_num, "%-20.20s", msgq_buf.qs905_ask.t_field.cancel_num);
		sprintf(kField.Send.pin_issue_amount, "%10d", msgq_buf.qs905_ask.t_field.cp_amount); 
		sprintf(kField.Send.can_amount, "%10d", msgq_buf.qs905_ask.t_field.cancel_amount); 
		sprintf(kField.Send.client_phone, "%-16.16s", msgq_buf.qs905_ask.t_field.client_phone); 
		sprintf(kField.Send.client_bank_name, "%-20.20s", msgq_buf.qs905_ask.t_field.client_bank_name); 
		sprintf(kField.Send.client_account, "%-20.20s", msgq_buf.qs905_ask.t_field.client_account); 
		sprintf(kField.Send.client_name, "%-24.24s", msgq_buf.qs905_ask.t_field.client_name); 

		sprintf(kField.Send.group_pay_num, "%-50.50s", "");
		sprintf(kField.Send.pin_num, "%-24.24s", msgq_buf.qs905_ask.t_field.manage_num);
		sprintf(kField.Send.error_code, "%02d", msgq_buf.qs905_ask.t_field.error_code);
	}
#else
	sprintf(kField.Send.cancel_type, "%c", msgq_buf.qs905_ask.t_field.cancel_type); // 취소유형
	if(kField.Send.cancel_type[0] == 'T') // 발권 취소
	{
		switch(msgq_buf.qs905_ask.t_field.error_code)
		{
			case 1: // 사용자취소
				break;
			case 3: // 용지걸림
				break;
			case 4: // 용지부족
				break;
			case 2: // 망취소
			case 7: // 핀번호 수신실패, 통신에러
				memcpy(&kField, &kField2, sizeof(KIOSK_ID_FIELD));
				sprintf(kField.Send.group_pay_num, "%-50.50s", "");
				sprintf(kField.Send.cancel_type, "%c", msgq_buf.qs905_ask.t_field.cancel_type); // 취소유형
				sprintf(kField.Send.can_amount, "%10d", msgq_buf.qs905_ask.t_field.cancel_amount); 

				sprintf(kField.Send.cp_code, "%04s", "");
				sprintf(kField.Send.pin_date, "%-8.8s", "");
				sprintf(kField.Send.pin_num, "%-24.24s", "");
				sprintf(kField.Send.pin_manage_num, "%-20.20s", "");
				sprintf(kField.Send.pin_issue_amount, "%10s", ""); 
				sprintf(kField.Send.can_amount, "%10s", ""); 
				sprintf(kField.Send.client_phone, "%-16.16s", ""); 
				sprintf(kField.Send.client_bank_name, "%-20.20s", ""); 
				sprintf(kField.Send.client_account, "%-20.20s", ""); 
				sprintf(kField.Send.client_name, "%-24.24s", ""); 
				sprintf(kField.Send.error_code, "%02d", msgq_buf.qs905_ask.t_field.error_code); 
				break;
			case 20: // 기타 H/W 오류
				break;
		}
		if(msgq_buf.qs905_ask.t_field.error_code==2)
		{
			sprintf(kField.Send.group_pay_num, "%-50.50s", Region01.Issue_Info.temp_group_pay_num);
			sprintf(kField.Send.can_amount, "%10d", Region01.Issue_Info.bill_input_amount); 
//			sprintf(kField.Send.can_amount, "%10d", msgq_buf.qs905_ask.t_field.cancel_amount); 
			sprintf(kField.Send.client_phone, "%-16.16s", msgq_buf.qs905_ask.t_field.client_phone); 
			sprintf(kField.Send.client_bank_name, "%-20.20s", msgq_buf.qs905_ask.t_field.client_bank_name); 
			sprintf(kField.Send.client_account, "%-20.20s", msgq_buf.qs905_ask.t_field.client_account); 
			sprintf(kField.Send.client_name, "%-24.24s", msgq_buf.qs905_ask.t_field.client_name); 
			sprintf(kField.Send.error_code, "%02d", msgq_buf.qs905_ask.t_field.error_code);
			network_cancel_flag = 1;
		}
	}
	else if(kField.Send.cancel_type[0] == 'P') // 관리자 단건 취소
	{
		sprintf(kField.Send.cp_code, "%04d", msgq_buf.qs905_ask.t_field.cp_code);
		sprintf(kField.Send.pin_date, "%-8.8s", msgq_buf.qs905_ask.t_field.issue_date);
		sprintf(kField.Send.pin_num, "%-24.24s", msgq_buf.qs905_ask.t_field.manage_num);
		sprintf(kField.Send.pin_manage_num, "%%-20.20s", msgq_buf.qs905_ask.t_field.cancel_num);
		sprintf(kField.Send.pin_issue_amount, "%10d", msgq_buf.qs905_ask.t_field.cp_amount); 
		sprintf(kField.Send.can_amount, "%10d", msgq_buf.qs905_ask.t_field.cancel_amount); 

		sprintf(kField.Send.group_pay_num, "%-50.50s", "");
		sprintf(kField.Send.client_phone, "%-16.16s", "");
		sprintf(kField.Send.client_bank_name, "%-20.20s", "");
		sprintf(kField.Send.client_account, "%-20.20s", "");
		sprintf(kField.Send.client_name, "%-24.24s", "");
		sprintf(kField.Send.error_code, "%02d", msgq_buf.qs905_ask.t_field.error_code); 
	}
	else if(kField.Send.cancel_type[0] == 'C') // 고객 단건 취소
	{
		switch(msgq_buf.qs905_ask.t_field.error_code)
		{
			case 1: // 사용자취소
				break;
			case 2: // 망취소
				break;
			case 3: // 용지걸림
				break;
			case 4: // 용지부족
				break;
			case 7: // 핀번호 수신실패, 통신에러
				memcpy(&kField, &kField2, sizeof(KIOSK_ID_FIELD));
				break;
			case 20: // 기타 H/W 오류
				break;
		}
		sprintf(kField.Send.cp_code, "%04d", msgq_buf.qs905_ask.t_field.cp_code);
		if(msgq_buf.qs905_ask.t_field.error_code==7)
		{
			sprintf(kField.Send.pin_date, "%-8.8s", "");
			sprintf(kField.Send.pin_manage_num, "%-20.20s", "");
			sprintf(kField.Send.group_pay_num, "%-50.50s", kField2.Receive.temp_group_pay_num);
		}
		else
		{
			sprintf(kField.Send.pin_date, "%-8.8s", msgq_buf.qs905_ask.t_field.issue_date);
			sprintf(kField.Send.pin_manage_num, "%-20.20s", msgq_buf.qs905_ask.t_field.cancel_num);
			sprintf(kField.Send.group_pay_num, "%-50.50s", "");
		}
		sprintf(kField.Send.pin_issue_amount, "%10d", msgq_buf.qs905_ask.t_field.cp_amount); 
		sprintf(kField.Send.can_amount, "%10d", msgq_buf.qs905_ask.t_field.cancel_amount); 
		sprintf(kField.Send.client_phone, "%-16.16s", msgq_buf.qs905_ask.t_field.client_phone); 
		sprintf(kField.Send.client_bank_name, "%-20.20s", msgq_buf.qs905_ask.t_field.client_bank_name); 
		sprintf(kField.Send.client_account, "%-20.20s", msgq_buf.qs905_ask.t_field.client_account); 
		sprintf(kField.Send.client_name, "%-24.24s", msgq_buf.qs905_ask.t_field.client_name); 
		sprintf(kField.Send.pin_num, "%-24.24s", msgq_buf.qs905_ask.t_field.manage_num);
		sprintf(kField.Send.error_code, "%02d", msgq_buf.qs905_ask.t_field.error_code);
	}
#endif
    get_dtm(kField.MakeTimeStamp);
	aDebugMsg("issue_can() started...");
	if(msgq_buf.qs905_ask.t_field.error_code==7) // 망취소
	{
		del_space(tempbuf, kField.Send.group_pay_num, sizeof(kField.Send.group_pay_num));
		log_printf(1, "망취소(S905) 요청:CP Code=%4.4s, 그룹과금번호=%s, 취소금액=%d", kField.Send.cp_code, tempbuf, msgq_buf.qs905_ask.t_field.cancel_amount);
	}
	else
	{
		del_space(tempbuf, kField.Send.pin_manage_num, sizeof(kField.Send.pin_manage_num));
		log_printf(1, "취소(S905) 요청:CP Code=%4.4s, 취소번호=%s, 발행일자=%8.8s", kField.Send.cp_code, tempbuf, kField.Send.pin_date);
	}
	make_packet(servkind);
    ret = host_msganal(servkind, 1);
    disconnect();
    if(ret) return ret;

#if 1
	qdata.command_type = qETHERNET;
	qdata.command_code = qETHERNET_CONNECT;
	qdata.qlan_resp.response_code = 0; // 통신정상완료
	qdata.qlan_resp.t_field.progress = 100;
	send_msgq(&qdata, 1);
#endif

    if (memcmp(kField.respcode, "0000", 4)==0)
    {
		sprintf(kField.Receive.fix_group_pay_num,  "%-50.50s", kField.Receive.resp_message);
		sprintf(kField.Receive.cancel_ok_count, "%-2.2s", &recvbuf[126]); 	// 발권취소 성공매수
		sprintf(kField.Receive.cancel_tcount, "%-2.2s", &recvbuf[128]); 	// 발권취소 전체매수 (이 개수만큼 6개 필드 반복)
		{
			int i=0;
			issue_cnt = (int)a2l(kField.Receive.cancel_tcount, 2);
			for(i=0; i<issue_cnt; i++)
			{
				sprintf(kField.Receive.cancel_unit_num[i], "%-2.2s", 	&recvbuf[130+(i*61)]); 	// 발권취소 순번
				sprintf(kField.Receive.pin_num[i], "%-24.24s", 			&recvbuf[132+(i*61)]); 	// 핀 번호 (인증키)
				sprintf(kField.Receive.pin_manage_num[i], "%-20.20s", 	&recvbuf[156+(i*61)]); 	// 핀 관리번호
				sprintf(kField.Receive.pin_issue_amount[i], "%-10.10s", &recvbuf[176+(i*61)]);	// 핀 액면가
				sprintf(kField.Receive.cancel_success[i], "%c", 			&recvbuf[186+(i*61)]); 	// 취소 성공 여부
				sprintf(kField.Receive.cancel_fail_code[i], "%-4.4s", 	&recvbuf[187+(i*61)]); 	// 취소 실패 코드
			}
		}
		sprintf(kField.Receive.vaccount_balance, "%20.20s", &recvbuf[191+((issue_cnt-1)*61)]); // 가상 계좌 잔액
        aDebugMsg("  발권/단건 취소 성공! 그룹과금번호=%s, 취소총매수=%s, 취소된매수", kField.Receive.fix_group_pay_num, kField.Receive.cancel_tcount, kField.Receive.cancel_ok_count);
        aDebugMsg("  가상계좌잔액=%s", kField.Receive.vaccount_balance);
		trans_capture(servkind);
        trans_print(servkind);
        disconnect();
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS905;
		qdata.qs905_resp.response_code = 0;
		qdata.qs905_resp.t_field.cancel_ok_count = (int)a2l(kField.Receive.cancel_ok_count, 2);
		qdata.qs905_resp.t_field.cancel_tcount = (int)a2l(kField.Receive.cancel_tcount, 2);
		del_space(tempbuf, kField.Receive.vaccount_balance, sizeof(kField.Receive.vaccount_balance));
		qdata.qs905_resp.t_field.vaccount_balance = (long)atoi(tempbuf);
		send_msgq(&qdata, 1);
		log_printf(1, "취소(S905) 요청 성공");
#endif
    }
    else // 오류메시지 처리
    {
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS905;
		if(kField.respcode[0] == 'E' || kField.respcode[0] == 'e')
			kField.respcode[0] = '8';
		qdata.qs905_resp.response_code = (int)a2l(kField.respcode, 4); // 905에는 msg 50바이트 필드 정의를 않함...
		send_msgq(&qdata, 1);
#endif
        disconnect();
        aDebugMsg("발권/단건 취소 실패:%s", kField.Receive.resp_message);
		log_printf(1, "취소(S905) 요청 실패:%4.4s", kField.respcode);
        return(2);
    }

	return 0;
}
//------------------------------------------------------------------------------------------------------
int operation_check(void)
{
	int servkind = KIOSK_SERVKIND;
	byte ret;
	t_qdata qdata;

#ifdef SELF_DEBUG
	msgq_buf.qs906_ask.t_field.error_code = 8; // 고객 임의 지원요청
	sprintf(msgq_buf.qs906_ask.t_field.client_num, "01053616681"); // 고객연락처
#endif
	sprintf(kField.Send.group_pay_num, "%-50.50s", "");
	sprintf(kField.Send.cash_num, "%-20.20s", "");
	sprintf(kField.Send.error_type_code, "%02d", msgq_buf.qs906_ask.t_field.error_code);
	sprintf(kField.Send.client_phone2, "%-20.20s", msgq_buf.qs906_ask.t_field.client_num);
	sprintf(kField.Send.etc_information, "%-100.100s", "");
	
    get_dtm(kField.MakeTimeStamp);
	aDebugMsg("operation_check() started...");
	make_packet(servkind);
    ret = host_msganal(servkind, 1);
    disconnect();
    if(ret) return ret;
#if 1
	qdata.command_type = qETHERNET;
	qdata.command_code = qETHERNET_CONNECT;
	qdata.qlan_resp.response_code = 0; 
	qdata.qlan_resp.t_field.progress = 100; // 통신정상완료
	send_msgq(&qdata, 1);
#endif

    if (memcmp(kField.respcode, "0000", 4)==0)
    {
		sprintf(kField.Receive.vaccount,  "%-50.50s", kField.Receive.resp_message);

        aDebugMsg("  운영오류상황 기록 성공! ");
        disconnect();
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS906;
		qdata.qs906_resp.response_code = 0;
		send_msgq(&qdata, 1);
#endif
    }
    else // 오류메시지 처리
    {
        aDebugMsg("운영오류상황 기록 실패:%s", kField.Receive.resp_message);
        disconnect();
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS906;
		if(kField.respcode[0] == 'E' || kField.respcode[0] == 'e')
			kField.respcode[0] = '8';
		qdata.qs906_resp.response_code = (int)a2l(kField.respcode, 4);
		memcpy(qdata.qs906_resp.t_field.message, kField.Receive.resp_message, 50);
		send_msgq(&qdata, 1);
#endif
        return(2);
    }

    return(0);
}
//------------------------------------------------------------------------------------------------------
int adjust_check(void)
{
	int servkind = KIOSK_SERVKIND;
	byte ret;
	t_qdata qdata;
	ulong in_amount=0, app_amount=0, can_amount=0, tempcan_amount=0, net_can_amount=0;
	ulong from_date, from_time, to_date, to_time;
	
    get_dtm(kField.MakeTimeStamp);

	sprintf(kField.Send.adjust_ref_time, "20%2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s", kField.MakeTimeStamp, &kField.MakeTimeStamp[2], &kField.MakeTimeStamp[4], &kField.MakeTimeStamp[6], &kField.MakeTimeStamp[8], &kField.MakeTimeStamp[10]);

	aDebugMsg("adjust_check() started...");
	make_packet(servkind);
    ret = host_msganal(servkind, 1);
    disconnect();
    if(ret) return ret;
#if 1
	qdata.command_type = qETHERNET;
	qdata.command_code = qETHERNET_CONNECT;
	qdata.qlan_resp.response_code = 0; 
	qdata.qlan_resp.t_field.progress = 100; // 통신정상완료
	send_msgq(&qdata, 1);
#endif

    if (memcmp(kField.respcode, "0000", 4)==0)
    {
		sprintf(kField.Receive.vaccount,  "%-50.50s", kField.Receive.resp_message);
		sprintf(kField.Receive.adjust_run_time,  "%-19.19s", 					&recvbuf[126]); // 정산마감 기준시간
		sprintf(kField.Receive.not_adjust_tamount,  "%-10.10s", 				&recvbuf[145]); // 미정산 총액
		sprintf(kField.Receive.not_adjust_sale_tamount,  "%-10.10s", 			&recvbuf[155]); // 미정산 판매총액
		sprintf(kField.Receive.not_adjust_temp_payed_tamount,  "%-10.10s", 	&recvbuf[165]); // 미정산 부분납총액

        aDebugMsg("  정산마감 금액확인 성공! ");
        aDebugMsg("  가상계좌=%s", kField.Receive.vaccount);
        aDebugMsg("  정산마감 기준시간=%s, 미정산 총액=%s, 미정산 판매총액=%s, 미정산 부분납총액=%s", kField.Receive.adjust_run_time, kField.Receive.not_adjust_tamount, kField.Receive.not_adjust_sale_tamount, kField.Receive.not_adjust_temp_payed_tamount);
        //trans_print(servkind);
        disconnect();
		sprintf(Region01.Jungsan_Info.adjust_run_time, "%2.2s%2.2s%2.2s%2.2s%2.2s%2.2s", &kField.Receive.adjust_run_time[2], &kField.Receive.adjust_run_time[5], &kField.Receive.adjust_run_time[8], &kField.Receive.adjust_run_time[11], &kField.Receive.adjust_run_time[14], &kField.Receive.adjust_run_time[17]);
#if 1
		from_date = a2l(Region01.Jungsan_Info.ymdsms, 6);
		from_time = a2l(&Region01.Jungsan_Info.ymdsms[6], 6);
		to_date = a2l(Region01.Jungsan_Info.adjust_run_time, 6);
		to_time = a2l(&Region01.Jungsan_Info.adjust_run_time[6], 6);

		adjust_amount_cal(&in_amount, &app_amount, &can_amount, &net_can_amount, &tempcan_amount, from_date, from_time, to_date, to_time);
	
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS907;
		qdata.qs907_resp.response_code = 0;
		memcpy(qdata.qs907_resp.t_field.vaccount_inform, kField.Receive.resp_message, 50);
		memcpy(qdata.qs907_resp.t_field.reference_time, kField.Receive.adjust_run_time, 19);

		qdata.qs907_resp.t_field.not_magam_total_amount = in_amount;					// 기기내총금액
		qdata.qs907_resp.t_field.not_magam_sale_amount = app_amount - can_amount;		// 정상판매금액 (총승인금액 - 취소금액)
		qdata.qs907_resp.t_field.not_magam_part_amount = tempcan_amount+net_can_amount; // 부분납금액
		qdata.qs907_resp.t_field.not_magam_approval_amount = app_amount; 				// 총승인금액
		
		send_msgq(&qdata, 1);
#endif
    }
    else // 오류메시지 처리
    {
        aDebugMsg("정산마감 금액확인 실패:%s", kField.Receive.resp_message);
        disconnect();
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS907;
		if(kField.respcode[0] == 'E' || kField.respcode[0] == 'e')
			kField.respcode[0] = '8';
		qdata.qs907_resp.response_code = (int)a2l(kField.respcode, 4);
		memcpy(qdata.qs907_resp.t_field.vaccount_inform, kField.Receive.resp_message, 50);
		send_msgq(&qdata, 1);
#endif
        return(2);
    }

    return(0);
}
//------------------------------------------------------------------------------------------------------
int adjust_app(void)
{
	int servkind = KIOSK_SERVKIND;
	byte ret;
	t_qdata qdata;
	ulong in_amount=0, app_amount=0, can_amount=0, tempcan_amount=0, net_can_amount=0;
	ulong from_date, from_time, to_date, to_time;


    get_dtm(kField.MakeTimeStamp);
#if 0
	from_date = a2l(Region01.Jungsan_Info.adjust_run_time, 6);
	from_time = a2l(&Region01.Jungsan_Info.adjust_run_time[6], 6);
	to_date = a2l(kField.MakeTimeStamp, 6);
	to_time = a2l(&kField.MakeTimeStamp[6], 6);
#endif
#if 1
	from_date = a2l(Region01.Jungsan_Info.ymdsms, 6);
	from_time = a2l(&Region01.Jungsan_Info.ymdsms[6], 6);
	to_date = a2l(Region01.Jungsan_Info.adjust_run_time, 6);
	to_time = a2l(&Region01.Jungsan_Info.adjust_run_time[6], 6);
#endif
	sprintf(tempbuf, "20%-12.12s", kField.MakeTimeStamp);
	sprintf(kField.Send.adjust_ref_time, "%-19.19s", tempbuf);
	adjust_amount_cal(&in_amount, &app_amount, &can_amount, &net_can_amount, &tempcan_amount, from_date, from_time, to_date, to_time);
	sprintf(kField.Send.issue_amount, "%-10.10d", app_amount);
	sprintf(kField.Send.can_amount, "%-10.10d", can_amount);
	sprintf(kField.Send.temp_payed_amount, "%-10.10d", tempcan_amount+net_can_amount);
	sprintf(kField.Send.kiosk_total_amount, "%-10.10d", in_amount);

	aDebugMsg("adjust_app() started...");
	make_packet(servkind);
    ret = host_msganal(servkind, 1);
    disconnect();
    if(ret) return ret;
#if 1
	qdata.command_type = qETHERNET;
	qdata.command_code = qETHERNET_CONNECT;
	qdata.qlan_resp.response_code = 0; 
	qdata.qlan_resp.t_field.progress = 100; // 통신정상완료
	send_msgq(&qdata, 1);
#endif

    if (memcmp(kField.respcode, "0000", 4)==0)
    {
		sprintf(kField.Receive.vaccount,  "%-50.50s", kField.Receive.resp_message);
		sprintf(kField.Receive.adjust_run_time,  "%-19.19s", 					&recvbuf[126]); // 정산마감 기준시간
		sprintf(kField.Receive.old_adjust_time,  "%-19.19s", 					&recvbuf[145]); // 이전 정산의 마감시간
		sprintf(kField.Receive.tamount_for_adjust,  "%-10.10s",				&recvbuf[164]); // 정산 마감금액
		sprintf(kField.Receive.sale_tamount,  "%-10.10s",						&recvbuf[174]); // 판매총액
		sprintf(kField.Receive.cancel_tamount,  "%-10.10s",					&recvbuf[184]); // 취소총액
		sprintf(kField.Receive.cancel_temp_payed_amount,  "%-10.10s",			&recvbuf[194]); // 부분납취소총액
		sprintf(kField.Receive.cancel_in_adjust,  "%-10.10s",					&recvbuf[204]); // 정산마감후 취소총액
		
        aDebugMsg("  정산마감 처리 성공! ");
        aDebugMsg("  가상계좌=%s", kField.Receive.vaccount);
        aDebugMsg("  정산마감 기준시간=%s, 이전 정산마감시간=%s, 정산 총액=%s, 판매총액=%s, 취소총액=%s, 부분납취소총액=%s, 정산후취소총액=%s", kField.Receive.adjust_run_time, kField.Receive.old_adjust_time, kField.Receive.tamount_for_adjust, kField.Receive.sale_tamount, kField.Receive.cancel_tamount, kField.Receive.cancel_temp_payed_amount, kField.Receive.cancel_in_adjust);
        trans_print(servkind);
        disconnect();
		sprintf(Region01.Jungsan_Info.adjust_run_time, "%2.2s%2.2s%2.2s%2.2s%2.2s%2.2s", &kField.Receive.adjust_run_time[2], &kField.Receive.adjust_run_time[5], &kField.Receive.adjust_run_time[8], &kField.Receive.adjust_run_time[11], &kField.Receive.adjust_run_time[14], &kField.Receive.adjust_run_time[17]);
		sprintf(Region01.Jungsan_Info.ymdsms, "%2.2s%2.2s%2.2s%2.2s%2.2s%2.2s", &kField.Receive.adjust_run_time[2], &kField.Receive.adjust_run_time[5], &kField.Receive.adjust_run_time[8], &kField.Receive.adjust_run_time[11], &kField.Receive.adjust_run_time[14], &kField.Receive.adjust_run_time[17]);
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS908;
		qdata.qs908_resp.response_code = 0;
		memcpy(qdata.qs908_resp.t_field.vaccount_inform, kField.Receive.resp_message, 50);
		sprintf(qdata.qs908_resp.t_field.reference_time, "%-18.18s", kField.Receive.adjust_run_time);
		sprintf(qdata.qs908_resp.t_field.old_reference_time, "%-18.18s", kField.Receive.old_adjust_time);
		del_space(tempbuf, kField.Receive.tamount_for_adjust, 10);
		qdata.qs908_resp.t_field.magam_total_amount = atoi(tempbuf);
		del_space(tempbuf, kField.Receive.sale_tamount, 10);
		qdata.qs908_resp.t_field.magam_sale_amount = atoi(tempbuf);
		del_space(tempbuf, kField.Receive.cancel_tamount, 10);
		qdata.qs908_resp.t_field.magam_cancel_amount = atoi(tempbuf);
		del_space(tempbuf, kField.Receive.cancel_temp_payed_amount, 10);
		qdata.qs908_resp.t_field.magam_part_cancel_amount = atoi(tempbuf);
		del_space(tempbuf, kField.Receive.cancel_in_adjust, 10);
		qdata.qs908_resp.t_field.magam_after_cancel_amount = atoi(tempbuf);
		send_msgq(&qdata, 1);
#endif
    }
    else // 오류메시지 처리
    {
        aDebugMsg("정산마감 금액확인 실패:%s", kField.Receive.resp_message);
        disconnect();
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS908;
		if(kField.respcode[0] == 'E' || kField.respcode[0] == 'e')
			kField.respcode[0] = '8';
		qdata.qs908_resp.response_code = (int)a2l(kField.respcode, 4);
		memcpy(qdata.qs908_resp.t_field.vaccount_inform, kField.Receive.resp_message, 50);
		send_msgq(&qdata, 1);
#endif
        return(2);
    }

	return 0;
}
//------------------------------------------------------------------------------------------------------
int admin_login_auth(void)
{
	int servkind = KIOSK_SERVKIND;
	byte ret;
	t_qdata qdata;

#ifdef SELF_DEBUG
	sprintf(msgq_buf.qs909_ask.t_field.admin_password, "1234  ");
	sprintf(kField.Send.admin_pw, "%-6.6s", msgq_buf.qs909_ask.t_field.admin_password);
#else
	sprintf(kField.Send.admin_pw, "%-6.6s", msgq_buf.qs909_ask.t_field.admin_password);
#endif
	
    get_dtm(kField.MakeTimeStamp);
	aDebugMsg("admin_login_auth() started...");
	make_packet(servkind);
    ret = host_msganal(servkind, 1);
    disconnect();
    if(ret) return ret;
#if 1
	qdata.command_type = qETHERNET;
	qdata.command_code = qETHERNET_CONNECT;
	qdata.qlan_resp.response_code = 0; // 통신정상완료
	qdata.qlan_resp.t_field.progress = 100;
	send_msgq(&qdata, 1);
#endif

    if (memcmp(kField.respcode, "0000", 4)==0)
    {
        aDebugMsg("  관리자 로그인 인증 성공! ");
        //trans_print(servkind);
        disconnect();
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS909;
		qdata.qs909_resp.response_code = 0;
		send_msgq(&qdata, 1);
#endif
    }
    else // 오류메시지 처리
    {
        aDebugMsg("관리자 로그인 인증 실패:%s", kField.Receive.resp_message);
        disconnect();
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS909;
		if(kField.respcode[0] == 'E' || kField.respcode[0] == 'e')
			kField.respcode[0] = '8';
		qdata.qs909_resp.response_code = (int)a2l(kField.respcode, 4);
		memcpy(qdata.qs909_resp.t_field.message, kField.Receive.resp_message, 50);
		send_msgq(&qdata, 1);
        return(2);
    }

	return 0;
}
//------------------------------------------------------------------------------------------------------
int temp_cancel(void)
{
	int servkind = KIOSK_SERVKIND;
	byte ret;
	t_qdata qdata;
	int issue_cnt=0;

	memcpy(&kField, &kField2, sizeof(KIOSK_ID_FIELD));

#ifdef SELF_DEBUG	
	msgq_buf.qs910_ask.t_field.part_amount = 1000; // 취소로 선수금 처리하는 (부분납)금액
	sprintf(msgq_buf.qs910_ask.t_field.user_information,"%s", "01053616681^기업은행^01053616681^김현동"); // 연락처^은행이름^계좌번호^예금주 정보
	msgq_buf.qs910_ask.t_field.error_code = 5; // 오류구분코드, 5:입금 대기시간 초과, 6:지폐걸림, 7:기타H/W오류, 8:사용자 입력대기시간 초과, 9:프린터 오류
	sprintf(kField.Send.temp_group_pay_num, "%-50.50s", Region01.Issue_Info.temp_group_pay_num);
	sprintf(kField.Send.temp_payed_amount, "%-10d", msgq_buf.qs910_ask.t_field.part_amount);
	sprintf(kField.Send.etc_information, "%-100.100s", msgq_buf.qs910_ask.t_field.user_information);
	sprintf(kField.Send.error_type_code, "%02d", msgq_buf.qs910_ask.t_field.error_code);
	Region01.Issue_Info.bill_target_amount = 5000;
	Region01.Issue_Info.bill_input_amount = 1000;
#else
	sprintf(kField.Send.temp_group_pay_num, "%-50.50s", Region01.Issue_Info.temp_group_pay_num);
	sprintf(kField.Send.temp_payed_amount, "%-10d",  msgq_buf.qs910_ask.t_field.part_amount);
	if(msgq_buf.qs910_ask.t_field.error_code==8 || msgq_buf.qs910_ask.t_field.error_code==9)
	{
		sprintf(kField.Send.etc_information, "%-100.100s", "");
		log_printf(1, "부분납취소:사용자 입력대기시간 초과!");
	}
	else
		sprintf(kField.Send.etc_information, "%-100.100s", msgq_buf.qs910_ask.t_field.user_information);

	if(msgq_buf.qs910_ask.t_field.error_code==8 || msgq_buf.qs910_ask.t_field.error_code==9)
		sprintf(kField.Send.error_type_code, "%02d", 7);
	else
		sprintf(kField.Send.error_type_code, "%02d", msgq_buf.qs910_ask.t_field.error_code);
#endif
    get_dtm(kField.MakeTimeStamp);
	aDebugMsg("temp_cancel() started...");
	del_space(tempbuf, kField.Send.temp_group_pay_num, sizeof(kField.Send.temp_group_pay_num));
	log_printf(1, "부분납취소(S910) 요청:그룹과금번호=%s, 부분납금액=%d, 에러코드=%2.2s", tempbuf, msgq_buf.qs910_ask.t_field.part_amount, kField.Send.error_type_code);
	make_packet(servkind);
    ret = host_msganal(servkind, 1);
    disconnect();
    if(ret) return ret;
#if 1
	qdata.command_type = qETHERNET;
	qdata.command_code = qETHERNET_CONNECT;
	qdata.qlan_resp.response_code = 0; // 통신정상완료
	qdata.qlan_resp.t_field.progress = 100;
		send_msgq(&qdata, 1);
#endif

    if (memcmp(kField.respcode, "0000", 4)==0)
    {
		sprintf(kField.Receive.temp_group_pay_num,  "%-50.50s", kField.Receive.resp_message);
		sprintf(kField.Receive.cash_num, "%-20.20s", &recvbuf[126]); 	// 가상계좌 사용여부에 따른 지폐함 금액 구분용
//		field_not_erase = 0;
        aDebugMsg("  부분납후 강제종료 성공! 임시그룹과금번호=%s, 캐시번호=%s", kField.Receive.temp_group_pay_num, kField.Receive.cash_num);
		trans_capture(servkind);
		if(msgq_buf.qs910_ask.t_field.error_code != 8)
	        trans_print(servkind);
        disconnect();
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS910;
		qdata.qs905_resp.response_code = 0;
		del_space(qdata.qs910_resp.t_field.message, kField.Receive.temp_group_pay_num, sizeof(kField.Receive.temp_group_pay_num));
		send_msgq(&qdata, 1);
		log_printf(1, "부분납취소(S910) 요청 성공");
#endif
    }
    else // 오류메시지 처리
    {
        disconnect();
        aDebugMsg("부분납후 강제종료:%s", kField.Receive.resp_message);
		log_printf(1, "부분납취소(S910) 요청 실패:%4.4s", kField.respcode);
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS910;
		if(kField.respcode[0] == 'E' || kField.respcode[0] == 'e')
			kField.respcode[0] = '8';
		qdata.qs910_resp.response_code = (int)a2l(kField.respcode, 4);
		memcpy(qdata.qs910_resp.t_field.message, kField.Receive.resp_message, 50);
		send_msgq(&qdata, 1);
#endif
        return(2);
    }

	return 0;
}
//------------------------------------------------------------------------------------------------------
int kiosk_onoff(void)
{
	int servkind = KIOSK_SERVKIND;
	byte ret;
	t_qdata qdata;

#ifdef SELF_DEBUG
	msgq_buf.qs911_ask.t_field.kiosk_power = 1; // 전원온:1 전원오프:2
	sprintf(kField.Send.kiosk_status, "%1d", msgq_buf.qs911_ask.t_field.kiosk_power);
#else
	sprintf(kField.Send.kiosk_status, "%1d", msgq_buf.qs911_ask.t_field.kiosk_power);
#endif

    get_dtm(kField.MakeTimeStamp);
	aDebugMsg("kiosk_onoff() started...");
	make_packet(servkind);
    ret = host_msganal(servkind, 1);
    disconnect();
    if(ret) return ret;
#if 1
	qdata.command_type = qETHERNET;
	qdata.command_code = qETHERNET_CONNECT;
	qdata.qlan_resp.response_code = 0; // 통신정상완료
	qdata.qlan_resp.t_field.progress = 100;
	send_msgq(&qdata, 1);
#endif

    if (memcmp(kField.respcode, "0000", 4)==0)
    {
        aDebugMsg("  kiosk_onoff 성공! ");
        //trans_print(servkind);
        disconnect();
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS911;
		qdata.qs911_resp.response_code = 0;
		del_space(qdata.qs911_resp.t_field.message, kField.Receive.resp_message, 50);
		send_msgq(&qdata, 1);
#endif
    }
    else // 오류메시지 처리
    {
        aDebugMsg("kiosk_onoff 실패:%s", kField.Receive.resp_message);
        disconnect();
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS911;
		if(kField.respcode[0] == 'E' || kField.respcode[0] == 'e')
			kField.respcode[0] = '8';
		qdata.qs911_resp.response_code = (int)a2l(kField.respcode, 4);
		memcpy(qdata.qs911_resp.t_field.message, kField.Receive.resp_message, 50);
		send_msgq(&qdata, 1);
#endif
        return(2);
    }

	return 0;
}
//------------------------------------------------------------------------------------------------------
int hw_error_check(void)
{
	return 0;
}
//------------------------------------------------------------------------------------------------------
int response_resend(void)
{
	return 0;
}
//------------------------------------------------------------------------------------------------------
int vaccount_balance(void)
{
	int servkind = KIOSK_SERVKIND;
	byte ret;
	t_qdata qdata;

    get_dtm(kField.MakeTimeStamp);

	sprintf(kField.Send.account_ref_time, "20%2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s", kField.MakeTimeStamp, &kField.MakeTimeStamp[2], &kField.MakeTimeStamp[4], &kField.MakeTimeStamp[6], &kField.MakeTimeStamp[8], &kField.MakeTimeStamp[10]);
	
	aDebugMsg("vaccount_balance() started...");
	make_packet(servkind);
    ret = host_msganal(servkind, 1);
    disconnect();
    if(ret) return ret;
#if 1
	qdata.command_type = qETHERNET;
	qdata.command_code = qETHERNET_CONNECT;
	qdata.qlan_resp.response_code = 0; // 통신정상완료
	qdata.qlan_resp.t_field.progress = 100;
		send_msgq(&qdata, 1);
#endif

    if (memcmp(kField.respcode, "0000", 4)==0)
    {
		sprintf(kField.Receive.vaccount,  "%-50.50s", kField.Receive.resp_message);	// 가상계좌번호
		sprintf(kField.Receive.vaccount_balance,  "%-10.10s", 	&recvbuf[126]); 		// 가상계좌 잔액
		sprintf(kField.Receive.vaccount_bank_name,  "%-20.20s", 	&recvbuf[136]); 		// 가상계좌 은행명
		sprintf(kField.Receive.vaccount_bank_user,  "%-50.50s", 	&recvbuf[156]); 		// 가상계좌 계좌주명
        aDebugMsg("  가맹점 가상계좌 잔액조회 성공! ");
        aDebugMsg("가상계좌번호=%s", kField.Receive.vaccount);
        aDebugMsg("가상계좌잔액=%s", kField.Receive.vaccount_balance);
        aDebugMsg("가상계좌은행=%s", kField.Receive.vaccount_bank_name);
        aDebugMsg("가상계좌유저=%s", kField.Receive.vaccount_bank_user);
        //trans_print(servkind);
        disconnect();
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS914;
		qdata.qs914_resp.response_code = 0;
		del_space(qdata.qs914_resp.t_field.message, kField.Receive.resp_message, 50);
		del_space(tempbuf, kField.Receive.vaccount_balance, 10);
		qdata.qs914_resp.t_field.balance = (int)atoi(tempbuf);
		del_space(qdata.qs914_resp.t_field.bank_name, kField.Receive.vaccount_bank_name, 20);
		del_space(qdata.qs914_resp.t_field.account_user, kField.Receive.vaccount_bank_user, 50);
		send_msgq(&qdata, 1);
#endif
    }
    else // 오류메시지 처리
    {
        aDebugMsg("가맹점 가상계좌 잔액조회 실패:%s", kField.Receive.resp_message);
        disconnect();
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS914;
		if(kField.respcode[0] == 'E' || kField.respcode[0] == 'e')
			kField.respcode[0] = '8';
		qdata.qs914_resp.response_code = (int)a2l(kField.respcode, 4);
		memcpy(qdata.qs914_resp.t_field.message, kField.Receive.resp_message, 50);
		send_msgq(&qdata, 1);
#endif
        return(2);
    }

	return 0;
}
//------------------------------------------------------------------------------------------------------
int remote_reboot(void)
{
	int servkind = KIOSK_SERVKIND;
	byte ret;
	t_qdata qdata;

    get_dtm(kField.MakeTimeStamp);
	aDebugMsg("remote_reboot() started...");
	make_packet(servkind);
    ret = host_msganal(servkind, 1);
    disconnect();
    if(ret) return ret;
#if 1
	qdata.command_type = qETHERNET;
	qdata.command_code = qETHERNET_CONNECT;
	qdata.qlan_resp.response_code = 0; // 통신정상완료
	qdata.qlan_resp.t_field.progress = 100;
	send_msgq(&qdata, 1);
#endif

    if (memcmp(kField.respcode, "0000", 4)==0)
    {
		sprintf(kField.Receive.reboot_enable,  "%-1.1s", 	&recvbuf[76]); 			// 리부팅 여부, 'Y', 'N'
		sprintf(kField.Receive.reboot_start_time,  "%-3.3s", 	&recvbuf[77]);	 		// 리부팅 시간, "005", "010", "900", 분단위
        aDebugMsg("  원격리부팅 성공! ");
        aDebugMsg("리부팅 여부=%s", kField.Receive.reboot_enable);
        aDebugMsg("리부팅 시간=%s", kField.Receive.reboot_start_time);
        //trans_print(servkind);
        disconnect();
#if 0
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS900;
		qdata.qs900_resp.response_code = 0;
		memcpy(qdata.qs900_resp.t_field.shop_name, kField.Receive.shop_name, sizeof(kField.Receive.shop_name));
		qdata.qs900_resp.t_field.vaccount_enable = (int)a2l(kField.Receive.vaccount_use, 1);
		send_msgq(&qdata, 1);
#endif
    }
    else // 오류메시지 처리
    {
        aDebugMsg("원격리부팅 실패:%s", kField.Receive.resp_message);
        disconnect();
        return(2);
    }
	
	return 0;
}
//------------------------------------------------------------------------------------------------------
int patch_check(void)
{
	int servkind = KIOSK_SERVKIND;
	byte ret;
	t_qdata qdata;
	int aa;
	byte tms_time[16+1]={0};
	ulong long_tms_time=0, long_tmp1=0, long_tmp2=0;
	byte ttt_buf[16+1]={0};
	int random_num1=0, random_num2=0, r_hour=0, r_hour_rest=0, r_min=0, r_sec=0;
	
	sprintf(kField.Send.kiosk_version, "%-10.10s", PROGRAM_VERSION);

    get_dtm(kField.MakeTimeStamp);
	aDebugMsg("patch_check() started...");
	make_packet(servkind);
    ret = host_msganal(servkind, 0);
    disconnect();
    if(ret) return ret;
#if 0
	qdata.command_type = qETHERNET;
	qdata.command_code = qETHERNET_CONNECT;
	qdata.qlan_resp.response_code = 0; // 통신정상완료
	qdata.qlan_resp.t_field.progress = 100;
	send_msgq(&qdata, 1);
#endif

    if (memcmp(kField.respcode, "0000", 4)==0)
    {
		sprintf(kField.Receive.patch_enable,  "%-1.1s", 	&recvbuf[76]); 			// 패치 여부, 'Y', 'N'
		sprintf(kField.Receive.patch_start_time,  "%-2.2s", 	&recvbuf[77]);	 		// 패치 시간, "00"~"24"
        aDebugMsg("  패치확인 성공! ");
        aDebugMsg("패치 여부=%s", kField.Receive.patch_enable);
        aDebugMsg("패치 시간=%s", kField.Receive.patch_start_time);
        //trans_print(servkind);
        disconnect();
#if 0
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS921;
		qdata.qs921_resp.response_code = 0;
		qdata.qs921_resp.t_field.patch_enable = kField.Receive.patch_enable[0];
		qdata.qs921_resp.t_field.patch_time = atoi(kField.Receive.patch_start_time);
		send_msgq(&qdata, 1);
#endif
		aa = atoi(kField.Receive.patch_start_time);
		if(kField.Receive.patch_enable[0] == 'Y' && aa == 0)
		{
			KIOSK_SERVKIND = PATCH_REQUEST;
			ret = patch_request();
			if(ret) return ret;
		}
		else if(kField.Receive.patch_enable[0] == 'Y' && aa != 0)
		{
#if 0
			get_dtm(tms_time);
			long_tms_time = a2l(&tms_time[6], 2);
			long_tms_time += aa;
			long_tms_time = long_tms_time % 24;
			sprintf(tempbuf, "%02d", (int)long_tms_time);
			memcpy(&tms_time[6], tempbuf, 2);
#endif
			get_dtm(tms_time);
			long_tms_time = a2l(&tms_time[6], 2);
			
//			if(aa==24)
//				aa = 0;
			long_tms_time = aa % 24;

			srand((unsigned)time(NULL));
			random_num1 = (rand() % 3540) + 1; // 1~3540까지의 난수 발생
			r_min = random_num1 / 60; // 0~59분까지 나올 수 있음
			r_sec = random_num1 % 60; // 0~59초까지 나올 수 있음

			aDebugMsg("update time:random_num=%d, r_min=%d, r_sec=%d", random_num1, r_min, r_sec);
			sprintf(&tms_time[6], "%02d%02d%02d", (int)long_tms_time, r_min, r_sec);
			sprintf(Region04.ymdsms, "%12.12s", tms_time);
			aDebugMsg("1:patch update time=========>>%s", Region04.ymdsms);

//			sprintf(tempbuf, "%02d", (int)long_tms_time);
//			memset(&tms_time[6], '0', 6);
//			memcpy(&tms_time[6], tempbuf, 2);
//			sprintf(Region04.ymdsms, "%12.12s", tms_time);
			
			tms_update_flag = 1;
			aDebugMsg("TMS update time Changed!! => %s", Region04.ymdsms);
		}
		else if(kField.Receive.patch_enable[0] == 'N')
		{
			get_dtm(tms_time);
			sprintf(Region04.check_ymdsms1, "%12.12s", tms_time);
			long_tms_time = a2l(&tms_time[6], 2);
			long_tms_time++; // 1시간 후마다 tms_check 날림

			srand((unsigned)time(NULL));
			random_num1 = (rand() % 3540) + 1; // 1~3540까지의 난수 발생
			long_tmp1 = a2l(&tms_time[8], 2); // 현재 시간의 분을 구한다.
			long_tmp2 = a2l(&tms_time[10], 2); // 현재 시간의 초를 구한다.
			long_tmp1 = long_tmp1*60+long_tmp2; // 초단위 환산
			random_num2 = random_num1 + long_tmp1; // 초단위 최종 시간
			r_hour = random_num2 / 3600; // 0~1시간 까지 나올 수 있음
			r_hour_rest = random_num2 % 3600;
			r_min = r_hour_rest / 60; // 0~59분까지 나올 수 있음
			r_sec = r_hour_rest % 60; // 0~59초까지 나올 수 있음

			long_tms_time += r_hour;
			long_tms_time = long_tms_time % 24;
			aDebugMsg("random_num=%d, curr_sec=%d, total_sec=%d, r_hour=%d, r_hour_rest=%d, r_min=%d, r_sec=%d long_tms_time=%d", random_num1, long_tmp1, random_num2, r_hour, r_hour_rest, r_min, r_sec, long_tms_time);
			sprintf(&Region04.check_ymdsms1[6], "%02d%02d%02d", (int)long_tms_time, r_min, r_sec);
			aDebugMsg("1:patch check time=========>>%s", &Region04.check_ymdsms1[6]);
			
//			sprintf(ttt_buf, "%02d", (int)long_tms_time);
//			memcpy(&Region04.check_ymdsms1[6], ttt_buf, 2);
//		    fi_region_4K_write((byte *)&Region04, FL_REGION04_AREA, sizeof(FL_REGION_04));
		}
	}
    else // 오류메시지 처리
    {
        aDebugMsg("패치확인 실패:%s", kField.Receive.resp_message);
        disconnect();
		get_dtm(tms_time);
		sprintf(Region04.check_ymdsms1, "%12.12s", tms_time);
		long_tms_time = a2l(&tms_time[6], 2);
		long_tms_time++; // 1시간 후마다 tms_check 날림

		srand((unsigned)time(NULL));
		random_num1 = (rand() % 3540) + 1; // 1~3540까지의 난수 발생
		long_tmp1 = a2l(&tms_time[8], 2); // 현재 시간의 분을 구한다.
		long_tmp2 = a2l(&tms_time[10], 2); // 현재 시간의 초를 구한다.
		long_tmp1 = long_tmp1*60+long_tmp2; // 초단위 환산
		random_num2 = random_num1 + long_tmp1; // 초단위 최종 시간
		r_hour = random_num2 / 3600; // 0~1시간 까지 나올 수 있음
		r_hour_rest = random_num2 % 3600;
		r_min = r_hour_rest / 60; // 0~59분까지 나올 수 있음
		r_sec = r_hour_rest % 60; // 0~59초까지 나올 수 있음

		long_tms_time += r_hour;
		long_tms_time = long_tms_time % 24;
		aDebugMsg("random_num=%d, curr_sec=%d, total_sec=%d, r_hour=%d, r_hour_rest=%d, r_min=%d, r_sec=%d long_tms_time=%d", random_num1, long_tmp1, random_num2, r_hour, r_hour_rest, r_min, r_sec, long_tms_time);
		sprintf(&Region04.check_ymdsms1[6], "%02d%02d%02d", (int)long_tms_time, r_min, r_sec);
		aDebugMsg("2:patch check time=========>>%s", &Region04.check_ymdsms1[6]);

//		sprintf(ttt_buf, "%02d", (int)long_tms_time);
//		memcpy(&Region04.check_ymdsms1[6], ttt_buf, 2);
        return(2);
    }
	
	return 0;
}
//------------------------------------------------------------------------------------------------------
int patch_request(void)
{
	int servkind = KIOSK_SERVKIND;
	byte ret;
	t_qdata qdata;

    get_dtm(kField.MakeTimeStamp);
	aDebugMsg("patch_request() started...");
	make_packet(servkind);
    ret = host_msganal(servkind, 0);
    disconnect();
    if(ret) return ret;
#if 0
	qdata.command_type = qETHERNET;
	qdata.command_code = qETHERNET_CONNECT;
	qdata.qlan_resp.response_code = 0; // 통신정상완료
	qdata.qlan_resp.t_field.progress = 100;
	send_msgq(&qdata, 1);
#endif

    if (memcmp(kField.respcode, "0000", 4)==0)
    {
		sprintf(kField.Receive.patch_url,  "%-128.128s", 	&recvbuf[76]); // 패치 URL
        aDebugMsg("  패치요청 성공! ");
        aDebugMsg("패치 URL=%s", kField.Receive.patch_url);
        //trans_print(servkind);
        disconnect();
#if 0
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS922;
		qdata.qs922_resp.response_code = 0;
		del_space(qdata.qs922_resp.t_field.patch_url, kField.Receive.patch_url, 128);
		send_msgq(&qdata, 1);
#endif
		ret = patch_start();
		if(ret) return ret;
    }
    else // 오류메시지 처리
    {
        aDebugMsg("패치확인 실패:%s", kField.Receive.resp_message);
        disconnect();
        return(2);
    }
	return 0;
}
//------------------------------------------------------------------------------------------------------
int is_word_cmp(byte *buf, byte *cmp_word)
{
	int i;
	byte nua_temp[16];
	
	for(i=0; i<strlen(buf); i++)
	{
        memcpy(nua_temp, nua_temp+1, sizeof(nua_temp)-1);
        nua_temp[sizeof(nua_temp)-1] = buf[i];
		if(!memcmp(&nua_temp[sizeof(nua_temp)-strlen(cmp_word)], cmp_word, strlen(cmp_word)))
		{
			return 1; // cmp ok
		}
	}
	return 0; // fail
}
//------------------------------------------------------------------------------------------------------
int down_ok(byte *buf)
{
	FILE *read_fp;
	char buffer[4096 + 1];
	int chars_read, i;
	memset(buffer, 0, sizeof(buffer));

	read_fp = popen(buf, "r");
	if (read_fp != NULL)
	{
		chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
		printf("printf >>>>>> [%s]\n", buffer);
		//aDebugMsg(">>>>>>>>>> [%s]", buffer);
		if (chars_read > 0)
		{
			if(is_word_cmp(buffer, "100%"))
			{
				pclose(read_fp);
				return 1;
			}
			else
			{
				pclose(read_fp);
				return 0;
			}
		}
	}
}
//------------------------------------------------------------------------------------------------------
int patch_complete(void);
//------------------------------------------------------------------------------------------------------
int patch_start(void)
{
	int i,j, cnt=1000, cnt1=900, ret;
	byte tbuf[1024+1], tbuf2[1024+1], tbuf3[1024+1], tbuf4[1024+1];;
	FILE	*fp;

//	sprintf(kField.Receive.patch_url, "http://patch.teencash.co.kr/patchinfomini.aspx");
	
	system("rm -rf /tmp/update");
	system("mkdir /tmp/update");
	
	memset(tbuf, 0, sizeof(tbuf));
	del_space(tbuf, kField.Receive.patch_url, 128);
	sprintf(tempbuf, "wget -P /tmp/update %s", tbuf);
	system(tempbuf);
	for(i=0; i<strlen(tbuf); i++)
	{
		if(tbuf[strlen(tbuf)-i-1]== '/') break;
	}
	if(i==0) return 1; // 파일이름이 없는 경우
	memset(tbuf2, 0, sizeof(tbuf2));
	for(j=0; j<i; j++)
	{
		tbuf2[j] = tbuf[strlen(tbuf)-i+j];
	}
	memset(tbuf4, 0, sizeof(tbuf2));
	for(j=0; j<(strlen(tbuf)-i-1); j++)
	{
		tbuf4[j] = tbuf[j]; // tbuf4 = http://patch.teencash.co.kr
	}
	sprintf(tbuf3, "%s", tbuf2); // tbuf3 = patchinfo.aspx
	aDebugMsg("1.tbuf3=%s, tbuf4=%s", tbuf3, tbuf4);

	sprintf(tempbuf, "cat /tmp/update/%s | wc -l > /tmp/update/down_file_num", tbuf3); // 정보파일에서 다운로드할 총 파일개수 구하기 (라인총수)
	system(tempbuf);
	file2mem(tbuf2, "/tmp/update/down_file_num", 10);
	cnt= atoi(tbuf2); // 다운로드할 총 파일개수
	if(cnt==0) return 1;
	aDebugMsg("2.cnt=%d", cnt);
	for(i=1; i<=cnt; i++)
	{
		sprintf(tempbuf, "sed -n \"%d,%dp\" /tmp/update/%s > /tmp/update/tmp_file", i,i,tbuf3); // sed -n "1,1p" filename <<--- 원하는 행의 문자열을 구해서 tmp_file로 쓴다.
		system(tempbuf);
		file2mem(tbuf2, "/tmp/update/tmp_file", sizeof(tbuf2)-1); // tmp_file을 열어서 해당 파일을 다운로드 한다.
//		sprintf(tempbuf, "wget -P /tmp/update %s/%s",tbuf4, tbuf2);
		sprintf(tempbuf, "/app/tms.sh %s/%s",tbuf4, tbuf2);
		aDebugMsg(">>>>>>>>>>>> %s", tempbuf);
		
		system(tempbuf);
		file2mem(tempbuf, "/tmp/update/result", 2);
		if(tempbuf[0]=='0')
		{
			system("rm -rf /tmp/update/*");
			return 1;
		}
//		if(!down_ok(tempbuf))
//			return 1;
	}
	system("rm /tmp/update/result");
	aDebugMsg("3.i=%d", i);
	sprintf(tempbuf, "rm /tmp/update/%s", tbuf3);
	system(tempbuf);
	system("rm /tmp/update/down_file_num");
	system("rm /tmp/update/tmp_file");
	system("ls /tmp/update -l | grep ^- | wc -l > /tmp/update/tmp_file"); // 다운로드한 파일의 갯수를 tmp_file에 쓴다.
	file2mem(tbuf2, "/tmp/update/tmp_file", sizeof(tbuf2)-1);
	system("rm /tmp/update/tmp_file");
	cnt1 = atoi(tbuf2);
	if(cnt1==0) return 1;
	aDebugMsg("4.cnt=%d, cnt1=%d", cnt, cnt1-1);
	if(cnt == (cnt1-1))
	{
		system("tar vxf /tmp/update/kioskmini.bin -C /tmp/update");
		system("chmod 777 /tmp/update/kiosk_update.sh");
		system("/tmp/update/kiosk_update.sh");
		KIOSK_SERVKIND = PATCH_COMPLETE;
		patch_complete();
		system("reboot");
		return 0;
	}
	else
	{
		system("rm -rf /tmp/update/*");
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------------
#if 0
int patch_start(void)
{
	int i,j, cnt=1000, cnt1=900, ret;
	byte tbuf[1024+1], tbuf2[1024+1], tbuf3[1024+1], tbuf4[1024+1];;
	FILE	*fp;

	sprintf(kField.Receive.patch_url, "http://patch.teencash.co.kr/patchinfo.aspx");
	
	system("rm -rf /app/tms/*");
	memset(tbuf, 0, sizeof(tbuf));
	del_space(tbuf, kField.Receive.patch_url, 128);
	sprintf(tempbuf, "wget -P /app/tms %s", tbuf);
	system(tempbuf);
	for(i=0; i<strlen(tbuf); i++)
	{
		if(tbuf[strlen(tbuf)-i-1]== '/') break;
	}
	if(i==0) return 1; // 파일이름이 없는 경우
	memset(tbuf2, 0, sizeof(tbuf2));
	for(j=0; j<i; j++)
	{
		tbuf2[j] = tbuf[strlen(tbuf)-i+j];
	}
	memset(tbuf4, 0, sizeof(tbuf2));
	for(j=0; j<(strlen(tbuf)-i-1); j++)
	{
		tbuf4[j] = tbuf[j]; // tbuf4 = http://patch.teencash.co.kr
	}
	sprintf(tbuf3, "%s", tbuf2); // tbuf3 = patchinfo.aspx
	aDebugMsg("1.tbuf3=%s, tbuf4=%s", tbuf3, tbuf4);

	sprintf(tempbuf, "cat /app/tms/%s | wc -l > /app/tms/down_file_num", tbuf3); // 정보파일에서 다운로드할 총 파일개수 구하기 (라인총수)
	system(tempbuf);
	file2mem(tbuf2, "/app/tms/down_file_num", 10);
	cnt= atoi(tbuf2); // 다운로드할 총 파일개수
	if(cnt==0) return 1;
	aDebugMsg("2.cnt=%d", cnt);
	for(i=1; i<=cnt; i++)
	{
		sprintf(tempbuf, "sed -n \"%d,%dp\" /app/tms/%s > /app/tms/tmp_file", i,i,tbuf3); // sed -n "1,1p" filename <<--- 원하는 행의 문자열을 구해서 tmp_file로 쓴다.
		system(tempbuf);
		file2mem(tbuf2, "/app/tms/tmp_file", sizeof(tbuf2)-1); // tmp_file을 열어서 해당 파일을 다운로드 한다.
		sprintf(tempbuf, "wget -P /app/tms %s/%s",tbuf4, tbuf2);
		system(tempbuf);
	}
	aDebugMsg("3.i=%d", i);
	sprintf(tempbuf, "rm /app/tms/%s", tbuf3);
	system(tempbuf);
	system("rm /app/tms/down_file_num");
	system("rm /app/tms/tmp_file");
	system("ls /app/tms -l | grep ^- | wc -l > /app/tms/tmp_file"); // 다운로드한 파일의 갯수를 tmp_file에 쓴다.
	file2mem(tbuf2, "/app/tms/tmp_file", sizeof(tbuf2)-1);
	system("rm /app/tms/tmp_file");
	cnt1 = atoi(tbuf2);
	if(cnt1==0) return 1;
	aDebugMsg("4.cnt=%d, cnt1=%d", cnt, cnt1);
	if(cnt == cnt1)
	{
		system("cp -rf /app/tms/* /app/");
		patch_complete();
		system("reboot");
	}
	else
	{
//		system("rm -rf /app/tms/*");
		return 1;
	}
	return 0;
}
#endif
//------------------------------------------------------------------------------------------------------
int patch_complete(void)
{
	int servkind = KIOSK_SERVKIND;
	byte ret;
	t_qdata qdata;

    get_dtm(kField.MakeTimeStamp);
	aDebugMsg("patch_complete() started...");
	make_packet(servkind);
    ret = host_msganal(servkind, 0);
    disconnect();
    if(ret) return ret;
#if 0
	qdata.command_type = qETHERNET;
	qdata.command_code = qETHERNET_CONNECT;
	qdata.qlan_resp.response_code = 0; // 통신정상완료
	qdata.qlan_resp.t_field.progress = 100;
	send_msgq(&qdata, 1);
#endif

    if (memcmp(kField.respcode, "0000", 4)==0)
    {
		sprintf(kField.Receive.kiosk_version, "%-10.10s", &recvbuf[76]); // 패치후 키오스크 버전
        aDebugMsg("  패치완료 성공! ");
        aDebugMsg("패치후 키오스크 버전=%s", kField.Receive.kiosk_version);
        //trans_print(servkind);
        disconnect();
#if 0
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS923;
		qdata.qs923_resp.response_code = 0;
		send_msgq(&qdata, 1);
#endif
    }
    else // 오류메시지 처리
    {
        aDebugMsg("패치완료 실패:%s", kField.Receive.resp_message);
        disconnect();
        return(2);
    }
	
	return 0;
}
//------------------------------------------------------------------------------------------------------
#if 1
int change_check(void) // 상품권 교환검사, S941
{
	int servkind = KIOSK_SERVKIND;
	byte ret;
	t_qdata qdata;
	byte tbuf[20], tbuf2[20];
	int issue_cnt=0;

    get_dtm(kField.MakeTimeStamp);
	memset(tbuf, 0, sizeof(tbuf));
	memcpy(tbuf, kField.MakeTimeStamp, 12);

	sprintf(kField.Send.cp_code, "%04d", msgq_buf.qs941_ask.t_field.cp_code);
	sprintf(kField.Send.pin_date, "%-8.8s", msgq_buf.qs941_ask.t_field.issue_date);
	sprintf(kField.Send.pin_num, "%-24.24s", "");
	sprintf(kField.Send.pin_manage_num, "%-20.20s", msgq_buf.qs941_ask.t_field.pin_manage_num);
	sprintf(kField.Send.pin_issue_amount, "%-10.10s", "");

	aDebugMsg("change_check() started...");
	make_packet(servkind);
    ret = host_msganal(servkind, 1);
    disconnect();
    if(ret) return ret;
#if 1
	qdata.command_type = qETHERNET;
	qdata.command_code = qETHERNET_CONNECT;
	qdata.qlan_resp.response_code = 0; // 통신정상완료
	qdata.qlan_resp.t_field.progress = 100;
	send_msgq(&qdata, 1);
#endif

//	kField.respcode[0] = 'E';
		
    if (memcmp(kField.respcode, "0000", 4)==0)
    {
		sprintf(Region01.Issue_Info.temp_group_pay_num,  "%-50.50s", kField.Receive.resp_message);
		sprintf(Region01.Issue_Info.temp_cp_code,  "%4.4s", kField.Send.cp_code);
		sprintf(Region01.Issue_Info.temp_pin_num,  "%-24.24s", &recvbuf[126]); // 핀번호(인증키), 핀번호
		sprintf(Region01.Issue_Info.temp_pin_manage_num,  "%-20.20s", &recvbuf[150]); // 핀 관리번호
		sprintf(Region01.Issue_Info.temp_payed_amount,  "%-10.10s", &recvbuf[170]); // 핀 액면가

		sprintf(kField.Receive.fix_group_pay_num,  "%-50.50s", kField.Receive.resp_message, 50);
		sprintf(kField.Receive.pin_num[0], "%-24.24s", &recvbuf[126]); // 핀번호(인증키), 핀번호
		sprintf(kField.Receive.pin_manage_num[0], "%-20.20s", &recvbuf[150]); // 핀 관리번호
		sprintf(kField.Receive.pin_issue_amount[0], "%-10.10s", &recvbuf[170]); // 핀 액면가

		if(recvbuf[180] == 'Y') //교환가능 여부
			kField.Receive.change_enable = 'Y';
		else
			kField.Receive.change_enable = 'N';
		
		memcpy(&kField2, &kField, sizeof(KIOSK_ID_FIELD));
		
        aDebugMsg("  상품권 교환검사 요청 성공! 확정그룹과금번호=%s", kField.Receive.fix_group_pay_num);
        aDebugMsg("  핀번호=%s, 핀관리번호=%s, 핀액면가=%s, 교환가능 여부=%c", kField.Receive.pin_num[0], kField.Receive.pin_manage_num[0], kField.Receive.pin_issue_amount[0], kField.Receive.change_enable);
//		trans_capture(servkind);
//      trans_print(servkind);
        disconnect();
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS941;
		qdata.qs941_resp.response_code = 0;
		del_space(qdata.qs941_resp.t_field.message, kField.Receive.resp_message, 50);
		memcpy(qdata.qs941_resp.t_field.pin_num, kField.Receive.pin_num[0], 24);
		memcpy(qdata.qs941_resp.t_field.pin_manage_num, kField.Receive.pin_manage_num[0], 20);
		memcpy(qdata.qs941_resp.t_field.pin_amount, kField.Receive.pin_issue_amount[0], 10);
		qdata.qs941_resp.t_field.change_enable = kField.Receive.change_enable;
		send_msgq(&qdata, 1);
#endif
    }
    else // 오류메시지 처리
    {
        aDebugMsg("발권 요청 실패:%s", kField.Receive.resp_message);
        disconnect();
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS941;
		if(kField.respcode[0] == 'E' || kField.respcode[0] == 'e')
			kField.respcode[0] = '8';
		qdata.qs941_resp.response_code = (int)a2l(kField.respcode, 4);
		aDebugMsg(">> response_code = %d", qdata.qs941_resp.response_code);
		memcpy(qdata.qs941_resp.t_field.message, kField.Receive.resp_message, 50);
		send_msgq(&qdata, 1);
#endif
        return(2);
    }

    return(0);
}
int change_app(void) // 상품권 교환요청, S942
{
	int servkind = KIOSK_SERVKIND;
	byte ret;
	t_qdata qdata;
	byte tbuf[20], tbuf2[20];
	int issue_cnt=0;

    get_dtm(kField.MakeTimeStamp);
	memset(tbuf, 0, sizeof(tbuf));
	memcpy(tbuf, kField.MakeTimeStamp, 12);

	sprintf(kField.Send.group_pay_num, "%-50.50s", Region01.Issue_Info.temp_group_pay_num); 	// 취소 그룹과금번호
	sprintf(kField.Send.pin_num, "%-24.24s", Region01.Issue_Info.temp_pin_num); 				// 취소핀번호
	sprintf(kField.Send.pin_manage_num, "%-20.20s", Region01.Issue_Info.temp_pin_manage_num); 	// 취소핀관리번호

	sprintf(kField.Send.cp_code, "%04d", msgq_buf.qs942_ask.t_field.cp_code); 					// 신규 발권 CP코드
	sprintf(kField.Send.issue_count, "%02d", msgq_buf.qs942_ask.t_field.cp_count); 				// 신규 발권 매수
	sprintf(kField.Send.issue_amount, "%-10d", msgq_buf.qs942_ask.t_field.cp_amount); 			// 신규 발권 금액
	sprintf(kField.Send.issue_type, "%c", (byte)msgq_buf.qs942_ask.t_field.issue_type); 		// 신규 발권 방식
	sprintf(kField.Send.client_phone, "%-16.16s", msgq_buf.qs942_ask.t_field.client_phone); 	// SMS 발권시 고객 전화번호

	aDebugMsg("change_app() started...");
	make_packet(servkind);
    ret = host_msganal(servkind, 1);
    disconnect();
    if(ret) return ret;
#if 1
	qdata.command_type = qETHERNET;
	qdata.command_code = qETHERNET_CONNECT;
	qdata.qlan_resp.response_code = 0; // 통신정상완료
	qdata.qlan_resp.t_field.progress = 100;
	send_msgq(&qdata, 1);
#endif

//	kField.respcode[0] = 'E';
		
    if (memcmp(kField.respcode, "0000", 4)==0)
    {
		sprintf(kField.Receive.group_pay_num,  "%-50.50s", kField.Receive.resp_message); // 신규 발권된 그룹과금번호

		sprintf(kField.Receive.fix_group_pay_num,  "%-50.50s", &recvbuf[126]); // 취소 그룹과금번호
		sprintf(kField.Receive.cancel_success[0],  "%-1.1s", &recvbuf[176]); // 취소 성공 여부, 'Y':취소완료, 'N':취소실패
		sprintf(kField.Receive.cancel_fail_code[0],  "%-4.4s", &recvbuf[177]); // 취소 실패 코드
		memcpy(kField.Receive.issue_count, &recvbuf[181], 2); // 발권 매수
		{
			int i=0;
			issue_cnt = (int)a2l(kField.Receive.issue_count, 2);
			for(i=0; i<issue_cnt; i++)
			{
				sprintf(kField.Receive.pin_num[i], "%-24.24s", &recvbuf[183+(i*84)]); // 핀번호(인증키), 핀번호
				sprintf(kField.Receive.pin_manage_num[i], "%-20.20s", &recvbuf[207+(i*84)]); // 핀 관리번호
				sprintf(kField.Receive.pin_issue_amount[i], "%-10.10s", &recvbuf[227+(i*84)]); // 핀 액면가
				sprintf(kField.Receive.issue_date[i], "%-10.10s", &recvbuf[237+(i*84)]); // 발행일, 해피머니 외에는 빈공간
				sprintf(kField.Receive.pin_manage_num2[i], "%-20.20s", &recvbuf[257+(i*84)]); // 핀 관리번호2, 핀 관리번호와 동일 (해피머니만 다름)
			}
		}
		sprintf(kField.Receive.event_message, "%-1000.1000s", &recvbuf[212+((issue_cnt-1)*84)]); // 이벤트 메시지
		sprintf(kField.Receive.event_message_pos, "%1.1s", &recvbuf[1212+((issue_cnt-1)*84)]); // 이벤트 메시지 위치, 상:1, 하:2

        disconnect();
		if(kField.Receive.cancel_success[0][0] == 'Y')
		{
	        aDebugMsg("  상품권 교환요청 성공! 확정그룹과금번호=%s, 발권매수=%s", kField.Receive.group_pay_num, kField.Receive.issue_count);
	        aDebugMsg("  이벤트 메시지=%s, 이벤트 메시지 위치=%s", kField.Receive.event_message, kField.Receive.event_message_pos);
//			trans_capture(servkind);
			trans_print(servkind);
		}
		else
		{
			memset(&qdata, 0, sizeof(t_qdata));
			qdata.command_type = qLOGIC;
			qdata.command_code  = qS942;
			qdata.qs942_resp.response_code = (int)a2l(kField.Receive.cancel_fail_code[0], 4);
			aDebugMsg(">> response_code = %d", qdata.qs942_resp.response_code);
			send_msgq(&qdata, 1);
			return(2);
		}
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS942;
		qdata.qs942_resp.response_code = 0;
		send_msgq(&qdata, 1);
#endif
    }
    else // 오류메시지 처리
    {
        aDebugMsg("상품권 교환요청 실패:%s", kField.Receive.resp_message);
        disconnect();
#if 1
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qLOGIC;
		qdata.command_code  = qS942;
		if(kField.respcode[0] == 'E' || kField.respcode[0] == 'e')
			kField.respcode[0] = '8';
		qdata.qs942_resp.response_code = (int)a2l(kField.respcode, 4);
		aDebugMsg(">> response_code = %d", qdata.qs942_resp.response_code);
		send_msgq(&qdata, 1);
#endif
        return(2);
    }

    return(0);
}
#endif
//------------------------------------------------------------------------------------------------------
void logic_process(void)
{
	switch(msgq_buf.command_code)
	{
		case qS900: // 설치인증
			KIOSK_SERVKIND = REG_DOWNLOAD;
			reg_download();
			break;
		case qS901: // 상품별 판매가능 여부 체크
			KIOSK_SERVKIND = SALE_ENABLE_CHECK;
			sale_enable_check();
			break;
		case qS902: // 상품 재고 체크
			KIOSK_SERVKIND = STOCK_CHECK;
			stock_check(0);
			break;
		case qS903: // 발권 요청
			KIOSK_SERVKIND = ISSUE_APP;
			issue_app(0);
			break;
		case qS905: // 발권/단권 취소
			KIOSK_SERVKIND = ISSUE_CAN;
			issue_can();
			break;
		case qS906: // 운영오류상황 기록
			KIOSK_SERVKIND = OPERATION_CHECK;
			operation_check();
			break;
		case qS907: // 정산마감 금액 확인
			KIOSK_SERVKIND = ADJUST_CHECK;
			adjust_check();
			break;
		case qS908: // 정산마감 처리
			KIOSK_SERVKIND = ADJUST_APP;
			adjust_app();
			break;
		case qS909: // 관리자 로그인 인증
			KIOSK_SERVKIND = ADMIN_LOGIN_AUTH;
			admin_login_auth();
			break;
		case qS910: // 부분납 후, 강제종료(임시결재번호 취소)
			KIOSK_SERVKIND = TEMP_CANCEL;
			temp_cancel();
			break;
		case qS911: // 키오스크 ON/OFF
			KIOSK_SERVKIND = KIOSK_ONOFF;
			kiosk_onoff();
			break;
		case qS912: // H/W 오류상황 기록
			KIOSK_SERVKIND = HW_ERROR_CHECK;
			hw_error_check();
			break;
		case qS913: // 서버응답전문 재전송
			KIOSK_SERVKIND = RESPONSE_RESEND;
			response_resend();
			break;
		case qS914: // 가맹점 가상계좌 잔액조회
			KIOSK_SERVKIND = VACCOUNT_BALANCE;
			vaccount_balance();
			break;
		case qS921: // 패치확인
			KIOSK_SERVKIND = PATCH_CHECK;
			patch_check();
			break;
		case qS922: // 패치요청
			KIOSK_SERVKIND = PATCH_REQUEST;
			patch_request();
			break;
		case qS923: // 패치완료
			KIOSK_SERVKIND = PATCH_COMPLETE;
			patch_complete();
			break;
	}
}
//------------------------------------------------------------------------------------------------------
void billcoin_accept(void)
{
	int amount, wtime;
#ifdef SELF_DEBUG
	byte chs[256];

	printf("\n지폐인식기 목표금액? ");
	scanf("%s", chs);
	amount = atoi(chs);
	Region01.Issue_Info.bill_target_amount = amount;
	printf("\n지폐인식기 동작시간? ");
	scanf("%s", chs);
	wtime = atoi(chs);
#else
	amount = msgq_buf.qbillcoin_ask.t_field.target_amount;
	Region01.Issue_Info.bill_target_amount = amount;
	wtime = msgq_buf.qbillcoin_ask.t_field.wait_time;
#endif
	switch(bill_accept(amount, wtime))
	{
		case 0:		// 금액이 목표금액과 같은 경우
			aDebugMsg("BILL:금액이 목표금액과 같은 경우");
			break;
		case 2:		// 금액이 목표금액보다 많은 경우
			aDebugMsg("BILL:금액이 목표금액보다 많은 경우");
			break;
		case 3:		// 금액이 0원인 경우
			log_printf(1, "BILL:금액이 0원으로 타임오버");
			aDebugMsg("BILL:금액이 0원으로 타임오버");
			break;
		case -1:	// 금액이 목표금액보다 작은채로 타임오버
			log_printf(1, "BILL:금액이 목표금액보다 작은채로 타임오버");
			aDebugMsg("BILL:금액이 목표금액보다 작은채로 타임오버");
			break;
		case -10:	// 지폐인식기 오류이면서 금액이 0원인 경우
			log_printf(1, "BILL:지폐인식기 오류이면서 금액이 0원인 경우");
			aDebugMsg("BILL:지폐인식기 오류이면서 금액이 0원인 경우");
			break;
		case -20:  // 지폐인식기 오류이면서 금액이 목표금액보다 적은 경우
			log_printf(1, "BILL:지폐인식기 오류이면서 금액이 목표금액보다 적은 경우");
			aDebugMsg("BILL:지폐인식기 오류이면서 금액이 목표금액보다 적은 경우");
			break;
		case -30:  // 잘못된 금액 입력 (1000원 미만 금액 입력)
			aDebugMsg("BILL:1000원 미만 금액 입력");
			break;
		case CANCEL:  // 사용자취소
			log_printf(1, "BILL:사용자취소");
			aDebugMsg("BILL:사용자취소");
			break;
	}
}
//------------------------------------------------------------------------------------------------------
int billcoin_accept2(void)
{
	int amount, wtime, ret;
	amount = msgq_buf.qtest_billaccept_test_ask.t_field.target_amount;
	wtime = msgq_buf.qtest_billaccept_test_ask.t_field.wait_time;

	ret = bill_accept2(amount, wtime);
	switch(ret)
	{
		case 0:		// 금액이 목표금액과 같은 경우
			aDebugMsg("BILL:금액이 목표금액과 같은 경우");
			break;
		case 2:		// 금액이 목표금액보다 많은 경우
			aDebugMsg("BILL:금액이 목표금액보다 많은 경우");
			break;
		case 3:		// 금액이 0원인 경우
			log_printf(1, "BILL:금액이 0원으로 타임오버");
			aDebugMsg("BILL:금액이 0원으로 타임오버");
			break;
		case -1:	// 금액이 목표금액보다 작은채로 타임오버
			log_printf(1, "BILL:금액이 목표금액보다 작은채로 타임오버");
			aDebugMsg("BILL:금액이 목표금액보다 작은채로 타임오버");
			break;
		case -10:	// 지폐인식기 오류이면서 금액이 0원인 경우
			log_printf(1, "BILL:지폐인식기 오류이면서 금액이 0원인 경우");
			aDebugMsg("BILL:지폐인식기 오류이면서 금액이 0원인 경우");
			break;
		case -20:  // 지폐인식기 오류이면서 금액이 목표금액보다 적은 경우
			log_printf(1, "BILL:지폐인식기 오류이면서 금액이 목표금액보다 적은 경우");
			aDebugMsg("BILL:지폐인식기 오류이면서 금액이 목표금액보다 적은 경우");
			break;
		case -30:  // 잘못된 금액 입력 (1000원 미만 금액 입력)
			aDebugMsg("BILL:1000원 미만 금액 입력");
			break;
		case CANCEL:  // 사용자취소
			log_printf(1, "BILL:사용자취소");
			aDebugMsg("BILL:사용자취소");
			break;
	}
	return ret;
}
//------------------------------------------------------------------------------------------------------
void amount_process(void)
{
	switch(msgq_buf.command_code)
	{
		case qBILL_COIN_ACCEPT:
#if 0
			tms_check_flag = 1;
			msg_to_ui(qTMS, qTMS_CC, 0);
			{
				int msgqid;
				t_qdata qbuf;
				if(-1==(msgqid=msgget((key_t)RECVQ_ID, IPC_CREAT|0666)))
				{
					aDebugMsg("msgget()실패");
				}
				while(1)
				{
					if(-1==msgrcv(msgqid, &qbuf, sizeof(t_qdata)-sizeof(long), 1, 0))
					{
						perror("msgrcv()실패");
					}
					printf("[amount received msg] ---> command_type:0x%02X command_code:0x%02X\n", qbuf.command_type, qbuf.command_code);
					break;
				}
			}
			sleep(1);
			msg_to_ui(qTMS, qTMS_UPDATED, 0);
#endif
			billcoin_accept();
			break;
	}
}
//------------------------------------------------------------------------------------------------------
int print_speed_set(void)
{
	byte ret=0, ret2=0;
	t_qdata qdata;

	
#ifdef SELF_DEBUG
//	printf("\nSet Volume value? (70~127) ");
//	scanf("%s", tempbuf);
//	msgq_buf.set_volume_ask.t_field.vol = (byte)atoi(tempbuf);
#endif
	ret = msgq_buf.qtest_print_speed_ask.t_field.speed;
#ifdef PRT_SPEED_TEST
	aDebugMsg(">>>> print_speed1 set = %d, print_speed2 set =%d", ret, ret2);
	ret2 = msgq_buf.qtest_print_speed_ask.t_field.speed2;
	if(ret < 0)
	{
		set_step(80);
		//set_bright(0);
	}
	else if(ret > 100)
	{
		set_step(80);
		//set_bright(60);
	}
	else
	{
		set_step(ret);
		//set_bright(ret);
	}
	if(ret2 < 0)
	{
		set_speed(40);
		//set_bright(0);
	}
	else if(ret2 > 100)
	{
		set_speed(40);
		//set_bright(60);
	}
	else
	{
		set_speed(ret);
		//set_bright(ret);
	}
#else
	aDebugMsg(">>>> print_speed set = %d", ret);

	if(ret < 1)
	{
		set_bright(0);
	}
	else if(ret > 60)
	{
		set_bright(60);
	}
	else
	{
		set_bright(ret);
	}

	Region01.Terminfo.PrtInfo.print_speed = ret;
#endif
	
	memset(&qdata, 0, sizeof(t_qdata));
	qdata.command_type = qTEST;
	qdata.command_code  = qTEST_PRINT_SPEED;
	qdata.response_code = 0;
	send_msgq(&qdata, 1);
    aDebugMsg("  인쇄속도 설정 성공! ");

	return 0;
}
//------------------------------------------------------------------------------------------------------
int variable_response(void)
{
	byte ret;
	t_qdata qdata;
	u8 dns1[18+1], dns2[18+1];
	byte set_val[]={70, 90, 100, 110, 120, 122, 124, 127};
	byte set_index=0;

	aDebugMsg("variable_response() started...");

	qdata.command_type = qETC;
	qdata.command_code = qVARIABLE;
	qdata.variable_resp.response_code = 0;
    if(memcmp(Region01.Merchant.registration, "[Register OK :", 14)!=0)
		qdata.variable_resp.t_field.registration = 0;
	else
		qdata.variable_resp.t_field.registration = 1;
	qdata.variable_resp.t_field.vaccount_enable = Region01.Merchant.vaccount_enable;
	qdata.variable_resp.t_field.background_sound = Region01.Terminfo.SoundInfo.background_sound;
	qdata.variable_resp.t_field.touch_sound = Region01.Terminfo.SoundInfo.touch_sound;
	
	qdata.variable_resp.t_field.volume = Region01.Terminfo.SoundInfo.volume+1;
	if(Region01.Comm.Ethernet.ipmethod == '2') // 수동
		qdata.variable_resp.t_field.dynamic_ip_use = 0;
	else
		qdata.variable_resp.t_field.dynamic_ip_use = 1;
#if 0
	memset(qdata.variable_resp.t_field.kiosk_ip, 0, sizeof(qdata.variable_resp.t_field.kiosk_ip));
	memset(qdata.variable_resp.t_field.kiosk_sm, 0, sizeof(qdata.variable_resp.t_field.kiosk_sm));
	memset(qdata.variable_resp.t_field.kiosk_gw, 0, sizeof(qdata.variable_resp.t_field.kiosk_gw));
	sprintf(qdata.variable_resp.t_field.kiosk_ip, "%s", Region01.Comm.Ethernet.staticIP);
	aDebugMsg("msg IP=%s", qdata.variable_resp.t_field.kiosk_ip);
	sprintf(qdata.variable_resp.t_field.kiosk_sm, "%s", Region01.Comm.Ethernet.staticSM);
	aDebugMsg("msg SM=%s", qdata.variable_resp.t_field.kiosk_sm);
	sprintf(qdata.variable_resp.t_field.kiosk_gw, "%s", Region01.Comm.Ethernet.staticGW);
	aDebugMsg("msg GW=%s", qdata.variable_resp.t_field.kiosk_gw);
#else
	memset(qdata.variable_resp.t_field.kiosk_ip, 0, sizeof(qdata.variable_resp.t_field.kiosk_ip));
	memset(qdata.variable_resp.t_field.kiosk_sm, 0, sizeof(qdata.variable_resp.t_field.kiosk_sm));
	memset(qdata.variable_resp.t_field.kiosk_gw, 0, sizeof(qdata.variable_resp.t_field.kiosk_gw));
	if(ifconfig_read(qdata.variable_resp.t_field.kiosk_ip, qdata.variable_resp.t_field.kiosk_sm, qdata.variable_resp.t_field.kiosk_gw, dns1, dns2)==0)
		aDebugMsg("IP정보 Ok!");
	else
		aDebugMsg("IP정보를확인하지 못하였습니다.");
#endif
	sprintf(qdata.variable_resp.t_field.server_ip, "%s", Region01.Comm.Ethernet.address);
	sprintf(qdata.variable_resp.t_field.server_port, "%s", Region01.Comm.Ethernet.portno);
	sprintf(qdata.variable_resp.t_field.kiosk_version, "%s", PROGRAM_VERSION);	
    get_dtm(kField.MakeTimeStamp);
	sprintf(qdata.variable_resp.t_field.kiosk_yymmddhhmmss, "%-12.12s", kField.MakeTimeStamp);
	file2mem(tempbuf, "/media/mmcblk0p1/product-info", 30);
	sprintf(qdata.variable_resp.t_field.kiosk_id, "%-10.10s", tempbuf);
	sprintf(qdata.variable_resp.t_field.kiosk_mac, "%17.17s", &tempbuf[10]);
	sprintf(qdata.variable_resp.t_field.shop_id, "%-10.10s", Region01.Merchant.shop_serial);
	qdata.variable_resp.t_field.print_speed = Region01.Terminfo.PrtInfo.print_speed;
	send_msgq(&qdata, 1);
    aDebugMsg("  변수 응답 성공! ");
	HexDump((byte *)&qdata,256);
	
	return 0;	
}
//------------------------------------------------------------------------------------------------------
int test_issue_response(void)
{
	int servkind = KIOSK_SERVKIND;
	t_qdata qdata;
	
	KIOSK_SERVKIND = STOCK_CHECK;
	if(stock_check(1) != 0) return 1;
	KIOSK_SERVKIND = ISSUE_APP;
	if(issue_app(1) != 0) return 2;

	memset(&qdata, 0, sizeof(t_qdata));
	qdata.command_type = qETC;
	qdata.command_code  = qTEST_ISSUE;
	qdata.response_code = 0;
	send_msgq(&qdata, 1);
    aDebugMsg("  테스트발권 성공! ");

	return 0;
}
//------------------------------------------------------------------------------------------------------
int volume_set(void)
{
	byte ret=0;
	t_qdata qdata;

#ifdef SELF_DEBUG
	printf("\nSet Volume value? (70~127) ");
	scanf("%s", tempbuf);
	msgq_buf.set_volume_ask.t_field.vol = (byte)atoi(tempbuf);
#endif
	ret = msgq_buf.set_volume_ask.t_field.vol;

	if(ret < 1)
	{
		system("amixer set Speaker 100");
		ret = 2;
	}
	else if(ret > 8)
	{
		system("amixer set Speaker 100");
		ret = 2;
	}
	else
	{
		sprintf(tempbuf, "amixer set Speaker %d", set_sound_table[ret-1]);
		system(tempbuf);
	}
	Region01.Terminfo.SoundInfo.volume = ret-1;
	
	memset(&qdata, 0, sizeof(t_qdata));
	qdata.command_type = qETC;
	qdata.command_code  = qVOLUME;
	qdata.response_code = 0;
	send_msgq(&qdata, 1);
    aDebugMsg("  볼륨 설정 성공! ");

	return 0;
}
//------------------------------------------------------------------------------------------------------
int tsound_set(void)
{
	byte ret=0;
	t_qdata qdata;

#ifdef SELF_DEBUG
	printf("\nSet Touch Sound play? (yes:1, no:0) ");
	scanf("%s", tempbuf);
	msgq_buf.set_tsound_ask.t_field.tsound = (byte)atoi(tempbuf);
#endif
	ret = msgq_buf.set_tsound_ask.t_field.tsound;
	aDebugMsg(">>>>>>>>>> tsound ret=%d", ret);
	if(ret == 1)
		Region01.Terminfo.SoundInfo.touch_sound = 1;
	else
		Region01.Terminfo.SoundInfo.touch_sound = 0;

	memset(&qdata, 0, sizeof(t_qdata));
	qdata.command_type = qETC;
	qdata.command_code  = qTOUCH_SOUND;
	qdata.response_code = 0;
	send_msgq(&qdata, 1);
    aDebugMsg("  터치음 설정 성공! ");

	return 0;
}
//------------------------------------------------------------------------------------------------------
int bsound_set(void)
{
	byte ret=0;
	t_qdata qdata;

#ifdef SELF_DEBUG
	printf("\nSet Background Sound play? (yes:1, no:0) ");
	scanf("%s", tempbuf);
	msgq_buf.set_bsound_ask.t_field.bsound = (byte)atoi(tempbuf);
#endif
	ret = msgq_buf.set_bsound_ask.t_field.bsound;
	aDebugMsg(">>>>>>>>>> bsound ret=%d", ret);
	if(ret == 1)
		Region01.Terminfo.SoundInfo.background_sound = 1;
	else
		Region01.Terminfo.SoundInfo.background_sound = 0;

	memset(&qdata, 0, sizeof(t_qdata));
	qdata.command_type = qETC;
	qdata.command_code  = qBACK_SOUND;
	qdata.response_code = 0;
	send_msgq(&qdata, 1);
    aDebugMsg("  배경음 설정 성공! ");

	return 0;
}
//------------------------------------------------------------------------------------------------------
int lantype_select(void)
{
	byte ret=0, ret2=0;
	t_qdata qdata;

#ifdef SELF_DEBUG
	printf("\nSet lan type? (Dynamic:1, Static:0) ");
	scanf("%s", tempbuf);
	msgq_buf.set_lantype_ask.t_field.lantype = tempbuf[0];
#endif
	ret = msgq_buf.set_lantype_ask.t_field.lantype;

	if(ret == 1)
	{
		ret2 = Region01.Comm.Ethernet.ipmethod;
		Region01.Comm.Ethernet.ipmethod = '1';
		
		if(init_ethernet()==FALSE)
		{
			memset(&qdata, 0, sizeof(t_qdata));
			qdata.command_type = qETC;
			qdata.command_code  = qLAN_SELECT;
			qdata.response_code = 1; // 네트워크 설정실패
			send_msgq(&qdata, 1);
		    aDebugMsg("  랜타입 설정 실패! ");
		    aDebugMsg("네트워크설정실패");
		    aDebugMsg(" 재시도바랍니다 ");
			Region01.Comm.Ethernet.ipmethod = ret2;
			return 1;
		}
		Region01.Comm.Ethernet.ipmethod = '1';
	}
	else
		Region01.Comm.Ethernet.ipmethod = '2';

	memset(&qdata, 0, sizeof(t_qdata));
	qdata.command_type = qETC;
	qdata.command_code  = qLAN_SELECT;
	qdata.response_code = 0;
	send_msgq(&qdata, 1);
    aDebugMsg("  랜타입 설정 성공! ");

	return 0;
}
//------------------------------------------------------------------------------------------------------
int lan_static_ip_set(void)
{
	byte ret=0;
	t_qdata qdata;
	byte ip[16+1], sm[16+1], gw[16+1];
	byte ip2[16+1], sm2[16+1], gw2[16+1];
	
#ifdef SELF_DEBUG
	printf("\nstatic ip? (112.217.109.93) ");
	scanf("%s", msgq_buf.set_lanip_ask.t_field.kiosk_ip);
	printf("\nstatic sm? (255.255.255.248) ");
	scanf("%s", msgq_buf.set_lanip_ask.t_field.kiosk_sm);
	printf("\nstatic gw? (112.217.109.89) ");
	scanf("%s", msgq_buf.set_lanip_ask.t_field.kiosk_gw);
#endif
	sprintf(ip, "%-15.15s", msgq_buf.set_lanip_ask.t_field.kiosk_ip);
	del_space(tempbuf, ip, 15);
	sprintf(ip, "%s", tempbuf);
	
	sprintf(sm, "%-15.15s", msgq_buf.set_lanip_ask.t_field.kiosk_sm);
	del_space(tempbuf, sm, 15);
	sprintf(sm, "%s", tempbuf);
	
	sprintf(gw, "%-15.15s", msgq_buf.set_lanip_ask.t_field.kiosk_gw);
	del_space(tempbuf, gw, 15);
	sprintf(gw, "%s", tempbuf);

	sprintf(ip2, "%s", Region01.Comm.Ethernet.staticIP);
	sprintf(sm2, "%s", Region01.Comm.Ethernet.staticSM);
	sprintf(gw2, "%s", Region01.Comm.Ethernet.staticGW);
	
	sprintf(Region01.Comm.Ethernet.staticIP, "%s", ip);
	sprintf(Region01.Comm.Ethernet.staticSM, "%s", sm);
	sprintf(Region01.Comm.Ethernet.staticGW, "%s", gw);
	
	ret = Region01.Comm.Ethernet.ipmethod;
	Region01.Comm.Ethernet.ipmethod = '2';
	if(init_ethernet()==FALSE)
	{
		sprintf(Region01.Comm.Ethernet.staticIP, "%s", ip2);
		sprintf(Region01.Comm.Ethernet.staticSM, "%s", sm2);
		sprintf(Region01.Comm.Ethernet.staticGW, "%s", gw2);
		memset(&qdata, 0, sizeof(t_qdata));
		qdata.command_type = qETC;
		qdata.command_code  = qLAN_IP_SET;
		qdata.response_code = 1;
		send_msgq(&qdata, 1);
	    aDebugMsg("  고정IP 설정 실패! ");
	    aDebugMsg("네트워크설정실패");
	    aDebugMsg(" 재시도바랍니다 ");
		Region01.Comm.Ethernet.ipmethod = ret;
		return 1;
	}

	Region01.Comm.Ethernet.ipmethod = '2';

	memset(&qdata, 0, sizeof(t_qdata));
	qdata.command_type = qETC;
	qdata.command_code  = qLAN_IP_SET;
	qdata.response_code = 0;
	send_msgq(&qdata, 1);
    aDebugMsg("  고정IP 설정 성공! ");

	return 0;
}
//------------------------------------------------------------------------------------------------------
int lan_server_ip_set(void)
{
	byte ret=0;
	t_qdata qdata;
	byte sip[16+1], sport[10+1];

#ifdef SELF_DEBUG
	printf("\nserver ip? (112.175.103.73, 112.175.103.105) ");
	scanf("%s", msgq_buf.set_serverip_ask.t_field.server_ip);
	printf("\nserver port? (11901) ");
	scanf("%s", msgq_buf.set_serverip_ask.t_field.server_port);
#endif
	sprintf(sip, "%-15.15s", msgq_buf.set_serverip_ask.t_field.server_ip);
	del_space(tempbuf, sip, 15);
	sprintf(sip, "%s", tempbuf);
	
	sprintf(sport, "%-10.10s", msgq_buf.set_serverip_ask.t_field.server_port);
	del_space(tempbuf, sport, 10);
	sprintf(sport, "%s", tempbuf);

	if(memcmp("112.175.103.73", sip, 14)==0 || memcmp("112.175.103.105", sip, 15)==0 || memcmp("112.175.103.86", sip, 14)==0)
	{
		sprintf(Region01.Comm.Ethernet.address, "%s", sip);
	}
	if(memcmp("11901", sport, 5)==0)
	{
		sprintf(Region01.Comm.Ethernet.portno, "%s", sport);
	}
	
	memset(&qdata, 0, sizeof(t_qdata));
	qdata.command_type = qETC;
	qdata.command_code  = qSERVER_IP_SET;
	qdata.response_code = 0;
	send_msgq(&qdata, 1);
    aDebugMsg("  서버IP 설정 성공! ");

	return 0;
}
//------------------------------------------------------------------------------------------------------
int kiosk_usb_update(void)
{
	FILE *fp;
	byte tt_buf[10];
	DIR *dir;
	t_qdata qdata;
	byte first = 0;
	int ret[10];
	byte usb_name[256];
	int i;

	aDebugMsg("KIOSK 프로그램이 있는 USB 메모리를 삽입하여주세요");

    api_set_apptimer(6, 10*100);
	while(1)
	{
		usleep(1);
		ret[0]=access("/sys/block/sda", F_OK);
		ret[1]=access("/sys/block/sdb", F_OK);
		ret[2]=access("/sys/block/sdc", F_OK);
		ret[3]=access("/sys/block/sdd", F_OK);
		//system("ls -td1 ./log/*.log | tail -n +11 | xargs rm -f"); // ls -td1 *.log : 파일목록을 시간내림차순으로 출력, | tail -n +11 : 출력되는 11번째 파일부터 출력, | xargs rm -f 출력되는 11번째 파일부터 rm명령으로 삭제
		//sprintf(tempbuf, "sed -n \"%d,%dp\" /tmp/update/%s > /tmp/update/tmp_file", i,i,tbuf3); // sed -n "1,1p" filename <<--- 원하는 행의 문자열을 구해서 tmp_file로 쓴다.
		if(ret[0]==0 || ret[1]==0 || ret[2]==0 || ret[3]==0)
		{
			system("ls /media | grep \"sd\" | sed -n \"1,1p\" > /tmp/uname_tmp");
			file2mem(tmbuf, "/tmp/uname_tmp", 10);
			system("rm /tmp/uname_tmp");
			sprintf(usb_name, "/media/%s/kioskmini.bin", tmbuf);
			printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
			printf("%s", usb_name);
			printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
			if((fp = fopen(usb_name, "r+")) == 0 )
			{
				if(first == 0)
				{
					memset(&qdata, 0, sizeof(t_qdata));
					qdata.command_type = qETC;
					qdata.command_code  = qUSB_UPDATE;
					qdata.qusb_resp.response_code = 2; // 업데이트 파일 없음
					qdata.qusb_resp.t_field.progress= 0;
					send_msgq(&qdata, 1);
					first = 1;
					aDebugMsg("Update file not found...");
					aDebugMsg("KIOSK 프로그램이 있는 USB 메모리를 삽입하여주세요");
				}
			}
			else
			{
				memset(&qdata, 0, sizeof(t_qdata));
				qdata.command_type = qETC;
				qdata.command_code  = qUSB_UPDATE;
				qdata.qusb_resp.response_code = 0;
				qdata.qusb_resp.t_field.progress= 10;
				send_msgq(&qdata, 1);
				system("mkdir /tmp/update");
				sprintf(tmbuf, "cp -rf %s /tmp/update", usb_name);
//				system("cp -rf /media/sda1/kioskmini.bin /tmp/update");
				system(tmbuf);
				memset(&qdata, 0, sizeof(t_qdata));
				qdata.command_type = qETC;
				qdata.command_code  = qUSB_UPDATE;
				qdata.qusb_resp.response_code = 0;
				qdata.qusb_resp.t_field.progress= 80;
				send_msgq(&qdata, 1);
				system("tar vxf /tmp/update/kioskmini.bin -C /tmp/update");
				system("chmod 777 /tmp/update/kiosk_update.sh");
				system("/tmp/update/kiosk_update.sh");
				system("sync");
				memset(&qdata, 0, sizeof(t_qdata));
				qdata.command_type = qETC;
				qdata.command_code  = qUSB_UPDATE;
				qdata.qusb_resp.response_code = 0;
				qdata.qusb_resp.t_field.progress= 100;
				send_msgq(&qdata, 1);
				system("reboot");
				return 0;
			}
		}
		if(api_chk_apptimer(6)==FALSE)
		{
			memset(&qdata, 0, sizeof(t_qdata));
			qdata.command_type = qETC;
			qdata.command_code  = qUSB_UPDATE;
			qdata.qusb_resp.response_code = 1; // 타임아웃
			qdata.qusb_resp.t_field.progress= 0;
			send_msgq(&qdata, 1);
			aDebugMsg("  타임 아웃! ");
			return 1;
		}
	}
}
//------------------------------------------------------------------------------------------------------
int app_memory_init(void)
{
	byte ret=0;
	t_qdata qdata;
	
	capture_delete(0);
	app_init(1);

	memset(&qdata, 0, sizeof(t_qdata));
	qdata.command_type = qETC;
	qdata.command_code  = qINIT_MEMORY;
	qdata.response_code = 0;
	send_msgq(&qdata, 1);
    aDebugMsg("  기기초기화 설정 성공! ");

	return 0;
}
//------------------------------------------------------------------------------------------------------
void etc_process(void)
{
	switch(msgq_buf.command_code)
	{
		case qREBOOT: // 원격 리부팅
			KIOSK_SERVKIND = REMOTE_REBOOT;
			remote_reboot();
			break;
		case qINIT_MEMORY: // 기기 초기화
			app_memory_init();
			break;
		case qVARIABLE: // 변수 요청
			variable_response();
			break;
		case qTEST_ISSUE: // 테스트 발권 요청
			test_issue_response();
			break;
		case qVOLUME: // 볼륨 설정 요청
			volume_set();
			break;
		case qTOUCH_SOUND: // 터치음 설정 요청
			tsound_set();
			break;
		case qBACK_SOUND:
			bsound_set();
			break;
		case qLAN_SELECT:
			lantype_select();
			break;
		case qLAN_IP_SET:
			lan_static_ip_set();
			break;
		case qSERVER_IP_SET:
			lan_server_ip_set();
			break;
		case qUSB_UPDATE:
			kiosk_usb_update();
			break;
	}
}
//------------------------------------------------------------------------------------------------------
int barcode_read(void)
{
	t_qdata qdata;
	byte tbuf[1024];
	int ret=0;

	memset(tbuf, 0, sizeof(tbuf));
	scanf("%s", tbuf);
	insert_char_at(tmbuf, &tbuf[10], 2, ':', 0);
	sprintf(tempbuf, "echo %10.10s%s > /media/mmcblk0p1/product-info", tbuf, tmbuf);
	system(tempbuf);
}
//------------------------------------------------------------------------------------------------------
extern void play_sound(byte * file);
extern int kiosk_usb_test(void);
extern int kmodem_test(void);
//------------------------------------------------------------------------------------------------------
int ta_test_process(void)
{
	byte ret=0;
	t_qdata qdata;
	byte biostemp[1024];
	
	api_set_apptimer(dtimer7, 10);
	api_set_apptimer(dtimer8, 20*100);
	aDebugMsg("TA TEST Started...");
	while(1)
	{
		usleep(1);
		if(msg_cancel)
		{
			aDebugMsg("TA TEST Stoped...");
			return CANCEL;
		}
		if(api_chk_apptimer(dtimer7)==FALSE)
		{
			if(ret > 10)
				ret = 0;
			printf("%d\n", ret++);
			api_set_apptimer(dtimer7, 10);
		}
		if(api_chk_apptimer(dtimer8)==FALSE)
		{
			issue_app_test_print();
			api_set_apptimer(dtimer8, 20*100);
		}
	}
	return 0;
}
//------------------------------------------------------------------------------------------------------
int ta_modem_process(void)
{
	byte ret=0;
	t_qdata qdata;
	byte biostemp[1024];
	if(first_modem_test_flag==0)
	{
		sio_set(PSTN_PORT, B9600);
	    ret=api_send_wait((byte *)"ATZ\r", (byte *)"OK", (byte *)"\0", 2);
		if(ret==MD_TIMEOUT)
			aDebugMsg("ATZ command timeout...");

		sprintf(biostemp, "ath0e0+gci=61\\T2\r"); // 20120405_PSTN_TJH
		api_send_wait(biostemp, (byte *)"OK", (byte *)"\0", 1);
		Delay(20);

		sprintf(biostemp, "at:u52,0000\r");
		api_send_wait(biostemp, (byte *)"OK", (byte *)"\0", 1);
		Delay(20);

		sprintf(biostemp, "at:u67,0006\r");
		api_send_wait(biostemp, (byte *)"OK", (byte *)"\0", 1);
		Delay(20);

		sprintf(biostemp, "at:u70,2700\r");
		api_send_wait(biostemp, (byte *)"OK", (byte *)"\0", 1);
		Delay(20);

		sprintf(biostemp, "at:u7a,0001\r");
		api_send_wait(biostemp, (byte *)"OK", (byte *)"\0", 1);
		Delay(20);

		sprintf(biostemp, "at:u46,0790\r");
		api_send_wait(biostemp, (byte *)"OK", (byte *)"\0", 1);
		Delay(20);

		sprintf(biostemp, "at:u84,0004\r");
		api_send_wait(biostemp, (byte *)"OK", (byte *)"\0", 1);
		Delay(20);

		sprintf(biostemp, "at:u35,1000\r");
		api_send_wait(biostemp, (byte *)"OK", (byte *)"\0", 1);
		Delay(20);

		sprintf(biostemp, "at&g3&h7\\n0\\v2h0%%v1\r");
		api_send_wait(biostemp, (byte *)"OK", (byte *)"\0", 1);
		Delay(20);
		first_modem_test_flag = 1;
	}
/*
	ret=api_send_wait((uchar *)"at:u52,5\r", (uchar *)"OK", (byte *)"\0", 2);   // reduce transmit power level -5dbm
	if(ret==MD_TIMEOUT)
		aDebugMsg("at:u52,5 command timeout...");
	Delay(30);
*/
	
	switch(msgq_buf.qtest_ta_modem_ask.t_field.command)
	{
		case 10: // off-hook
		    ret=api_send_wait((uchar *)"ath1\r", (uchar *)"OK", (byte *)"\0", 2);
			if(ret==MD_TIMEOUT)
			{
				aDebugMsg("off-hook command timeout...");
				return 10;
			}
			aDebugMsg("off-hook command OK...");
			break;
		case 20: // on-hook
		    ret=api_send_wait((uchar *)"ath0\r", (uchar *)"OK", (byte *)"\0", 2);
			if(ret==MD_TIMEOUT)
			{
				aDebugMsg("on-hook command timeout...");
				return 20;
			}
			aDebugMsg("on-hook command OK...");
			break;
		case 30: // data send
    		ret=api_send_wait((uchar *)"at&t4\r", (uchar *)"OK", (byte *)"\0", 1);
			qdata.command_type = qTEST;
			qdata.command_code  = qTEST_TA_MODEM;
			qdata.response_code = 0;
			send_msgq(&qdata, 1);
			aDebugMsg("data send command OK...");
			while(1)
			{
				usleep(1);
				if(msg_cancel)
				{
		    		ret=api_send_wait((uchar *)"+++", (uchar *)"OK", (byte *)"\0", 1);
					if(ret==MD_TIMEOUT)
					{
						aDebugMsg("+++ command timeout...");
					}
		    		Delay(30);
					ret=api_send_wait((uchar *)"ath0\r", (uchar *)"NO CARRIER", (byte *)"\0", 2);
					if(ret==MD_TIMEOUT)
					{
						aDebugMsg("ath0 command timeout...");
					}
		    		Delay(10);
					aDebugMsg("data send stop OK...");
					return CANCEL;
				}
			}
			break;
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			sprintf(tempbuf, "at*y1d%d\r", msgq_buf.qtest_ta_modem_ask.t_field.command);
	        ret=api_send_wait(tempbuf, (uchar *)"OK", (byte *)"\0", 1); // DTMF Sending..
			if(ret==MD_TIMEOUT)
			{
				aDebugMsg("DTMF Send command timeout...");
			}
			break;
		case '*':
#if 0
			ret=api_send_wait((uchar *)"atz\r", (uchar *)"OK", (byte *)"\0", 2);
			if(ret==MD_TIMEOUT)
			{
				aDebugMsg("atz command timeout...");
			}
			Delay(10);
		    ret=api_send_wait((uchar *)"atx0+gci=61\r", (uchar *)"OK", (byte *)"\0", 2); // blind dialling, korea
			if(ret==MD_TIMEOUT)
			{
				aDebugMsg("atx0+gci=61 command timeout...");
			}
			Delay(10);
			ret=api_send_wait((uchar *)"at:u46,0ac0\r", (uchar *)"OK", (byte *)"\0", 2); // reduce dtmf power level
			if(ret==MD_TIMEOUT)
			{
				aDebugMsg("at:u46,0ac0 command timeout...");
			}
		    Delay(10);
#endif
			break;
		case '#':
			ret=api_send_wait((uchar *)"atj0\r", (uchar *)"OK", (byte *)"\0", 2); // Test Stop.
			if(ret==MD_TIMEOUT)
			{
				aDebugMsg("atj0 command timeout...");
			}
	        Delay(10);
    		ret=api_send_wait((uchar *)"ath0\r", (uchar *)"OK", (byte *)"\0", 2); // Test Stop.
			if(ret==MD_TIMEOUT)
			{
				aDebugMsg("ath0 command timeout...");
			}
	        Delay(10);
			break;
		default:
			return 99;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------------
int usb_logout_process(void)
{
	FILE *fp;
	byte tt_buf[10];
	DIR *dir;
	int ret[10];
	byte ttt_buf[20];

	aDebugMsg("   USB 로그출력   ");
	aDebugMsg("  USB 메모리를  ");
	aDebugMsg(" 삽입하여주세요 ");

    api_set_apptimer(6, 3*100);
	while(1)
	{
		ret[0]=access("/sys/block/sda", F_OK);
		ret[1]=access("/sys/block/sdb", F_OK);
		ret[2]=access("/sys/block/sdc", F_OK);
		ret[3]=access("/sys/block/sdd", F_OK);
		if(ret[0]==0 || ret[1]==0 || ret[2]==0 || ret[3]==0)
		{
			aDebugMsg("access=[%d]", ret);
		    get_dtm(ttt_buf);
			sprintf(tempbuf, "mkdir /media/sda1/mini_%12.12s", ttt_buf);
			system(tempbuf);
			sprintf(tempbuf, "cp -rf /app/log /media/sda1/mini_%12.12s", ttt_buf);
			system(tempbuf);
			system("sync");
			return 0;
			
		}
		if(api_chk_apptimer(6)==FALSE)
		{
			printf("\nusb 로그출력 타임아웃..\n");
			return 1;
		}
	}
}
//------------------------------------------------------------------------------------------------------
void test_process(void)
{
	t_qdata qdata;
	int ret=0;
	
	switch(msgq_buf.command_code)
	{
		case qTEST_SOUND:
			play_sound("/app/kiosksound/TC_UI_Button.wav");
			memset(&qdata, 0, sizeof(t_qdata));
			qdata.command_type = qTEST;
			qdata.command_code  = qTEST_SOUND;
			qdata.response_code = 0;
			send_msgq(&qdata, 1);
			break;
		case qTEST_TIME_SET:
			del_space(tempbuf, msgq_buf.qtest_time_set_ask.t_field.yymmddhhmmss, 13);
			set_dtm(tempbuf); 
			qdata.command_type = qTEST;
			qdata.command_code  = qTEST_TIME_SET;
			qdata.response_code = 0;
			send_msgq(&qdata, 1);
			break;
		case qTEST_USB:
			ret = kiosk_usb_test();
			qdata.command_type = qTEST;
			qdata.command_code  = qTEST_USB;
			qdata.response_code = ret;
			send_msgq(&qdata, 1);
			break;
		case qTEST_MODEM:
			ret = kmodem_test();
			qdata.command_type = qTEST;
			qdata.command_code  = qTEST_MODEM;
			qdata.response_code = ret;
			send_msgq(&qdata, 1);
			break;
		case qTEST_BILLACCEPT:
			ret = billcoin_accept2();
//			qdata.command_type = qTEST;
//			qdata.command_code  = qTEST_BILLACCEPT;
//			qdata.response_code = ret;
//			send_msgq(&qdata, 1);
			break;
		case qTEST_LAN:
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
			
		    if (Eth_info.IP_state == 1) 
				ret = 0;
		    else
				ret = 1;
			qdata.command_type = qTEST;
			qdata.command_code  = qTEST_LAN;
			qdata.response_code = ret;
			send_msgq(&qdata, 1);
			break;
		case qTEST_PRINT:
			sleep(1);
			issue_app_test_print();
			//prt_test1(1);
			qdata.command_type = qTEST;
			qdata.command_code  = qTEST_PRINT;
			qdata.response_code = 0;
			send_msgq(&qdata, 1);
			break;
		case qTEST_BARCODE:
			barcode_read();
			qdata.command_type = qTEST;
			qdata.command_code  = qTEST_BARCODE;
			qdata.response_code = 0;
			send_msgq(&qdata, 1);
		case qTEST_TMS:
			KIOSK_SERVKIND = PATCH_REQUEST;
			ret = patch_request();
			qdata.command_type = qTEST;
			qdata.command_code  = qTEST_TMS;
			qdata.response_code = ret;
			send_msgq(&qdata, 1);
			break;
		case qTEST_TA_MODEM:
			ret = ta_modem_process();
			if(ret==CANCEL)
				return;
			qdata.command_type = qTEST;
			qdata.command_code  = qTEST_TA_MODEM;
			qdata.response_code = ret;
			send_msgq(&qdata, 1);
			break;
		case qTEST_PRINT_SPEED:
			ret=print_speed_set();
			break;
		case qTEST_TA_TEST:
			ret=ta_test_process();
			break;
		case qTEST_USB_LOGOUT:
			ret=usb_logout_process();
			qdata.command_type = qTEST;
			qdata.command_code  = qTEST_USB_LOGOUT;
			qdata.response_code = ret;
			send_msgq(&qdata, 1);
			break;
	}
}
//------------------------------------------------------------------------------------------------------
void tms_process(void)
{
	t_qdata qdata;
	int ret=0;
	byte tms_time[16+1]={0}, ttt_buf[16+1]={0};
	ulong long_tms_time=0;
	
	switch(msgq_buf.command_code)
	{
		case qTMS_FIRST:
			get_dtm(tms_time);
			sprintf(Region04.check_ymdsms1, "%12.12s", tms_time);
			msg_to_ui(qTMS, qTMS_CC, 0);
			
			long_tms_time = a2l(&tms_time[6], 2);
			long_tms_time++; // 1시간 후마다 tms_check 날림
			long_tms_time = long_tms_time % 24;
			sprintf(ttt_buf, "%02d", (int)long_tms_time);
			memcpy(&tms_time[6], ttt_buf, 2);
			sprintf(Region04.check_ymdsms1, "%12.12s", tms_time);
		    fi_region_4K_write((byte *)&Region04, FL_REGION04_AREA, sizeof(FL_REGION_04));
			break;
		case qTMS_CC:
			aDebugMsg("UI response_code=%d, tms_check_flag=%d, tms_update_flag=%d", msgq_buf.response_code, tms_check_flag, tms_update_flag);
			if(msgq_buf.response_code)
			{
				if(tms_check_flag==2)
				{
					tms_update_flag=0;
					get_dtm(tms_time);
					sprintf(Region04.check_ymdsms1, "%12.12s", tms_time);
					long_tms_time = a2l(&tms_time[6], 2);
					long_tms_time++; // 1시간 후마다 tms_check 날림
					long_tms_time = long_tms_time % 24;
					sprintf(ttt_buf, "%02d", (int)long_tms_time);
					memcpy(&tms_time[6], ttt_buf, 2);
					sprintf(Region04.check_ymdsms1, "%12.12s", tms_time);
				}
				tms_check_flag = 0;
				sleep(1);
				return;
			}
			else
			{
				if(tms_check_flag==1)
				{
					KIOSK_SERVKIND = PATCH_CHECK;
					ret = patch_check();
					msg_to_ui(qTMS, qTMS_UPDATED, ret);
				}
				else if(tms_check_flag==2)
				{
					KIOSK_SERVKIND = PATCH_REQUEST;
					ret = patch_request();
					msg_to_ui(qTMS, qTMS_UPDATED, ret);
				}
			}
			sleep(1);
			if(tms_check_flag==2)
			{
				tms_update_flag = 0;
				get_dtm(tms_time);
				sprintf(Region04.check_ymdsms1, "%12.12s", tms_time);
				long_tms_time = a2l(&tms_time[6], 2);
				long_tms_time++; // 1시간 후마다 tms_check 날림
				long_tms_time = long_tms_time % 24;
				sprintf(ttt_buf, "%02d", (int)long_tms_time);
				memcpy(&tms_time[6], ttt_buf, 2);
				sprintf(Region04.check_ymdsms1, "%12.12s", tms_time);
			}
			tms_check_flag = 0;
			break;
	}
}
//------------------------------------------------------------------------------------------------------
void msgq_process(void)
{
	now_idle = 0;
	switch(msgq_buf.command_type)
	{
		case qLCD:
			printf("msgq_process():qLCD...\n");
			break;
		case qPRINTER:
			printf("msgq_process():qPRINTER...\n");
			break;
		case qAMOUNT_INPUT:
			printf("msgq_process():qAMOUNT_INPUT...\n");
			amount_process();
			break;
		case qETHERNET:
			printf("msgq_process():qETHERNET...\n");
			break;
		case qMODEM:
			printf("msgq_process():qMODEM...\n");
			break;
		case qMSR:
			printf("msgq_process():qMSR...\n");
			break;
		case qIC:
			printf("msgq_process():qIC...\n");
			break;
		case qUSB:
			printf("msgq_process():qUSB...\n");
			break;
		case qLOGIC:
			printf("msgq_process():qLOGIC...\n");
			logic_process();
			break;
		case qETC:
			printf("msgq_process():qETC...\n");
			etc_process();
			break;
		case qTEST:
			printf("msgq_process():qTEST...\n");
			test_process();
			break;
		case qTMS:
			printf("msgq_process():qTMS...\n");
			tms_process();
			break;
		default:
			printf("msgq_process():default...\n");
			break;
	}
}
//------------------------------------------------------------------------------------------------------
int search_cp_code(int cp_code)
{
	int i;

	for(i=0; cp_code_list[i].int_cp_code != 0; i++)
	{
		if(cp_code == cp_code_list[i].int_cp_code)
		{
			return i;
		}
	}
	return 10000;
}
//------------------------------------------------------------------------------------------------------
int search_cp_code2(int cp_code)
{
	int i;

	for(i=0; cp_code_list2[i].int_cp_code != 0; i++)
	{
		if(cp_code == cp_code_list2[i].int_cp_code)
		{
			return i;
		}
	}
	return 10000;
}
//------------------------------------------------------------------------------------------------------
#if 0
#include <iconv.h>
int conversion_test(void)
{
    int ret;
    iconv_t it;
    char ksc_buf[1024] = "한글과 유니코드";
     
    // KSC(완성형) 코드를 UTF(유니코드)로 변환하면 원래 크기보다 커지므로 크게~
    char utf_buf[1024] = {0x00, };
    size_t in_size, out_size;
 
    sprintf(ksc_buf, "%s", "한글과 유니코드");
    memset(utf_buf, '\0', 1024);
 
    // 어떤 시스템에서는 char** 가 아니라 const char** 인 경우도 있음
    char *input_buf_ptr = ksc_buf;
    char *output_buf_ptr = utf_buf;
 
    in_size = strlen(ksc_buf);
    out_size = sizeof(utf_buf);
 
    it = iconv_open("UTF-8", "EUC-KR"); // EUC-KR을 UTF-8로
    ret = iconv(it, &input_buf_ptr, &in_size, &output_buf_ptr, &out_size);
     
    if (ret < 0)
    {
        printf("ret : %d, errno : %d\n", ret, errno);
        return(-1);
    }
    else
    {
        printf("[%s](%d) => [%s][(%d)\n",
            ksc_buf, in_size, utf_buf, out_size);
    }
     
    iconv_close(it);
 
    input_buf_ptr = utf_buf;
    output_buf_ptr = ksc_buf;
 
    in_size = strlen(utf_buf);
    out_size = sizeof(ksc_buf);
 
    it = iconv_open("EUC-KR", "UTF-8"); // UTF-8을 EUC-KR로
    ret = iconv(it, &input_buf_ptr, &in_size, &output_buf_ptr, &out_size);
 
    if (ret < 0)
    {
        printf("ret : %d, errno : %d\n", ret, errno);
        return(-1);
    }
    else
    {
        printf("[%s](%d) => [%s][(%d)\n",
            utf_buf, in_size, ksc_buf, out_size);
    }
     
    iconv_close(it);
	return 0;
}
#endif
//------------------------------------------------------------------------------------------------------


