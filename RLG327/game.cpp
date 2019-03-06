#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>
#include<time.h>
#include<string.h>
#include<string>
#include <vector> 
#include "game.h"
#include "gameUtil.h"
#include "monsterMap.h"
#include "heap.h"
#include "switches.h"
#include "generateDungeon.h"
#include "monsterParser.h"
#include "itemParser.h"

char outDungeon[21][80];

using namespace std;

void simulate(int monsterCount, uint8_t hardness[21][80], int roomStats[24], uint8_t pcLocation[2], object* carry[10], object* equipment[12], int pcHP)
{

  srand(time(0));
  
  uint8_t modHardness[21][80];
  character *nextCharacterMove;
  int ioCostArr[21][80];
  uint8_t newPCLoc[2];
  int cost = INT_MAX;
  int nextX;
  int nextY;
  int initialX;
  int initialY;
  int bossisDead = 0;
  int pcisDead = 0;
  
  int i,j;
  for(i = 0; i < 21; i++)
  {
		for(j = 0; j < 80; j++)
		{
			modHardness[i][j] = hardness[i][j];
		}
  }
  
  int upStair[2];
  upStair[0] = roomStats[5] + 2;
  upStair[1] = roomStats[4] + 3;
  int downStair[2];
  downStair[0] = roomStats[9] + 2;
  downStair[1] = roomStats[8] + 3;
  
  int downPlaced = 0;
  int upPlaced = 0;
	
			
  //Initializing heap
  heap_t h;
  heap_init(&h, speed_cmp, NULL);
  
  //Initializing pc and mons in array and adding to heap

  vector<character> characters;
  generateMonsters(monsterCount,characters);
  
  
	for(i = 0; i < 10; i++)
	{
		characters[0].carry[i] = carry[i];
		if(characters[0].carry[i] != NULL)
		{
			characters[0].speed = characters[0].speed + characters[0].equipment[j]->speed;
		}
	}
	
	
	
	for(j = 0; j < 12; j++)
	{
		characters[0].equipment[j] = equipment[j];
		if(characters[0].equipment[j] != NULL)
		{
			characters[0].speed = characters[0].speed + characters[0].equipment[j]->speed;
		}
	}
	
	if(characters[0].speed < 1)
	{
		characters[0].speed = 1;
	}
	
	characters[0].hp = pcHP;
  
  for(i = 0; i < (signed) characters.size(); i++)
  {
		int placed = 0;
	  	if(characters[i].tunneling == 0)
		{
			while(placed != 1)
			{
			int randY = (rand() % 11) + 2;
			int randX = (rand() % 68) + 2;
		
			if(hardness[randY][randX] == 0)
			{
				characters[i].xPos = randX;
				characters[i].yPos = randY;
				placed = 1;
		
			}
		
			}
		
		}else{
			
			int randY = (rand() % 11) + 2;
			int randX = (rand() % 68) + 2;
			characters[i].xPos = randX;
			characters[i].yPos = randY;
			placed = 1;
			
		}
		characters[i].hn = heap_insert(&h, &characters[i]);
  }
  
   characters[0].xPos = pcLocation[0];
   characters[0].yPos = pcLocation[1];
   characters[0].hp = 10000;
  
    //Make sure to populate object x and y fields
	vector<object> objects;
	generateObjects(10,objects);
	
	for(i = 0; i < (signed) objects.size(); i++)
	{
		int placed = 0;
		while(placed != 1)
		{
			int randY = (rand() % 11) + 2;
			int randX = (rand() % 68) + 2;
			int taken = 0;
		
			if(hardness[randY][randX] == 0)
			{
				
			for(j = 0; j < (signed) objects.size(); j++)
			{
			if((characters[i].xPos == randX && characters[i].yPos == randY))
				{
					taken = 1;
				}
			}
			
			if((downStair[1] == randX && downStair[0] == randY) || (upStair[1] == randX && upStair[0] == randY))
			{
				taken = 1;
			}
				if(taken == 0)
				{
					objects[i].xPos = randX;
					objects[i].yPos = randY;
					placed = 1;
				}
			}
		
		}
	}
	
	//Initializing npcMap to keep track of npc positions.
	character *npcMap[21][80];
	
	memset(npcMap, 0 , sizeof(character *) * 80 * 21);
	
	for(i = 0; i < (signed) characters.size(); i++)
	{
		npcMap[characters[i].yPos][characters[i].xPos] = &characters[i];
	}
	
	
	io_init_terminal();
	
	//Fog stuff
	int fogOn = 1;
	char pastSeen[21][80];

	for(i = 0; i < 21; i++)
			{
				for(j = 0; j < 80; j++)
				{
					if(i == 0 || i == 20)
					{
						pastSeen[i][j] = '-';
					}else if((j == 0 || j == 79) && i != 0 && i != 20)
					{
						pastSeen[i][j] = '|';
					}else{
						pastSeen[i][j] = ' ';
					}
					
				}
				
			}
	
	
	setOutDungeon(outDungeon,modHardness,roomStats);
	
	 outDungeon[downStair[0]][downStair[1]] = '>';
	 outDungeon[upStair[0]][upStair[1]] = '<';
	 
	  for(i = 0; i < (signed) objects.size(); i++)
	  {
			outDungeon[objects[i].yPos][objects[i].xPos] = objects[i].symb;  
	  }
	  
		
	  for(i = 0; i < (signed) characters.size(); i++)
	  {
			outDungeon[characters[i].yPos][characters[i].xPos] = characters[i].symb;  
	  }
	  
	  setTerminal(outDungeon,characters,objects);
	  refresh();
	
	//Begin Game
	while(1)
	{ 		
	 setOutDungeon(outDungeon,modHardness,roomStats);
	  
	  nextCharacterMove = (character *) heap_remove_min(&h);
	  
	  initialX = nextCharacterMove->xPos;
	  initialY = nextCharacterMove->yPos;
	  
    int isErratic = 0;
	if(nextCharacterMove)
	{
	if(nextCharacterMove->isPC == 1)
	  {  
	   clrtobot();
	  
		int input = 0;
		
		while(input == 0)
		{
			input = getchar();
		}
		//Determining user input and choosing an action
		int escapeKey;
		switch(input)
		{
			
			case 'f':
			
			if(fogOn == 1)
			{
				fogOn = 0;
			}else if(fogOn == 0)
			{
				fogOn = 1;
			}
			
			break;
			
			case '7':
			case 'y':
			
			if(modHardness[nextCharacterMove->yPos - 1][nextCharacterMove->xPos - 1] > 0)
			{
				addstr("THATS A WALL BRO XD");
			}else{
				nextCharacterMove->yPos = nextCharacterMove->yPos - 1;
				nextCharacterMove->xPos = nextCharacterMove->xPos - 1;
			}
			
			break;
			
			case '8':
			case 'k':
			
			if(modHardness[nextCharacterMove->yPos - 1][nextCharacterMove->xPos] > 0)
			{
				addstr("THATS A WALL BRO XD");
			}else{
				nextCharacterMove->yPos = nextCharacterMove->yPos - 1;
				nextCharacterMove->xPos = nextCharacterMove->xPos;
			}
			
			break;
			
			case '9':
			case 'u':
			
			if(modHardness[nextCharacterMove->yPos - 1][nextCharacterMove->xPos + 1] > 0)
			{
				addstr("THATS A WALL BRO XD");
			}else{
				nextCharacterMove->yPos = nextCharacterMove->yPos - 1;
				nextCharacterMove->xPos = nextCharacterMove->xPos + 1;
			}
			
			break;
			
			case '6':
			case 'l':
			
			if(modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos + 1] > 0)
			{
				addstr("THATS A WALL BRO XD");
			}else{
				nextCharacterMove->yPos = nextCharacterMove->yPos;
				nextCharacterMove->xPos = nextCharacterMove->xPos + 1;
			}
			
			break;
			
			case '3':
			case 'n':
			
			if(modHardness[nextCharacterMove->yPos + 1][nextCharacterMove->xPos + 1] > 0)
			{
				addstr("THATS A WALL BRO XD");
			}else{
				nextCharacterMove->yPos = nextCharacterMove->yPos + 1;
				nextCharacterMove->xPos = nextCharacterMove->xPos + 1;
			}
			
			break;
			
			case '2':
			case 'j':
			
			if(modHardness[nextCharacterMove->yPos + 1][nextCharacterMove->xPos] > 0)
			{
				addstr("THATS A WALL BRO XD");
			}else{
				nextCharacterMove->yPos = nextCharacterMove->yPos + 1;
				nextCharacterMove->xPos = nextCharacterMove->xPos;
			}
			
			break;
			
			case '1':
			case 'b':
			
			if(modHardness[nextCharacterMove->yPos + 1][nextCharacterMove->xPos - 1] > 0)
			{
				addstr("THATS A WALL BRO XD");
			}else{
				nextCharacterMove->yPos = nextCharacterMove->yPos + 1;
				nextCharacterMove->xPos = nextCharacterMove->xPos - 1;
			}
			
			break;
			
			case '4':
			case 'h':
			
			if(modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos - 1] > 0)
			{
				addstr("THATS A WALL BRO XD");
			}else{
				nextCharacterMove->yPos = nextCharacterMove->yPos;
				nextCharacterMove->xPos = nextCharacterMove->xPos - 1;
			}
			
			break;
			
			case '5':
			case ' ':
			
			clrtobot();
			addstr("Resting...");
			
			break;
			
			case 'Q':
			
			endwin();
			printf("\n\nYOURE A QUITTER!!!\n\n");
			exit(-1);
			
			break;
			
			case '>':
			
			if(nextCharacterMove->yPos == downStair[0] && nextCharacterMove->xPos == downStair[1])
			{
				string emptyString[1] = {" "};
	            generateDungeon((char **)emptyString,1,(characters.size() - 1), characters[0].carry, characters[0].equipment, characters[0].hp);
			}else{
				addstr("YOU NEED TO BE STANDING ON A DOWN STAIRCASE ===> (>) <===");
			}
			
			break;
			
			case '<':
			
			if(nextCharacterMove->yPos == upStair[0] && nextCharacterMove->xPos == upStair[1])
			{
				string emptyString[1] = {" "};
	            generateDungeon((char **)emptyString,1,(characters.size() - 1), characters[0].carry, characters[0].equipment, characters[0].hp);
			}else{
				addstr("YOU NEED TO BE STANDING ON AN UP STAIRCASE ===> (<) <===");
			}
			
			break;

			case 'm':
			printMonsterList(characters);
			break;
			
			case 'g':
			 
			int placed;
			placed = 0;;
			int teleX;
			teleX = nextCharacterMove->xPos;
			int teleY;
			teleY = nextCharacterMove->yPos - 1;
			
			addstr("**TELEPORTER MODE ENGAGED**");
			
			while(placed == 0)
			{
			 
			  setOutDungeon(outDungeon,modHardness,roomStats);
			 
			  outDungeon[downStair[0]][downStair[1]] = '>';
			  outDungeon[upStair[0]][upStair[1]] = '<';
				
			  for(i = 0; i < (signed) objects.size(); i++)
			  {
				  if(objects[i].isDestroyed == 0 && objects[i].isPickedUp == 0)
				  {
					  outDungeon[objects[i].yPos][objects[i].xPos] = objects[i].symb;  
				  }	
			  }
				
			  for(i = 0; i < (signed) characters.size(); i++)
			  {
				  if(characters[i].isDead == 0)
				  {
					  outDungeon[characters[i].yPos][characters[i].xPos] = characters[i].symb;  
				  }
			  }
			  
			  outDungeon[teleY][teleX] = '*';
			  
			  setTerminal(outDungeon,characters,objects);
			  refresh();
				
			int teleMove = 0;
			
			while(teleMove == 0)
			{
				teleMove = getch();
			}
			clrtobot();
			
			switch(teleMove)
			{
			
			case '7':
			case 'y':
			
			if(teleY >= 2 && teleX - 1 >= 2)
			{
			teleY = teleY - 1;
			teleX = teleX - 1;
			}else{
				addstr("PLEASE PICK AN AREA TO TELEPORT WITHIN THE BOUNDARY.");
			}
			
			break;
			
			case '8':
			case 'k':
			
			if(teleY - 1 >= 2)
			{
				teleY = teleY - 1;
				teleX = teleX;
			}else{
				addstr("PLEASE PICK AN AREA TO TELEPORT WITHIN THE BOUNDARY.");
			}
		
			break;
			
			case '9':
			case 'u':
			
			if(teleY - 1 >= 2 && teleX + 1 <= 78)
			{
				teleY = teleY - 1;
				teleX = teleX + 1;
			}else{
				addstr("PLEASE PICK AN AREA TO TELEPORT WITHIN THE BOUNDARY.");
			}
			
			break;
			
			case '6':
			case 'l':
			
			if(teleX + 1 <= 78)
			{
				teleY = teleY;
				teleX = teleX + 1;
			}else{
				addstr("PLEASE PICK AN AREA TO TELEPORT WITHIN THE BOUNDARY.");
			}
						
			break;
			
			case '3':
			case 'n':
			
			if(teleY + 1 <= 19 && teleX + 1 <= 78)
			{
				teleY = teleY + 1;
				teleX = teleX + 1;	
			}else{
				addstr("PLEASE PICK AN AREA TO TELEPORT WITHIN THE BOUNDARY.");
			}

			break;
			
			case '2':
			case 'j':
			
			if(teleY + 1 <= 19)
			{
				teleY = teleY + 1;
				teleX = teleX;
			}else{
				addstr("PLEASE PICK AN AREA TO TELEPORT WITHIN THE BOUNDARY.");
			}
	
			break;
			
			case '1':
			case 'b':
			
			if(teleY + 1 <= 19 && teleX - 1 >= 2)
			{
				teleY = teleY + 1;
				teleX = teleX - 1;	
			}else{
				addstr("PLEASE PICK AN AREA TO TELEPORT WITHIN THE BOUNDARY.");
			}

			break;
			
			case '4':
			case 'h':
			
			if(teleX - 1 >= 2)
			{
				teleY = teleY;
				teleX = teleX - 1;
			}else{
				addstr("PLEASE PICK AN AREA TO TELEPORT WITHIN THE BOUNDARY.");
			}
			
			break;
			
			case 'g':
			
			if(!npcMap[teleY][teleX])
			{
				modHardness[teleY][teleX] = 0;
				characters[0].xPos = teleX;
				characters[0].yPos = teleY;
				placed = 1;
			}else if(npcMap[teleY][teleX] != &characters[0]){
				addstr("YOU ARE NOT PERMITTED TO TELEPORT ON TOP OF A MONSTER >:)");
			}else if(npcMap[teleY][teleX] == &characters[0]){
				addstr("WHY WOULD YOU TELEPORT TO YOURSELF.... THATS WEIRD");
			}
			
			break;
			
			case 'r':
			
			int valid = 0;
			
			while(valid == 0)
			{
				int randTeleX = (rand() % 75) + 2;
				int randTeleY = (rand() % 16) + 2;
				if(!npcMap[randTeleY][randTeleX])
				{
					characters[0].xPos = randTeleX;
					characters[0].yPos = randTeleY;
					placed = 1;
					valid = 1;
				}
			}
			
			break;
				
			}
				
			}
			 
			break;
			
			case 'L':
			
			int monX;
			monX = nextCharacterMove->xPos;
			int monY;
			monY = nextCharacterMove->yPos - 1;
			int monsterLookedAt;
			monsterLookedAt = 0;
			
			addstr("**PLEASE SELECT A MONSTER TO OBSERVE**");
			
			while(monsterLookedAt == 0)
			{
			 
			  setOutDungeon(outDungeon,modHardness,roomStats);
			 
			  outDungeon[downStair[0]][downStair[1]] = '>';
			  outDungeon[upStair[0]][upStair[1]] = '<';
				
			  for(i = 0; i < (signed) objects.size(); i++)
			  {
				  if(objects[i].isDestroyed == 0 && objects[i].isPickedUp == 0)
				  {
					outDungeon[objects[i].yPos][objects[i].xPos] = objects[i].symb;   
				  } 
			  }
				
			  for(i = 0; i < (signed) characters.size(); i++)
			  {
				  if(characters[i].isDead == 0)
				  {
					outDungeon[characters[i].yPos][characters[i].xPos] = characters[i].symb;    
				  }
			  }
			  
			  outDungeon[monY][monX] = '*';
			  
			  setTerminal(outDungeon,characters,objects);
			  refresh();
				
			int monMove = 0;
			
			while(monMove == 0)
			{
				monMove = getch();
			}
			clrtobot();
			
			switch(monMove)
			{
			
			case '7':
			case 'y':
			
			if(monY >= 2 && monX - 1 >= 2)
			{
			monY = monY - 1;
			monX = monX - 1;
			}else{
				addstr("PLEASE PICK A MONSTER TO WITHIN THE BOUNDARY.");
			}
			
			break;
			
			case '8':
			case 'k':
			
			if(monY - 1 >= 2)
			{
				monY = monY - 1;
				monX = monX;
			}else{
				addstr("PLEASE PICK A MONSTER TO WITHIN THE BOUNDARY.");
			}
		
			break;
			
			case '9':
			case 'u':
			
			if(monY - 1 >= 2 && monX + 1 <= 78)
			{
				monY = monY - 1;
				monX = monX + 1;
			}else{
				addstr("PLEASE PICK A MONSTER TO WITHIN THE BOUNDARY.");
			}
			
			break;
			
			case '6':
			case 'l':
			
			if(monX + 1 <= 78)
			{
				monY = monY;
				monX = monX + 1;
			}else{
				addstr("PLEASE PICK A MONSTER TO WITHIN THE BOUNDARY.");
			}
						
			break;
			
			case '3':
			case 'n':
			
			if(monY + 1 <= 19 && monX + 1 <= 78)
			{
				monY = monY + 1;
				monX = monX + 1;	
			}else{
				addstr("PLEASE PICK A MONSTER TO WITHIN THE BOUNDARY.");
			}

			break;
			
			case '2':
			case 'j':
			
			if(monY + 1 <= 19)
			{
				monY = monY + 1;
				monX = monX;
			}else{
				addstr("PLEASE PICK A MONSTER TO WITHIN THE BOUNDARY.");
			}
	
			break;
			
			case '1':
			case 'b':
			
			if(monY + 1 <= 19 && monX - 1 >= 2)
			{
				monY = monY + 1;
				monX = monX - 1;	
			}else{
				addstr("PLEASE PICK A MONSTER TO WITHIN THE BOUNDARY.");
			}

			break;
			
			case '4':
			case 'h':
			
			if(monX - 1 >= 2)
			{
				monY = monY;
				monX = monX - 1;
			}else{
				addstr("PLEASE PICK A MONSTER TO WITHIN THE BOUNDARY.");
			}
			
			break;
			
			case 't':
			
			for(i = 0; i < (signed) characters.size(); i++)
			{
				if(monX == characters[i].xPos && monY == characters[i].yPos && characters[i].isDead == 0)
				{	
					clear();
					refresh();
					escapeKey = 0;
					while(escapeKey != 27)
					{
						char intell[4];
						sprintf(intell,"%d", characters[i].intelligence);
						char tele[4];
						sprintf(tele,"%d", characters[i].telepathy);
						char tunn[4];
						sprintf(tunn,"%d", characters[i].tunneling);
						char erra[4];
						sprintf(erra,"%d", characters[i].erratic);
						char pass[4];
						sprintf(pass,"%d", characters[i].pass);
						char pickup[4];
						sprintf(pickup,"%d", characters[i].pickup);
						char destroy[4];
						sprintf(destroy,"%d", characters[i].destroy);
						char boss[4];
						sprintf(boss,"%d", characters[i].boss);
						char hp[4];
						sprintf(hp,"%d", characters[i].hp);
		
						char char_array[1250]; 
						
						strcpy(char_array, characters[i].name.c_str());
						strcat(char_array, "\n\n" );
						strcat(char_array, characters[i].desc.c_str());
						strcat(char_array, "\n\n" );
						strcat(char_array, "INTELLIGENCE ===> ");
						strcat(char_array, intell);
						strcat(char_array, "\t" );
						strcat(char_array, "TELEPATHY ===> ");
						strcat(char_array, tele);
						strcat(char_array, "\t" );						
						strcat(char_array, "TUNNELING ===> ");
						strcat(char_array, tunn);
						strcat(char_array, "\n\n" );
						strcat(char_array, "ERRATIC ===> ");
						strcat(char_array, erra);
						strcat(char_array, "\t\t" );						
						strcat(char_array, "PASS ===> ");
						strcat(char_array, pass);
						strcat(char_array, "\t\t" );
						strcat(char_array, "PICKUP ===> ");
						strcat(char_array, pickup);
						strcat(char_array, "\n\n" );
						strcat(char_array, "DESTROY ===> ");
						strcat(char_array, destroy);
						strcat(char_array, "\t\t" );						
						strcat(char_array, "BOSS ===> ");
						strcat(char_array, boss);
						strcat(char_array, "\t\t" );
						strcat(char_array, "HP ===> ");
						strcat(char_array, hp);
						strcat(char_array, "\n\n" );
						
						printw("%s", char_array);
						
						while(escapeKey != KEY_DOWN && escapeKey != KEY_UP && escapeKey != 27)
						{
							escapeKey = getch();
						}
					}
		
					monsterLookedAt = 1;
					
					clear();
					refresh();
				}
			}
			
			break;
			
			case 27:
			
			monsterLookedAt = 1;
			
			break;
			
				
			}
			
			}
			
			break;
			
			case ',':
			
			for(i = 0; i < (signed) objects.size(); i++)
			{
				if(objects[i].xPos == nextCharacterMove->xPos && objects[i].yPos == nextCharacterMove->yPos && objects[i].isDestroyed == 0 && objects[i].isPickedUp == 0)
				{
					for(j = 0; j < 10; j++)
					{
						if(characters[0].carry[j] == NULL)
						{
							characters[0].carry[j] = &objects[i];
							objects[i].isPickedUp = 1;
							break;
						}
					}
				}
			}
			
			break;
			
			case 'i':
			
			clear();
			refresh();

			escapeKey = 0;
			
			while(escapeKey != 27)
			{ 
				char carryList[2000];		
				strcpy(carryList, "ITEMS BEING CARRIED:\n\n");
				
				for(i = 0; i < 10; i++)
				{
					if(characters[0].carry[i] != NULL)
					{
					strcat(carryList, characters[0].carry[i]->name.c_str());
					strcat(carryList, "\n\n");
					}
				}
				
				printw("%s", carryList);
				
				while(escapeKey != 27)
				{
					escapeKey = getch();
				}
			}
			
			clear();
			refresh();
			
			break;
			
			//Wear an item.  Prompts the user for a carry slot.  If an item of that type is alreadyequipped, items are swapped.
			case 'w':
			
			clear();
			refresh();

			escapeKey = 0;
			
			while(escapeKey != 27)
			{
				char carryPrompt[1250];		
				strcpy(carryPrompt, "Carry Slots:\nTo equip an item you're carrying, press select the slot associated with the carried item.\n\n");
				strcat(carryPrompt, "0 ===> ");
				if(characters[0].carry[0] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[0]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "1 ===> ");
				if(characters[0].carry[1] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[1]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "2 ===> ");
				if(characters[0].carry[2] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[2]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "3 ===> ");
				if(characters[0].carry[3] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[3]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "4 ===> ");
				if(characters[0].carry[4] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[4]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "5 ===> ");
				if(characters[0].carry[5] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[5]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "6 ===> ");
				if(characters[0].carry[6] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[6]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "7 ===> ");
				if(characters[0].carry[7] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[7]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "8 ===> ");
				if(characters[0].carry[8] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[8]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "9 ===> ");
				if(characters[0].carry[9] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[9]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				
				printw("%s", carryPrompt);
				
				while(escapeKey != 27 && escapeKey != '0' && escapeKey != '1' && escapeKey != '2' && escapeKey != '3' 
				&& escapeKey != '4' && escapeKey != '5' && escapeKey != '6' && escapeKey != '7' && escapeKey != '8' && escapeKey != '9')
				{
					escapeKey = getch();
				}
				//WEAPON,OFFHAND,RANGED,ARMOR,HELMET,CLOAK,GLOVES,BOOTS,AMULET,LIGHT, and two forRING,
				
				//) [ ] ( { \ =
				int slot;
				if(escapeKey != 27)
				{
					if(escapeKey == '0')
					{
						slot = 0;
					}else if(escapeKey == '1')
					{
						slot = 1;
					}else if(escapeKey == '2')
					{
						slot = 2;
					}else if(escapeKey == '3')
					{
						slot = 3;
					}else if(escapeKey == '4')
					{
						slot = 4;
					}else if(escapeKey == '5')
					{
						slot = 5;
					}else if(escapeKey == '6')
					{
						slot = 6;
					}else if(escapeKey == '7')
					{
						slot = 7;
					}else if(escapeKey == '8')
					{
						slot = 8;
					}else if(escapeKey == '9')
					{
						slot = 9;
					}
					
					
					if(characters[0].carry[slot] != NULL)
					{
						if(characters[0].carry[slot]->symb == '|')
						{
							if(characters[0].equipment[0] == NULL)
							{
								characters[0].equipment[0] = characters[0].carry[slot];
								characters[0].carry[slot] = NULL;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[0]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
											escapeKey = 27;
							}else{
								object* temp;
								temp = characters[0].carry[slot];
								characters[0].carry[slot] = characters[0].equipment[0];
								characters[0].equipment[0] = temp;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[0]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
								escapeKey = 27;
							}
						}else if(characters[0].carry[slot]->symb == ')')
						{
							if(characters[0].equipment[1] == NULL)
							{
								characters[0].equipment[1] = characters[0].carry[slot];
								characters[0].carry[slot] = NULL;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[1]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
								escapeKey = 27;
							}else{
								object* temp;
								temp = characters[0].carry[slot];
								characters[0].carry[slot] = characters[0].equipment[1];
								characters[0].equipment[1] = temp;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[1]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
								escapeKey = 27;
							}
						}else if(characters[0].carry[slot]->symb == '}')
						{
							if(characters[0].equipment[2] == NULL)
							{
								characters[0].equipment[2] = characters[0].carry[slot];
								characters[0].carry[slot] = NULL;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[2]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
								escapeKey = 27;
							}else{
								object* temp;
								temp = characters[0].carry[slot];
								characters[0].carry[slot] = characters[0].equipment[2];
								characters[0].equipment[2] = temp;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[2]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
								escapeKey = 27;
							}
						}else if(characters[0].carry[slot]->symb == '[')
						{
							if(characters[0].equipment[3] == NULL)
							{
								characters[0].equipment[3] = characters[0].carry[slot];
								characters[0].carry[slot] = NULL;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[3]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
								escapeKey = 27;
							}else{
								object* temp;
								temp = characters[0].carry[slot];
								characters[0].carry[slot] = characters[0].equipment[3];
								characters[0].equipment[3] = temp;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[3]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
								escapeKey = 27;
							}
						}else if(characters[0].carry[slot]->symb == ']')
						{
							if(characters[0].equipment[4] == NULL)
							{
								characters[0].equipment[4] = characters[0].carry[slot];
								characters[0].carry[slot] = NULL;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[4]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
								escapeKey = 27;
							}else{
								object* temp;
								temp = characters[0].carry[slot];
								characters[0].carry[slot] = characters[0].equipment[4];
								characters[0].equipment[4] = temp;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[4]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
								escapeKey = 27;
							}
						}else if(characters[0].carry[slot]->symb == '(')
						{
							if(characters[0].equipment[5] == NULL)
							{
								characters[0].equipment[5] = characters[0].carry[slot];
								characters[0].carry[slot] = NULL;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[5]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
								escapeKey = 27;
							}else{
								object* temp;
								temp = characters[0].carry[slot];
								characters[0].carry[slot] = characters[0].equipment[5];
								characters[0].equipment[5] = temp;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[5]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
								escapeKey = 27;
							}
						}else if(characters[0].carry[slot]->symb == '{')
						{
							if(characters[0].equipment[6] == NULL)
							{
								characters[0].equipment[6] = characters[0].carry[slot];
								characters[0].carry[slot] = NULL;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[6]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
								escapeKey = 27;
							}else{
								object* temp;
								temp = characters[0].carry[slot];
								characters[0].carry[slot] = characters[0].equipment[6];
								characters[0].equipment[6] = temp;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[6]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
								escapeKey = 27;
							}
						}else if(characters[0].carry[slot]->symb == '\\')
						{
							if(characters[0].equipment[7] == NULL)
							{
								characters[0].equipment[7] = characters[0].carry[slot];
								characters[0].carry[slot] = NULL;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[7]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
								escapeKey = 27;
							}else{
								object* temp;
								temp = characters[0].carry[slot];
								characters[0].carry[slot] = characters[0].equipment[7];
								characters[0].equipment[7] = temp;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[7]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
								escapeKey = 27;
							}
						}else if(characters[0].carry[slot]->symb == '"')
						{
							if(characters[0].equipment[8] == NULL)
							{
								characters[0].equipment[8] = characters[0].carry[slot];
								characters[0].carry[slot] = NULL;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[8]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
								escapeKey = 27;
							}else{
								object* temp;
								temp = characters[0].carry[slot];
								characters[0].carry[slot] = characters[0].equipment[8];
								characters[0].equipment[8] = temp;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[8]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
								escapeKey = 27;
							}
						}else if(characters[0].carry[slot]->symb == '_')
						{
							if(characters[0].equipment[9] == NULL)
							{
								characters[0].equipment[9] = characters[0].carry[slot];
								characters[0].carry[slot] = NULL;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[9]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
								escapeKey = 27;
							}else{
								object* temp;
								temp = characters[0].carry[slot];
								characters[0].carry[slot] = characters[0].equipment[7];
								characters[0].equipment[7] = temp;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[9]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
								escapeKey = 27;
							}
						}else if(characters[0].carry[slot]->symb == '=')
						{
							if(characters[0].equipment[10] == NULL)
							{
								characters[0].equipment[10] = characters[0].carry[slot];
								characters[0].carry[slot] = NULL;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[10]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
								escapeKey = 27;
							}else if(characters[0].equipment[11] == NULL)
							{
								characters[0].equipment[11] = characters[0].carry[slot];
								characters[0].carry[slot] = NULL;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[11]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
								escapeKey = 27;
							}else{
								object* temp;
								temp = characters[0].carry[slot];
								characters[0].carry[slot] = characters[0].equipment[10];
								characters[0].equipment[10] = temp;
								nextCharacterMove->speed = nextCharacterMove->speed + characters[0].equipment[10]->speed;
								if(characters[0].speed < 1)
								{
									characters[0].speed = 1;
								}
								escapeKey = 27;
							}
								
							
						}	
					
				
					
					}else{
						printw("YO DONT HAVE AN ITEM TO EQUIP.");
						escapeKey = 27;
					}
				}

			}				
			
			clear();
			refresh();
			
			break;
			//END 'w'
			
			//Take off an item. Prompts for equipment slot. Item goes to an open carry slot.
			case 't':
			
			
			clear();
			refresh();

			escapeKey = 0;
			
			while(escapeKey != 27)
			{
				char equipmentPrompt[1250];		
				strcpy(equipmentPrompt, "Equipment Slots:\nTo take off an item you're wearing, press select the slot associated with the carried item.\n\n");
				strcat(equipmentPrompt, "a ===> ");
				if(characters[0].equipment[0] != NULL)
				{
					strcat(equipmentPrompt, characters[0].equipment[0]->name.c_str());
					strcat(equipmentPrompt, "\t");
				}else{
					strcat(equipmentPrompt, "NOTHING\t");
				}
				strcat(equipmentPrompt, "b ===> ");
				if(characters[0].equipment[1] != NULL)
				{
					strcat(equipmentPrompt, characters[0].equipment[1]->name.c_str());
					strcat(equipmentPrompt, "\t");
				}else{
					strcat(equipmentPrompt, "NOTHING\t");
				}
				strcat(equipmentPrompt, "c ===> ");
				if(characters[0].equipment[2] != NULL)
				{
					strcat(equipmentPrompt, characters[0].equipment[2]->name.c_str());
					strcat(equipmentPrompt, "\n\n");
				}else{
					strcat(equipmentPrompt, "NOTHING\n\n");
				}
				strcat(equipmentPrompt, "d ===> ");
				if(characters[0].equipment[3] != NULL)
				{
					strcat(equipmentPrompt, characters[0].equipment[3]->name.c_str());
					strcat(equipmentPrompt, "\t");
				}else{
					strcat(equipmentPrompt, "NOTHING\t");
				}
				strcat(equipmentPrompt, "e ===> ");
				if(characters[0].equipment[4] != NULL)
				{
					strcat(equipmentPrompt, characters[0].equipment[4]->name.c_str());
					strcat(equipmentPrompt, "\t");
				}else{
					strcat(equipmentPrompt, "NOTHING\t");
				}
				strcat(equipmentPrompt, "f ===> ");
				if(characters[0].equipment[5] != NULL)
				{
					strcat(equipmentPrompt, characters[0].equipment[5]->name.c_str());
					strcat(equipmentPrompt, "\n\n");
				}else{
					strcat(equipmentPrompt, "NOTHING\n\n");
				}
				strcat(equipmentPrompt, "g ===> ");
				if(characters[0].equipment[6] != NULL)
				{
					strcat(equipmentPrompt, characters[0].equipment[6]->name.c_str());
					strcat(equipmentPrompt, "\t");
				}else{
					strcat(equipmentPrompt, "NOTHING\t");
				}
				strcat(equipmentPrompt, "h ===> ");
				if(characters[0].equipment[7] != NULL)
				{
					strcat(equipmentPrompt, characters[0].equipment[7]->name.c_str());
					strcat(equipmentPrompt, "\t");
				}else{
					strcat(equipmentPrompt, "NOTHING\t");
				}
				strcat(equipmentPrompt, "i ===> ");
				if(characters[0].equipment[8] != NULL)
				{
					strcat(equipmentPrompt, characters[0].equipment[8]->name.c_str());
					strcat(equipmentPrompt, "\n\n");
				}else{
					strcat(equipmentPrompt, "NOTHING\n\n");
				}
				strcat(equipmentPrompt, "j ===> ");
				if(characters[0].equipment[9] != NULL)
				{
					strcat(equipmentPrompt, characters[0].equipment[9]->name.c_str());
					strcat(equipmentPrompt, "\t");
				}else{
					strcat(equipmentPrompt, "NOTHING\t");
				}
				strcat(equipmentPrompt, "k ===> ");
				if(characters[0].equipment[10] != NULL)
				{
					strcat(equipmentPrompt, characters[0].equipment[10]->name.c_str());
					strcat(equipmentPrompt, "\t");
				}else{
					strcat(equipmentPrompt, "NOTHING\t");
				}
				strcat(equipmentPrompt, "l ===> ");
				if(characters[0].equipment[11] != NULL)
				{
					strcat(equipmentPrompt, characters[0].equipment[11]->name.c_str());
					strcat(equipmentPrompt, "\t");
				}else{
					strcat(equipmentPrompt, "NOTHING\t");
				}
				
				printw("%s", equipmentPrompt);
				
				while(escapeKey != 27 && escapeKey != 'a' && escapeKey != 'b' && escapeKey != 'c' && escapeKey != 'd' 
				&& escapeKey != 'e' && escapeKey != 'f' && escapeKey != 'g' && escapeKey != 'h' && escapeKey != 'i' && escapeKey != 'j' && escapeKey != 'k' && escapeKey != 'l')
				{
					escapeKey = getch();
				}
				
				int slot;
				if(escapeKey != 27)
				{
					if(escapeKey == 'a')
					{
						slot = 0;
					}else if(escapeKey == 'b')
					{
						slot = 1;
					}else if(escapeKey == 'c')
					{
						slot = 2;
					}else if(escapeKey == 'd')
					{
						slot = 3;
					}else if(escapeKey == 'e')
					{
						slot = 4;
					}else if(escapeKey == 'f')
					{
						slot = 5;
					}else if(escapeKey == 'g')
					{
						slot = 6;
					}else if(escapeKey == 'h')
					{
						slot = 7;
					}else if(escapeKey == 'i')
					{
						slot = 8;
					}else if(escapeKey == 'j')
					{
						slot = 9;
					}else if(escapeKey == 'k')
					{
						slot = 10;
					}else if(escapeKey == 'l')
					{
						slot = 11;
					}
					
					for(i = 0; i < 10; i++)
					{
						if(characters[0].carry[i] == NULL && characters[0].equipment[slot] != NULL)
						{
							characters[0].speed = characters[0].speed - characters[0].equipment[slot]->speed;
							characters[0].carry[i] = characters[0].equipment[slot];
							characters[0].equipment[slot] = NULL;
							escapeKey = 27;
							break;
						}
					}
					escapeKey = 27;
				}
					
			}
			
			clear();
			refresh();
			

			break;
			//END 't'
			
			//Drop an item. Prompts user for carry slot. Item goes to floor.
			case 'd':
			
			clear();
			refresh();

			escapeKey = 0;
			
			while(escapeKey != 27)
			{
				char carryPrompt[1250];		
				strcpy(carryPrompt, "Carry Slots:\nTo drop an item you're carrying, press select the slot associated with the carried item.\n\n");
				strcat(carryPrompt, "0 ===> ");
				if(characters[0].carry[0] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[0]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "1 ===> ");
				if(characters[0].carry[1] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[1]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "2 ===> ");
				if(characters[0].carry[2] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[2]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "3 ===> ");
				if(characters[0].carry[3] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[3]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "4 ===> ");
				if(characters[0].carry[4] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[4]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "5 ===> ");
				if(characters[0].carry[5] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[5]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "6 ===> ");
				if(characters[0].carry[6] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[6]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "7 ===> ");
				if(characters[0].carry[7] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[7]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "8 ===> ");
				if(characters[0].carry[8] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[8]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "9 ===> ");
				if(characters[0].carry[9] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[9]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				
				printw("%s", carryPrompt);
				
				while(escapeKey != 27 && escapeKey != '0' && escapeKey != '1' && escapeKey != '2' && escapeKey != '3' 
				&& escapeKey != '4' && escapeKey != '5' && escapeKey != '6' && escapeKey != '7' && escapeKey != '8' && escapeKey != '9')
				{
					escapeKey = getch();
				}
				
				clear();
				refresh();
				
				char dropItem[1250];
				
				switch(escapeKey)
				{
					case '0':
					
					if(characters[0].carry[0] != NULL)
					{
						characters[0].carry[0]->isPickedUp = 0;
						characters[0].carry[0]->xPos = nextCharacterMove->xPos;
						characters[0].carry[0]->yPos = nextCharacterMove->yPos;
						objects.push_back(*(characters[0].carry[0]));
						characters[0].carry[0] = NULL;
						escapeKey = 27;
					}else{
						strcpy(dropItem, "YOU DONT HAVE AN ITEM THERE");
						printw("%s", dropItem);
					}
					
					break;
					
					case '1':
					
					if(characters[0].carry[1] != NULL)
					{
						characters[0].carry[1]->isPickedUp = 0;
						characters[0].carry[1]->xPos = nextCharacterMove->xPos;
						characters[0].carry[1]->yPos = nextCharacterMove->yPos;
						objects.push_back(*(characters[0].carry[1]));
						characters[0].carry[1] = NULL;
						escapeKey = 27;
					}else{
						strcpy(dropItem, "YOU DONT HAVE AN ITEM THERE");
						printw("%s", dropItem);
					}
					
					break;
					
					case '2':
					
					if(characters[0].carry[2] != NULL)
					{
						characters[0].carry[2]->isPickedUp = 0;
						characters[0].carry[2]->xPos = nextCharacterMove->xPos;
						characters[0].carry[2]->yPos = nextCharacterMove->yPos;
						objects.push_back(*(characters[0].carry[2]));
						characters[0].carry[2] = NULL;
						escapeKey = 27;
					}else{
						strcpy(dropItem, "YOU DONT HAVE AN ITEM THERE");
						printw("%s", dropItem);
					}
					
					break;
					
					case '3':
					
					if(characters[0].carry[3] != NULL)
					{
						characters[0].carry[3]->isPickedUp = 0;
						characters[0].carry[3]->xPos = nextCharacterMove->xPos;
						characters[0].carry[3]->yPos = nextCharacterMove->yPos;
						objects.push_back(*(characters[0].carry[3]));
						characters[0].carry[3] = NULL;
						escapeKey = 27;
					}else{
						strcpy(dropItem, "YOU DONT HAVE AN ITEM THERE");
						printw("%s", dropItem);
					}
					
					break;
					
					case '4':
					
					if(characters[0].carry[4] != NULL)
					{
						characters[0].carry[4]->isPickedUp = 0;
						characters[0].carry[4]->xPos = nextCharacterMove->xPos;
						characters[0].carry[4]->yPos = nextCharacterMove->yPos;
						objects.push_back(*(characters[0].carry[4]));
						characters[0].carry[4] = NULL;
						escapeKey = 27;
					}else{
						strcpy(dropItem, "YOU DONT HAVE AN ITEM THERE");
						printw("%s", dropItem);
					}
					
					break;
					
					case '5':
					
					if(characters[0].carry[5] != NULL)
					{
						characters[0].carry[5]->isPickedUp = 0;
						characters[0].carry[5]->xPos = nextCharacterMove->xPos;
						characters[0].carry[5]->yPos = nextCharacterMove->yPos;
						objects.push_back(*(characters[0].carry[5]));
						characters[0].carry[5] = NULL;
						escapeKey = 27;
					}else{
						strcpy(dropItem, "YOU DONT HAVE AN ITEM THERE");
						printw("%s", dropItem);
					}
					
					break;
					
					case '6':
					
					if(characters[0].carry[6] != NULL)
					{
						characters[0].carry[6]->isPickedUp = 0;
						characters[0].carry[6]->xPos = nextCharacterMove->xPos;
						characters[0].carry[6]->yPos = nextCharacterMove->yPos;
						objects.push_back(*(characters[0].carry[6]));
						characters[0].carry[6] = NULL;
						escapeKey = 27;
					}else{
						strcpy(dropItem, "YOU DONT HAVE AN ITEM THERE");
						printw("%s", dropItem);
					}
					
					break;
					
					case '7':
					
					if(characters[0].carry[7] != NULL)
					{
						characters[0].carry[7]->isPickedUp = 0;
						characters[0].carry[7]->xPos = nextCharacterMove->xPos;
						characters[0].carry[7]->yPos = nextCharacterMove->yPos;
						objects.push_back(*(characters[0].carry[7]));
						characters[0].carry[7] = NULL;
						escapeKey = 27;
					}else{
						strcpy(dropItem, "YOU DONT HAVE AN ITEM THERE");
						printw("%s", dropItem);
					}
					
					break;
					
					case '8':
					
					if(characters[0].carry[8] != NULL)
					{
						characters[0].carry[8]->isPickedUp = 0;
						characters[0].carry[8]->xPos = nextCharacterMove->xPos;
						characters[0].carry[8]->yPos = nextCharacterMove->yPos;
						objects.push_back(*(characters[0].carry[8]));
						characters[0].carry[8] = NULL;
						escapeKey = 27;
					}else{
						strcpy(dropItem, "YOU DONT HAVE AN ITEM THERE");
						printw("%s", dropItem);
					}
					
					break;
					
					case '9':
					
					if(characters[0].carry[9] != NULL)
					{
						characters[0].carry[9]->isPickedUp = 0;
						characters[0].carry[9]->xPos = nextCharacterMove->xPos;
						characters[0].carry[9]->yPos = nextCharacterMove->yPos;
						objects.push_back(*(characters[0].carry[9]));
						characters[0].carry[9] = NULL;
						escapeKey = 27;
					}else{
						strcpy(dropItem, "YOU DONT HAVE AN ITEM THERE");
						printw("%s", dropItem);
					}
					
					break;
				}
				
				while(escapeKey != 27)
				{
					escapeKey = getch();
				}
				
			}
			
			clear();
			refresh();
			
			break;
			//END 'd'
			
			//Expunge an item from the game. Prompts the user for a carry slot. Item is permanently removed from the game
			case 'x':
			
			clear();
			refresh();

			escapeKey = 0;
			
			while(escapeKey != 27)
			{
				char carryPrompt[1250];		
				strcpy(carryPrompt, "Carry Slots:\nTo delete an item you're carrying, press select the slot associated with the carried item.\n\n");
				strcat(carryPrompt, "0 ===> ");
				if(characters[0].carry[0] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[0]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "1 ===> ");
				if(characters[0].carry[1] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[1]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "2 ===> ");
				if(characters[0].carry[2] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[2]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "3 ===> ");
				if(characters[0].carry[3] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[3]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "4 ===> ");
				if(characters[0].carry[4] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[4]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "5 ===> ");
				if(characters[0].carry[5] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[5]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "6 ===> ");
				if(characters[0].carry[6] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[6]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "7 ===> ");
				if(characters[0].carry[7] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[7]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "8 ===> ");
				if(characters[0].carry[8] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[8]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "9 ===> ");
				if(characters[0].carry[9] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[9]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				
				printw("%s", carryPrompt);
				
				while(escapeKey != 27 && escapeKey != '0' && escapeKey != '1' && escapeKey != '2' && escapeKey != '3' 
				&& escapeKey != '4' && escapeKey != '5' && escapeKey != '6' && escapeKey != '7' && escapeKey != '8' && escapeKey != '9')
				{
					escapeKey = getch();
				}
				
				clear();
				refresh();
				
				char dropItem[1250];
				
				switch(escapeKey)
				{
					case '0':
					
					if(characters[0].carry[0] != NULL)
					{
						characters[0].carry[0]->isDestroyed = 0;
						characters[0].carry[0] = NULL;
						escapeKey = 27;
					}else{
						strcpy(dropItem, "YOU DONT HAVE AN ITEM THERE");
						printw("%s", dropItem);
					}
					
					break;
					
					case '1':
					
					if(characters[0].carry[1] != NULL)
					{
						characters[0].carry[1]->isDestroyed = 0;
						characters[0].carry[1] = NULL;
						escapeKey = 27;
					}else{
						strcpy(dropItem, "YOU DONT HAVE AN ITEM THERE");
						printw("%s", dropItem);
					}
					
					break;
					
					case '2':
					
					if(characters[0].carry[2] != NULL)
					{
						characters[0].carry[2]->isDestroyed = 0;
						characters[0].carry[2] = NULL;
						escapeKey = 27;
					}else{
						strcpy(dropItem, "YOU DONT HAVE AN ITEM THERE");
						printw("%s", dropItem);
					}
					
					break;
					
					case '3':
					
					if(characters[0].carry[3] != NULL)
					{
						characters[0].carry[3]->isDestroyed = 0;
						characters[0].carry[3] = NULL;
						escapeKey = 27;
					}else{
						strcpy(dropItem, "YOU DONT HAVE AN ITEM THERE");
						printw("%s", dropItem);
					}
					
					break;
					
					case '4':
					
					if(characters[0].carry[4] != NULL)
					{
						characters[0].carry[4]->isDestroyed = 0;
						characters[0].carry[4] = NULL;
						escapeKey = 27;
					}else{
						strcpy(dropItem, "YOU DONT HAVE AN ITEM THERE");
						printw("%s", dropItem);
					}
					
					break;
					
					case '5':
					
					if(characters[0].carry[5] != NULL)
					{
						characters[0].carry[5]->isDestroyed = 0;
						characters[0].carry[5] = NULL;
						escapeKey = 27;
					}else{
						strcpy(dropItem, "YOU DONT HAVE AN ITEM THERE");
						printw("%s", dropItem);
					}
					
					break;
					
					case '6':
					
					if(characters[0].carry[6] != NULL)
					{
						characters[0].carry[6]->isDestroyed = 0;
						characters[0].carry[6] = NULL;
						escapeKey = 27;
					}else{
						strcpy(dropItem, "YOU DONT HAVE AN ITEM THERE");
						printw("%s", dropItem);
					}
					
					break;
					
					case '7':
					
					if(characters[0].carry[7] != NULL)
					{
						characters[0].carry[7]->isDestroyed = 0;
						characters[0].carry[7] = NULL;
						escapeKey = 27;
					}else{
						strcpy(dropItem, "YOU DONT HAVE AN ITEM THERE");
						printw("%s", dropItem);
					}
					
					break;
					
					case '8':
					
					if(characters[0].carry[8] != NULL)
					{
						characters[0].carry[8]->isDestroyed = 0;
						characters[0].carry[8] = NULL;
						escapeKey = 27;
					}else{
						strcpy(dropItem, "YOU DONT HAVE AN ITEM THERE");
						printw("%s", dropItem);
					}
					
					break;
					
					case '9':
					
					if(characters[0].carry[9] != NULL)
					{
						characters[0].carry[9]->isDestroyed = 0;
						characters[0].carry[9] = NULL;
						escapeKey = 27;
					}else{
						strcpy(dropItem, "YOU DONT HAVE AN ITEM THERE");
						printw("%s", dropItem);
					}
					
					break;
				}
				
				while(escapeKey != 27)
				{
					escapeKey = getch();
				}
				
			}
			
			clear();
			refresh();
			
			break;
			//END 'x'
		
			//List PC equipment
			case 'e':
			
			clear();
			refresh();

			escapeKey = 0;
			
			while(escapeKey != 27)
			{
				char equipmentList[1500];		
				strcpy(equipmentList, "ITEMS BEING WORN:\n\n");
				
				for(i = 0; i < 12; i++)
				{
					if(characters[0].equipment[i] != NULL)
					{
					strcat(equipmentList, characters[0].equipment[i]->name.c_str());
					strcat(equipmentList, "\n\n");
					}
				}
				
				printw("%s", equipmentList);
				
				while(escapeKey != 27)
				{
					escapeKey = getch();
				}
			}
			
			clear();
			refresh();
			
			break;
			//END 'e'
			
			//Inspect an item. Prompts user for a carry slot. Item’s description is displayed
			case 'I':
			
			clear();
			refresh();

			escapeKey = 0;
			
			while(escapeKey != 27)
			{
				char carryPrompt[1250];		
				strcpy(carryPrompt, "Carry Slots:\nTo equip an item you're carrying, press select the slot associated with the carried item.\n\n");
				strcat(carryPrompt, "0 ===> ");
				if(characters[0].carry[0] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[0]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "1 ===> ");
				if(characters[0].carry[1] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[1]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "2 ===> ");
				if(characters[0].carry[2] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[2]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "3 ===> ");
				if(characters[0].carry[3] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[3]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "4 ===> ");
				if(characters[0].carry[4] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[4]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "5 ===> ");
				if(characters[0].carry[5] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[5]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "6 ===> ");
				if(characters[0].carry[6] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[6]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "7 ===> ");
				if(characters[0].carry[7] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[7]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "8 ===> ");
				if(characters[0].carry[8] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[8]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				strcat(carryPrompt, "9 ===> ");
				if(characters[0].carry[9] != NULL)
				{
					strcat(carryPrompt, characters[0].carry[9]->name.c_str());
					strcat(carryPrompt, "\n\n");
				}else{
					strcat(carryPrompt, "NOTHING\n\n");
				}
				
				printw("%s", carryPrompt);
				
				while(escapeKey != 27 && escapeKey != '0' && escapeKey != '1' && escapeKey != '2' && escapeKey != '3' 
				&& escapeKey != '4' && escapeKey != '5' && escapeKey != '6' && escapeKey != '7' && escapeKey != '8' && escapeKey != '9')
				{
					escapeKey = getch();
				}
				
				clear();
				refresh();
				
				char inspectItem[1250];
				
				switch(escapeKey)
				{
					case '0':
					
					if(characters[0].carry[0] != NULL)
					{
						strcpy(inspectItem, characters[0].carry[0]->desc.c_str());
					}else{
						strcpy(inspectItem, "YOU DONT HAVE AN ITEM THERE");
					}
					
					break;
					
					case '1':
					
					if(characters[0].carry[1] != NULL)
					{
						strcpy(inspectItem, characters[0].carry[1]->desc.c_str());
					}else{
						strcpy(inspectItem, "YOU DONT HAVE AN ITEM THERE");
					}
					
					break;
					
					case '2':
					
					if(characters[0].carry[2] != NULL)
					{
						strcpy(inspectItem, characters[0].carry[2]->desc.c_str());
					}else{
						strcpy(inspectItem, "YOU DONT HAVE AN ITEM THERE");
					}
					
					break;
					
					case '3':
					
					if(characters[0].carry[3] != NULL)
					{
						strcpy(inspectItem, characters[0].carry[3]->desc.c_str());
					}else{
						strcpy(inspectItem, "YOU DONT HAVE AN ITEM THERE");
					}
					
					break;
					
					case '4':
					
					if(characters[0].carry[4] != NULL)
					{
						strcpy(inspectItem, characters[0].carry[4]->desc.c_str());
					}else{
						strcpy(inspectItem, "YOU DONT HAVE AN ITEM THERE");
					}
					
					break;
					
					case '5':
					
					if(characters[0].carry[5] != NULL)
					{
						strcpy(inspectItem, characters[0].carry[5]->desc.c_str());
					}else{
						strcpy(inspectItem, "YOU DONT HAVE AN ITEM THERE");
					}
					
					break;
					
					case '6':
					
					if(characters[0].carry[6] != NULL)
					{
						strcpy(inspectItem, characters[0].carry[6]->desc.c_str());
					}else{
						strcpy(inspectItem, "YOU DONT HAVE AN ITEM THERE");
					}
					
					break;
					
					case '7':
					
					if(characters[0].carry[7] != NULL)
					{
						strcpy(inspectItem, characters[0].carry[7]->desc.c_str());
					}else{
						strcpy(inspectItem, "YOU DONT HAVE AN ITEM THERE");
					}
					
					break;
					
					case '8':
					
					if(characters[0].carry[8] != NULL)
					{
						strcpy(inspectItem, characters[0].carry[8]->desc.c_str());
					}else{
						strcpy(inspectItem, "YOU DONT HAVE AN ITEM THERE");
					}
					
					break;
					
					case '9':
					
					if(characters[0].carry[9] != NULL)
					{
						strcpy(inspectItem, characters[0].carry[9]->desc.c_str());
					}else{
						strcpy(inspectItem, "YOU DONT HAVE AN ITEM THERE");
					}
					
					break;
				}
				
				printw("%s", inspectItem);
				
				while(escapeKey != 27)
				{
					escapeKey = getch();
				}
				
			}
			
			clear();
			refresh();
			
			break;
			//END 'I'
			
		}
		
		
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
		
					if(abs(characters[0].xPos - nextCharacterMove->xPos) <= 5 && abs(characters[0].yPos - nextCharacterMove->yPos) <= 5)
					{
						
						if(characters[0].xPos - nextCharacterMove->xPos < 0)
						{
							
							if(nextCharacterMove->xPos - 1 > 1 && modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos - 1] == 0)
							{
								nextCharacterMove->xPos = nextCharacterMove->xPos - 1;
							}
							
						}else if(characters[0].xPos - nextCharacterMove->xPos > 0)
						{
							
							if(nextCharacterMove->xPos + 1 < 79  && modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos + 1] == 0)
							{
								nextCharacterMove->xPos = nextCharacterMove->xPos + 1;
							}
			
						}
						
						if(characters[0].yPos - nextCharacterMove->yPos < 0)
						{
							
							if(nextCharacterMove->yPos - 1 > 1  && modHardness[nextCharacterMove->yPos - 1][nextCharacterMove->xPos] == 0)
							{
								nextCharacterMove->yPos = nextCharacterMove->yPos - 1;
							}
						
						}else if(characters[0].yPos - nextCharacterMove->yPos > 0 && modHardness[nextCharacterMove->yPos + 1][nextCharacterMove->xPos] == 0)
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
					newPCLoc[0] = characters[0].xPos;
					newPCLoc[1] = characters[0].yPos;
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
					
						if(characters[0].xPos - nextCharacterMove->xPos < 0)
						{
							
							if(nextCharacterMove->xPos - 1 > 1 && modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos - 1] == 0)
							{
								nextCharacterMove->xPos = nextCharacterMove->xPos - 1;
							}
							
						}else if(characters[0].xPos - nextCharacterMove->xPos > 0)
						{
							
							if(nextCharacterMove->xPos + 1 < 79  && modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos + 1] == 0)
							{
								nextCharacterMove->xPos = nextCharacterMove->xPos + 1;
							}
			
						}
						
						if(characters[0].yPos - nextCharacterMove->yPos < 0)
						{
							
							if(nextCharacterMove->yPos - 1 > 1  && modHardness[nextCharacterMove->yPos - 1][nextCharacterMove->xPos] == 0)
							{
								nextCharacterMove->yPos = nextCharacterMove->yPos - 1;
							}
						
						}else if(characters[0].yPos - nextCharacterMove->yPos > 0 && modHardness[nextCharacterMove->yPos + 1][nextCharacterMove->xPos] == 0)
						{
								
							if(nextCharacterMove->yPos + 1 < 79)
							{
								nextCharacterMove->yPos = nextCharacterMove->yPos + 1;
							}
						
						}
					
					break;
					
					case 3:
					
					cost = INT_MAX;
					newPCLoc[0] = characters[0].xPos;
					newPCLoc[1] = characters[0].yPos;
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
					
					if(abs(characters[0].xPos - nextCharacterMove->xPos) <= 5 && abs(characters[0].yPos - nextCharacterMove->yPos) <= 5)
					{
						
						if(characters[0].xPos - nextCharacterMove->xPos < 0)
						{
							
							if(nextCharacterMove->xPos - 1 > 1 && modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos - 1] == 0)
							{
								nextCharacterMove->xPos = nextCharacterMove->xPos - 1;
							}else if(nextCharacterMove->xPos - 1 > 1 && modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos - 1] != 0)
							{
								modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos - 1] = 0;
								nextCharacterMove->xPos = nextCharacterMove->xPos - 1;
							}
							
						}else if(characters[0].xPos - nextCharacterMove->xPos > 0)
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
						
						if(characters[0].yPos - nextCharacterMove->yPos < 0)
						{
							
							if(nextCharacterMove->yPos - 1 > 1  && modHardness[nextCharacterMove->yPos - 1][nextCharacterMove->xPos] == 0)
							{
								nextCharacterMove->yPos = nextCharacterMove->yPos - 1;
							}else if(nextCharacterMove->yPos - 1 > 1  && modHardness[nextCharacterMove->yPos - 1][nextCharacterMove->xPos] != 0)
							{
								modHardness[nextCharacterMove->yPos - 1][nextCharacterMove->xPos] = 0;
								nextCharacterMove->yPos = nextCharacterMove->yPos - 1;
							}
						
						}else if(characters[0].yPos - nextCharacterMove->yPos > 0)
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
					newPCLoc[0] = characters[0].xPos;
					newPCLoc[1] = characters[0].yPos;
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
					
					if(characters[0].xPos - nextCharacterMove->xPos < 0)
						{
							
							if(nextCharacterMove->xPos - 1 > 1)
							{
								modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos - 1] = 0;
								nextCharacterMove->xPos = nextCharacterMove->xPos - 1;
							}
							
						}else if(characters[0].xPos - nextCharacterMove->xPos > 0)
						{
							
							if(nextCharacterMove->xPos + 1 < 79)
							{
								modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos + 1] = 0;
								nextCharacterMove->xPos = nextCharacterMove->xPos + 1;
							}
			
						}
						
						if(characters[0].yPos - nextCharacterMove->yPos < 0)
						{
							
							if(nextCharacterMove->yPos - 1 > 1)
							{
								modHardness[nextCharacterMove->yPos - 1][nextCharacterMove->xPos] = 0;
								nextCharacterMove->yPos = nextCharacterMove->yPos - 1;
							}
						
						}else if(characters[0].yPos - nextCharacterMove->yPos > 0)
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
					newPCLoc[0] = characters[0].xPos;
					newPCLoc[1] = characters[0].yPos;
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
	  
	    // if(nextCharacterMove->yPos == characters[0].yPos && nextCharacterMove->xPos == characters[0].xPos && nextCharacterMove->isPC == 0)
		// {
			// done = 1;
		// }
		
		

		character *moveTo = npcMap[nextCharacterMove->yPos][nextCharacterMove->xPos];
		if(moveTo)
		{
			if(nextCharacterMove->isPC == 1 && modHardness[nextCharacterMove->yPos][nextCharacterMove->xPos] == 0 && (initialX != nextCharacterMove->xPos || initialY != nextCharacterMove->yPos))
			{
				int pcDamage = 0;
				pcDamage  = pcDamage + nextCharacterMove->dam.rollDice();
				for(i = 0; i < 12; i++)
				{
					if(characters[0].equipment[i] != NULL)
					{
						pcDamage = pcDamage +  characters[0].equipment[i]->dam.rollDice();
					}
				}
				moveTo->hp = moveTo->hp - pcDamage;
				
				if(moveTo->hp <= 0)
				{
					moveTo->isDead = 1;
					npcMap[nextCharacterMove->yPos][nextCharacterMove->xPos] = NULL;
					if(moveTo->boss == 1)
					{
						bossisDead = 1;
					}
				}
			}else if(moveTo->isPC == 1)
			{
				int monDamage = 0;
				monDamage += nextCharacterMove->dam.rollDice();
				moveTo->hp = moveTo->hp - monDamage;
				if(moveTo->hp <= 0)
				{
					moveTo->isDead = 1;
					pcisDead = 1;
					npcMap[nextCharacterMove->yPos][nextCharacterMove->xPos] = NULL;
				}
				
			}
			nextCharacterMove->xPos = initialX;
			nextCharacterMove->yPos = initialY;
		}else{
			npcMap[initialY][initialX] = NULL;
			npcMap[nextCharacterMove->yPos][nextCharacterMove->xPos] = nextCharacterMove;
		}
		
		nextCharacterMove->hn = heap_insert(&h, nextCharacterMove);
		
	}
	
	if(bossisDead || pcisDead) break;
	
	if(fogOn)
	{
		//Print using fog of war
		
		int z;
		
		for(i = characters[0].yPos - 3; i <= characters[0].yPos + 3; i++)
		{
			for(j = characters[0].xPos - 3; j <= characters[0].xPos + 3; j++)
			{
				if(modHardness[i][j] == 0)
				{
					pastSeen[i][j] = '#';
					for(z = 0; z <= 20; z = z + 4)
					{
						if((j <= (roomStats[z] + roomStats[z+2] - 1)) && (j >= roomStats[z]) && (i <= (roomStats[z+1] + roomStats[z+3])) && (i >= roomStats[z+1]))
						{
							pastSeen[i][j] = '.';
							break;
						}
					}
					
				}else if(modHardness[i][j] > 0 && modHardness[i][j] != 255){
					pastSeen[i][j] = ' ';
				}
			
			}
		}
		
		if((downStair[1] < (characters[0].xPos + 4) && downStair[1] > (characters[0].xPos - 2) && downStair[0] < (characters[0].yPos + 4) && downStair[0] > (characters[0].yPos - 2)) || downPlaced) 
		{
			pastSeen[downStair[0]][downStair[1]] = '>';
			downPlaced = 1;
		}
		
		if((upStair[1] < (characters[0].xPos + 4) && upStair[1] > (characters[0].xPos - 2) && upStair[0] < (characters[0].yPos + 4) && upStair[0] > (characters[0].yPos - 2)) || upPlaced)
		{
			pastSeen[upStair[0]][upStair[1]] = '<';
			upPlaced = 1;
		}
		
		for(z = 0; z < (signed) objects.size(); z++)
		{
				if((objects[z].isPickedUp == 0 && objects[z].isDestroyed == 0) && objects[z].xPos <= (characters[0].xPos + 3) && objects[z].xPos >= (characters[0].xPos - 3) && objects[z].yPos <= (characters[0].yPos + 3) && objects[z].yPos >= (characters[0].yPos - 3))
				{
					pastSeen[objects[z].yPos][objects[z].xPos] = objects[z].symb;
				}
		}
		
		
		for(z = 1; z < (signed) characters.size();z++)
		{
				if(characters[z].isDead == 0 &&characters[z].xPos <= (characters[0].xPos + 3) && characters[z].xPos >= (characters[0].xPos - 3) && characters[z].yPos <= (characters[0].yPos + 3) && characters[z].yPos >= (characters[0].yPos - 3))
				{
					pastSeen[characters[z].yPos][characters[z].xPos] = characters[z].symb;
				}
		}
		
		for(i = 0; i < 21; i++)
		{
			for(j = 0; j < 80; j++)
			{
				outDungeon[i][j] = pastSeen[i][j];
			}
		}
		
		outDungeon[characters[0].yPos][characters[0].xPos] = characters[0].symb;
		
	  setTerminal(outDungeon,characters,objects);
	  refresh();
		
		
	}else{
	
	  outDungeon[downStair[0]][downStair[1]] = '>';
	  outDungeon[upStair[0]][upStair[1]] = '<';
	  
	  for(i = 0; i < (signed) objects.size(); i++)
	  {
		  if(objects[i].isPickedUp == 0 && objects[i].isDestroyed == 0)
		  {
			outDungeon[objects[i].yPos][objects[i].xPos] = objects[i].symb;   
		  } 
	  }
		
	  for(i = 0; i < (signed) characters.size(); i++)
	  {
		  if(characters[i].isDead == 0)
		  {
			outDungeon[characters[i].yPos][characters[i].xPos] = characters[i].symb;   
		  } 
	  }
	  
	  setTerminal(outDungeon,characters,objects);
	  refresh();
	
	}
	  usleep(250000);
	}
   
    heap_delete(&h);
	
	endwin();
	if(pcisDead)
	{
		printf("\n\nYO\n.\n.\n.\n.\nTHATS GONNA BE A GAME OVER FOR YOU DAWG\n\nXD\n\n");
	}else if(bossisDead)
	{
		printf("WOW. YOU ACTUALLY WON.\n\nHOW ABOUT YOU TRY TO DO IT AGAIN >:)\n\n");
	}
	exit(-1);
	}
