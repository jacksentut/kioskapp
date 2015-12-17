#include "include.h"

const byte RECEPT_TYPE_01[3][7] = {
	{"ī���1"},	// 0
	{"��  ��2"},	// 1
	{"������3"},	// 2
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
//            : dest -> dest string, sour -> source string, width -> width, convert_type -> ���� or ����
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
// 20130802_NTI �ּ� ����Ʈ �ݺ��Ǵ� �κ��� ���� �Լ��� ��ȯ
// 20140425_KHD �������� �ּ� ����Ʈ �����־ �ٽ� «
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
	// �и� ������ ���� ������ ������ 2���� �� ���� �־ �ٽ� üũ�Ѵ�.
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
    // BMP_FILE_HEADER �� BMP_PICTURE_HEADER �� size��ŭ ����Ÿ�� �а���,bgr(8����Ʈ)��ŭ ����Ÿ��
    // �� �о�� real BMP ����Ÿ���� �����̴�.
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

    // BMP DATA�� �Ʒ��������� width8��ŭ ���ʴ�� �μ����ָ� �ȴ�.
    for(cnt=0; cnt<CC.height; cnt++)
    {
        real_pos -= (width8 + w_remainder);
        convert_bmplogo(&bmp_image[8+(width8*cnt)], &buf[real_pos], width8, convert_type);
    }

	prtqueue_insert("\x1B\x61\x01"); // ��� ����

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
	sprintf(tempbuf, "%s", "ƾĳ�ô� �Ѱ���,�ؽ�,�ݸ���,�Ǹ�,�����ӿ��� ��밡��\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ǰ ��ҹ��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "����ȭ�� > ��ǰ����� > ��ǰ�Ǽ��� > �������� > ��ҹ�ȣ > ��ҿϷ�\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ȿ�Ⱓ]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "�����Ϸκ��� 1��");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[�̿빮��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1666-3009");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void nexoncard_print(void)
{
	sprintf(tempbuf, "%s", "�ؽ�ī��� �ؽ�(www.nexon.com)������ ��� ����\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ǰ ��ҹ��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "����ȭ�� > ��ǰ����� > ��ǰ�Ǽ��� > �������� > ��ҹ�ȣ > ��ҿϷ�\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ȿ�Ⱓ]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "�����Ϸκ��� 1��");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[�̿빮��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1577-2111");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void book_gift_certificate_print(void)
{
	sprintf(tempbuf, "%s", "�� ��ǰ���� �¶��ο����� ��� ����\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ǰ ��ҹ��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "����ȭ�� > ��ǰ����� > ��ǰ�Ǽ��� > �������� > ��ҹ�ȣ > ��ҿϷ�\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ȿ�Ⱓ]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "�����Ϸκ��� 1��");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[�̿빮��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1544-5111");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void oncash_print(void)
{
	sprintf(tempbuf, "%s", "�� ��ǰ���� �¶��ο����� ��� ����\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ǰ ��ҹ��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "����ȭ�� > ��ǰ����� > ��ǰ�Ǽ��� > �������� > ��ҹ�ȣ > ��ҿϷ�\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ȿ�Ⱓ]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "�����Ϸκ��� 1��");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[�̿빮��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1588-1991");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void funnycard_print(void)
{
	sprintf(tempbuf, "%s", "�� ��ǰ���� �¶��ο����� ��� ����\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ǰ ��ҹ��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "����ȭ�� > ��ǰ����� > ��ǰ�Ǽ��� > �������� > ��ҹ�ȣ > ��ҿϷ�\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ȿ�Ⱓ]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "�����Ϸκ��� 1��");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[�̿빮��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1600-0523");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void itembay_print(void)
{
	sprintf(tempbuf, "%s", "�����ۺ���(www.itembay.com)������ ��� ����\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	sprintf(tempbuf, "%s", "�������� 'IB��������' ���� ��, ī���ȣ �Է�\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "�� ��ǰ���� �׸鰡�� 9%�� ������ �ݾ��� �����˴ϴ�.\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ǰ ��ҹ��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "����ȭ�� > ��ǰ����� > ��ǰ�Ǽ��� > �������� > ��ҹ�ȣ >");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "������ ��� ���ɿ��� Ȯ�� �� �뺸");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ȿ�Ⱓ]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "�����Ϸκ��� 1��");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[�̿빮��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1644-3333");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void itemmania_print(void)
{
	sprintf(tempbuf, "%s", "�����۸ŴϾ�(www.itemmania.com)���� ��� ����\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	sprintf(tempbuf, "%s", "�������� '�ŴϾƼ�������' ���� ��, ī���ȣ �Է�\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "�� ��ǰ���� �׸鰡�� 9%�� ������ �ݾ��� �����˴ϴ�.\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ǰ ��ҹ��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "����ȭ�� > ��ǰ����� > ��ǰ�Ǽ��� > �������� > ��ҹ�ȣ >");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "������ ��� ���ɿ��� Ȯ�� �� �뺸");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ȿ�Ⱓ]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "�����Ϸκ��� 1��");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[�̿빮��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1666-3009");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void happymoney_print(void)
{
	sprintf(tempbuf, "%s", "�� ��ǰ���� �¶��ο����� ��� ����\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ǰ ��ҹ��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "����ȭ�� > ��ǰ����� > ��ǰ�Ǽ��� > �������� > ��ҹ�ȣ > ��ҿϷ�\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ȿ�Ⱓ]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "�����Ϸκ��� 1��");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[�̿빮��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1588-5245");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void eggmoney_print(void)
{
	sprintf(tempbuf, "%s", "�� ��ǰ���� �¶��ο����� ��� ����\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ǰ ��ҹ��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "����ȭ�� > ��ǰ����� > ��ǰ�Ǽ��� > �������� > ��ҹ�ȣ > ��ҿϷ�\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ȿ�Ⱓ]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "�����Ϸκ��� 1��");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[�̿빮��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1588-3206");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void cultureland_print(void)
{
	sprintf(tempbuf, "%s", "�� ��ǰ���� �¶��ο����� ��� ����\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ǰ ��ҹ��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "����ȭ�� > ��ǰ����� > ��ǰ�Ǽ��� > �������� > ��ҹ�ȣ >");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "������ ��� ���ɿ��� Ȯ�� �� �뺸");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ȿ�Ⱓ]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "�����Ϸκ��� 1��");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[�̿빮��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1666-3009");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void ncsoft_print(void)
{
	sprintf(tempbuf, "%s", "��������Ʈ(www.plaync.co.kr)���� ��� ����\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ǰ ��ҹ��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "����ȭ�� > ��ǰ����� > ��ǰ�Ǽ��� > �������� > ��ҹ�ȣ > ��ҿϷ�\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ȿ�Ⱓ]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "�����Ϸκ��� 1��");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[�̿빮��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1566-0123");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void eyevision_print(void)
{
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[������ �������]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "#66 �Ǵ� 010-5200-0114 �Է� > ī���ȣ �Է�\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ǰ ��ҹ��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "����ȭ�� > ��ǰ����� > ��ǰ�Ǽ��� > �������� > ��ҹ�ȣ > ��ҿϷ�\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ȿ�Ⱓ]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "�����Ϸκ��� 1��");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[�̿빮��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1566-0392");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void freetelecom_print(void)
{
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[������ �������]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "�� freeT : ##95+������ȣ+��ȭ��ư\n");
	sprintf(tempbuf, "%s", "�� freeC : 080-720-0114+��ȭ+������ȣ\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ǰ ��ҹ��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "����ȭ�� > ��ǰ����� > ��ǰ�Ǽ��� > �������� > ��ҹ�ȣ > ��ҿϷ�\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");

	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[��ȿ�Ⱓ]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "�����Ϸκ��� 1��");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	prtqueue_insert("\x1b\x45\x01");
	sprintf(tempbuf, "%s", "[�̿빮��]");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00");
	sprintf(tempbuf, "%s", "1577-4517");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
}

void kt161_print(void)
{
	sprintf(tempbuf, "%s", "�����\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	sprintf(tempbuf, "%s", "1.���ӹ�ȣ[Access NO]: 161\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "2.����[Language Select]: 1~9\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "3.ī���ȣ[Card No] + #: 123456789012+#\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "4.������ȣ�� ������ ���� ��ȭ��ȣ(0 ����) + #\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "  [Country Code + Phone No(except 0)] + #\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "  ex) �̱�(U.S.A) 1-212-556-2222 + #\n");
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
	sprintf(tempbuf, "%s", "�����\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	
	sprintf(tempbuf, "%s", "1.���ӹ�ȣ[Access NO]: 00721\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "2.����[Language Select]: 1~9\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "3.ī���ȣ[Card No] + #: 123456789012+#\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "4.������ȣ�� ������ ���� ��ȭ��ȣ(0 ����) + #\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "  [Country Code + Phone No(except 0)] + #\n");
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "%s", "  ex) �̱�(U.S.A) 1-212-556-2222 + #\n");
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
		case 6: // �۴�ī��
			funnycard_print();
			break;
		case 9: // ���ǸӴϹ�ȭ��ǰ��(�����þ�)
			break;
		case 17: // ƾĳ��
			teencash_print();
			break;
		case 21: // �����ۺ��� ��������
			itembay_print();
			break;
		case 22: // �����۸ŴϾ� ��������
			itemmania_print();
			break;
		case 23: // ��ĳ��
			oncash_print();
			break;
		case 24: //������ǰ��
			book_gift_certificate_print();
			break;
		case 25: // ������ȭ
			break;
		case 26: // NCC
			break;
		case 27: // ���ǸӴϻ�ǰ��
			happymoney_print();
			break;
		case 30: // ��ȭ��ǰ��(���ķ���)
			cultureland_print();
			break;
		case 41: // ���׸Ӵ�
			eggmoney_print();
			break;
		case 43: // �ؽ�ī��
			nexoncard_print();
			break;
		case 45: // ��Ű������
			break;
		case 46: // WOW����
			break;
		case 47: // N����
		case 52: // N����
			ncsoft_print();
			break;
		case 48: // ���������
			eyevision_print();
			break;
		case 49: // �����ڷ���(freeT/C)
			freetelecom_print();
			break;
		case 54: // ������ȭ��ǰ��(�����þ�)
		case 2253: // ƾ�÷���
		case 1512: // LGU+����ġ
		case 6714: // K-ONE
		case 349: // ������
		case 6713: // KTž�����
		case 3201: // SK���
		case 2106: // SK������
			break;
		case 4011: // KT161 Dream
			kt161_print();
			break;
		case 4022: // Olleh Wifi
			break;
		case 4033: // 00721������ȭī��
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
	sleep(1); // UI ���� �ϷḦ ����Ͽ�, �����Ͱ� �ڿ��� ��� ������ �����Ѵ�.
    switch(ServKind)
    {
        case ISSUE_APP:
        case CHANGE_APP:
            issue_app_print();
			log_printf(1, "�߱� �μ�ó�� ����");
            break;
        case ISSUE_CAN:
            issue_can_print();
			log_printf(1, "��� �μ�ó�� ����");
            break;
        case TEMP_CANCEL:
            temp_can_print();
			log_printf(1, "�κг� �μ�ó�� ����");
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
	
	sprintf(prt_tmp_buf, "%cSMS���ۿϷ� ������%c", SO, SI);
	centerposition(tempbuf, prt_tmp_buf, 48-strlen(prt_tmp_buf), 0x20);
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	line_feed(1);
	line_print('-');
	line_feed(1);
	
	sprintf(tempbuf, "�Ǹ�����:%s\n", Region01.Merchant.shop_name);
	prtqueue_insert(tempbuf);
	sprintf(tempbuf, "��������ȣ:%-10.10s\n", kField.Receive.resp_shop_serial);
	prtqueue_insert(tempbuf);
	sprintf(prt_tmp_buf, "20%2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s", TimeStamp, &TimeStamp[2], &TimeStamp[4], &TimeStamp[6], &TimeStamp[8], &TimeStamp[10]);
	sprintf(tempbuf, "�����Ͻ�:%s\n", prt_tmp_buf);
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
		sprintf(tempbuf, "%c��ǰ��:%s%c\n", SO, cp_code_list[cp_code_index].cp_code_msg, SI);
		prtqueue_insert(tempbuf);
	}

	sprintf(prt_tmp_buf, "%s", kField.Receive.pin_issue_amount[0]);
	erase_space(prt_tmp_buf, 2);
	memset(inbuf, 0x00, sizeof(inbuf));
	comma_ins2(inbuf, 0x20,a2l(prt_tmp_buf, strlen(prt_tmp_buf)));
	erase_space(&inbuf[0], 0);
	sprintf(tempbuf, "%c�ݾ�:%s��%c\n", SO, inbuf, SI);
	prtqueue_insert(tempbuf);
	line_feed(1);

	del_space(prt_tmp_buf, kField.Send.client_phone, 16);
	memset(&prt_tmp_buf[5], '*', 4);
	sprintf(tempbuf, "%c��ȭ��ȣ:%-3.3s-%-4.4s-%-4.4s%c\n", SO, &prt_tmp_buf[0], &prt_tmp_buf[3], &prt_tmp_buf[7], SI);
	prtqueue_insert(tempbuf);
	line_feed(1);
	line_print('-');
	line_feed(1);
	prtqueue_insert("��û�Ͻ� ��ȭ��ȣ�� SMS�� �������� �ʾ��� ��� �����ͷ� ���� �ֽñ� �ٶ��ϴ�.\n");
	line_feed(1);
	prtqueue_insert("������:1666-3009\n");

	print_end();
}
//----------------------------------------------------------------------------------------------
// ���ϰ��� RR�� ���� ��ġ��
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
	
	// ESC + 'G' + n : n=1 ����, n=0 ����
	prtqueue_insert("\x1B\x47\x01"); // ���� ����
	centerposition(tempbuf, "�¶��� ������ ��ĳ�� �ϳ��� OK!!", 48, 0x20);
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	prtqueue_insert("\x1B\x47\x00"); // ���� ����
	prtqueue_insert("\n");
	
	sprintf(tempbuf, "%c%c%c%c%c%c%c%c%c%c", 0x1b,0x57,0x18,0x00,0x00,0x00,0x10,0x02,0x00,0x00); // 42�ڷ� ����
//	    sprintf(tempbuf, "%c%c%c%c%c%c%c%c%c%c", 0x1b,0x57,0x30,0x00,0x00,0x00,0x10,0x02,0x00,0x00); // 40�ڷ� ����
	prtqueue_insert(tempbuf);
	
	sprintf(tempbuf, "%c[EVENT]%c\n", SO, SI);
	prtqueue_insert(tempbuf);

	sprintf(tempbuf, "%s", "�ƾ����׽�Ʈ2");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	sprintf(tempbuf, "%s", "*** ��ĳ��, �Ѱ��� �����ѵ� �� 70,000��!!\n");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\n");
	

    sprintf(tempbuf, "%c%c%c%c%c%c%c%c%c%c", 0x1b,0x57,0x00,0x00,0x00,0x00,0x10,0x02,0x00,0x00); // 48�ڷ� ����
	prtqueue_insert(tempbuf);

	line_print('=');
	sprintf(tempbuf, "�Ǹ�����:%s\n", "�ƾ����׽�Ʈ2");
	prtqueue_insert(tempbuf);
	sprintf(prt_tmp_buf, "20%2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s", "14", "12", "01", "10", "22", "33");
	sprintf(tempbuf, "�����Ͻ�:%s\n", prt_tmp_buf);
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
				prtqueue_insert("�� ǰ ��:");
				prtqueue_insert("\x1b\x45\x00");
				sprintf(tempbuf, "%c%s%c\n", SO, "��ĳ��", SI);
				prtqueue_insert(tempbuf);
			}

			sprintf(prt_tmp_buf, "%s", "10000");
			erase_space(prt_tmp_buf, 2);

			memset(inbuf, 0x00, sizeof(inbuf));
			comma_ins2(inbuf, 0x20,a2l(prt_tmp_buf, strlen(prt_tmp_buf)));
			erase_space(&inbuf[0], 0);
			prtqueue_insert("\x1b\x45\x01");
			prtqueue_insert("��    ��:");
			prtqueue_insert("\x1b\x45\x00");
			sprintf(tempbuf, "%c%s��%c\n", SO, inbuf, SI);
			prtqueue_insert(tempbuf);
			prtqueue_insert("\n");
		}

		line_print('-');
		prtqueue_insert("\n");
		sprintf(prt_tmp_buf, "%s", "ABCD-EFGH-IJKL");
		erase_space(prt_tmp_buf, 2);
//			prtqueue_insert("\x1b\x21\x10");
		prtqueue_insert("\x1b\x45\x01");
		prtqueue_insert("ī���ȣ:");
		sprintf(tempbuf, "\n%s\n", prt_tmp_buf);
		prtqueue_insert("\x1b\x21\x30");
		prtqueue_insert(tempbuf);
		prtqueue_insert("\x1b\x45\x00");
		prtqueue_insert("\x1b\x21\x00");
		tt = 17; 
		if(tt==27) // ���ǸӴ��� ��쿡 ���ǸӴ� �������� ���� �μ��Ѵ�.
		{
			del_space(prt_tmp_buf, kField.Receive.issue_date[i], 10);
			sprintf(prt_tmp_buf2, "���ǸӴ� ������:%-4.4s-%-2.2s-%-2.2s\n", prt_tmp_buf, &prt_tmp_buf[4], &prt_tmp_buf[6]);
			prtqueue_insert(prt_tmp_buf2);
		}
		prtqueue_insert("\n");

		del_space(prt_tmp_buf, "123456789012", 20);
		//insert_char_at(prt_tmp_buf2, prt_tmp_buf, 4, ' ', 0);
		prtqueue_insert("\x1b\x45\x01");
		prtqueue_insert("������ȣ:");
		prtqueue_insert("\x1b\x45\x00");
		sprintf(tempbuf, "%s\n", prt_tmp_buf);
		prtqueue_insert("\x1b\x21\x20");
		prtqueue_insert(tempbuf);
		prtqueue_insert("\x1b\x21\x00");

		
		prtqueue_insert("\x1b\x45\x01");
		prtqueue_insert("��ҹ�ȣ:");
		prtqueue_insert("\x1b\x45\x00");
		if(strlen(prt_tmp_buf) > 8)
		{
			sprintf(prt_tmp_buf2, "%-8.8s\n", &prt_tmp_buf[strlen(prt_tmp_buf)-8]); // ���� 8�ڸ� ���
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

	sprintf(tempbuf, "%s", "���ó: ��L,�����,�뽼,�븶��,���\n [��ǰ ��ҹ��]\n����ȭ�� > ��ǰ����� > ��ǰ�Ǽ��� > ��ҹ�ȣ\n> �������� > ��ҿϷ�\n [��ȿ�Ⱓ] �����Ϸκ��� 1��\n[�̿빮��] 1666-3009");
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

	if(kField.Receive.event_message_pos[0] == '1') // ��
	{
		order = 0;
	}
	else if(kField.Receive.event_message_pos[0] == '2') // ��
		order = 1;

	if(kField.Send.issue_type[0]=='2')
	{
		issue_app_sms_print();
		return;
	}

	tcnt = (byte)a2l(kField.Receive.issue_count, 2);
 
    	if(print_start(SW_ON, 1)==0) return;
		
		// ESC + 'G' + n : n=1 ����, n=0 ����
		prtqueue_insert("\x1B\x47\x01"); // ���� ����
		centerposition(tempbuf, "�¶��� ������ ƾĳ�� �ϳ��� OK!!", 48, 0x20);
		prtqueue_insert(tempbuf);
		prtqueue_insert("\n");
		prtqueue_insert("\x1B\x47\x00"); // ���� ����
		prtqueue_insert("\n");
		
		sprintf(tempbuf, "%c%c%c%c%c%c%c%c%c%c", 0x1b,0x57,0x18,0x00,0x00,0x00,0x10,0x02,0x00,0x00); // 42�ڷ� ����
//	    sprintf(tempbuf, "%c%c%c%c%c%c%c%c%c%c", 0x1b,0x57,0x30,0x00,0x00,0x00,0x10,0x02,0x00,0x00); // 40�ڷ� ����
		prtqueue_insert(tempbuf);
		
		sprintf(tempbuf, "%c[EVENT]%c\n", SO, SI);
		prtqueue_insert(tempbuf);

		sprintf(tempbuf, "%s", kField.Receive.event_message);
		erase_space(tempbuf, 3);
		rr = eventmsg_convert(prt_tmp_buf, tempbuf, strlen(tempbuf));
		prtqueue_insert(prt_tmp_buf);
		prtqueue_insert("\n");
		prtqueue_insert("\n");
		
//		sprintf(tempbuf, "%s", "�̺�Ʈ�� ��÷�ǽø� ���ο� 'Ȳ���뷡��' 1�ð� �̿���� �帳�ϴ�.\n���� ���ϸ� ����� 1�ð� �̿�ǵ� ������ �帮�ڽ��ϴ�.\n�����մϴ�.\n");
//		prtqueue_insert(tempbuf);
//		prtqueue_insert("\n");

	    sprintf(tempbuf, "%c%c%c%c%c%c%c%c%c%c", 0x1b,0x57,0x00,0x00,0x00,0x00,0x10,0x02,0x00,0x00); // 48�ڷ� ����
		prtqueue_insert(tempbuf);

		line_print('=');
		sprintf(tempbuf, "�Ǹ�����:%s\n", Region01.Merchant.shop_name);
		prtqueue_insert(tempbuf);
		sprintf(prt_tmp_buf, "20%2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s", TimeStamp, &TimeStamp[2], &TimeStamp[4], &TimeStamp[6], &TimeStamp[8], &TimeStamp[10]);
		sprintf(tempbuf, "�����Ͻ�:%s\n", prt_tmp_buf);
		prtqueue_insert(tempbuf);

//		sprintf(tempbuf, "��������ȣ:%-10.10s\n", kField.Receive.resp_shop_serial);
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
					prtqueue_insert("�� ǰ ��:");
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
				prtqueue_insert("��    ��:");
				prtqueue_insert("\x1b\x45\x00");
				sprintf(tempbuf, "%c%s��%c\n", SO, inbuf, SI);
				prtqueue_insert(tempbuf);
				prtqueue_insert("\n");
			}

			line_print('-');
			prtqueue_insert("\n");
			sprintf(prt_tmp_buf, "%s", kField.Receive.pin_num[i]);
			erase_space(prt_tmp_buf, 2);
//			prtqueue_insert("\x1b\x21\x10");
			prtqueue_insert("\x1b\x45\x01");
			prtqueue_insert("ī���ȣ:");
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
			
			if(tt==27) // ���ǸӴ��� ��쿡 ���ǸӴ� �������� ���� �μ��Ѵ�.
			{
				del_space(prt_tmp_buf, kField.Receive.issue_date[i], 10);
				sprintf(prt_tmp_buf2, "���ǸӴ� ������:%-4.4s-%-2.2s-%-2.2s\n", prt_tmp_buf, &prt_tmp_buf[4], &prt_tmp_buf[6]);
				prtqueue_insert(prt_tmp_buf2);
			}
			prtqueue_insert("\n");

			del_space(prt_tmp_buf, kField.Receive.pin_manage_num[i], 20);
			//insert_char_at(prt_tmp_buf2, prt_tmp_buf, 4, ' ', 0);
			prtqueue_insert("\x1b\x45\x01");
			prtqueue_insert("������ȣ:");
			prtqueue_insert("\x1b\x45\x00");
			sprintf(tempbuf, "%s\n", prt_tmp_buf);
			prtqueue_insert("\x1b\x21\x20");
			prtqueue_insert(tempbuf);
			prtqueue_insert("\x1b\x21\x00");

			
			prtqueue_insert("\x1b\x45\x01");
			prtqueue_insert("��ҹ�ȣ:");
			prtqueue_insert("\x1b\x45\x00");
			if(strlen(prt_tmp_buf) > 8)
			{
				sprintf(prt_tmp_buf2, "%-8.8s\n", &kField.Receive.pin_manage_num[i][strlen(prt_tmp_buf)-8]); // ���� 8�ڸ� ���
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
			sprintf(prt_tmp_buf, "%cȯ�ҿ�����%c", SO, SI);
			centerposition(tempbuf, prt_tmp_buf, 38, 0x20);
			prtqueue_insert(tempbuf);
			prtqueue_insert("\n");
			prtqueue_insert("\n");
			prtqueue_insert("\n");
			
			sprintf(tempbuf, "�Ǹ�����:%s\n", Region01.Merchant.shop_name);
			prtqueue_insert(tempbuf);

			sprintf(prt_tmp_buf, "20%2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s", TimeStamp, &TimeStamp[2], &TimeStamp[4], &TimeStamp[6], &TimeStamp[8], &TimeStamp[10]);
			sprintf(tempbuf, "�����Ͻ�:%s\n", prt_tmp_buf);
			prtqueue_insert(tempbuf);
			prtqueue_insert("\n");
			line_print('-');
			prtqueue_insert("\n");

			sprintf(tempbuf, "����ҿ� ���� ȯ��\n");
			prtqueue_insert(tempbuf);
			prtqueue_insert("\n");

			memset(inbuf, 0x00, sizeof(inbuf));
			comma_ins2(inbuf, 0x20, Region01.Issue_Info.bill_input_amount);
			erase_space(&inbuf[0], 0);
			sprintf(tempbuf, "ȯ�ұݾ�:%s��\n", inbuf);
			prtqueue_insert("\x1b\x45\x01");
			prtqueue_insert(tempbuf);
			prtqueue_insert("\x1b\x45\x00");
			prtqueue_insert("\n");
			line_print('-');
			prtqueue_insert("\n");

			centerposition(tempbuf, "�̿��� �ּż� �����մϴ�.\n", 48, 0x20);
			prtqueue_insert(tempbuf);
			prtqueue_insert("\n");

			if(Region01.Merchant.vaccount_enable == 1)
			{
				prtqueue_insert("ȯ�ҿ������� ī���Ϳ� �����Ͻø� ȯ�ұݾ��� �����帳�ϴ�.\n");
			}
			else
			{
				prtqueue_insert("��ҵ� �ݾ��� ȯ���� ������ ���� 24�ð� �̳��� ó���� �帳�ϴ�. (�ָ� �� �������� ��� ���� �����Ͽ� ó���˴ϴ�.)\n");
				prtqueue_insert("24�ð� �̳��� ȯ���� ���� ���� ��� �����ͷ� �����ֽñ� �ٶ��ϴ�.\n");
			}
			prtqueue_insert("\n");
			prtqueue_insert("\n");
			
			centerposition(tempbuf, "������:1666-3009  www.teencash.co.kr", 48, 0x20);
			prtqueue_insert(tempbuf);
			prtqueue_insert("\n");
		}
		else
		{
			cp_code_index = search_cp_code((int)a2l(kField.Send.cp_code, 4));
			if(cp_code_list[cp_code_index].int_cp_code == 21 || cp_code_list[cp_code_index].int_cp_code == 22)
			{
				sprintf(prt_tmp_buf, "%c��ҿ�û Ȯ�� ������%c", SO, SI);
				centerposition(tempbuf, prt_tmp_buf, 28, 0x20);
				prtqueue_insert(tempbuf);
				prtqueue_insert("\n");
			}
			else
			{
				sprintf(prt_tmp_buf, "%c��ҿ�����%c", SO, SI);
				centerposition(tempbuf, prt_tmp_buf, 38, 0x20);
				prtqueue_insert(tempbuf);
				prtqueue_insert("\n");
			}
			prtqueue_insert("\n");
			prtqueue_insert("\n");
			
			sprintf(tempbuf, "�Ǹ�����:%s\n", Region01.Merchant.shop_name);
			prtqueue_insert(tempbuf);

			sprintf(prt_tmp_buf, "20%2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s", TimeStamp, &TimeStamp[2], &TimeStamp[4], &TimeStamp[6], &TimeStamp[8], &TimeStamp[10]);
			sprintf(tempbuf, "�����Ͻ�:%s\n", prt_tmp_buf);
			prtqueue_insert(tempbuf);
			prtqueue_insert("\n");
			line_print('-');
			prtqueue_insert("\n");

	//		cp_code_index = search_cp_code((int)a2l(kField.Send.cp_code, 4));
			if(cp_code_index != 10000)
			{
				prtqueue_insert("\x1b\x45\x01");
				prtqueue_insert("�� ǰ ��:");
				prtqueue_insert("\x1b\x45\x00");
				sprintf(tempbuf, "%c%s%c\n", SO, cp_code_list[cp_code_index].cp_code_msg, SI);
				prtqueue_insert(tempbuf);
				prtqueue_insert("\n");
			}

			del_space(prt_tmp_buf, kField.Receive.pin_manage_num[i], 20);
			//insert_char_at(prt_tmp_buf2, prt_tmp_buf, 4, ' ', 0);
			prtqueue_insert("\x1b\x45\x01");
			prtqueue_insert("������ȣ:");
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
			prtqueue_insert("��ұݾ�:");
			prtqueue_insert("\x1b\x45\x00");
			sprintf(tempbuf, "%c%s��%c\n", SO, inbuf, SI);
			prtqueue_insert(tempbuf);
			prtqueue_insert("\n");
			line_print('-');
			prtqueue_insert("\n");

			if(cp_code_list[cp_code_index].int_cp_code == 21 || cp_code_list[cp_code_index].int_cp_code == 22)
			{
				centerposition(tempbuf, "��ҿ�û ������ ���������� ó���Ǿ����ϴ�.\n", 48, 0x20);
				prtqueue_insert(tempbuf);
				prtqueue_insert("\n");
				prtqueue_insert("�� ��ǰ���� ��뿩�� Ȯ�� �� ��� ������ ��ǰ�Դϴ�.\n");
				prtqueue_insert("�̹� ���� ��ǰ���� ��� ��Ұ� �Ұ����մϴ�.\n");
				prtqueue_insert("��뿩�θ� Ȯ�� �� �ִ��� ���� �ð� ���� ����(SMS) �帮�ڽ��ϴ�.\n");
				
				if(Region01.Merchant.vaccount_enable == 0)
				{
					prtqueue_insert("ȯ���� ������ ���� 24�ð� �̳��� ó���� �帳�ϴ�. (�ָ� �� �������� ��� ���� �����Ͽ� ó���˴ϴ�.)\n");
				}
				prtqueue_insert("\n");
				centerposition(tempbuf, "������:1666-3009  www.teencash.co.kr", 48, 0x20);
				prtqueue_insert(tempbuf);
				prtqueue_insert("\n");
			}
			else
			{
				centerposition(tempbuf, "�̿��� �ּż� �����մϴ�.\n", 48, 0x20);
				prtqueue_insert(tempbuf);
				prtqueue_insert("\n");
				if(Region01.Merchant.vaccount_enable == 1)
				{
					prtqueue_insert("��ҿ������� ī���Ϳ� �����Ͻø� �ݾ��� �����帳�ϴ�.\n");
					prtqueue_insert("��ҵ� ��ǰ���� ����� �Ұ����մϴ�.\n");
				}
				else
				{
					prtqueue_insert("��ҵ� �ݾ��� ȯ���� ������ ���� 24�ð� �̳��� ó���� �帳�ϴ�. (�ָ� �� �������� ��� ���� �����Ͽ� ó���˴ϴ�.)\n");
					prtqueue_insert("24�ð� �̳��� ȯ���� ���� ���� ��� �����ͷ� �����ֽñ� �ٶ��ϴ�.\n");
				}
				prtqueue_insert("\n");
				prtqueue_insert("\n");
				
				centerposition(tempbuf, "������:1666-3009  www.teencash.co.kr", 48, 0x20);
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
	aDebugMsg("�κг��� �������� �����....");
    for(i=0; i<tcnt; i++)
    {
    	if(print_start(SW_ON, 1)==0) return;
		
		sprintf(prt_tmp_buf, "%cȯ�ҿ�����%c", SO, SI);
		centerposition(tempbuf, prt_tmp_buf, 38, 0x20);
		prtqueue_insert(tempbuf);
		prtqueue_insert("\n");
		prtqueue_insert("\n");
		prtqueue_insert("\n");
		
		sprintf(tempbuf, "�Ǹ�����:%s\n", Region01.Merchant.shop_name);
		prtqueue_insert(tempbuf);

		sprintf(prt_tmp_buf, "20%2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s", TimeStamp, &TimeStamp[2], &TimeStamp[4], &TimeStamp[6], &TimeStamp[8], &TimeStamp[10]);
		sprintf(tempbuf, "�����Ͻ�:%s\n", prt_tmp_buf);
		prtqueue_insert(tempbuf);
		prtqueue_insert("\n");
		line_print('-');
		prtqueue_insert("\n");

		err_code = (int)a2l(kField.Send.error_type_code, 2);
		switch(err_code)
		{
			case 5:
				sprintf(tempbuf, "�Ա� ���ð� �ʰ��� ���� ȯ��\n");
				break;
			case 6:
				sprintf(tempbuf, "����ɸ����� ���� ȯ��\n");
				break;
			case 7:
				sprintf(tempbuf, "��Ÿ H/W ������ ���� ȯ��\n");
				break;
		}
		prtqueue_insert(tempbuf);
		prtqueue_insert("\n");

		memset(inbuf, 0x00, sizeof(inbuf));
		comma_ins2(inbuf, 0x20, Region01.Issue_Info.bill_target_amount);
		erase_space(&inbuf[0], 0);
		sprintf(tempbuf, "���űݾ�:%s��\n", inbuf);
		prtqueue_insert(tempbuf);
			
		memset(inbuf, 0x00, sizeof(inbuf));
		comma_ins2(inbuf, 0x20, Region01.Issue_Info.bill_input_amount);
		erase_space(&inbuf[0], 0);
		sprintf(tempbuf, "�Աݱݾ�:%s��\n", inbuf);
		prtqueue_insert(tempbuf);

		if(err_code==5)
		{
			memset(inbuf, 0x00, sizeof(inbuf));
			comma_ins2(inbuf, 0x20, (Region01.Issue_Info.bill_target_amount - Region01.Issue_Info.bill_input_amount));
			erase_space(&inbuf[0], 0);
			sprintf(tempbuf, "���Աݱݾ�:%s��\n", inbuf);
			prtqueue_insert(tempbuf);
		}
		
		prtqueue_insert("\n");
		memset(inbuf, 0x00, sizeof(inbuf));
		comma_ins2(inbuf, 0x20, Region01.Issue_Info.bill_input_amount);
		erase_space(&inbuf[0], 0);
		sprintf(tempbuf, "ȯ�ұݾ�:%s��\n", inbuf);
		prtqueue_insert("\x1b\x45\x01");
		prtqueue_insert(tempbuf);
		prtqueue_insert("\x1b\x45\x00");
		prtqueue_insert("\n");
		line_print('-');
		prtqueue_insert("\n");

		centerposition(tempbuf, "�̿��� �ּż� �����մϴ�.\n", 48, 0x20);
		prtqueue_insert(tempbuf);
		prtqueue_insert("\n");

		if(Region01.Merchant.vaccount_enable == 1)
		{
			prtqueue_insert("ȯ�ҿ������� ī���Ϳ� �����Ͻø� ȯ�ұݾ��� �����帳�ϴ�.\n");
		}
		else
		{
			prtqueue_insert("��ҵ� �ݾ��� ȯ���� ������ ���� 24�ð� �̳��� ó���� �帳�ϴ�. (�ָ� �� �������� ��� ���� �����Ͽ� ó���˴ϴ�.)\n");
			prtqueue_insert("24�ð� �̳��� ȯ���� ���� ���� ��� �����ͷ� �����ֽñ� �ٶ��ϴ�.\n");
		}
		prtqueue_insert("\n");
		prtqueue_insert("\n");
		
		centerposition(tempbuf, "������:1666-3009  www.teencash.co.kr", 48, 0x20);
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
// type: ������� ���=1, ����=0
// ysdsms1: YYYY-MM-DD HH:MM:SS
//-----------------------------------------------------------------------------------------------------------------------------
void adjust_title_print(byte type, byte *ymdsms1, byte *ymdsms2)
{
	byte prt_tmp_buf[256];
	
	print_start(SW_ON, 1);
	
	sprintf(prt_tmp_buf, "%c�������� ������%c", SO, SI);
	centerposition(tempbuf, prt_tmp_buf, 33, 0x20);
	prtqueue_insert(tempbuf);
    line_feed(2);

	sprintf(tempbuf, "�Ǹ�����:%s\n", Region01.Merchant.shop_name);
	prtqueue_insert(tempbuf);

	sprintf(tempbuf, "��������ȣ:%s\n", Region01.Merchant.shop_serial);
	prtqueue_insert(tempbuf);
    line_feed(1);

	sprintf(tempbuf, "�����ȣ:%4.4s%4.4s\n", &TimeStamp[2], &Region01.Merchant.shop_serial[6]); // �����ȣ(8�ڸ�) = ��³�¥(MMDD) + ��������ȣ ��4�ڸ� 
	prtqueue_insert(tempbuf);
	sprintf(prt_tmp_buf, "20%2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s", TimeStamp, &TimeStamp[2], &TimeStamp[4], &TimeStamp[6], &TimeStamp[8], &TimeStamp[10]);
	sprintf(tempbuf, "�����Ͻ�:%s\n", prt_tmp_buf);
	prtqueue_insert(tempbuf);

	sprintf(tempbuf, "����Ⱓ:20%2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s~20%2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s\n", ymdsms1, &ymdsms1[2], &ymdsms1[4], &ymdsms1[6], &ymdsms1[8], &ymdsms1[10], ymdsms2, &ymdsms2[2], &ymdsms2[4], &ymdsms2[6], &ymdsms2[8], &ymdsms2[10]);
	prtqueue_insert(tempbuf);
    line_feed(1);
	line_print('=');
    line_feed(1);
}

int  app_cnt, can_cnt, temp_can_cnt;				// �߱ǽ���, �߱����, �κг���� Count
ulong app_amt,can_amt,temp_can_amt, net_can_amt;  // �߱ǽ���, �߱����, �κг����, ����� �ݾ�
//-----------------------------------------------------------------------------------------------------------------------------
// type: ������� ���=1, ����=0
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

	prtqueue_insert("���곻��\n");
	line_feed(1);
	
	qfront   = Region02.c_qfront;
	qrear	 = Region02.c_qrear;

	aDebugMsg("qfront=%d, qrear=%d", qfront, qrear);
	
	app_cnt=can_cnt=temp_can_cnt=0;
	app_amt=can_amt=temp_can_amt=net_can_amt=0;
	
	capture = c_cap1;              // ���迵�����۹���        
#if 0
    tempbuf[0] = ServKind;                          // 1, SERVKIND
	memcpy(&tempbuf[1], kField.ymdsms, 12); 		// 12, �ŷ��Ͻ�
	case ISSUE_APP:
		memcpy(&tempbuf[13], kField.Send.cp_code, 4); // 4, cp_code
		tempbuf[17] = (byte)a2l(kField.Send.approval_code, 2); // 1, approval_type
		tempbuf[18] = (byte)a2l(kField.Send.issue_count, 2); // 1, �ѹ߱Ǹż�
		del_space(inbuf, kField.Send.payed_tamount, 10);
		tmp_amount = a2l(inbuf, strlen(inbuf));
		memcpy(&tempbuf[19],  &tmp_amount, 4); // 4, �ŷ��ѱݾ�
		tempbuf[23] = (byte)a2l(kField.Send.issue_type, 1); // 1, �߱ǹ�� (�߱�1, sms2)
		memcpy(&tempbuf[24], kField.Send.client_phone, 16); // 16, sms�϶� ����ȭ��ȣ
		memcpy(&tempbuf[40], kField.Receive.fix_group_pay_num, 50); // 50, Ȯ���׷���ݹ�ȣ
		memcpy(&tempbuf[90], kField.Send.pg_tran_date, 14); // PG�� �ŷ��Ͻ� (��ҽ� �ŷ��� �ִ��� �Ǵܱ����� �ȴ�)
		break;
	case ISSUE_CAN:
		tempbuf[13] = kField.Send.cancel_type[0]; // 1, �������
		memcpy(&tempbuf[14], kField.Send.cp_code, 4); // 4, cp_code
		del_space(inbuf, kField.Send.can_amount, 10);
		tmp_amount = a2l(inbuf, strlen(inbuf));
		memcpy(&tempbuf[18],  &tmp_amount, 4); // 4, �ŷ��ѱݾ�
		memcpy(&tempbuf[22], kField.Send.pin_date, 8); // 8, �ܰ���ҵ� ��ǰ���� ��������
		memcpy(&tempbuf[30], kField.Send.pin_num, 24); // 24, �ɹ�ȣ
		memcpy(&tempbuf[54], kField.Send.pin_manage_num, 20); // 20, �ɰ�����ȣ
		tempbuf[74] = (byte)a2l(kField.Send.error_code, 2); // 1, �����ڵ�
		break;
	case TEMP_CANCEL:
		memcpy(&tempbuf[13], kField.Send.temp_group_pay_num, 50); // 50, �ӽñ׷���ݹ�ȣ
		del_space(inbuf, kField.Send.temp_payed_amount, 10);
		tmp_amount = a2l(inbuf, strlen(inbuf));
		memcpy(&tempbuf[63], &tmp_amount, 4); // 4, �κг��ݾ�
		tempbuf[67] = (byte)a2l(kField.Send.error_type_code, 2); // 1, ���������ڵ�
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
				if(servkind == ISSUE_CAN && temp[74]== 2) // ������� ���
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
	prtqueue_insert("��� �� �ѱݾ� : ");
	prtqueue_insert("\x1b\x45\x00");
	sprintf(prt_tmp_buf, "%c%s��%c\n", SO, inbuf, SI);
	prtqueue_insert(prt_tmp_buf);
	line_feed(1);

	amount = app_amt;
	memset(inbuf, 0x00, sizeof(inbuf));
	comma_ins2(inbuf, 0x20, amount);
	erase_space(&inbuf[0], 3);
	sprintf(tempbuf, "�ѽ��αݾ� : %s��", inbuf);
	prtqueue_insert("\x1b\x45\x01");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00\n");
	
	amount = app_amt-can_amt;
	memset(inbuf, 0x00, sizeof(inbuf));
	comma_ins2(inbuf, 0x20, amount);
	erase_space(&inbuf[0], 3);
	sprintf(tempbuf, "�����Ǹűݾ� : %s��", inbuf);
	prtqueue_insert("\x1b\x45\x01");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00\n");

	amount = can_amt+temp_can_amt+net_can_amt;
	memset(inbuf, 0x00, sizeof(inbuf));
	comma_ins2(inbuf, 0x20, amount);
	erase_space(&inbuf[0], 3);
	if(type)
		sprintf(tempbuf, "ī���Ϳ��� ȯ���� �ݾ� : %s��", inbuf);
	else
		sprintf(tempbuf, "ȯ���� �ݾ� : %s��", inbuf);
	prtqueue_insert("\x1b\x45\x01");
	prtqueue_insert(tempbuf);
	prtqueue_insert("\x1b\x45\x00\n");

	amount = can_amt+net_can_amt;
	memset(inbuf, 0x00, sizeof(inbuf));
	comma_ins2(inbuf, 0x20, amount);
	erase_space(&inbuf[0], 3);
	sprintf(tempbuf, "  �� ���ȯ�ұݾ� : %s��\n", inbuf);
	prtqueue_insert(tempbuf);

	amount = temp_can_amt;
	memset(inbuf, 0x00, sizeof(inbuf));
	comma_ins2(inbuf, 0x20, amount);
	erase_space(&inbuf[0], 3);
	sprintf(tempbuf, "  �� �κг�ȯ�ұݾ� : %s��\n", inbuf);
	prtqueue_insert(tempbuf);

    line_feed(1);
	line_print('=');
    line_feed(1);
}
//-----------------------------------------------------------------------------------------------------------------------------
// type: ������� ���=1, ����=0
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
	
	capture = c_cap1;              // ���迵�����۹���        

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
			prtqueue_insert("���ȯ�� ��");
			prtqueue_insert("\x1b\x45\x00\n");
			first = 1;
		}
#if 0
			tempbuf[13] = kField.Send.cancel_type[0]; // 1, �������
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
			memcpy(&tempbuf[18],  &tmp_amount, 4); // 4, �ŷ��ѱݾ�
			memcpy(&tempbuf[22], kField.Send.pin_date, 8); // 8, �ܰ���ҵ� ��ǰ���� ��������
			memcpy(&tempbuf[30], kField.Send.pin_num, 24); // 24, �ɹ�ȣ
			memcpy(&tempbuf[54], kField.Send.pin_manage_num, 20); // 20, �ɰ�����ȣ
			tempbuf[74] = (byte)a2l(kField.Send.error_code, 2); // 1, �����ڵ�
#endif
		code = search_cp_code2(((int)a2l(&temp[14], 4)));
		if(code == 10000)
		{
			if(servkind == ISSUE_CAN && temp[74]== 2) // ������� ���
			{
				sprintf(prt_tmp_buf, "%s", "�����");
			}
		}
		else
		{
			sprintf(prt_tmp_buf, "%s", cp_code_list2[code].cp_code_msg);
		}
		centerposition(inbuf, prt_tmp_buf, 20, 0x20);

		memcpy((byte *)&amount, &temp[18], 4); // �ݾ� �μ�
		memset(prt_tmp_buf, 0, sizeof(prt_tmp_buf));
		comma_ins2(prt_tmp_buf, 0x20, amount);
		erase_space(&prt_tmp_buf[0], 0);
		
		sprintf(tempbuf, "%d.20%2.2s/%2.2s/%2.2s %s%10.10s��\n", seq++, &temp[1], &temp[3], &temp[5], inbuf, prt_tmp_buf);
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
			prtqueue_insert("�κг�ȯ�� ��");
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
				sprintf(intemp, "�Աݴ��ð� �ʰ�");
				break;
			case 6:
				sprintf(intemp, "���� �ɸ�");
				break;
			case 7:
				sprintf(intemp, "�ϵ���� ���");
				break;
		}
		centerposition(inbuf, intemp, 20, 0x20);
		sprintf(tempbuf, "%d.20%2.2s/%2.2s/%2.2s %s%10.10s��\n", seq++, &temp[1], &temp[3], &temp[5], inbuf, prt_tmp_buf);
		prtqueue_insert(tempbuf);
	}
	line_feed(1);

	prtqueue_insert("\x1b\x45\x01");
	if(type)
		prtqueue_insert("�� ��� �� �ѱݾ� = �����Ǹűݾ� + ī���Ϳ��� ȯ���� �ݾ�");
	else
		prtqueue_insert("�� ��� �� �ѱݾ� = �����Ǹűݾ� + ȯ���� �ݾ�");
	prtqueue_insert("\x1b\x45\x00\n");
}
//-----------------------------------------------------------------------------------------------------------------------------
#endif

