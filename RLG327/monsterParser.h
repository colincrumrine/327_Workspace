#ifndef MONSTERPARSER_H
# define MONSTERPARSER_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<string.h>  
#include <stdint.h>
#include <endian.h>
#include <vector> 
#include <ncurses.h>
#include "game.h"

using namespace std;

class dice{
	
	public:
	int base, dice, sides;
	
	int rollDice()
	{
		int i = 0, rolledVal = base;
		while(i <= (dice - 1))																				
		{
			rolledVal += rand() % sides + 1;
			i++;
		}
		return rolledVal;
	}
	
};

int isValidMon();

void printDesc();

int checkString(string s);

void resetTraits();

void parseMonster();

void generateMonsters(int monsterCount, vector<class character> &monsters);

# endif
