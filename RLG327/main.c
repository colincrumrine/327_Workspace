#include "generateDungeon.h"
#include<time.h>

int main(int argc, char *argv[])
{
	srand(time(0));
	generateDungeon(argv,0,0);
	return 0;
}