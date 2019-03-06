#ifndef GAME_H
# define GAME_H

#include <vector> 

using namespace std;

class racer{
	
	public:
	int xPos;
	int yPos;
	int color;
	int isDead;
	int isPlayer;
	vector<int> xTrail;
	vector<int> yTrail;
	
};

void play();

# endif