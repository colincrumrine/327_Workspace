#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<string.h>  
#include <stdint.h>
#include <endian.h>
#include <ncurses.h>
#include <climits>
#include <vector> 
#include "game.h"

using namespace std;

int baseSPEED, diceSPEED, sidesSPEED;
int baseHP, diceHP, sidesHP;
int baseDAM, diceDAM, sidesDAM;
int RRTY;
string NAME, DESC, COLOR, ABIL;
char SYMB;

vector <characterDescription> templates;

int isValidMon()
{
	if(baseSPEED == INT_MAX || baseHP == INT_MAX || baseDAM == INT_MAX || diceSPEED == INT_MAX || diceHP == INT_MAX || diceDAM == INT_MAX || sidesSPEED == INT_MAX || sidesHP == INT_MAX || sidesDAM == INT_MAX ||
	   RRTY == 101 || NAME == "" || DESC == "" || COLOR == "" || ABIL == "" || SYMB == ' ')
	   {
		   return 0;
	   }
	   return 1;
}

void printDesc()
{
	cout << endl;
	cout << NAME << endl;
	cout << DESC;
	cout << COLOR << endl;
	cout << baseSPEED << "+"<< diceSPEED << "d" << sidesSPEED << endl;
	cout << ABIL << endl;
	cout << baseHP << "+"<< diceHP << "d" << sidesHP << endl;
	cout << baseDAM << "+"<< diceDAM << "d" << sidesDAM << endl;
	cout << SYMB << endl;
	cout << RRTY << endl;
	cout << endl;	
}

int checkString(string s)
{
	if(s.substr(0,4) == "NAME")
	{
		if(NAME == "")
		{
			NAME = s.substr(5,s.length() - 1);
			return 1;
		}else{
			return 0;
		}
	}else if(s.substr(0,5) == "COLOR")
	{
		if(COLOR == "")
		{
			COLOR = s.substr(6,s.length() - 1);
				return 1;
		}else{
			return 0;
		}
	}else if(s.substr(0,4) == "SYMB")
	{
		if(SYMB == ' ')
		{
			SYMB = s[5];
			return 1;
		}else{
			return 0;
		}
	}else if(s.substr(0,4) == "ABIL")
	{
		if(ABIL == "")
		{
			ABIL = s.substr(5,s.length() - 1);
			return 1;
		}else{
			return 0;
		}
	}else if(s.substr(0,5) == "SPEED")
	{
		if(baseSPEED == INT_MAX && diceSPEED == INT_MAX && sidesSPEED == INT_MAX)
		{
			baseSPEED = atoi(s.substr(6,s.find("+")).c_str());
			diceSPEED = atoi(s.substr(s.find("+"),s.find("d")).c_str());
			sidesSPEED = atoi(s.substr(s.find("d") + 1).c_str());
			return 1;
		}else{
			return 0;
		}
	}else if(s.substr(0,2) == "HP")
	{
		if(baseHP == INT_MAX && diceHP == INT_MAX && sidesHP == INT_MAX)
		{
			baseHP = atoi(s.substr(3,s.find("+")).c_str());
			diceHP = atoi(s.substr(s.find("+"),s.find("d")).c_str());
			sidesHP = atoi(s.substr(s.find("d") + 1).c_str());
			return 1;
		}else{
			return 0;
		}
	}else if(s.substr(0,3) == "DAM")
	{
		if(baseDAM == INT_MAX && diceDAM == INT_MAX && sidesDAM == INT_MAX)
		{
			baseDAM = atoi(s.substr(4,s.find("+")).c_str());
			diceDAM = atoi(s.substr(s.find("+"),s.find("d")).c_str());
			sidesDAM = atoi(s.substr(s.find("d") + 1).c_str());
			return 1;
		}else{
			return 0;
		}
	}else if(s.substr(0,4) == "RRTY")
	{
		if(RRTY == 101)
		{
			RRTY = atoi(s.substr(5).c_str());
				return 1;
		}else{
			return 0;
		}
	}
	return -1;
}

