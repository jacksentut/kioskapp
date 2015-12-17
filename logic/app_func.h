
/* appmain.c */
void goto_mainstep(void);


/* jtnet.c */
void kiosk_MainLp(void);

/* appinit.c */
void capture_delete(int type);
void app_init(byte type);
void tms_app_init(void);
void AllRegion_Update(void);
void init_val(void);
void tms_app_init(void);

/* applib.c */
void m_delay(int dterm);
byte disprtc(byte type);
void set_cursor(byte pos, byte cchar);
byte get_rangekey(byte frkey, byte tokey);
byte get_rangekey_01(byte frkey, byte tokey, int timeout);
byte get_rangekey_02(byte frkey, byte tokey);
byte get_rangekey_03(byte frkey, byte tokey);
byte get_rangekey_04(byte frkey, byte tokey);
void erase_space(byte *ostr, byte type);
void comma_insert(byte *dest, byte *const_str, ulong value, int type);
void comma_insert1(uchar *dest1, ulong value1, uchar type);
void comma_ins2(u8 *dest, u8 sign, ulong value);
void comma_ins0(u8 *dest, u8 sign, ulong value);
void itoaform(word value);
void ltoaform(ulong value);
word find_FS(byte *dest, byte *sour, word siz);
word find_GS(byte *dest, byte *sour, word siz);
word find_RS(byte *dest, byte *sour, word siz);
word find_US(byte *dest, byte *sour, word siz);
void so_si_delete(byte *ddata);
byte KeyIn(byte dstart,byte *dest,word flag,byte match,byte fr,byte to);
byte KeyIn_2(byte dstart,byte *dest,word flag,byte match,byte fr,byte to);
byte KeyIn_Right(byte dstart,byte *dest,word flag,byte match,byte fr,byte to);
void KeyInRes_Init(void);
byte KeyInRes(byte dstart, byte *dest, word flag, byte match, byte fr, byte to);
byte Appcard_KeyInRes(byte dstart, byte *dest, word flag, byte match, byte fr, byte to, int time);
void MaskFormatCopy(byte *dest);
byte data_cgets(byte *item, byte frkey, byte tokey);
byte data_cgett(byte *item, byte frkey, byte tokey, byte type);
byte data_pdgets(byte *item, byte frkey, byte tokey, byte type);
byte data_pgets(byte pos, byte *item, word siz, word flag, byte fr, byte to, byte wrflag);
byte data_qugets(byte lpos, byte totcnt, byte *sour, byte bkeyon);
byte data_qugets2(byte lpos, byte totcnt, byte *sour, byte bkeyon);
byte data_disp(byte *dsp, byte hflag);
byte data_dgets2(byte lpos, byte totcnt, byte *sour,byte d_value, byte bkeyon);
byte data_dgets3(byte lpos, byte totcnt, byte *sour,byte d_value, byte bkeyon);
byte secret_input(byte siz);
void get_dtm(byte *dtm);
byte set_dtm(byte *time_stamp);
void valid_copy(byte *valid_da, byte *sour);
byte track2_copy(byte *dest, byte *sour);
void A_copy(byte *dest, byte *sour, word siz);
void H_copy(byte *dest, byte *sour, word siz);
byte character_check(byte *dest, byte ch, word siz);
byte numeric_check(byte *dest, word siz);
byte alpha_numeric_check(byte *dest, word siz);
byte h20under_check(byte *dest, word siz);
byte ymdsms_check(byte *sour, byte type);
byte make_lrc(uchar *buf, uint len);
byte xor_sum(uchar *buf, uint len);
void Atoh(byte *dest, byte *src, word siz);
void Htoa(byte *dest, byte *src, word siz);
void aatoh(byte src[],word scnt,word cnt,byte dest[],word dcnt);
int  GetDay(byte *sour);
void calcu_in(byte *dest);
void calcu_out(byte *dest);
void set_str2num(byte *dest, void *sourstr);
void fillzero(uchar *data, int len);
int  decodeEscape(unsigned char* input, unsigned char* output, int in_len);
int  encodeEscape(unsigned char* input, unsigned char* output, int in_len);
byte ip_format_check(byte *ipbuf);
byte ip_format_copy(byte *dest, byte *ipbuf);
unsigned short GetCrc16(unsigned char *buffer, int length);
extern int HexDump( unsigned char* addr, int len );
extern unsigned char CompnStr(unsigned char *s,unsigned char *s1,int Len);
extern void StrnCpy(unsigned char *s,unsigned char *s1,int Len);
extern void TotalAmountMeth(unsigned char *s,unsigned char *s1,unsigned char Sign);
extern void LineDo(unsigned char *s,unsigned char Len);
extern void LineUp(unsigned char *s,unsigned char method,int Len);
extern void LineDoSpaceFirst(unsigned char *s,unsigned char Len);
void dn_arrow_2(unsigned char position, unsigned char disp_type);
void up_arrow_2(unsigned char position, unsigned char disp_type);
int LastHanPosition(byte *ss, int len);
u8 ifconfig_read(u8 *ip, u8 *subnet_mask, u8 *gateway, u8 *dns1, u8 *dns2);
u8 ifconfig_mac_read(u8 *mac_addr);
void centerposition(u8 *dest, u8 *src, u32 len, u8 filler);
void adjust_amount_cal(ulong *in_amount, ulong *app_amount, ulong *can_amount, ulong *net_can_amount, ulong *tempcan_amount, ulong fr, ulong frtm, ulong to, ulong totm);
void sound_init(byte vol);
void sound_play(byte *file);
void log_printf(byte log_level, const char *format, ...);
int file2mem(byte *des, byte *src_file_path, int len);
void play_sound(byte *file);
void insert_char_at(byte *des, byte *src, int pos, byte ch, byte count);
void erase_char(byte *ostr, byte ch);


