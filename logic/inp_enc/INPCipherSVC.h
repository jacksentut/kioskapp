#ifdef __cplusplus
extern "C" {
#endif

int 	INP_EncData(char* insrc, int insrclen, char* outencdata);
int 	INP_DecData(char* inencdata, int inenclen, char* outdecdata);

#ifdef __cplusplus
}
#endif

