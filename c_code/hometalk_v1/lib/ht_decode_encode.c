#include "ht_common.h"
#include "ht_circularBuffer.h"
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

UBYTE ht_encode_cmd(hometalkCMD* cmd, circular_buffer* buffer) {
	DBYTE crcSum;
	UBYTE queueBuffer[10];

	if(NULL == cmd || NULL == buffer) {
		return 0U;
	}

	ht_cb_push_back(buffer, (UBYTE)HT_MAGIC_NUMBER);
	//buffer[1] = (UBYTE)((cmd->header.seq << 4) | (cmd->header.ctrlFrt << 3) | (cmd->header.ctrlRes << 2) | (cmd->header.ctrlExt << 1) | cmd->header.ctrlRou);
	queueBuffer[0] = (UBYTE)((cmd->header.seq << 4) | (0U << 1) | cmd->header.ctrlRou);
	queueBuffer[1] = (UBYTE)((cmd->addr >> 8 ) & 0xFF);
	queueBuffer[2] = (UBYTE)(cmd->addr & 0xFF);
	queueBuffer[3] = (UBYTE)(cmd->func);
	queueBuffer[4] = (UBYTE)(cmd->cmd[0]);
	queueBuffer[5] = (UBYTE)(cmd->cmd[1]);
	queueBuffer[6] = (UBYTE)(cmd->cmd[2]);
	queueBuffer[7] = (UBYTE)(cmd->cmd[3]);

	/* TODO: FIX BUFFER CONCEPT */
	crcSum = crc16(queueBuffer, 8, 0);

	queueBuffer[8] = (UBYTE)((crcSum >> 8) & 0xFF);
	queueBuffer[9] = (UBYTE)(crcSum & 0xFF);
	
	ht_cb_push_block_back(buffer, queueBuffer, 10);

	return 11U;
}

UBYTE ht_encode_flow(hometalkFLOW* flow, circular_buffer* buffer) {
	DBYTE crcSum;
	UBYTE queueBuffer[5];
	
	if(NULL == flow || NULL == buffer) {
		return 0U;
	}

	ht_cb_push_back(buffer, (UBYTE)HT_MAGIC_NUMBER);
	//buffer[1] = (UBYTE)((flow->header.seq << 4) | (flow->header.ctrlFrt << 3) | (flow->header.ctrlRes << 2) | (flow->header.ctrlExt << 1) | flow->header.ctrlRou);
	queueBuffer[0] = (UBYTE)((flow->header.seq << 4) | (4U << 1) | flow->header.ctrlRou);
	queueBuffer[1] = (UBYTE)((flow->sender >> 8 ) & 0xFF);
	queueBuffer[2] = (UBYTE)(flow->sender & 0xFF);

	crcSum = crc16(queueBuffer, 3, 0);

	queueBuffer[3] = (UBYTE)((crcSum >> 8) & 0xFF);
	queueBuffer[4] = (UBYTE)(crcSum & 0xFF);
	
	ht_cb_push_block_back(buffer, queueBuffer, 5);

	return 6U;
}

UBYTE ht_encode_psi(hometalkPSI* psi, circular_buffer* buffer) {
	DBYTE crcSum;
	UBYTE queueBuffer[14];

	if(NULL == psi || NULL == buffer) {
		return 0U;
	}

	ht_cb_push_back(buffer, (UBYTE)HT_MAGIC_NUMBER);
	//buffer[1] = (UBYTE)((psi->header.seq << 4) | (psi->header.ctrlFrt << 3) | (psi->header.ctrlRes << 2) | (psi->header.ctrlExt << 1) | psi->header.ctrlRou);
	queueBuffer[0] = (UBYTE)((psi->header.seq << 4) | (5U << 1) | psi->header.ctrlRou);
	queueBuffer[1] = (UBYTE)((psi->addr >> 8 ) & 0xFF);
	queueBuffer[2] = (UBYTE)(psi->addr & 0xFF);
	queueBuffer[3] = (UBYTE)((psi->ID << 4) | (psi->res & 0x0F));
	queueBuffer[4] = psi->payload[0];
	queueBuffer[5] = psi->payload[1];
	queueBuffer[6] = psi->payload[2];
	queueBuffer[7] = psi->payload[3];
	queueBuffer[8] = psi->payload[4];
	queueBuffer[9] = psi->payload[5];
	queueBuffer[10] = psi->payload[6];
	queueBuffer[11] = psi->payload[7];

	crcSum = crc16(queueBuffer, 12, 0);

	queueBuffer[12] = (UBYTE)((crcSum >> 8) & 0xFF);
	queueBuffer[13] = (UBYTE)(crcSum & 0xFF);
	
	ht_cb_push_block_back(buffer, queueBuffer, 14);

	return 15U;
}

