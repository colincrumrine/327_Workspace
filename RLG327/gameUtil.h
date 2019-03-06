#ifndef GAMEUTIL_H
# define GAMEUTIL_H

#include "game.h"
#include <vector>

int speed_cmp(const void *key, const void *with);

void randMove(character *character, uint8_t hardness[21][80]);

void setOutDungeon(char outDungeon[21][80], uint8_t modHardness[21][80], int roomStats[24]);

void io_init_terminal(void);

void setTerminal(char dungeon[21][80], vector<character> characters, vector<object> objects);

void printMonsterList(vector<character> characters);

# endif
