#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>
#include<time.h>
#include<string.h>
#include "game.h"
#include <vector>
#include "game.h"
#include "monsterMap.h"
#include "heap.h"
#include "switches.h"
#include "generateDungeon.h"

const int dungeonI = 21;
const int dungeonJ = 80;

int speed_cmp(const void *key, const void *with)
{
	if(((character *) key)->nextTurn - ((character *) with)->nextTurn == 0)
	{
		return ((character *) key)->priority - ((character *) with)->priority;
	}
	
   return ((character *) key)->nextTurn - ((character *) with)->nextTurn;
 }
 
void randMove(character *character, uint8_t hardness[21][80])
{
	  int characterMove = (rand() % 7);
	  
	  if(characterMove == 0 && hardness[character->yPos - 1][character->xPos - 1] == 0)
	  {
		  character->xPos = character->xPos - 1;
		  character->yPos = character->yPos - 1;
		  
	  }else if(characterMove == 1 && hardness[character->yPos - 1][character->xPos] == 0)
	  {
		  character->yPos = character->yPos - 1;
		  
	  }else if(characterMove == 2 && hardness[character->yPos][character->xPos - 1] == 0)
	  {
		  character->xPos = character->xPos - 1;
		  
	  }else if(characterMove == 3 && hardness[character->yPos + 1][character->xPos - 1] == 0)
	  {
		  character->xPos = character->xPos - 1;
		  character->yPos = character->yPos + 1;
		  
	  }else if(characterMove == 4 && hardness[character->yPos - 1][character->xPos + 1] == 0)
	  {
		  character->xPos = character->xPos + 1;
		  character->yPos = character->yPos - 1;
	  }else if(characterMove == 5 && hardness[character->yPos + 1][character->xPos + 1] == 0)
	  {
		  character->xPos = character->xPos + 1;
		  character->yPos = character->yPos + 1;
		  
	  }else if(characterMove == 6 && hardness[character->yPos][character->xPos + 1] == 0)
	  {
		  character->xPos = character->xPos + 1;
	  }else if(characterMove == 7 && hardness[character->yPos + 1][character->xPos] == 0)
	  {
		  character->yPos = character->yPos + 1;
	  }
}

void setOutDungeon(char outDungeon[21][80], uint8_t modHardness[21][80], int roomStats[24])
{
	int i,j;
	for(i = 0; i < dungeonI; i++)
			{
				for(j = 0; j < dungeonJ; j++)
				{
					if(i == 0 || i == 20)
					{
						outDungeon[i][j] = '-';
					}else if((j == 0 || j == 79) && i != 0 && i != 20)
					{
						outDungeon[i][j] = '|';
					}else if(modHardness[i][j] == 0)
					{
						outDungeon[i][j] = '#';
					}else{
						outDungeon[i][j] = ' ';
					}
					
				}
				
			}
		 
			int roomPos = 0;
			while(roomPos < 24)
			{
				int startRoomX = roomStats[roomPos];
				roomPos++;
				int startRoomY = roomStats[roomPos];
				roomPos++;
				int maximumX = startRoomX + roomStats[roomPos];
				roomPos++;
				int maximumY = startRoomY + roomStats[roomPos];
				roomPos++;
		
					int c,d;
					for(c = startRoomY; c < (maximumY); c++)
					{
							for(d = startRoomX; d < (maximumX); d++)
							{
								outDungeon[c][d] = '.';
							}
					}	
			}
}

void io_init_terminal(void)
{
	initscr();
	raw();
	noecho();
	curs_set(0);
	keypad(stdscr,TRUE);
	start_color();
}

