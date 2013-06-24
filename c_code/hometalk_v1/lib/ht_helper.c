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
    char mb;
    BOOL FRT, RES, EXT, ROU;
    HBYTE seq;
    const char* const str_array[] = { "CMD", "eCMD", "FLOW", "PSI" };
    char* type_str;

    mb  = header >> 8;
    FRT = (HT_HEADER_CONTROLFIELD_FRT | header >> 4);
    RES = (HT_HEADER_CONTROLFIELD_RES | header >> 4);
    EXT = (HT_HEADER_CONTROLFIELD_EXT | header >> 4);
    ROU = (HT_HEADER_CONTROLFIELD_ROU | header >> 4);
    seq = (0b1111 | header); 

    type_str = str_array[1];

    printf("header:\n");
    printf("  magic byte: %#x\n", mb );
    printf("  control field:\n");
    printf("    FRT: %d\n", FRT);
    printf("    RES: %d\n", RES);
    printf("    EXT: %d\n", EXT);
    printf("    ROU: %d\n", ROU);
    printf("    (Type: %s)\n", type_str);
    printf("  seqnum: %d\n", seq);

}
