

int Control_NoOperation(void);
int Control_StartOfText(void);
int Control_HorizonTab(void);           
int Control_LineFeed(void);             
int Control_VerticalTab(void);            
int Control_FormFeed(void);            
int Control_CarriageReturn(void);       
int Control_DLE(void);                 
int Control_ShiftOut(void);                
int Control_ShiftIn(void);                   
int Control_Cancel(void);               
int Control_Escape(void);                
int Control_FS(void);
int Control_GS(void);



int Esc_FormFeed(void);             
int Esc_RightSideSpace(void);             
int Esc_SelectPrintMode(void);               
int Esc_SetAbsPosition(void);      
int Esc_UserDefineCharSet(void);
int Esc_UserDefineChar(void);
int Esc_ImageMode(void);
int Esc_ImageMode_01(void);
int Esc_NoOperation(void);
int Esc_Underline(void);
int Esc_SelectDefaultLineSpacing(void);
int Esc_SetLineSpacing(void);
int Esc_ReturnHome(void);
int Esc_SelectPeripherDev(void);
int Esc_UserDefCharCancel(void);
int Esc_Initialize(void);
int Esc_SetHTPosition(void);
int Esc_Emphasize(void);
int Esc_DoubleStrike(void);
int Esc_PrintandFeed(void);
int Esc_PrintAndReverseFeed(void);
int Esc_SelectPageMode(void);
int Esc_SelectCharFont(void);
int Esc_SelectIntCharSet_SetToNVmemory(void);
int Esc_SelectStandardMode(void);
int Esc_DirectionMode(void);
int Esc_RotateClockwise(void);
int Esc_SetPrintAreaInPageMode(void);
int Esc_SetRelativePrtPosition(void);
int Esc_SetPositionAlignment(void);
int Esc_PaperSensorPanel(void);
int Esc_PrintnlineFeed(void);
int Esc_PrintandReverseFeednlines(void);
int Esc_GeneratePulse(void);
int Esc_SelectCodeTable(void);
int Esc_UpsideDownMode(void);
//int Esc_Paper_fullcut(void);                  // Hiena_101217
int Esc_Paper_partialcut(void);
int Esc_Select_Print_Color(void);               // Hiena_110113
int Esc_TransmitPeripheralDeviceStatus(void);   // Hiena_110117
int Esc_TransmitPaperSensorStatus(void);	    // SNG_110114
int Esc_DefineMacro_ExecuteMacro(void);         // Hiena_110117
int Esc_PrintDataInPageMode(void);              // Hiena_110117





int GS_NoOperation(void);
int GS_CutPaper(void);
int GS_Print_Barcode(void);
int GS_Select_barcode_height(void);
int GS_Set_barcode_width(void);
int GS_Set_left_margin(void);
int GS_Select_HRI_position(void);
int GS_Image_print(void);
int GS_SelectCharSize(void);                    // Hiena_110111
int GS_Select_font_HRI_Char(void);              // Hiena_110111
int GS_Get_Printer_Status(void);                // Hiena_110111
int GS_SetAbsoluteVerticalPrintPosition(void);  // Hiena_110117
int GS_Hex28Function(void);                     // Hiena_110117
int GS_DefineDownloadBitImage(void);            // Hiena_110117
int GS_PrintDownloadBitImage(void);             // Hiena_110117
int GS_StartEndMacroDefinition(void);           // Hiena_110117
int GS_BlackMarkInitialization(void);           // Hiena_110117
int GS_TurnsWB_ReversePrintingModeOnOff(void);  // Hiena_110117
int GS_TransmitPrinterID_BatteryStatus(void);   // Hiena_110117
int GS_SetPrintPosBeginningOfPrintLine(void);   // Hiena_110117
int GS_SetPrintingAreaWidth(void);              // Hiena_110117
int GS_SetRelativeVerticalPrintPosition(void);  // Hiena_110117
int GS_ExecuteMacro(void);                      // Hiena_110117
int GS_EnableDisableASB(void);                  // Hiena_110117

int FS_NoOperation(void);
int FS_SetPrintModeForKanjiCharacters(void);    //SNG_110114
int FS_SelectKanjiCharacterMode(void);          //SNG_110114
int FS_TurnUnderlineModeOnOff(void);            //SNG_110114
int FS_CancelKanjiCharacterMode(void);          //SNG_110114
int FS_SpecifyUserDefinedKanjiCharacters(void);
int FS_Print_Bright(void);
int FS_NVbit_Image(void);                       // Hiena_110111
int FS_DefineNVbitImage(void);                  // Hiena_110117
int FS_SetKanjiCharacterSpacing(void);          // Hiena_110117
int FS_SetQuadrupleOnOffForKanjiChar(void);     // Hiena_110117
int FS_CancelUser_definedKanjiChar(void);       // Hiena_110117

struct {                                        
    int  (*controlfunc)( void );
} Printer_Ctrl_Sequence[32] = {                          
    {Control_NoOperation},                  //0x00 NUL
    {Control_NoOperation},                  //0x01 SOH
    {Control_StartOfText},                  //0x02 STX
    {Control_NoOperation},                  //0x03 ETX
    {Control_NoOperation},                  //0x04 EOT
    {Control_NoOperation},                  //0x05 ENQ
    {Control_NoOperation},                  //0x06 ACK
    {Control_NoOperation},                  //0x07 BEL
    {Control_NoOperation},                  //0x08 BS
    {Control_HorizonTab},                   //0x09 TAB
    {Control_LineFeed},                     //0x0a LF
    {Control_VerticalTab},                  //0x0b VT
    {Control_FormFeed},                     //0x0c FF
    {Control_CarriageReturn},               //0x0d CR
    {Control_ShiftOut},                     //0x0e SO 
    {Control_ShiftIn},                      //0x0f SI                                                          
    {Control_DLE},                          //0x10 DLE                                                         
    {Control_NoOperation},                  //0x11 DC1                                                         
    {Control_NoOperation},                  //0x12 DC2                                                         
    {Control_NoOperation},                  //0x13 DC3                                                      
    {Control_NoOperation},                  //0x14 DC4
    {Control_NoOperation},                  //0x15 NAK
    {Control_NoOperation},                  //0x16 SYN
    {Control_NoOperation},                  //0x17 ETB                                                   
    {Control_Cancel},                       //0x18 CANCEL
    {Control_NoOperation},                  //0x19 EM                                                   
    {Control_NoOperation},                  //0x1a SUB
    {Control_Escape},                       //0x1b ESC  
    {Control_FS},                           //0x1c FS
    {Control_GS},                           //0x1d GS                                                   
    {Control_NoOperation},                  //0x1e RS
    {Control_NoOperation}                   //0x1f US
};                                                  
                                                     