void resetTraits()
{
baseSPEED = INT_MAX, diceSPEED = INT_MAX, sidesSPEED = INT_MAX;
baseHP = INT_MAX, diceHP = INT_MAX, sidesHP = INT_MAX;
baseDAM = INT_MAX, diceDAM = INT_MAX, sidesDAM = INT_MAX;
RRTY = 101;
NAME = "", DESC = "", COLOR = "", ABIL = "";
SYMB = ' ';
}

void parseMonster()
{
	string s;
	char * path = (char *) malloc(strlen(getenv("HOME")) + strlen("/.rlg327/monster_desc.txt") + 1);
	strcpy(path, getenv("HOME"));
	strcat(path,"/.rlg327/monster_desc.txt");
	fstream f(path);
	free(path);
	
	int valMon = 1;
	
	getline(f,s);
	if(s == "RLG327 MONSTER DESCRIPTION 1")
	{
		while(!f.eof())
		{
		valMon = 1;
		getline(f,s);
		if(s == "BEGIN MONSTER")
		{
			resetTraits();
			while(s != "END")
			{
				getline(f,s);
				if(s == "BEGIN MONSTER")
				{
					resetTraits();
				}
				if(s.length() > 1)
				{
					if(checkString(s) == 0)
					{
						valMon = 0;
						break;
					}
				}
				if(s == "DESC")
				{
					if(DESC == "")
					{
						getline(f,s);
						while(s != ".")
						{
							if(s.length() > 77)
							{
								valMon = 0;
								break;
							}
							DESC = DESC + s + "\n";
							getline(f,s);
						}
			
					}else{
						valMon = 0;
						break;
					}
				}
			}
			if(isValidMon() == 1 && valMon == 1)
			{
				characterDescription description;
	
				description.name = NAME;
				description.desc = DESC;
				description.color = COLOR;
				description.abilities = ABIL;
				description.symb = SYMB;
				description.rarity = RRTY;
				
				dice HP;
				HP.base = baseHP;
				HP.dice = diceHP;
				HP.sides = sidesHP;
				description.hp = HP;
				
				dice DAM;
				DAM.base = baseDAM;
				DAM.dice = diceDAM;
				DAM.sides = sidesDAM;
				description.dam = DAM;
				
				dice SPEED;
				SPEED.base = baseSPEED;
				SPEED.dice = diceSPEED;
				SPEED.sides = sidesSPEED;
				description.speed = SPEED;
				
				templates.push_back(description);
			}
		}
		
		}
		
	}else{
		cout << "INCORRECT FILE FORMAT" << endl;
	}
}