UBYTE ht_get_frame_length_by_type(UBYTE buffer) {
	UBYTE frameType;
	
	frameType = (UBYTE)(buffer & 0x0F);
	if(0 == (frameType & HT_HEADER_CONTROLFIELD_FRT) ) {
		if(0 == (frameType & HT_HEADER_CONTROLFIELD_EXT)){
			return HT_CMD_FRAME_LENGTH;
		} else {
			return HT_EXCMD_FRAME_LENGTH;
		}
	} else {
		if(0 == (frameType & HT_HEADER_CONTROLFIELD_EXT)){
			return HT_FLOW_FRAME_LENGTH;
		} else {
			return HT_PSI_FRAME_LENGTH;
		}
	}

	return 0 /* ERROR */;
}

HtFrameType ht_determine_frame_type(circular_buffer* buffer) {
	UBYTE frameType;
	UBYTE queueBuffer;

	if(NULL == buffer) {
		return ERROR;
	}

	ht_cb_look_front(buffer, &queueBuffer, 0);	/* check first byte to determine frame type */
	frameType = (UBYTE)(queueBuffer & 0x0F);	/* mask out reserved bits */
	
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

HtDecodeResult ht_decode_cmd(hometalkCMD* cmd, circular_buffer* buffer) {
	DBYTE crcSum;
	UBYTE queueBuffer[10];

	if(NULL == buffer || NULL == cmd) {
		return POINTER_ERROR;
	}
	
	//ht_cb_pop_front(buffer, queueBuffer); /* get magic number */
	//if(HT_MAGIC_NUMBER != queueBuffer[0]) {
	//	return MAGIC_NUM_ERROR;
	//}
	
	ht_cb_pop_block_front(buffer, queueBuffer, 10); /* get whole CMD frame from queue */
	
	/* check CRC check sum */
	crcSum = (((DBYTE)queueBuffer[8]) << 8) | ((DBYTE)queueBuffer[9]);
	if(crc16(queueBuffer, 8, 0) != crcSum) {
		return CRC_ERROR;
	}

	cmd->header.seq = (UBYTE)(queueBuffer[0] >> 4);	
	cmd->addr = ((DBYTE)(queueBuffer[1]) << 8) | (DBYTE)queueBuffer[2];	
	cmd->func = queueBuffer[3];	
	cmd->cmd[0] = queueBuffer[4];
	cmd->cmd[1] = queueBuffer[5];
	cmd->cmd[2] = queueBuffer[6];
	cmd->cmd[3] = queueBuffer[7];

	return OK;
}

HtDecodeResult ht_decode_flow(hometalkFLOW* flow, circular_buffer* buffer) {
	DBYTE crcSum;
	UBYTE queueBuffer[5];
	
	if(NULL == buffer || NULL == flow) {
		return POINTER_ERROR;
	}
	
	//ht_cb_pop_front(buffer, queueBuffer); /* get magic number */
	//if(HT_MAGIC_NUMBER != queueBuffer[0]) {
	//	return MAGIC_NUM_ERROR;
	//}

	ht_cb_pop_block_front(buffer, queueBuffer, 5); /* get whole FLOW frame from queue */

	/* check CRC check sum */
	crcSum = (((DBYTE)queueBuffer[3]) << 8) | ((DBYTE)queueBuffer[4]);
	if(crc16(queueBuffer, 3, 0) != crcSum) {
		return CRC_ERROR;
	}

	flow->header.seq = (UBYTE)(queueBuffer[0] >> 4) & 0x0F;
	flow->sender = ((DBYTE)(queueBuffer[1]) << 8) | (DBYTE)queueBuffer[2];

	return OK;
}

HtDecodeResult ht_decode_psi(hometalkPSI* psi, circular_buffer* buffer) {
	DBYTE crcSum;
	UBYTE queueBuffer[14];
	
	if(NULL == buffer || NULL == psi) {
		return POINTER_ERROR;
	}
	
	//ht_cb_pop_front(buffer, queueBuffer); /* get magic number */
	//if(HT_MAGIC_NUMBER != queueBuffer[0]) {
	//	return MAGIC_NUM_ERROR;
	//}

	ht_cb_pop_block_front(buffer, queueBuffer, 14); /* get whole PSI frame from queue */
	
	/* check CRC check sum */
	crcSum = (((DBYTE)queueBuffer[12]) << 8) | ((DBYTE)queueBuffer[13]);
	if(crc16(queueBuffer, 12, 0) != crcSum) {
		return CRC_ERROR;
	}

	psi->header.seq = (UBYTE)(queueBuffer[0] >> 4);
	psi->addr = ((DBYTE)(queueBuffer[1]) << 8) | (DBYTE)queueBuffer[2];
	psi->ID = queueBuffer[3] >> 4;
	psi->res = queueBuffer[3] & 0x0F;
	psi->payload[0]	= queueBuffer[4];
	psi->payload[1]	= queueBuffer[5];
	psi->payload[2]	= queueBuffer[6];
	psi->payload[3]	= queueBuffer[7];
	psi->payload[4]	= queueBuffer[8];
	psi->payload[5]	= queueBuffer[9];
	psi->payload[6]	= queueBuffer[10];
	psi->payload[7]	= queueBuffer[11];

	return OK;
}
