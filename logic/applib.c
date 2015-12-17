#include "include.h"


int HexDump( unsigned char* addr, int len );
unsigned char CompnStr(unsigned char *s,unsigned char *s1,int Len);
void StrnCpy(unsigned char *s,unsigned char *s1,int Len);
void TotalAmountMeth(unsigned char *s,unsigned char *s1,unsigned char Sign);
void LineDo(unsigned char *s,unsigned char Len);
void LineUp(unsigned char *s,unsigned char method,int Len);
void LineDoSpaceFirst(unsigned char *s,unsigned char Len);


const byte yoil[15] = "일월화수목금토";

//화살표
const byte updir1[16] = {0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,  0x00,0x00,0x04,0x06,0x07,0x07,0x07,0x07};
const byte updir2[16] = {0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,  0x07,0x07,0x07,0x07,0x07,0x06,0x04,0x00};
const byte dndir1[16] = {0x00,0x20,0x60,0xE0,0xE0,0xE0,0xE0,0xE0,  0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x0F};
const byte dndir2[16] = {0xE0,0xE0,0xE0,0xE0,0x60,0x20,0x00,0x00,  0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00};

void set_cursor(byte pos, byte cchar)
{
    curpos = pos;
    curchar = cchar;
}

// Parameters :
// ostr : source buffer
// type : 0 -> ALL TRIM
//        1 -> LEFT TRIM, 2 -> RIGHT TRIM, 3 -> LEFT/RIGHT TRIM
void erase_space(byte *ostr, byte type)
{
    int i, j, k;
    byte erbuf[1024*2]={0};

    j = Strlen(ostr);
    Memset(erbuf, 0, sizeof(erbuf));
    if (!type)           // ALL TRIM
    {
        for(i=0,k=0; i<j; i++)
        {
            if (ostr[i] != 0x20) erbuf[k++] = ostr[i];
        }
        Memcpy(ostr, erbuf, k);
        ostr[k] = 0;
        return;
    }

    if (type & 0x01)     // LEFT TRIM
    {
        for(i=0; i<j; i++)
        {
            if (ostr[i] && ostr[i] != 0x20) break;
        }
        Memcpy(erbuf, &ostr[i], Strlen(&ostr[i]));
        Memcpy(ostr, erbuf, Strlen(erbuf));
        ostr[Strlen(erbuf)] = 0;
    }

    if (type & 0x02)     // RIGHT TRIM
    {
        j = Strlen(ostr);
        j--;
        for(; j > 0; j--)
        {
            if (ostr[j] && ostr[j] != 0x20) break;
            ostr[j] = 0;
        }
    }
}

// value를 asc로 전환하여 컴마 추가한 형태의 16바이트로 전환
// value를 전환한값의 길이가 10바이트를 넘을경우 오류 처리함
// dest에 결과 전송 const_str에 있는 값을 앞에 4바이트 복사함
// type : 1: 마이너스 표시
void comma_insert(byte *dest, byte *const_str, ulong value, int type)
{
    byte len, siz=13, pnt=0;
    byte temp[32]={0};

    Memset(temp, 0, sizeof(temp));
    if ( type == 1 &&  value > 0)
    {
        temp[0] = '-';
        l2a(value, temp+1);
        }
        else
        {
            if ( value == 0 )
                type = 0;
        l2a(value, temp);
    }
    Memset(dest, 0x20, 16);

    len = (byte)Strlen(temp);
    if (len <= 8)
        Memcpy(&dest[0], const_str, 4);
    else
    if (len > 10) return;

    while(len--)
    {
        if (pnt == 3 && (len-type) >= 0)
        {
            dest[siz--] = ',';
            pnt = 1;
        }
        else
            pnt++;
        dest[siz--] = temp[len];
    }
    Memcpy(&dest[14], "원\0", 3);
}

void comma_insert1(uchar *dest1, ulong value1, uchar type)
{
    uchar len, siz=11, pnt=0;
    uchar temp[32]={0};

    Memcpy(dest1, "            \0", 13);

    Memset(temp, 0, sizeof(temp));
    if (!value1)
        temp[0]=0x30;
    else
        l2a(value1, temp);
    len = (uchar)Strlen(temp);
    if (len > 9) return;

    while(len--)
    {
        if (pnt++ == 3 )
        {
            dest1[siz--] = ',';
            pnt = 1;
        }
        dest1[siz--] = temp[len];
    }
    dest1[siz] = type;
}

void comma_ins2(u8 *dest, u8 sign, ulong value)
{
    u8 len, siz=13, pnt=0;
    u8 temp[32]={0};

    if(value == 0) sign = 0x20;
    memset(temp, 0, sizeof(temp));
    l2a(value, temp);
    memset(dest, 0x20, 14);

    len = (u8)strlen(temp);
    if (len > 10) return;

    while(len--)
    {
        if (pnt++ == 3 )
        {
            dest[siz--] = ',';
            pnt = 1;
        }
        dest[siz--] = temp[len];
    }
    dest[siz] = sign;
    dest[14]  = 0;
}

void comma_ins0(u8 *dest, u8 sign, ulong value)
{
    u8 len, siz=13, pnt=0, cnt = 0;
    u8 temp[32]={0};

    memset(temp, 0, sizeof(temp));
    l2a(value, temp);
    memset(dest, 0x20, 14);

    len = (u8)Strlen(temp);
    if (len > 13) return;

    while(len--)
    {
        if (pnt++ == 3 )
        {
            dest[siz--] = ',';
            pnt = 1;cnt++;
        }
        dest[siz--] = temp[len];
    }
    dest[siz] = sign;
    //dest[14]  = 0;
    printf("comma[%d][%ld][%s]\n", strlen(dest), value, dest);
    memcpy(&dest[strlen(dest)], "원\0", 3);
    
}

void itoaform(word value)
{
    ulong val;

    val = value;
    comma_insert(formstr1, (byte *)"    ", val, 0);
    formstr1[14] = 0;
    erase_space(formstr1, 0);
}

void ltoaform(ulong value)
{
    comma_insert(formstr2, (byte *)"    ", value, 0);
    formstr2[14] = 0;
    erase_space(formstr2, 1);
}