void generateMonsters(int monsterCount, vector<character> &monsters)
{
	parseMonster();
	int generatedMonsters = 1;
	int i;
	
	  character pc;
	  
	  for(i = 0; i < 10; i++)
	  {
		  pc.carry[i] = NULL;
	  }
	  
	   for(i = 0; i < 12; i++)
	  {
		  pc.equipment[i] = NULL;
	  }
	  
	  pc.isDead = 0;
	  pc.isPC = 1;
	  pc.priority = 0;
	  pc.speed = 10;
	  pc.nextTurn = 0;
	  pc.name = "PC";
	  pc.desc = "This is the PC.";
	  pc.color = COLOR_WHITE;
	  pc.intelligence = 0;
	  pc.telepathy = 0;
	  pc.tunneling = 0;
	  pc.erratic = 0;
	  pc.pass = 0;
	  pc.pickup = 0;
	  pc.destroy = 0;
	  pc.uniq = 0;
	  pc.boss = 0;
	  pc.hp = 500000;
	  dice pcDam;
	  pcDam.base = 0;
	  pcDam.dice = 1;
	  pcDam.sides = 4;
	  pc.dam = pcDam;
	  pc.symb = '@';
	  pc.rarity = 0;
	  
	  monsters.push_back(pc);
	
	while(generatedMonsters <= monsterCount)
	{
		int eligibleMon = 0;
		int randTemplate = rand() % (templates.size() - 1) + 1;
		for(i = 0; i < (signed) monsters.size(); i++)
		{
			if(templates[randTemplate].name == monsters[i].name && (signed) templates[randTemplate].abilities.find("UNIQ") != -1)
			{
				eligibleMon = 1;
			}
		}
		
		int randRarity = rand() % 99;
		if(randRarity >= templates[randTemplate].rarity)
		{
			eligibleMon = 1;
		}	
		
		if(eligibleMon == 0)
		{
		
		generatedMonsters++;
			
		character mon;
		
		  for(i = 0; i < 10; i++)
		  {
		  mon.carry[i] = NULL;
		  }
	  
		   for(i = 0; i < 12; i++)
		   {
			  mon.equipment[i] = NULL;
		   }
		mon.isDead = 0;
		mon.isPC = 0;
		mon.speed = templates[randTemplate].speed.rollDice();
		mon.nextTurn = 0;
		mon.rarity = templates[randTemplate].rarity;
		mon.symb = templates[randTemplate].symb;
		mon.dam = templates[randTemplate].dam;
		mon.hp = templates[randTemplate].hp.rollDice();
		mon.name = templates[randTemplate].name;
		mon.desc = templates[randTemplate].desc;
		
		string colorVal = templates[randTemplate].color.substr(0,3);
		if(colorVal[0] == 'R')
		{
			mon.color = COLOR_RED;
			
		}else if(colorVal[0] == 'G')
		{
			mon.color = COLOR_GREEN;
			
		}else if(colorVal[0] == 'C')
		{
			mon.color = COLOR_CYAN;
		}else if(colorVal[0] == 'Y')
		{
			mon.color = COLOR_YELLOW;
			
		}else if(colorVal[0] == 'M')
		{
			mon.color = COLOR_MAGENTA;
		}else if(colorVal[0] == 'W')
		{
			mon.color = COLOR_WHITE;
			
		}else if(colorVal[0] == 'B' && colorVal[2] == 'A')
		{
			mon.color = COLOR_BLACK;
			
		}else if(colorVal[0] == 'B' && colorVal[2] == 'U')
		{
			mon.color = COLOR_BLUE;
			
		}
		
		mon.intelligence = 0, mon.telepathy = 0, mon.tunneling = 0, mon.erratic = 0, mon.pass = 0, mon.pickup = 0, mon.destroy = 0, mon.uniq = 0, mon.boss = 0;
		if(templates[randTemplate].abilities.find("SMART") != std::string::npos)
		{
			mon.intelligence = 1;
		}
		if(templates[randTemplate].abilities.find("TELE") != std::string::npos)
		{
			mon.telepathy = 1;
		}
		if(templates[randTemplate].abilities.find("TUNNEL") != std::string::npos)
		{
			mon.tunneling = 1;
		}
		if(templates[randTemplate].abilities.find("ERRATIC") != std::string::npos)
		{
			mon.erratic = 1;
		}
		if(templates[randTemplate].abilities.find("PASS") != std::string::npos)
		{
			mon.pass = 1;
		}
		if(templates[randTemplate].abilities.find("PICKUP") != std::string::npos)
		{
			mon.pickup = 1;
		}
		if(templates[randTemplate].abilities.find("DESTROY") != std::string::npos)
		{
			mon.destroy = 1;
		}
		if(templates[randTemplate].abilities.find("UNIQ") != std::string::npos)
		{
			mon.uniq = 1;
		}
		if(templates[randTemplate].abilities.find("BOSS") != std::string::npos)
		{
			mon.boss = 1;
		}
		
		mon.priority = (mon.intelligence * 1) + (mon.telepathy * 2) + (mon.tunneling * 4) + (mon.erratic * 8);
		
		monsters.push_back(mon);
		
		}
		
		
	}
}
