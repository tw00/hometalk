/* demo_read_decode.c
 * 
 * 
 */

#include <stdio.h>  
#include <string.h>
#include <ht_decode_encode.h>

int usage()
{
    fputs("Usage: demo_read_decode <testfile> [--help/-h]\n", stdout);
    fputs("\n", stdout);
    return -1;
}

int main(int argc, char *argv[])
{
    BYTE command_raw[HT_FRAME_LENGTH];

    hometalkCommand* command;
    char* filename;

    if (argc > 1) {
        filename = argv[1];
        if( strcmp(argv[1],"--help" ) == 0 || strcmp(argv[1],"-h" ) == 0) {
            return usage();
        }
    } else {
        return usage();
    }

    ht_readdata(command_raw, filename);
    ht_debug_data(&command_raw);

    command = ht_decode_frame(command_raw, HT_FRAME_LENGTH);
    ht_debug_frame(command);

    return 0;
}
