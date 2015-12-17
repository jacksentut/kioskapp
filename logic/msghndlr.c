#include "include.h"

//#define ETHERNET_DEBUG

// 999999_TJH ->
#define ETH_RX_QSIZE 8191
struct ETH_PORT
{
    unsigned int rd_ptr;
    unsigned int wr_ptr;
    unsigned char rx_buf[ETH_RX_QSIZE+1];
    unsigned char temp_buf[ETH_RX_QSIZE+1];
};
struct ETH_PORT EthStr;
// <-

byte dport_no[16];

byte ComPort_Ready(void);
byte ComPort_Getc(void);

void data_skip(void)
{
	// 20120510_TJH : -> 라인이 PSTN인 경우도 버퍼 클리어하기 위함.
	if(linetype==PSTN_LINE)
	{
    //    while(Com_Ready(PSTN_PORT)) Com_Getc(PSTN_PORT);
	}
	else
	if(linetype==ETHERNET_LINE)
		while(ComPort_Ready()) ComPort_Getc();
	// <-
}

// 추가
// 반환값 => 1 : 정상, 0 : 비정상
byte socket_check(void)
{
	struct pollfd event;
	int result=0;
    
    if(socketfd < 0) return 0;
    else
    {
        event.fd = socketfd;
        event.events = POLLERR | POLLHUP;
        result = poll((struct pollfd *)&event, 1, 0);
        if(event.revents & (POLLERR | POLLHUP))
        {
        	return 0;
        }
    }
        
    return 1;
}
// <-

byte ComPort_Ready(void)
{
    byte ret=0;
	// 999999_TJH ->
	struct pollfd event;
	int result, cnt;
	// <-
    
    if(linetype==PSTN_LINE)
	{
//        ret = Com_Ready(PSTN_PORT);
	}
	else
    if(linetype==ETHERNET_LINE)
    {
        if(socketfd < 0) return 0;
            
        if(EthStr.wr_ptr != EthStr.rd_ptr) return 1;
        else
        {
            event.fd = socketfd;
            event.events = POLLIN;
            result = poll((struct pollfd *)&event, 1, 0);
            if(result)
            {
                if((result = read(socketfd, EthStr.temp_buf, sizeof(EthStr.temp_buf))) > 0)
                {
                    for(cnt=0; cnt<result; cnt++)
                    {
                        EthStr.rx_buf[EthStr.wr_ptr++ & ETH_RX_QSIZE] = EthStr.temp_buf[cnt];
#ifdef ETHERNET_DEBUG
sputc(AUX_PORT, EthStr.temp_buf[cnt]); 
#endif
                    }
                }
            }
			
            if(EthStr.wr_ptr != EthStr.rd_ptr)	return 1;
            else								return 0;
        }
    }
	// <-
    else
	{
       // ret = Com_Ready(PRIVATE_PORT);
	}
        
    return(ret);
}

byte ComPort_Getc(void)
{
    byte ret = 0;
    
    if (linetype==PSTN_LINE)
    {
//        ret = Com_Getc(PSTN_PORT);
    }
	else
    if(linetype==ETHERNET_LINE)
    {
        ret = EthStr.rx_buf[EthStr.rd_ptr++ & ETH_RX_QSIZE];
    }
	// <-
    else
	{
//        ret = Com_Getc(PRIVATE_PORT);
	}
        
    return(ret);
}

byte ComPort_Write(void *Txdat, int len)
{
    if(linetype==PSTN_LINE)
    {
//        Com_Write(PSTN_PORT, Txdat, len);
    }
	else
    if(linetype==ETHERNET_LINE)
    {
    	if(socket_check())
    	{
#ifdef ETHERNET_DEBUG
int siz;

for(siz=0; siz<len; siz++)
{
	sputc(AUX_PORT, *((char*)Txdat+siz));
}
#endif
        	write(socketfd, Txdat, len);
    	}
		else
			return 1;
    }
	// <-
    else
	{
//        Com_Write(PRIVATE_PORT, Txdat, len);
	}

	return 0;
}

