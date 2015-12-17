#include "include.h"

pthread_cond_t  qmsgq_ok = PTHREAD_COND_INITIALIZER;
pthread_cond_t  qmsgq_act_on = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qmsgq_mutex = PTHREAD_MUTEX_INITIALIZER;

//----------------------------------------------------------------------------------------------
void *MsgTread(void * data)
{
	int ret=0, t_step=0;
	int msgqid;
	t_qdata qbuf;

	if(-1==(msgqid=msgget((key_t)RECVQ_ID, IPC_CREAT|0666)))
	{
		aDebugMsg("msgget()실패");
		return 0;
	}

	msg_thread_state = 1;
	while(msg_thread_state == 1 && msg_thread_exit==0)
	{
		if(-1==msgrcv(msgqid, &qbuf, sizeof(t_qdata)-sizeof(long), 1, 0))
		{
			perror("msgrcv()실패");
			return 0;
		}
		printf("[logic received msg] ---> command_type:0x%02X command_code:0x%02X\n", qbuf.command_type, qbuf.command_code);
		HexDump((byte *)&qbuf,256);
		memcpy(&msgq_buf, &qbuf, sizeof(t_qdata));
		pthread_mutex_lock(&qmsgq_mutex);
		pthread_cond_signal(&qmsgq_ok);
		pthread_cond_wait(&qmsgq_act_on, &qmsgq_mutex);
		pthread_mutex_unlock(&qmsgq_mutex);
	}

	msg_thread_state = 0;
	return 0;
}
//----------------------------------------------------------------------------------------------
void *MsgTread2(void * data)
{
	int ret=0, t_step=0;
	int msgqid;
	t_qdata qbuf;

	if(-1==(msgqid=msgget((key_t)RECVQ_ID, IPC_CREAT|0666)))
	{
		aDebugMsg("msgget()실패");
		return 0;
	}

	msg_thread_state2 = 1;
	while(msg_thread_state2 == 1 && msg_thread_exit2==0)
	{
		if(-1==msgrcv(msgqid, &qbuf, sizeof(t_qdata)-sizeof(long), 2, 0))
		{
			perror("msgrcv()실패");
			return 0;
		}
		printf("[logic cancel received msg] ---> command_type:0x%02X command_code:0x%02X\n", qbuf.command_type, qbuf.command_code);
		HexDump((byte *)&qbuf,256);
		if(qbuf.command_type == qETC && qbuf.command_code == qCANCEL)
		{
			if(!now_idle)
			{
				msg_cancel = 1;
			}
			else
				send_msg_cancel();
		}
	}

	msg_thread_state2 = 0;
	return 0;
}
//----------------------------------------------------------------------------------------------
void *MsgTread4(void * data) // tms check thread
{
	int ret=0, t_step=0;
	int msgqid;
	t_qdata qbuf;
	ulong long_tms_time=0;
	byte ttt_buf[16+1]={0};

	msg_thread_state4 = 1;
	tms_check_flag = 1;
	
	while(msg_thread_state4 == 1 && msg_thread_exit4==0)
	{
		if(tms_check_flag==0 && tms_update_flag==0)
		{
			if(tms_check())
			{
				tms_check_flag = 1;
				msg_to_ui(qTMS, qTMS_CC, 0);
			}
		}
		if(tms_update_flag==1)
		{
			if(tms_update_check())
			{
				tms_check_flag = 2;
				msg_to_ui(qTMS, qTMS_CC, 0);
			}
		}
		usleep(200);
	}

	msg_thread_state4 = 0;
	return 0;
}
//------------------------------------------------------------------------------------------------------
int send_msgq(t_qdata *qdata, int type)
{
	int msgqid;
	
	qdata->msg_type = type;
	
	if(-1==(msgqid=msgget((key_t)SENDQ_ID, IPC_CREAT|0666)))
	{
		aDebugMsg("msgget()실패");
		return 1;
	}
#ifndef SELF_DEBUG
	if(-1==(msgsnd(msgqid, qdata, sizeof(t_qdata)-sizeof(long), 0)))
	{
		perror("msgsnd() 실패");
		return 2;
	}
#else
	if(-1==(msgsnd(msgqid, qdata, sizeof(t_qdata)-sizeof(long), IPC_NOWAIT)))
	{
		perror("msgsnd() 실패");
		return 2;
	}
#endif
	return 0; // success
}
//------------------------------------------------------------------------------------------------------


