#ifndef SWITCHES_H
# define SWITCHES_H

void switches(char *argv[],char dungeon[21][80], uint8_t hardness[21][80], int roomStats[24], uint8_t pcLocation[2]);
void save(char dungeon[21][80], uint8_t hardness[21][80], int roomStats[24], uint8_t pcLocation[2]);
void load(int summon, int monsterCount);
void save_load(int summon, int monsterCount);
void printArray(char dungeon[21][80]);

# endif