#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector> 
#include <stdlib.h>  
#include <time.h>
#include <unistd.h>
#include <cmath>
#include "game.h"
#include "gameUtil.h"

using namespace std;

int i,j;

int isTrailThere(int xPos, int yPos, vector<racer> racers, racer r)
{
	int rI, rXY;
	for(rI = 0; rI < (signed) racers.size(); rI++)
	{
		for(rXY = 0; rXY < (signed) racers[rI].xTrail.size(); rXY++)
		{
			
			if(r.color != racers[rI].color && xPos == racers[rI].xTrail[rXY] && yPos == racers[rI].yTrail[rXY])
			{
				return 1;
			}
		
		}
	}
	return 0;
}

void cluAI(racer& r, racer pc, vector<racer> racers)
{
	int xDiff = r.xPos - pc.xPos;
	int yDiff = r.yPos - pc.yPos;
	
	int willMove = rand() % 10;
	
	int cluKnows = rand() % 10;
	
	if(cluKnows == 8)
	{
		if(willMove >= 5)
		{
			int moveToDo = rand() % 2;
			
			if(moveToDo == 0 && xDiff > 0 && r.xPos - 1 != 0)
			{
				r.xPos = r.xPos - 1;
				
			}else if(moveToDo == 0 && xDiff < 0 && r.xPos + 1 != 131)
			{
				r.xPos = r.xPos + 1;
			}
			
			if(moveToDo == 1 && yDiff > 0 && r.yPos - 1 != 0)
			{
				r.yPos = r.yPos - 1;
				
			}else if(moveToDo == 1 && yDiff < 0 && r.yPos + 1 != 40)
			{
				r.yPos = r.yPos + 1;
			}	
		}	
	}else{
		if(willMove >= 5)
		{
			int moveToDo = rand() % 2;
			
			if(moveToDo == 0 && xDiff > 0 && r.xPos - 1 != 0)
			{
				if(isTrailThere(r.xPos - 1, r.yPos, racers,r) == 0)
				{
					r.xPos = r.xPos - 1;
				}
				
			}else if(moveToDo == 0 && xDiff < 0 && r.xPos + 1 != 131)
			{
				if(isTrailThere(r.xPos + 1, r.yPos, racers,r) == 0)
				{
					r.xPos = r.xPos + 1;
				}
			}
			
			if(moveToDo == 1 && yDiff > 0 && r.yPos - 1 != 0)
			{
				if(isTrailThere(r.xPos, r.yPos - 1, racers,r) == 0)
				{
					r.yPos = r.yPos - 1;
				}
				
			}else if(moveToDo == 1 && yDiff < 0 && r.yPos + 1 != 40)
			{
				if(isTrailThere(r.xPos, r.yPos + 1, racers,r) == 0)
				{
					r.yPos = r.yPos + 1;
				}	
			}
		}
	
	}
	
}

void flynnAI(racer& r, int& flynnTurns, vector<racer> racers)
{
	
	int randBump = rand() % 10;
	
	if(randBump >= 0 && randBump <= 7)
	{
		if(flynnTurns >= 0 && flynnTurns <= 7)
		{
			if(r.yPos - 1 != 0)
			{
				r.yPos = r.yPos - 1;
				flynnTurns++;
			}else{
				flynnTurns = 8;
			}
		}
	
		if(flynnTurns >= 8 && flynnTurns <= 15)
		{
			if(r.xPos + 1 != 131)
			{
				r.xPos = r.xPos + 1;
				flynnTurns++;
			}else{
				flynnTurns = 16;
			}
		}
	
		if(flynnTurns >= 16 && flynnTurns <= 23)
		{
			if(r.yPos + 1 != 40)
			{
				r.yPos = r.yPos + 1;
				flynnTurns++;
			}else{
				flynnTurns = 24;
			}
		}
		
		if(flynnTurns >= 24 && flynnTurns <= 31)
		{
			if(r.xPos - 1 != 0)
			{
				r.xPos = r.xPos - 1;
				flynnTurns++;
			}else{
				flynnTurns = 0;
			}
		}
	
	}else{
		int isPlaced = 0;
		
		int randTurnReset = rand() % 4;
		switch(randTurnReset)
		{
			case 0:
			
			flynnTurns = 0;
			
			break;
			
			case 1:
			
			flynnTurns = 8;
			
			break;
			
			case 2:
			
			flynnTurns = 16;
			
			break;
			
			case 3:
			
			flynnTurns = 24;
			
			break;
		}
		
		while(isPlaced == 0)
		{
			int randXY = rand() % 4;
			switch(randXY)
			{
				case 0:
				
				if(r.xPos + 1 != 131)
				{
					r.xPos = r.xPos + 1;
					isPlaced = 1;
				}
				
				break;
				
				case 1:
				
				if(r.xPos - 1 != 0)
				{
					r.xPos = r.xPos - 1;
					isPlaced = 1;
				}
				
				break;
				
				case 2:
				
				if(r.yPos + 1 != 40)
				{
					r.yPos = r.yPos + 1;
					isPlaced = 1;
				}
				
				break;
				
				case 3:
				
				if(r.yPos - 1 != 0)
				{
					r.yPos = r.yPos - 1;
					isPlaced = 1;
				}
				
				break;
			}
		}
	}
	

}

