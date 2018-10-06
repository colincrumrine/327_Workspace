#ifndef LOADSAVE_H
# define LOADSAVE_H

void load_save(char *argv[],char dungeon[21][80], uint8_t hardness[21][80], int roomStats[24], uint8_t pcLocation[2]);
void printArray(char dungeon[21][80]);

# endif