void ComPort_Putc(char Txch)
{
    byte Txdat[3];
    
    Txdat[0] = Txch;
    
    if(linetype==PSTN_LINE)
   	{
//        Com_Write(PSTN_PORT, Txdat, 1);
   	}
	else
    if(linetype==ETHERNET_LINE)
   	{
#ifdef ETHERNET_DEBUG 
sputc(AUX_PORT, Txdat[0]);
#endif
		write(socketfd, Txdat, 1);  
    }
	// <-
    else
	{
//        Com_Write(PRIVATE_PORT, Txdat, 1);
	}
}


//return value :
//				 MD_CONNECT, MD_TIMEOUT, CANCEL
byte ethernet_connect(void)
{
	struct sockaddr_in serv_addr; 
	fd_set read_fs;
	struct timeval timeout ; /* 0.1 sec */
	struct linger option; 
	ulong  laddr;
	byte Id=0;
	int  nport;
	int  val;
	byte ret;
	byte temp[64];
	

	if(socketfd > 0)
	{
		close(socketfd);
		socketfd = -1;
	}

	memset(&EthStr, 0x00, sizeof(struct ETH_PORT)); // 999999_TJH 추가
	socketfd = socket(PF_INET, SOCK_STREAM, 0);
	if(socketfd < 0)
	{
		printf("socket open error...\n");
		socketfd = -1;
		aDebugMsg("  소켓생성실패  ");
		aDebugMsg("다시시도해주세요");
		return(2);
	}

	option.l_onoff = 1;
	option.l_linger = 0;

	setsockopt(socketfd, SOL_SOCKET, SO_LINGER, (char*)&option, sizeof(option));
	memset(&serv_addr, 0x00, sizeof(serv_addr));
	serv_addr.sin_family= AF_INET;

	sprintf(temp, "%s", Region01.Comm.Ethernet.address);
	nport = atoi(Region01.Comm.Ethernet.portno);

	if((strlen(temp)==0) || (nport <= 0))
	{
		if(socketfd > 0)
			close(socketfd);
        socketfd = -1;
		aDebugMsg("IP주소 확인요망!");
		aDebugMsg("다시시도해주세요");
		return(2);
	}

	laddr = inet_addr(temp);
	if(laddr == INADDR_NONE)
	{
		if(socketfd > 0)
			close(socketfd);
		socketfd = -1;
		aDebugMsg("IP주소 확인요망?");
		aDebugMsg("다시시도해주세요");
		return(2);
	}

	serv_addr.sin_addr.s_addr = laddr;
	serv_addr.sin_port = htons(nport);

	//set_timer(&Id, 5*100); // 20120405_LAN_TJH 임의로 5초로 잡았음.

	val =fcntl(socketfd, F_GETFL, 0);
	assert( val != -1);
	val |= O_NONBLOCK;
	val = fcntl(socketfd, F_SETFL, val);
	assert( val != -1);

	set_timer(&Id, 3*100);
	while(1)
	{
		usleep(1);
    	if(msg_cancel) return CANCEL;
		if(chk_timer(Id)==FALSE)
		{
			if(socketfd > 0)
				close(socketfd);
			socketfd = -1;
			release_timer(Id);
			return MD_TIMEOUT;
		}

		if(connect(socketfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) != 0)
		{
			FD_ZERO(&read_fs);
			FD_SET(socketfd, &read_fs);
			timeout.tv_sec = 0;
			timeout.tv_usec=10000;
			select(socketfd+1, NULL, &read_fs, NULL, &timeout);
		}
		else
		{
			release_timer(Id);
			return MD_CONNECT;
		}
	}
}

byte Eth_connect_check(int ServKind)
{
	int nread, iLen, iRtn;
	char cRcvData[255]={0};
	int str_len;
	byte ucEncryptData[1024]={0};
	byte ucDecryptData[1024]={0};
	byte Id=0, kch; // 20120405_LAN_TJH 추가
	

	struct pollfd client;

	
	set_timer(&Id, Region01.Comm.enq_tm*100); // 20120405_LAN_TJH 타임아웃 설정

	client.fd = socketfd;
	client.events = POLLIN;

	while(1)
	{
		// 20120405_LAN_TJH ->
		usleep(1);
		if(chk_timer(Id)==FALSE)
		{
			release_timer(Id);
			return TIMEOUT;
		}
		
		nread = poll(&client, 1, 100);
		if(client.revents & POLLIN)
		{
			str_len = read(socketfd, cRcvData, sizeof(cRcvData)-1);
			cRcvData[str_len] = 0;
		}
	}

	release_timer(Id); // 20120405_LAN_TJH
	return 1; // 20120405_LAN_TJH
}

