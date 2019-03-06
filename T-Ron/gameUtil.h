#ifndef GAMEUTIL_H
# define GAMEUTIL_H

#include <vector> 
#include "game.h"

using namespace std;

int isTrailThere(int xPos, int yPos, vector<racer> racers, racer r);

void cluAI(racer& r, racer pc, vector<racer> racers);

void flynnAI(racer& r, int& flynnTurns, vector<racer> racers);

void tronAI(racer& r, vector<racer> racers);

void checkPCTrail(racer& r, int turns);

void racerMoves(vector<racer>& racers, int& flynnTurns);

int checkUserInput(int userInput, racer& r, int& turns);

void makeRacers(vector<racer>& racers);

void addToTrail(racer& r);

void resetTerminal(char outMap[41][132]);

void setTerminal(char outMap[41][132], vector<racer> racers);

# endif