#ifndef GENERATEDUNGEON_H
# define GENERATEDUNGEON_H

#include "game.h"

void generateDungeon(char *argv[], int initializeGame, int monsterCount, object* carry[10], object* equipment[12], int pcHP);

void addSpaces(char dungeon[21][80]);

# endif
