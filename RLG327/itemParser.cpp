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
#include "monsterParser.h"
#include "itemParser.h"

using namespace std;

dice HIT, DODGE, DAM, DEF, SPEED, ATTR, VAL, WEIGHT;
int objRRTY;
string objNAME, objDESC, objCOLOR, TYPE, ART;

vector <objectDescription> objTemplates;

int isValidObj()
{
	if(HIT.base == INT_MAX || HIT.dice == INT_MAX || HIT.sides == INT_MAX ||
	   DODGE.base == INT_MAX || DODGE.dice == INT_MAX || DODGE.sides == INT_MAX ||
	   DAM.base == INT_MAX || DAM.dice == INT_MAX || DAM.sides == INT_MAX ||
	   DEF.base == INT_MAX || DEF.dice == INT_MAX || DEF.sides == INT_MAX ||
	   SPEED.base == INT_MAX || SPEED.dice == INT_MAX || SPEED.sides == INT_MAX ||
	   ATTR.base == INT_MAX || ATTR.dice == INT_MAX || ATTR.sides == INT_MAX ||
	   VAL.base == INT_MAX || VAL.dice == INT_MAX || VAL.sides == INT_MAX ||
	   WEIGHT.base == INT_MAX || WEIGHT.dice == INT_MAX || WEIGHT.sides == INT_MAX ||
	   objRRTY == 101 || objNAME == "" || objDESC == "" || objCOLOR == "" || TYPE == "" || ART == "")
	   {
		   return 0;
	   }
	   return 1;
}

void printObjDesc()
{
	cout << endl;
	cout << objNAME << endl;
	cout << objDESC << endl;
	cout << objCOLOR << endl;
	cout << TYPE << endl;
	cout << ART << endl;
	cout << HIT.base << "+"<< HIT.dice << "d" << HIT.sides << endl;
	cout << DODGE.base << "+"<< DODGE.dice << "d" << DODGE.sides << endl;
	cout << DAM.base << "+"<< DAM.dice << "d" << DAM.sides << endl;
	cout << DEF.base << "+"<< DEF.dice << "d" << DEF.sides << endl;
	cout << SPEED.base << "+"<< SPEED.dice << "d" << SPEED.sides << endl;
	cout << ATTR.base << "+"<< ATTR.dice << "d" << ATTR.sides << endl;
	cout << VAL.base << "+"<< VAL.dice << "d" << VAL.sides << endl;
	cout << WEIGHT.base << "+"<< WEIGHT.dice << "d" << WEIGHT.sides << endl;
	cout << objRRTY << endl;
	cout << endl;	
}