struct {                                        
    int  (*escfunc)( void );
} Printer_ESC_Sequence[128] = {                          
    {Esc_NoOperation},                       //0x00     
    {Esc_NoOperation},                       //0x01     
    {Esc_NoOperation},                       //0x02     
    {Esc_NoOperation},                       //0x03     
    {Esc_NoOperation},                       //0x04     
    {Esc_NoOperation},                       //0x05     
    {Esc_NoOperation},                       //0x06     
    {Esc_NoOperation},                       //0x07     
    {Esc_NoOperation},                       //0x08     
    {Esc_NoOperation},                       //0x09     
    {Esc_NoOperation},                       //0x0A     
    {Esc_NoOperation},                       //0x0B     
    {Esc_FormFeed},                          //0x0C     
    {Esc_NoOperation},                       //0x0D     
    {Esc_NoOperation},                       //0x0E     
    {Esc_NoOperation},                       //0x0F     
    {Esc_NoOperation},                       //0x10     
    {Esc_NoOperation},                       //0x11     
    {Esc_NoOperation},                       //0x12     
    {Esc_NoOperation},                       //0x13     
    {Esc_NoOperation},                       //0x14     
    {Esc_NoOperation},                       //0x15     
    {Esc_NoOperation},                       //0x16     
    {Esc_NoOperation},                       //0x17     
    {Esc_NoOperation},                       //0x18     
    {Esc_NoOperation},                       //0x19     
    {Esc_NoOperation},                       //0x1A     
    {Esc_NoOperation},                       //0x1B                           
    {Esc_NoOperation},                       //0x1C     
    {Esc_NoOperation},                       //0x1D     
    {Esc_NoOperation},                       //0x1E     
    {Esc_NoOperation},                       //0x1F                           
    {Esc_RightSideSpace},                    //0x20  ' '
    {Esc_SelectPrintMode},                   //0x21  '!'
    {Esc_NoOperation},                       //0x22  '"'
    {Esc_NoOperation},                       //0x23  '#'
    {Esc_SetAbsPosition},                    //0x24  '$'
    {Esc_UserDefineCharSet},                 //0x25  '%'
    {Esc_UserDefineChar},                    //0x26  '&'
    {Esc_NoOperation},                       //0x27  '''
    {Esc_NoOperation},                       //0x28  '('
    {Esc_NoOperation},                       //0x29  ')'
    {Esc_ImageMode},                         //0x2A  '*'
    {Esc_ImageMode_01},                      //0x2B  '+'
    {Esc_NoOperation},                       //0x2C  ','
    {Esc_Underline},                         //0x2D  '-'
    {Esc_NoOperation},                       //0x2E  '.'
    {Esc_NoOperation},                       //0x2F  '/'
    {Esc_NoOperation},                       //0x30  '0'
    {Esc_NoOperation},                       //0x31  '1'
    {Esc_SelectDefaultLineSpacing},          //0x32  '2'                                
    {Esc_SetLineSpacing},                    //0x33  '3'
    {Esc_NoOperation},                       //0x34  '4'
    {Esc_NoOperation},                       //0x35  '5'
    {Esc_NoOperation},                       //0x36  '6'
    {Esc_NoOperation},                       //0x37  '7'
    {Esc_NoOperation},                       //0x38  '8'
    {Esc_NoOperation},                       //0x39  '9'
    {Esc_NoOperation},                       //0x3a  ':'
    {Esc_NoOperation},                       //0x3b  ';'
    {Esc_ReturnHome},                        //0x3c  '<'
    {Esc_SelectPeripherDev},                 //0x3d  '='                          
    {Esc_NoOperation},                       //0x3e  '>'
    {Esc_UserDefCharCancel},                 //0x3f  '?'                                
    {Esc_Initialize},                        //0x40  '@'                                
    {Esc_NoOperation},                       //0x41  'A'
    {Esc_NoOperation},                       //0x42  'B'
    {Esc_NoOperation},                       //0x43  'C'
    {Esc_SetHTPosition},                     //0x44  'D'
    {Esc_Emphasize},                         //0x45  'E'
    {Esc_NoOperation},                       //0x46  'F'
    {Esc_DoubleStrike},                      //0x47  'G'
    {Esc_NoOperation},                       //0x48  'H'
    {Esc_NoOperation},                       //0x49  'I'
    {Esc_PrintandFeed},                      //0x4A  'J'
    {Esc_PrintAndReverseFeed},               //0x4B  'K'
    {Esc_SelectPageMode},                    //0x4C  'L'                                        
    {Esc_SelectCharFont},                    //0x4D  'M'                                
    {Esc_NoOperation},                       //0x4E  'N'
    {Esc_NoOperation},                       //0x4F  'O'
    {Esc_NoOperation},                       //0x50  'P'
    {Esc_NoOperation},                       //0x51  'Q'
    {Esc_SelectIntCharSet_SetToNVmemory},    //0x52  'R'                                
    {Esc_SelectStandardMode},                //0x53  'S'                                
    {Esc_DirectionMode},                     //0x54  'T'                          
    {Esc_NoOperation},                       //0x55  'U'
    {Esc_RotateClockwise},                   //0x56  'V'                                
    {Esc_SetPrintAreaInPageMode},            //0x57  'W'                                
    {Esc_NoOperation},                       //0x58  'X'
    {Esc_NoOperation},                       //0x59  'Y'
    {Esc_NoOperation},                       //0x5A  'Z'
    {Esc_NoOperation},                       //0x5B  '['
    {Esc_SetRelativePrtPosition},            //0x5c  '\'                                
    {Esc_NoOperation},                       //0x5d  ']'
    {Esc_NoOperation},                       //0x5e  '^'
    {Esc_NoOperation},                       //0x5f  '_'
    {Esc_NoOperation},                       //0x60  '`'
    {Esc_SetPositionAlignment},              //0x61  'a'                                
    {Esc_NoOperation},                       //0x62  'b'                                
    {Esc_PaperSensorPanel},                  //0x63  'c'                                
    {Esc_PrintnlineFeed},                    //0x64  'd'                                
    {Esc_PrintandReverseFeednlines},         //0x65  'e'
    {Esc_NoOperation},                       //0x66  'f'
    {Esc_DefineMacro_ExecuteMacro},          //0x67  'g'
    {Esc_NoOperation},                       //0x68  'h'
    {Esc_Paper_partialcut},                  //0x69  'i'    // Hiena_101217
    {Esc_NoOperation},                       //0x6a  'j'
    {Esc_NoOperation},                       //0x6b  'k'
    {Esc_NoOperation},                       //0x6c  'l'
    {Esc_Paper_partialcut},                  //0x6d  'm'    // Hiena_110111
    {Esc_NoOperation},                       //0x6e  'n'
    {Esc_NoOperation},                       //0x6f  'o'
    {Esc_GeneratePulse},                     //0x70  'p'                          
    {Esc_NoOperation},                       //0x71  'q'
    {Esc_Select_Print_Color},                //0x72  'r'
    {Esc_NoOperation},                       //0x73  's'
    {Esc_SelectCodeTable},                   //0x74  't'                                
    {Esc_TransmitPeripheralDeviceStatus},    //0x75  'u'    // Hiena_110117
    {Esc_TransmitPaperSensorStatus},         //0x76  'v'    
    {Esc_NoOperation},                       //0x77  'w'
    {Esc_NoOperation},                       //0x78  'x'
    {Esc_NoOperation},                       //0x79  'y'
    {Esc_NoOperation},                       //0x7a  'z'
    {Esc_UpsideDownMode},                    //0x7b  '{'                              
    {Esc_NoOperation},                       //0x7c  '|'
    {Esc_NoOperation},                       //0x7d  '}'
    {Esc_NoOperation},                       //0x7e  '~'
    {Esc_NoOperation}                        //0x7f  DEL
};                                                  
 