// 20120405_LAN_TJH ->
byte Eth_rcv_data(int ServKind) // 20111012_TJH_006
{
	int nread, iLen=0;
	char cRcvData[4096]={0};
	int str_len, read_len;
	byte ucEncryptData[1024]={0};
	struct pollfd client;
	byte kch; // 20120405_LAN_TJH
	

	client.fd = socketfd;
	client.events = POLLIN;

	str_len = 0;
	while(1)
	{
		usleep(1);
		if(api_chk_apptimer(dtimer2)==FALSE)
		{
			return TIMEOUT; // 20120405_LAN_TJH
		}

		// 20120405_LAN_TJH ->
		if(kField.netcan_flag!=TRUE)
    	{
    	}
		// <-
			
		nread = poll(&client, 1, 100);
		
		if(client.revents & POLLIN)
		{
			memset(cRcvData, 0x00, sizeof(cRcvData));
			read_len = read(socketfd, cRcvData, sizeof(cRcvData)-1);
			cRcvData[read_len] = 0x00;

			if(str_len==0)
			{
				if(cRcvData[0]=='E')
				{
				}
				else
				if(cRcvData[0]=='D')
				{
				}
				else
				if(cRcvData[0]==ACK)
				{
					// 20120405_LAN_TJH ->
	                aDebugMsg("  인터넷수신중  ");
					aDebugMsg("잠시기다려주세요");
					// <-
					
					str_len = 0;
					api_set_apptimer(dtimer2, Region01.Comm.resp_tm*100);
					continue;
				}
				else
				if(cRcvData[0]==NAK)
				{
					return 2;
				}
				else
				if(cRcvData[0]==DLE)
				{
					return 3;
				}
				else
				if(cRcvData[0]==EOT)
				{
					return 4;
				}
				// 20120201_TJH ->
				else
				{
					continue;
				}
				// <-
			}
			else
			{
				memcpy(&recvbuf[str_len], cRcvData, read_len);
				str_len += read_len;
			}

			if(str_len==iLen)
			{
			}
		}
	}
}
// <-

//return value :
//               MD_CONNECT, MD_TIMEOUT, CANCEL
byte dlineconn_chk(int second)
{
    byte Id=0, ret, rch, kch;
    byte nua_temp[16]={0};
    
    set_timer(&Id, second*100);
    while(1)
    {
    	if(kField.netcan_flag!=TRUE)
    	{
    	}

        if(chk_timer(Id)==FALSE)
        {
            release_timer(Id);
            return(MD_TIMEOUT);
        }

        if(ComPort_Ready())
        {
            rch = ComPort_Getc();
            memcpy(nua_temp, nua_temp+1, sizeof(nua_temp)-1);
            nua_temp[sizeof(nua_temp)-1] = rch;
            if (!memcmp(&nua_temp[sizeof(nua_temp)-12], "DESTINATION\r", 12))
            {
                if (ComPort_Ready()) continue;
                
                release_timer(Id);
                ret = MD_CONNECT;

                aDebugMsg(" 접속되었습니다 ");
				aDebugMsg("잠시기다려주세요");
				
                break;
            }
        }
    }

    if (ret==MD_CONNECT)
    {
        ComPort_Write(dport_no, Strlen(dport_no));
    }
    return(ret);
}


