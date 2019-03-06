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
#include "gameUtil.h"
#include "game.h"

using namespace std;

void play()
{
	
int i,j;
	
timeout(0);

char outMap[41][132];
	
vector <racer> racers;

makeRacers(racers);

resetTerminal(outMap);
for(i = 0; i < (signed) racers.size(); i++)
{
	outMap[racers[i].yPos][racers[i].xPos] = '8';
}
setTerminal(outMap,racers);
refresh();

int game = 0;

srand (time(NULL));

int turns = 0, flynnTurns = 0;

while(game == 0)
{

resetTerminal(outMap);
int userInput = 0;

while(userInput != 27 && userInput != KEY_UP && userInput != KEY_DOWN && userInput != KEY_LEFT && userInput != KEY_RIGHT && userInput != ERR)
{
	userInput = getch();
}

turns++;

game = checkUserInput(userInput,racers[0],turns);

checkPCTrail(racers[0],turns);

for(i = 1; i < (signed) racers.size(); i++)
{
	for(j = 0; j < (signed) racers[i].xTrail.size(); j++)
	{
	
		if(((racers[0].xPos == racers[i].xTrail[j] && racers[0].yPos == racers[i].yTrail[j]) || (racers[0].xPos == racers[i].xPos && racers[0].yPos == racers[i].yPos)) && racers[i].isDead == 0)
		{
			game = 1;
		}
	
	}
	
}

racerMoves(racers,flynnTurns);

if(racers[0].isDead == 1)
{
	game = 1;
}
	

for(i = 0; i < (signed) racers.size(); i++)
{
	for(j = 0; j < (signed) racers[i].xTrail.size(); j++)
	{
		if(racers[i].isDead == 0)
		{
			outMap[racers[i].yTrail[j]][racers[i].xTrail[j]] = '#';
		}
	
	}
	if(racers[i].isDead == 0)
	{
	outMap[racers[i].yPos][racers[i].xPos] = '8';
	}
	
}

if(racers[1].isDead == 1 && racers[2].isDead == 1 && racers[3].isDead == 1)
{
	game = 2;
}

setTerminal(outMap,racers);
refresh();

usleep(100000);

}

if(game == 1)
{
	clear();
	refresh();
	
	ifstream file( "GAME_OVER.txt" );
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	
	init_pair(2, COLOR_RED , COLOR_BLACK);		
	attron(COLOR_PAIR(2));
	printw("%s", buffer.str().c_str());
	attroff(COLOR_PAIR(2));
	
	int gameOverDecision = 0;
	
	while(gameOverDecision != 10 && gameOverDecision != 'M')
	{
		gameOverDecision = getch();
	}
	switch(gameOverDecision)
	{
		case 10:
		
		clear();
		refresh();
		
		play();
		
		break;
		
		case 'M':
		
		clear();
		refresh();
		
		break;
	}
	
	
}else if(game == 2)
{
	clear();
	refresh();
	
	ifstream file( "VICTORY_SCREEN.txt" );
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	
	init_pair(2, COLOR_CYAN , COLOR_BLACK);		
	attron(COLOR_PAIR(2));
	printw("%s", buffer.str().c_str());
	attroff(COLOR_PAIR(2));
	
	int gameOverDecision = 0;
	
	while(gameOverDecision != 10 && gameOverDecision != 'M')
	{
		gameOverDecision = getch();
	}
	switch(gameOverDecision)
	{
		case 10:
		
		clear();
		refresh();
		
		play();
		
		break;
		
		case 'M':
		
		clear();
		refresh();
		
		break;
	}
}
	
	
}