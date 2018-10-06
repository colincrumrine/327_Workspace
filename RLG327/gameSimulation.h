#ifndef GAMESIMULATION_H
# define GAMESIMULATION_H

#include "heap.h"

struct character{
  heap_node_t *hn;
  int isPC;
  int isDefeated;
  int priority;
  int speed;
  int nextTurn;
  int xPos;
  int yPos;
  int intelligence;
  int telepathy;
  int tunneling;
  int erratic;
  char sign;
};

int speed_cmp(const void *key, const void *with);
 
void randMove(struct character *character, uint8_t hardness[21][80]);

void setOutDungeon(char outDungeon[21][80], uint8_t modHardness[21][80], int roomStats[24]);

void simulate(int monsterCount, uint8_t hardness[21][80], int roomStats[24], uint8_t pcLocation[2]);

# endif