void tronAI(racer& r, vector<racer> racers)
{
	
	int doMove = rand () % 10, tronKnows = rand() % 5;
	
	if(doMove >= 0 && doMove <= 6)
	{
	int placed = 0, turnCount = 0;
	while(placed == 0)
	{
		
		int randNum = rand() % 4;
		
			if(randNum == 0 && r.xPos  + 1 != 131 && r.yPos  + 1 != 40)
			{
				int k, notValid = 0;
				for(k = 0; k < (signed) r.xTrail.size(); k++)
				{
					if(r.xTrail[k] == r.xPos + 1 && r.yTrail[k] == r.yPos + 1)
					{
						notValid = 1;
					}
				}
				
				if(notValid == 0)
				{
					if(tronKnows >= 2)
					{
						if(isTrailThere(r.xPos + 1,r.yPos + 1,racers,r) == 0)
						{
							r.xPos = r.xPos + 1;
							r.yPos = r.yPos + 1;
							placed = 1;
						}
					}else{
						r.xPos = r.xPos + 1;
						r.yPos = r.yPos + 1;
						placed = 1;
					}
				
				}
				
			}
			
			if(randNum == 1 && r.xPos  + 1 != 131 && r.yPos - 1 != 0)
			{
				int k, notValid = 0;
				for(k = 0; k < (signed) r.xTrail.size(); k++)
				{
					if(r.xTrail[k] == r.xPos + 1 && r.yTrail[k] == r.yPos - 1)
					{
						notValid = 1;
					}
				}
				
				if(notValid == 0)
				{
					if(tronKnows >= 2)
					{
						if(isTrailThere(r.xPos + 1,r.yPos - 1,racers,r) == 0)
						{
							r.xPos = r.xPos + 1;
							r.yPos = r.yPos - 1;
							placed = 1;
						}
					}else{
						r.xPos = r.xPos + 1;
						r.yPos = r.yPos - 1;
						placed = 1;
					}
				}
				
			}
			
			if(randNum == 2 && r.xPos - 1 != 0 && r.yPos - 1 != 0)
			{
				int k, notValid = 0;
				for(k = 0; k < (signed) r.xTrail.size(); k++)
				{
					if(r.xTrail[k] == r.xPos - 1 && r.yTrail[k] == r.yPos - 1)
					{
						notValid = 1;
					}
				}
				
				if(notValid == 0)
				{
					if(tronKnows >= 2)
					{
						if(isTrailThere(r.xPos - 1,r.yPos - 1,racers,r) == 0)
						{
							r.xPos = r.xPos - 1;
							r.yPos = r.yPos - 1;
							placed = 1;
						}
					}else{
						r.xPos = r.xPos - 1;
						r.yPos = r.yPos - 1;
						placed = 1;
					}
				}
				
			}
			
			if(randNum == 3 && r.xPos - 1 != 0 && r.yPos + 1 != 40)
			{
				int k, notValid = 0;
				for(k = 0; k < (signed) r.xTrail.size(); k++)
				{
					if(r.xTrail[k] == r.xPos - 1 && r.yTrail[k] == r.yPos + 1)
					{
						notValid = 1;
					}
				}
				
				if(notValid == 0)
				{
					if(tronKnows >= 2)
					{
						if(isTrailThere(r.xPos - 1,r.yPos + 1,racers,r) == 0)
						{
							r.xPos = r.xPos - 1;
							r.yPos = r.yPos + 1;
							placed = 1;
						}
					}else{
						r.xPos = r.xPos - 1;
						r.yPos = r.yPos + 1;
						placed = 1;
					}
				}
				
			}
			turnCount++;
			if(turnCount == 8)
			{
				if(r.xPos - 1 != 0)
				{
					r.xPos = r.xPos - 1;
					break;
				}else if(r.xPos + 1 != 131){
					r.xPos = r.xPos + 1;
					break;
				}
			}
			
	}

	 
  }

}

