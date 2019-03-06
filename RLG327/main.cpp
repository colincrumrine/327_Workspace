#include "generateDungeon.h"
#include<time.h>
#include <cstdlib>
#include "itemParser.h"
#include <vector>
#include "game.h"

int main(int argc, char *argv[])
{
	int i,j;
	object* carry[10];
	object* equipment[12];
	
	for(i = 0; i < 10; i++)
	{
		carry[i] = NULL;
	}
	
	for(j = 0; j < 12; j++)
	{
		equipment[j] = NULL;
	}
	
	srand(time(0));
	generateDungeon(argv,0,0,carry,equipment,200000);
	return 0;
}
