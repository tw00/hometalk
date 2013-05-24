/*
 *  cbreak.c
 *  Pieter Droogendijk
 */
#include <termios.h>
#include <stdio.h>

int set_cbreak(struct termios *save_termios)
{
    struct termios buf;

    /* Remove line buffering from stdin */
    if (setvbuf(stdin, NULL, _IONBF, 0) != 0)
        return -1;

    /* Store previous terminal settins */
    if (tcgetattr(0, save_termios))
        return -1;

    /* Modify terminal settings. */
    buf = *save_termios;
    buf.c_lflag &= ~(ECHO|ICANON);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;

    /* Set new terminal settings. */
    if (tcsetattr(0, TCSAFLUSH, &buf) < 0)
        return -1;
    return 0;
}

int unset_cbreak(struct termios *save_termios)
{
    /* Return line buffering to stdin. */
    if (setvbuf(stdin, NULL, _IOLBF, 0) != 0)
        return -1;

    /* Return original terminal settings. */
    if (tcsetattr(0, TCSAFLUSH, save_termios) < 0)
        return -1;
    return 0;
}

int pause(void)
{
    int ch;
    struct termios save_termios;
    set_cbreak(&save_termios);
    fflush(stdout);
    ch = getchar();
    if( ch == 27 ) {
        ch = getchar();
        if( ch == 91 ) {
            ch = getchar();
            ch = ch + 2000;
        }
        ch = ch + 1000;
    }
    if( ch == 195 ) {
        ch = getchar();
        ch = ch + 10000;
    }
    unset_cbreak(&save_termios);
    return ch;
}

// int main(void)
int main(int argc, char *argv[])
{
    int ch;
    char mode;
    if (argc > 1) {
        if( strcmp(argv[1],"--help" ) == 0 || strcmp(argv[1],"-h" ) == 0) {
            fputs("Usage: cbreak <mode>\n", stdout);
            fputs("    mode:\n", stdout);
            fputs("      -d   decimal (default)\n", stdout);
            fputs("      -h   hex\n", stdout);
            fputs("      -c   char\n", stdout);
            fputs("\n", stdout);
            return -1;
        }
        if( strcmp(argv[1],"-d" ) == 0 ) mode = 1;
        if( strcmp(argv[1],"-h" ) == 0 ) mode = 2;
        if( strcmp(argv[1],"-c" ) == 0) mode = 3;
    } else {
        mode = 1;
    }

    ch = pause();
    switch(mode) {
        case 1: printf("%d\n", ch); break;
        case 2: printf("%#x\n", ch); break;
        case 3: printf("%c\n", ch); break;

    }
    return 0;
}