// Parameters :
//              dest -> sour의 특정데이타가 복사될 버퍼
//              sour -> FS, ETX가 구분자로 구성되어있는 버퍼
//              siz  -> dest로 복사될때 총한계치를 명시함, 주로 dest버퍼의 SIZE
//                      보다 1바이트 적은 숫자를 지정하며, 이유는 복사할때 dest
//                      의 영역보다 더많이 복사 되지 않게 하기위함.
// Returns :
//              sour로 부터 dest로 카피할때 FS를 만나면 FS전까지 복사하고 Return
//              값은 FS까지의 COUNT를 Retuns한다. 그리고 ETX또는 NULL을 만나면
//              그 전까지 복사하고 그 전까지의 COUNT를 Return한다.
word find_FS(byte *dest, byte *sour, word siz)
{
    word cnt;

    dest[0] = 0;
    for(cnt=0; cnt<siz; cnt++)
    {
        if (sour[cnt]==FS)
        {
            dest[cnt] = 0;
            break;
        }
        else
        if (sour[cnt]==ETX)
        {
            dest[cnt] = 0;
            return(cnt);
        }
        dest[cnt] = sour[cnt];
    }

	//printf("sour[cnt]aaaaaaaaaaa =0x%02x\n", sour[cnt]);
    if (sour[cnt]!=FS)
    {
    	//int iii;
		//for(iii=0; iii<rlen; iii++)
		//{
		//	if(iii==40)
		//		printf("\n");
		//	if(recvbuf[iii]==0x1c)
		//		printf("\n");
		//	printf("0x%02x ", recvbuf[iii]);
		//}

		//printf("\n\n=================================\n\n");
		//printf("siz : %d\n\n", siz);

		//for(iii=0; iii<strlen(sour); iii++)
		//{
		//	printf("0x%02x ", sour[iii]);
		//}

		//printf("\n\n=================================\n\n");
		
        printf("FS not exist!!\n");
        for(;cnt<512;cnt++)
        {
            if (sour[cnt]==FS) break;
        }
        if (sour[cnt]!=FS)
        {
            printf("Electronic Text deferrent format!!\n");
        }
    }
    cnt++;

    return(cnt);
}

word find_GS(byte *dest, byte *sour, word siz)
{
    word cnt;
    dest[0] = 0;
    for(cnt=0; cnt<siz; cnt++)
    {
        if (sour[cnt]==GS)
        {
            dest[cnt] = 0;
            break;
        }
        else
        if (sour[cnt]==ETX ||sour[cnt]==0x00)
        {
            dest[cnt] = 0;
            return(cnt);
        }
        dest[cnt] = sour[cnt];
    }

    if (sour[cnt]!=GS)
    {
        printf("GS not exist!!\n");
        for(;cnt<512;cnt++)
        {
            if (sour[cnt]==GS ||sour[cnt]==ETX) break;
        }
        if (sour[cnt]!=GS)
        {
            printf("Electronic Text deferrent format!!\n");
        }
    }
    cnt++;
    return(cnt);
}
//word find_GS(byte *dest, byte *sour, word siz)
//{
//    word cnt;
//
//    dest[0] = 0;
//    for(cnt=0; cnt<siz; cnt++)
//    {
//        if (sour[cnt]==GS) break;
//        else
//        if (!sour[cnt])
//        {
//            dest[cnt] = 0;
//            return(cnt);
//        }
//        dest[cnt] = sour[cnt];
//    }
//    dest[cnt++] = 0;
//    return(cnt);
//}

word find_RS(byte *dest, byte *sour, word siz)
{
    word cnt;

    dest[0] = 0;
    for(cnt=0; cnt<siz; cnt++)
    {
        if (sour[cnt]==RS) break;
        else
        if (!sour[cnt])
        {
            dest[cnt] = 0;
            return(cnt);
        }
        dest[cnt] = sour[cnt];
    }
    dest[cnt++] = 0;
    return(cnt);
}

word find_US(byte *dest, byte *sour, word siz)
{
    word cnt;

    dest[0] = 0;
    for(cnt=0; cnt<siz; cnt++)
    {
        if (sour[cnt]==US) break;
        else
        if (!sour[cnt])
        {
            dest[cnt] = 0;
            return(cnt);
        }
        dest[cnt] = sour[cnt];
    }
    dest[cnt++] = 0;
    return(cnt);
}

void so_si_delete(byte *ddata)
{
    byte buf[256]={0};
    word  cnt, siz, bsiz;

    siz = strlen(ddata);
    for(cnt=0,bsiz=0; cnt<siz; cnt++)
    {
        if (ddata[cnt] != SO && ddata[cnt] != SI)
            buf[bsiz++] = ddata[cnt];
    }
    memcpy(ddata, buf, siz);
}

void get_cursor(word flag)
{
    byte i, j;

    curpos = curchar = 0;
    j = (byte)Strlen(Mask);
    if (flag & BACKWARD_DIRECT)
    {
        while(j--)
        {
            if (Mask[j] == '@')
            {
                curpos = j;
                curchar = kinbuf1[j];
                break;
            }
        }
    }
    else
    {
        for(i=0; i<j; i++)
        {
            if (Mask[i] == '@')
            {
                curpos = i;
                curchar = kinbuf1[i];
                if (kinbuf1[i] == 0x20) break;
            }
            if (flag & S_FLAG) curchar = '*';
        }
    }
}

void A_copy(byte *dest, byte *sour, word siz)
{
    byte temp[64] = {0};
    word  i;

    Memset(temp, 'A', siz);
    for(i=0; i < siz; i++)
    {
        if (sour[i] == '=' || !sour[i]) break;
    }
    if (i)
    {
        Memcpy(temp, sour, i);
        Atoh(dest, temp, siz/2);
    }
}

void H_copy(byte *dest, byte *sour, word siz)
{
    byte temp[64] = {0};
    word  i;

    Htoa(temp, sour, siz/2);
    for(i=0; i<siz; i++)
    {
        if (temp[i] == 'A') break;
    }
    Memcpy(dest, temp, i);
}