/* down.c */
void rcv_xmodem_main(void);

/* spcfunc.c */
void mac_address_gets(byte *mac_addr);
void domain_ip_gets(void);
void termset_proc(void);

/* appscen.c */
byte down_load_func(void);                 /* 다운로드 */
byte debit_app_func(void);                 /* 직불카드 승인요청전문 */
byte debit_can_func(void);                 /* 직불카드 승인취소요청전문 */
byte debit_bal_func(void);                 /* 직불카드 잔액조회요청전문 */
byte check_inq_func(void);                 /* 수표조회 */
byte credit_app_func(void);                /* 신용카드 승인요청전문 */
byte credit_can_func(void);                /* 신용카드 승인취소요청전문 */
byte telreg_app_func(void);                /* 신용전화승인등록 */
byte telreg_can_func(void);                /* 신용전화승인등록취소 */
byte cashreg_app_func(void);               /* 현금매출등록 승인 */
byte cashreg_can_func(void);               /* 현금매출등록 취소 */
byte point_app_func(void);                 /* 포인트카드적립요청 */
byte point_can_func(void);                 /* 포인트카드적립취소 */
byte pointuse_app_func(void);              /* 포인트카드사용요청 */
byte pointuse_can_func(void);              /* 포인트카드사용취소 */
byte point_inq_func(void);                 /* 포인트카드 확인요청  */
byte mbship_app_func(void);                /* 멤버쉽승인요청 */
byte mbship_can_func(void);                /* 멤버쉽취소요청 */
byte etax_app_func(void);                  /* 전자세금계산서 발행 */
byte emvcredit_app_func(void);             /* EMV 신용카드 승인요청전문 */
byte emvcredit_can_func(void);             /* EMV 신용카드 승인취소요청전문 */
byte cash_app_func(void);                  /* 현금매출영수증출력 (전문없슴) */
byte cash_can_func(void);                  /* 현금매출영수증취소 (전문없슴) */
byte dailyjungsan_inq_func(void);          /* 일일정산요청*/
byte tmscheck_inq_func(void);       		/* TMS CHECK 요청*/
byte ficode_inq_func(void);                /* FI CODE 요청*/
byte settle_cdt_func(void);                /* 신용카드 수동정산요청*/

byte No_app_func(void);

/* chshic.c */
extern byte CashIc_app_func(void);				/* 현금IC 승인 */
extern byte CashIc_can_func(void);				/* 현금IC 취소 */
extern byte CashIc_bal_func(void);				/* 현금IC 잔액조회 */
extern byte despin_dec_func(byte *encryptdata);

