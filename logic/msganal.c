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
    Tbuf[len++] = 'Y'; // ��������, 'N':��, 'Y':��ȣȭ
    memcpy(&Tbuf[len], "0000", 4); // ��������, �ŷ������ڵ������ ��ü����
    len += 4;

	// �ŷ������ڵ�
    switch(ServKind)
    {
        case REG_DOWNLOAD: // ��ġ���� (S900 <-> R900)
            memcpy(&Tbuf[len], "S900", 4); 
            len += 4;
            break;
    	case SALE_ENABLE_CHECK: // ��ǰ�� �Ǹ� ���ɿ��� üũ (S901 <-> R901)
            memcpy(&Tbuf[len], "S901", 4);
            len += 4;
            break;
        case STOCK_CHECK: // ��ǰ ��� üũ (S902 <-> R902)
            memcpy(&Tbuf[len], "S902", 4);
            len += 4;
            break;
    	case ISSUE_APP: // �߱� ��û (S903 <-> R903)
            memcpy(&Tbuf[len], "S903", 4);
            len += 4;
            break;
        case ISSUE_CAN: // �߱�/�ܰ� ��� (S905 <-> R905)
            memcpy(&Tbuf[len], "S905", 4);
            len += 4;
            break;
        case OPERATION_CHECK: // ����� ��Ȳ��� (S906 <-> R906)
            memcpy(&Tbuf[len], "S906", 4);
            len += 4;
            break;
        case ADJUST_CHECK: // ���긶�� �ݾ� Ȯ�� (S907 <-> R907)
            memcpy(&Tbuf[len], "S907", 4);
            len += 4;
            break;
        case ADJUST_APP: // ���긶�� ó�� (S908 <-> R908)
            memcpy(&Tbuf[len], "S908", 4);
            len += 4;
            break;
        case ADMIN_LOGIN_AUTH: // ������ �α��� ���� (S909 <-> R909)
            memcpy(&Tbuf[len], "S909", 4);
            len += 4;
            break;
        case TEMP_CANCEL: // �κг� ��, �������� (�ӽð����ȣ ���) (S910 <-> R910)
            memcpy(&Tbuf[len], "S910", 4);
            len += 4;
            break;
        case KIOSK_ONOFF: // Ű����ũ ��/���� (S911 <-> R911)
            memcpy(&Tbuf[len], "S911", 4);
            len += 4;
            break;
        case HW_ERROR_CHECK: // H/W ������Ȳ ��� (S912 <-> R912)
            memcpy(&Tbuf[len], "S912", 4);
            len += 4;
            break;
        case RESPONSE_RESEND: // ������������ ������ (S913 <-> R913)
            memcpy(&Tbuf[len], "S913", 4);
            len += 4;
            break;
        case VACCOUNT_BALANCE: // ������ ������� �ܾ���ȸ (S914 <-> R914)
            memcpy(&Tbuf[len], "S914", 4);
            len += 4;
            break;
        case REMOTE_REBOOT: // ���ݸ����� (S915 <-> R915)
            memcpy(&Tbuf[len], "S915", 4);
            len += 4;
            break;
        case PATCH_CHECK: // ��ġȮ�� (S921 <-> R921)
            memcpy(&Tbuf[len], "S921", 4);
            len += 4;
            break;
        case PATCH_REQUEST: // ��ġ��û (S922 <-> R922)
            memcpy(&Tbuf[len], "S922", 4);
            len += 4;
            break;
        case PATCH_COMPLETE: // ��ġ�Ϸ� (S923 <-> R923)
            memcpy(&Tbuf[len], "S923", 4);
            len += 4;
            break;
        case CHANGE_CHECK: // ��ǰ�� ��ȯ�˻� (S941 <-> R941)
            memcpy(&Tbuf[len], "S941", 4);
            len += 4;
            break;
        case CHANGE_APP: // ��ǰ�� ��ȯ��û (S942 <-> R942)
            memcpy(&Tbuf[len], "S942", 4);
            len += 4;
            break;
        default:
            aDebugMsg("service kind error [%d]\n", ServKind);
            break;
    }

    memcpy(&Tbuf[len], "0000", 4); // ��û�����ڵ�, "0000" ����, ������ ���� ����
    len += 4;
	
	sprintf(tmp_buf, "20%12.12s", kField.MakeTimeStamp); // ��û�Ͻ�, ���� ��û �ð� (YYYYMMDDHHMMSS)
	memset(&Tbuf[len], 0x20, 15);
	memcpy(&Tbuf[len], tmp_buf, 14);
    len += 15;

	sprintf(tmp_buf, "%-16.16s", "KIOSK");
//	memcpy(&Tbuf[len], "KIOSK", 5); // �͹̳� �̸�
	memcpy(&Tbuf[len], tmp_buf, 16);
	len += 16;
//	sprintf(tmp_buf, "%-10.10s", msgq_buf.qs900_ask.t_field.shop_serial_num); // ������ �ø����ȣ, I&Play���� �������� �߱�
#ifndef SELF_DEBUG
    if(memcmp(Region01.Merchant.registration, "[Register OK :", 14)!=0)
	{
		sprintf(tmp_buf, "%-10.10s", msgq_buf.qs900_ask.t_field.shop_serial_num); // ������ �ø����ȣ, I&Play���� �������� �߱�
	}
	else
		sprintf(tmp_buf, "%-10.10s", Region01.Merchant.shop_serial); // ������ �ø����ȣ, I&Play���� �������� �߱�
#else
		sprintf(tmp_buf, "%-10.10s", Region01.Merchant.shop_serial); // ������ �ø����ȣ, I&Play���� �������� �߱�
#endif
	memcpy(&Tbuf[len], tmp_buf, 10);
	len += 10;

	sprintf(tmp_buf, "%-20.20s", Region01.Merchant.cat_id); // KIOSK ���ο��� �����ϴ� �Ϸù�ȣ
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

	// ���
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
			if(kField.Send.cancel_type[0] == 'T') // �߱� ���
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
			else if(kField.Send.cancel_type[0] == 'P') // ������ �ܰ� ���
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
			else if(kField.Send.cancel_type[0] == 'C') // �� �ܰ� ���
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
    memcpy(&sendbuf[3], Ltemp, 4); // ��������, �ŷ������ڵ������ ��ü����
	
	if(sendbuf[2] == 'Y') // ���� ��ȣȭ ����
	{
		int msize=0;
		int ret=0;

		// ��ȣȭ�� ����Ʈ��=(x/16+2)*32, x:��ȣȭ�� ����Ʈ��
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
	printf("��û����\n");
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
		printf("\n��ȣȭ�� �������� : \n");
		//HexDump(recvbuf,rlen);
		
		// ��ȣȭ�� ����Ʈ��=(y-64)/2, y:��ȣȭ�� ����Ʈ��
		iLen = INP_DecData(&recvbuf[7], rlen-7, cyper_tmp_buf);
		aDebugMsg(">>>>>>> INP_DecData():ret=%d", iLen);
		memset(&recvbuf[7], 0, sizeof(recvbuf)-7);
//		iLen = (rlen-7-64)/2;
		iLen = strlen(cyper_tmp_buf);
		aDebugMsg(">>>>>>> strlen(cyper_tmp_buf):ret=%d", iLen);
		memcpy(&recvbuf[7], cyper_tmp_buf, iLen);
		rlen = iLen+7;
		sprintf(WorkBuf, "%04d", iLen);
		memcpy(&recvbuf[3], WorkBuf, 4); // ��ȣȭ�� ���̷� recvbuf�� �������̸� �ٲ۴�.
	}
	printf("\n�������� : \n");
	HexDump(recvbuf,rlen);

	memcpy(kField.Receive.resp_transcode, &recvbuf[7], 4);
   	memcpy(kField.respcode, &recvbuf[11], 4); // �������ڵ�
   	memcpy(kField.Receive.resp_terminal, &recvbuf[30], 16); // ���� �͹̳��̸�
   	memcpy(kField.Receive.resp_shop_serial, &recvbuf[46], 10); // ���� ������ �ø����ȣ
   	memcpy(kField.Receive.resp_kiosk_serial, &recvbuf[56], 20); // ���� Ű����ũ ���ΰ����Ϸù�ȣ

	if( ((memcmp(kField.Receive.resp_terminal, "I&Play", 6) != 0) && (memcmp(kField.Receive.resp_terminal, "I&PLAY", 6) != 0)) || (memcmp(kField.Receive.resp_kiosk_serial, Region01.Merchant.cat_id, 10) != 0))
	{
        aDebugMsg("  ������������1  ");
		if(type)
		{
			qdata.command_type = qETHERNET;
			qdata.command_code = qETHERNET_CONNECT;
			qdata.qlan_resp.response_code = 4; // ������������
			qdata.qlan_resp.t_field.progress = 0;
			send_msgq(&qdata, 1);
		}
		log_printf(0, "���:�������� ����1!");
		return 1;
	}
	
    if(memcmp(Region01.Merchant.registration, "[Register OK :", 14)==0)
   	{
		if(memcmp(kField.Receive.resp_shop_serial, Region01.Merchant.shop_serial, 10) != 0 )
		{
	        aDebugMsg("  ������������2  ");
			if(type)
			{
				qdata.command_type = qETHERNET;
				qdata.command_code = qETHERNET_CONNECT;
				qdata.qlan_resp.response_code = 4; // ������������
				qdata.qlan_resp.t_field.progress = 0;
				send_msgq(&qdata, 1);
			}
			log_printf(0, "���:�������� ����2!");
			return 1;
		}
    }
	ret = 0;
	if(memcmp(kField.Receive.resp_transcode, trans_code_list[ServKind].str_trans_rcode, 4) != 0)
	{
        aDebugMsg("  ������������3  ");
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
			memcpy(kField.Receive.resp_message, &recvbuf[76], 50); // �����ڵ尡 ������ ���, ���� �� ����
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
			qdata.qlan_resp.response_code = 4; // ������������
			qdata.qlan_resp.t_field.progress = 0;
			send_msgq(&qdata, 1);
		}
		log_printf(0, "���:�������� ����3!");
		return ret;
	}
	memset(TimeStamp, 0, sizeof(TimeStamp));
	memcpy(TimeStamp, &recvbuf[17], 12);
	memcpy(kField.ymdsms, &recvbuf[17], 12);
	set_dtm(TimeStamp);
	aDebugMsg("�ð�������!!!!!!!!! : %12.12s", TimeStamp);
    
    return ret;
}



