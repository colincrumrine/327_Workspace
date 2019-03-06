#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gallery.h"
#include "game.h"

using namespace std;

void io_init_terminal(void)
{
	initscr();
	raw();
	noecho();
	curs_set(0);
	keypad(stdscr,TRUE);
	start_color();
}

int main(int argc, char *argv[])
{
  io_init_terminal();
  while(1)
  {
  
  ifstream file( "opening_screen.txt" );
  stringstream buffer;
  buffer << file.rdbuf();
  file.close();
  
  init_pair(1, COLOR_MAGENTA , COLOR_BLACK);		
  attron(COLOR_PAIR(1));
  printw("%s", buffer.str().c_str());
  attroff(COLOR_PAIR(1));

  int initialKey = 0;
  while(initialKey != 27 && initialKey != 'G' && initialKey != 10)
  {
	  initialKey = getch();
  }
  
  switch(initialKey)
  {
	  case 27:
	  
	  endwin();
	  exit(-1);
	  
	  break;
	  
	  case 'G':
	  
	  mainGallery();
	  
	  clear();
	  refresh();
	  
	  break;
	  
	  case 10:
	  
	  play();
	  
	  clear();
	  refresh();
	  
	  break;
  }
  
  }
  
}