//return value : modem return value
byte pstnconn_chk(word second)
{
    byte ret, Id1=0, Id2=0;
    byte blink;

    set_timer(&Id1, second*100);
    set_timer(&Id2, 110);
    blink = 48;
    while(1)
    {
    	if(msg_cancel) return CANCEL;
    	if(kField.netcan_flag!=TRUE)
    	{
    	}

        if (chk_timer(Id1)==FALSE)
        {
            release_timer(Id1);
            release_timer(Id2);
            return(MD_TIMEOUT);
        }
        if (chk_timer(Id2)==FALSE)
        {
            if (blink==48 || blink >= 64)
            {
                aDebugMsg("                ");
                blink = 48;
            }
            //lcddir((byte *)right, blink, L_NORMAL);
            blink += 1;
            reset_timer(Id2, 110);
        }

//        md_conn_check();
        if (mdflag)
        {
            release_timer(Id1);
            release_timer(Id2);
            return(mdflag);
        }
    }
}

void conn_start_init(int line)
{
    if(!line)
    {
        if(Region01.Comm.use_comm_type=='1')
			line = PSTN_LINE;
		else
			line = (~PSTN_LINE & Region01.Comm.comm_line);
    }
    
    if(!((line & PSTN_LINE) || (line & C2I_LINE) || (line & CDMA_LINE) || (line & CHECK_LINE) || (line & WIBRO_LINE) || (line & ETHERNET_LINE) || (line & ETHMODEM_LINE)))
        line = PSTN_LINE;

    dialcnt = 0;
    linetype = line;
    HostConnected = 1;
    Predial = 0;
}

void get_dialno(void)
{
    word cnt=0;

    Memset(telbuf, 0x00, sizeof(telbuf));
    switch(linetype)
    {
        case PSTN_LINE:
            if(Region01.Comm.Pstn.pabx_on=='1')
            {
                memcpy(&telbuf[cnt], Region01.Comm.Pstn.pabx_no, Strlen(Region01.Comm.Pstn.pabx_no));
                cnt+=strlen(Region01.Comm.Pstn.pabx_no);
            }
            if(Region01.Comm.Pstn.pabx_on=='1' && (Region01.Comm.Pstn.pabx_wait>='1' && Region01.Comm.Pstn.pabx_wait<='3'))
            {
                memset(&telbuf[cnt] , '.', (word)Region01.Comm.Pstn.pabx_wait-0x30);
                cnt += (word)Region01.Comm.Pstn.pabx_wait-0x30;
            }

            if(dialcnt % 2)
                memcpy(telbuf+cnt, Region01.Comm.Pstn.apptel_02, strlen(Region01.Comm.Pstn.apptel_02));
            else
                memcpy(telbuf+cnt, Region01.Comm.Pstn.apptel_01, strlen(Region01.Comm.Pstn.apptel_01));

            break;
			
        case ETHERNET_LINE:
			aDebugMsg("Server IP:%s", Region01.Comm.Ethernet.address);
            ip_format_copy(telbuf, Region01.Comm.Ethernet.address);
			host_port_num = (int)a2l(Region01.Comm.Ethernet.portno, strlen(Region01.Comm.Ethernet.portno));
            break;
    }
}

void disconnect(void)
{

    if (HostConnected==1)
    {
        if (linetype == PSTN_LINE)
        {
            //api_md_disconnect();
        }
		else
        if (linetype == ETHERNET_LINE)
        {
			if(socketfd>0) // 20120201_TJH
			{
				//write(socketfd, "\x10\x10\x10", 3); // DLE // 20120405_LAN_TJH
				if(socket_check())
				{
        			//write(socketfd, "\x10\x10\x10", 3);
				}
				close(socketfd);
			}
			socketfd = -1;
        }
        else
        {
            ComPort_Write("\x10\x10\x10", 3); // DLE
        }
    }

    HostConnected = 0;
    Predial = 0;
}

extern int serial_pos_thread_exit;
extern int serial_pos_thread_state;
extern int serial_pos_thread_stop;

extern pthread_mutex_t sync_mutex;
extern pthread_cond_t  ser_pos_ok;
extern pthread_cond_t  ser_pos_act_on;

