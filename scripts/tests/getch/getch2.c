#include <curses.h>

int main(int argc, char **argv)
{
  int ch;
  initscr();
  cbreak();
  printw("press any key to exit...");
  ch = getch();
  printw("%d\n",ch);
  endwin();
  return 0;
}
