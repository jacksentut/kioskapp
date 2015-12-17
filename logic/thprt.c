#include "include.h"

const byte RECEPT_TYPE_01[3][7] = {
	{"카드사1"},	// 0
	{"고  객2"},	// 1
	{"가맹점3"},	// 2
};

#define BMP_FILE_HEADER_SIZE       14
#define BMP_PICTURE_HEADER_SIZE    40

typedef struct {
    char  ch1, ch2;
    long  FILE_SIZE;
    short R1,R2;
    long  HEADER_SIZE;    // real bmp data start position
} BMP_FILE_HEADER;

typedef struct {
    long size;
    long width;           // width
    long height;          // height
    short papers;
    short colorbits;
    long compressed;
    long colordatasize;
    long R1,R2,R3,R4;     
}  BMP_PICTURE_HEADER;

void issue_app_print(void);
void issue_can_print(void);
void temp_can_print(void);
void adjust_print(void);
void adjust_title_print(byte type, byte *ymdsms1, byte *ymdsms2);
void adjust_body_print(byte type, ulong fr, ulong frtm, ulong to, ulong totm);
void adjust_detail_print(byte type, ulong fr, ulong frtm, ulong to, ulong totm);

extern const CP_CODE_LIST cp_code_list[];
extern const CP_CODE_LIST cp_code_list2[];

// Parameters :
//            : dest -> dest string, sour -> source string, width -> width, convert_type -> 역상 or 정상
void convert_bmplogo(byte *dest, byte *sour, long width8, byte convert_type)
{
    byte hhex;
    long ii;

    for(ii=0; ii<width8; ii++)
    {   
        hhex = sour[ii];
        if (convert_type==1) hhex = ~hhex;
        dest[ii] = hhex;
    }
}
//----------------------------------------------------------------------------------------------
// 20130802_NTI 주소 프린트 반복되는 부분이 많아 함수로 전환
// 20140425_KHD 노태일이 주소 프린트 문제있어서 다시 짬
//----------------------------------------------------------------------------------------------
void print_address()
{
    int t=0;
    byte tmp[200+1];
	int i=0, j=0, mok, rest;

    memset(tmp, 0x00, sizeof(tmp));
    memcpy(tmp, Region01.Merchant.retailer_address,strlen(Region01.Merchant.retailer_address));
	mok = strlen(tmp) / 48;
	rest = strlen(tmp) % 48;

	for(i=0; i<mok; i++)
	{
		j = LastHanPosition(&tmp[t], 48);
		if(j > 46)
		{
	        sprintf(tempbuf, "%-47.47s\n", &tmp[t]);
	        prtqueue_insert(tempbuf);
			t += 47;
		}
		else
		{
	        sprintf(tempbuf, "%-48.48s\n", &tmp[t]);
	        prtqueue_insert(tempbuf);
			t += 48;
		}
	}
	// 밀린 값으로 인해 나머지 라인이 2줄이 될 수도 있어서 다시 체크한다.
	mok = strlen(&tmp[t]) / 48;
	rest = strlen(&tmp[t]) % 48;
	for(i=0; i<mok; i++)
	{
		j = LastHanPosition(&tmp[t], 48);
		if(j > 46)
		{
	        sprintf(tempbuf, "%-47.47s\n", &tmp[t]);
	        prtqueue_insert(tempbuf);
			t += 47;
		}
		else
		{
	        sprintf(tempbuf, "%-48.48s\n", &tmp[t]);
	        prtqueue_insert(tempbuf);
			t += 48;
		}
	}
	if(rest)
	{
		sprintf(tempbuf, "%-48.48s\n", &tmp[t]);
        prtqueue_insert(tempbuf);
	}
}
//----------------------------------------------------------------------------------------------
int bmp_print(byte *pass)
{
    FILE *fp1;
    int len;
    unsigned char *buf;
    unsigned char temp[128];
    int real_position, real_size, real_pos;
    BMP_FILE_HEADER BB;
    BMP_PICTURE_HEADER CC;
    byte bmp_image[50000+1] = {0};
    byte convert_type;
    int width8, w_remainder, cnt;

    if ((fp1 = fopen(pass, "rb")) == NULL)
    {
        printf("No BMP file [%s]\n", pass);
        return(1);
    }

	line_print('=');

    fread(temp, 1, BMP_FILE_HEADER_SIZE, fp1);
    memcpy((unsigned char *)&BB.FILE_SIZE,   &temp[2],  4);
    memcpy((unsigned char *)&BB.HEADER_SIZE, &temp[10], 4);
    
    fread(temp, 1, BMP_PICTURE_HEADER_SIZE, fp1);
    memcpy((unsigned char *)&CC.width,  &temp[4], 4);
    memcpy((unsigned char *)&CC.height, &temp[8], 4);
    // BMP_FILE_HEADER 와 BMP_PICTURE_HEADER 의 size만큼 데이타를 읽고난후,bgr(8바이트)만큼 데이타를
    // 더 읽어야 real BMP 데이타값의 시작이다.
    real_position = BB.HEADER_SIZE - (BMP_FILE_HEADER_SIZE + BMP_PICTURE_HEADER_SIZE);
    fread(temp, 1, real_position, fp1);
    
    if (temp[0]==0xff && temp[1]==0xff && temp[2]==0xff)
        convert_type = 0;
    else
        convert_type = 1;

    real_size = BB.FILE_SIZE;
    buf = (char *)malloc(sizeof(char)*real_size+1);

    fclose(fp1);
    fp1 = fopen(pass, "rb");
    len = fread(buf, 1, real_size, fp1);

    memcpy(&bmp_image[0], "\x1D\x76\x30\x00", 4);
    //memcpy(&bmp_image[0], "\x1D\x76\x30\x02", 4);
    width8 = CC.width/8;
    w_remainder = (BB.FILE_SIZE-BB.HEADER_SIZE)/CC.height;
    if (w_remainder > width8) w_remainder -= width8;
    else                      w_remainder = 0;
    memcpy(&bmp_image[4], (byte *)&width8, 2);
    memcpy(&bmp_image[6], (byte *)&CC.height, 2);

    real_pos = BB.HEADER_SIZE + (CC.height * (width8+w_remainder));

//printf("BB.FILE_SIZE is [%ld]\n", BB.FILE_SIZE);
//printf("BB.HEADER_SIZE is [%ld]\n", BB.HEADER_SIZE);
//printf("CC.width is [%ld]\n", CC.width);
//printf("CC.height is [%ld]\n", CC.height);
//printf("width8 is [%d]\n", width8);
//printf("w_remainder is [%d]\n", w_remainder);
//printf("real_pos is [%d]\n", real_pos);

    // BMP DATA는 아래에서부터 width8만큼 차례대로 인쇄해주면 된다.
    for(cnt=0; cnt<CC.height; cnt++)
    {
        real_pos -= (width8 + w_remainder);
        convert_bmplogo(&bmp_image[8+(width8*cnt)], &buf[real_pos], width8, convert_type);
    }

	prtqueue_insert("\x1B\x61\x01"); // 가운데 정렬

    prtqueue_insert(bmp_image);
    fclose(fp1);
    free(buf);
    return(0);
}
void line_print(byte ch)
{
    byte Temp[64] = {0};
    
    memset(Temp, ch, 48);
    Temp[48] = '\n';
    prtqueue_insert(Temp);
}