struct {                                        
    int  (*gsfunc)( void );
} Printer_GS_Sequence[128] = {
    {GS_NoOperation},                       //0x00                                                     
    {GS_NoOperation},                       //0x01                                                     
    {GS_NoOperation},                       //0x02                                                     
    {GS_NoOperation},                       //0x03                                                     
    {GS_NoOperation},                       //0x04                                                     
    {GS_NoOperation},                       //0x05                                                     
    {GS_NoOperation},                       //0x06                                                     
    {GS_NoOperation},                       //0x07                                                     
    {GS_NoOperation},                       //0x08                                                     
    {GS_NoOperation},                       //0x09                                                     
    {GS_NoOperation},                       //0x0A                                                     
    {GS_NoOperation},                       //0x0B                                                     
    {GS_NoOperation},                       //0x0C                                                     
    {GS_NoOperation},                       //0x0D                                                     
    {GS_NoOperation},                       //0x0E                                                 
    {GS_NoOperation},                       //0x0F                                                     
    {GS_NoOperation},                       //0x10                                                     
    {GS_NoOperation},                       //0x11                                                     
    {GS_NoOperation},                       //0x12                                                     
    {GS_NoOperation},                       //0x13                                                     
    {GS_NoOperation},                       //0x14                                                     
    {GS_NoOperation},                       //0x15                                                     
    {GS_NoOperation},                       //0x16                                                     
    {GS_NoOperation},                       //0x17                                                     
    {GS_NoOperation},                       //0x18                                                     
    {GS_NoOperation},                       //0x19                                                     
    {GS_NoOperation},                       //0x1A                                                     
    {GS_NoOperation},                       //0x1B                                                     
    {GS_NoOperation},                       //0x1C                                                     
    {GS_NoOperation},                       //0x1D                                                     
    {GS_NoOperation},                       //0x1E                                                     
    {GS_NoOperation},                       //0x1F                                                         
    {GS_NoOperation},                       //0x20  ' '                                                
    {GS_SelectCharSize},                    //0x21  '!'   // Hiena_110111
    {GS_NoOperation},                       //0x22  '"'                                                
    {GS_NoOperation},                       //0x23  '#'                                                
    {GS_SetAbsoluteVerticalPrintPosition},  //0x24  '$'                                                
    {GS_NoOperation},                       //0x25  '%'                                                
    {GS_NoOperation},                       //0x26  '&'                                                
    {GS_NoOperation},                       //0x27  '''                                                
    {GS_Hex28Function},                     //0x28  '('                                                
    {GS_NoOperation},                       //0x29  ')'                                                
    {GS_DefineDownloadBitImage},            //0x2A  '*'                                                
    {GS_NoOperation},                       //0x2B  '+'                                                
    {GS_NoOperation},                       //0x2C  ','                                                
    {GS_NoOperation},                       //0x2D  '-'                                                
    {GS_NoOperation},                       //0x2E  '.'                                                
    {GS_PrintDownloadBitImage},             //0x2F  '/'                                                
    {GS_NoOperation},                       //0x30  '0'                       
    {GS_NoOperation},                       //0x31  '1'                      
    {GS_NoOperation},                       //0x32  '2'                    
    {GS_NoOperation},                       //0x33  '3'                    
    {GS_NoOperation},                       //0x34  '4'                      
    {GS_NoOperation},                       //0x35  '5'                    
    {GS_NoOperation},                       //0x36  '6'                       
    {GS_NoOperation},                       //0x37  '7'                       
    {GS_NoOperation},                       //0x38  '8'                       
    {GS_NoOperation},                       //0x39  '9'                       
    {GS_StartEndMacroDefinition},           //0x3a  ':'                       
    {GS_NoOperation},                       //0x3b  ';'                       
    {GS_BlackMarkInitialization},           //0x3c  '<'                       
    {GS_NoOperation},                       //0x3d  '='                    
    {GS_NoOperation},                       //0x3e  '>'                       
    {GS_NoOperation},                       //0x3f  '?'                    
    {GS_NoOperation},                       //0x40  '@'                    
    {GS_NoOperation},                       //0x41  'A'                       
    {GS_TurnsWB_ReversePrintingModeOnOff},  //0x42  'B'                      
    {GS_NoOperation},                       //0x43  'C'                       
    {GS_NoOperation},                       //0x44  'D'                    
    {GS_NoOperation},                       //0x45  'E'                    
    {GS_NoOperation},                       //0x46  'F'                       
    {GS_NoOperation},                       //0x47  'G'                    
    {GS_Select_HRI_position},               //0x48  'H'                       
    {GS_TransmitPrinterID_BatteryStatus},   //0x49  'I'                       
    {GS_NoOperation},                       //0x4A  'J'                    
    {GS_NoOperation},                       //0x4B  'K'                       
    {GS_Set_left_margin},                   //0x4C  'L'
    {GS_NoOperation},                       //0x4D  'M'                    
    {GS_NoOperation},                       //0x4E  'N'                       
    {GS_NoOperation},                       //0x4F  'O'                       
    {GS_NoOperation},                       //0x50  'P'                       
    {GS_NoOperation},                       //0x51  'Q'                       
    {GS_NoOperation},                       //0x52  'R'                    
    {GS_NoOperation},                       //0x53  'S'                    
    {GS_SetPrintPosBeginningOfPrintLine},   //0x54  'T'                    
    {GS_NoOperation},                       //0x55  'U'                       
    {GS_CutPaper},                          //0x56  'V'                    
    {GS_SetPrintingAreaWidth},              //0x57  'W'                    
    {GS_NoOperation},                       //0x58  'X'                       
    {GS_NoOperation},                       //0x59  'Y'                       
    {GS_NoOperation},                       //0x5A  'Z'                       
    {GS_NoOperation},                       //0x5B  '['                       
    {GS_SetRelativeVerticalPrintPosition},  //0x5c  '\'                    
    {GS_NoOperation},                       //0x5d  ']'                       
    {GS_ExecuteMacro},                      //0x5e  '^'                       
    {GS_NoOperation},                       //0x5f  '_'                       
    {GS_NoOperation},                       //0x60  '`'                       
    {GS_EnableDisableASB},                  //0x61  'a'                    
    {GS_NoOperation},                       //0x62  'b'                      
    {GS_NoOperation},                       //0x63  'c'         
    {GS_NoOperation},                       //0x64  'd'                    
    {GS_NoOperation},                       //0x65  'e'                       
    {GS_Select_font_HRI_Char},              //0x66  'f'     // Hiena_110111
    {GS_NoOperation},                       //0x67  'g'                       
    {GS_Select_barcode_height},             //0x68  'h'                       
    {GS_NoOperation},                       //0x69  'i'                   
    {GS_NoOperation},                       //0x6a  'j'                       
    {GS_Print_Barcode},                     //0x6b  'k'                       
    {GS_NoOperation},                       //0x6c  'l'                       
    {GS_NoOperation},                       //0x6d  'm'                       
    {GS_NoOperation},                       //0x6e  'n'                       
    {GS_NoOperation},                       //0x6f  'o'                       
    {GS_NoOperation},                       //0x70  'p'                    
    {GS_NoOperation},                       //0x71  'q'                       
    {GS_Get_Printer_Status},                //0x72  'r'     // Hiena_110111   
    {GS_NoOperation},                       //0x73  's'                       
    {GS_NoOperation},                       //0x74  't'                    
    {GS_NoOperation},                       //0x75  'u'                       
    {GS_Image_print},                       //0x76  'v'     //bitmap 이미지 출력                  
    {GS_Set_barcode_width},                 //0x77  'w'                       
    {GS_NoOperation},                       //0x78  'x'                       
    {GS_NoOperation},                       //0x79  'y'                       
    {GS_NoOperation},                       //0x7a  'z'                       
    {GS_NoOperation},                       //0x7b  '('                       
    {GS_NoOperation},                       //0x7c  '|'                       
    {GS_NoOperation},                       //0x7d  '}'                       
    {GS_NoOperation},                       //0x7e  '~'                      
    {GS_NoOperation}                        //0x7f  DEL                      
};                                                    
                                                            