/* appsub.c */
int debugstrs(unsigned char* indata, int datalen, unsigned char* dispmsg, int kind);
int debugstr(unsigned char* indata, int datalen, unsigned char* dispmsg, int kind);
void serv_title_display(int ServKind);
byte sailer_input_func(int ServKind);
void sailer_regster_func(void);
byte freeoilarea_input_func(int ServKind);
byte freeoillitter_input_func(int ServKind);
byte creditin_input_func(int ServKind);
byte tms_check(void);
byte pumok_input_func(int ServKind);
byte freeoiltype_input_func(int ServKind);
byte amount_input_func(int ServKind, byte type);
byte optional_cost_input_func(int ServKind);
byte tax_input_func(int ServKind);
byte taxfree_input_func(int ServKind);
byte fee_input_func(int ServKind);
byte cashamount_input_func(int ServKind);
byte ins_input_func(int ServKind);
byte seqno_input_func(int ServKind);
void seqno_reload(void);
void seqno_func(int SercKind, byte *dest);
void seqno_update(int SercKind, byte *SeqStr);
byte sangcode_input_func(int ServKind);
void ficode_copy(byte *fi_sour, int size, byte *fi_dest);
void fi_name_copy(byte *dest, byte *sour);
byte valid_date_input(int ServKind);
byte cardnum_input(int ServKind);
byte cardnum_input_01(int ServKind);
byte pm2000_cardnum_input(void);
byte appcard_cardnum_input_error(int ServKind, byte *pad_enable);
byte appcard_cardnum_input(int ServKind);
byte etax_cardnum_input(int ServKind, byte ucType);
byte cardnum_input_02(int ServKind);
byte personal_input(int ServKind, byte type);
byte personal_input_pos(byte type);
byte orgdate_input_func(int ServKind);
byte orgappno_input_func(int ServKind);
byte emvcard_yn(byte *dest);
byte fallback_cdnum_input(int ServKind);
byte pin_device_input(int ServKind, byte type);
byte sign_device_input(int ServKind);
void cat_infoset_func(byte *catinfo);
byte pointpin_device_input(int ServKind);
byte hanwhapin_device_input(int ServKind);
byte daeback_pindevice_input(int ServKind);
byte point_trans_choice(void);
byte CMD32_set_func(ulong ulAmt, double dDanga);
byte yymmdd_check(byte ucTitleNum, int iDays);
byte yymmdd_check2(int iDays, byte *ucInputDate, byte *ucDeadlineDate);
void capture_cnt_check(void);

void app_msg_display(int ServKind);
void reject_msg_display(int ServKind);
void bankapp_msg_display(int ServKind);
void bankreject_msg_display(int ServKind);
byte cauto_input(void);
byte daily_limit_check(byte *track2_data, byte *ymdsms);
byte marutns_check(byte *track2);
byte umoney_check(byte *track2);
void tms_receive_func(void);
byte bfr_cancle_func(int ServKind);
void reprt_func(void);
byte bfrcan_func(void);
byte cashreg_cancel_reason_func(int ServKind); // 20111119_TJH_004
byte cardnum_disp_func(int ServKind);
byte payment_disp_func(int ServKind);
byte credit_can_confirm_func(int ServKind);

/* msghndlr.c */
void cdma_predial(int cnt);
byte ethernet_connect(void);
void get_dialno(void);
void disconnect(void);
void pre_dialing(void);
void host_send_dle(int ServKind);
byte host_msganal(int ServKind, int type);
byte emv_host_msganal(int ServKind, int type);
byte bank_host_msganal(int ServKind, int type);
byte host_connect(int ServKind, int type);
byte host_recv_enq(int ServKind);
byte host_send_recv(int ServKind, byte ResendData, int type);
void host_send_ack(int ServKind);
void host_send_dle(int ServKind);
byte host_recv_eot(int ServKind);
//----------------------------------------------------------------------------------------------
// 20130802_NTI 주소 프린트 반복되는 부분이 많아 함수로 전환
// 20130801_NTI parameter로 받은 길이 넘을 경우 2줄로 나누는 함수 from info-310_nor
//----------------------------------------------------------------------------------------------
//void make_address(byte *new_address , byte *org_address, byte len);
int make_address(byte *new_address , byte *org_address, byte len);
void print_address();
//----------------------------------------------------------------------------------------------
byte host_msganal_not_EOT(int ServKind);


/* msganal.c */
int packet_common_paste(int ServKind, byte *CMNBuf, byte *dest);
int packet_paste(int ServKind, byte *CommandBuf, byte *dest);
void make_packet(int ServKind);
void packet_phase(int ServKind, byte *Tbuf, int buflen);
void down_packet_phase(int ServKind, byte *Tbuf, int buflen);
byte packet_anal(int ServKind, int type);
void aPrtSpeedUp(void);

/* pdevice.c */
byte despin_cat_func(int ServKind, byte *encryptdata);
void send2pin(byte FC, byte CODE, byte *sour, word len);
void pinpad_init(void);
void personal_input_request(void);
byte pin_gets(byte *temp);
byte pinpad_check(void);
byte pinpad_PIN_gets(byte kind, byte *encryptdata);
byte ack_packet_gets(void);
void pindisp_proc(byte type);
void send2sign(byte Cmd, void *sour, word len);
byte signpad_gets(word *sign_len, byte *sign_data, byte *code_version);
byte signpad_check(void);
byte barcodepad_print(void);
void barcode_input_request(void);
byte appcard_signpad_gets(byte *Step, byte *resp_data, byte *resp_len);
byte sign_device_input_pos(void);
byte signpad_gets_pos(word *sign_len, byte *sign_data, byte *code_version);

