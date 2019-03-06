#ifndef GAME_H
# define GAME_H

#include "heap.h"
#include "monsterParser.h"

class object{
public:
  int xPos;
  int yPos;
  int isPickedUp;
  int isDestroyed;
  string name;
  string desc;
  int color;
  int hit;
  dice dam;
  int dodge;
  int def;
  int weight;
  int speed;
  int attr;
  int val;
  int art;
  char symb;
  int rarity;
};

class objectDescription{
public:
  string name;
  string desc;
  string type;
  string color;
  dice hit;
  dice dam;
  dice dodge;
  dice def;
  dice weight;
  dice speed;
  dice attr;
  dice val;
  string art;
  int rarity;
};

class character{
public:
  heap_node_t *hn;
  object* carry[10];
  object* equipment[12];
  int isDead;
  int isPC;
  int priority;
  int speed;
  int nextTurn;
  int xPos;
  int yPos;
  string name;
  string desc;
  int color;
  int intelligence;
  int telepathy;
  int tunneling;
  int erratic;
  int pass;
  int pickup;
  int destroy;
  int uniq;
  int boss;
  int hp;
  dice dam;
  char symb;
  int rarity;
  
};

class characterDescription{
public:
  dice speed;
  string name;
  string desc;
  string color;
  string abilities;
  dice hp;
  dice dam;
  char symb;
  int rarity;
};


void simulate(int monsterCount, uint8_t hardness[21][80], int roomStats[24], uint8_t pcLocation[2], object* carry[10], object* equipment[12], int pcHP);

# endif