struct {                                        
    int  (*fsfunc)( void );
} Printer_FS_Sequence[128] = {
    {FS_NoOperation},                       //0x00                                                     
    {FS_NoOperation},                       //0x01                                                     
    {FS_NoOperation},                       //0x02                                                     
    {FS_NoOperation},                       //0x03                                                     
    {FS_NoOperation},                       //0x04                                                     
    {FS_NoOperation},                       //0x05                                                     
    {FS_NoOperation},                       //0x06                                                     
    {FS_NoOperation},                       //0x07                                                     
    {FS_NoOperation},                       //0x08                                                     
    {FS_NoOperation},                       //0x09                                                     
    {FS_NoOperation},                       //0x0A                                                     
    {FS_NoOperation},                       //0x0B                                                     
    {FS_NoOperation},                       //0x0C                                                     
    {FS_NoOperation},                       //0x0D                                                     
    {FS_NoOperation},                       //0x0E                                                 
    {FS_NoOperation},                       //0x0F                                                     
    {FS_NoOperation},                       //0x10                                                     
    {FS_NoOperation},                       //0x11                                                     
    {FS_NoOperation},                       //0x12                                                     
    {FS_NoOperation},                       //0x13                                                     
    {FS_NoOperation},                       //0x14                                                     
    {FS_NoOperation},                       //0x15                                                     
    {FS_NoOperation},                       //0x16                                                     
    {FS_NoOperation},                       //0x17                                                     
    {FS_NoOperation},                       //0x18                                                     
    {FS_NoOperation},                       //0x19                                                     
    {FS_NoOperation},                       //0x1A                                                     
    {FS_NoOperation},                       //0x1B                                                     
    {FS_NoOperation},                       //0x1C                                                     
    {FS_NoOperation},                       //0x1D                                                     
    {FS_NoOperation},                       //0x1E                                                     
    {FS_NoOperation},                       //0x1F                                                         
    {FS_NoOperation},                       //0x20  ' '                                                
    {FS_SetPrintModeForKanjiCharacters},    //0x21  '!'                                                
    {FS_NoOperation},                       //0x22  '"'                                                
    {FS_NoOperation},                       //0x23  '#'                                                
    {FS_NoOperation},                       //0x24  '$'                                                
    {FS_NoOperation},                       //0x25  '%'                                                
    {FS_SelectKanjiCharacterMode},          //0x26  '&'                                                
    {FS_NoOperation},                       //0x27  '''                                                
    {FS_NoOperation},                       //0x28  '('                                                
    {FS_NoOperation},                       //0x29  ')'                                                
    {FS_NoOperation},                       //0x2A  '*'                                                
    {FS_NoOperation},                       //0x2B  '+'                                                
    {FS_NoOperation},                       //0x2C  ','                                                
    {FS_TurnUnderlineModeOnOff},            //0x2D  '-'                                                
    {FS_CancelKanjiCharacterMode},          //0x2E  '.'                                                
    {FS_NoOperation},                       //0x2F  '/'                                                
    {FS_NoOperation},                       //0x30  '0'                       
    {FS_NoOperation},                       //0x31  '1'                      
    {FS_SpecifyUserDefinedKanjiCharacters}, //0x32  '2'                    
    {FS_NoOperation},                       //0x33  '3'                    
    {FS_NoOperation},                       //0x34  '4'                      
    {FS_NoOperation},                       //0x35  '5'                    
    {FS_NoOperation},                       //0x36  '6'                       
    {FS_NoOperation},                       //0x37  '7'                       
    {FS_NoOperation},                       //0x38  '8'                       
    {FS_NoOperation},                       //0x39  '9'                       
    {FS_NoOperation},                       //0x3a  ':'                       
    {FS_NoOperation},                       //0x3b  ';'                       
    {FS_NoOperation},                       //0x3c  '<'                       
    {FS_NoOperation},                       //0x3d  '='                    
    {FS_NoOperation},                       //0x3e  '>'                       
    {FS_CancelUser_definedKanjiChar},       //0x3f  '?'                    
    {FS_NoOperation},                       //0x40  '@'                    
    {FS_NoOperation},                       //0x41  'A'                       
    {FS_Print_Bright},                      //0x42  'B'                      
    {FS_NoOperation},                       //0x43  'C'                       
    {FS_NoOperation},                       //0x44  'D'                    
    {FS_NoOperation},                       //0x45  'E'                    
    {FS_NoOperation},                       //0x46  'F'                       
    {FS_NoOperation},                       //0x47  'G'                    
    {FS_NoOperation},                       //0x48  'H'                       
    {FS_NoOperation},                       //0x49  'I'                       
    {FS_NoOperation},                       //0x4A  'J'                    
    {FS_NoOperation},                       //0x4B  'K'                       
    {FS_NoOperation},                       //0x4C  'L'
    {FS_NoOperation},                       //0x4D  'M'                    
    {FS_NoOperation},                       //0x4E  'N'                       
    {FS_NoOperation},                       //0x4F  'O'                       
    {FS_NoOperation},                       //0x50  'P'                       
    {FS_NoOperation},                       //0x51  'Q'                       
    {FS_NoOperation},                       //0x52  'R'                    
    {FS_SetKanjiCharacterSpacing},          //0x53  'S'                    
    {FS_NoOperation},                       //0x54  'T'                    
    {FS_NoOperation},                       //0x55  'U'                       
    {FS_NoOperation},                       //0x56  'V'                    
    {FS_SetQuadrupleOnOffForKanjiChar},     //0x57  'W'                    
    {FS_NoOperation},                       //0x58  'X'                       
    {FS_NoOperation},                       //0x59  'Y'                       
    {FS_NoOperation},                       //0x5A  'Z'                       
    {FS_NoOperation},                       //0x5B  '['                       
    {FS_NoOperation},                       //0x5c  '\'                    
    {FS_NoOperation},                       //0x5d  ']'                       
    {FS_NoOperation},                       //0x5e  '^'                       
    {FS_NoOperation},                       //0x5f  '_'                       
    {FS_NoOperation},                       //0x60  '`'                       
    {FS_NoOperation},                       //0x61  'a'                    
    {FS_NoOperation},                       //0x62  'b'                      
    {FS_NoOperation},                       //0x63  'c'         
    {FS_NoOperation},                       //0x64  'd'                    
    {FS_NoOperation},                       //0x65  'e'                       
    {FS_NoOperation},                       //0x66  'f'                       
    {FS_NoOperation},                       //0x67  'g'                       
    {FS_NoOperation},                       //0x68  'h'                       
    {FS_NoOperation},                       //0x69  'i'                   
    {FS_NoOperation},                       //0x6a  'j'                       
    {FS_NoOperation},                       //0x6b  'k'                       
    {FS_NoOperation},                       //0x6c  'l'                       
    {FS_NoOperation},                       //0x6d  'm'                       
    {FS_NoOperation},                       //0x6e  'n'                       
    {FS_NoOperation},                       //0x6f  'o'                       
    {FS_NVbit_Image},                       //0x70  'p'    // Hiena_110111
    {FS_DefineNVbitImage},                  //0x71  'q'                       
    {FS_NoOperation},                       //0x72  'r'                       
    {FS_NoOperation},                       //0x73  's'                       
    {FS_NoOperation},                       //0x74  't'                    
    {FS_NoOperation},                       //0x75  'u'                       
    {FS_NoOperation},                       //0x76  'v'                       
    {FS_NoOperation},                       //0x77  'w'                       
    {FS_NoOperation},                       //0x78  'x'                       
    {FS_NoOperation},                       //0x79  'y'                       
    {FS_NoOperation},                       //0x7a  'z'                       
    {FS_NoOperation},                       //0x7b  '('                       
    {FS_NoOperation},                       //0x7c  '|'                       
    {FS_NoOperation},                       //0x7d  '}'                       
    {FS_NoOperation},                       //0x7e  '~'                      
    {FS_NoOperation}                        //0x7f  DEL                      
};      