void teencash_print(void)
{
	sprintf(tempbuf, "%s", "틴캐시는 한게임,넥슨,넷마블,피망,엠게임에서 사용가능\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[상품 취소방법]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "메인화면 > 상품권취소 > 상품권선택 > 발행일자 > 취소번호 > 취소완료\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[유효기간]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "발행일로부터 1년");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[이용문의]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1666-3009");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void nexoncard_print(void)
{
	sprintf(tempbuf, "%s", "넥슨카드는 넥슨(www.nexon.com)에서만 사용 가능\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[상품 취소방법]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "메인화면 > 상품권취소 > 상품권선택 > 발행일자 > 취소번호 > 취소완료\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[유효기간]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "발행일로부터 1년");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[이용문의]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1577-2111");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void book_gift_certificate_print(void)
{
	sprintf(tempbuf, "%s", "본 상품권은 온라인에서만 사용 가능\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[상품 취소방법]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "메인화면 > 상품권취소 > 상품권선택 > 발행일자 > 취소번호 > 취소완료\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[유효기간]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "발행일로부터 1년");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[이용문의]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1544-5111");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void oncash_print(void)
{
	sprintf(tempbuf, "%s", "본 상품권은 온라인에서만 사용 가능\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[상품 취소방법]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "메인화면 > 상품권취소 > 상품권선택 > 발행일자 > 취소번호 > 취소완료\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[유효기간]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "발행일로부터 1년");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[이용문의]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1588-1991");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void funnycard_print(void)
{
	sprintf(tempbuf, "%s", "본 상품권은 온라인에서만 사용 가능\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[상품 취소방법]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "메인화면 > 상품권취소 > 상품권선택 > 발행일자 > 취소번호 > 취소완료\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[유효기간]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "발행일로부터 1년");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[이용문의]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1600-0523");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void itembay_print(void)
{
	sprintf(tempbuf, "%s", "아이템베이(www.itembay.com)에서만 사용 가능\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	sprintf(tempbuf, "%s", "결제수단 'IB선불쿠폰' 선택 후, 카드번호 입력\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "본 상품권은 액면가의 9%를 제외한 금액이 충전됩니다.\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[상품 취소방법]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "메인화면 > 상품권취소 > 상품권선택 > 발행일자 > 취소번호 >");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "고객센터 취소 가능여부 확인 후 통보");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[유효기간]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "발행일로부터 1년");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[이용문의]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1644-3333");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void itemmania_print(void)
{
	sprintf(tempbuf, "%s", "아이템매니아(www.itemmania.com)에서 사용 가능\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	sprintf(tempbuf, "%s", "결제수단 '매니아선불쿠폰' 선택 후, 카드번호 입력\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "본 상품권은 액면가의 9%를 제외한 금액이 충전됩니다.\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[상품 취소방법]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "메인화면 > 상품권취소 > 상품권선택 > 발행일자 > 취소번호 >");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "고객센터 취소 가능여부 확인 후 통보");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[유효기간]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "발행일로부터 1년");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[이용문의]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1666-3009");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void happymoney_print(void)
{
	sprintf(tempbuf, "%s", "본 상품권은 온라인에서만 사용 가능\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[상품 취소방법]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "메인화면 > 상품권취소 > 상품권선택 > 발행일자 > 취소번호 > 취소완료\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[유효기간]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "발행일로부터 1년");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[이용문의]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1588-5245");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void eggmoney_print(void)
{
	sprintf(tempbuf, "%s", "본 상품권은 온라인에서만 사용 가능\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[상품 취소방법]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "메인화면 > 상품권취소 > 상품권선택 > 발행일자 > 취소번호 > 취소완료\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[유효기간]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "발행일로부터 1년");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[이용문의]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1588-3206");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void cultureland_print(void)
{
	sprintf(tempbuf, "%s", "본 상품권은 온라인에서만 사용 가능\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[상품 취소방법]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "메인화면 > 상품권취소 > 상품권선택 > 발행일자 > 취소번호 >");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "고객센터 취소 가능여부 확인 후 통보");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[유효기간]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "발행일로부터 1년");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[이용문의]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1666-3009");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void ncsoft_print(void)
{
	sprintf(tempbuf, "%s", "엔씨소프트(www.plaync.co.kr)에서 사용 가능\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[상품 취소방법]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "메인화면 > 상품권취소 > 상품권선택 > 발행일자 > 취소번호 > 취소완료\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[유효기간]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "발행일로부터 1년");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[이용문의]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1566-0123");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void eyevision_print(void)
{
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[선불폰 충전방법]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "#66 또는 010-5200-0114 입력 > 카드번호 입력\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[상품 취소방법]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "메인화면 > 상품권취소 > 상품권선택 > 발행일자 > 취소번호 > 취소완료\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[유효기간]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "발행일로부터 1년");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[이용문의]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1566-0392");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void freetelecom_print(void)
{
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[선불폰 충전방법]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "■ freeT : ##95+쿠폰번호+통화버튼\n");
	sprintf(tempbuf, "%s", "■ freeC : 080-720-0114+통화+쿠폰번호\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[상품 취소방법]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "메인화면 > 상품권취소 > 상품권선택 > 발행일자 > 취소번호 > 취소완료\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[유효기간]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "발행일로부터 1년");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[이용문의]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1577-4517");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void kt161_print(void)
{
	sprintf(tempbuf, "%s", "사용방법\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	sprintf(tempbuf, "%s", "1.접속번호[Access NO]: 161\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "2.언어선택[Language Select]: 1~9\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "3.카드번호[Card No] + #: 123456789012+#\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "4.국가번호를 포함한 상대방 전화번호(0 제외) + #\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "  [Country Code + Phone No(except 0)] + #\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "  ex) 미국(U.S.A) 1-212-556-2222 + #\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[Customer service]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "080-2680-100");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[Expiration date]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1 year after charge");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void kt00721_print(void)
{
	sprintf(tempbuf, "%s", "사용방법\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	sprintf(tempbuf, "%s", "1.접속번호[Access NO]: 00721\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "2.언어선택[Language Select]: 1~9\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "3.카드번호[Card No] + #: 123456789012+#\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "4.국가번호를 포함한 상대방 전화번호(0 제외) + #\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "  [Country Code + Phone No(except 0)] + #\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "  ex) 미국(U.S.A) 1-212-556-2222 + #\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[Customer service]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "080-2680-100");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[Expiration date]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1 year after charge");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void cp_code_lower_print(int cp_code_index)
{
	int i;
	
	switch(cp_code_list[cp_code_index].int_cp_code)
	{
		case 6: // 퍼니카드
			funnycard_print();
			break;
		case 9: // 해피머니문화상품권(갤럭시아)
			break;
		case 17: // 틴캐시
			teencash_print();
			break;
		case 21: // 아이템베이 선불쿠폰
			itembay_print();
			break;
		case 22: // 아이템매니아 선불쿠폰
			itemmania_print();
			break;
		case 23: // 온캐시
			oncash_print();
			break;
		case 24: //도서상품권
			book_gift_certificate_print();
			break;
		case 25: // 뮤직문화
			break;
		case 26: // NCC
			break;
		case 27: // 해피머니상품권
			happymoney_print();
			break;
		case 30: // 문화상품권(컬쳐랜드)
			cultureland_print();
			break;
		case 41: // 에그머니
			eggmoney_print();
			break;
		case 43: // 넥슨카드
			nexoncard_print();
			break;
		case 45: // 아키에이지
			break;
		case 46: // WOW코인
			break;
		case 47: // N코인
		case 52: // N코인
			ncsoft_print();
			break;
		case 48: // 아이즈비전
			eyevision_print();
			break;
		case 49: // 프리텔레콤(freeT/C)
			freetelecom_print();
			break;
		case 54: // 도서문화상품권(갤럭시아)
		case 2253: // 틴플러스
		case 1512: // LGU+원터치
		case 6714: // K-ONE
		case 349: // 투웨이
		case 6713: // KT탑모바일
		case 3201: // SK브라보
		case 2106: // SK프렌드
			break;
		case 4011: // KT161 Dream
			kt161_print();
			break;
		case 4022: // Olleh Wifi
			break;
		case 4033: // 00721국제전화카드
			kt00721_print();
			break;
	}
}

void trans_print(int ServKind)
{
	t_qdata qdata;
#if 0
	qdata.command_type = qPRINTER;
	qdata.command_code = qPRINT_START;
	qdata.qprint_resp.response_code = 0;
	qdata.qprint_resp.t_field.progress = 0;
	send_msgq(&qdata, 1);
#endif
	sleep(1); // UI 동작 완료를 대기하여, 프린터가 자원을 모두 쓰도록 유도한다.
    switch(ServKind)
    {
        case ISSUE_APP:
        case CHANGE_APP:
            issue_app_print();
			log_printf(1, "발권 인쇄처리 종료");
            break;
        case ISSUE_CAN:
            issue_can_print();
			log_printf(1, "취소 인쇄처리 종료");
            break;
        case TEMP_CANCEL:
            temp_can_print();
			log_printf(1, "부분납 인쇄처리 종료");
            break;
        case ADJUST_APP:
            adjust_print();
            break;
    }
	qdata.command_type = qPRINTER;
	qdata.command_code = qPRINT_START;
	qdata.qprint_resp.response_code = 0;
	qdata.qprint_resp.t_field.progress = 0;
	send_msgq(&qdata, 1);

	qdata.command_type = qPRINTER;
	qdata.command_code = qPRINT_START;
	qdata.qprint_resp.response_code = 0;
	qdata.qprint_resp.t_field.progress = 100;
	send_msgq(&qdata, 1);
}
//----------------------------------------------------------------------------------------------
void issue_app_sms_print(void)
{
    int cp_code_index;
	byte prt_tmp_buf[256];
	int servkind = KIOSK_SERVKIND;
	
	if(print_start(SW_ON, 1)==0) return;
	
	sprintf(prt_tmp_buf, "%cSMS전송완료 영수증%c", SO, SI);
	centerposition(tempbuf, prt_tmp_buf, 48-strlen(prt_tmp_buf), 0x20);
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	line_feed(1);
	line_print('-');
	line_feed(1);
	
	sprintf(tempbuf, "판매점명:%s\n", Region01.Merchant.shop_name);
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "가맹점번호:%-10.10s\n", kField.Receive.resp_shop_serial);
	prtqueue_insert(tempbuf);
	sprintf(prt_tmp_buf, "20%2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s", TimeStamp, &TimeStamp[2], &TimeStamp[4], &TimeStamp[6], &TimeStamp[8], &TimeStamp[10]);
	sprintf(tempbuf, "발행일시:%s\n", prt_tmp_buf);
	prtqueue_insert(tempbuf);
	line_feed(1);
	line_print('-');
	line_feed(1);

	if(servkind == CHANGE_APP)
	{
		cp_code_index = search_cp_code((int)a2l(kField.Send.cp_code, 4));
	}
	else
	{
		cp_code_index = search_cp_code((int)a2l(Region01.Issue_Info.temp_cp_code, 4));
	}
	if(cp_code_index != 10000)
	{
		sprintf(tempbuf, "%c상품명:%s%c\n", SO, cp_code_list[cp_code_index].cp_code_msg, SI);
		prtqueue_insert(tempbuf);
	}

	sprintf(prt_tmp_buf, "%s", kField.Receive.pin_issue_amount[0]);
	erase_space(prt_tmp_buf, 2);
	memset(inbuf, 0x00, sizeof(inbuf));
	comma_ins2(inbuf, 0x20,a2l(prt_tmp_buf, strlen(prt_tmp_buf)));
	erase_space(&inbuf[0], 0);
	sprintf(tempbuf, "%c금액:%s원%c\n", SO, inbuf, SI);
	prtqueue_insert(tempbuf);
	line_feed(1);

	del_space(prt_tmp_buf, kField.Send.client_phone, 16);
	memset(&prt_tmp_buf[5], '*', 4);
	sprintf(tempbuf, "%c전화번호:%-3.3s-%-4.4s-%-4.4s%c\n", SO, &prt_tmp_buf[0], &prt_tmp_buf[3], &prt_tmp_buf[7], SI);
	prtqueue_insert(tempbuf);
	line_feed(1);
	line_print('-');
	line_feed(1);
	prtqueue_insert("요청하신 전화번호로 SMS가 도착하지 않았을 경우 고객센터로 문의 주시기 바랍니다.\n");
	line_feed(1);
	prtqueue_insert("고객센터:1666-3009\n");

	print_end();
}
//----------------------------------------------------------------------------------------------
// 리턴값은 RR의 시작 위치임
//----------------------------------------------------------------------------------------------
int eventmsg_convert(byte *des, byte *src, int len)
{
	byte ret;
    byte nua_temp[16]={0};
	int i=0, j=0;
	for(i=0,j=0;i<len;i++,j++)
	{
        memcpy(nua_temp, nua_temp+1, sizeof(nua_temp)-1);
        nua_temp[sizeof(nua_temp)-1] = src[i];
		if(!memcmp(&nua_temp[sizeof(nua_temp)-2], "NN", 2)) // newline
		{
			des[--j]='\n';
		}
		else if(!memcmp(&nua_temp[sizeof(nua_temp)-2], "RR", 2)) // 
		{
			des[--j] = 0;
			return (i-1);
		}
		else
		{
			des[j]=src[i];
		}
	}
	des[j] = 0;
	return 0;
}
//----------------------------------------------------------------------------------------------
void issue_app_test_print(void)
{
    int i, tcnt, cp_code_index, rr, tt;
	byte order = 0;
	byte prt_tmp_buf[1024];
	byte prt_tmp_buf2[126];


	tcnt = 1;

	if(print_start(SW_ON, 1)==0) return;
	
	// ESC + 'G' + n : n=1 강조, n=0 보통
	prtqueue_insert("\x1B\x47\x01"); // 강조 설정
	centerposition(tempbuf, "온라인 게임은 통캐시 하나면 OK!!", 48, 0x20);
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	prtqueue_insert("\x1B\x47\x00"); // 강조 해제
	prtqueue_insert("\n");
	
	sprintf(tempbuf, "%c%c%c%c%c%c%c%c%c%c", 0x1b,0x57,0x18,0x00,0x00,0x00,0x10,0x02,0x00,0x00); // 42자로 지정
//	    sprintf(tempbuf, "%c%c%c%c%c%c%c%c%c%c", 0x1b,0x57,0x30,0x00,0x00,0x00,0x10,0x02,0x00,0x00); // 40자로 지정
	prtqueue_insert(tempbuf);
	
	sprintf(tempbuf, "%c[EVENT]%c\n", SO, SI);
	prtqueue_insert(tempbuf);

	sprintf(tempbuf, "%s", "아앤플테스트2");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "*** 통캐시, 한게임 결제한도 월 70,000원!!\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	

    sprintf(tempbuf, "%c%c%c%c%c%c%c%c%c%c", 0x1b,0x57,0x00,0x00,0x00,0x00,0x10,0x02,0x00,0x00); // 48자로 복원
	prtqueue_insert(tempbuf);

	line_print('=');
	sprintf(tempbuf, "판매점명:%s\n", "아앤플테스트2");
	prtqueue_insert(tempbuf);
	sprintf(prt_tmp_buf, "20%2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s", "14", "12", "01", "10", "22", "33");
	sprintf(tempbuf, "발행일시:%s\n", prt_tmp_buf);
	prtqueue_insert(tempbuf);

	line_print('=');
	prtqueue_insert("\n");
#if 1
    for(i=0; i<tcnt; i++)
    {
		cp_code_index = search_cp_code(17);
		if(i==0)
		{
			if(cp_code_index != 10000)
			{
				prtqueue_insert("\x1b\x45\x01");
				prtqueue_insert("상 품 명:");
				prtqueue_insert("\x1b\x45\x00");
				sprintf(tempbuf, "%c%s%c\n", SO, "통캐시", SI);
				prtqueue_insert(tempbuf);
			}

			sprintf(prt_tmp_buf, "%s", "10000");
			erase_space(prt_tmp_buf, 2);

			memset(inbuf, 0x00, sizeof(inbuf));
			comma_ins2(inbuf, 0x20,a2l(prt_tmp_buf, strlen(prt_tmp_buf)));
			erase_space(&inbuf[0], 0);
			prtqueue_insert("\x1b\x45\x01");
			prtqueue_insert("금    액:");
			prtqueue_insert("\x1b\x45\x00");
			sprintf(tempbuf, "%c%s원%c\n", SO, inbuf, SI);
			prtqueue_insert(tempbuf);
			prtqueue_insert("\n");
		}

		line_print('-');
		prtqueue_insert("\n");
		sprintf(prt_tmp_buf, "%s", "ABCD-EFGH-IJKL");
		erase_space(prt_tmp_buf, 2);
//			prtqueue_insert("\x1b\x21\x10");
		prtqueue_insert("\x1b\x45\x01");
		prtqueue_insert("카드번호:");
		sprintf(tempbuf, "\n%s\n", prt_tmp_buf);
		prtqueue_insert("\x1b\x21\x30");
		prtqueue_insert(tempbuf);
		prtqueue_insert("\x1b\x45\x00");
		prtqueue_insert("\x1b\x21\x00");
		tt = 17; 
		if(tt==27) // 해피머니인 경우에 해피머니 발행일을 따로 인쇄한다.
		{
			del_space(prt_tmp_buf, kField.Receive.issue_date[i], 10);
			sprintf(prt_tmp_buf2, "해피머니 발행일:%-4.4s-%-2.2s-%-2.2s\n", prt_tmp_buf, &prt_tmp_buf[4], &prt_tmp_buf[6]);
			prtqueue_insert(prt_tmp_buf2);
		}
		prtqueue_insert("\n");

		del_space(prt_tmp_buf, "123456789012", 20);
		//insert_char_at(prt_tmp_buf2, prt_tmp_buf, 4, ' ', 0);
		prtqueue_insert("\x1b\x45\x01");
		prtqueue_insert("관리번호:");
		prtqueue_insert("\x1b\x45\x00");
		sprintf(tempbuf, "%s\n", prt_tmp_buf);
		prtqueue_insert("\x1b\x21\x20");
		prtqueue_insert(tempbuf);
		prtqueue_insert("\x1b\x21\x00");

		
		prtqueue_insert("\x1b\x45\x01");
		prtqueue_insert("취소번호:");
		prtqueue_insert("\x1b\x45\x00");
		if(strlen(prt_tmp_buf) > 8)
		{
			sprintf(prt_tmp_buf2, "%-8.8s\n", &prt_tmp_buf[strlen(prt_tmp_buf)-8]); // 하위 8자리 출력
			if(numeric_check(prt_tmp_buf2, strlen(prt_tmp_buf2))==FALSE)
			{
			    int ii;

			    for(ii=0; ii<8; ii++)
			    {
			        if (!(prt_tmp_buf2[ii]>='0' && prt_tmp_buf2[ii]<='9')) prt_tmp_buf2[ii] = '0';
			    }
			}
			sprintf(tempbuf, "%-8.8s\n", prt_tmp_buf2);
		}
		else
		{
			int ii;
			sprintf(prt_tmp_buf2, "%s", prt_tmp_buf);
		    ii=atoi(prt_tmp_buf2);
			sprintf(tempbuf, "%08d\n", ii);
		}
		insert_char_at(prt_tmp_buf2, tempbuf, 4, ' ', 1);
		sprintf(tempbuf, "%s", prt_tmp_buf2);
		
		prtqueue_insert("\x1b\x21\x20");
		prtqueue_insert(tempbuf);
		prtqueue_insert("\x1b\x21\x00");
		prtqueue_insert("\n");
//			if((i+1) < tcnt)
//				line_print('-');
	}
#endif
	line_print('=');
	prtqueue_insert("\n");

	sprintf(tempbuf, "%s", "사용처: 통L,통게임,통슨,통마블,통망\n [상품 취소방법]\n메인화면 > 상품권취소 > 상품권선택 > 취소번호\n> 발행일자 > 취소완료\n [유효기간] 발행일로부터 1년\n[이용문의] 1666-3009");
	erase_space(tempbuf, 3);
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n\n");
	
//		cp_code_lower_print(cp_code_index);

//		sprintf(tempbuf, "%s", kField.Receive.event_message);
//		erase_space(tempbuf, 3);
//		prtqueue_insert(tempbuf);
	print_end();
}
//----------------------------------------------------------------------------------------------
void issue_app_print(void)
{
    int i, tcnt, cp_code_index, rr, tt;
	byte order = 0;
	byte prt_tmp_buf[1024];
	byte prt_tmp_buf2[126];
    FILE *fp1;
	int servkind = KIOSK_SERVKIND;
#if 0
	tms_check_flag = 1;
	msg_to_ui(qTMS, qTMS_CC, 0);
#endif

	if(kField.Receive.event_message_pos[0] == '1') // 상
	{
		order = 0;
	}
	else if(kField.Receive.event_message_pos[0] == '2') // 하
		order = 1;

	if(kField.Send.issue_type[0]=='2')
	{
		issue_app_sms_print();
		return;
	}

	tcnt = (byte)a2l(kField.Receive.issue_count, 2);
 
    	if(print_start(SW_ON, 1)==0) return;
		
		// ESC + 'G' + n : n=1 강조, n=0 보통
		prtqueue_insert("\x1B\x47\x01"); // 강조 설정
		centerposition(tempbuf, "온라인 게임은 틴캐시 하나면 OK!!", 48, 0x20);
		prtqueue_insert(tempbuf);
		prtqueue_insert("\n");
		prtqueue_insert("\x1B\x47\x00"); // 강조 해제
		prtqueue_insert("\n");
		
		sprintf(tempbuf, "%c%c%c%c%c%c%c%c%c%c", 0x1b,0x57,0x18,0x00,0x00,0x00,0x10,0x02,0x00,0x00); // 42자로 지정
//	    sprintf(tempbuf, "%c%c%c%c%c%c%c%c%c%c", 0x1b,0x57,0x30,0x00,0x00,0x00,0x10,0x02,0x00,0x00); // 40자로 지정
		prtqueue_insert(tempbuf);
		
		sprintf(tempbuf, "%c[EVENT]%c\n", SO, SI);
		prtqueue_insert(tempbuf);

		sprintf(tempbuf, "%s", kField.Receive.event_message);
		erase_space(tempbuf, 3);
		rr = eventmsg_convert(prt_tmp_buf, tempbuf, strlen(tempbuf));
		prtqueue_insert(prt_tmp_buf);
		prtqueue_insert("\n");
		prtqueue_insert("\n");
		
//		sprintf(tempbuf, "%s", "이벤트에 당첨되시면 종로에 '황제노래방' 1시간 이용권을 드립니다.\n말만 잘하면 도우미 1시간 이용권도 제공해 드리겠습니다.\n감사합니다.\n");
//		prtqueue_insert(tempbuf);
//		prtqueue_insert("\n");

	    sprintf(tempbuf, "%c%c%c%c%c%c%c%c%c%c", 0x1b,0x57,0x00,0x00,0x00,0x00,0x10,0x02,0x00,0x00); // 48자로 복원
		prtqueue_insert(tempbuf);

		line_print('=');
		sprintf(tempbuf, "판매점명:%s\n", Region01.Merchant.shop_name);
		prtqueue_insert(tempbuf);
		sprintf(prt_tmp_buf, "20%2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s", TimeStamp, &TimeStamp[2], &TimeStamp[4], &TimeStamp[6], &TimeStamp[8], &TimeStamp[10]);
		sprintf(tempbuf, "발행일시:%s\n", prt_tmp_buf);
		prtqueue_insert(tempbuf);

//		sprintf(tempbuf, "가맹점번호:%-10.10s\n", kField.Receive.resp_shop_serial);
//		prtqueue_insert(tempbuf);
		line_print('=');
		prtqueue_insert("\n");

	    for(i=0; i<tcnt; i++)
	    {
			if(servkind == CHANGE_APP)
			{
				cp_code_index = search_cp_code((int)a2l(kField.Send.cp_code, 4));
			}
			else
			{
				cp_code_index = search_cp_code((int)a2l(Region01.Issue_Info.temp_cp_code, 4));
			}
			if(i==0)
			{
				if(cp_code_index != 10000)
				{
					prtqueue_insert("\x1b\x45\x01");
					prtqueue_insert("상 품 명:");
					prtqueue_insert("\x1b\x45\x00");
					sprintf(tempbuf, "%c%s%c\n", SO, cp_code_list[cp_code_index].cp_code_msg, SI);
					prtqueue_insert(tempbuf);
				}

				sprintf(prt_tmp_buf, "%s", kField.Receive.pin_issue_amount[i]);
				erase_space(prt_tmp_buf, 2);

				memset(inbuf, 0x00, sizeof(inbuf));
				comma_ins2(inbuf, 0x20,a2l(prt_tmp_buf, strlen(prt_tmp_buf)));
				erase_space(&inbuf[0], 0);
				prtqueue_insert("\x1b\x45\x01");
				prtqueue_insert("금    액:");
				prtqueue_insert("\x1b\x45\x00");
				sprintf(tempbuf, "%c%s원%c\n", SO, inbuf, SI);
				prtqueue_insert(tempbuf);
				prtqueue_insert("\n");
			}

			line_print('-');
			prtqueue_insert("\n");
			sprintf(prt_tmp_buf, "%s", kField.Receive.pin_num[i]);
			erase_space(prt_tmp_buf, 2);
//			prtqueue_insert("\x1b\x21\x10");
			prtqueue_insert("\x1b\x45\x01");
			prtqueue_insert("카드번호:");
			sprintf(tempbuf, "\n%s\n", prt_tmp_buf);
			prtqueue_insert("\x1b\x21\x30");
			prtqueue_insert(tempbuf);
			prtqueue_insert("\x1b\x45\x00");
			prtqueue_insert("\x1b\x21\x00");

			if(servkind == CHANGE_APP)
			{
				tt = (int)a2l(kField.Send.cp_code, 4);
			}
			else
			{
				tt = (int)a2l(Region01.Issue_Info.temp_cp_code, 4);
			}
			
			if(tt==27) // 해피머니인 경우에 해피머니 발행일을 따로 인쇄한다.
			{
				del_space(prt_tmp_buf, kField.Receive.issue_date[i], 10);
				sprintf(prt_tmp_buf2, "해피머니 발행일:%-4.4s-%-2.2s-%-2.2s\n", prt_tmp_buf, &prt_tmp_buf[4], &prt_tmp_buf[6]);
				prtqueue_insert(prt_tmp_buf2);
			}
			prtqueue_insert("\n");

			del_space(prt_tmp_buf, kField.Receive.pin_manage_num[i], 20);
			//insert_char_at(prt_tmp_buf2, prt_tmp_buf, 4, ' ', 0);
			prtqueue_insert("\x1b\x45\x01");
			prtqueue_insert("관리번호:");
			prtqueue_insert("\x1b\x45\x00");
			sprintf(tempbuf, "%s\n", prt_tmp_buf);
			prtqueue_insert("\x1b\x21\x20");
			prtqueue_insert(tempbuf);
			prtqueue_insert("\x1b\x21\x00");

			
			prtqueue_insert("\x1b\x45\x01");
			prtqueue_insert("취소번호:");
			prtqueue_insert("\x1b\x45\x00");
			if(strlen(prt_tmp_buf) > 8)
			{
				sprintf(prt_tmp_buf2, "%-8.8s\n", &kField.Receive.pin_manage_num[i][strlen(prt_tmp_buf)-8]); // 하위 8자리 출력
				if(numeric_check(prt_tmp_buf2, strlen(prt_tmp_buf2))==FALSE)
				{
				    int ii;

				    for(ii=0; ii<8; ii++)
				    {
				        if (!(prt_tmp_buf2[ii]>='0' && prt_tmp_buf2[ii]<='9')) prt_tmp_buf2[ii] = '0';
				    }
				}
				sprintf(tempbuf, "%-8.8s\n", prt_tmp_buf2);
			}
			else
			{
				int ii;
				sprintf(prt_tmp_buf2, "%s", kField.Receive.pin_manage_num[i]);
			    ii=atoi(prt_tmp_buf2);
				sprintf(tempbuf, "%08d\n", ii);
			}
			insert_char_at(prt_tmp_buf2, tempbuf, 4, ' ', 1);
			sprintf(tempbuf, "%s", prt_tmp_buf2);
			
			prtqueue_insert("\x1b\x21\x20");
			prtqueue_insert(tempbuf);
			prtqueue_insert("\x1b\x21\x00");
			prtqueue_insert("\n");
//			if((i+1) < tcnt)
//				line_print('-');
		}
		line_print('=');
		prtqueue_insert("\n");

		sprintf(tempbuf, "%s", kField.Receive.event_message);
		erase_space(tempbuf, 3);
		eventmsg_convert(prt_tmp_buf, &tempbuf[rr+2], strlen(&tempbuf[rr+2]));
		prtqueue_insert(prt_tmp_buf);
		prtqueue_insert("\n\n");
	    if ((fp1=fopen("/app/Happy New Year.bmp", "rb")) == NULL)
	    {
		    line_feed(1);
	    }
		else
		{
			fclose(fp1);
			//bmp_print("/app/Happy New Year.bmp");
		}
//		cp_code_lower_print(cp_code_index);
	
//		sprintf(tempbuf, "%s", kField.Receive.event_message);
//		erase_space(tempbuf, 3);
//		prtqueue_insert(tempbuf);
		print_end();
}
//----------------------------------------------------------------------------------------------
void issue_can_print(void)
{
    int i, tcnt, cp_code_index, err_code;
	byte order = 0;
	byte prt_tmp_buf[256];
	byte prt_tmp_buf2[126];

	tcnt = (byte)a2l(kField.Receive.cancel_tcount, 2);
	if(network_cancel_flag==1)
		tcnt = 1;
 
    for(i=0; i<tcnt; i++)
    {
    	if(print_start(SW_ON, 1)==0) return;

		if(network_cancel_flag==1)
		{
			sprintf(prt_tmp_buf, "%c환불영수증%c", SO, SI);
			centerposition(tempbuf, prt_tmp_buf, 38, 0x20);
			prtqueue_insert(tempbuf);
			prtqueue_insert("\n");
			prtqueue_insert("\n");
			prtqueue_insert("\n");
			
			sprintf(tempbuf, "판매점명:%s\n", Region01.Merchant.shop_name);
			prtqueue_insert(tempbuf);

			sprintf(prt_tmp_buf, "20%2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s", TimeStamp, &TimeStamp[2], &TimeStamp[4], &TimeStamp[6], &TimeStamp[8], &TimeStamp[10]);
			sprintf(tempbuf, "발행일시:%s\n", prt_tmp_buf);
			prtqueue_insert(tempbuf);
			prtqueue_insert("\n");
			line_print('-');
			prtqueue_insert("\n");

			sprintf(tempbuf, "망취소에 의한 환불\n");
			prtqueue_insert(tempbuf);
			prtqueue_insert("\n");

			memset(inbuf, 0x00, sizeof(inbuf));
			comma_ins2(inbuf, 0x20, Region01.Issue_Info.bill_input_amount);
			erase_space(&inbuf[0], 0);
			sprintf(tempbuf, "환불금액:%s원\n", inbuf);
			prtqueue_insert("\x1b\x45\x01");
			prtqueue_insert(tempbuf);
			prtqueue_insert("\x1b\x45\x00");
			prtqueue_insert("\n");
			line_print('-');
			prtqueue_insert("\n");

			centerposition(tempbuf, "이용해 주셔서 감사합니다.\n", 48, 0x20);
			prtqueue_insert(tempbuf);
			prtqueue_insert("\n");

			if(Region01.Merchant.vaccount_enable == 1)
			{
				prtqueue_insert("환불영수증을 카운터에 제출하시면 환불금액을 돌려드립니다.\n");
			}
			else
			{
				prtqueue_insert("취소된 금액의 환불은 영업일 기준 24시간 이내에 처리해 드립니다. (주말 및 공휴일의 경우 다음 영업일에 처리됩니다.)\n");
				prtqueue_insert("24시간 이내에 환불이 되지 않을 경우 고객센터로 문의주시기 바랍니다.\n");
			}
			prtqueue_insert("\n");
			prtqueue_insert("\n");
			
			centerposition(tempbuf, "고객센터:1666-3009  www.teencash.co.kr", 48, 0x20);
			prtqueue_insert(tempbuf);
			prtqueue_insert("\n");
		}
		else
		{
			cp_code_index = search_cp_code((int)a2l(kField.Send.cp_code, 4));
			if(cp_code_list[cp_code_index].int_cp_code == 21 || cp_code_list[cp_code_index].int_cp_code == 22)
			{
				sprintf(prt_tmp_buf, "%c취소요청 확인 영수증%c", SO, SI);
				centerposition(tempbuf, prt_tmp_buf, 28, 0x20);
				prtqueue_insert(tempbuf);
				prtqueue_insert("\n");
			}
			else
			{
				sprintf(prt_tmp_buf, "%c취소영수증%c", SO, SI);
				centerposition(tempbuf, prt_tmp_buf, 38, 0x20);
				prtqueue_insert(tempbuf);
				prtqueue_insert("\n");
			}
			prtqueue_insert("\n");
			prtqueue_insert("\n");
			
			sprintf(tempbuf, "판매점명:%s\n", Region01.Merchant.shop_name);
			prtqueue_insert(tempbuf);

			sprintf(prt_tmp_buf, "20%2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s", TimeStamp, &TimeStamp[2], &TimeStamp[4], &TimeStamp[6], &TimeStamp[8], &TimeStamp[10]);
			sprintf(tempbuf, "발행일시:%s\n", prt_tmp_buf);
			prtqueue_insert(tempbuf);
			prtqueue_insert("\n");
			line_print('-');
			prtqueue_insert("\n");

	//		cp_code_index = search_cp_code((int)a2l(kField.Send.cp_code, 4));
			if(cp_code_index != 10000)
			{
				prtqueue_insert("\x1b\x45\x01");
				prtqueue_insert("상 품 명:");
				prtqueue_insert("\x1b\x45\x00");
				sprintf(tempbuf, "%c%s%c\n", SO, cp_code_list[cp_code_index].cp_code_msg, SI);
				prtqueue_insert(tempbuf);
				prtqueue_insert("\n");
			}

			del_space(prt_tmp_buf, kField.Receive.pin_manage_num[i], 20);
			//insert_char_at(prt_tmp_buf2, prt_tmp_buf, 4, ' ', 0);
			prtqueue_insert("\x1b\x45\x01");
			prtqueue_insert("관리번호:");
			prtqueue_insert("\x1b\x45\x00");
			sprintf(tempbuf, "%s\n", prt_tmp_buf);
			prtqueue_insert("\x1b\x21\x20");
			prtqueue_insert(tempbuf);
			prtqueue_insert("\x1b\x21\x00");
			prtqueue_insert("\n");

			sprintf(prt_tmp_buf, "%s", kField.Receive.pin_issue_amount[i]);
			erase_space(prt_tmp_buf, 2);
			memset(inbuf, 0x00, sizeof(inbuf));
			comma_ins2(inbuf, 0x20,a2l(prt_tmp_buf, strlen(prt_tmp_buf)));
			erase_space(&inbuf[0], 0);

			prtqueue_insert("\x1b\x45\x01");
			prtqueue_insert("취소금액:");
			prtqueue_insert("\x1b\x45\x00");
			sprintf(tempbuf, "%c%s원%c\n", SO, inbuf, SI);
			prtqueue_insert(tempbuf);
			prtqueue_insert("\n");
			line_print('-');
			prtqueue_insert("\n");

			if(cp_code_list[cp_code_index].int_cp_code == 21 || cp_code_list[cp_code_index].int_cp_code == 22)
			{
				centerposition(tempbuf, "취소요청 접수가 정상적으로 처리되었습니다.\n", 48, 0x20);
				prtqueue_insert(tempbuf);
				prtqueue_insert("\n");
				prtqueue_insert("본 상품권은 사용여부 확인 후 취소 가능한 상품입니다.\n");
				prtqueue_insert("이미 사용된 상품권일 경우 취소가 불가능합니다.\n");
				prtqueue_insert("사용여부를 확인 후 최대한 빠른 시간 내에 연락(SMS) 드리겠습니다.\n");
				
				if(Region01.Merchant.vaccount_enable == 0)
				{
					prtqueue_insert("환불은 영업일 기준 24시간 이내에 처리해 드립니다. (주말 및 공휴일의 경우 다음 영업일에 처리됩니다.)\n");
				}
				prtqueue_insert("\n");
				centerposition(tempbuf, "고객센터:1666-3009  www.teencash.co.kr", 48, 0x20);
				prtqueue_insert(tempbuf);
				prtqueue_insert("\n");
			}
			else
			{
				centerposition(tempbuf, "이용해 주셔서 감사합니다.\n", 48, 0x20);
				prtqueue_insert(tempbuf);
				prtqueue_insert("\n");
				if(Region01.Merchant.vaccount_enable == 1)
				{
					prtqueue_insert("취소영수증을 카운터에 제출하시면 금액을 돌려드립니다.\n");
					prtqueue_insert("취소된 상품권은 사용이 불가능합니다.\n");
				}
				else
				{
					prtqueue_insert("취소된 금액의 환불은 영업일 기준 24시간 이내에 처리해 드립니다. (주말 및 공휴일의 경우 다음 영업일에 처리됩니다.)\n");
					prtqueue_insert("24시간 이내에 환불이 되지 않을 경우 고객센터로 문의주시기 바랍니다.\n");
				}
				prtqueue_insert("\n");
				prtqueue_insert("\n");
				
				centerposition(tempbuf, "고객센터:1666-3009  www.teencash.co.kr", 48, 0x20);
				prtqueue_insert(tempbuf);
				prtqueue_insert("\n");
			}
		}
		
		print_end();
    }
}
//----------------------------------------------------------------------------------------------
void temp_can_print(void)
{
    int i, tcnt, cp_code_index, err_code;
	byte order = 0;
	byte prt_tmp_buf[256];

	tcnt = (byte)a2l(kField.Receive.cancel_tcount, 2);

 	tcnt = 1;
	aDebugMsg("부분납후 강제종료 실행됨....");
    for(i=0; i<tcnt; i++)
    {
    	if(print_start(SW_ON, 1)==0) return;
		
		sprintf(prt_tmp_buf, "%c환불영수증%c", SO, SI);
		centerposition(tempbuf, prt_tmp_buf, 38, 0x20);
		prtqueue_insert(tempbuf);
		prtqueue_insert("\n");
		prtqueue_insert("\n");
		prtqueue_insert("\n");
		
		sprintf(tempbuf, "판매점명:%s\n", Region01.Merchant.shop_name);
		prtqueue_insert(tempbuf);

		sprintf(prt_tmp_buf, "20%2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s", TimeStamp, &TimeStamp[2], &TimeStamp[4], &TimeStamp[6], &TimeStamp[8], &TimeStamp[10]);
		sprintf(tempbuf, "발행일시:%s\n", prt_tmp_buf);
		prtqueue_insert(tempbuf);
		prtqueue_insert("\n");
		line_print('-');
		prtqueue_insert("\n");

		err_code = (int)a2l(kField.Send.error_type_code, 2);
		switch(err_code)
		{
			case 5:
				sprintf(tempbuf, "입금 대기시간 초과에 의한 환불\n");
				break;
			case 6:
				sprintf(tempbuf, "지폐걸림으로 인한 환불\n");
				break;
			case 7:
				sprintf(tempbuf, "기타 H/W 오류로 인한 환불\n");
				break;
		}
		prtqueue_insert(tempbuf);
		prtqueue_insert("\n");

		memset(inbuf, 0x00, sizeof(inbuf));
		comma_ins2(inbuf, 0x20, Region01.Issue_Info.bill_target_amount);
		erase_space(&inbuf[0], 0);
		sprintf(tempbuf, "구매금액:%s원\n", inbuf);
		prtqueue_insert(tempbuf);
			
		memset(inbuf, 0x00, sizeof(inbuf));
		comma_ins2(inbuf, 0x20, Region01.Issue_Info.bill_input_amount);
		erase_space(&inbuf[0], 0);
		sprintf(tempbuf, "입금금액:%s원\n", inbuf);
		prtqueue_insert(tempbuf);

		if(err_code==5)
		{
			memset(inbuf, 0x00, sizeof(inbuf));
			comma_ins2(inbuf, 0x20, (Region01.Issue_Info.bill_target_amount - Region01.Issue_Info.bill_input_amount));
			erase_space(&inbuf[0], 0);
			sprintf(tempbuf, "미입금금액:%s원\n", inbuf);
			prtqueue_insert(tempbuf);
		}
		
		prtqueue_insert("\n");
		memset(inbuf, 0x00, sizeof(inbuf));
		comma_ins2(inbuf, 0x20, Region01.Issue_Info.bill_input_amount);
		erase_space(&inbuf[0], 0);
		sprintf(tempbuf, "환불금액:%s원\n", inbuf);
		prtqueue_insert("\x1b\x45\x01");
		prtqueue_insert(tempbuf);
		prtqueue_insert("\x1b\x45\x00");
		prtqueue_insert("\n");
		line_print('-');
		prtqueue_insert("\n");

		centerposition(tempbuf, "이용해 주셔서 감사합니다.\n", 48, 0x20);
		prtqueue_insert(tempbuf);
		prtqueue_insert("\n");

		if(Region01.Merchant.vaccount_enable == 1)
		{
			prtqueue_insert("환불영수증을 카운터에 제출하시면 환불금액을 돌려드립니다.\n");
		}
		else
		{
			prtqueue_insert("취소된 금액의 환불은 영업일 기준 24시간 이내에 처리해 드립니다. (주말 및 공휴일의 경우 다음 영업일에 처리됩니다.)\n");
			prtqueue_insert("24시간 이내에 환불이 되지 않을 경우 고객센터로 문의주시기 바랍니다.\n");
		}
		prtqueue_insert("\n");
		prtqueue_insert("\n");
		
		centerposition(tempbuf, "고객센터:1666-3009  www.teencash.co.kr", 48, 0x20);
		prtqueue_insert(tempbuf);
		prtqueue_insert("\n");
		
		print_end();
    }
}
void adjust_print(void)
{
	byte ymdhms1[12+1], ymdhms2[12+1];
	ulong from_date, from_time, to_date, to_time;

	sprintf(ymdhms1, "%2.2s%2.2s%2.2s%2.2s%2.2s%2.2s", &kField.Receive.old_adjust_time[2], &kField.Receive.old_adjust_time[5], &kField.Receive.old_adjust_time[8], &kField.Receive.old_adjust_time[11], &kField.Receive.old_adjust_time[14], &kField.Receive.old_adjust_time[17]);
	sprintf(ymdhms2, "%2.2s%2.2s%2.2s%2.2s%2.2s%2.2s", &kField.Receive.adjust_run_time[2], &kField.Receive.adjust_run_time[5], &kField.Receive.adjust_run_time[8], &kField.Receive.adjust_run_time[11], &kField.Receive.adjust_run_time[14], &kField.Receive.adjust_run_time[17]);

	from_date = a2l(ymdhms1, 6);
	from_time = a2l(&ymdhms1[6], 6);
	to_date = a2l(ymdhms2, 6);
	to_time = a2l(&ymdhms2[6], 6);
//	aDebugMsg("from_date=%d, from_time=%d, to_date=%d, to_time=%d", from_date, from_time, to_date, to_time);
	adjust_title_print(Region01.Merchant.vaccount_enable, ymdhms1, ymdhms2);
	adjust_body_print(Region01.Merchant.vaccount_enable, from_date, from_time, to_date, to_time);
	adjust_detail_print(Region01.Merchant.vaccount_enable, from_date, from_time, to_date, to_time);
	print_end();
}
#if 1
//-----------------------------------------------------------------------------------------------------------------------------
// type: 가상계좌 사용=1, 비사용=0
// ysdsms1: YYYY-MM-DD HH:MM:SS
//-----------------------------------------------------------------------------------------------------------------------------
void adjust_title_print(byte type, byte *ymdsms1, byte *ymdsms2)
{
	byte prt_tmp_buf[256];
	
	print_start(SW_ON, 1);
	
	sprintf(prt_tmp_buf, "%c마감정산 영수증%c", SO, SI);
	centerposition(tempbuf, prt_tmp_buf, 33, 0x20);
	prtqueue_insert(tempbuf);
    line_feed(2);

	sprintf(tempbuf, "판매점명:%s\n", Region01.Merchant.shop_name);
	prtqueue_insert(tempbuf);

	sprintf(tempbuf, "가맹점번호:%s\n", Region01.Merchant.shop_serial);
	prtqueue_insert(tempbuf);
    line_feed(1);

	sprintf(tempbuf, "정산번호:%4.4s%4.4s\n", &TimeStamp[2], &Region01.Merchant.shop_serial[6]); // 정산번호(8자리) = 출력날짜(MMDD) + 가맹점번호 뒤4자리 
	prtqueue_insert(tempbuf);
	sprintf(prt_tmp_buf, "20%2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s", TimeStamp, &TimeStamp[2], &TimeStamp[4], &TimeStamp[6], &TimeStamp[8], &TimeStamp[10]);
	sprintf(tempbuf, "정산일시:%s\n", prt_tmp_buf);
	prtqueue_insert(tempbuf);

	sprintf(tempbuf, "정산기간:20%2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s~20%2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s\n", ymdsms1, &ymdsms1[2], &ymdsms1[4], &ymdsms1[6], &ymdsms1[8], &ymdsms1[10], ymdsms2, &ymdsms2[2], &ymdsms2[4], &ymdsms2[6], &ymdsms2[8], &ymdsms2[10]);
	prtqueue_insert(tempbuf);
    line_feed(1);
	line_print('=');
    line_feed(1);
}

int  app_cnt, can_cnt, temp_can_cnt;				// 발권승인, 발권취소, 부분납취소 Count
ulong app_amt,can_amt,temp_can_amt, net_can_amt;  // 발권승인, 발권취소, 부분납취소, 망취소 금액
//-----------------------------------------------------------------------------------------------------------------------------
// type: 가상계좌 사용=1, 비사용=0
//-----------------------------------------------------------------------------------------------------------------------------
void adjust_body_print(byte type, ulong fr, ulong frtm, ulong to, ulong totm)
{
	byte prt_tmp_buf[256];
	ulong qfront;
	ulong qrear;
	byte temp[CAP_RECORD_SIZE+1];
	byte tymdsms[16]={0};
	int cnt;
	ulong cm, cmtm;
	int servkind;
	ulong amount;

	prtqueue_insert("정산내역\n");
	line_feed(1);
	
	qfront   = Region02.c_qfront;
	qrear	 = Region02.c_qrear;

	aDebugMsg("qfront=%d, qrear=%d", qfront, qrear);
	
	app_cnt=can_cnt=temp_can_cnt=0;
	app_amt=can_amt=temp_can_amt=net_can_amt=0;
	
	capture = c_cap1;              // 집계영역시작번지        
#if 0
    tempbuf[0] = ServKind;                          // 1, SERVKIND
	memcpy(&tempbuf[1], kField.ymdsms, 12); 		// 12, 거래일시
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
		del_space(inbuf, kField.Send.can_amount, 10);
		tmp_amount = a2l(inbuf, strlen(inbuf));
		memcpy(&tempbuf[18],  &tmp_amount, 4); // 4, 거래총금액
		memcpy(&tempbuf[22], kField.Send.pin_date, 8); // 8, 단건취소된 상품권의 발행일자
		memcpy(&tempbuf[30], kField.Send.pin_num, 24); // 24, 핀번호
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
#endif	
	for(cnt=0; cnt<qrear; cnt++)
	{  
		memcpy(temp, capture+(qfront*CAP_RECORD_SIZE), CAP_RECORD_SIZE);

		if (++qfront >= MAXCAP) 
			qfront = 0;

		servkind = temp[0];

		if(!(servkind == ISSUE_APP || servkind == ISSUE_CAN || servkind == TEMP_CANCEL)) continue;
		sprintf(tymdsms, "%-12.12s", &temp[1]);
		cm = a2l(&tymdsms[0], 6);
		if (!(cm >= fr && cm <= to)) continue;
		cmtm = a2l(&tymdsms[6], 6);
		if (cm == fr && cmtm < frtm) continue;
		if (cm == to && cmtm > totm) continue;

		switch(servkind)
		{
			case ISSUE_APP:
				app_cnt++;
				memcpy((byte *)&amount, &temp[19], 4);
				app_amt += amount;
				break;
			case ISSUE_CAN:
				if(servkind == ISSUE_CAN && temp[74]== 2) // 망취소인 경우
				{
					memcpy((byte *)&amount, &temp[18], 4);
					net_can_amt += amount;
				}
				else
				{
					can_cnt++;
					memcpy((byte *)&amount, &temp[18], 4);
					can_amt += amount;
				}
				break;
			case TEMP_CANCEL:
				temp_can_cnt++;
				memcpy((byte *)&amount, &temp[63], 4);
				temp_can_amt += amount;
				break;
		}
	}
	amount = app_amt+temp_can_amt+net_can_amt;
	memset(inbuf, 0x00, sizeof(inbuf));
	comma_ins2(inbuf, 0x20, amount);
	erase_space(&inbuf[0], 3);
	prtqueue_insert("\x1b\x45\x01");
	prtqueue_insert("기기 내 총금액 : ");
	prtqueue_insert("\x1b\x45\x00");
	sprintf(prt_tmp_buf, "%c%s원%c\n", SO, inbuf, SI);
	prtqueue_insert(prt_tmp_buf);
	line_feed(1);

	amount = app_amt;
	memset(inbuf, 0x00, sizeof(inbuf));
	comma_ins2(inbuf, 0x20, amount);
	erase_space(&inbuf[0], 3);
	sprintf(tempbuf, "총승인금액 : %s원", inbuf);
	prtqueue_insert("\x1b\x45\x01");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00\n");
	
	amount = app_amt-can_amt;
	memset(inbuf, 0x00, sizeof(inbuf));
	comma_ins2(inbuf, 0x20, amount);
	erase_space(&inbuf[0], 3);
	sprintf(tempbuf, "정상판매금액 : %s원", inbuf);
	prtqueue_insert("\x1b\x45\x01");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00\n");

	amount = can_amt+temp_can_amt+net_can_amt;
	memset(inbuf, 0x00, sizeof(inbuf));
	comma_ins2(inbuf, 0x20, amount);
	erase_space(&inbuf[0], 3);
	if(type)
		sprintf(tempbuf, "카운터에서 환불한 금액 : %s원", inbuf);
	else
		sprintf(tempbuf, "환불한 금액 : %s원", inbuf);
	prtqueue_insert("\x1b\x45\x01");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00\n");

	amount = can_amt+net_can_amt;
	memset(inbuf, 0x00, sizeof(inbuf));
	comma_ins2(inbuf, 0x20, amount);
	erase_space(&inbuf[0], 3);
	sprintf(tempbuf, "  ① 취소환불금액 : %s원\n", inbuf);
	prtqueue_insert(tempbuf);

	amount = temp_can_amt;
	memset(inbuf, 0x00, sizeof(inbuf));
	comma_ins2(inbuf, 0x20, amount);
	erase_space(&inbuf[0], 3);
	sprintf(tempbuf, "  ② 부분납환불금액 : %s원\n", inbuf);
	prtqueue_insert(tempbuf);

    line_feed(1);
	line_print('=');
    line_feed(1);
}
//-----------------------------------------------------------------------------------------------------------------------------
// type: 가상계좌 사용=1, 비사용=0
//-----------------------------------------------------------------------------------------------------------------------------
void adjust_detail_print(byte type, ulong fr, ulong frtm, ulong to, ulong totm)
{
	byte prt_tmp_buf[256], intemp[128];
	ulong qfront, qrear;
	byte temp[CAP_RECORD_SIZE+1];
	byte tymdsms[16]={0};
	int cnt;
	ulong cm, cmtm;
	int servkind;
	ulong amount;
	int code, seq=1;
	byte first=0;

	qfront   = Region02.c_qfront;
	qrear	 = Region02.c_qrear;

	app_cnt=can_cnt=temp_can_cnt=0;
	app_amt=can_amt=temp_can_amt=0;
	
	capture = c_cap1;              // 집계영역시작번지        

	seq = 1;
	for(cnt=0; cnt<qrear; cnt++)
	{  
		memcpy(temp, capture+(qfront*CAP_RECORD_SIZE), CAP_RECORD_SIZE);

		if (++qfront >= MAXCAP) 
			qfront = 0;

		servkind = temp[0];

		if(!(servkind == ISSUE_CAN)) continue;
		sprintf(tymdsms, "%-12.12s", &temp[1]);
		cm = a2l(&tymdsms[0], 6);
		if (!(cm >= fr && cm <= to)) continue;
		cmtm = a2l(&tymdsms[6], 6);
		if (cm == fr && cmtm < frtm) continue;
		if (cm == to && cmtm > totm) continue;

		if(first==0)
		{
			prtqueue_insert("\x1b\x45\x01");
			prtqueue_insert("취소환불 상세");
			prtqueue_insert("\x1b\x45\x00\n");
			first = 1;
		}
#if 0
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
			memcpy(&tempbuf[18],  &tmp_amount, 4); // 4, 거래총금액
			memcpy(&tempbuf[22], kField.Send.pin_date, 8); // 8, 단건취소된 상품권의 발행일자
			memcpy(&tempbuf[30], kField.Send.pin_num, 24); // 24, 핀번호
			memcpy(&tempbuf[54], kField.Send.pin_manage_num, 20); // 20, 핀관리번호
			tempbuf[74] = (byte)a2l(kField.Send.error_code, 2); // 1, 오류코드
#endif
		code = search_cp_code2(((int)a2l(&temp[14], 4)));
		if(code == 10000)
		{
			if(servkind == ISSUE_CAN && temp[74]== 2) // 망취소인 경우
			{
				sprintf(prt_tmp_buf, "%s", "망취소");
			}
		}
		else
		{
			sprintf(prt_tmp_buf, "%s", cp_code_list2[code].cp_code_msg);
		}
		centerposition(inbuf, prt_tmp_buf, 20, 0x20);

		memcpy((byte *)&amount, &temp[18], 4); // 금액 인쇄
		memset(prt_tmp_buf, 0, sizeof(prt_tmp_buf));
		comma_ins2(prt_tmp_buf, 0x20, amount);
		erase_space(&prt_tmp_buf[0], 0);
		
		sprintf(tempbuf, "%d.20%2.2s/%2.2s/%2.2s %s%10.10s원\n", seq++, &temp[1], &temp[3], &temp[5], inbuf, prt_tmp_buf);
		prtqueue_insert(tempbuf);
	}
	line_feed(1);

	first = 0;
	seq = 1;
	qfront   = Region02.c_qfront;
	qrear	 = Region02.c_qrear;
	for(cnt=0; cnt<qrear; cnt++)
	{  
		memcpy(temp, capture+(qfront*CAP_RECORD_SIZE), CAP_RECORD_SIZE);

		if (++qfront >= MAXCAP) 
			qfront = 0;

		servkind = temp[0];

		if(!(servkind == TEMP_CANCEL)) continue;
		sprintf(tymdsms, "%-12.12s", &temp[1]);
		cm = a2l(&tymdsms[0], 6);
		if (!(cm >= fr && cm <= to)) continue;
		cmtm = a2l(&tymdsms[6], 6);
		if (cm == fr && cmtm < frtm) continue;
		if (cm == to && cmtm > totm) continue;

		if(first==0)
		{
			prtqueue_insert("\x1b\x45\x01");
			prtqueue_insert("부분납환불 상세");
			prtqueue_insert("\x1b\x45\x00\n");
			first = 1;
		}
		memcpy((byte *)&amount, &temp[63], 4);
		memset(prt_tmp_buf, 0, sizeof(prt_tmp_buf));
		comma_ins2(prt_tmp_buf, 0x20, amount);
		erase_space(&prt_tmp_buf[0], 0);
		switch(temp[67])
		{
			case 5:
				sprintf(intemp, "입금대기시간 초과");
				break;
			case 6:
				sprintf(intemp, "지폐 걸림");
				break;
			case 7:
				sprintf(intemp, "하드웨어 장애");
				break;
		}
		centerposition(inbuf, intemp, 20, 0x20);
		sprintf(tempbuf, "%d.20%2.2s/%2.2s/%2.2s %s%10.10s원\n", seq++, &temp[1], &temp[3], &temp[5], inbuf, prt_tmp_buf);
		prtqueue_insert(tempbuf);
	}
	line_feed(1);

	prtqueue_insert("\x1b\x45\x01");
	if(type)
		prtqueue_insert("※ 기기 내 총금액 = 정상판매금액 + 카운터에서 환불한 금액");
	else
		prtqueue_insert("※ 기기 내 총금액 = 정상판매금액 + 환불한 금액");
	prtqueue_insert("\x1b\x45\x00\n");
}
//-----------------------------------------------------------------------------------------------------------------------------
#endif

