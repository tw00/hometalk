#include "ht_decode_encode.h"

// TODO: CRC

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
hometalkCommand* ht_decode_frame(BYTE* frame, int length)
{
    char   data[11]; // ??????
    hometalkCommand* command;
    // unsigned long test;

    memcpy(data, &frame[4], sizeof(hometalkCommand)-1);
    command = (hometalkCommand *)data;

    return command;
}

/*****************************************************************************/
QUAD_BYTE ht_cmd_as_quad(hometalkCommand* command)
{
    return (QUAD_BYTE)(
        (command->cmd[0] << 24) |
        (command->cmd[1] << 16) | 
        (command->cmd[2] <<  8) |
        (command->cmd[3] <<  0));
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
        case CMD:  FRT = 0x0; EXT = 0x0; break;
        case eCMD: FRT = 0x0; EXT = 0xf; break;
        case FLOW: FRT = 0xf; EXT = 0x0; break;
        case PSI:  FRT = 0xf; EXT = 0xf; break;
        default:
            // TODO
        break;
    }

    if( isRoutedFrame ) ROU = 0xf;
    else                ROU = 0x0;

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