int checkObjString(string s)
{
	if(s.substr(0,4) == "NAME")
	{
		if(objNAME == "")
		{
			objNAME = s.substr(5,s.length() - 1);
			return 1;
		}else{
			return 0;
		}
	}else if(s.substr(0,5) == "COLOR")
	{
		if(objCOLOR == "")
		{
			objCOLOR = s.substr(6,s.length() - 1);
				return 1;
		}else{
			return 0;
		}
	}else if(s.substr(0,4) == "TYPE")
	{
		if(TYPE == "")
		{
			TYPE = s.substr(5,s.length() - 1);
			return 1;
		}else{
			return 0;
		}
	}else if(s.substr(0,3) == "ART")
	{
		if(ART == "")
		{
			ART = s.substr(4,s.length() - 1);
			return 1;
		}else{
			return 0;
		}
	}else if(s.substr(0,3) == "HIT")
	{
		if(HIT.base == INT_MAX && HIT.dice == INT_MAX && HIT.sides == INT_MAX)
		{
			HIT.base = atoi(s.substr(4,s.find("+")).c_str());
			HIT.dice = atoi(s.substr(s.find("+"),s.find("d")).c_str());
			HIT.sides = atoi(s.substr(s.find("d") + 1).c_str());
			return 1;
		}else{
			return 0;
		}
	}else if(s.substr(0,5) == "DODGE")
	{
		if(DODGE.base == INT_MAX && DODGE.dice == INT_MAX && DODGE.sides == INT_MAX)
		{
			DODGE.base = atoi(s.substr(6,s.find("+")).c_str());
			DODGE.dice = atoi(s.substr(s.find("+"),s.find("d")).c_str());
			DODGE.sides = atoi(s.substr(s.find("d") + 1).c_str());
			return 1;
		}else{
			return 0;
		}
	}else if(s.substr(0,3) == "DEF")
	{
		if(DEF.base == INT_MAX && DEF.dice == INT_MAX && DEF.sides == INT_MAX)
		{
			DEF.base = atoi(s.substr(4,s.find("+")).c_str());
			DEF.dice = atoi(s.substr(s.find("+"),s.find("d")).c_str());
			DEF.sides = atoi(s.substr(s.find("d") + 1).c_str());
			return 1;
		}else{
			return 0;
		}
	}else if(s.substr(0,5) == "SPEED")
	{
		if(SPEED.base == INT_MAX && SPEED.dice == INT_MAX && SPEED.sides == INT_MAX)
		{
			SPEED.base = atoi(s.substr(6,s.find("+")).c_str());
			SPEED.dice = atoi(s.substr(s.find("+"),s.find("d")).c_str());
			SPEED.sides = atoi(s.substr(s.find("d") + 1).c_str());
			return 1;
		}else{
			return 0;
		}
	}else if(s.substr(0,4) == "ATTR")
	{
		if(ATTR.base == INT_MAX && ATTR.dice == INT_MAX && ATTR.sides == INT_MAX)
		{
			ATTR.base = atoi(s.substr(5,s.find("+")).c_str());
			ATTR.dice = atoi(s.substr(s.find("+"),s.find("d")).c_str());
			ATTR.sides = atoi(s.substr(s.find("d") + 1).c_str());
			return 1;
		}else{
			return 0;
		}
	}else if(s.substr(0,3) == "VAL")
	{
		if(VAL.base == INT_MAX && VAL.dice == INT_MAX && VAL.sides == INT_MAX)
		{
			VAL.base = atoi(s.substr(4,s.find("+")).c_str());
			VAL.dice = atoi(s.substr(s.find("+"),s.find("d")).c_str());
			VAL.sides = atoi(s.substr(s.find("d") + 1).c_str());
			return 1;
		}else{
			return 0;
		}
	}else if(s.substr(0,6) == "WEIGHT")
	{
		if(WEIGHT.base == INT_MAX && WEIGHT.dice == INT_MAX && WEIGHT.sides == INT_MAX)
		{
			WEIGHT.base = atoi(s.substr(7,s.find("+")).c_str());
			WEIGHT.dice = atoi(s.substr(s.find("+"),s.find("d")).c_str());
			WEIGHT.sides = atoi(s.substr(s.find("d") + 1).c_str());
			return 1;
		}else{
			return 0;
		}
	}else if(s.substr(0,3) == "DAM")
	{
		if(DAM.base == INT_MAX && DAM.dice == INT_MAX && DAM.sides == INT_MAX)
		{
			DAM.base = atoi(s.substr(4,s.find("+")).c_str());
			DAM.dice = atoi(s.substr(s.find("+"),s.find("d")).c_str());
			DAM.sides = atoi(s.substr(s.find("d") + 1).c_str());
			return 1;
		}else{
			return 0;
		}
	}else if(s.substr(0,4) == "RRTY")
	{
		if(objRRTY == 101)
		{
			objRRTY = atoi(s.substr(5).c_str());
				return 1;
		}else{
			return 0;
		}
	}
	return -1;
}

