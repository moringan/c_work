#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

//read a file

#define ROWS 24
#define COLS 80

int main (int argc, char *argv[]) {
	int c;

	initscr();	
	cbreak(); 		//no return needed
	curs_set(TRUE);		//show the cursor
	keypad(stdscr, TRUE);	//allow arrow keys 
	noecho();		//don't show typed characters
	start_color();		//allow color


	fclose(fp);
	endwin();

	return 0;
}
	