unsigned char code93_data[128][2] = {
    {44, 30},   //0x00  NUL %U                                                 
    {43, 10},   //0x01  SOH $A                                                 
    {43, 11},   //0x02  STX $B                                             
    {43, 12},   //0x03  ETX $C                                             
    {43, 13},   //0x04  EOT $D                                             
    {43, 14},   //0x05  ENQ $E                                             
    {43, 15},   //0x06  ACK $F                                             
    {43, 16},   //0x07  BEL $G                                             
    {43, 17},   //0x08  BS  $H                                             
    {43, 18},   //0x09  HT  $I                                             
    {43, 19},   //0x0A  LF  $J                                             
    {43, 20},   //0x0B  VT  $K                                             
    {43, 21},   //0x0C  FF  $L                                             
    {43, 22},   //0x0D  CR  $M                                             
    {43, 23},   //0x0E  SO  $N                                         
    {43, 24},   //0x0F  SI  $O                                             
    {43, 25},   //0x10  DEL $P                                             
    {43, 26},   //0x11  DC1 $Q                                             
    {43, 27},   //0x12  DC2 $R                                             
    {43, 28},   //0x13  DC3 $S                                             
    {43, 29},   //0x14  DC4 $T                                             
    {43, 30},   //0x15  NAK $U                                             
    {43, 31},   //0x16  SYN $V                                             
    {43, 32},   //0x17  ETB $W                                             
    {43, 33},   //0x18  CAN $X                                             
    {43, 34},   //0x19  EM  $Y                                             
    {43, 35},   //0x1A  SUB $Z  
    {44, 10},   //0x1B  ESC %A                                                 
    {44, 11},   //0x1C  FS  %B                                                 
    {44, 12},   //0x1D  GS  %C                                                 
    {44, 13},   //0x1E  RS  %D                                                 
    {44, 14},   //0x1F  US  %E      
    {38,  0},   //0x20  ' '     
    {45, 10},   //0x21  '!' /A                                         
    {45, 11},   //0x22  '"' /B                                         
    {45, 12},   //0x23  '#' /C      
    {39,  0},   //0x24  '$' 
    {42,  0},   //0x25  '%'                                     
    {45, 15},   //0x26  '&' /F                                         
    {45, 16},   //0x27  ''' /G                                         
    {45, 17},   //0x28  '(' /H                                         
    {45, 18},   //0x29  ')' /I                                         
    {45, 19},   //0x2A  '*' /J      
    {41,  0},   //0x2B  '+'                                     
    {45, 21},   //0x2C  ',' /L      
    {36,  0},   //0x2D  '-'                                    
    {37,  0},   //0x2E  '.'                                    
    {40,  0},   //0x2F  '/'  
    { 0,  0},   //0x30  '0'                       
    { 1,  0},   //0x31  '1'                      
    { 2,  0},   //0x32  '2'                    
    { 3,  0},   //0x33  '3'                    
    { 4,  0},   //0x34  '4'                      
    { 5,  0},   //0x35  '5'                    
    { 6,  0},   //0x36  '6'                       
    { 7,  0},   //0x37  '7'                       
    { 8,  0},   //0x38  '8'                       
    { 9,  0},   //0x39  '9'     
    {45, 35},   //0x3a  ':' /Z                      
    {44, 15},   //0x3b  ';' %F                      
    {44, 16},   //0x3c  '<' %G                      
    {44, 17},   //0x3d  '=' %H                   
    {44, 18},   //0x3e  '>' %I                      
    {44, 19},   //0x3f  '?' %J                   
    {44, 31},   //0x40  '@' %V  
    {10,  0},   //0x41  'A'                       
    {11,  0},   //0x42  'B'                      
    {12,  0},   //0x43  'C'                       
    {13,  0},   //0x44  'D'                    
    {14,  0},   //0x45  'E'                    
    {15,  0},   //0x46  'F'                       
    {16,  0},   //0x47  'G'                    
    {17,  0},   //0x48  'H'                       
    {18,  0},   //0x49  'I'                       
    {19,  0},   //0x4A  'J'                    
    {20,  0},   //0x4B  'K'                       
    {21,  0},   //0x4C  'L'
    {22,  0},   //0x4D  'M'                    
    {23,  0},   //0x4E  'N'                       
    {24,  0},   //0x4F  'O'                       
    {25,  0},   //0x50  'P'                       
    {26,  0},   //0x51  'Q'                       
    {27,  0},   //0x52  'R'                    
    {28,  0},   //0x53  'S'                    
    {29,  0},   //0x54  'T'                    
    {30,  0},   //0x55  'U'                       
    {31,  0},   //0x56  'V'                    
    {32,  0},   //0x57  'W'                    
    {33,  0},   //0x58  'X'                       
    {34,  0},   //0x59  'Y'                       
    {35,  0},   //0x5A  'Z'             
    {44, 20},   //0x5B  '[' %K                      
    {44, 21},   //0x5c  '\' %L                   
    {44, 22},   //0x5d  ']' %M                      
    {44, 23},   //0x5e  '^' %N                      
    {44, 24},   //0x5f  '_' %O                      
    {44, 33},   //0x60  '`' %W  
    {46, 10},   //0x61  'a' +A                   
    {46, 11},   //0x62  'b' +B                     
    {46, 12},   //0x63  'c' +C        
    {46, 13},   //0x64  'd' +D                   
    {46, 14},   //0x65  'e' +E                      
    {46, 15},   //0x66  'f' +F                      
    {46, 16},   //0x67  'g' +G                      
    {46, 17},   //0x68  'h' +H                      
    {46, 18},   //0x69  'i' +I                  
    {46, 19},   //0x6a  'j' +J                      
    {46, 20},   //0x6b  'k' +K                      
    {46, 21},   //0x6c  'l' +L                      
    {46, 22},   //0x6d  'm' +M                      
    {46, 23},   //0x6e  'n' +N                      
    {46, 24},   //0x6f  'o' +O                      
    {46, 25},   //0x70  'p' +P                   
    {46, 26},   //0x71  'q' +Q                      
    {46, 27},   //0x72  'r' +R                      
    {46, 28},   //0x73  's' +S                      
    {46, 29},   //0x74  't' +T                   
    {46, 30},   //0x75  'u' +U                      
    {46, 31},   //0x76  'v' +V                      
    {46, 32},   //0x77  'w' +W                      
    {46, 33},   //0x78  'x' +X                      
    {46, 34},   //0x79  'y' +Y                      
    {46, 35},   //0x7a  'z' +Z  
    {44, 25},   //0x7b  '(' %P                      
    {44, 26},   //0x7c  '|' %Q                      
    {44, 27},   //0x7d  '}' %R                      
    {44, 28},   //0x7e  '~' %S                     
    {44, 29},   //0x7f  DEL %T
};
                