void resetObjTraits()
{
HIT.base = INT_MAX, HIT.dice = INT_MAX, HIT.sides = INT_MAX;
DODGE.base = INT_MAX, DODGE.dice = INT_MAX, DODGE.sides = INT_MAX;
DAM.base = INT_MAX, DAM.dice = INT_MAX, DAM.sides = INT_MAX;
DEF.base = INT_MAX, DEF.dice = INT_MAX, DEF.sides = INT_MAX;
SPEED.base = INT_MAX, SPEED.dice = INT_MAX, SPEED.sides = INT_MAX;
ATTR.base = INT_MAX, ATTR.dice = INT_MAX, ATTR.sides = INT_MAX;
VAL.base = INT_MAX, VAL.dice = INT_MAX, VAL.sides = INT_MAX;
WEIGHT.base = INT_MAX, WEIGHT.dice = INT_MAX, WEIGHT.sides = INT_MAX;
objRRTY = 101;
objNAME = "", objDESC = "", objCOLOR = "", TYPE = "", ART = "";
}

void parseObjects()
{
	string s;
	char * path = (char *) malloc(strlen(getenv("HOME")) + strlen("/.rlg327/object_desc.txt") + 1);
	strcpy(path, getenv("HOME"));
	strcat(path,"/.rlg327/object_desc.txt");
	fstream f(path);
	free(path);
	
	int valObj = 1;
	
	getline(f,s);
	if(s == "RLG327 OBJECT DESCRIPTION 1")
	{
		while(!f.eof())
		{
		valObj = 1;
		getline(f,s);
		if(s == "BEGIN OBJECT")
		{
			resetObjTraits();
			while(s != "END")
			{
				getline(f,s);
				if(s == "BEGIN OBJECT")
				{
					resetTraits();
				}
				if(s.length() > 1)
				{
					if(checkObjString(s) == 0)
					{
						valObj = 0;
						break;
					}
				}
				if(s == "DESC")
				{
					if(objDESC == "")
					{
						getline(f,s);
						while(s != ".")
						{
							objDESC = objDESC + s + "\n";
							getline(f,s);
						}
			
					}else{
						valObj = 0;
						break;
					}
				}
			}
			if(isValidObj() == 1 && valObj == 1)
			{	  
				objectDescription description;
	
				description.name = objNAME;
				description.desc = objDESC;
				description.type = TYPE;
				description.color = objCOLOR;
				description.art = ART;
				description.hit = HIT;
				description.dam = DAM;
				description.dodge = DODGE;
				description.def = DEF;
				description.weight = WEIGHT;
				description.speed = SPEED;
				description.attr = ATTR;
				description.val = VAL;
				description.rarity = objRRTY;
				
				objTemplates.push_back(description);
			}
		}
		
		}
		
	}else{
		cout << "INCORRECT FILE FORMAT" << endl;
	}
}