void checkPCTrail(racer& r, int turns)
{
	if(turns > 14 && r.xTrail.size() >= 1)
	{
		r.xTrail.erase(r.xTrail.begin());
		r.yTrail.erase(r.yTrail.begin());
	}
}

void racerMoves(vector<racer>& racers, int& flynnTurns)
{
	if(racers[1].isDead == 0)
	{
		addToTrail(racers[1]);
		cluAI(racers[1], racers[0], racers);
	}
	
	if(racers[2].isDead == 0)
	{
	addToTrail(racers[2]);
	tronAI(racers[2], racers);
	}
	
	if(racers[3].isDead == 0)
	{
	addToTrail(racers[3]);
	flynnAI(racers[3],flynnTurns, racers);
	}

	for(i = 0; i < (signed) racers.size(); i++)
	{
		int a,b;
		for(a = 0; a < (signed) racers.size(); a++)
		{
			for(b = 0; b < (signed) racers[a].xTrail.size(); b++)
			{
			
				if(((racers[i].xPos == racers[a].xTrail[b] && racers[i].yPos == racers[a].yTrail[b]))
					&& racers[i].color != racers[a].color && racers[a].isDead == 0)
				{
					racers[i].isDead = 1;
				}else if((racers[i].xPos == racers[a].xPos && racers[i].yPos == racers[a].yPos) && racers[i].color != racers[a].color && racers[a].isDead == 0 && racers[i].isDead == 0)
				{
					racers[i].isDead = 1;
					racers[a].isDead = 1;
				}
			
			}
			
		}
	}
	
}


int checkUserInput(int userInput, racer& r, int& turns)
{
	switch(userInput)
	{
	
	case 27:

	return 1;
	
	break;
	
	case KEY_DOWN:
	
	if(r.yPos +  1 != 40)
	{
		addToTrail(r);
		r.yPos += 1;
		turns = 0;
	}else{
     return 1;
	}
	
	break;
	
	case KEY_UP:
	
	if(r.yPos -  1 != 0)
	{
		addToTrail(r);
		r.yPos -= 1;
		turns = 0;
	}else{
		return 1;
	}
	
	break;
	
	case KEY_LEFT:
	
	if(r.xPos -  1 != 0)
	{
		addToTrail(r);
		r.xPos -= 1;
		turns = 0;
	}else{
		return 1;
	}
	
	break;
	
	case KEY_RIGHT:
	
	if(r.xPos +  1 != 131)
	{
		addToTrail(r);
		r.xPos += 1;
		turns = 0;
	}else{
		return 1;
	}
	
	break;
	
	}
	
	return 0;
}

void makeRacers(vector<racer>& racers)
{
	racer pc;
	pc.xPos = 130;
	pc.yPos = 39;
	pc.isDead = 0;
	pc.isPlayer = 1;
	pc.color = COLOR_CYAN;
	racers.push_back(pc);

	racer clue;
	clue.xPos = 1;
	clue.yPos = 1;
	clue.isDead = 0;
	clue.isPlayer = 0;
	clue.color = COLOR_YELLOW;
	racers.push_back(clue);

	racer trone;
	trone.xPos = 130;
	trone.yPos = 1;
	trone.isDead = 0;
	trone.isPlayer = 0;
	trone.color = COLOR_BLUE;
	racers.push_back(trone);

	racer flin;
	flin.xPos = 1;
	flin.yPos = 39;
	flin.isDead = 0;
	flin.isPlayer = 0;
	flin.color = COLOR_GREEN;
	racers.push_back(flin);
}


void addToTrail(racer& r)
{
	
	if(r.xTrail.size() >= 32)
	{
		r.xTrail.erase(r.xTrail.begin());
		r.yTrail.erase(r.yTrail.begin());
		r.xTrail.push_back(r.xPos);
		r.yTrail.push_back(r.yPos);
	}else{
		r.xTrail.push_back(r.xPos);
		r.yTrail.push_back(r.yPos);
	}
}

void resetTerminal(char outMap[41][132])
{
	for(i = 0; i < 41; i ++)
	{
		for(j = 0; j < 132; j++)
		{
			if(i == 0 || i == 40)
			{
				outMap[i][j] = '-'; 
				
			}else if((j == 0 || j == 131) && i != 0 && i != 40)
			{
				outMap[i][j] = '|'; 
				
			}else
			{
				outMap[i][j] = ' ';
			}
		}

	}
}


