#include "ht_common.h"
#include "ht_helper.h"

/*****************************************************************************/
int ht_readdata(BYTE* frame, const char* filename)
{
    /* declare the file pointer */
    FILE *fr;            
    size_t result;

    /* open the file for reading */
    fr = fopen(filename, "rb");  
    if (fr==NULL) {
        fputs ("File error", stderr);
        return 1;
    }

    result = fread(frame,1,HT_FRAME_LENGTH,fr);
    if (result != HT_FRAME_LENGTH) {
        fputs("Reading error (16 bytes expected)", stderr);
        return 3;
    }
    fclose(fr);
    return 0;
}
/*****************************************************************************/
int ht_writedata(hometalkCommand* cmd, const char* filename)
{
    /* declare the file pointer */
    FILE *fr;            
    size_t result;

    /* open the file for reading */
    fr = fopen(filename, "wb");  
    if (fr==NULL) {
        fputs ("File error", stderr);
        return 1;
    }

    result = fwrite(cmd,1,HT_FRAME_LENGTH,fr);
    if (result != HT_FRAME_LENGTH) {
        fputs("Writing error (16 bytes expected)", stderr);
        return 3;
    }

    fclose(fr);
    return 0;
}


/*****************************************************************************/
void ht_debug_data(BYTE* frame)
{
    int i;

    printf("data:\n");
    for(i = 0; i < 16; i++) {
        printf("%02hhx ", (unsigned char)frame[i]);
        if( (i+1) % 8 == 0 ) fputs("\n", stdout);
    }
}

/*****************************************************************************/
void ht_debug_frame(hometalkCommand* command)
{
    printf("\n");
    printf("command:\n");
    printf("  addr:   %d\n", command->addr);
    printf("  device: %d\n", command->device);
    printf("  cmd:    0x%02hhx%02hhx%02hhx%02hhx\n",
                command->cmd[0],
                command->cmd[1],
                command->cmd[2],
                command->cmd[3]);
    // printf("  cmd:    0x%08lx\n", test); //(test>>8) | (test<<8));
    printf("  value:  %d\n", command->value);
    printf("  seqnum: %d\n", command->seqnum);
}

/*****************************************************************************/
void ht_debug_header(DBYTE header)
{
    char mb;
    BOOL FRT, RES, EXT, ROU;
    HBYTE seq;
    BYTE B1, B2;
    const char* const str_array[] = { "CMD", "eCMD", "FLOW", "PSI" };
    char* type_str;

    mb  = (BYTE)(header >> 8);
    FRT = (BOOL)((HT_HEADER_CONTROLFIELD_FRT & header >> 4) > 0);
    RES = (BOOL)((HT_HEADER_CONTROLFIELD_RES & header >> 4) > 0);
    EXT = (BOOL)((HT_HEADER_CONTROLFIELD_EXT & header >> 4) > 0);
    ROU = (BOOL)((HT_HEADER_CONTROLFIELD_ROU & header >> 4) > 0);
    seq = (HBYTE)(0b00001111 & header); 

    if( !FRT && !EXT) type_str = str_array[CMD];
    if( !FRT &&  EXT) type_str = str_array[eCMD];
    if(  FRT && !EXT) type_str = str_array[FLOW];
    if(  FRT &&  EXT) type_str = str_array[PSI];

    B1 = (header & 0x000000ff);
    B2 = (header & 0x0000ff00) >> 8;

    printf("header:\n");
    printf("  binary: (1) 0b%s\n", ht_byte_to_binary( B1 ) );
    printf("          (2) 0b%s\n", ht_byte_to_binary( B2 ) );
    printf("  magic byte: %#x\n", mb );
    printf("  control field:\n");
    printf("    FRT: %d\n", FRT);
    printf("    RES: %d\n", RES);
    printf("    EXT: %d\n", EXT);
    printf("    ROU: %d\n", ROU);
    printf("    (Type: %s)\n", type_str);
    printf("  seqnum: %d\n", seq);
}

/*****************************************************************************/
const char* ht_byte_to_binary(BYTE x)
{
    static char b[sizeof(BYTE)*8+1] = {0};
    int y;
    long long z;

    for (z=1LL<<(sizeof(BYTE)*8-1),y=0; z>0; z>>=1,y++) {
        b[y] = ( ((x & z) == z) ? '1' : '0');
    }

    b[y] = 0;
    return b;
}


/*****************************************************************************/
/*
 * Gray Encoding/Decoding
 * ================================
 * based on http://rosettacode.org/wiki/Gray_code#C
 *****************************************************************************/
UBYTE ht_gray_encode_ubyte(UBYTE n) {
    return n ^ (n >> 1);
}

/*****************************************************************************/
UBYTE ht_gray_decode_ubyte(UBYTE n) {
	UBYTE p = n;
    while (n >>= 1) {
    	p ^= n;
    }
    return p;
}

/*****************************************************************************/
BYTE ht_gray_decode_byte(BYTE n){
	return (BYTE)ht_gray_decode_ubyte((UBYTE)n);
}

/*****************************************************************************/
BYTE ht_gray_encode_byte(BYTE n) {
	return (BYTE)ht_gray_encode_ubyte((UBYTE)n);
}

/*****************************************************************************/
/*
 * 	CRC-16 calculation
 *  ================================
 *  based on http://www.mikrocontroller.net/topic/12177#79681
 *****************************************************************************/

#define crc16_poly 0xA001

DBYTE crc16(UBYTE *addr, UBYTE num, DBYTE crc) {
	BYTE i;

	for (; num > 0; num--) {
		crc ^= *addr;
		addr++;
		for (i = 8; i; i--) {
			crc = (crc >> 1) ^ ((crc & 1) ? crc16_poly : 0 );
		}
	}

	return crc;
}

unsigned int ter2dec(UBYTE *bin, UBYTE len) {
	UBYTE k;
	unsigned int factor = 1;
	unsigned int res = 0;

	for (k = len; k >= 0; k--) {

		switch (bin[k]) {
		case '0':
			/* nothing to add */
			break;

		case '1':
			res += factor;
			break;

		case '2':
		case 'f':
		case 'F':
			res += factor * 2;
			break;

		default:
			/* should never be reached */
			break;
		}


		factor *= 3;	/* move to next digit */
	}
	return res;
}

