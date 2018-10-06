#ifndef MONSTERMAP_H
# define MONSTERMAP_H

int tunnelingMap(int ioCostArr[21][80], uint8_t hardness[21][80], uint8_t pcLocation[2]);
int nonTunnelingMap(int ioCostArr[21][80], uint8_t hardness[21][80], uint8_t pcLocation[2]);
int node_cmp(const void *key, const void *with);

# endif