/* demo_header_encode.c
 * 
 * 
 */

#include <stdio.h>  
#include <string.h>
#include <ht_decode_encode.h>
#include <ht_helper.h>

int usage()
{
    fputs("Usage: demo_header_encode [--help/-h]\n", stdout);
    fputs("\n", stdout);
    return -1;
}

int main(int argc, char *argv[])
{

    DBYTE header1, header2, header3, header4, header5;
    if (argc > 1) {
        if( strcmp(argv[1],"--help" ) == 0 || strcmp(argv[1],"-h" ) == 0) {
            return usage();
        }
    } 

    printf("[1] ht_enconde_header( CMD,  false, 5 )\n");
    printf("[2] ht_enconde_header( eCMD,  false, 6 )\n");
    printf("[3] ht_enconde_header( FLOW, false, 7 )\n");
    printf("[4] ht_enconde_header( PSI,  false, 8 )\n");
    printf("[5] ht_enconde_header( eCMD, true,  9645 )\n");

    header1 = ht_enconde_header( CMD,  false, 5 );
    header2 = ht_enconde_header( eCMD, false, 6 );
    header3 = ht_enconde_header( FLOW, false, 7 );
    header4 = ht_enconde_header( PSI,  false, 8 );
    header5 = ht_enconde_header( eCMD, true,  9645 );

    fputs("[1] ---------------------------------\n", stdout);
    ht_debug_header(header1);
    fputs("[2] ---------------------------------\n", stdout);
    ht_debug_header(header2);
    fputs("[3] ---------------------------------\n", stdout);
    ht_debug_header(header3);
    fputs("[4] ---------------------------------\n", stdout);
    ht_debug_header(header4);
    fputs("[5] ---------------------------------\n", stdout);
    ht_debug_header(header5);

    return 0;
}
