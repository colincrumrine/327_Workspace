#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

#include <stdint.h>
#include <stdlib.h>
#include<time.h>
#include "gameSimulation.h"
#include "monsterMap.h"
#include "heap.h"
#include "switches.h"
#include "generateDungeon.h"


char outDungeon[21][80];

const int dungeonI = 21;
const int dungeonJ = 80;

int speed_cmp(const void *key, const void *with) {
	if(((struct character *) key)->nextTurn - ((struct character *) with)->nextTurn == 0)
	{
		return ((struct character *) key)->priority - ((struct character *) with)->priority;
	}
	
   return ((struct character *) key)->nextTurn - ((struct character *) with)->nextTurn;
 }
 
void randMove(struct character *character, uint8_t hardness[21][80])
{
	  int characterMove = (rand() % 7);
	  
	  if(characterMove == 0 && hardness[character->yPos - 1][character->xPos - 1] == 0)
	  {
		  character->xPos = character->xPos - 1;
		  character->yPos = character->yPos - 1;
		  
	  }else if(characterMove == 1 && hardness[character->yPos - 1][character->xPos] == 0)
	  {
		  character->yPos = character->yPos - 1;
		  
	  }else if(characterMove == 2 && hardness[character->yPos][character->xPos - 1] == 0)
	  {
		  character->xPos = character->xPos - 1;
		  
	  }else if(characterMove == 3 && hardness[character->yPos + 1][character->xPos - 1] == 0)
	  {
		  character->xPos = character->xPos - 1;
		  character->yPos = character->yPos + 1;
		  
	  }else if(characterMove == 4 && hardness[character->yPos - 1][character->xPos + 1] == 0)
	  {
		  character->xPos = character->xPos + 1;
		  character->yPos = character->yPos - 1;
	  }else if(characterMove == 5 && hardness[character->yPos + 1][character->xPos + 1] == 0)
	  {
		  character->xPos = character->xPos + 1;
		  character->yPos = character->yPos + 1;
		  
	  }else if(characterMove == 6 && hardness[character->yPos][character->xPos + 1] == 0)
	  {
		  character->xPos = character->xPos + 1;
	  }else if(characterMove == 7 && hardness[character->yPos + 1][character->xPos] == 0)
	  {
		  character->yPos = character->yPos + 1;
	  }
}

void setOutDungeon(char outDungeon[21][80], uint8_t modHardness[21][80], int roomStats[24])
{
	int i,j;
	for(i = 0; i < dungeonI; i++)
			{
				for(j = 0; j < dungeonJ; j++)
				{
					if(i == 0 || i == 20)
					{
						outDungeon[i][j] = '-';
					}else if((j == 0 || j == 79) && i != 0 && i != 20)
					{
						outDungeon[i][j] = '|';
					}else if(modHardness[i][j] == 0)
					{
						outDungeon[i][j] = '#';
					}else{
						outDungeon[i][j] = ' ';
					}
					
				}
				
			}
		 
			int roomPos = 0;
			while(roomPos < 24)
			{
				int startRoomX = roomStats[roomPos];
				roomPos++;
				int startRoomY = roomStats[roomPos];
				roomPos++;
				int maximumX = startRoomX + roomStats[roomPos];
				roomPos++;
				int maximumY = startRoomY + roomStats[roomPos];
				roomPos++;
		
					int c,d;
					for(c = startRoomY; c < (maximumY); c++)
					{
							for(d = startRoomX; d < (maximumX); d++)
							{
								outDungeon[c][d] = '.';
							}
					}	
			}
}

