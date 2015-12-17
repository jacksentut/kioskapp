/////////////////////////////////////////////////////////////
// libsprinter.c(dot printer)
/////////////////////////////////////////////////////////////
void printer_version(unsigned char *version);

int  prt_get_column_position(void);
int  prt_set_column_position(int col);

void KS_to_KSSM(unsigned char *KS_Str, unsigned int len);
void KSSM_to_KS(unsigned char *KSSM_Str, unsigned int len);
void prt_step_move(void);
void prt_step_stop(void);

/* kjm 2013 0626 */
void set_bright(int x);

int printer_open(void);
int printer_close(void);

/* 프린터 초기화 */
// 정상 return 1; 	비정상 : 상태표시(printer.h 참고)
int printer_start(void);
/* printer_start 동일 */
int printer_end(void);

int prtqueue_insert(unsigned char *buf);
/* 프린터 상태 체크 .. 
   return: 상태표시(printer.h 참고)*/
int device_print_check(void);

// sign image 등과같은 특수인쇄시 사용.
int get_bmp_data(unsigned char *path);
int copy_image_data(unsigned char *pdata);
int paper_chk(void);
int ac_position_check(void);
int cover_switch_check(void);
int overhit_check(void);
int black_mark_check(void);
int cutter_switch_check(void);
int auto_cutting(int method);		// 정상 return 1; 	비정상 : 상태표시(printer.h 참고)
int font_open(void);
int font_close(void);
void print_speed(int speed);
//void prt_move_paper(int cnt);
int printer_temperature(void);
void print_stop(void);

/////////////////////////////////////////////////////////////////////////////////////////
// libtimer.c
/////////////////////////////////////////////////////////////////////////////////////////
int timer_open(void);
int timer_close(void);
void release_alltimer(void);

/**
 @biref     ID 타이터를 설정한다.
            id      : 타이머 ID (MAX = 10)
            expires : 설정 시간 (1 sec = 100)
 @return    success(1), fail(0)
 */
char set_apptimer(int id, int expires);
char reset_apptimer(int id, int expires);
char chk_apptimer(int id);
char release_apptimer(int id);
int timer_useful(void);
void timer_version(unsigned char *version);


