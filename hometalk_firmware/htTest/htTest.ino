#include "ht_common.h"
#include "ht_helper.h"
#include "ht_circularBuffer.h"
#include "ht_decode_encode.h"

#define SEND_BUFFER_SIZE 18U
#define RECEIVE_BUFFER_SIZE 18U

UBYTE sendRawBuffer[SEND_BUFFER_SIZE];
circular_buffer sendBuffer;
UBYTE receiveRawBuffer[RECEIVE_BUFFER_SIZE];
circular_buffer receiveBuffer;

/* needed for receiving functionality */
UBYTE frameStarted = 0;
UBYTE frameEnded = 0;
UBYTE frameRemainingBytes = 255;

hometalkPSI psiReceiveBuffer;
hometalkFLOW flowReceiveBuffer;
hometalkCMD cmdReceiveBuffer;
BOOL psiFrameReceived = false;
BOOL flowFrameReceived = false;
BOOL cmdFrameReceived = false;

hometalkPSI psiFrame;

void setup() {
  ht_cb_init(&sendBuffer, sendRawBuffer, (UBYTE)SEND_BUFFER_SIZE);
  ht_cb_init(&receiveBuffer, receiveRawBuffer, (UBYTE)RECEIVE_BUFFER_SIZE);

  Serial.begin(19200);   /* initialize serial: */
  pinMode(13, OUTPUT);
}


long pause = 160000;
int i = 0;

void loop() { 
/* 
  if(true == psiFrameReceived) {    
    psiFrameReceived = false;
	
	digitalWrite(13, (psiReceiveBuffer.payload[0] == 1)? HIGH:LOW);
  }
  
  if(0 == pause) {
  
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
	
	(void) ht_encode_psi(&psiFrame, &sendBuffer);
  
	i++;
	pause = 160000;
  }*/
  
	if(true == flowFrameReceived) {    
		flowFrameReceived = false;

		digitalWrite(13, (flowReceiveBuffer.sender == 127)? HIGH:LOW);
	}
  
  pause--;
  
  
  
  /* send output data */
	if(0 < sendBuffer.count) {
		UBYTE serialSendBuffer[15]; /* buffer for sending one frame, 15 = maximal frame length */
		UBYTE length;

		length = (sendBuffer.count < 15U)? sendBuffer.count : 15U;  /* check if more than 15 bytes are in the buffer */
		ht_cb_pop_block_front(&sendBuffer, serialSendBuffer, length);
		Serial.write(serialSendBuffer, length);
    
	/*
    for(int i = 0; i < length-1; i++) {
      if(serialSendBuffer[i] < 16) {
        Serial.print(0);
      }
      Serial.print(serialSendBuffer[i], HEX);
    }
    if(serialSendBuffer[length-1] < 16) {
       Serial.print(0);
    }
    Serial.println(serialSendBuffer[length-1], HEX); */
  }
}


int j = 0;
void serialEvent() {
	UBYTE inputByte;
  
	while (Serial.available()) {
		inputByte = (UBYTE)Serial.read();    
    
		if(0 == frameStarted && HT_MAGIC_NUMBER == inputByte) {
			frameStarted = 1;
			frameEnded = 0;
			frameRemainingBytes = 255;	  
		} else if(1 == frameStarted) {
			if(255 == frameRemainingBytes) {
				frameRemainingBytes = ht_get_frame_length_by_type(inputByte);

				if(0 == frameRemainingBytes) {  /* frame type not detected => error => stop receiving for this frame */
					frameStarted = 0;
					frameEnded = 0;
				}

				ht_cb_push_back(&receiveBuffer, inputByte);
				frameRemainingBytes -= 2; /* minus magic byte and first byte which was just added */
			} else {			
				ht_cb_push_back(&receiveBuffer, inputByte);  /* add byte and decrement byte counter */
				frameRemainingBytes--;  
			}
		
			if(1 == frameStarted && 0 >= frameRemainingBytes) {
				frameEnded = 1;
			}
		} else {
		}
    
		if(1 == frameStarted && 1 == frameEnded) {
			HtDecodeResult conversationResult = MAGIC_NUM_ERROR;
			HtFrameType frameType = ht_determine_frame_type(&receiveBuffer);
			
			Serial.print("frameType: ");
			Serial.println(frameType,HEX);
		  
			switch (frameType) {
				case CMD: {
					conversationResult = ht_decode_cmd(&cmdReceiveBuffer, &receiveBuffer);

					if(OK == conversationResult) {
						cmdFrameReceived = true;
					}
					break;
				}
				case eCMD: {
					/* not implemented, yet */
					break;
				}
				case FLOW: {
					conversationResult = ht_decode_flow(&flowReceiveBuffer, &receiveBuffer);
					if(OK == conversationResult) {
						flowFrameReceived = true;
					}
					break;
				}
				case PSI: {
					conversationResult = ht_decode_psi(&psiReceiveBuffer, &receiveBuffer);

					if(OK == conversationResult) {
						psiFrameReceived = true;
					}
					break;
				}
				default: {
				/* should never happen */
			}
			};
		  
			frameStarted = 0;
			frameEnded = 0;
			return; /* stop evaluation of serial buffer for this run */
		}
	}
}



