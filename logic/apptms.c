#include "include.h"

pthread_mutex_t patch_check_sync_mutex = PTHREAD_MUTEX_INITIALIZER;

byte tms_check(void)
{
	byte currDate[16+1]={0};
	int ret=0;
	ulong long_tms_time=0, long_tmp1=0, long_tmp2=0;
	byte ttt_buf[16+1]={0};
	int random_num1=0, random_num2=0, r_hour=0, r_hour_rest=0, r_min=0, r_sec=0;

	get_dtm(currDate);
	if((ret=memcmp(&Region04.check_ymdsms1[6], &currDate[6], 6)) == 0)
	{
		return 1;
	}
	pthread_mutex_lock(&patch_check_sync_mutex);
	if(ret < 0) // 시간이 경과됨
	{
		sprintf(Region04.check_ymdsms1, "%12.12s", currDate);
		long_tms_time = a2l(&currDate[6], 2);
		long_tms_time++; // 1시간 후마다 tms_check 날림

		srand((unsigned)time(NULL));
		random_num1 = (rand() % 3540) + 1; // 1~3540까지의 난수 발생
		long_tmp1 = a2l(&currDate[8], 2); // 현재 시간의 분을 구한다.
		long_tmp2 = a2l(&currDate[10], 2); // 현재 시간의 초를 구한다.
		long_tmp1 = long_tmp1*60+long_tmp2; // 초단위 환산
		random_num2 = random_num1 + long_tmp1; // 초단위 최종 시간
		r_hour = random_num2 / 3600; // 0~1시간 까지 나올 수 있음
		r_hour_rest = random_num2 % 3600;
		r_min = r_hour_rest / 60; // 0~59분까지 나올 수 있음
		r_sec = r_hour_rest % 60; // 0~59초까지 나올 수 있음

		long_tms_time += r_hour;
		long_tms_time = long_tms_time % 24;
//		aDebugMsg("random_num=%d, curr_sec=%d, total_sec=%d, r_hour=%d, r_hour_rest=%d, r_min=%d, r_sec=%d long_tms_time=%d", random_num1, long_tmp1, random_num2, r_hour, r_hour_rest, r_min, r_sec, long_tms_time);
		sprintf(&Region04.check_ymdsms1[6], "%02d%02d%02d", (int)long_tms_time, r_min, r_sec);
//		aDebugMsg("3:patch check time=========>>%s", &Region04.check_ymdsms1[6]);


//		long_tms_time = long_tms_time % 24;
//		sprintf(ttt_buf, "%02d", (int)long_tms_time);
//		memcpy(&Region04.check_ymdsms1[6], ttt_buf, 2);
	}
	pthread_mutex_unlock(&patch_check_sync_mutex);
	return 0;
}

byte tms_check1(void)
{
	byte currDate[16+1]={0};

	get_dtm(currDate);
	if(memcmp(&Region04.check_ymdsms2[6], &currDate[6], 6) == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	//fi_region_4K_write((byte *)&Region04, FL_REGION04_AREA, sizeof(FL_REGION_04));
	return 0;
}

byte tms_update_check(void)
{
	byte currDate[16+1]={0};
	int ret=0;
	ulong long_tms_time=0, long_tmp1=0, long_tmp2=0;
	byte ttt_buf[16+1]={0};
	int random_num1=0, random_num2=0, r_hour=0, r_hour_rest=0, r_min=0, r_sec=0;

	get_dtm(currDate);
	if((ret=memcmp(&Region04.ymdsms, currDate, 12)) == 0)
	{
		return 1;
	}
	pthread_mutex_lock(&patch_check_sync_mutex);
	if(ret < 0) // 시간이 경과됨
	{
		sprintf(Region04.check_ymdsms1, "%12.12s", currDate);
		long_tms_time = a2l(&currDate[6], 2);
		long_tms_time++; // 1시간 후마다 tms_check 날림

		srand((unsigned)time(NULL));
		random_num1 = (rand() % 3540) + 1; // 1~3540까지의 난수 발생
		long_tmp1 = a2l(&currDate[8], 2); // 현재 시간의 분을 구한다.
		long_tmp2 = a2l(&currDate[10], 2); // 현재 시간의 초를 구한다.
		long_tmp1 = long_tmp1*60+long_tmp2; // 초단위 환산
		random_num2 = random_num1 + long_tmp1; // 초단위 최종 시간
		r_hour = random_num2 / 3600; // 0~1시간 까지 나올 수 있음
		r_hour_rest = random_num2 % 3600;
		r_min = r_hour_rest / 60; // 0~59분까지 나올 수 있음
		r_sec = r_hour_rest % 60; // 0~59초까지 나올 수 있음

		long_tms_time += r_hour;
		long_tms_time = long_tms_time % 24;
//		aDebugMsg("random_num=%d, curr_sec=%d, total_sec=%d, r_hour=%d, r_hour_rest=%d, r_min=%d, r_sec=%d long_tms_time=%d", random_num1, long_tmp1, random_num2, r_hour, r_hour_rest, r_min, r_sec, long_tms_time);
		sprintf(&Region04.check_ymdsms1[6], "%02d%02d%02d", (int)long_tms_time, r_min, r_sec);
//		aDebugMsg("2:patch check time=========>>%s", &Region04.check_ymdsms1[6]);

//		long_tms_time = long_tms_time % 24;
//		sprintf(ttt_buf, "%02d", (int)long_tms_time);
//		memcpy(&Region04.check_ymdsms1[6], ttt_buf, 2);
		tms_update_flag = 0;
		tms_check_flag = 0;
	}
	pthread_mutex_unlock(&patch_check_sync_mutex);
	return 0;
}