void setTerminal(char dungeon[21][80], vector<character> characters, vector<object> objects)
{
	int i,k,l;
		for(k = 0; k < 21;k++)
		{
			for(l = 0; l < 80;l++)	
			{
				int colored = 0;
				for(i = 0; i < (signed) characters.size();i++)
				{
									
				if(characters[i].xPos == l && characters[i].yPos == k && characters[i].isDead == 0)
				{
						if(characters[i].color == COLOR_RED)
						{
							init_pair(1, COLOR_RED , COLOR_BLACK);		
							attron(COLOR_PAIR(1));
							mvaddch(k,l,dungeon[k][l]);
							attroff(COLOR_PAIR(1));
							colored = 1;
							
						}
						if(characters[i].color == COLOR_GREEN)
						{
							init_pair(2, COLOR_GREEN , COLOR_BLACK);
							attron(COLOR_PAIR(2));
							mvaddch(k,l,dungeon[k][l]);
							attroff(COLOR_PAIR(2));
							colored = 1;
							
						}
						if(characters[i].color == COLOR_CYAN)
						{
							init_pair(3, COLOR_CYAN , COLOR_BLACK);
							attron(COLOR_PAIR(3));
							mvaddch(k,l,dungeon[k][l]);
							attroff(COLOR_PAIR(3));
							colored = 1;
							
						}
						if(characters[i].color == COLOR_YELLOW)
						{
							init_pair(4, COLOR_YELLOW , COLOR_BLACK);
							attron(COLOR_PAIR(4));
							mvaddch(k,l,dungeon[k][l]);
							attroff(COLOR_PAIR(4));
							colored = 1;
							
						}
						if(characters[i].color == COLOR_MAGENTA)
						{
							init_pair(5, COLOR_MAGENTA , COLOR_BLACK);
							attron(COLOR_PAIR(5));
							mvaddch(k,l,dungeon[k][l]);
							attroff(COLOR_PAIR(5));
							colored = 1;
							
						}
						if(characters[i].color == COLOR_WHITE)
						{
							init_pair(6, COLOR_WHITE , COLOR_BLACK);
							attron(COLOR_PAIR(6));
							mvaddch(k,l,dungeon[k][l]);
							attroff(COLOR_PAIR(6));
							colored = 1;
							
						}
						if(characters[i].color == COLOR_BLUE)
						{
							init_pair(7, COLOR_BLUE , COLOR_BLACK);
							attron(COLOR_PAIR(7));
							mvaddch(k,l,dungeon[k][l]);
							attroff(COLOR_PAIR(7));
							colored = 1;
							
						}
						if(characters[i].color == COLOR_BLACK)
						{
							init_pair(8, COLOR_WHITE , COLOR_BLACK);
							attron(COLOR_PAIR(8));
							mvaddch(k,l,dungeon[k][l]);
							attroff(COLOR_PAIR(8));
							colored = 1;
							
						}	
					}					
				}
				
				for(i = 0; i < (signed) objects.size();i++)
				{
									
				if(objects[i].xPos == l && objects[i].yPos == k && objects[i].isDestroyed == 0 && objects[i].isPickedUp == 0)
				{
						if(objects[i].color == COLOR_RED)
						{
							init_pair(1, COLOR_RED , COLOR_BLACK);		
							attron(COLOR_PAIR(1));
							mvaddch(k,l,dungeon[k][l]);
							attroff(COLOR_PAIR(1));
							colored = 1;
							
						}
						if(objects[i].color == COLOR_GREEN)
						{
							init_pair(2, COLOR_GREEN , COLOR_BLACK);
							attron(COLOR_PAIR(2));
							mvaddch(k,l,dungeon[k][l]);
							attroff(COLOR_PAIR(2));
							colored = 1;
							
						}
						if(objects[i].color == COLOR_CYAN)
						{
							init_pair(3, COLOR_CYAN , COLOR_BLACK);
							attron(COLOR_PAIR(3));
							mvaddch(k,l,dungeon[k][l]);
							attroff(COLOR_PAIR(3));
							colored = 1;
							
						}
						if(objects[i].color == COLOR_YELLOW)
						{
							init_pair(4, COLOR_YELLOW , COLOR_BLACK);
							attron(COLOR_PAIR(4));
							mvaddch(k,l,dungeon[k][l]);
							attroff(COLOR_PAIR(4));
							colored = 1;
							
						}
						if(objects[i].color == COLOR_MAGENTA)
						{
							init_pair(5, COLOR_MAGENTA , COLOR_BLACK);
							attron(COLOR_PAIR(5));
							mvaddch(k,l,dungeon[k][l]);
							attroff(COLOR_PAIR(5));
							colored = 1;
							
						}
						if(objects[i].color == COLOR_WHITE)
						{
							init_pair(6, COLOR_WHITE , COLOR_BLACK);
							attron(COLOR_PAIR(6));
							mvaddch(k,l,dungeon[k][l]);
							attroff(COLOR_PAIR(6));
							colored = 1;
							
						}
						if(objects[i].color == COLOR_BLUE)
						{
							init_pair(7, COLOR_BLUE , COLOR_BLACK);
							attron(COLOR_PAIR(7));
							mvaddch(k,l,dungeon[k][l]);
							attroff(COLOR_PAIR(7));
							colored = 1;
							
						}
						if(objects[i].color == COLOR_BLACK)
						{
							init_pair(8, COLOR_WHITE , COLOR_BLACK);
							attron(COLOR_PAIR(8));
							mvaddch(k,l,dungeon[k][l]);
							attroff(COLOR_PAIR(8));
							colored = 1;
							
						}	
					}					
				}
				
				if(colored == 0)
				{
					mvaddch(k,l,dungeon[k][l]);
				}
				
				
			}
		}
}