/* datacap.c */
void cicode_update(byte *ci_code, byte *issuer_name);
void ficode_update(byte *fi_code, byte *buying_bank_name);
void bfrdata_clear(void);
void bfrdata_save(int ServKind);
void trans_capture(int ServKind);

/* tprt.c */
void down_load_print(void);
void credit_app_print(void);
void debit_app_print(void);
void telreg_app_print(void);
void emvcredit_app_print(void);
void cashreg_app_print(void);
void credit_can_print(void);
void debit_can_print(void);
void telreg_can_print(void);
void emvcredit_can_print(void);
void cashreg_can_print(void);
void trans_print(int ServKind);
void trans_simple_print(int ServKind);

/* selftest.c */
void prg_dnload(void);
void spy_proc(void);
void selftest(int type);
void port_menu(void);
void send_program(void);
byte msr_test(void);
void prt1_test(word i);

/* emvl2.c */
byte mkicc_lrc(byte *buf, uint len);
void mrcpy(byte *dest, byte *sour, word size);
byte get_numkey(byte frkey, byte tokey);
byte get_EntCan(void);
byte gen_rand(byte min, byte max);
void disp_delay(word time);
void n2h(byte *sbuf, byte *dbuf, byte len);
void h2a(byte *src, byte *dest, byte len);
void hexcpy(byte *dest, byte *sour, byte len, byte type);
byte TLV_Check(word rlen, byte type);
void EmvOth_Set(void);
void EmvCfg_init(void);
void Set_Term(byte *RTC_input);
void TermCfg(void);
void emv_data_make(int ServKind);
byte EMVL2(void);

/* tdkicc.c */
void TDK_module_init(void);
byte TDK_select_psam(byte psam_no);
byte TDK_psam_ATR(void);
void TDK_psam_PWR_down(void);
u16  calcrcXX(byte *ptr, int len);
void TDK_port_shift(void);
void TDK_send_ACK(void);
void TDK_send_NAK(void);
void TDK_send_packet(byte *p, int len);
int  TDK_recv_ANAK(byte *p, word tm);
int  TDK_recv_packet(byte *p, int len, word tm);
byte TDK_icc_gbpsr(word pos);
byte chk_TDK_CORE(void);
byte init_TDK_CORE(void);
byte TDK_ATR(void);
void TDK_PWR_down(void);
byte init_iccsio(void);
byte chk_insert(byte mode);
int  TDK_cr_apdu(byte SLOT, word len);

/* thdprt.c */
void line_print(byte ch);
void capture_erase_func(void);
byte jungsan_proc(void);
void credit_jungsan_proc(void);
byte daily_jungsan_proc(byte detail_type);
byte jungsan_set_func(void);


/* seed2.c */
void SeedPIN(short nLen, byte *AccNoInData, byte *PinBlockData , byte *cCurntDate);

/* shopatm.c */
void shop_atm_init(void);
byte shop_atm_reload(byte type);
void shop_atm_func(void);
void shop_atm_menu(void);


/* forpos.c */
byte pos_host_msganal(int ServKind);
void pos_check(void);
int pos_process(int kind);
void pos_data_send02(int Trans, byte types);

/* spcfunc.c */
byte program_info_func(void);

int EncryptData(unsigned char* pbInputData, unsigned char* pbKey, int nSize, unsigned char* pbOutputData);
int DecryptData(unsigned char* pbInputData, unsigned char* pbKey, int nSize, unsigned char* pbOutputData);

void lcd_level(void);
byte prtpaper_chk(void);


/* md5.c */
void MD5File(char *filename, unsigned char *ucDest);

/* thprt.c */
extern void cashic_app_print(void);
extern void cashic_can_print(void);
extern void cashic_bal_print(void);

/* lanpos.c */
void AO_socket_init(void);
void AO_socket_close(void);
void AO_client_close(byte kind);
int  AO_socket_open(void);
byte AO_ComPort_Ready(void);
byte AO_ComPort_Getc(void);
void AO_ComPort_Write(void *Txdat, int len);
void AO_ComPort_Putc(char Txch);
void aDebugMsg(const char *format, ...);
void lanpos_print_check(void);
byte lanpos_print_process(int prtstep, byte pdata);

// msgq.c
int search_cp_code(int cp_code);
void del_space(u8 *des, u8 *src, int len);

int bill_init(void);
void send2bill(byte *cmd, byte data);
void send_msg_cancel(void);
void msg_to_ui(int cmd_type, int cmd_code, int response);

// apptms.c
byte tms_check(void);
byte tms_update_check(void);