// returns
// TRUE  -> character ok
// FLASE -> fail
byte character_check(byte *dest, byte ch, word siz)
{
    word ii;

    for(ii=0; ii<siz; ii++)
    {
        if (dest[ii] != ch) break;
    }
    if (ii==siz) return(TRUE);
    else         return(FALSE);
}

// returns
// TRUE  -> numeric ok
// FLASE -> fail
byte numeric_check(byte *dest, word siz)
{
    word ii;

    for(ii=0; ii<siz; ii++)
    {
        if (!(dest[ii]>='0' && dest[ii]<='9')) break;
    }
    if (ii==siz) return(TRUE);
    else         return(FALSE);
}

// returns
// TRUE  -> numeric ok
// FLASE -> fail
byte alpha_numeric_check(byte *dest, word siz)
{
    word ii;

    for(ii=0; ii<siz; ii++)
    {
        if ((dest[ii]>='0'&&dest[ii]<='9')||(dest[ii]>='A'&&dest[ii]<='Z')||\
            (dest[ii]>='a'&&dest[ii]<='z'))
            continue;
        else
            break;
    }
    if (ii==siz) return(TRUE);
    else         return(FALSE);
}

// returns
// TRUE  -> hex 0x20 under ok
// FLASE -> none under hex 0x20
byte h20under_check(byte *dest, word siz)
{
    word ii;

    for(ii=0; ii<siz; ii++)
    {
        if (dest[ii] < 0x20) return(TRUE);
    }
    return(FALSE);
}

// returns
// TRUE  -> ok
// FLASE -> fail
// Parameters :
//            : sour -> check data
//            : 0 -> full check
//            : 1 -> half check
byte ymdsms_check(byte *sour, byte type)
{
    word year,month,date,time=0,minute=0,sec=0;

    if (numeric_check(sour, 12)==FALSE)
        return(FALSE);
    year   = (word)a2l(sour+0, 2);
    month  = (word)a2l(sour+2, 2);
    date   = (word)a2l(sour+4, 2);
    if (!type)
    {
        time   = (word)a2l(sour+6, 2);
        minute = (word)a2l(sour+8, 2);
        sec    = (word)a2l(sour+10, 2);
    }

    if (!(year   >= 1 && year   <= 99)) return(FALSE);
    if (!(month  >= 1 && month  <= 12)) return(FALSE);
    if (!(date   >= 1 && date   <= 31)) return(FALSE);
    if (!type)
    {
        if (!(time   >= 1 && time   <= 12)) return(FALSE);
        if (!(minute >= 1 && minute <= 60)) return(FALSE);
        if (!(sec    >= 1 && sec    <= 60)) return(FALSE);
    }
    return(TRUE);
}

byte make_lrc(byte *buf, word len)
{
    byte x_or=0;
    word  si=0;

    for(;si < len; si++)
    {
        x_or = x_or ^ buf[si];
    }
    return(x_or|0x20);
}

uchar xor_sum(uchar *buf, uint len)
{
    uchar x_or=0;
    uint  si=0;

    for(;si < len; si++)
    {
        x_or = x_or ^ buf[si];
    }
    return(x_or);
}

void Atoh(byte *dest, byte *src, word siz)
{
    word i, scnt, dcnt;
    byte ret,one,two;

    for(scnt=dcnt=i=0; i < siz; i++)
    {
        one = src[scnt+i*2];
        two = src[scnt+i*2+1];
        ret =  ((one > '9') ? (one - 0x37) : (one - 0x30)) << 4;
        ret |= ((two > '9') ? (two - 0x37) : (two - 0x30)) & 0x0f;
        dest[dcnt++] = ret;
    }
}

void Htoa(byte *dest, byte *src, word siz)
{
    word  i;

    for(i = 0; i < siz; i++)
    {
        htoa(src[i], dest+i*2);
    }
}

void aatoh(byte src[],word scnt,word cnt,byte dest[],word dcnt)
{
    word i;
    byte ret,one,two;

    for(i = 0; i < cnt; i++)
    {
        one = src[scnt+i*2];
        two = src[scnt+i*2+1];
        ret = ((one > '9') ? (one - 0x37) : (one - 0x30)) << 4;
        ret |= (two > '9') ? (two - 0x37) : (two - 0x30);
        dest[dcnt++] = ret;
    }
}

int GetDay(byte *sour)
{
    char months[]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int i;
    long year, month, date, total;

    year  = a2l(sour+0, 2) + 2000;
    month = a2l(sour+2, 2);
    date  = a2l(sour+4, 2);

    if (!(year%4) && ((year%100) || !(year%400)))
        months[1]=29;
    else
        months[1]=28;

    total=(year-1)*365L+(year-1)/4L-(year-1)/100L+(year-1)/400L + date;

    for(i=0; i<month-1; i++)
       total += (long)months[i];

    return (int)((total)%7L);
}

void calcu_in(byte *dest)
{
    byte ii, jj, kk,ll;
    byte temp[33]={0};

    Memset(temp, 0, sizeof(temp));
    jj=(byte)Strlen(dest);
    if (!jj) return;

    for(ii=0; ii<jj; ii++)
    {
        if (dest[ii]=='.') break;
    }

    if (ii)
    {
        ll = 16+ii;
        ll--;
        for(kk=0; kk<ii; kk++)
        {
            if (dest[kk]>='0' && dest[kk]<='9') temp[ll--] = dest[kk]-0x30;
            else ll--;
        }
    }

    for(ll=15; ii<jj; ii++)
    {
        if (dest[ii]!='.')
        {
            if (dest[ii]>='0' && dest[ii]<='9') temp[ll--] = dest[ii]-0x30;
            else ll--;
        }
    }
    Memcpy(dest, temp, 32);
}