unsigned int code93_id[47] = {
    0x114,      //0         0  
    0x148,      //1         1  
    0x144,      //2         2  
    0x142,      //3         3  
    0x128,      //4         4  
    0x124,      //5         5  
    0x122,      //6         6  
    0x150,      //7         7  
    0x112,      //8         8  
    0x10A,      //9         9  
    0x1A8,      //A         10 
    0x1A4,      //B         11 
    0x1A2,      //C         12 
    0x194,      //D         13 
    0x192,      //E         14 
    0x18A,      //F         15 
    0x168,      //G         16 
    0x164,      //H         17 
    0x162,      //I         18 
    0x134,      //J         19 
    0x11A,      //K         20 
    0x158,      //L         21 
    0x14C,      //M         22 
    0x146,      //N         23 
    0x12C,      //O         24 
    0x116,      //P         25 
    0x1B4,      //Q         26 
    0x1B2,      //R         27 
    0x1AC,      //S         28 
    0x1A6,      //T         29 
    0x196,      //U         30 
    0x19A,      //V         31 
    0x16C,      //W         32 
    0x166,      //X         33 
    0x136,      //Y         34 
    0x13A,      //Z         35 
    0x12E,      //-         36 
    0x1D4,      //.         37 
    0x1D2,      //SPACE     38 
    0x1CA,      //$         39 
    0x16E,      ///         40 
    0x176,      //+         41 
    0x1AE,      //%         42 
    0x126,      //($)       43 
    0x1DA,      //(%)       44 
    0x1D6,      //(/)       45 
    0x132,      //(+)       46 
};      

