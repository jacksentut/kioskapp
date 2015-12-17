#include "include.h"
//-----------------------------------------------------------------------------------------------------------------------------
void bfrdata_clear(void)
{ 
    memset((byte *)&kBfrField, 0x00, sizeof(KIOSK_ID_FIELD));
    fi_area_write((byte *)&kBfrField, BFR_CAPTURE_AREA, sizeof(KIOSK_ID_FIELD));
}

void bfrdata_save(int ServKind)
{
    kField.servkind = ServKind;
    memcpy((byte *)&kBfrField, (byte *)&kField, sizeof(KIOSK_ID_FIELD));
    fi_area_write((byte *)&kBfrField, BFR_CAPTURE_AREA, sizeof(KIOSK_ID_FIELD));
}
#if 0
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
    byte pg_tran_date[14];

	byte cancel_type; // 1, 취소유형, 관리자발권취소T, 관리자단건취소P, 고객단건취소C
	byte pin_issue_date[8]; //8, 단건취소된 상품권의 발행일자
    byte pin_num[24]; // 24, 핀번호
    byte pin_manage_num[20]; // 20, 핀관리번호
    byte error_type_code; // 1, 오류구분코드, 입금대기시간초과5, 지폐걸림6, 기타H/W오류7
} CAP_DATA;
HNX720T_EXT CAP_DATA kcap_data;
#endif
#if 1
//-----------------------------------------------------------------------------------------------------------------------------
void trans_capture(int ServKind)
{
    FILE *fp;
    struct stat statbuf;
	ulong tmp_amount;
	int tmp_int;

    memset(tempbuf, 0x00, CAP_RECORD_SIZE+1); 
    tempbuf[0] = ServKind;                             // 1, SERVKIND
	
	memcpy(&tempbuf[1], kField.ymdsms, 12); // 12, 거래일시

	switch(ServKind)
	{
		case ISSUE_APP:
			memcpy(&tempbuf[13], kField.Send.cp_code, 4); // 4, cp_code
			tempbuf[17] = (byte)a2l(kField.Send.approval_code, 2); // 1, approval_type
			tempbuf[18] = (byte)a2l(kField.Send.issue_count, 2); // 1, 총발권매수
			del_space(inbuf, kField.Send.payed_tamount, 10);
			tmp_amount = a2l(inbuf, strlen(inbuf));
			memcpy(&tempbuf[19],  &tmp_amount, 4); // 4, 거래총금액
			tempbuf[23] = (byte)a2l(kField.Send.issue_type, 1); // 1, 발권방식 (발권1, sms2)
			memcpy(&tempbuf[24], kField.Send.client_phone, 16); // 16, sms일때 고객전화번호
			memcpy(&tempbuf[40], kField.Receive.fix_group_pay_num, 50); // 50, 확정그룹과금번호
			memcpy(&tempbuf[90], kField.Send.pg_tran_date, 14); // PG사 거래일시 (취소시 거래가 있는지 판단기준이 된다)
			break;
		case ISSUE_CAN:
			tempbuf[13] = kField.Send.cancel_type[0]; // 1, 취소유형
			memcpy(&tempbuf[14], kField.Send.cp_code, 4); // 4, cp_code
			if(	network_cancel_flag == 1)
			{
				tmp_int = 1;
				del_space(inbuf, kField.Send.can_amount, 10);
			}
			else
			{
				tmp_int = atoi(kField.Receive.cancel_ok_count);
				del_space(inbuf, kField.Receive.pin_issue_amount[0], 10);
			}
			tmp_amount = a2l(inbuf, strlen(inbuf));
			tmp_amount = tmp_int * tmp_amount;
//			aDebugMsg("Cancel amount=%d", tmp_amount);
			memcpy(&tempbuf[18],  &tmp_amount, 4); // 4, 거래총금액
			memcpy(&tempbuf[22], kField.Send.pin_date, 8); // 8, 단건취소된 상품권의 발행일자
			memset(&tempbuf[30], 0x20, 24); // 핀번호 저장 않하도록 스페이스로 바꿔서 저장함 20150312_khd
			//memcpy(&tempbuf[30], kField.Send.pin_num, 24); // 24, 핀번호 
			memcpy(&tempbuf[54], kField.Send.pin_manage_num, 20); // 20, 핀관리번호
			tempbuf[74] = (byte)a2l(kField.Send.error_code, 2); // 1, 오류코드
			break;
		case TEMP_CANCEL:
			memcpy(&tempbuf[13], kField.Send.temp_group_pay_num, 50); // 50, 임시그룹과금번호
			del_space(inbuf, kField.Send.temp_payed_amount, 10);
			tmp_amount = a2l(inbuf, strlen(inbuf));
			memcpy(&tempbuf[63], &tmp_amount, 4); // 4, 부분납금액
			tempbuf[67] = (byte)a2l(kField.Send.error_type_code, 2); // 1, 오류구분코드
			break;
	}
    
    if ((fp = fopen(FL_CDATACAP_AREA, "r+")) == NULL)
    {
        if ((fp = fopen(FL_CDATACAP_AREA, "w+")) == NULL)
        {
            printf("region write(%s) open error :", FL_CDATACAP_AREA);
            return;
        }
    }
    fstat(fileno(fp), &statbuf);
    if (statbuf.st_size >  Region02.c_qrear * CAP_RECORD_SIZE)
    {
        printf("capture limit overflow :");
        fclose(fp);
        if ((fp = fopen(FL_CDATACAP_AREA, "w+")) == NULL)
        {
            printf("region write(%s) open error :", FL_CDATACAP_AREA);
            return;
        }
        fstat(fileno(fp), &statbuf);
        Region02.c_qrear = Region02.c_qfront = (statbuf.st_size/CAP_RECORD_SIZE);
    }

    capture = c_cap1;
    if (Region02.c_qrear >= MAXCAP)
    {
        memcpy(capture+Region02.c_qfront*CAP_RECORD_SIZE, tempbuf, CAP_RECORD_SIZE);
        fseek(fp, Region02.c_qfront*CAP_RECORD_SIZE, SEEK_SET);
        fwrite(tempbuf, 1, CAP_RECORD_SIZE, fp);
        if (++Region02.c_qfront >= MAXCAP) Region02.c_qfront = 0;
    }
    else
    {
        memcpy(capture+Region02.c_qrear*CAP_RECORD_SIZE, tempbuf, CAP_RECORD_SIZE);
        fseek(fp, Region02.c_qrear*CAP_RECORD_SIZE, SEEK_SET);
        fwrite(tempbuf, 1, CAP_RECORD_SIZE, fp);
        Region02.c_qrear++;
    }
    fclose(fp);

    Region02.changed = SW_ON;
    memcpy(Region02.ymdsms, kField.ymdsms, 12);

    fi_region_4K_write((byte *)&Region02, FL_REGION02_AREA, sizeof(FL_REGION_02));
}
//-----------------------------------------------------------------------------------------------------------------------------
#endif

