#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<string.h>  
#include <stdint.h>
#include <endian.h>
#include "switches.h"
#include "monsterMap.h"
#include "gameSimulation.h"

void switches(char *argv[],char dungeon[21][80], uint8_t hardness[21][80], int roomStats[24], uint8_t pcLocation[2])
{	 
	char saveArr[10];
	char loadArr[10];
	char nummonArr[10];
	strcpy(saveArr, "--save");
	strcpy(loadArr, "--load");
	strcpy(nummonArr, "--nummon");
	
	int i;
	int s = 0;
	int l = 0;
	int nummon = 0;
	int nummonCount;
	for(i = 1; i < 5; i++)
	{
		if(argv[i] != NULL)
		{
			if(strcmp(argv[i],nummonArr) == 0)
			{
				if(argv[i+1] != NULL && (strcmp(argv[i+1], loadArr) != 0) && (strcmp(argv[i+1], saveArr) != 0))
				{
					nummon = 2;
					nummonCount = atoi(argv[i+1]);
				}else{
					nummon = 1;
				}
			}else if(strcmp(argv[i],saveArr) == 0)
			{
				s = 1;
			}else if(strcmp(argv[i],loadArr) == 0)
			{
				l = 1;
			}
		}
	}
	
	
	if(l == 0 && s == 0 && nummon == 0)
	{
		printArray(dungeon);
	}else if(l == 1 && s == 0 && nummon == 0)
	{
		load(0,0);
	}else if(l == 1 && s == 1 && nummon == 0)
	{
		save_load(0,0);
	}else if(l == 1 && s == 1 && nummon == 1)
	{
		save_load(1,0);
	}else if(l == 1 && s == 1 && nummon == 2)
	{
		save_load(1,nummonCount);
	}else if(l == 1 && s == 0 && nummon == 1)
	{
		load(1,0);
	}else if(l == 1 && s == 0 && nummon == 2)
	{
		load(1,nummonCount);
	}else if(l == 0 && s == 1 && nummon == 1)
	{
		save(dungeon, hardness, roomStats, pcLocation);
		printf("Summoning mons\n");
		simulate(0,hardness,roomStats,pcLocation);
	}else if(l == 0 && s == 1 && nummon == 2)
	{
		save(dungeon, hardness, roomStats, pcLocation);
		simulate(0,hardness,roomStats,pcLocation);
	}else if(l == 0 && s == 0 && nummon == 1)
	{
		simulate(0,hardness,roomStats,pcLocation);
	}else if(l == 0 && s == 0 && nummon == 2)
	{
		simulate(nummonCount,hardness,roomStats,pcLocation);
	}else if(l == 0 && s == 1 && nummon == 0)
	{
		save(dungeon, hardness, roomStats, pcLocation);
	}
}

void save(char dungeon[21][80], uint8_t hardness[21][80], int roomStats[24], uint8_t pcLocation[2])
{
	 uint8_t savedHardness[21][80];
	 int a ,b;
	 for(a = 0; a < 21; a++)
	 {
		 for(b = 0; b < 80; b++)
		 {
			 savedHardness[a][b] = hardness[a][b];
		 }
	 }
	
	 uint8_t savedRooms[24];
	 int k;
	 for(k = 0; k < 24; k++)
	 {
		savedRooms[k] = roomStats[k];
	 }
			printArray(dungeon);
			
			FILE *fp;
			char *path;
			path = malloc(strlen(getenv("HOME")) + strlen("/.rlg327/dungeon") + 1);
			strcpy(path, getenv("HOME"));
			strcat(path,"/.rlg327/dungeon");
			fp = fopen(path, "w");
			free(path);			
			
			char fileMarker[12] = "RLG327-F2018";
			fwrite(fileMarker,sizeof(char),sizeof(fileMarker),fp);

            uint32_t versionNum = 0;
			versionNum = htobe32(versionNum);
			fwrite(&versionNum,sizeof(uint32_t),1,fp);	
			
			uint32_t fileSize = 1726;
			fileSize = htobe32(fileSize);
			fwrite(&fileSize,sizeof(uint32_t),1,fp);
	
			uint8_t pcX = pcLocation[0];
			fwrite(&pcX,sizeof(uint8_t),1,fp);
			
			uint8_t pcY = pcLocation[1];
			fwrite(&pcY,sizeof(uint8_t),1,fp);
			
			fwrite(savedHardness,sizeof(uint8_t),sizeof(savedHardness),fp);
			
			fwrite(savedRooms,sizeof(uint8_t),sizeof(savedRooms),fp);
}

