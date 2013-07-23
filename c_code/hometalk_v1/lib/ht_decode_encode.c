#include "ht_decode_encode.h"
#include "ht_helper.h"

/*****************************************************************************/
BYTE* ht_encode_frame(hometalkCommand* ht_cmd)
{
    static BYTE frame[HT_FRAME_LENGTH]; // HACK

    frame[0] = 0xd7;
    frame[1] = 0xd7;
    frame[2] = 0xd7;

//    memcpy(&frame[4], ht_cmd, sizeof(hometalkCommand)-1);

    return (BYTE *)&frame;
}

/*****************************************************************************/
DBYTE ht_enconde_header( HtFrameType type, BOOL isRoutedFrame, long seq )
{
    // TODO: falschrum

    HBYTE FRT, RES, EXT, ROU;
    HBYTE seq_hbyte = seq % 16; // = 2^4

    RES = 0xf;
    FRT = 0x0; EXT = 0x0;
    switch( type ) {
        case CMD: FRT = 0x0; EXT = 0x0; break;
        case eCMD: FRT = 0x0; EXT = 0xf; break;
        case FLOW: FRT = 0xf; EXT = 0x0; break;
        case PSI: FRT = 0xf; EXT = 0xf; break;
        default:
            // TODO
        break;
    }

    if( isRoutedFrame ) ROU = 0xf;
    else ROU = 0x0;

    /*printf("FRT: %#x\n", FRT);
printf("RES: %#x\n", RES);
printf("EXT: %#x\n", EXT);
printf("ROU: %#x\n", ROU);*/

    return (DBYTE)(
        (HT_HEADER_MAGIC_BYTE << 8 ) |
        ( (HT_HEADER_CONTROLFIELD_FRT & FRT ) << 4 ) |
        ( (HT_HEADER_CONTROLFIELD_RES & RES ) << 4 ) |
        ( (HT_HEADER_CONTROLFIELD_EXT & EXT ) << 4 ) |
        ( (HT_HEADER_CONTROLFIELD_ROU & ROU ) << 4 ) |
        (seq_hbyte << 0 ));
}

/*****************************************************************************/
hometalkCommand* ht_decode_frame(BYTE* frame, int length)
{
    char data[11]; // ??????
    hometalkCommand* command;
    // unsigned long test;

    memcpy(data, &frame[4], sizeof(hometalkCommand)-1);
    command = (hometalkCommand *)data;

    return command;
}

UBYTE ht_encode_cmd(hometalkCMD* cmd, UBYTE* buffer) {
	DBYTE crcSum;

	if(NULL == cmd || NULL == buffer) {
		return 0U;
	}

	buffer[0] = (UBYTE)HT_MAGIC_NUMBER;
	//buffer[1] = (UBYTE)((cmd->header.seq << 4) | (cmd->header.ctrlFrt << 3) | (cmd->header.ctrlRes << 2) | (cmd->header.ctrlExt << 1) | cmd->header.ctrlRou);
	buffer[1] = (UBYTE)((cmd->header.seq << 4) | (0U << 1) | cmd->header.ctrlRou);
	buffer[2] = (UBYTE)((cmd->addr >> 8 ) & 0xFF);
	buffer[3] = (UBYTE)(cmd->addr & 0xFF);
	buffer[4] = (UBYTE)(cmd->func);
	buffer[5] = (UBYTE)(cmd->cmd[0]);
	buffer[6] = (UBYTE)(cmd->cmd[1]);
	buffer[7] = (UBYTE)(cmd->cmd[2]);
	buffer[8] = (UBYTE)(cmd->cmd[3]);

	crcSum = crc16(buffer+1, 8, 0);

	buffer[9] = (UBYTE)((crcSum >> 8) & 0xFF);
	buffer[10] = (UBYTE)(crcSum & 0xFF);

	return 11U;
}

UBYTE ht_encode_flow(hometalkFLOW* flow, UBYTE* buffer){
	DBYTE crcSum;

	if(NULL == flow || NULL == buffer) {
		return 0U;
	}

	buffer[0] = (UBYTE)HT_MAGIC_NUMBER;
	//buffer[1] = (UBYTE)((flow->header.seq << 4) | (flow->header.ctrlFrt << 3) | (flow->header.ctrlRes << 2) | (flow->header.ctrlExt << 1) | flow->header.ctrlRou);
	buffer[1] = (UBYTE)((flow->header.seq << 4) | (4U << 1) | flow->header.ctrlRou);
	buffer[2] = (UBYTE)((flow->sender >> 8 ) & 0xFF);
	buffer[3] = (UBYTE)(flow->sender & 0xFF);

	crcSum = crc16(buffer+1, 3, 0);

	buffer[4] = (UBYTE)((crcSum >> 8) & 0xFF);
	buffer[5] = (UBYTE)(crcSum & 0xFF);

	return 6U;
}

