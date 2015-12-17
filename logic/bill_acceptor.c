#include "include.h"

int bill_sendrcv(byte *send_cmd, byte send_data, byte *recv_buf);
//------------------------------------------------------------------------------------------------------
void send2bill(byte *cmd, byte data)
{
    byte pstr[256]={0}, Id=0, ret=0;
    int  cnt=0, step=0;

    pstr[cnt++] = '$'; // STX
    pstr[cnt++] =cmd[0];
    pstr[cnt++] =cmd[1];
    pstr[cnt++] =data;
    pstr[cnt++] = pstr[1]+pstr[2]+pstr[3];
	//printf("---> %c%c%c[0x%02X][0x%02X]\n", pstr[0], pstr[1], pstr[2], pstr[3], pstr[4]);
    DataTxl(BARCODE_PORT, pstr, (word)cnt);
}
//------------------------------------------------------------------------------------------------------
int bill_init(void)
{
	int i;
	byte tbuf[10];

	sio_set(BARCODE_PORT, B9600);
	bill_sendrcv("SC", 0x1F, tbuf);
	return 0;
}
//------------------------------------------------------------------------------------------------------
int bill_config(int amount)
{
	int i;
	byte tbuf[10];

	if(amount >= 50000)
		bill_sendrcv("SC", 0x1F, tbuf);
	else if(amount >= 10000)
		bill_sendrcv("SC", 0x17, tbuf);
	else if(amount >= 5000)
		bill_sendrcv("SC", 0x13, tbuf);
	else if(amount >= 1000)
		bill_sendrcv("SC", 0x11, tbuf);
	else
		bill_sendrcv("SC", 0x10, tbuf);
	return 0;
}
//------------------------------------------------------------------------------------------------------
// success:1, fail:0
// type:0�� send_cmd�� �빮���� �� �������� �ҹ��ڰ� ���� ���
// type:1, type=0 �̿��� ���
//------------------------------------------------------------------------------------------------------
int bill_sendrcv(byte *send_cmd, byte send_data, byte *recv_buf)
{
    byte tbuf[256]={0}, Id=0, ret=0;
    int  cnt=0, step=0;
	byte while_exit = 0;
	byte lrc=0;
	byte type=0;

//	bill_init();
	memset(tbuf, 0, sizeof(tbuf));
	memcpy(tbuf, send_cmd, 2);
	send2bill(tbuf, send_data);

	if(memcmp(&send_cmd[1], "GA", 2)==0 || memcmp(&send_cmd[1], "GB", 2)==0 || memcmp(&send_cmd[1], "GC", 2)==0 || memcmp(&send_cmd[1], "GE", 2)==0)
	{
		type = 0;
	}
	else
		type = 1;


	cnt=0;
    set_timer(&Id, 150);
    while(1)
    {
		usleep(1);
        if(chk_timer(Id)==FALSE)
            break;

		switch(step)
		{
			case 0:
				cnt = 0;
			    if(Com_Ready(BARCODE_PORT))
			    {
			        ret = Com_Getc(BARCODE_PORT);
					if(ret == '$')
					{
						tbuf[cnt++] = ret;
						step++;
					}
			    }
				break;
			case 1: // $ES Ŀ�ǵ��� ��� �ѹ� ������� ������ ���߱� ������ �߰��� �κ�
			    if(Com_Ready(BARCODE_PORT))
			    {
			        ret = Com_Getc(BARCODE_PORT);
					if(ret == 'E')
					{
						tbuf[cnt++] = ret;
						step = 2;
					}
					else
					{
						tbuf[cnt++] = ret;
						step = 3;
					}
			    }
				break;
			case 2: // $ES Ŀ�ǵ��� ��� �ѹ� ������� ������ ���߱� ������ �߰��� �κ�
			    if(Com_Ready(BARCODE_PORT))
			    {
			        ret = Com_Getc(BARCODE_PORT);
					tbuf[cnt++] = ret;
					if(cnt >= 5)
					{
						send2bill("es", tbuf[3]);
						step = 0;
					}
			    }
				break;
			case 3:
			    if(Com_Ready(BARCODE_PORT))
			    {
			        ret = Com_Getc(BARCODE_PORT);
					tbuf[cnt++] = ret;
			    }
				if(cnt== 5)
				{
					lrc = tbuf[1]+tbuf[2]+tbuf[3];
					if(lrc == tbuf[4])
						while_exit = 1;
					else
						while_exit = 2;
					step++;
				}
				break;
			case 4:
				if(!type)
				{
					send_cmd[1] = send_cmd[1]+('a'-'A');
					send_cmd[2] = send_cmd[2]+('a'-'A');
					if(memcmp(&tbuf[1], &send_cmd[1], 2))
						step = 0;
				}
				break;
		}
		if(while_exit) 
			break;
    }


    release_timer(Id);
	if(while_exit==1)
	{
		if(memcmp(&tbuf[1], "NG", 2))
		{
			memcpy(recv_buf, tbuf, 5);
//			Delay(20);
			return 1; // success
		}
	}
//	Delay(20);
	return 0; // fail
}
//------------------------------------------------------------------------------------------------------
// success:1, fail:0
//------------------------------------------------------------------------------------------------------
int bill_connect(void)
{
    byte tbuf[256]={0};
	byte Id;
	
	set_timer(&Id, 200);
	while(1)
	{
		usleep(1);
        if(chk_timer(Id)==FALSE)
            break;
		if(bill_sendrcv("Hi", '?', tbuf))
		{
			if(memcmp("$me!", tbuf, 4)==0)
			{
				release_timer(Id);
				return 1;
			}
			else
				continue;
		}
	}
	release_timer(Id);
	return 0;
}
//------------------------------------------------------------------------------------------------------
// success:0, time out:0x99, �ν��۾��� ������ ���� ��ȯ�۾�:0x03, ��ȯ������:0x07, Stack������:0x0A
//------------------------------------------------------------------------------------------------------
int bill_is_running(void)
{
    byte tbuf[256]={0};
	byte Id;
	
	set_timer(&Id, 200);
//	bill_init();
	while(1)
	{
		usleep(1);
        if(chk_timer(Id)==FALSE)
            break;
		if(bill_sendrcv("GA", '?', tbuf))
		{
			if(memcmp("$ga", tbuf, 3)==0)
			{
				switch(tbuf[3])
				{
					case 0x03:
					case 0x04:
					case 0x07:
					case 0x0C:
					case 0x0A:
						release_timer(Id);
						return (int)tbuf[3];
				}
				release_timer(Id);
				return 0;
			}
		}
	}
	release_timer(Id);
	return 99;
}
//------------------------------------------------------------------------------------------------------
// success:1, fail:0
//------------------------------------------------------------------------------------------------------
int bill_accept_onoff(byte onoff)
{
    byte tbuf[256]={0};
	byte set, Id;
	
//	bill_init();
	if(onoff==OFF)
		set = 0x0E;
	else
		set = 0x0D;

	if(set==0x0E)
		set_timer(&Id, 50);
	else	
		set_timer(&Id, 200);
	while(1)
	{
		usleep(1);
        if(chk_timer(Id)==FALSE)
            break;
		if(bill_sendrcv("SA", set, tbuf))
		{
			if(memcmp("$OKa", tbuf, 3)==0)
			{
				release_timer(Id);
				return 1;
			}
			else
				continue;
		}
	}
	release_timer(Id);
	return 0;
}
//------------------------------------------------------------------------------------------------------
// ��ǥ�ݾװ� ����ä�� ����:0, (��ǥ�ݾ׺��� ���� ���·� �������:1), ��ǥ�ݾ׺��� ūä�� ����:2, �Աݾ��� ����ä�� ����:3, 
// �Աݾ��� ����ä�� �ð�����:-1, �����νı� �ҷ��̸鼭 �ݾ��� ����ä�� ����: -10, �����νı� �ҷ��̸鼭 �ݾ��� ����ä�� ����:-20
// �߸��� �ݾ��Է����� ����: -30, (����ɸ� ���·� ����:99)
// tamount:������, ttime:�ʴ���
//------------------------------------------------------------------------------------------------------
int bill_accept(int tamount, int ttime)
{
	byte Id, Id2, ret;
	byte tbuf[20];
	int amount=0, gret=0;
	byte only_one=0;
	t_qdata qdata;
	int cnt=0;
	if(tamount < 1000)
	{
		gret = -30;
		goto exit_bill;
	}
	bill_init();
	bill_config(tamount);
	if(!bill_connect())
	{
		gret = -10;
		goto exit_bill;
	}
	
	Region01.Issue_Info.bill_target_amount = tamount;
	Region01.Issue_Info.bill_input_amount = amount;
    api_set_apptimer(dtimer2, ttime*100);
    api_set_apptimer(dtimer3, 40);
	if(bill_accept_onoff(ON))
	{
		while(1)
		{
			usleep(1);
			if(msg_cancel)
			{
				while(bill_is_running()) ;
				bill_accept_onoff(OFF);
				return CANCEL;
			}
			if(api_chk_apptimer(dtimer2)==FALSE)
			{
				aDebugMsg("Ÿ�Ӿƿ�!!\n");
				log_printf(0, "BILL:Ÿ�Ӿƿ�!");
				if(bill_sendrcv("GA", '?', tbuf))
				{
					if(memcmp(&tbuf[1], "ga", 2)==0)
					{
						if(tbuf[3]==0x03) // �ɷ��ִ� ���� �����ϴ� ���!
						{
							if(only_one==0)
							{
								send_msg_ui(qAMOUNT_INPUT, qBILL_COIN_ACCEPT, 99);
								only_one=1;
							}
						    api_set_apptimer(dtimer2, ttime*100);
							log_printf(0, "BILL:[%d]���� �ɷ�����!", cnt++);
						}
						else
						{
							if(amount)
							{
								
								gret = -1;
							}
							else
							{
								gret = 3;
							}
							goto exit_bill;
						}
					}
				}
				else
				{
					if(amount)
						gret = -20;
					else
						gret = -10;
					goto exit_bill;
				}
			}
			if(api_chk_apptimer(dtimer3)==FALSE)
			{
				if(bill_sendrcv("GA", '?', tbuf))
				{
					if(memcmp(&tbuf[1], "ga", 2)==0)
					{
						//printf("<--- %c%c%c[0x%02X][0x%02X]\n", tbuf[0], tbuf[1], tbuf[2], tbuf[3], tbuf[4]);
						
						if(tbuf[3]==0x03) // �ɷ��ִ� ���� �����ϴ� ���! 5�ʰ� �����ɸ��� �����Ǿ� ������ 99�� ������.
						{
							if(only_one==0)
							{
								api_set_apptimer(dtimer4, 500);
								only_one=1;
							}
							else
							{
								if(api_chk_apptimer(dtimer4)==FALSE)
								{
									send_msg_ui(qAMOUNT_INPUT, qBILL_COIN_ACCEPT, 99);
									only_one=0;
									log_printf(0, "BILL:�����ɸ�!!!");
									// �����޽��� ������, ��� üũ�Ѵ�.
									api_set_apptimer(dtimer2, ttime*100);
								}
							}
							//printf("ActiveStatus=0x%02X\n", tbuf[3]);
						}
						if(tbuf[3]==0x0B)
						{
							byte Id;
							set_timer(&Id, 200);
							while(1)
							{
								usleep(1);
								if(chk_timer(Id)==FALSE)
									break;
								if(bill_sendrcv("GB", '?', tbuf))
								{
									//printf("<--- %c%c%c[0x%02X][0x%02X]\n", tbuf[0], tbuf[1], tbuf[2], tbuf[3], tbuf[4]);
									if(memcmp(&tbuf[1], "gb", 2)==0)
									{
										if(tbuf[3]==0) 
										{
											aDebugMsg("�Լ��� ���� ����");
										}
										else if(tbuf[3]==1) 
										{
											aDebugMsg("1,000�� �Լ���");
											amount += 1000;
											Region01.Issue_Info.bill_input_amount = amount;
											send_msg_ui(qAMOUNT_INPUT, qBILL_COIN_ACCEPT, 1);
											bill_config(tamount-amount);
											log_printf(1, "BILL:���Աݾ�=%d, ��ǥ�ݾ�=%d, ����ݾ�=%d", 1000, tamount, amount);
										}
										else if(tbuf[3]==5) 
										{
											aDebugMsg("5,000�� �Լ���");
											amount += 5000;
											Region01.Issue_Info.bill_input_amount = amount;
											send_msg_ui(qAMOUNT_INPUT, qBILL_COIN_ACCEPT, 1);
											bill_config(tamount-amount);
											log_printf(1, "BILL:���Աݾ�=%d, ��ǥ�ݾ�=%d, ����ݾ�=%d", 5000, tamount, amount);
										}
										else if(tbuf[3]==10) 
										{
											aDebugMsg("10,000�� �Լ���");
											amount += 10000;
											Region01.Issue_Info.bill_input_amount = amount;
											send_msg_ui(qAMOUNT_INPUT, qBILL_COIN_ACCEPT, 1);
											bill_config(tamount-amount);
											log_printf(1, "BILL:���Աݾ�=%d, ��ǥ�ݾ�=%d, ����ݾ�=%d", 10000, tamount, amount);
										}
										else if(tbuf[3]==50) 
										{
											aDebugMsg("50,000�� �Լ���");
											amount += 50000;
											Region01.Issue_Info.bill_input_amount = amount;
											send_msg_ui(qAMOUNT_INPUT, qBILL_COIN_ACCEPT, 1);
											bill_config(tamount-amount);
											log_printf(1, "BILL:���Աݾ�=%d, ��ǥ�ݾ�=%d, ����ݾ�=%d", 50000, tamount, amount);
										}

										if(amount == tamount)
										{
											gret = 0;
											release_timer(Id);
											goto exit_bill;
										}
										else if(amount > tamount)
										{
											gret = 2;
											release_timer(Id);
											goto exit_bill;
										}
										else
										{
											while(bill_is_running()) ;
											if(!bill_accept_onoff(ON))
											{
												aDebugMsg("�Լ����� ��������!!\n");
												log_printf(0, "BILL:�Լ����� ��������");
												if(amount)
													gret = -20;
												else
													gret = -10;

												release_timer(Id);
												goto exit_bill;
											}
											break;
										}
									}
										
								}
							}
							release_timer(Id);
						}
					}
				}
			    api_set_apptimer(dtimer3, 40);
			}
#if 0
		    if(Com_Ready(BARCODE_PORT))
		    {
		        ret = Com_Getc(BARCODE_PORT);
				if(ret >= 0x20 && ret <=127)
					printf("%c ", ret);
				else
					printf("[0x%02X] ", ret);
		    }
#endif
		}
	}
	gret = -10;
exit_bill:
	Region01.Issue_Info.bill_input_amount = amount;
	bill_accept_onoff(OFF);
	send_msg_ui(qAMOUNT_INPUT, qBILL_COIN_ACCEPT, gret);
	return gret;
}