void calcu_out(byte *dest)
{
    byte ii, jj=0;
    byte temp[33]={0};

    Memset(temp, 0, sizeof(temp));
    for(ii=31; ii>15; ii--)
    {
        if (dest[ii]) break;
    }

    if (ii==15) temp[jj++]='0';
    else
    {
        for(jj=0; ii>15; ii--)
        {
            temp[jj++]=dest[ii]+0x30;
        }
    }

    if (dest[13] || dest[14] || dest[15])
    {
        temp[jj++] = '.';
        temp[jj++] = dest[15]+0x30;
        temp[jj++] = dest[14]+0x30;
        temp[jj++] = dest[13]+0x30;
    }
    else
    {
        temp[jj++] = '.';
        temp[jj++] = 0x30;
        temp[jj++] = 0x30;
        temp[jj++] = 0x30;
    }
    Memcpy(dest, temp, 32);
}

void set_str2num(byte *dest, void *sourstr)
{
    byte *sour;
    byte temp[8]={0};
    word cnt=0, pos=0, tcnt=0;

    sour = (byte *)sourstr;
    while(1)
    {
        if (sour[cnt]==0)
        {
            if (tcnt) dest[pos++] = (byte)a2l(temp, tcnt);
            break;
        }
        else
        if (sour[cnt]==',')
        {
            dest[pos++] = (byte)a2l(temp, tcnt);
            tcnt = 0;
        }
        else
        if (sour[cnt]>='0' && sour[cnt]<='9')
            temp[tcnt++ & 7] = sour[cnt];
        cnt++;
    }
    dest[pos] = 0;
}

void fillzero(uchar *data, int len)
{
    int ilen;
    uchar tempzero[40]={0};

    ilen = Strlen(data);

    Memset(tempzero, 0, sizeof(tempzero));

    Memset(tempzero, 0x30, len);
    Memcpy(&tempzero[len-ilen], data, ilen);
    Memcpy(data, tempzero, len);
    data[len] = 0x0;
}

int decodeEscape(unsigned char* input, unsigned char* output, int in_len)
{
    int i, out_len = 0;

    for(i = 0; i < in_len; i++)
    {
        if (*(input+i) == 0x23) // 통신제어문자 or Prefix (0x23)
        {
            i++;
            *output++ = *(input+i) ^ 0x40;
        }
        else
        {
            *output++ = *(input+i);
        }
        out_len++;
    }
    return(out_len);
}

int encodeEscape(unsigned char* input, unsigned char* output, int in_len)
{
    int i, out_len = 0;

    for(i = 0; i < in_len; i++)
    {
        //if ((*(input+i) >= 0x00 && *(input+i) < 0x20) || *(input+i) == 0x23) // 통신제어문자 or Prefix (0x23)
        if ((*(input+i) < 0x20) || *(input+i) == 0x23) // 통신제어문자 or Prefix (0x23)
        {
            *output++ = 0x23;
            *output++ = *(input+i) ^ 0x40;
            out_len += 2;
        }
        else
        {
            *output++ = *(input+i);
            out_len++;
        }
    }
    return(out_len);
}

// purpose : ip string 의 유효성 검증
// returns : 0 -> true, 1 -> false
byte ip_format_check(byte *ipbuf)
{
    int ii, jj, cnt, addr, dot;
    byte buf[32];

    dot = 0;
    cnt = strlen(ipbuf);
    memset(buf, 0, sizeof(buf));
    for(ii=jj=0; ii<cnt; ii++)
    {
        if (ipbuf[ii] == '.' || (ipbuf[ii] >= '0' && ipbuf[ii] <= '9'))
        {
            if (ipbuf[ii] == '.')
            {
                addr = (int)a2l(buf, jj);
                if (addr > 255) return(1);
                dot++;
                jj = 0;
                memset(buf, 0, sizeof(buf));
            }
            else
            {
                buf[jj++] = ipbuf[ii];
            }
        }
        else return(1);
    }
    if (dot != 3) return(1);
    else          return(0);
}

// purpose : ip string 을 copy
// returns : 0 -> true, 1 -> false
byte ip_format_copy(byte *dest, byte *ipbuf)
{
    int ii, jj, cnt, addr, dot;
    byte buf[32];

    dot = 0;
    cnt = strlen(ipbuf);
    memset(buf, 0, sizeof(buf));
    for(ii=jj=0; ii<cnt; ii++)
    {
        if (ipbuf[ii] == '.' || (ipbuf[ii] >= '0' && ipbuf[ii] <= '9'))
        {
            if (ipbuf[ii] == '.')
            {
                addr = (int)a2l(buf, jj);
                if (addr > 255) return(1);
                dot++;
                jj = 0;
                memset(buf, 0, sizeof(buf));
                sprintf(&dest[strlen(dest)], "%d.", addr);
            }
            else
            {
                buf[jj++] = ipbuf[ii];
            }
        }
        else return(1);
    }
    if (dot != 3) return(1);
    else
    {
        addr = (int)a2l(buf, jj);
        sprintf(&dest[strlen(dest)], "%d", addr);
        return(0);
    }
}

const unsigned short CRC16Table[] =
{
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
	0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
	0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
	0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
	0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
	0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
	0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12, 
	0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A, 
	0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41, 
	0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49, 
	0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
	0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78, 
	0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
	0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067, 
	0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
	0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
	0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
	0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
	0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
	0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
	0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
	0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
	0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
	0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};


unsigned short GetCrc16(
	unsigned char *buffer,	/* this is the actual data */
	int length)				/* how big is the buffer to calculate the crc on */
{
	unsigned short crc_val = 0x0000;

	/* a local register copy will speed up the operations below */
	unsigned short const * crc_table = CRC16Table;
	while (length--)
		crc_val = (crc_val << 8) ^ crc_table[(crc_val >> 8) ^ *buffer++];

	return crc_val;
}

