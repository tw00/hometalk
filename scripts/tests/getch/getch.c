#include <stdio.h>
#include <ncurses.h>

int main(void)
{
        int ch = -1;

        raw();  // line-buffering off, echo off, etc.

        //ch = getchar();
        while(ch == -1)
        {
            //ch = getch();    
            ch = wgetch( stdscr );
            if( ch != -1 )
                printf("%d\n", ch);
        }
        printf("%d\n", ch);

        //int ch;
        //initscr();                      /* Start curses mode            */
        //raw();                          /* Line buffering disabled      */
        //keypad(stdscr, TRUE);           /* We get F1, F2 etc..          */
        //noecho();                       /* Don't echo() while we do getch */
        //ch = getch();
        //endwin();                       /* End curses mode                */
        return 0;
}
