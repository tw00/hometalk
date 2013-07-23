/* demo_write_encode.c
 *
 *
 */

#include <stdio.h>
#include <string.h>
#include <ht_helper.h>
#include <ht_decode_encode.h>

void printBuffer(UBYTE* buffer, UBYTE length) {
	UBYTE i;

    printf("BufferContent:");
    for( i=0; i < length; i++) {
    	printf(" %d", (int)buffer[i]);
    }
    printf("\n");
}

void printCmdFrame(hometalkCMD* cmd) {

	printf("CMD Frame:\n");
	printf("-Seq: %d\n", cmd->header.seq);
	printf("-Addr: %d\n", (int)cmd->addr);
	printf("-Func: %d\n", (int)cmd->func);
	printf("-Cmd: %d %d %d %d\n", (int)cmd->cmd[0], (int)cmd->cmd[1], (int)cmd->cmd[2], (int)cmd->cmd[3]);

}

void printPsiFrame(hometalkPSI* psi) {

	printf("PSI Frame:\n");
	printf("-Seq: %d\n", psi->header.seq);
	printf("-Addr: %d\n", (int)psi->addr);
	printf("-ID: %d\n", (int)psi->ID);
	printf("-Data: %d %d %d %d %d %d %d %d\n", (int)psi->payload[0], (int)psi->payload[1], (int)psi->payload[2], (int)psi->payload[3], (int)psi->payload[4], (int)psi->payload[5], (int)psi->payload[6], (int)psi->payload[7]);

}

void printFlowFrame(hometalkFLOW* flow) {

	printf("FLOW Frame:\n");
	printf("-Seq: %d\n", flow->header.seq);
	printf("-Sender: %d\n", (int)flow->sender);
}

int main(int argc, char *argv[])
{
    int i;

    hometalkCMD cmdFrame;
    hometalkCMD cmdFrameResult;

    hometalkPSI psiFrame;
    hometalkPSI psiFrameResult;

    hometalkFLOW flowFrame;
    hometalkFLOW flowFrameResult;

    UBYTE buffer1[20];
    HtFrameType frameType;
    HtDecodeResult frameState;
    UBYTE frameLength;

    for(i=0; i < 20;i++) {
    	buffer1[i] = 0;
    }

    for(i=1; i < 3; i++) {
		cmdFrame.addr = 12345U;
		cmdFrame.cmd[0] = i;
		cmdFrame.cmd[1] = (12 * i) % 256;
		cmdFrame.cmd[2] = (123 * i) % 256;
		cmdFrame.cmd[3] = (23 * i) % 256;
		cmdFrame.func = 155;
		cmdFrame.header.seq = (UBYTE)i;

		psiFrame.ID = (133*i)%256;
		psiFrame.addr = (1252*i)%65536;
		psiFrame.header.seq = i % 256;
		psiFrame.payload[0] = (124 * i) % 256;
		psiFrame.payload[1] = (76 * i) % 256;
		psiFrame.payload[2] = (53 * i) % 256;
		psiFrame.payload[3] = (188 * i) % 256;
		psiFrame.payload[4] = (21 * i) % 256;
		psiFrame.payload[5] = (4 * i) % 256;
		psiFrame.payload[6] = (78 * i) % 256;
		psiFrame.payload[7] = (24 * i) % 256;

		flowFrame.header.seq = i;
		flowFrame.sender = (42125*i) % 65536;


		frameLength = ht_encode_cmd(&cmdFrame, buffer1);
		printBuffer(buffer1, frameLength);

		frameType = ht_determine_frame_type(buffer1, 25);
		printf(" Frame Type: %d\n", (int)frameType);
		frameState = ht_decode_cmd(&cmdFrameResult, buffer1);
		printf(" Frame State: %d\n", (int)frameState);

		printCmdFrame(&cmdFrame);
		printCmdFrame(&cmdFrameResult);

		printf("---------------------------------------\n\n");


		frameLength = ht_encode_psi(&psiFrame, buffer1);
		printBuffer(buffer1, frameLength);

		frameType = ht_determine_frame_type(buffer1, 25);
		printf(" Frame Type: %d\n", (int)frameType);
		frameState = ht_decode_psi(&psiFrameResult, buffer1);
		printf(" Frame State: %d\n", (int)frameState);

		printPsiFrame(&psiFrame);
		printPsiFrame(&psiFrameResult);

		printf("---------------------------------------\n\n");

		frameLength = ht_encode_flow(&flowFrame, buffer1);
		printBuffer(buffer1, frameLength);

		frameType = ht_determine_frame_type(buffer1, 25);
		printf(" Frame Type: %d\n", (int)frameType);
		frameState = ht_decode_flow(&flowFrameResult, buffer1);
		printf(" Frame State: %d\n", (int)frameState);

		printFlowFrame(&flowFrame);
		printFlowFrame(&flowFrameResult);

		printf("---------------------------------------\n\n");
    }
    return 0;
}