//------------------------------------------------------------------------------------------------------
// ��ǥ�ݾװ� ����ä�� ����:0, (��ǥ�ݾ׺��� ���� ���·� �������:1), ��ǥ�ݾ׺��� ūä�� ����:2, �Աݾ��� ����ä�� ����:3, 
// �Աݾ��� ����ä�� �ð�����:-1, �����νı� �ҷ��̸鼭 �ݾ��� ����ä�� ����: -10, �����νı� �ҷ��̸鼭 �ݾ��� ����ä�� ����:-20
// �߸��� �ݾ��Է����� ����: -30, (����ɸ� ���·� ����:99)
// tamount:������, ttime:�ʴ���
//------------------------------------------------------------------------------------------------------
int bill_accept2(int tamount, int ttime)
{
	byte Id, Id2, ret;
	byte tbuf[20];
	int amount=0, gret=0;
	byte only_one=0;
	t_qdata qdata;
	int cnt=0;
	if(tamount < 1000)
	{
		gret = -30;
		goto exit_bill;
	}
	bill_init();
	bill_config(tamount);
	if(!bill_connect())
	{
		gret = -10;
		goto exit_bill;
	}
	
//	Region01.Issue_Info.bill_target_amount = tamount;
//	Region01.Issue_Info.bill_input_amount = amount;
    api_set_apptimer(dtimer2, ttime*100);
    api_set_apptimer(dtimer3, 40);
	if(bill_accept_onoff(ON))
	{
		while(1)
		{
			usleep(1);
			if(msg_cancel)
			{
				while(bill_is_running()) ;
				bill_accept_onoff(OFF);
				return CANCEL;
			}
			if(api_chk_apptimer(dtimer2)==FALSE)
			{
				aDebugMsg("Ÿ�Ӿƿ�!!\n");
				log_printf(0, "BILL:Ÿ�Ӿƿ�!");
				if(bill_sendrcv("GA", '?', tbuf))
				{
					if(memcmp(&tbuf[1], "ga", 2)==0)
					{
						if(tbuf[3]==0x03) // �ɷ��ִ� ���� �����ϴ� ���!
						{
							if(only_one==0)
							{
								send_msg_ui(qTEST, qTEST_BILLACCEPT, 99);
								only_one=1;
							}
						    api_set_apptimer(dtimer2, ttime*100);
							log_printf(0, "BILL:[%d]���� �ɷ�����!", cnt++);
						}
						else
						{
							if(amount)
							{
								
								gret = -1;
							}
							else
							{
								gret = 3;
							}
							goto exit_bill;
						}
					}
				}
				else
				{
					if(amount)
						gret = -20;
					else
						gret = -10;
					goto exit_bill;
				}
			}
			if(api_chk_apptimer(dtimer3)==FALSE)
			{
				if(bill_sendrcv("GA", '?', tbuf))
				{
					if(memcmp(&tbuf[1], "ga", 2)==0)
					{
						//printf("<--- %c%c%c[0x%02X][0x%02X]\n", tbuf[0], tbuf[1], tbuf[2], tbuf[3], tbuf[4]);
						
						if(tbuf[3]==0x03) // �ɷ��ִ� ���� �����ϴ� ���! 5�ʰ� �����ɸ��� �����Ǿ� ������ 99�� ������.
						{
							if(only_one==0)
							{
								api_set_apptimer(dtimer4, 500);
								only_one=1;
							}
							else
							{
								if(api_chk_apptimer(dtimer4)==FALSE)
								{
									send_msg_ui(qTEST, qTEST_BILLACCEPT, 99);
									only_one=0;
									log_printf(0, "BILL:�����ɸ�!!!");
									// �����޽��� ������, ��� üũ�Ѵ�.
									api_set_apptimer(dtimer2, ttime*100);
								}
							}
							//printf("ActiveStatus=0x%02X\n", tbuf[3]);
						}
						if(tbuf[3]==0x0B)
						{
							byte Id;
							set_timer(&Id, 200);
							while(1)
							{
								usleep(1);
								if(chk_timer(Id)==FALSE)
									break;
								if(bill_sendrcv("GB", '?', tbuf))
								{
									//printf("<--- %c%c%c[0x%02X][0x%02X]\n", tbuf[0], tbuf[1], tbuf[2], tbuf[3], tbuf[4]);
									if(memcmp(&tbuf[1], "gb", 2)==0)
									{
										if(tbuf[3]==0) 
										{
											aDebugMsg("�Լ��� ���� ����");
										}
										else if(tbuf[3]==1) 
										{
											aDebugMsg("1,000�� �Լ���");
											amount += 1000;
											//Region01.Issue_Info.bill_input_amount = amount;
											//send_msg_ui(qTEST, qTEST_BILLACCEPT, 1);
											bill_config(tamount-amount);
											log_printf(1, "BILL:���Աݾ�=%d, ��ǥ�ݾ�=%d, ����ݾ�=%d", 1000, tamount, amount);
										}
										else if(tbuf[3]==5) 
										{
											aDebugMsg("5,000�� �Լ���");
											amount += 5000;
											//Region01.Issue_Info.bill_input_amount = amount;
											//send_msg_ui(qTEST, qTEST_BILLACCEPT, 1);
											bill_config(tamount-amount);
											log_printf(1, "BILL:���Աݾ�=%d, ��ǥ�ݾ�=%d, ����ݾ�=%d", 5000, tamount, amount);
										}
										else if(tbuf[3]==10) 
										{
											aDebugMsg("10,000�� �Լ���");
											amount += 10000;
											//Region01.Issue_Info.bill_input_amount = amount;
											//send_msg_ui(qTEST, qTEST_BILLACCEPT, 1);
											bill_config(tamount-amount);
											log_printf(1, "BILL:���Աݾ�=%d, ��ǥ�ݾ�=%d, ����ݾ�=%d", 10000, tamount, amount);
										}
										else if(tbuf[3]==50) 
										{
											aDebugMsg("50,000�� �Լ���");
											amount += 50000;
											//Region01.Issue_Info.bill_input_amount = amount;
											//send_msg_ui(qTEST, qTEST_BILLACCEPT, 1);
											bill_config(tamount-amount);
											log_printf(1, "BILL:���Աݾ�=%d, ��ǥ�ݾ�=%d, ����ݾ�=%d", 50000, tamount, amount);
										}

										if(amount == tamount)
										{
											gret = 0;
											release_timer(Id);
											goto exit_bill;
										}
										else if(amount > tamount)
										{
											gret = 2;
											release_timer(Id);
											goto exit_bill;
										}
										else
										{
											while(bill_is_running()) ;
											if(!bill_accept_onoff(ON))
											{
												aDebugMsg("�Լ����� ��������!!\n");
												log_printf(0, "BILL:�Լ����� ��������");
												if(amount)
													gret = -20;
												else
													gret = -10;

												release_timer(Id);
												goto exit_bill;
											}
											break;
										}
									}
										
								}
							}
							release_timer(Id);
						}
					}
				}
			    api_set_apptimer(dtimer3, 40);
			}
#if 0
		    if(Com_Ready(BARCODE_PORT))
		    {
		        ret = Com_Getc(BARCODE_PORT);
				if(ret >= 0x20 && ret <=127)
					printf("%c ", ret);
				else
					printf("[0x%02X] ", ret);
		    }
#endif
		}
	}
	gret = -10;
exit_bill:
	//Region01.Issue_Info.bill_input_amount = amount;
	bill_accept_onoff(OFF);
	send_msg_ui(qTEST, qTEST_BILLACCEPT, gret);
	return gret;
}