unsigned char code39_data[128][2] = {
    {44, 30},   //0x00  NUL %U                                                 
    {43, 10},   //0x01  SOH $A                                                 
    {43, 11},   //0x02  STX $B                                             
    {43, 12},   //0x03  ETX $C                                             
    {43, 13},   //0x04  EOT $D                                             
    {43, 14},   //0x05  ENQ $E                                             
    {43, 15},   //0x06  ACK $F                                             
    {43, 16},   //0x07  BEL $G                                             
    {43, 17},   //0x08  BS  $H                                             
    {43, 18},   //0x09  HT  $I                                             
    {43, 19},   //0x0A  LF  $J                                             
    {43, 20},   //0x0B  VT  $K                                             
    {43, 21},   //0x0C  FF  $L                                             
    {43, 22},   //0x0D  CR  $M                                             
    {43, 23},   //0x0E  SO  $N                                         
    {43, 24},   //0x0F  SI  $O                                             
    {43, 25},   //0x10  DEL $P                                             
    {43, 26},   //0x11  DC1 $Q                                             
    {43, 27},   //0x12  DC2 $R                                             
    {43, 28},   //0x13  DC3 $S                                             
    {43, 29},   //0x14  DC4 $T                                             
    {43, 30},   //0x15  NAK $U                                             
    {43, 31},   //0x16  SYN $V                                             
    {43, 32},   //0x17  ETB $W                                             
    {43, 33},   //0x18  CAN $X                                             
    {43, 34},   //0x19  EM  $Y                                             
    {43, 35},   //0x1A  SUB $Z  
    {44, 10},   //0x1B  ESC %A                                                 
    {44, 11},   //0x1C  FS  %B                                                 
    {44, 12},   //0x1D  GS  %C                                                 
    {44, 13},   //0x1E  RS  %D                                                 
    {44, 14},   //0x1F  US  %E      
    {38,  0},   //0x20  ' '     
    {45, 10},   //0x21  '!' /A                                         
    {45, 11},   //0x22  '"' /B                                         
    {45, 12},   //0x23  '#' /C      
    {39,  0},   //0x24  '$' 
    {42,  0},   //0x25  '%'                                     
    {45, 15},   //0x26  '&' /F                                         
    {45, 16},   //0x27  ''' /G                                         
    {45, 17},   //0x28  '(' /H                                         
    {45, 18},   //0x29  ')' /I                                         
    {45, 19},   //0x2A  '*' /J      
    {41,  0},   //0x2B  '+'                                     
    {45, 21},   //0x2C  ',' /L      
    {36,  0},   //0x2D  '-'                                    
    {37,  0},   //0x2E  '.'                                    
    {40,  0},   //0x2F  '/'  
    { 0,  0},   //0x30  '0'                       
    { 1,  0},   //0x31  '1'                      
    { 2,  0},   //0x32  '2'                    
    { 3,  0},   //0x33  '3'                    
    { 4,  0},   //0x34  '4'                      
    { 5,  0},   //0x35  '5'                    
    { 6,  0},   //0x36  '6'                       
    { 7,  0},   //0x37  '7'                       
    { 8,  0},   //0x38  '8'                       
    { 9,  0},   //0x39  '9'     
    {45, 35},   //0x3a  ':' /Z                      
    {44, 15},   //0x3b  ';' %F                      
    {44, 16},   //0x3c  '<' %G                      
    {44, 17},   //0x3d  '=' %H                   
    {44, 18},   //0x3e  '>' %I                      
    {44, 19},   //0x3f  '?' %J                   
    {44, 31},   //0x40  '@' %V  
    {10,  0},   //0x41  'A'                       
    {11,  0},   //0x42  'B'                      
    {12,  0},   //0x43  'C'                       
    {13,  0},   //0x44  'D'                    
    {14,  0},   //0x45  'E'                    
    {15,  0},   //0x46  'F'                       
    {16,  0},   //0x47  'G'                    
    {17,  0},   //0x48  'H'                       
    {18,  0},   //0x49  'I'                       
    {19,  0},   //0x4A  'J'                    
    {20,  0},   //0x4B  'K'                       
    {21,  0},   //0x4C  'L'
    {22,  0},   //0x4D  'M'                    
    {23,  0},   //0x4E  'N'                       
    {24,  0},   //0x4F  'O'                       
    {25,  0},   //0x50  'P'                       
    {26,  0},   //0x51  'Q'                       
    {27,  0},   //0x52  'R'                    
    {28,  0},   //0x53  'S'                    
    {29,  0},   //0x54  'T'                    
    {30,  0},   //0x55  'U'                       
    {31,  0},   //0x56  'V'                    
    {32,  0},   //0x57  'W'                    
    {33,  0},   //0x58  'X'                       
    {34,  0},   //0x59  'Y'                       
    {35,  0},   //0x5A  'Z'             
    {44, 20},   //0x5B  '[' %K                      
    {44, 21},   //0x5c  '\' %L                   
    {44, 22},   //0x5d  ']' %M                      
    {44, 23},   //0x5e  '^' %N                      
    {44, 24},   //0x5f  '_' %O                      
    {44, 33},   //0x60  '`' %W  
    {46, 10},   //0x61  'a' +A                   
    {46, 11},   //0x62  'b' +B                     
    {46, 12},   //0x63  'c' +C        
    {46, 13},   //0x64  'd' +D                   
    {46, 14},   //0x65  'e' +E                      
    {46, 15},   //0x66  'f' +F                      
    {46, 16},   //0x67  'g' +G                      
    {46, 17},   //0x68  'h' +H                      
    {46, 18},   //0x69  'i' +I                  
    {46, 19},   //0x6a  'j' +J                      
    {46, 20},   //0x6b  'k' +K                      
    {46, 21},   //0x6c  'l' +L                      
    {46, 22},   //0x6d  'm' +M                      
    {46, 23},   //0x6e  'n' +N                      
    {46, 24},   //0x6f  'o' +O                      
    {46, 25},   //0x70  'p' +P                   
    {46, 26},   //0x71  'q' +Q                      
    {46, 27},   //0x72  'r' +R                      
    {46, 28},   //0x73  's' +S                      
    {46, 29},   //0x74  't' +T                   
    {46, 30},   //0x75  'u' +U                      
    {46, 31},   //0x76  'v' +V                      
    {46, 32},   //0x77  'w' +W                      
    {46, 33},   //0x78  'x' +X                      
    {46, 34},   //0x79  'y' +Y                      
    {46, 35},   //0x7a  'z' +Z  
    {44, 25},   //0x7b  '(' %P                      
    {44, 26},   //0x7c  '|' %Q                      
    {44, 27},   //0x7d  '}' %R                      
    {44, 28},   //0x7e  '~' %S                     
    {44, 29},   //0x7f  DEL %T
};

unsigned int code39_id[44] = {
    0xA6D,
    0xD2B,
    0xB2B,
    0xD95,
    0xA6B,
    0xD35,
    0xB35,
    0xA5B,
    0xD2D,
    0xB2D,
    0xD4B,
    0xB4B,
    0xDA5,
    0xACB,
    0xD65,
    0xB65,
    0xA9B,
    0xD4D,
    0xB4D,
    0xACD,
    0xD53,
    0xB53,
    0xDA9,
    0xAD3,
    0xD69,
    0xB69,
    0xAB3,
    0xD59,
    0xB59,
    0xAD9,
    0xCAB,
    0x9AB,
    0xCD5,
    0x96B,
    0xCB5,
    0x9B5,
    0x95B,
    0xCAD,
    0x9AD,
    0x925,
    0x929,
    0x949,
    0xA49,
    0x96D,
};

unsigned char ean13_lhenc_table[10][2] = 
{
                        // value ODD P   EVEN P 
   {0x0D, 0x27},        // 0     0001101 0100111
   {0x19, 0x33},        // 1     0011001 0110011
   {0x13, 0x1B},        // 2     0010011 0011011
   {0x3D, 0x21},        // 3     0111101 0100001
   {0x23, 0x1D},        // 4     0100011 0011101
   {0x31, 0x39},        // 5     0110001 0111001
   {0x2F, 0x05},        // 6     0101111 0000101
   {0x3B, 0x11},        // 7     0111011 0010001
   {0x37, 0x09},        // 8     0110111 0001001
   {0x0B, 0x17}         // 9     0001011 0010111
};

unsigned char ean13_rhenc_table[10] =
{
                        // value ALL
    0x72,               // 0     1110010 
    0x66,               // 1     1100110 
    0x6C,               // 2     1101100 
    0x42,               // 3     1000010 
    0x5C,               // 4     1011100 
    0x4E,               // 5     1001110 
    0x50,               // 6     1010000 
    0x44,               // 7     1000100 
    0x48,               // 8     1001000 
    0x74                // 9     1110100 
};

unsigned char ean13_penc_table[10][6] =
{
   {0,0,0,0,0,0},       // 0 (UPC-A) Odd Odd Odd Odd Odd Odd 
   {0,0,1,0,1,1},       // 1 Odd Odd Even Odd Even Even 
   {0,0,1,1,0,1},       // 2 Odd Odd Even Even Odd Even 
   {0,0,1,1,1,0},       // 3 Odd Odd Even Even Even Odd 
   {0,1,0,0,1,1},       // 4 Odd Even Odd Odd Even Even 
   {0,1,1,0,0,1},       // 5 Odd Even Even Odd Odd Even 
   {0,1,1,1,0,0},       // 6 Odd Even Even Even Odd Odd 
   {0,1,0,1,0,1},       // 7 Odd Even Odd Even Odd Even 
   {0,1,0,1,1,0},       // 8 Odd Even Odd Even Even Odd 
   {0,1,1,0,1,0}        // 9 Odd Even Even Odd Even Odd 
};


#define FNC3_128    1000 
#define FNC2_128    1001
#define SHIFT_128   1002 
#define Code_C_128  1003 
#define Code_B_128  1004 
#define FNC4_128    1005 
#define FNC1_128    1006 
#define START_A_128 1007
#define START_B_128 1008 
#define START_C_128 1009
#define STOP_128    1010