void load(int summon, int monsterCount)
{
			FILE *fp;
			char *path;
			path = malloc(strlen(getenv("HOME")) + strlen("/.rlg327/dungeon") + 1);
			strcpy(path, getenv("HOME"));
			strcat(path,"/.rlg327/dungeon");
			fp = fopen(path, "r");
			free(path);		
		
			if (!(NULL == fp)) {
			
			char fileMarker[12];
			fread(fileMarker,sizeof(char),sizeof(fileMarker),fp);
			
			uint32_t versionNum;
			fread(&versionNum,sizeof(uint32_t),1,fp);
			versionNum = be32toh(versionNum);
			
			uint32_t fileSize;
			fread(&fileSize,sizeof(uint32_t),1,fp);
			fileSize = be32toh(fileSize);
			
			uint8_t pcX;
			fread(&pcX,sizeof(uint8_t),1,fp);

			uint8_t pcY;
			fread(&pcY,sizeof(uint8_t),1,fp);
			
			uint8_t readPCLoc[2];
			readPCLoc[0] = pcX;
			readPCLoc[1] = pcY;
			
			uint8_t readHardness[21][80];
			fread(readHardness,sizeof(uint8_t),sizeof(readHardness),fp);
			
			int roomSizes = fileSize - ftell(fp);
			uint8_t readRooms[roomSizes];
			fread(readRooms,sizeof(uint8_t),sizeof(readRooms),fp);
			
			int increment;
			int simulationRooms[roomSizes];
			for(increment = 0; increment < roomSizes; increment++)
			{
				simulationRooms[increment] = readRooms[increment];
			}
			
			char outDungeon[21][80];
			
			int i,j;
			for(i = 0; i < 21; i++)
			{
				for(j = 0; j < 80; j++)
				{
					if(i == 0 || i == 20)
					{
						outDungeon[i][j] = '-';
					}else if((j == 0 || j == 79) && i != 0 && i != 20)
					{
						outDungeon[i][j] = '|';
					}else if(readHardness[i][j] == 0)
					{
						outDungeon[i][j] = '#';
					}else{
						outDungeon[i][j] = ' ';
					}
					
				}
				
			}
		 
			int roomPos = 0;
			while(roomPos < roomSizes)
			{
				int startRoomX = readRooms[roomPos];
				roomPos++;
				int startRoomY = readRooms[roomPos];
				roomPos++;
				int maximumX = startRoomX + readRooms[roomPos];
				roomPos++;
				int maximumY = startRoomY + readRooms[roomPos];
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
			outDungeon[pcY][pcX] = '@';
			printArray(outDungeon);
			
			if(summon == 1)
			{
				if(monsterCount > 0)
				{
					simulate(monsterCount,readHardness,simulationRooms,readPCLoc);
				}else{
					simulate(0,readHardness,simulationRooms,readPCLoc);
				}
			}	

			}else{
				printf("\nNo file to load from.\n");
			}
}

void save_load(int summon, int monsterCount)
{				
			FILE *fp;
			char *path;
			path = malloc(strlen(getenv("HOME")) + strlen("/.rlg327/dungeon") + 1);
			strcpy(path, getenv("HOME"));
			strcat(path,"/.rlg327/dungeon");
			fp = fopen(path, "r");
			free(path);		
		
			if (!(NULL == fp)) {
			
			char fileMarker[12];
			fread(fileMarker,sizeof(char),sizeof(fileMarker),fp);
			
			uint32_t versionNum;
			fread(&versionNum,sizeof(uint32_t),1,fp);
			versionNum = be32toh(versionNum);
			
			uint32_t fileSize;
			fread(&fileSize,sizeof(uint32_t),1,fp);
			fileSize = be32toh(fileSize);
			
			uint8_t pcX;
			fread(&pcX,sizeof(uint8_t),1,fp);

			uint8_t pcY;
			fread(&pcY,sizeof(uint8_t),1,fp);
			
			uint8_t readPCLoc[2];
			readPCLoc[0] = pcX;
			readPCLoc[1] = pcY;
			
			uint8_t readHardness[21][80];
			fread(readHardness,sizeof(uint8_t),sizeof(readHardness),fp);
			
			int roomSizes = fileSize - ftell(fp);
			uint8_t readRooms[roomSizes];
			fread(readRooms,sizeof(uint8_t),sizeof(readRooms),fp);
			
			int increment;
			int simulationRooms[roomSizes];
			for(increment = 0; increment < roomSizes; increment++)
			{
				simulationRooms[increment] = readRooms[increment];
			}
			
			fclose(fp);
			
			char outDungeon[21][80];
			
			int i,j;
			for(i = 0; i < 21; i++)
			{
				for(j = 0; j < 80; j++)
				{
					if(i == 0 || i == 20)
					{
						outDungeon[i][j] = '-';
					}else if((j == 0 || j == 79) && i != 0 && i != 20)
					{
						outDungeon[i][j] = '|';
					}else if(readHardness[i][j] == 0)
					{
						outDungeon[i][j] = '#';
					}else{
						outDungeon[i][j] = ' ';
					}
					
				}
				
			}
		 
			int roomPos = 0;
			while(roomPos < roomSizes)
			{
				int startRoomX = readRooms[roomPos];
				roomPos++;
				int startRoomY = readRooms[roomPos];
				roomPos++;
				int maximumX = startRoomX + readRooms[roomPos];
				roomPos++;
				int maximumY = startRoomY + readRooms[roomPos];
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
			outDungeon[pcY][pcX] = '@';
			printArray(outDungeon);			
			FILE *f;
			char *p;
			p = malloc(strlen(getenv("HOME")) + strlen("/.rlg327/dungeon") + 1);
			strcpy(p, getenv("HOME"));
			strcat(p,"/.rlg327/dungeon");
			f = fopen(p, "w");
			free(p);		

			fwrite(fileMarker,sizeof(char),sizeof(fileMarker),f);

			versionNum = htobe32(versionNum);
			fwrite(&versionNum,sizeof(uint32_t),1,f);	
			
			fileSize = htobe32(fileSize);
			fwrite(&fileSize,sizeof(uint32_t),1,f);
	
			fwrite(&pcX,sizeof(uint8_t),1,f);
			
			fwrite(&pcY,sizeof(uint8_t),1,f);
			
			fwrite(readHardness,sizeof(uint8_t),sizeof(readHardness),f);
			
			fwrite(readRooms,sizeof(uint8_t),sizeof(readRooms),f);
			
			fclose(f);
			
			if(summon == 1)
			{
				if(monsterCount > 0)
				{
					simulate(monsterCount,readHardness,simulationRooms,readPCLoc);
				}else{
					simulate(0,readHardness,simulationRooms,readPCLoc);
				}
			}	
				
			}else{
				printf("\nNo file to load from.\n");
			}
		
			
}
void printArray(char dungeon[21][80])
{
	int k,l;
		for(k = 0; k < 21;k++)
		{
			for(l = 0; l < 80;l++)	
			{
			printf("%c", dungeon[k][l]);	
			}
			printf("\n");
		}
}