int HexDump( unsigned char* addr, int len )
{
    unsigned char* s=(unsigned char*)addr;
	unsigned char* pp=addr;
    unsigned char* endPtr;
    unsigned char       i, remainder=len%16;

	debug_mod = 0;
    if(debug_mod==0) return;

	pp+=len;
	endPtr=pp;
    printf("Offset      Hex Value                                        Ascii value\n");
    
    while (s+16<=endPtr){
        printf("0x%04x  ", (unsigned short)(s-addr));
        for (i=0; i<16; i++){
            printf("%02x ", s[i]);
        }
        printf(" ");
        for (i=0; i<16; i++){
            if      (s[i]>=32 && s[i]<=125) printf("%c", s[i]);
            else                            printf(".");
        }
        s += 16; 
        printf("\n");
    }   
    
    if (remainder){
        printf("0x%04x  ", (short)(s-addr));
 
        for (i=0; i<remainder; i++){
            printf("%02x ", s[i]);
        }
        for (i=0; i<(16-remainder); i++){
            printf("   ");
        }
        printf(" ");
        for (i=0; i<remainder; i++){
            if      (s[i]>=32 && s[i]<=125) printf("%c", s[i]);
            else                            printf(".");
        }
        for (i=0; i<(16-remainder); i++){
            printf(" ");
        }
        printf("\n");
    }   
    return 0;
}

