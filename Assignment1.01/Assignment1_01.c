#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<string.h>  
#include <stdint.h>

/* Main method that initializes borders of room and fills the rest with 0's. It then calls addRooms_Corridors() and addSpaces() to finalize dungeon format. */
void generateDungeon(char *argv[]);
/* Uses the rand() function which is seeded by the srand(tinme(0)) argument to first generate rooms throughout the dungeon and then generate corridors connecting the rooms.*/
void addRooms_Corridors(char dungeon[21][80]);
/* Parses the given dungeon and add spaces where the value at the array position is '0'. */
void addSpaces(char dungeon[21][80]);
/* Uses argument values given in argv[i] to determine whether to read or write the dungeon to the disk. */
void load_save(char *argv[],char dungeon[21][80], int hardness[21][80], int roomStats[24], uint8_t pcLocation[2]);
/* Prints dugeon to the console. */
void printArray(char dungeon[21][80]);

int main(int argc, char *argv[])
{
	srand(time(0));
	generateDungeon(argv);
	return 0;
}

void generateDungeon(char *argv[])
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

	int hardness[21][80];
	
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
	
	load_save(argv,dungeon,hardness,roomStats,pcLocation);
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

void load_save(char *argv[],char dungeon[21][80], int hardness[21][80], int roomStats[24], uint8_t pcLocation[2])
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
	
	 
	if(argv[1] != NULL)
	{
	char saveArr[10];
	char loadArr[10];
	char binaryArr[20];
	strcpy(saveArr, "--save");
	strcpy(loadArr, "--load");
	strcpy(binaryArr,argv[1]);
		if(strcmp(binaryArr,saveArr) == 0)
		{
			printArray(dungeon);
			
			FILE *fp;
			fp = fopen("dungeon", "w");
			
			char fileMarker[12] = "RLG327-F2018";
			fwrite(fileMarker,sizeof(char),sizeof(fileMarker),fp);

            uint32_t versionNum = 0;
			fwrite(&versionNum,sizeof(uint32_t),1,fp);	
			
			uint32_t fileSize = 1726;
			fwrite(&fileSize,sizeof(uint32_t),1,fp);
	
			uint8_t pcX = pcLocation[0];
			fwrite(&pcX,sizeof(uint8_t),1,fp);
			
			uint8_t pcY = pcLocation[1];
			fwrite(&pcY,sizeof(uint8_t),1,fp);
			
			fwrite(savedHardness,sizeof(uint8_t),sizeof(savedHardness),fp);
			
			fwrite(savedRooms,sizeof(uint8_t),sizeof(savedRooms),fp);
			
		}else if(strcmp(binaryArr,loadArr) == 0)
		{	
			FILE *fp;
			fp = fopen("dungeon", "r");
		
			if (!(NULL == fp)) {
			
			char fileMarker[12];
			fread(fileMarker,sizeof(char),sizeof(fileMarker),fp);
			
			uint32_t versionNum;
			fread(&versionNum,sizeof(uint32_t),1,fp);
			
			uint32_t fileSize;
			fread(&fileSize,sizeof(uint32_t),1,fp);
			
			uint8_t pcX;
			fread(&pcX,sizeof(uint8_t),1,fp);

			uint8_t pcY;
			fread(&pcY,sizeof(uint8_t),1,fp);
			
			uint8_t readHardness[21][80];
			fread(readHardness,sizeof(uint8_t),sizeof(readHardness),fp);
			
			int roomSizes = fileSize - ftell(fp);
			uint8_t readRooms[roomSizes];
			fread(readRooms,sizeof(uint8_t),sizeof(readRooms),fp);
			
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
			}else{
				printf("\nNo file to load from.\n");
			}
		}
		
		if(argv[2] != NULL)
		{
			strcpy(binaryArr,argv[2]);
			if(strcmp(binaryArr,saveArr) == 0 && !(strcmp(argv[1],loadArr) == 0))
			{
			printArray(dungeon);
	
			FILE *fp;
			fp = fopen("dungeon", "w");
			
			char fileMarker[12] = "RLG327-F2018";
			fwrite(fileMarker,sizeof(char),sizeof(fileMarker),fp);

            uint32_t versionNum = 0;
			fwrite(&versionNum,sizeof(uint32_t),1,fp);	
			
			uint32_t fileSize = 1726;
			fwrite(&fileSize,sizeof(uint32_t),1,fp);
	
			uint8_t pcX = pcLocation[0];
			fwrite(&pcX,sizeof(uint8_t),1,fp);
			
			uint8_t pcY = pcLocation[1];
			fwrite(&pcY,sizeof(uint8_t),1,fp);
			
			fwrite(savedHardness,sizeof(uint8_t),sizeof(savedHardness),fp);
			
			fwrite(savedRooms,sizeof(uint8_t),sizeof(savedRooms),fp);
			
			}else if(strcmp(binaryArr,loadArr) == 0 && !(strcmp(argv[1],saveArr) == 0))
			{	
			FILE *fp;
			fp = fopen("dungeon", "r");
		
			if (!(NULL == fp)) {
			
			char fileMarker[12];
			fread(fileMarker,sizeof(char),sizeof(fileMarker),fp);
			
			uint32_t versionNum;
			fread(&versionNum,sizeof(uint32_t),1,fp);
			
			uint32_t fileSize;
			fread(&fileSize,sizeof(uint32_t),1,fp);
			
			uint8_t pcX;
			fread(&pcX,sizeof(uint8_t),1,fp);

			uint8_t pcY;
			fread(&pcY,sizeof(uint8_t),1,fp);
			
			uint8_t readHardness[21][80];
			fread(readHardness,sizeof(uint8_t),sizeof(readHardness),fp);
			
			int roomSizes = fileSize - ftell(fp);
			uint8_t readRooms[roomSizes];
			fread(readRooms,sizeof(uint8_t),sizeof(readRooms),fp);
			
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
			}else{
		printf("\nNo file to load from.\n");
				}
			}			
		}

	}else{
		printArray(dungeon);
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