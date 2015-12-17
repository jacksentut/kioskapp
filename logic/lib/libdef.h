
#define LIB_RELEASE_VERSION "140808"	//라이브러리 배포 버전

/////////////////////////////////////////////////////////
// libautoprinter.c
/////////////////////////////////////////////////////////
#define TH_3INCH                  // 3inch printer
#define TH_SMARTROVAN

#define STANDARD_MODE           0
#define PAGE_MODE               1

#define ON                      1
#define OFF                     0

#define FONT_B                  0
#define FONT_A                  1

#define UNDER_LINE_OFF          0
#define UNDER_LINE_ONE          1
#define UNDER_LINE_TWO          2

// ESC ! n : Select print mode definition
#define FONT_MODE               0x01
#define REVERSED_MODE           0x01
#define EMPHASIZE_MODE          0x08
#define DOUBLE_HIGHT_MODE       0x10
#define DOUBLE_WIDTH_MODE       0x20
#define CHAR_LNIE_PRINT_MODE    0x40
#define UNDERLINE_MODE          0x80

#define ENG_FONT_SIZE           12
#define KOR_FONT_SIZE           24
#define SPC_FONT_SIZE           24
#define FONT_HIGHT_SIZE         24

//#define DEFAULT_PRINT_MODE          STANDARD_MODE
#define DEFAULT_PRINT_MODE          PAGE_MODE
#define DEFAULT_FONT_TYPE           FONT_A
#define DEFAULT_RESIZING_ROW        1
#define DEFAULT_RESIZING_COL        1
#define DEFAULT_TABL_SIZE           48
#define DEFAULT_SPACING_RIGHT       0     // 자간 //전표 글자간 마진 : 짝수로만 줄것	//0

// 2010/12/28 MPOS_CATPOS 관련추가 HKSHIN_CORRECT
//#define DEFAULT_SPACING_BOTTOM      8       // 줄간격
#define DEFAULT_SPACING_BOTTOM      0       // 줄간격
#define DEFAULT_UNDERLINE_MODE      UNDER_LINE_OFF
#define DEFAULT_EMPHASIZED_MODE     OFF
#define DEFAULT_REVERSED_MODE       OFF
#define DEFAULT_BARCODE_WIDTH       2
#define DEFAULT_BARCODE_HEIGHT      162

#define DEFAULT_HRI_POSITION        2 //20110126_RDK_ 바코드 아래출력 포지션은 2가 기본값임 3은 위아래 동시 출력
#define IMAGE_MODE0             0           // 8-dot single-density
#define IMAGE_MODE1             1           // 8-dot double-density
#define IMAGE_MODE2             32          // 24-dot single-density
#define IMAGE_MODE3             33          // 24-dot double-density

// Bar Code System  //20110126_RDK_ 입력값이 숫자가 아닌 핵사값
#define UPC_A       65
#define UPC_E       66
#define JAN13       67
#define JAN8        68
#define CODE39      69
#define ITF         70
#define CODABAR     71
#define CODE93      72
#define CODE128     73
// (110105, HKSHIN)
#define EAN_13      JAN13
// (110127, HKSHIN)
#define UPC_A_1     0x65
#define UPC_E_1     0x66
#define JAN13_1     0x67
#define JAN8_1      0x68
#define CODE39_1    0x69
#define ITF_1       0x70
#define CODABAR_1   0x71
#define CODE93_1    0x72
#define CODE128_1   0x73

// (110210, HKSHIN)
#define UPC_A_0     0
#define UPC_E_0     1
#define JAN13_0     2
#define JAN8_0      3
#define CODE39_0    4
#define ITF_0       5
#define CODABAR_0   6


#define AC_FULL     0
#define AC_PARTIAL  1
#define AC_CUTINIT  2
                        
#define SELF_PRINTER_MODE       0
#define RECEIPT_PRINTER_MODE    1

// The maximum paper feed amount is approximately 900mm {35.5”} if [n X vertical motion unit] exceeds 900mm {35.5”}.
// 900mm / 0.125mm = 7200
#define MAX_COL_SIZE        7200
//#define MAX_COL_SIZE        2000

#define MAX_ROW_SIZE        72    // 3inch
#define DOTS_PER_LINE       576

#define START_DOT       0	  //KFTC 및 기타 전표 시작 지점 : 짝수로만 줄것	//24	->전표글자수 48자

#define ENGLISH_SIZE_PER_FONT   48
#define KOREAN_SIZE_PER_FONT    72
#define SPECIAL_SIZE_PER_FONT   72
#define END_DOT             (DOTS_PER_LINE - START_DOT)

#define VERTI_DOT_DENSITY0      3
#define VERTI_DOT_DENSITY1      1 
#define HORIZ_DOT_DENSITY0      2 
#define HORIZ_DOT_DENSITY1      1
#define BRIGHT_130P             1
#define BRIGHT_120P             2
#define BRIGHT_110P             3
#define BRIGHT_100P             4

#define MAX_LINE_STRING_SIZE	48
#define MIN_LINE_STRING_DOTS	42 * ENG_FONT_SIZE

int printfd;
int timerfd;
int muxfd;
int uart0_fd, uart1_fd, uart2_fd, uart3_fd;