/***************************************
s > s1 = 1
s < s1 = 2
s = s1 = 0
***************************************/
unsigned char CompnStr(unsigned char *s,unsigned char *s1,int Len)
{
	int i;
	
	for(i=0;i<Len;i++)
	{
		if(s[i] != s1[i])
		{
			if(s[i] > s1[i])
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
	}
	return 0;
}

void StrnCpy(unsigned char *s,unsigned char *s1,int Len)
{
    int i;

    for(i=0;i<Len;i++)
    {
        s[i] = s1[i];
    }
    s[Len] = 0x00;

}

void TotalAmountMeth(unsigned char *s,unsigned char *s1,unsigned char Sign)
{
	int i,j,k,l;
	unsigned char TempBuf[20+1];
	
	i = strlen(s1);
	l = 15;
    for(k=0;k<l;k++)
    {
        TempBuf[k] = 0x20;
    }
    TempBuf[k] = 0x00;
    k = l-1;
    j=0;
    while((i--) && (k+1))
    {
        if(!(j%3) && j)
        {
            TempBuf[k--] = ',';
        }
        TempBuf[k--] = s1[i];
        j++;
    }
	for(i=0;i<l;i++)
	{
		if(TempBuf[i] != 0x20)
		{
			if(Sign == 1)
			{
				if(!i)
				{
					i = 1;
				}
				s[i-1] = '-';
			}
			s[i] = TempBuf[i];
			break;
		}
		else
		{
			s[i] = TempBuf[i];
		}
	}
	for(;i<l;i++)
	{
		s[i] = TempBuf[i];
	}
	s[i] = 0x00;
}

void LineDo(unsigned char *s,unsigned char Len)
{
	int i,j;
	
	for(i=0;i<Len;i++)
	{
		if(s[i] != 0x30)
		{
			break;
		}
	}
	if(i != Len)
	{
		for(j=0;i<Len;j++,i++)
		{
			s[j] = s[i];
		}
		s[j] = 0x00;
	}
	else
	{
		memset(s,0x00,Len);
	}
}

void LineUp(unsigned char *s,unsigned char method,int Len)
{
	int i,j;
	
	for(i=0;i<Len;i++)
	{
		if(!s[i])
		{
			break;
		}
	}
	if(i != Len)
	{
		switch(method)
		{
			case 0:					/* left 0   1230000 */
				for(;i<Len;i++)
				{
					s[i] = 0x30;
				}
				break;
			case 1:					/* left space */
				for(;i<Len;i++)
				{
					s[i] = 0x20;
				}
				break;
			case 2:					/* right 0 0000123 */
				for(j=0;j<i;j++)
				{
					s[Len-j-1] = s[i-j-1];
				}
				for(;j<Len;j++)
				{
					s[Len-j-1] = 0x30;
				}
				break;
			case 3:					/* right space */
				for(j=0;j<i;j++)
				{
					s[Len-j-1] = s[i-j-1];
				}
				for(;j<Len;j++)
				{
					s[Len-j-1] = 0x20;
				}
				break;
			case 4:					/* left Null */
				for(;i<Len;i++)
				{
					s[i] = 0x00;
				}
				break;
			default:
				s[0] = 0x00;
				break;
		}
	}
	s[Len] = 0x00;
}

void LineDoSpaceFirst(unsigned char *s,unsigned char Len)
{
	int i,j;
	
	for(i=0;i<Len;i++)
	{
		if(s[i] != 0x20)
		{
			break;
		}
	}
	if(i != Len)
	{
		for(j=0;i<Len;j++,i++)
		{
			s[j] = s[i];
		}
		s[j] = 0x00;
	}
	else
	{
		memset(s,0x00,Len);
	}
}

int LastHanPosition(byte *ss, int len)
{
	byte Last_han=0;
	int i=0,j=0;

	j=len;
	for(i=0;i<j;i++) {
		if(ss[i] >= 0x80) {
			Last_han=i;
			i++;
		}
	}
	return Last_han;
}

//----------------------------------------------------------------------------------------
// 성공:0, 실패:1
//----------------------------------------------------------------------------------------
u8 ifconfig_read(u8 *ip, u8 *subnet_mask, u8 *gateway, u8 *dns1, u8 *dns2)
{
	FILE	*fp;
	int i,j,k;
	unsigned char cData;
	unsigned char TempBuf[1024+1];
	int IsFlg;
	unsigned char BackTermIpAddr[15+1];
	unsigned char BackTermGateWayAddr[15+1];
	unsigned char BackTermNetMaskAddr[15+1];
	unsigned char BackTermDNS1[15+1];
	unsigned char BackTermDNS2[15+1];

	system("ifconfig eth0 | grep \"inet\" | grep \"Bcast:\" | awk '{print $2}' | awk -F: '{print $2}' > /tmp/temp.txt"); // IP
	fp = fopen("/tmp/temp.txt", "r");
	if(fp == NULL){
		printf("FILE 찾기 실패\n");
		return 1;
	}
	memset(TempBuf, 0, sizeof(TempBuf));
	for(i = 0; !feof(fp) && (i<1024); i++){
		cData = (unsigned char)fgetc(fp);
		TempBuf[i] = cData;
	}
	fclose(fp);
	
	erase_char(TempBuf, 0x0a);
	erase_char(TempBuf, 0xff);
	memset(BackTermIpAddr, 0, sizeof(BackTermIpAddr));
	del_space(BackTermIpAddr, TempBuf, 15); 

	system("ifconfig eth0 | grep \"inet\" | grep \"Bcast:\" | awk '{print $4}' | awk -F: '{print $2}' > /tmp/temp.txt"); // SUBNETMASK
	fp = fopen("/tmp/temp.txt", "r");
	if(fp == NULL){
		printf("FILE 찾기 실패\n");
		return 1;
	}
	memset(TempBuf, 0, sizeof(TempBuf));
	for(i = 0; !feof(fp) && (i<1024); i++){
		cData = (unsigned char)fgetc(fp);
		TempBuf[i] = cData;
	}
	fclose(fp);

	erase_char(TempBuf, 0x0a);
	erase_char(TempBuf, 0xff);
	memset(BackTermNetMaskAddr, 0, sizeof(BackTermNetMaskAddr));
	del_space(BackTermNetMaskAddr, TempBuf, 15); 
	
	system("route -n | grep \"UG\" | awk '{print $2}' > /tmp/temp.txt"); // GATEWAY
	fp = fopen("/tmp/temp.txt", "r");
	if(fp == NULL){
		printf("FILE 찾기 실패\n");
		return 1;
	}
	memset(TempBuf, 0, sizeof(TempBuf));
	for(i = 0; !feof(fp) && (i<1024); i++){
		cData = (unsigned char)fgetc(fp);
		TempBuf[i] = cData;
	}
	fclose(fp);

	erase_char(TempBuf, 0x0a);
	erase_char(TempBuf, 0xff);
	memset(BackTermGateWayAddr, 0, sizeof(BackTermGateWayAddr));
	del_space(BackTermGateWayAddr, TempBuf, 15); 

#if 0
	for(i=0,j=0;i<15;i++) // Gateway는 Ip 마지막 1로 고정하여 보여준다.
	{
		if(j == 3)
		{
			BackTermGateWayAddr[i] = '1';
			BackTermGateWayAddr[i+1] = 0x00;
			break;
		}
		if(BackTermIpAddr[i] == '.')
		{
			j++;
		}
		BackTermGateWayAddr[i] = BackTermIpAddr[i];
	}
#endif
#if 0
	system("ifconfig > /tmp/temp.txt");
	fp = fopen("/tmp/temp.txt", "r");
	if(fp == NULL){
		printf("FILE 찾기 실패\n");
		return 1;
	}
	for(i = 0; !feof(fp) && (i<1024); i++){
		cData = (unsigned char)fgetc(fp);
		TempBuf[i] = cData;
	}
	fclose(fp);
	system("rm /tmp/temp.txt");
	IsFlg = 0;
	for(j=0;j<i;j++)
	{
		if(IsFlg == 4)
		{
			break;
		}
		if(!memcmp("eth0",&TempBuf[j],4))
		{
			j += 4;
			IsFlg++;
		}
		if(!memcmp("inet addr:127.0.0.1",&TempBuf[j],19))
		{
			j += 19;
			IsFlg++;
			continue;
		}
		if(!memcmp("inet addr:",&TempBuf[j],10))
		{
			j += 10;
			for(k=0;k<15;k++)
			{
				if(TempBuf[j] == 0x20)
				{
					BackTermIpAddr[k] = 0x00;
					IsFlg++;
					break;
				}
				BackTermIpAddr[k] = TempBuf[j];
				j++;
			}
		}
		if(!memcmp("Mask:",&TempBuf[j],5))
		{
			j += 5;
			for(k=0;k<15;k++)
			{
				if((TempBuf[j] == 0x20) || (TempBuf[j] == 0x0a))
				{
					BackTermNetMaskAddr[k] = 0x00;
					IsFlg++;
					break;
				}
				BackTermNetMaskAddr[k] = TempBuf[j];
				j++;
			}
		}
	}
	if(IsFlg != 4)
	{
		printf("IP 찾기 실패\n");
		return 1;
	}
	for(i=0,j=0;i<15;i++)
	{
		if(j == 3)
		{
			BackTermGateWayAddr[i] = '1';
			BackTermGateWayAddr[i+1] = 0x00;
			break;
		}
		if(BackTermIpAddr[i] == '.')
		{
			j++;
		}
		BackTermGateWayAddr[i] = BackTermIpAddr[i];
	}
	printf("IP 찾기 성공\n");
#endif
	fp = fopen("/etc/resolv.conf", "r");
	if(fp == NULL){
		printf("FILE 찾기 실패\n");
		return 1;
	}
	for(i = 0; !feof(fp) && (i<1024); i++){
		cData = (unsigned char)fgetc(fp);
		TempBuf[i] = cData;
	}
	fclose(fp);
	IsFlg = 0;
	for(j=0;j<i;j++)
	{
		if(IsFlg == 2)
		{
			break;
		}
		if(!memcmp("nameserver ",&TempBuf[j],11))
		{
			j += 11;
			for(k=0;k<15;k++)
			{
				if((TempBuf[j] == 0x20) || (TempBuf[j] == 0x0a))
				{
					if(IsFlg)
					{
						BackTermDNS2[k] = 0x00;
					}
					else
					{
						BackTermDNS1[k] = 0x00;
					}
					IsFlg++;
					break;
				}
				if(IsFlg)
				{
					BackTermDNS2[k] = TempBuf[j];
				}
				else
				{
					BackTermDNS1[k] = TempBuf[j];
				}
				j++;
			}
		}
	}
	if(!IsFlg)
	{
		printf("DNS 찾기 실패\n");
		return 1;
	}

//	aDebugMsg("BackTermIpAddr = %s\n",BackTermIpAddr);
//	aDebugMsg("BackTermGateWayAddr = %s\n",BackTermGateWayAddr);
//	aDebugMsg("BackTermNetMaskAddr = %s\n",BackTermNetMaskAddr);
//	aDebugMsg("BackTermDNS1 = %s\n",BackTermDNS1);
//	aDebugMsg("BackTermDNS2 = %s\n",BackTermDNS2);

	StrnCpy(ip, BackTermIpAddr, 15);
	StrnCpy(gateway, BackTermGateWayAddr, 15);
	StrnCpy(subnet_mask, BackTermNetMaskAddr, 15);
	StrnCpy(dns1, BackTermDNS1, 15);
	StrnCpy(dns2, BackTermDNS2, 15);
//	SaveFlash(FLASH_BASE_PARMETER,&pParmeter,sizeof(stParmeter));
	return 0;
}
//----------------------------------------------------------------------------------------
// 성공:0, 실패:1
//----------------------------------------------------------------------------------------
u8 ifconfig_mac_read(u8 *mac_addr)
{
    byte buf[128];
    int  cnt;
    FILE *fp;
    
    sprintf(buf, "ifconfig eth0 > /tmp/temp.txt");
    system(buf);
    fp = fopen("/tmp/temp.txt", "r");
    if (fp==NULL)
    {
        printf("file open error [/tmp/temp.txt]\n");
        return 1;
    }
    else
    {
        while(1)
        {
            memset(buf, 0, sizeof(buf));
            if (fgets(buf, sizeof(buf), fp)==NULL) break;
            
            for(cnt=0; cnt<strlen(buf); cnt++)
            {
                if (!memcmp(&buf[cnt], "Ethernet  HWaddr", 16) && strlen(&buf[cnt+16]))
                {
                    memcpy(mac_addr, &buf[cnt+16], 18);
//                    memcpy(mac_addr, &buf[cnt+16], strlen(&buf[cnt+16]));
//                    if (mac_addr[strlen(mac_addr)-1] == '\n') mac_addr[strlen(mac_addr)-1] = 0;
                    erase_space(mac_addr, 0);
                    break;
                }
            }
            if (strlen(mac_addr)) break;
        }
        fclose(fp);
    }
	system("rm /tmp/temp.txt");
	return 0;
}
//----------------------------------------------------------------------------------------
#include <stdarg.h> 
void aDebugMsg(const char *format, ...)
{
	char	szStr[4096];
	int		count;
	va_list	arg;
//	debug_mod=0;
//    if(debug_mod==0) return;
	memset(szStr, 0x00, sizeof(szStr));
	va_start(arg, format);
	count = vsprintf(szStr, format, arg);
	va_end(arg);
    printf("=================================================================\n");
	printf("%s \n", szStr);
}
//----------------------------------------------------------------------------------------------
void centerposition(u8 *dest, u8 *src, u32 len, u8 filler)
{
	u32 nspace;
	if (strlen(src) >= len )
		memcpy(dest, src, len);
	else
	{
		nspace = (len-strlen(src))/2;
		memset(dest, filler, len);
		memcpy(&dest[nspace], src, strlen(src));
	}
	memset(&dest[len], 0, 1);
}
//----------------------------------------------------------------------------------------------
void del_space(u8 *des, u8 *src, int len)
{
	u8 tbuf[2048];
	
	memset(tbuf, 0, sizeof(tbuf));
	memcpy(tbuf, src, len);
	erase_space(tbuf, 3);
	sprintf(des, "%s", tbuf);
}
//----------------------------------------------------------------------------------------------
void send_msg_cancel(void)
{
#ifndef SELF_DEBUG	
	t_qdata qdata;

	memset(&qdata, 0, sizeof(t_qdata));
	qdata.msg_type = 1;
	qdata.command_type = qETC;
	qdata.command_code  = qCANCEL;
	qdata.response_code=0;
	send_msgq(&qdata, 1);
	aDebugMsg("[logic cancel] CANCEL Message sended...!");
#endif
}
//----------------------------------------------------------------------------------------------
void msg_to_ui(int cmd_type, int cmd_code, int response)
{
#ifndef SELF_DEBUG	
	t_qdata qdata;

	memset(&qdata, 0, sizeof(t_qdata));
	qdata.msg_type = 1;
	qdata.command_type  = cmd_type;
	qdata.command_code  = cmd_code;
	qdata.response_code = response;
	send_msgq(&qdata, 1);
	aDebugMsg("LOGIC->UI Sended");
	HexDump((byte *)&qdata,256);
#endif
}
//----------------------------------------------------------------------------------------------
void send_msg_ui(int cmd_type, int cmd_code, int response)
{
#ifndef SELF_DEBUG	
	t_qdata qdata;

	memset(&qdata, 0, sizeof(t_qdata));
	qdata.msg_type = 1;
	qdata.command_type  = cmd_type;
	qdata.command_code  = cmd_code;
	qdata.response_code = response;
	qdata.qbillcoin_resp.t_field.machine = 1;
	qdata.qbillcoin_resp.t_field.input_amount = Region01.Issue_Info.bill_input_amount;
	send_msgq(&qdata, 1);
#endif
}
//-----------------------------------------------------------------------------------------------------------------------------
void adjust_amount_cal(ulong *in_amount, ulong *app_amount, ulong *can_amount, ulong *net_can_amount, ulong *tempcan_amount, ulong fr, ulong frtm, ulong to, ulong totm)
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

	long  app_cnt, can_cnt, temp_can_cnt;	// 발권승인, 발권취소, 부분납취소 Count
	ulong app_amt,can_amt,temp_can_amt, net_can_amt;  	// 발권승인, 발권취소, 부분납취소 금액

	qfront   = Region02.c_qfront;
	qrear	 = Region02.c_qrear;

	app_cnt=can_cnt=temp_can_cnt=0;
	app_amt=can_amt=temp_can_amt=net_can_amt=0;
	
	capture = c_cap1;              // 집계영역시작번지        

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
	*in_amount = app_amt+temp_can_amt+net_can_amt; // 기기 내 총금액
	*app_amount = app_amt; // 총 승인금액
	*can_amount = can_amt; // 취소환불금액
	*net_can_amount = net_can_amt;
	*tempcan_amount = temp_can_amt; // 부분납환불금액
}
//------------------------------------------------------------------------------------------------------
// success:0, other fail
// 마지막에 널을 붙인다.
//------------------------------------------------------------------------------------------------------
int file2mem(byte *des, byte *src_file_path, int len)
{
	FILE *fp;
	int ch;
	int i;

	fp = fopen(src_file_path, "r");
	if(fp == NULL){
		printf("FILE 찾기 실패:%s\n", src_file_path);
		return 1;
	}
//	for(i=0; !feof(fp) && (i<len); i++)
	for(i=0; i<len; i++)
	{
		ch = fgetc(fp);
		if(ch  == -1 || ch < 20) // 파일의 끝이면
		{
			des[i] = 0;
			break;
		}
		des[i] = (byte)ch;
	}
	des[i]=0;
	fclose(fp);
}
//----------------------------------------------------------------------------------------------------------------------------------------------
byte soundthread_alive = 0;
pthread_mutex_t sound_mutex = PTHREAD_MUTEX_INITIALIZER;
//----------------------------------------------------------------------------------------------------------------------------------------------
void *MsgTread3(void * data)
{
	pthread_mutex_lock(&sound_mutex);
	soundthread_alive = 1;
	sound_play(data);
	soundthread_alive = 0;
	pthread_mutex_unlock(&sound_mutex);
	return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------
void play_sound(byte *file)
{
	pthread_t ptest;
	int thread_status;
	int aa, i;
	byte tbuf[128];
	
	if(soundthread_alive==0)
	{
		if(pthread_create(&ptest, NULL, MsgTread3, (void*)file)<0)
		{
			aDebugMsg("Sound 쓰레드 생성 실패1");
		}
		else
		{
			aDebugMsg("Sound 쓰레드 생성 성공1");
		}
	}
	else
	{
		file2mem(tbuf, "/app/apid.dat", 127);
		i = atoi(tbuf);
		printf("\n>>>>>>>>>>>>>>>> aplay pid = %d", i);
		kill(i, SIGABRT);
	    pthread_join(ptest, (void*) &thread_status);
		if(pthread_create(&ptest, NULL, MsgTread3, (void*)file)<0)
		{
			aDebugMsg("Sound 쓰레드 생성 실패2");
		}
		else
		{
			aDebugMsg("Sound 쓰레드 생성 성공2");
		}
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------
void sound_play(byte *file)
{
	byte tbuf[256];
	sprintf(tbuf, "aplay %s --process-id-file /app/apid.dat", file);
	printf("\n%s\n", tbuf);
	system(tbuf);
}
//----------------------------------------------------------------------------------------------------------------------------------------------
void sound_init(byte vol)
{
	byte tbuf[128];
	sprintf(tbuf, "amixer set Speaker %d", vol);
	system(tbuf);
}
//----------------------------------------------------------------------------------------------------------------------------------------------
// 2014.09.05 by KHD
// 실행되는 폴더 밑에 ./log 폴더를 생성하여 날짜별로 로그를 기록
// 최근 10일까지의 로그만 남기도록 구현
//----------------------------------------------------------------------------------------------------------------------------------------------
void log_printf(byte log_level, const char *format, ...)
{
	char szTime[16];
	char szFileName[255];
    byte szBuf[4096];
	FILE *fp;
	int tmpLen;
	va_list arg;

	if(!(log_level <= Region01.log_depth)) return;
	
	get_dtm(szTime);
	sprintf(szFileName,"./log/20%2.2s%2.2s%2.2s.log", &szTime[0], &szTime[2], &szTime[4]);

	memset(szBuf, 0, sizeof(szBuf));
	va_start(arg, format);
    tmpLen = vsprintf(szBuf, format, arg);
    va_end(arg);
	szBuf[tmpLen] = 0x00;

	if ( (fp = fopen(szFileName, "a+")) == NULL )
	{
		system("mkdir ./log");
		if ( (fp = fopen(szFileName, "a+")) != NULL )
		{
			fprintf(fp,"[20%2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s] %s\n\r", &szTime[0], &szTime[2], &szTime[4], &szTime[6], &szTime[8], &szTime[10], szBuf);
			fclose(fp);
		}		
	}
	else
	{
		fprintf(fp,"[20%2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s] %s\n\r", &szTime[0], &szTime[2], &szTime[4], &szTime[6], &szTime[8], &szTime[10], szBuf);
		fclose(fp);
	}
	// 가장 최근의 10개 파일만(10일치) 로그로 남긴다.
	system("ls -td1 ./log/*.log | tail -n +11 | xargs rm -f"); // ls -td1 *.log : 파일목록을 시간내림차순으로 출력, | tail -n +11 : 출력되는 11번째 파일부터 출력, | xargs rm -f 출력되는 11번째 파일부터 rm명령으로 삭제
}
//----------------------------------------------------------------------------------------------------------------------------------------------
// string 버퍼인 src 버퍼의 내용에 pos 위치에 ch문자를 삽입하여 des 버퍼로 저장한다. (마지막에 널을 채움)
// count는 pos 위치에 삽입하는 반복 횟수를 의미한다. 0인 경우 src버퍼 전체크기에 대하여 pos위치마다 삽입함을 의미한다.
// 2014.10.01 by KHD
//----------------------------------------------------------------------------------------------------------------------------------------------
void insert_char_at(byte *des, byte *src, int pos, byte ch, byte count)
{
	int i, j;
	if(pos==0)	return;

	if(count == 0)
	{
		for(i=0,j=0; i<strlen(src); i++, j++)
		{
			if( ((i % pos)==0) && (i != 0) )
			{
				des[j++] = ch;
			}
			des[j] = src[i];
		}
		des[j] = 0;
	}
	else
	{
		for(i=0,j=0; i<strlen(src); i++, j++)
		{
			if( ((i % pos)==0) && (i != 0) && count != 0 )
			{
				des[j++] = ch;
				count--;
			}
			des[j] = src[i];
		}
		des[j] = 0;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------
void erase_char(byte *ostr, byte ch)
{
    int i, j, k;
    byte erbuf[1024*2]={0};

    j = Strlen(ostr);
    Memset(erbuf, 0, sizeof(erbuf));
    for(i=0,k=0; i<j; i++)
    {
        if (ostr[i] != ch) erbuf[k++] = ostr[i];
    }
    Memcpy(ostr, erbuf, k);
    ostr[k] = 0;
    return;
}
//----------------------------------------------------------------------------------------------------------------------------------------------