/* returns :                       */
/*           0 : success, 1 : fail, CANCEL */
byte host_connect(int ServKind, int type)
{
    byte ret=0, cdma_retry=0, cr_retry=0, Cstep = 0;
    int  second=0, bps=B9600;
	byte first = 0;
	t_qdata qdata;

    while(1)
    {
		usleep(1);
    	if(msg_cancel) return CANCEL;
        switch(Cstep)
        {
            case 0:
                if(Predial != 1) 
                {
					aDebugMsg("conn_start_init(0), Predial=%d, linetype=%d", Predial, linetype);
					conn_start_init(0);
                }
				
                if(linetype & PSTN_LINE)
                {
					Cstep = 10;
                }
                else if(linetype & ETHERNET_LINE)
                {
                    Cstep = 30;
                }
                else
                {
                    aDebugMsg("라인할당실패(1) 다시 거래해주세요");
                    return CANCEL;
                }
                aDebugMsg("  접속중입니다  ");
				aDebugMsg("잠시기다려주세요");
                break;
                
            // PSTN
            case 10:
                if (Predial != 1)
                {
//                    sio_set(PSTN_PORT, Region01.Comm.Pstn.pstn_spd);
                    get_dialno();
//                    md_start();
                }
                if (Region01.Comm.Pstn.pabx_on=='1')
                    ret = pstnconn_chk(Region01.Comm.tm02);
                else
                    ret = pstnconn_chk(Region01.Comm.tm01);
                if (ret==CANCEL)
                {
                    disconnect();
                    return(CANCEL);
                }
                else
                if (ret==MD_CONNECT) 
                {
                    aDebugMsg(" 연결되었습니다 ");
					aDebugMsg("잠시기다려주세요");
                    return(0);
                }
                disconnect();
    
                conn_start_init(linetype);
                aDebugMsg(" 재접속중입니다 ");
				aDebugMsg("잠시기다려주세요");
                
                dialcnt++;
//                sio_set(PSTN_PORT, Region01.Comm.Pstn.pstn_spd);
                get_dialno();
//                md_start();
                if (Region01.Comm.Pstn.pabx_on=='1')
                    ret = pstnconn_chk(Region01.Comm.tm02);
                else
                    ret = pstnconn_chk(Region01.Comm.tm01);
                if (ret==CANCEL)
                {
                    disconnect();
                    return(CANCEL);
                }
                else
                if (ret==MD_CONNECT)
                {
                    aDebugMsg(" 연결되었습니다 ");
					aDebugMsg("잠시기다려주세요");
                    return(0);
                }
                disconnect();
        
                aDebugMsg("   전화선장애   ");
                aDebugMsg("재다이얼(입력키)");
                if(ret==CANCEL || ret==TIMEOUT) return CANCEL;
                Cstep = 0;
                break;
            case 30:
				aDebugMsg("case 30...");
                for(cr_retry=0; cr_retry<3; cr_retry++) // 5회 -> 3회
                {
                    if(!cr_retry)
                    {
                        get_dialno();
                    }
                    else
                    {
                        aDebugMsg(" 재접속중입니다 ");
						aDebugMsg("잠시기다려주세요");
                        
                        HostConnected = 1;
                        get_dialno();
                    }
					
					aDebugMsg("ethernet_connect() started...");
                    
                    ret = ethernet_connect();
					// 20120427_TJH ->
					if(ret==MD_CONNECT)
                    {
	                    aDebugMsg(" 연결되었습니다 ");
						aDebugMsg("잠시기다려주세요");
						if(type)
						{
							qdata.command_type = qETHERNET;
							qdata.command_code = qETHERNET_CONNECT;
							qdata.qlan_resp.response_code = 0;
							qdata.qlan_resp.t_field.progress = 30;
							send_msgq(&qdata, 1);
						}
                        return 0;
                    }
					
					disconnect();
					if(ret==CANCEL)
	                {
	                    return CANCEL;
	                }
					else
					{
						if(ethernet_link_chk()==1)
	                    {
	                        if(dns_info_check())
	                    	{
				                aDebugMsg(" 인터넷재설정중 ");
			                    aDebugMsg("잠시만기다리세요");
	                    		if(init_ethernet()!=TRUE)
				                {
				                    aDebugMsg(" 인터넷설정실패 ");
				                    aDebugMsg(" 인터넷재설정후 ");
				                    aDebugMsg("다시거래해주세요");
								//	return CANCEL;
				                }
	                    	}
	                    }
						else
		                {
							if(type)
							{
#if 0
								qdata.command_type = qETHERNET;
								qdata.command_code = qETHERNET_CONNECT;
								qdata.qlan_resp.response_code = 2; // 랜선빠짐
								qdata.qlan_resp.t_field.progress = 0;
								send_msgq(&qdata, 1);
#endif
							}
		                    aDebugMsg(" 인터넷설정실패 ");
		                    aDebugMsg("랜선이빠졌습니다");
							aDebugMsg("   랜선연결후   ");
		                    aDebugMsg("다시거래해주세요");
							log_printf(0, "통신:랜선 빠짐!");
						//	return CANCEL;
		                }
					}
                }

                if(ret==CANCEL || ret==TIMEOUT) return CANCEL;

				if(type)
				{
					qdata.command_type = qETHERNET;
					qdata.command_code = qETHERNET_CONNECT;
					qdata.qlan_resp.response_code = 1; // 접속실패
					qdata.qlan_resp.t_field.progress = 0;
					send_msgq(&qdata, 1);
				}
				log_printf(0, "통신:접속 실패!");
				return 1;
        }
    }
    
    return(1);
}

