#include <stdio.h>
#include <ncurses.h>

int main(void)
{
        int ch;

        initscr();                      /* Start curses mode            */
        raw();                          /* Line buffering disabled      */
        keypad(stdscr, TRUE);           /* We get F1, F2 etc..          */
        noecho();                       /* Don't echo() while we do getch */

        printw("Type any character to see it in bold\n");
        ch = getch();
        while (ch != KEY_F(1))
        {
                if(ch == KEY_F(1))      
                        printw("F1 Key pressed: Ending program.\n");

                else
                {       printw("The pressed key is ");
                        attron(A_BOLD);
                        printw("%c\n", ch);
                        attroff(A_BOLD);
                }

                refresh();       
                ch = getch();
        }
        printw("end\n");
        endwin();                       /* End curses mode                */

        return 0;
}