void generateObjects(int objectCount, vector<object> &objects)
{
	parseObjects();
	int generatedObjects = 1;
	
	while(generatedObjects <= objectCount)
	{
		int eligibleObj = 0;
		int randTemplate = rand() % (objTemplates.size() - 1) + 1;
		int i;
		for(i = 0; i < (signed) objects.size(); i++)
		{
			if(objTemplates[randTemplate].name == objects[i].name && (signed) objTemplates[randTemplate].art.find("TRUE") != -1)
			{
				eligibleObj = 1;
			}
		}
		
		int randRarity = rand() % 99;
		if(randRarity >= objTemplates[randTemplate].rarity)
		{
			eligibleObj = 1;
		}	
		
		if(eligibleObj == 0)
		{
				
		generatedObjects++;
			
		object obj;
		obj.name = objTemplates[randTemplate].name;
		obj.desc = objTemplates[randTemplate].desc;
		obj.hit = objTemplates[randTemplate].hit.rollDice();
		obj.dam = objTemplates[randTemplate].dam;
		obj.dodge = objTemplates[randTemplate].dodge.rollDice();
		obj.def = objTemplates[randTemplate].def.rollDice();
		obj.weight = objTemplates[randTemplate].weight.rollDice();
		if(objTemplates[randTemplate].speed.base < 0)
		{
			obj.speed = objTemplates[randTemplate].speed.base;
		}else{
			obj.speed = objTemplates[randTemplate].speed.rollDice();
		}
		
		obj.attr = objTemplates[randTemplate].attr.rollDice();
		obj.val = objTemplates[randTemplate].val.rollDice();
		obj.rarity = objTemplates[randTemplate].rarity;
		obj.isPickedUp = 0;
		obj.isDestroyed = 0;
		
		
		//ART ASSIGNING
		if(objTemplates[randTemplate].art.find("TRUE") == std::string::npos)
		{
			obj.art = 0;
		}else{
			obj.art = 1;
		}
		
		//SYMB ASSIGNING
		
		if(objTemplates[randTemplate].type.find("WEAPON") != std::string::npos)
		{
			obj.symb = '|';
		}else if(objTemplates[randTemplate].type.find("OFFHAND") != std::string::npos)
		{
			obj.symb = ')';
		}else if(objTemplates[randTemplate].type.find("RANGED") != std::string::npos)
		{
			obj.symb = '}';
		}else if(objTemplates[randTemplate].type.find("ARMOR") != std::string::npos)
		{
			obj.symb = '[';
		}else if(objTemplates[randTemplate].type.find("HELMET") != std::string::npos)
		{
			obj.symb = ']';
		}else if(objTemplates[randTemplate].type.find("CLOAK") != std::string::npos)
		{
			obj.symb = '(';
		}else if(objTemplates[randTemplate].type.find("GLOVES") != std::string::npos)
		{
			obj.symb = '{';
		}else if(objTemplates[randTemplate].type.find("BOOTS") != std::string::npos)
		{
			obj.symb = '\\';
		}else if(objTemplates[randTemplate].type.find("RING") != std::string::npos)
		{
			obj.symb = '=';
		}else if(objTemplates[randTemplate].type.find("AMULET") != std::string::npos)
		{
			obj.symb = '"';
		}else if(objTemplates[randTemplate].type.find("LIGHT") != std::string::npos)
		{
			obj.symb = '_';
		}else if(objTemplates[randTemplate].type.find("SCROLL") != std::string::npos)
		{
			obj.symb = '~';
		}else if(objTemplates[randTemplate].type.find("BOOK") != std::string::npos)
		{
			obj.symb = '?';
		}else if(objTemplates[randTemplate].type.find("FLASK") != std::string::npos)
		{
			obj.symb = '!';
		}else if(objTemplates[randTemplate].type.find("GOLD") != std::string::npos)
		{
			obj.symb = '$';
		}else if(objTemplates[randTemplate].type.find("AMMUNITION") != std::string::npos)
		{
			obj.symb = '/';
		}else if(objTemplates[randTemplate].type.find("FOOD") != std::string::npos)
		{
			obj.symb = ',';
		}else if(objTemplates[randTemplate].type.find("WAND") != std::string::npos)
		{
			obj.symb = '-';
		}else if(objTemplates[randTemplate].type.find("CONTAINER") != std::string::npos)
		{
			obj.symb = '%';
		}
		
	
		//COLOR ASSIGNING
		string colorVal = objTemplates[randTemplate].color.substr(0,3);
		if(colorVal[0] == 'R')
		{
			obj.color = COLOR_RED;
			
		}else if(colorVal[0] == 'G')
		{
			obj.color = COLOR_GREEN;
			
		}else if(colorVal[0] == 'C')
		{
			obj.color = COLOR_CYAN;
		}else if(colorVal[0] == 'Y')
		{
			obj.color = COLOR_YELLOW;
			
		}else if(colorVal[0] == 'M')
		{
			obj.color = COLOR_MAGENTA;
		}else if(colorVal[0] == 'W')
		{
			obj.color = COLOR_WHITE;
			
		}else if(colorVal[0] == 'B' && colorVal[2] == 'A')
		{
			obj.color = COLOR_BLACK;
			
		}else if(colorVal[0] == 'B' && colorVal[2] == 'U')
		{
			obj.color = COLOR_BLUE;
			
		}
		
		objects.push_back(obj);
		
		}
		
		
	}
}
