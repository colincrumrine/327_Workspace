#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<string.h>  
#include <stdint.h>
#include <endian.h>
#include "monsterMap.h"
#include "switches.h"
#include "generateDungeon.h"
#include "game.h"

void generateDungeon(char *argv[], int initializeGame, int monsterCount, object* carry[10], object* equipment[12], int pcHP)
{
	char dungeon[21][80];
	int x,y;
	for(x = 0; x < 21;x++)
	{
		for(y = 0; y < 80;y++)	
		{
			if(x == 0 || x == 20)
			{
				dungeon[x][y] = '-'; 
				
			}else if((y == 0 || y == 79) && x != 0 && x != 20)
			{
				dungeon[x][y] = '|'; 
				
			}else
			{
				dungeon[x][y] = '0';
			}
			
		}
	}
		int roomCount = 0;
		int positionsList[12];
		int roomStats[24];
		int roomStatsPos = 0;
		int point = 0;
		uint8_t pcLocation[2];
		int pcPlaced = 0;
		
		//Begin adding rooms
		
		while(roomCount < 6)
		{
		int randRoomX = (rand() % 6) + 3;
		int randRoomY = (rand() % 5) + 2;
		int randStartI = (rand() % 11) + 2;
		int randStartJ = (rand() % 68) + 2;
		int maxI = randStartI + randRoomY;
		int maxJ = randStartJ + randRoomX;
		int roomUnits = 1;
		int flag = 0;
		int i,j;
			for(i = randStartI; i < (maxI); i++)
			{
				for(j = randStartJ; j < (maxJ); j++)
				{
					if(dungeon[i][j] != '.' && dungeon[i][j+2] != '.' && dungeon[i][j-2] != '.' && dungeon[i+2][j] != '.' && dungeon[i-2][j] != '.' 
					&& dungeon[i-2][j-2] != '.' && dungeon[i-2][j+2] != '.' && dungeon[i+2][j-2] != '.' && dungeon[i+2][j+2] != '.')
					{
					roomUnits++;	
					}else{
						flag = 1;
						break;
					}
				}
				if(flag == 1){break;}
			}
			
			if(roomUnits == ((randRoomX * randRoomY) + 1))
						{
							roomStats[roomStatsPos] = randStartJ;
							roomStatsPos++;
							roomStats[roomStatsPos] = randStartI;
							roomStatsPos++;
							roomStats[roomStatsPos] = randRoomX + 1;
							roomStatsPos++;
							roomStats[roomStatsPos] = randRoomY + 1;
							roomStatsPos++;
							
							for(i = randStartI; i <= (maxI); i++)
							{
								for(j = randStartJ; j <= (maxJ); j++)
								{
										dungeon[i][j] = '.';
								}
							}
							if(pcPlaced == 0)
							{
								dungeon[randStartI][randStartJ] = '@';
								pcLocation[0] = randStartJ;
								pcLocation[1] = randStartI;
								pcPlaced++;
							}
							roomCount++;
							positionsList[point] = randStartI;
							point++;
							positionsList[point] = randStartJ;
							point++;
						}
						
		}
		
		int corridorCount = 0;
		int gridPoint = 0;
		
		while(corridorCount != 5)
		{
		
		int startCorridorY = positionsList[gridPoint];
		gridPoint++;
		int startCorridorX = positionsList[gridPoint];
		gridPoint++;
		int endCorridorY = positionsList[gridPoint];
		gridPoint++;
		int endCorridorX = positionsList[gridPoint];
		
		int yWidth = 0;
		int xWidth = 0;
		
		while(startCorridorY != endCorridorY)
		{
			if(dungeon[startCorridorY][startCorridorX] != '.' && dungeon[startCorridorY][startCorridorX] != '@')
			{
				dungeon[startCorridorY][startCorridorX] = '#';
			}
			
			if(dungeon[startCorridorY+1][startCorridorX] != '.' && yWidth < 3 && startCorridorY+1 < 20 && dungeon[startCorridorY+1][startCorridorX] != '@')
			{
				dungeon[startCorridorY+1][startCorridorX] = '#';
				yWidth++;
			}else if(dungeon[startCorridorY-1][startCorridorX] != '.' && yWidth < 3 && startCorridorY-1 > 1 && dungeon[startCorridorY-1][startCorridorX] != '@')
			{
				dungeon[startCorridorY-1][startCorridorX] = '#';
				yWidth++;
			}else if(yWidth == 3)
			{
				yWidth = 0;
			}
			
			if(startCorridorY - endCorridorY > 0)
			{
				startCorridorY--;
			}else{
				startCorridorY++;
			}
			
		}
		
		while(startCorridorX != endCorridorX)
		{
			if(dungeon[startCorridorY][startCorridorX] != '.' && dungeon[startCorridorY][startCorridorX] != '@')
			{
				dungeon[startCorridorY][startCorridorX] = '#';
			}
				
			if(dungeon[startCorridorY][startCorridorX+1] != '.' && xWidth < 3 && startCorridorX+1 < 79 && dungeon[startCorridorY][startCorridorX+1] != '@')
			{
				dungeon[startCorridorY][startCorridorX+1] = '#';
				xWidth++;
			}else if(dungeon[startCorridorY][startCorridorX-1] != '.' && xWidth < 3 && startCorridorX-1 > 1 && dungeon[startCorridorY][startCorridorX-1] != '@')
			{
				dungeon[startCorridorY][startCorridorX-1] = '#';
				xWidth++;
			}else if(xWidth == 3)
			{
				xWidth = 0;
			}
				
			if(startCorridorX - endCorridorX > 0)
			{
				startCorridorX--;
			}else{
				startCorridorX++;
			}
		}
		corridorCount++;
		gridPoint--;
		} 
	

	addSpaces(dungeon);

	uint8_t hardness[21][80];
	
	int row, col;
	for(row = 0; row < 21;row++)
	{
		for(col = 0; col < 80;col++)	
		{
			if(dungeon[row][col] == '|' || dungeon[row][col] == '-')
			{
				hardness[row][col] = 255;
			}else if(dungeon[row][col] == '.' || dungeon[row][col] == '#' || dungeon[row][col] == '@'){
				hardness[row][col] = 0;
			}else if(dungeon[row][col] == ' '){
				hardness[row][col] = (rand() % 253) + 1;
			}
		}
	}
	
	if(initializeGame == 0 && monsterCount == 0)
	{
		switches(argv,dungeon,hardness,roomStats,pcLocation);
	}else{
		simulate(monsterCount,hardness,roomStats,pcLocation,carry,equipment,pcHP);
	}

	
}

void addSpaces(char dungeon[21][80])
{
	int i,j;
	for(i = 0; i < 21;i++)
	{
		for(j = 0; j < 80;j++)	
		{
			if(dungeon[i][j] != '|' && dungeon[i][j] != '-' && dungeon[i][j] != '.' && dungeon[i][j] != '#' && dungeon[i][j] != '@')
			{
				dungeon[i][j] = ' ';
			}
		}
	}
}