void setTerminal(char outMap[41][132], vector<racer> racers)
{
	int k,l,colored;
	for(i = 0; i < 41; i ++)
	{
		for(j = 0; j < 132; j++)
		{
			colored = 0;
			for(k = 0; k < (signed) racers.size(); k++)
			{
				if(racers[k].yPos == i && racers[k].xPos == j && racers[k].isDead == 0)
				{
					if(racers[k].color == COLOR_BLUE)
					{
							init_pair(1, COLOR_BLUE , COLOR_BLACK);		
							attron(COLOR_PAIR(1));
							mvaddch(i,j,outMap[i][j]);
							attroff(COLOR_PAIR(1));
							colored = 1;
					}
					if(racers[k].color == COLOR_CYAN)
					{
							init_pair(2, COLOR_CYAN , COLOR_BLACK);		
							attron(COLOR_PAIR(2));
							mvaddch(i,j,outMap[i][j]);
							attroff(COLOR_PAIR(2));
							colored = 1;
					}
					if(racers[k].color == COLOR_YELLOW)
					{
							init_pair(3, COLOR_YELLOW , COLOR_BLACK);		
							attron(COLOR_PAIR(3));
							mvaddch(i,j,outMap[i][j]);
							attroff(COLOR_PAIR(3));
							colored = 1;
					}
					if(racers[k].color == COLOR_GREEN)
					{
							init_pair(4, COLOR_GREEN , COLOR_BLACK);		
							attron(COLOR_PAIR(4));
							mvaddch(i,j,outMap[i][j]);
							attroff(COLOR_PAIR(4));
							colored = 1;
					}
				}else{
					for(l = 0; l < (signed) racers[k].xTrail.size(); l++)
					{
					if(racers[k].xTrail[l] == j && racers[k].yTrail[l] == i && racers[k].isDead == 0)
					{
						if(racers[k].color == COLOR_BLUE)
					{
							init_pair(1, COLOR_BLUE , COLOR_BLACK);		
							attron(COLOR_PAIR(1));
							mvaddch(i,j,outMap[i][j]);
							attroff(COLOR_PAIR(1));
							colored = 1;
					}
					if(racers[k].color == COLOR_CYAN)
					{
							init_pair(2, COLOR_CYAN , COLOR_BLACK);		
							attron(COLOR_PAIR(2));
							mvaddch(i,j,outMap[i][j]);
							attroff(COLOR_PAIR(2));
							colored = 1;
					}
					if(racers[k].color == COLOR_YELLOW)
					{
							init_pair(3, COLOR_YELLOW , COLOR_BLACK);		
							attron(COLOR_PAIR(3));
							mvaddch(i,j,outMap[i][j]);
							attroff(COLOR_PAIR(3));
							colored = 1;
					}
					if(racers[k].color == COLOR_GREEN)
					{
							init_pair(4, COLOR_GREEN , COLOR_BLACK);		
							attron(COLOR_PAIR(4));
							mvaddch(i,j,outMap[i][j]);
							attroff(COLOR_PAIR(4));
							colored = 1;
					}
						}
					}
				}
			}
			
			if(colored == 0)
			{
				if(i == 0 || i == 40)
				{
					if(j % 2 == 0)
					{
						init_pair(5, COLOR_CYAN , COLOR_BLACK);		
						attron(COLOR_PAIR(5));
						mvaddch(i,j,outMap[i][j]);
						attroff(COLOR_PAIR(5));	
					}else{
						init_pair(6, COLOR_MAGENTA , COLOR_BLACK);		
						attron(COLOR_PAIR(6));
						mvaddch(i,j,outMap[i][j]);
						attroff(COLOR_PAIR(6));	
					}
					
				}else if((j == 0 || j == 131) && i != 0 && i != 40)
				{
					if(i % 2 == 0)
					{
						init_pair(5, COLOR_CYAN , COLOR_BLACK);		
						attron(COLOR_PAIR(5));
						mvaddch(i,j,outMap[i][j]);
						attroff(COLOR_PAIR(5));	
					}else{
						init_pair(6, COLOR_MAGENTA , COLOR_BLACK);		
						attron(COLOR_PAIR(6));
						mvaddch(i,j,outMap[i][j]);
						attroff(COLOR_PAIR(6));	
					}
					
				}else
				{
					mvaddch(i,j,outMap[i][j]);
				}
						
			}
		}

	}
	
}
