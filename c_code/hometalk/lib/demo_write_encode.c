/* demo_write_encode.c
 * 
 * 
 */

#include <stdio.h>  
#include <string.h>

#include "ht_decode_encode.h"

int usage()
{
    fputs("Usage: demo_write_encode <exportfile> [--help/-h]\n", stdout);
    fputs("\n", stdout);
    return -1;
}

int main(int argc, char *argv[])
{
    hometalkCommand command;

    BYTE* command_raw;
    char* filename;

    if (argc > 1) {
        filename = argv[1];
        if( strcmp(argv[1],"--help" ) == 0 || strcmp(argv[1],"-h" ) == 0) {
            return usage();
        }
    } else {
        return usage();
    }

    fputs("\n", stdout);
    fputs("Enter <hometalkCommand>\n", stdout);
    fputs(" address: ", stdout); scanf("%u", (unsigned int *)&command.addr);
    fputs(" device: ", stdout); scanf("%u", (unsigned int *)&command.device);
    fputs(" cmd[0] (hex): 0x", stdout); scanf("%x", (unsigned int *)&command.cmd[0]);
    fputs(" cmd[1] (hex): 0x", stdout); scanf("%x", (unsigned int *)&command.cmd[1]);
    fputs(" cmd[2] (hex): 0x", stdout); scanf("%x", (unsigned int *)&command.cmd[2]);
    fputs(" cmd[3] (hex): 0x", stdout); scanf("%x", (unsigned int *)&command.cmd[3]);
    fputs(" value: ", stdout); scanf("%d", (int *)&command.value);
    command.seqnum = 1;

    ht_debug_frame(&command);
    ht_writedata(&command, filename);

    command_raw = ht_encode_frame(&command);
    ht_debug_data(&command_raw);

    return 0;
}