void printMonsterList(vector<character> characters)
{
			clear();
			refresh();
			
			int i;
			const char *yDir = "";
			const char *xDir = "";
			int yDiff = 0;
			int yDiffAbs = 0;
			int xDiff = 0;
			int xDiffAbs = 0;
			int escapeKey = 0;
			int startList = 1;
			int endList = characters.size() - 1;
			int initialPrint = 0;
			int initialScroll = 0;
			while(escapeKey != 27)
			{
			
				if(initialPrint == 0 || escapeKey == KEY_DOWN || escapeKey == KEY_UP)
				{
					
					if(escapeKey == KEY_UP)
					{
						if((characters.size() - 1) > 21)
						{
							if(initialScroll == 0)
							{
							startList = 1;
							endList = startList + 20;
 							initialScroll = 1;
							}else{
									endList--;
							
							if(endList - 20 >= 1)
							{
								startList = endList - 20;
							}else{
								endList++;
							}
							
							}
							
						}
					}
					if(escapeKey == KEY_DOWN)
					{
						if((characters.size() - 1) > 21)
						{
							
							if(initialScroll == 0)
							{
							startList = 2;
							endList = startList + 20;
 							initialScroll = 1;
							}else{
								startList++;
							if(startList + 20 <= (signed) (characters.size() - 1))
							{
								endList = startList + 20;
							}else{
								startList--;
							}
							
							}
							
						}
					}
					
					char outStr[1250] = {};
					
					for(i = startList; i <= endList; i++)
					{
						yDiff = characters[i].yPos - characters[0].yPos;
						yDiffAbs = abs(yDiff);
						xDiff = characters[i].xPos - characters[0].xPos;
						xDiffAbs = abs(xDiff);
						if(yDiff < 0)
						{
							yDir = "North";
						}else if(yDir > 0)
						{
							yDir = "South";
						}
						
						if(xDiff < 0)
						{
							xDir = "East";
						}else if(xDir > 0)
						{
							xDir = "West";
						}
						
						char yDiffAbsOut[4];
						sprintf(yDiffAbsOut,"%d", yDiffAbs);
						char xDiffAbsOut[4];
						sprintf(xDiffAbsOut,"%d", xDiffAbs);
						char signString[2];
						signString[0] = characters[i].symb;
						signString[1] = '\0';
						strcat(outStr,signString);
						strcat(outStr,": ");
						strcat(outStr,yDiffAbsOut);
						strcat(outStr," ");
						strcat(outStr,yDir);
						strcat(outStr," by ");
						strcat(outStr,xDiffAbsOut);
						strcat(outStr," ");
						strcat(outStr,xDir);
						strcat(outStr,"\n");
					}
					strcat(outStr, "Scrolling uses a viewing window of 21 or less to\nview lists greater than 21 in length.");
					printw("%s", outStr);
					refresh();
					initialPrint = 1;
				}
				
			escapeKey = getch();
			while(escapeKey != KEY_DOWN && escapeKey != KEY_UP && escapeKey != 27)
			{
				escapeKey = getch();
			}
			
			clear();
			refresh();
			
		   } 
		
			clear();
			refresh();
}
