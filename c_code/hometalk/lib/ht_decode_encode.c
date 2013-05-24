#include "ht_decode_encode.h"

// TODO: CRC

/*****************************************************************************/
BYTE* ht_encode_frame(hometalkCommand* ht_cmd)
{
    BYTE frame[HT_FRAME_LENGTH];

    frame[0] = 0xd7;
    frame[1] = 0xd7;
    frame[2] = 0xd7;

//    memcpy(&frame[4], ht_cmd, sizeof(hometalkCommand)-1);

    return &frame;
}

/*****************************************************************************/
hometalkCommand* ht_decode_frame(BYTE* frame, int length)
{
    char   data[11]; // ??????
    hometalkCommand* command;
    unsigned long test;

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