UBYTE ht_encode_psi(hometalkPSI* psi, UBYTE* buffer){
	DBYTE crcSum;

	if(NULL == psi || NULL == buffer) {
		return 0U;
	}

	buffer[0] = (UBYTE)HT_MAGIC_NUMBER;
	//buffer[1] = (UBYTE)((psi->header.seq << 4) | (psi->header.ctrlFrt << 3) | (psi->header.ctrlRes << 2) | (psi->header.ctrlExt << 1) | psi->header.ctrlRou);
	buffer[1] = (UBYTE)((psi->header.seq << 4) | (5U << 1) | psi->header.ctrlRou);
	buffer[2] = (UBYTE)((psi->addr >> 8 ) & 0xFF);
	buffer[3] = (UBYTE)(psi->addr & 0xFF);
	buffer[4] = (UBYTE)((psi->ID << 4) | (psi->res & 0x0F));
	buffer[5] = psi->payload[0];
	buffer[6] = psi->payload[1];
	buffer[7] = psi->payload[2];
	buffer[8] = psi->payload[3];
	buffer[9] = psi->payload[4];
	buffer[10] = psi->payload[5];
	buffer[11] = psi->payload[6];
	buffer[12] = psi->payload[7];

	crcSum = crc16(buffer+1, 12, 0);

	buffer[13] = (UBYTE)((crcSum >> 8) & 0xFF);
	buffer[14] = (UBYTE)(crcSum & 0xFF);

	return 15U;
}

HtFrameType ht_determine_frame_type(UBYTE* buffer, UBYTE maxLength) {
	UBYTE frameType;

	if(NULL == buffer || 0 == maxLength) {
		return ERROR;
	}

	if(HT_MAGIC_NUMBER != buffer[0]) {
		return ERROR;
	}

	/* determine message type by checking control field bits */
	frameType = (UBYTE)(buffer[1] & 0x0F);
	if(0 == (frameType & HT_HEADER_CONTROLFIELD_FRT) ) {
		if(0 == (frameType & HT_HEADER_CONTROLFIELD_EXT)){
			return CMD;
		} else {
			return eCMD;
		}
	} else {
		if(0 == (frameType & HT_HEADER_CONTROLFIELD_EXT)){
			return FLOW;
		} else {
			return PSI;
		}
	}

	return ERROR;
}

HtDecodeResult ht_decode_cmd(hometalkCMD* cmd, UBYTE* buffer) {
	DBYTE crcSum;
	if(NULL == buffer || NULL == cmd) {
		return POINTER_ERROR;
	}

	/* check CRC check sum */
	crcSum = (((DBYTE)buffer[9]) << 8) | ((DBYTE)buffer[10]);
	if(crc16(buffer+1, 8, 0) != crcSum) {
		return CRC_ERROR;
	}

	cmd->header.seq = (UBYTE)(buffer[1] >> 4);
	cmd->addr = ((DBYTE)(buffer[2]) << 8) | (DBYTE)buffer[3];
	cmd->func = buffer[4];
	cmd->cmd[0] = buffer[5];
	cmd->cmd[1] = buffer[6];
	cmd->cmd[2] = buffer[7];
	cmd->cmd[3] = buffer[8];

	return OK;
}

HtDecodeResult ht_decode_flow(hometalkFLOW* flow, UBYTE* buffer){
	DBYTE crcSum;
	if(NULL == buffer || NULL == flow) {
		return POINTER_ERROR;
	}

	/* check CRC check sum */
	crcSum = (((DBYTE)buffer[4]) << 8) | ((DBYTE)buffer[5]);
	if(crc16(buffer+1, 3, 0) != crcSum) {
		return CRC_ERROR;
	}

	flow->header.seq = (UBYTE)(buffer[1] >> 4) & 0x0F;
	flow->sender = ((DBYTE)(buffer[2]) << 8) | (DBYTE)buffer[3];

	return OK;
}

HtDecodeResult ht_decode_psi(hometalkPSI* psi, UBYTE* buffer){
	DBYTE crcSum;

	if(NULL == buffer || NULL == psi) {
		return POINTER_ERROR;
	}

	/* check CRC check sum */
	crcSum = (((DBYTE)buffer[13]) << 8) | ((DBYTE)buffer[14]);
	if(crc16(buffer+1, 12, 0) != crcSum) {
		return CRC_ERROR;
	}

	psi->header.seq = (UBYTE)(buffer[1] >> 4);
	psi->addr = ((DBYTE)(buffer[2]) << 8) | (DBYTE)buffer[3];
	psi->ID = buffer[4] >> 4;
	psi->res = buffer[4] & 0x0F;
	psi->payload[0]	= buffer[5];
	psi->payload[1]	= buffer[6];
	psi->payload[2]	= buffer[7];
	psi->payload[3]	= buffer[8];
	psi->payload[4]	= buffer[9];
	psi->payload[5]	= buffer[10];
	psi->payload[6]	= buffer[11];
	psi->payload[7]	= buffer[12];

	return OK;
}
