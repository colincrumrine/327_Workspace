#ifndef ITEMPARSER_H
# define ITEMPARSER_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<string.h>  
#include <stdint.h>
#include <endian.h>
#include <vector> 
#include <ncurses.h>
#include "game.h"
#include "monsterParser.h"

using namespace std;

int isValidObj();

void printObjDesc();

int checkObjString(string s);

void resetObjTraits();

void parseObject();

void generateObjects(int objectCount, vector<class object> &objects);

# endif
