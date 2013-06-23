/*
 The circuit: 
 * RX is digital pin 2 (connect to TX of other device)
 * TX is digital pin 5 (connect to RX of other device)
 * IR is digital pin 3
 */
 
// TODO: Befehlspipeline
// TODO: Autotalk
// TODO: Bei unbekanntem Befehl nichts tun
// TODO: echo -n Return\! > /dev/tty.usbmodemfa131
// TODO: Nach "+Ethernet eth" parsen und bis "Please press Enter to activate this console. " ignorieren
// TOOD: "^[" immer ignorieren (dmesg)

// echo -n "foo" > /dev/ttyS0
// cat < /dev/ttyS0

// CMD:       ***device:cmd:seq!
// Response:  ###R:status:seq!
  
#include <IRremote.h>
#include <HashMap.h>

#define DEBUG false
#define DEVICE_ADDR 0x01

#define PANASONIC_ADDRESS 0x4004 // Panasonic address (Pre data)
IRsend irsend;

unsigned int pos = 0;
/*unsigned int magic = 0;*/
char input_buffer[16];
char data[11];

/*char device[16]; // 1 char = 2 byte ??
char command[16];
char seqnum[8];
int readMode = 0;*/

typedef struct
{
    unsigned int addr; // little endian
    unsigned int device; // little endian
    unsigned char cmd[4];
    unsigned int value;
    unsigned char seqnum;
} hometalkCommand;

unsigned long code;
hometalkCommand* command;

const byte PANASONIC_HASH_SIZE = 65;
HashType<char*,unsigned long> panasonicHashType[PANASONIC_HASH_SIZE];
HashMap<char*,unsigned long> panasonic_code_map = HashMap<char*,unsigned long>(panasonicHashType, PANASONIC_HASH_SIZE);

// *******************************************************************
//
void load_panasonic_codes()
{
  // Name: Panasonic_N2QAYB000239
  panasonic_code_map[0]("Power", 0x0100BCBD);
  panasonic_code_map[1]("TV", 0x01400C4D);
  panasonic_code_map[2]("AV", 0x0100A0A1);
  panasonic_code_map[3]("Aspect", 0x01207B5A);
  panasonic_code_map[4]("Link", 0x01908D1C);
  panasonic_code_map[5]("Option", 0x0190E574);
  panasonic_code_map[6]("SDCard", 0x0190D544);
  panasonic_code_map[7]("Guide", 0x0190E170);
  panasonic_code_map[8]("Exit", 0x0100CBCA);
  panasonic_code_map[9]("Up", 0x01005253);
  panasonic_code_map[10]("Down", 0x0100D2D3);
  panasonic_code_map[11]("Left", 0x01007273);
  panasonic_code_map[12]("Right", 0x0100F2F3);
  panasonic_code_map[13]("Ok", 0x01009293);
  panasonic_code_map[14]("Menu", 0x01004A4B);
  panasonic_code_map[15]("Return", 0x01002B2A);
  panasonic_code_map[16]("Red", 0x01000E0F);
  panasonic_code_map[17]("Green", 0x01008E8F);
  panasonic_code_map[18]("Yellow", 0x01004E4F);
  panasonic_code_map[19]("Blue", 0x0100CECF);
  panasonic_code_map[20]("Text", 0x0180C041);
  panasonic_code_map[21]("Sttl", 0x0180A021);
  panasonic_code_map[22]("Index", 0x01801091);
  panasonic_code_map[23]("Hold", 0x01809011);
  panasonic_code_map[24]("1", 0x01000809);
  panasonic_code_map[25]("2", 0x01008889);
  panasonic_code_map[26]("3", 0x01004849);
  panasonic_code_map[27]("4", 0x0100C8C9);
  panasonic_code_map[28]("5", 0x01002829);
  panasonic_code_map[29]("6", 0x0100A8A9);
  panasonic_code_map[30]("7", 0x01006869);
  panasonic_code_map[31]("8", 0x0100E8E9);
  panasonic_code_map[32]("9", 0x01001819);
  panasonic_code_map[33]("0", 0x01009899);
  panasonic_code_map[34]("Info", 0x01009C9D);
  panasonic_code_map[35]("Mute", 0x01004C4D);
  panasonic_code_map[36]("ProgUp", 0x01002C2D);
  panasonic_code_map[37]("ProgDown", 0x0100ACAD);
  panasonic_code_map[38]("VolUp", 0x01000405);
  panasonic_code_map[39]("VolDown", 0x01008485);
  panasonic_code_map[40]("VcrProgDown", 0x0900ACA5);
  panasonic_code_map[41]("VcrProgUp", 0x09002C25);
  panasonic_code_map[42]("VcrSkipBack", 0x09004049);
  panasonic_code_map[43]("VcrSkipFwd", 0x0900C0C9);
  panasonic_code_map[44]("VcrRewind", 0x0900929B);
  panasonic_code_map[45]("VcrFFwd", 0x0900525B);
  panasonic_code_map[46]("VcrStop", 0x09000009);
  panasonic_code_map[47]("VcrPause", 0x09006069);
  panasonic_code_map[48]("VcrPlay", 0x09005059);
  panasonic_code_map[49]("VcrPower", 0x0900BCB5);
  panasonic_code_map[50]("VcrRec", 0x09001019);
  panasonic_code_map[51]("DvdProgDown", 0x0D00ACA1);
  panasonic_code_map[52]("DvdProgUp", 0x0D002C21);
  panasonic_code_map[53]("DvdSkipBack", 0x0D00929F);
  panasonic_code_map[54]("DvdSkipFwd", 0x0D00525F);
  panasonic_code_map[55]("DvdRewind", 0x0D00202D);
  panasonic_code_map[56]("DvdFFwd", 0x0D00A0AD);
  panasonic_code_map[57]("DvdStop", 0x0D00000D);
  panasonic_code_map[58]("DvdPause", 0x0D00606D);
  panasonic_code_map[59]("DvdPlay", 0x0D00505D);
  panasonic_code_map[60]("DvdPower", 0x0D00BCB1);
  panasonic_code_map[61]("DvdRec", 0x0D00101D);
  panasonic_code_map[62]("N", 0x01003031);
  panasonic_code_map[63]("Audio", 0x01008C8D);
  panasonic_code_map[64]("DirectTvRec", 0x01909100);
}
  
