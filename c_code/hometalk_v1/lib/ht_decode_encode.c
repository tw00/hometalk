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
    char FRT, RES, EXT, ROU;
    HBYTE seq_hbyte = seq % 16; // = 2^4 

    RES = 1;
    FRT = 0; EXT = 0;
    switch( type ) {
        case CMD:  FRT = 0; EXT = 0; break;
        case eCMD: FRT = 0; EXT = 1; break;
        case FLOW: FRT = 1; EXT = 0; break;
        case PSI:  FRT = 1; EXT = 1; break;
        default:
            // TODO
        break;
    }

    if( isRoutedFrame ) ROU = 1;
    else                ROU = 0;

    return (DBYTE)(
        (HT_HEADER_MAGIC_BYTE << 8 ) |
        (HT_HEADER_CONTROLFIELD_FRT << 4 | FRT ) | 
        (HT_HEADER_CONTROLFIELD_RES << 4 | RES ) | 
        (HT_HEADER_CONTROLFIELD_EXT << 4 | EXT ) | 
        (HT_HEADER_CONTROLFIELD_ROU << 4 | ROU ) |
        (seq_hbyte << 0 ));
}
