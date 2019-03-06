#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gallery.h"
#include <ncurses.h>

using namespace std;

void mainGallery()
{
	
	while(1)
	{
	
	int isDone = 0;
	
	clear();
	refresh();
	
	string mainGalleryMenu = "";
	mainGalleryMenu.append("WELCOME TO THE GALLERY!\nHere, you can check out some cool Tron style\nconcept art sketches I found and converted to ASCII art.\n\n");
	mainGalleryMenu.append("To view a picture, press the corresponding number key.\n\n");
	mainGalleryMenu.append("1 ===> ""Lone Program""\n\n");
	mainGalleryMenu.append("2 ===> ""The Grid""\n\n");
	mainGalleryMenu.append("3 ===> ""Light Cycle""\n\n");
	mainGalleryMenu.append("4 ===> ""Retro TitleCard""\n\n");
	mainGalleryMenu.append("5 ===> ""80's Tron Movie Poster""\n\n");
	mainGalleryMenu.append("To exit the gallery, press the escape key.");
	
	printw("%s", mainGalleryMenu.c_str());
	
	
	int userInput = 0;
	while(userInput != 27 && userInput != '1' && userInput != '2' && userInput != '3' && userInput != '4' && userInput != '5')
	{
		userInput = getch();
	}
	
	int pictureUserInput = 0;
	switch(userInput)
	{
		case 27:
		
		isDone = 1;
		
		break;
		
		//START CASE 1
		case '1':
		{
		clear();
		refresh();
		
		ifstream file( "tron_concept1.txt" );
		stringstream buffer;
		buffer << file.rdbuf();
		file.close();
		
		init_pair(1, COLOR_RED , COLOR_BLACK);		
		attron(COLOR_PAIR(1));
		printw("%s", buffer.str().c_str());
		attroff(COLOR_PAIR(1));
		
		while(pictureUserInput != 27)
		{
			pictureUserInput = getch();
		}
		
		clear();
		refresh();
		}
		break;
		//END CASE 1
		
		//START CASE 2
		case '2':
		{
		clear();
		refresh();
		
		ifstream file2( "tron_concept2.txt" );
		stringstream buffer2;
		buffer2 << file2.rdbuf();
		file2.close();
		
		init_pair(2, COLOR_MAGENTA , COLOR_BLACK);		
		attron(COLOR_PAIR(2));
		printw("%s", buffer2.str().c_str());
		attroff(COLOR_PAIR(2));
		
		while(pictureUserInput != 27)
		{
			pictureUserInput = getch();
		}
		
		clear();
		refresh();
		}
		break;
		//END CASE 2
		
		//START CASE 3
		case '3':
		{
		clear();
		refresh();
		
		ifstream file2( "tron_concept3.txt" );
		stringstream buffer2;
		buffer2 << file2.rdbuf();
		file2.close();
		
		init_pair(2, COLOR_BLUE , COLOR_BLACK);		
		attron(COLOR_PAIR(2));
		printw("%s", buffer2.str().c_str());
		attroff(COLOR_PAIR(2));
		
		while(pictureUserInput != 27)
		{
			pictureUserInput = getch();
		}
		
		clear();
		refresh();
		}
		break;
		//END CASE 3
		
		//START CASE 4
		case '4':
		{
		clear();
		refresh();
		
		ifstream file2( "tron_concept4.txt" );
		stringstream buffer2;
		buffer2 << file2.rdbuf();
		file2.close();
		
		init_pair(2, COLOR_YELLOW , COLOR_BLACK);		
		attron(COLOR_PAIR(2));
		printw("%s", buffer2.str().c_str());
		attroff(COLOR_PAIR(2));
		
		while(pictureUserInput != 27)
		{
			pictureUserInput = getch();
		}
		
		clear();
		refresh();
		}
		break;
		//END CASE 4
		
		//START CASE 5
		case '5':
		{
		clear();
		refresh();
		
		ifstream file2( "tron_concept5.txt" );
		stringstream buffer2;
		buffer2 << file2.rdbuf();
		file2.close();
		
		init_pair(2, COLOR_CYAN , COLOR_BLACK);		
		attron(COLOR_PAIR(2));
		printw("%s", buffer2.str().c_str());
		attroff(COLOR_PAIR(2));
		
		while(pictureUserInput != 27)
		{
			pictureUserInput = getch();
		}
		
		clear();
		refresh();
		}
		break;
		//END CASE 5
	}
	
	if(isDone == 1)
	{
		break;
	}
	
	}
}