// *******************************************************************
//
void setup()  
{  
  Serial.begin(9600);
  Serial.println("DEBUG:");
  
/*  memset(device,  '\0', sizeof(device));
  memset(command, '\0', sizeof(command));
  memset(seqnum,  '\0', sizeof(seqnum)); */
    
  load_panasonic_codes();
  // panasonic_code_map.debug();    
}

// *******************************************************************
//
int execCommand(unsigned int device, unsigned int command)
{
  switch(device)
  {
      case 1: // generic IR
      break;
      
      case 2: // panasonic
        if( command == 100) {
          Serial.println("POWER ON ROUTINE...");
          code = panasonic_code_map.getValueOf("Power");  
          for(int i = 1; i < 7; i++) {
            irsend.sendPanasonic(PANASONIC_ADDRESS, code);
            delayMicroseconds(73900/2);                     // TODO: #define
          } 
        } else {
//          code = panasonic_code_map.getValueOf(command);
          code = 0x00000000;
          Serial.print("Code: ");
          Serial.println(code, HEX);
          irsend.sendPanasonic(PANASONIC_ADDRESS, code);          
        }

      break;
      
      default: return -1;
  }
  return device;
}

// *******************************************************************
//
void loop()
{  
  // TODO: Autodiscover
  
  char sym;
  int statusByte;
  int i;
  unsigned long cmd_num;
  
  if (Serial.available()) {
    sym = Serial.read();  

    memcpy( input_buffer, &input_buffer[1], 15 );
    input_buffer[15] = sym;
    
    if(DEBUG) {
      for(i = 0; i < 16; i++){
          Serial.print(input_buffer[i], HEX);
          Serial.print(" ");
      }
      Serial.println();
    }
    
    if((input_buffer[0] & 0x000000FF) == 0xD7 &&
       (input_buffer[1] & 0x000000FF) == 0xD7 &&
       (input_buffer[2] & 0x000000FF) == 0xD7 &&       
       (input_buffer[15] & 0x000000FF) == 0x21) {
        if((input_buffer[3] & 0x000000FF) == 0x31) {
            memcpy(data, &input_buffer[4], sizeof(hometalkCommand));
            command = (hometalkCommand *)data;
            
            cmd_num = (unsigned long)(
                (((unsigned long)command->cmd[0] | 0x00000000) << 24) |
                (((unsigned long)command->cmd[1] | 0x00000000) << 16) |
                ((command->cmd[2] | 0x00000000) <<  8) |
                ((command->cmd[3] | 0x00000000) <<  0));
            
            Serial.println("Command:");
            Serial.print("  ->addr:   ");
            Serial.println(command->addr, DEC);
            Serial.print("  ->device: ");            
            Serial.println(command->device, DEC);
            Serial.print("  ->cmd:    ");
            Serial.println(cmd_num, HEX);            
            Serial.print("  ->value:  ");            
            Serial.println(command->value, DEC);
            Serial.print("  ->seqnum: ");            
            Serial.println(command->seqnum, DEC);            
            memset(input_buffer, '\0', sizeof(input_buffer)); 
            
            if( command->addr == DEVICE_ADDR ) {
              Serial.println("go..");
            }
      }
    }
  }
    
    /*if( sym == 0xD7 )
    
    if( magic == 3 && sym == 0xD7 ) return; // HACK?
    if( magic == 3 ) {
      
      iif(sym != '!') {
        if ( sym != ':') {
          switch(readMode) {
            case 0: device[pos] = sym; break;
            case 1: command[pos] = sym; break;
            case 2: seqnum[pos] = sym; break;
          }
        }
        pos++;
        if( sym == ':' || pos == 32) {       
          readMode++;
          pos = 0;
        }
      }
            
      if(sym == '!' || pos == 32) { // HACK
        magic = 0;       
        pos = 0;
            
        statusByte = execCommand(device, command);
    
        Serial.print("###R:");
        Serial.print(statusByte);
        Serial.print(":");
        Serial.print(seqnum);
        Serial.print("!\r\n");

        readMode = 0;
        memset(device,  '\0', sizeof(device));
        memset(command, '\0', sizeof(command));
        memset(seqnum,  '\0', sizeof(seqnum)); 
      }    
    } else {
      if( sym == 0xD7 ) { magic++; }
      else             { magic = 0; }
    }
  }*/
    
  // TODO: Recv Commands
  // if (Serial.available())
  // mySerial.write(Serial.read());
}


