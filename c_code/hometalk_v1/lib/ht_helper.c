#include "ht_helper.h"

/*****************************************************************************/
int ht_readdata(BYTE* frame, const char* filename)
{
    /* declare the file pointer */
    FILE *fr;            
    size_t result;

    /* open the file for reading */
    fr = fopen(filename, "rb");  
    if (fr==NULL) {
        fputs ("File error", stderr);
        return 1;
    }

    result = fread(frame,1,HT_FRAME_LENGTH,fr);
    if (result != HT_FRAME_LENGTH) {
        fputs("Reading error (16 bytes expected)", stderr);
        return 3;
    }
    fclose(fr);
    return 0;
}
/*****************************************************************************/
int ht_writedata(hometalkCommand* cmd, const char* filename)
{
    /* declare the file pointer */
    FILE *fr;            
    size_t result;

    /* open the file for reading */
    fr = fopen(filename, "wb");  
    if (fr==NULL) {
        fputs ("File error", stderr);
        return 1;
    }

    result = fwrite(cmd,1,HT_FRAME_LENGTH,fr);
    if (result != HT_FRAME_LENGTH) {
        fputs("Writing error (16 bytes expected)", stderr);
        return 3;
    }

    fclose(fr);
    return 0;
}


/*****************************************************************************/
void ht_debug_data(BYTE* frame)
{
    int i;

    printf("data:\n");
    for(i = 0; i < 16; i++) {
        printf("%02hhx ", (unsigned char)frame[i]);
        if( (i+1) % 8 == 0 ) fputs("\n", stdout);
    }
}

/*****************************************************************************/
void ht_debug_frame(hometalkCommand* command)
{
    printf("\n");
    printf("command:\n");
    printf("  addr:   %d\n", command->addr);
    printf("  device: %d\n", command->device);
    printf("  cmd:    0x%02hhx%02hhx%02hhx%02hhx\n",
                command->cmd[0],
                command->cmd[1],
                command->cmd[2],
                command->cmd[3]);
    // printf("  cmd:    0x%08lx\n", test); //(test>>8) | (test<<8));
    printf("  value:  %d\n", command->value);
    printf("  seqnum: %d\n", command->seqnum);
}

/*****************************************************************************/
void ht_debug_header(DBYTE header)
{
    int x = 0;
    const char* type_str = "test";
    
    printf("header:\n");
    printf("  magic byte: %d\n", x );
    printf("  control field:\n");
    printf("    FRT: %d\n", x);
    printf("    RES: %d\n", x);
    printf("    EXT: %d\n", x);
    printf("    ROU: %d\n", x);
    printf("    (Type: %s)\n", type_str);
    printf("  seqnum: %d\n", x);

}