byte Ether_Ready(void)
{
	int client_sockfd;
    int i;
    int nread;
    socklen_t clilen;
    struct sockaddr_in clientaddr;
    char message[255];
    int str_len;

    struct pollfd client;
	
    client.fd = socketfd;
    client.events = POLLIN;

    for(i=0;;)
    {
        nread = poll(&client, 1, 100);
        if(client.revents & POLLIN)
        {
            clilen=sizeof(clientaddr);
            client_sockfd = accept(socketfd, (struct sockaddr *)&clientaddr,  &clilen);

            //printf("EEIP=[%s] GIP[%s]\n", inet_ntoa(serveraddr.sin_addr), inet_ntoa(clientaddr.sin_addr));
            str_len = read(client_sockfd, message, sizeof(message)-1);
            //printf("READEND[%d]\n", str_len);
            message[str_len] = 0;
            //printf("messge [%s]\n", message);

			break;
        }
    }
	
    return 0;
}

/* returns :                       */
/*           0 : success, 1 : fail, CANCEL */
byte host_send_recv(int ServKind, byte ResendData, int type)
{
    byte ret;
    byte SRstep = 0;
    byte nua_temp[16]={0};
    int  nRet=0, control_cnt=1;
	int iLen=0;
	t_qdata qdata;

	if(linetype==PSTN_LINE) control_cnt = 3;
    else                    control_cnt = 1;

    while(1)
    {
    	if(msg_cancel) return CANCEL;
    	if(print_start(SW_ON, type)==0) return CANCEL;
		
        switch(SRstep)
        {    
            case 0:
                rlen = 0;
                memset(recvbuf, 0x00, sizeof(recvbuf));
                if(linetype==ETHERNET_LINE)
                {
					printf("요청전문 : \n");
					HexDump(sendbuf, slen);
					if(sendbuf[2]=='Y')
					{
						printf("암호화 요청전문 : \n");
						//HexDump(cyper_tmp_buf, enc_send_len);
						ret = ComPort_Write(cyper_tmp_buf, enc_send_len);
					}
					else
					{
						ret = ComPort_Write(sendbuf, slen);
					}
					if(ret)
					{
		                aDebugMsg(" 송신실패!!!!!! ");
						SRstep = 22;
						break;
					}
        		}
				else
				{
					ComPort_Write(sendbuf, slen);
				}
                aDebugMsg(" 송신결과확인중 ");
				aDebugMsg("잠시기다려주세요");
                api_set_apptimer(dtimer2, 3000); // 30초 대기
                SRstep++;
                break;
            case 1:
                if(api_chk_apptimer(dtimer2)==FALSE)
                {
                    SRstep = 21;
                    break;
                }
                
                if(ComPort_Ready())
                {
                    ret = ComPort_Getc();
                    memcpy(nua_temp, nua_temp+1, sizeof(nua_temp)-1);
                    nua_temp[sizeof(nua_temp)-1] = ret;

					// 999999_TJH -> 수정
					if(linetype==ETHERNET_LINE)
					{
						if(!memcmp(&nua_temp[sizeof(nua_temp)-1], "P", 1))
						{
	                        aDebugMsg("  인터넷수신중  ");
							aDebugMsg("잠시기다려주세요");
							recvbuf[rlen++] = ret;
	                        SRstep++;
							
						}
						else
						{
						}
					}
                }
                break;
            case 2:
                if(api_chk_apptimer(dtimer2)==FALSE)
                {
                    SRstep = 21;
                    break;
                }
                
                if(ComPort_Ready())
                {
                    ret = ComPort_Getc();
                    memcpy(nua_temp, nua_temp+1, sizeof(nua_temp)-1);
                    nua_temp[sizeof(nua_temp)-1] = ret;

					if(linetype==ETHERNET_LINE)
					{
						recvbuf[rlen++] = ret;

						if(rlen > sizeof(recvbuf))
	                    {
	                    	return 3;
	                    } 
	                    else
	                    {
							if(rlen==7)
							{
								//if(numeric_check(&recvbuf[3], 4))
								{
									iLen = (int)a2l(&recvbuf[3], 4);
									if(iLen > sizeof(recvbuf))
										return 3;
									SRstep++;
									break;
								}
							}
							else
							{
								break;
							}
	                    }
					}
                }
                break;
            case 3:
                if (api_chk_apptimer(dtimer2)==FALSE)
                {
                    SRstep = 21;
                    break;
                }

                if (ComPort_Ready())
                {
                    ret = ComPort_Getc();
					recvbuf[rlen++] = ret;
					//aDebugMsg("recv rlen=%d:recvbuf[rlen]=%02X", rlen-1, recvbuf[rlen-1]);
					if(rlen == (iLen+7))
					{
						if(type)
						{
							qdata.command_type = qETHERNET;
							qdata.command_code = qETHERNET_CONNECT;
							qdata.qlan_resp.response_code = 0;
							qdata.qlan_resp.t_field.progress = 80;
							send_msgq(&qdata, 1);
						}
						return 0;
					}
                }
                break;
            case 21:
				
                disconnect();

				aDebugMsg(">>>>>>>>>>>>>>>>>>> recv rlen=%d", rlen);
				if(type)
				{
					qdata.command_type = qETHERNET;
					qdata.command_code = qETHERNET_CONNECT;
					qdata.qlan_resp.response_code = 3; // 수신대기 시간초과
					qdata.qlan_resp.t_field.progress = 0;
					send_msgq(&qdata, 1);
				}
                aDebugMsg(" 송수신시간초과 ");
                aDebugMsg(" 다시거래하세요 ");
				log_printf(0, "통신:송수신 시간초과!");
                return(CANCEL);
        }
    }
}