#define MAX_CODE128_CHARSET_A    107
unsigned int Code128_CharSetA[MAX_CODE128_CHARSET_A][2] =  
{                
    {' ',0},
    {'!',1},
    {'"',2}, 
    {'#',3}, 
    {'$',4}, 
    {'%',5}, 
    {'&',6}, 
    {39,7},
    {'(',8}, 
    {')',9}, 
    {'*',10},
    {'+',11},
    {',',12}, 
    {'-',13}, 
    {'.',14}, 
    {'/',15},
    {'0',16}, 
    {'1',17}, 
    {'2',18}, 
    {'3',19}, 
    {'4',20}, 
    {'5',21}, 
    {'6',22}, 
    {'7',23},
    {'8',24}, 
    {'9',25}, 
    {',',26}, 
    {';',27}, 
    {'<',28}, 
    {'=',29}, 
    {'>',30}, 
    {'?',31},
    {'@',32}, 
    {'A',33}, 
    {'B',34}, 
    {'C',35}, 
    {'D',36}, 
    {'E',37}, 
    {'F',38}, 
    {'G',39},
    {'H',40}, 
    {'I',41}, 
    {'J',42}, 
    {'K',43}, 
    {'L',44}, 
    {'M',45}, 
    {'N',46}, 
    {'O',47},
    {'P',48}, 
    {'Q',49}, 
    {'R',50}, 
    {'S',51}, 
    {'T',52}, 
    {'U',53}, 
    {'V',54}, 
    {'W',55},
    {'X',56}, 
    {'Y',57}, 
    {'Z',58}, 
    {'[',59}, 
    {'\\',60}, 
    {']',61}, 
    {'^',62}, 
    {'_',63},
    {'\x00',64},
    {'\x01',65}, 
    {'\x02',66}, 
    {'\x03',67}, 
    {'\x04',68}, 
    {'\x05',69}, 
    {'\x06',70}, 
    {'\x07',71},
    {'\x08',72}, 
    {'\x09',73}, 
    {'\x0A',74}, 
    {'\x0B',75}, 
    {'\x0C',76}, 
    {'\x0D',77}, 
    {'\x0E',78}, 
    {'\x0F',79},
    {'\x10',80}, 
    {'\x11',81}, 
    {'\x12',82}, 
    {'\x13',83}, 
    {'\x14',84}, 
    {'\x15',85}, 
    {'\x16',86}, 
    {'\x17',87},
    {'\x18',88}, 
    {'\x19',89}, 
    {'\x1A',90}, 
    {'\x1B',91}, 
    {'\x1C',92}, 
    {'\x1D',93}, 
    {'\x1E',94}, 
    {'\x1F',95},
    {FNC3_128,96}, 
    {FNC2_128,97}, 
    {SHIFT_128,98}, 
    {Code_C_128,99}, 
    {Code_B_128,100}, 
    {FNC4_128,101}, 
    {FNC1_128,102}, 
    {START_A_128,103},
    {START_B_128,104}, 
    {START_C_128,105},
    {STOP_128,106}
};

unsigned int Code128_value[MAX_CODE128_CHARSET_A] =
{
  0x6CC,    //'11011001100'
  0x66C,    //'11001101100'
  0x666,    //'11001100110'
  0x498,    //'10010011000'
  0x48C,    //'10010001100'
  0x44C,    //'10001001100'
  0x4C8,    //'10011001000'  
  0x4C4,    //'10011000100'
  0x464,    //'10001100100'
  0x648,    //'11001001000'
  0x644,    //'11001000100' 
  0x624,    //'11000100100'
  0x59C,    //'10110011100'
  0x4DC,    //'10011011100'
  0x4CE,    //'10011001110'
  0x5CC,    //'10111001100'
  0x4EC,    //'10011101100'
  0x4E6,    //'10011100110'
  0x672,    //'11001110010'
  0x65C,    //'11001011100'
  0x64E,    //'11001001110'
  0x6E4,    //'11011100100'
  0x674,    //'11001110100'
  0x76E,    //'11101101110'
  0x74C,    //'11101001100'
  0x72C,    //'11100101100'
  0x726,    //'11100100110'
  0x764,    //'11101100100'
  0x734,    //'11100110100'
  0x732,    //'11100110010'
  0x6D8,    //'11011011000'
  0x6C6,    //'11011000110'
  0x636,    //'11000110110'
  0x518,    //'10100011000'
  0x458,    //'10001011000'
  0x446,    //'10001000110'
  0x588,    //'10110001000'
  0x468,    //'10001101000'
  0x462,    //'10001100010'
  0x688,    //'11010001000'
  0x628,    //'11000101000'
  0x622,    //'11000100010'
  0x5B8,    //'10110111000'
  0x58E,    //'10110001110'
  0x46E,    //'10001101110'
  0x5D8,    //'10111011000'
  0x5C6,    //'10111000110'
  0x476,    //'10001110110'
  0x776,    //'11101110110'
  0x68E,    //'11010001110'
  0x62E,    //'11000101110'
  0x6E8,    //'11011101000'
  0x6E2,    //'11011100010'
  0x6EE,    //'11011101110'
  0x758,    //'11101011000'
  0x746,    //'11101000110'
  0x716,    //'11100010110'
  0x768,    //'11101101000'
  0x762,    //'11101100010'
  0x71A,    //'11100011010'
  0x77A,    //'11101111010'
  0x642,    //'11001000010'
  0x78A,    //'11110001010'
  0x530,    //'10100110000'
  0x50C,    //'10100001100'
  0x4B0,    //'10010110000'
  0x486,    //'10010000110'
  0x42C,    //'10000101100'
  0x426,    //'10000100110'
  0x590,    //'10110010000'
  0x584,    //'10110000100'
  0x4D0,    //'10011010000'
  0x4C2,    //'10011000010'
  0x434,    //'10000110100'
  0x432,    //'10000110010'
  0x612,    //'11000010010'
  0x650,    //'11001010000'
  0x7BA,    //'11110111010'
  0x614,    //'11000010100'
  0x47A,    //'10001111010'
  0x53C,    //'10100111100'
  0x4BC,    //'10010111100'
  0x49E,    //'10010011110'
  0x5E4,    //'10111100100'
  0x4F4,    //'10011110100'
  0x4F2,    //'10011110010'
  0x7A4,    //'11110100100'
  0x794,    //'11110010100'
  0x792,    //'11110010010'
  0x6DE,    //'11011011110'
  0x6F6,    //'11011110110'
  0x7B6,    //'11110110110'
  0x578,    //'10101111000'
  0x51E,    //'10100011110'
  0x45E,    //'10001011110'
  0x5E8,    //'10111101000'
  0x5E2,    //'10111100010'
  0x7A8,    //'11110101000'
  0x7A2,    //'11110100010'
  0x5DE,    //'10111011110'
  0x5EE,    //'10111101110'
  0x75E,    //'11101011110'
  0x7AE,    //'11110101110'
  0x684,    //'11010000100'
  0x690,    //'11010010000'
  0x69C,    //'11010011100'
  0x63A     //'11000111010'
};
