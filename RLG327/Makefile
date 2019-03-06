main:main.o generateDungeon.o heap.o monsterMap.o switches.o game.o gameUtil.o monsterParser.o itemParser.o
	g++ -g -Wall main.o generateDungeon.o heap.o monsterMap.o switches.o game.o gameUtil.o monsterParser.o itemParser.o -lncurses -lstdc++ -g -o main

main.o:main.cpp
	g++ -c main.cpp -Wall -o main.o
	
heap.o:heap.cpp
	g++ -c heap.cpp -Wall -o heap.o
	
generateDungeon.o:generateDungeon.cpp
	g++ -c generateDungeon.cpp -Wall -o generateDungeon.o

monsterMap.o:monsterMap.cpp
	g++ -c monsterMap.cpp -Wall -o monsterMap.o

switches.o:switches.cpp
	g++ -c switches.cpp -Wall -o switches.o

game.o:game.cpp
	g++ -c game.cpp -Wall -o game.o

gameUtil.o:gameUtil.cpp
	g++ -c gameUtil.cpp -Wall -o gameUtil.o


monsterParser.o:monsterParser.cpp
	g++ -c monsterParser.cpp -Wall -o monsterParser.o

itemParser.o:itemParser.cpp
	g++ -c itemParser.cpp -Wall -o itemParser.o

clean:
	rm -f main monsterMap.o heap.o mainDungeon.o switches.o game.o generateDungeon.o main.o gameUtil.o monsterParser.o itemParser.o