/* returns :                       */
/*           0 : success, 기타 : fail */
/*           1 : connect fail, 2 : ENQ fail, 3 : send, receive fail, 4 : eot fail, CANCEL */
byte host_msganal(int ServKind, int type)
{
    byte ret=0, SubSTEP=0;
    byte ResendData=0;

#ifdef ETHERNET_DEBUG
sio_set(AUX_PORT, B115200);
#endif
    
    switch(ServKind)
    {
        default:
            while(1)
            {
		    	if(print_start(SW_ON, type)==0) return CANCEL;
//				usleep(1);
                switch(SubSTEP)
                {
                    case 0:
						aDebugMsg("case 0...");
                        ret = host_connect(ServKind, type);
                        if (ret) 
                        {
                            if (ret==CANCEL) return CANCEL;
                            else             return 1;
                        }
                        SubSTEP++;
                        break;
                    case 1:
                        ret = host_send_recv(ServKind, ResendData, type);
                        if(ret==CANCEL) return CANCEL;
                        else if(ret) 
                        {
                           	return 3;
                        }
                        ret = packet_anal(ServKind, type);
                        if(ret==CANCEL) return CANCEL;
                        else if(ret) 
                        {
							return 4;
                        }
						else
							return 0; // success
						break;

                }
            }
            break;
    }
    return(1);
}
//----------------------------------------------------------------------------------------------