void simulate(int monsterCount, uint8_t hardness[21][80], int roomStats[24], uint8_t pcLocation[2])
{
	
  srand(time(0));
  
  uint8_t modHardness[21][80];
  struct character *nextCharacterMove;
  int ioCostArr[21][80];
  uint8_t newPCLoc[2];
  int cost = INT_MAX;
  int nextX;
  int nextY;
  int initialX;
  int initialY;
  int done = 0;
  int charArrSize;

  int i,j;
  for(i = 0; i < 21; i++)
  {
		for(j = 0; j < 80; j++)
		{
			modHardness[i][j] = hardness[i][j];
		}
  }		
			
  //Initializing heap
  heap_t h;
  heap_init(&h, speed_cmp, NULL);
  
  //Initializing pc and mons in array and adding to heap
  
  if(monsterCount >= 50)
  {
	  monsterCount = 50;
  }
  
  if(monsterCount == 0)
  {
	charArrSize = 11;
	
  }else{
	  charArrSize = monsterCount + 1;
  }
	struct character *(charArr[charArrSize]);
	
	for(i = 0; i < charArrSize; i++)
	{
		charArr[i] = (struct character *)malloc(sizeof(struct character));
	}
	
	charArr[0]->isPC = 1;
	charArr[0]->priority = 0;
	charArr[0]->speed = 10;
	charArr[0]->nextTurn = 0;
	charArr[0]->xPos = pcLocation[0];
	charArr[0]->yPos = pcLocation[1];
	charArr[0]->intelligence = 0;
	charArr[0]->telepathy = 0;
	charArr[0]->tunneling = 0;
	charArr[0]->erratic = 0;
	charArr[0]->sign = '@';
	charArr[0]->hn = heap_insert(&h, &(*charArr[0]));

	int count = 1;
	while(count < charArrSize)
	{
	int randSpeed = (rand() % 15) + 5;
	int randIntelligence = (rand() % 2);
	int randTelepathy = (rand() % 2);
	int randTunneling = (rand() % 2);
	int randErratic = (rand() % 2);
	
	charArr[count]->speed = randSpeed;
	charArr[count]->nextTurn = 0;
	charArr[count]->intelligence = randIntelligence;
	charArr[count]->telepathy = randTelepathy;
	charArr[count]->tunneling = randTunneling;
	charArr[count]->erratic = randErratic;
	
	charArr[count]->isPC = 0;
	
	if(charArr[count]->tunneling == 0)
	{
		int placed = 0;
		while(placed != 1)
		{
			
		int randY = (rand() % 11) + 2;
		int randX = (rand() % 68) + 2;
		
		if(hardness[randY][randX] == 0)
		{
			charArr[count]->xPos = randX;
			charArr[count]->yPos = randY;
			placed = 1;
		}
		
		}
		
	}else{
		int randY = (rand() % 11) + 2;
		int randX = (rand() % 68) + 2;
		charArr[count]->xPos = randX;
		charArr[count]->yPos = randY;
	}
	
	int signDeterminant = (charArr[count]->intelligence * 1) + (charArr[count]->telepathy * 2) + (charArr[count]->tunneling * 4) + (charArr[count]->erratic * 8);
	
	char hex;
	
	if (signDeterminant < 10) {
		hex = signDeterminant + '0';
	} else {
		hex = signDeterminant + 'A' - 10;
	}
	
	charArr[count]->sign = hex;
	
	charArr[count]->priority = signDeterminant;
	
	outDungeon[charArr[count]->yPos][charArr[count]->xPos] = charArr[count]->sign;

	charArr[count]->hn = heap_insert(&h, &(*charArr[count]));
	
	count++;
	}
	
	//Initializing npcMap to keep track of npc positions.
	struct character *npcMap[21][80];
	
	memset(npcMap,NULL, sizeof(struct character *) * 80 * 21);
	
	for(i = 0; i < charArrSize; i++)
	{
		npcMap[charArr[i]->yPos][charArr[i]->xPos] = charArr[i];
	}
	
	while(1)
	{ 		

	  setOutDungeon(outDungeon,modHardness,roomStats);
	  
	  nextCharacterMove = heap_remove_min(&h);
	  
	  initialX = nextCharacterMove->xPos;
	  initialY = nextCharacterMove->yPos;
	  
	
    int isErratic = 0;
	if(nextCharacterMove->isPC == 1)
	  {  
		randMove(nextCharacterMove,modHardness);
		
	  }else if(nextCharacterMove->erratic == 1){
		  isErratic = (rand() % 2);
		  
		  if(isErratic == 1)
		  {	
			  if(nextCharacterMove->tunneling == 1)
			  {
				int tunnelX = (rand() % 3) - 1;
				int tunnelY = (rand() % 3) - 1;
				
				if(nextCharacterMove->yPos + tunnelY < 20 && nextCharacterMove->yPos + tunnelY > 1 && nextCharacterMove->xPos + tunnelX < 79 && nextCharacterMove->xPos + tunnelX > 1)
				{
					modHardness[nextCharacterMove->yPos + tunnelY][nextCharacterMove->xPos + tunnelX] = 0;
					nextCharacterMove->xPos = nextCharacterMove->xPos + tunnelX;
					nextCharacterMove->yPos = nextCharacterMove->yPos + tunnelY;
		
				}
					
			  }else{
				  randMove(nextCharacterMove,modHardness);
			  }
		  
		  
			}
			}
			if(isErratic != 1)
			{
				int moveDeterminant = (nextCharacterMove->intelligence * 1) + (nextCharacterMove->telepathy * 2) + (nextCharacterMove->tunneling * 4);
				
				switch(moveDeterminant)
				{
					case 0:
		
					if(abs(charArr[0]->xPos - nextCharacterMove->xPos) <= 5 && abs(charArr[0]->yPos - nextCharacterMove->yPos) <= 5)
					{
						
						if(charArr[0]->xPos - nextCharacterMove->xPos < 0)
						{
							
							if(nextCharacterMove->xPos - 1 > 1 && modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos - 1] == 0)
							{
								nextCharacterMove->xPos = nextCharacterMove->xPos - 1;
							}
							
						}else if(charArr[0]->xPos - nextCharacterMove->xPos > 0)
						{
							
							if(nextCharacterMove->xPos + 1 < 79  && modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos + 1] == 0)
							{
								nextCharacterMove->xPos = nextCharacterMove->xPos + 1;
							}
			
						}
						
						if(charArr[0]->yPos - nextCharacterMove->yPos < 0)
						{
							
							if(nextCharacterMove->yPos - 1 > 1  && modHardness[nextCharacterMove->yPos - 1][nextCharacterMove->xPos] == 0)
							{
								nextCharacterMove->yPos = nextCharacterMove->yPos - 1;
							}
						
						}else if(charArr[0]->yPos - nextCharacterMove->yPos > 0 && modHardness[nextCharacterMove->yPos + 1][nextCharacterMove->xPos] == 0)
						{
								
							if(nextCharacterMove->yPos + 1 < 79)
							{
								nextCharacterMove->yPos = nextCharacterMove->yPos + 1;
							}
						
						}
						
					}
					
					break;
					
					case 1:
					
					cost = INT_MAX;
					newPCLoc[0] = charArr[0]->xPos;
					newPCLoc[1] = charArr[0]->yPos;
					nonTunnelingMap(ioCostArr,modHardness,newPCLoc);

						if(ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos - 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos - 1];
							nextY =  nextCharacterMove->yPos - 1;
							nextX =  nextCharacterMove->xPos - 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos + 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos + 1];
							nextY = nextCharacterMove->yPos - 1;
							nextX =  nextCharacterMove->xPos + 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos - 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos - 1];
							nextY = nextCharacterMove->yPos + 1;
							nextX =  nextCharacterMove->xPos - 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos + 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos + 1];
							nextY =  nextCharacterMove->yPos + 1;
							nextX =  nextCharacterMove->xPos + 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos][nextCharacterMove->xPos + 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos][nextCharacterMove->xPos + 1];
							nextY =  nextCharacterMove->yPos;
							nextX =  nextCharacterMove->xPos + 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos][nextCharacterMove->xPos - 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos][nextCharacterMove->xPos - 1];
							nextY =  nextCharacterMove->yPos;
							nextX =  nextCharacterMove->xPos - 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos];
							nextY =  nextCharacterMove->yPos + 1;
							nextX =  nextCharacterMove->xPos;
						}
						
						if(ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos];
							nextY =  nextCharacterMove->yPos - 1;
							nextX =  nextCharacterMove->xPos;
						}
						
						nextCharacterMove->yPos = nextY;
						nextCharacterMove->xPos = nextX;
					
					break;
					
					case 2:
					
						if(charArr[0]->xPos - nextCharacterMove->xPos < 0)
						{
							
							if(nextCharacterMove->xPos - 1 > 1 && modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos - 1] == 0)
							{
								nextCharacterMove->xPos = nextCharacterMove->xPos - 1;
							}
							
						}else if(charArr[0]->xPos - nextCharacterMove->xPos > 0)
						{
							
							if(nextCharacterMove->xPos + 1 < 79  && modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos + 1] == 0)
							{
								nextCharacterMove->xPos = nextCharacterMove->xPos + 1;
							}
			
						}
						
						if(charArr[0]->yPos - nextCharacterMove->yPos < 0)
						{
							
							if(nextCharacterMove->yPos - 1 > 1  && modHardness[nextCharacterMove->yPos - 1][nextCharacterMove->xPos] == 0)
							{
								nextCharacterMove->yPos = nextCharacterMove->yPos - 1;
							}
						
						}else if(charArr[0]->yPos - nextCharacterMove->yPos > 0 && modHardness[nextCharacterMove->yPos + 1][nextCharacterMove->xPos] == 0)
						{
								
							if(nextCharacterMove->yPos + 1 < 79)
							{
								nextCharacterMove->yPos = nextCharacterMove->yPos + 1;
							}
						
						}
					
					break;
					
					case 3:
					
					cost = INT_MAX;
					newPCLoc[0] = charArr[0]->xPos;
					newPCLoc[1] = charArr[0]->yPos;
					nonTunnelingMap(ioCostArr,modHardness,newPCLoc);
					

						if(ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos - 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos - 1];
							nextY =  nextCharacterMove->yPos - 1;
							nextX =  nextCharacterMove->xPos - 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos + 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos + 1];
							nextY = nextCharacterMove->yPos - 1;
							nextX =  nextCharacterMove->xPos + 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos - 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos - 1];
							nextY = nextCharacterMove->yPos + 1;
							nextX =  nextCharacterMove->xPos - 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos + 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos + 1];
							nextY =  nextCharacterMove->yPos + 1;
							nextX =  nextCharacterMove->xPos + 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos][nextCharacterMove->xPos + 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos][nextCharacterMove->xPos + 1];
							nextY =  nextCharacterMove->yPos;
							nextX =  nextCharacterMove->xPos + 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos][nextCharacterMove->xPos - 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos][nextCharacterMove->xPos - 1];
							nextY =  nextCharacterMove->yPos;
							nextX =  nextCharacterMove->xPos - 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos];
							nextY =  nextCharacterMove->yPos + 1;
							nextX =  nextCharacterMove->xPos;
						}
						
						if(ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos];
							nextY =  nextCharacterMove->yPos - 1;
							nextX =  nextCharacterMove->xPos;
						}
						
						if(modHardness[nextY][nextX] == 0)
						{
						nextCharacterMove->yPos = nextY;
						nextCharacterMove->xPos = nextX;
						}
						
						
					
					break;
					
					case 4:
					
					if(abs(charArr[0]->xPos - nextCharacterMove->xPos) <= 5 && abs(charArr[0]->yPos - nextCharacterMove->yPos) <= 5)
					{
						
						if(charArr[0]->xPos - nextCharacterMove->xPos < 0)
						{
							
							if(nextCharacterMove->xPos - 1 > 1 && modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos - 1] == 0)
							{
								nextCharacterMove->xPos = nextCharacterMove->xPos - 1;
							}else if(nextCharacterMove->xPos - 1 > 1 && modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos - 1] != 0)
							{
								modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos - 1] = 0;
								nextCharacterMove->xPos = nextCharacterMove->xPos - 1;
							}
							
						}else if(charArr[0]->xPos - nextCharacterMove->xPos > 0)
						{
							
							if(nextCharacterMove->xPos + 1 < 79  && modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos + 1] == 0)
							{
								nextCharacterMove->xPos = nextCharacterMove->xPos + 1;
							}else if(nextCharacterMove->xPos + 1 < 79  && modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos + 1] != 0)
							{
								modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos + 1] = 0;
								nextCharacterMove->xPos = nextCharacterMove->xPos + 1;
							}
			
						}
						
						if(charArr[0]->yPos - nextCharacterMove->yPos < 0)
						{
							
							if(nextCharacterMove->yPos - 1 > 1  && modHardness[nextCharacterMove->yPos - 1][nextCharacterMove->xPos] == 0)
							{
								nextCharacterMove->yPos = nextCharacterMove->yPos - 1;
							}else if(nextCharacterMove->yPos - 1 > 1  && modHardness[nextCharacterMove->yPos - 1][nextCharacterMove->xPos] != 0)
							{
								modHardness[nextCharacterMove->yPos - 1][nextCharacterMove->xPos] = 0;
								nextCharacterMove->yPos = nextCharacterMove->yPos - 1;
							}
						
						}else if(charArr[0]->yPos - nextCharacterMove->yPos > 0)
						{
								
							if(nextCharacterMove->yPos + 1 < 79  && modHardness[nextCharacterMove->yPos + 1][nextCharacterMove->xPos] == 0)
							{
								nextCharacterMove->yPos = nextCharacterMove->yPos + 1;
							}else if(nextCharacterMove->yPos + 1 < 79  && modHardness[nextCharacterMove->yPos + 1][nextCharacterMove->xPos] != 0)
							{
								modHardness[nextCharacterMove->yPos + 1][nextCharacterMove->xPos] = 0;
								nextCharacterMove->yPos = nextCharacterMove->yPos + 1;
							}
					
						}
						
					}
					
					break;
					
					case 5:
					
					cost = INT_MAX;
					newPCLoc[0] = charArr[0]->xPos;
					newPCLoc[1] = charArr[0]->yPos;
					tunnelingMap(ioCostArr,modHardness,newPCLoc);
					

						if(ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos - 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos - 1];
							nextY =  nextCharacterMove->yPos - 1;
							nextX =  nextCharacterMove->xPos - 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos + 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos + 1];
							nextY = nextCharacterMove->yPos - 1;
							nextX =  nextCharacterMove->xPos + 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos - 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos - 1];
							nextY = nextCharacterMove->yPos + 1;
							nextX =  nextCharacterMove->xPos - 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos + 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos + 1];
							nextY =  nextCharacterMove->yPos + 1;
							nextX =  nextCharacterMove->xPos + 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos][nextCharacterMove->xPos + 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos][nextCharacterMove->xPos + 1];
							nextY =  nextCharacterMove->yPos;
							nextX =  nextCharacterMove->xPos + 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos][nextCharacterMove->xPos - 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos][nextCharacterMove->xPos - 1];
							nextY =  nextCharacterMove->yPos;
							nextX =  nextCharacterMove->xPos - 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos];
							nextY =  nextCharacterMove->yPos + 1;
							nextX =  nextCharacterMove->xPos;
						}
						
						if(ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos];
							nextY =  nextCharacterMove->yPos - 1;
							nextX =  nextCharacterMove->xPos;
						}
						
						nextCharacterMove->yPos = nextY;
						nextCharacterMove->xPos = nextX;
						modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos] = 0;
					
					break;
					
					case 6:
					
					if(charArr[0]->xPos - nextCharacterMove->xPos < 0)
						{
							
							if(nextCharacterMove->xPos - 1 > 1)
							{
								modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos - 1] = 0;
								nextCharacterMove->xPos = nextCharacterMove->xPos - 1;
							}
							
						}else if(charArr[0]->xPos - nextCharacterMove->xPos > 0)
						{
							
							if(nextCharacterMove->xPos + 1 < 79)
							{
								modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos + 1] = 0;
								nextCharacterMove->xPos = nextCharacterMove->xPos + 1;
							}
			
						}
						
						if(charArr[0]->yPos - nextCharacterMove->yPos < 0)
						{
							
							if(nextCharacterMove->yPos - 1 > 1)
							{
								modHardness[nextCharacterMove->yPos - 1][nextCharacterMove->xPos] = 0;
								nextCharacterMove->yPos = nextCharacterMove->yPos - 1;
							}
						
						}else if(charArr[0]->yPos - nextCharacterMove->yPos > 0)
						{
								
							if(nextCharacterMove->yPos + 1 < 79)
							{
								modHardness[nextCharacterMove->yPos + 1][nextCharacterMove->xPos] = 0;
								nextCharacterMove->yPos = nextCharacterMove->yPos + 1;
							}
						
						}
					
					break;
					
					case 7:
					
					cost = INT_MAX;
					newPCLoc[0] = charArr[0]->xPos;
					newPCLoc[1] = charArr[0]->yPos;
					tunnelingMap(ioCostArr,modHardness,newPCLoc);
					

						if(ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos - 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos - 1];
							nextY =  nextCharacterMove->yPos - 1;
							nextX =  nextCharacterMove->xPos - 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos + 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos + 1];
							nextY = nextCharacterMove->yPos - 1;
							nextX =  nextCharacterMove->xPos + 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos - 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos - 1];
							nextY = nextCharacterMove->yPos + 1;
							nextX =  nextCharacterMove->xPos - 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos + 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos + 1];
							nextY =  nextCharacterMove->yPos + 1;
							nextX =  nextCharacterMove->xPos + 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos][nextCharacterMove->xPos + 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos][nextCharacterMove->xPos + 1];
							nextY =  nextCharacterMove->yPos;
							nextX =  nextCharacterMove->xPos + 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos][nextCharacterMove->xPos - 1] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos][nextCharacterMove->xPos - 1];
							nextY =  nextCharacterMove->yPos;
							nextX =  nextCharacterMove->xPos - 1;
						}
						
						if(ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos + 1][nextCharacterMove->xPos];
							nextY =  nextCharacterMove->yPos + 1;
							nextX =  nextCharacterMove->xPos;
						}
						
						if(ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos] < cost)
						{
							cost = ioCostArr[nextCharacterMove->yPos - 1][nextCharacterMove->xPos];
							nextY =  nextCharacterMove->yPos - 1;
							nextX =  nextCharacterMove->xPos;
						}
						
						nextCharacterMove->yPos = nextY;
						nextCharacterMove->xPos = nextX;
						modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos] = 0;
					
					break;
					
					default: break;
				}
			}
      
	  	nextCharacterMove->nextTurn = nextCharacterMove->nextTurn + (1000/nextCharacterMove->speed);
	  
	    if(nextCharacterMove->yPos == charArr[0]->yPos && nextCharacterMove->xPos == charArr[0]->xPos && nextCharacterMove->isPC == 0)
		{
			done = 1;
		}	
		
		// struct character *moveTo = npcMap[nextCharacterMove->yPos][nextCharacterMove->xPos];
		// if(moveTo)
		// {
			// nextCharacterMove->xPos = initialX;
			// nextCharacterMove->yPos = initialY;
		// }else{
			// npcMap[initialY][initialX] = NULL;
			// npcMap[nextCharacterMove->yPos][nextCharacterMove->xPos] = nextCharacterMove;
		// }
		
		struct character *moveTo = npcMap[nextCharacterMove->yPos][nextCharacterMove->xPos];
		if(moveTo && ((nextCharacterMove->yPos != initialY && nextCharacterMove->xPos == initialX) || (nextCharacterMove->yPos == initialY && nextCharacterMove->xPos != initialX) 
			|| (nextCharacterMove->yPos != initialY && nextCharacterMove->xPos != initialX)))
		{
			free(moveTo);
			npcMap[initialY][initialX] = NULL;
			npcMap[nextCharacterMove->yPos][nextCharacterMove->xPos] = nextCharacterMove;
		}else{
			npcMap[initialY][initialX] = NULL;
			npcMap[nextCharacterMove->yPos][nextCharacterMove->xPos] = nextCharacterMove;
		}
		
		
		
	  for(i = 0; i < charArrSize; i++)
	  {
		  if(charArr[i])
		  {
			outDungeon[charArr[i]->yPos][charArr[i]->xPos] = charArr[i]->sign;  
		  }
	  }
	
	  printArray(outDungeon);
  
	  if(done) break;
  
	  usleep(250000);
	  
	 nextCharacterMove->hn = heap_insert(&h, nextCharacterMove);
	
	}
   
    heap_delete(&h);
   
	int a;
	for(a = 0; a < charArrSize; a++)
	{
		free(charArr[a]);
	}
	
    int c;
    FILE *file;
    file = fopen("gameOver.txt", "r");
    if (file)
	{
		while ((c = getc(file)) != EOF)
		putchar(c);
    fclose(file);
    }
	
	// char *emptyString[] = {" "};
	// generateDungeon(emptyString,1,monsterCount);
	
	}