/* ht_test.c
 * 
 * 
 */

#include <stdio.h>   /* required for file operations */
#include <string.h>

FILE *fr;            /* declare the file pointer */

typedef struct 
{
    unsigned short addr; // little endian
    unsigned short device; // little endian
    unsigned char cmd[4];
    unsigned short value;
    unsigned char seqnum;
} hometalkCommand;

int usage()
{
    fputs("Usage: ht_test <testfile> [--help/-h]\n", stdout);
    fputs("\n", stdout);
    return -1;
}

int main(int argc, char *argv[])
{
    size_t result;
    char   frame[16];
    char   data[11];
    hometalkCommand* command;
    char*  filename;
    int    i;
    unsigned long test;

    if (argc > 1) {
        filename = argv[1];
        if( strcmp(argv[1],"--help" ) == 0 || strcmp(argv[1],"-h" ) == 0) {
            return usage();
        }
    } else {
        return usage();
    }

    fr = fopen(filename, "rb");  /* open the file for reading */
    if (fr==NULL) {
        fputs ("File error",stderr);
        return 1;
    }

    result = fread(frame,1,16,fr);
    if (result != 16) {
        fputs("Reading error (16 bytes expected)",stderr);
        return 3;
    }

    printf("data:\n");
    for(i = 0; i < 16; i++) {
        printf("%02hhx ", (unsigned char)frame[i]);
        if( (i+1) % 8 == 0 ) fputs("\n", stdout);
    }
    memcpy(data, &frame[4], sizeof(hometalkCommand)-1);
    command = (hometalkCommand *)data;

    test = (unsigned long)(
                (command->cmd[0] << 24) |
                (command->cmd[1] << 16) | 
                (command->cmd[2] <<  8) |
                (command->cmd[3] <<  0));
    printf("\n");
    printf("command:\n");
    printf("  addr:   %d\n", command->addr);
    printf("  device: %d\n", command->device);
    printf("  cmd:    0x%02hhx%02hhx%02hhx%02hhx\n",
                command->cmd[0], command->cmd[1], command->cmd[2], command->cmd[3]);
    printf("  cmd:    0x%08lx\n", test); //(test>>8) | (test<<8));
    printf("  value:  %d\n", command->value);
    printf("  seqnum: %d\n", command->seqnum);

    fclose(fr);
    return 0;
}

