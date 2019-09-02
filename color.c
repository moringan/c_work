/*
Simple Editor
*/

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

#define ROWS 24
#define COLS 80

void fill_screen_array(FILE **filep, int screen_array[ROWS][COLS]) {
	for(int i=0; i<ROWS; i++) {
		for(int j =0; j < COLS; j++) 
			screen_array[i][j] = getc(*filep);
		if(i != ROWS)
			getc(*filep); //discard the newlines
	}
	
}

void init_screen_array(int screen_array[ROWS][COLS]) {
	for(int i=0; i<ROWS; i++)
		for(int j =0; j < COLS; j++)
			screen_array[i][j] = 32;
}

void draw_screen_array(WINDOW *window, int screen_array[ROWS][COLS]) {
	for(int i=0; i<ROWS; i++)
		for(int j =0; j < COLS; j++) {
			wmove(window, i, j);
			if(screen_array[i][j] == 'X')	{
				wattron(window, COLOR_PAIR(1));
				waddch(window, ACS_CKBOARD | A_BOLD);
				wattroff(window, COLOR_PAIR(1));	
				}
		}
}

//See if file exists, if so return TRUE.  If not then create it and return FALSE
bool read_file(FILE **filep, int argc, char *argv[]) {

	if(argc !=2) {
		printf("Need a filename\n");
		exit(0);
	}

	//try to open for reading to see if it already exists
	*filep = fopen(argv[1], "r");
	if(*filep) {  
		//file exists
		fclose(*filep);
		//open for reading and writing (display on screen and write changes)
		*filep = fopen(argv[1], "r+");
		if(*filep == NULL) {
			printf("Error opening %s\n", argv[1]);
			exit(0);
		}
		return true;

	}
	else {
		//file doesn't exist (or there was an error)
		//try to create the file (and write changes later)
		*filep = fopen(argv[1], "w");
		if(*filep == NULL) {
			printf("Could not create %s\n", argv[1]);
			exit(0);
		}
		return false;

	}
}

void save_file(FILE *filep, WINDOW *window, char *argv[], int screen_array[ROWS][COLS]) {
	//Save file or just exit
	int c;
	do {
		mvwprintw(window, 0, 0, "Do you want to save (Y/n)?, \n");
		wrefresh(window);
		c = wgetch(window);
	} while ( (c != 'y') && (c != 'Y') && (c != 'n') && (c != 'N') && (c != '\n') );
	
	switch (c) {
		case 'y':  case 'Y': case '\n': 

			//write the array to the file
			fseek(filep, 0L, SEEK_SET);
			for( int i=0; i<ROWS; i++) {
				for( int j=0; j< COLS; j++) 	
					putc(screen_array[i][j], filep);
				if(i != ROWS)
					putc('\n', filep);
			}
			mvwprintw(window, 1, 0, "file %s saved\n", argv[1]);
			wrefresh(window);
			sleep(2);
			break;
		case 'n': case 'N':
			mvwprintw(window, 1, 0, "file %s NOT saved\n", argv[1]);
			wrefresh(window);
			sleep(2);
			break;
		default:
			break;

	}
	fclose(filep);
}

int main (int argc, char *argv[]) {
	int c;
	FILE *fp;

	int curs_x;
	int curs_y;
	
	//main screen array
	int screen[ROWS][COLS];
	
	if( read_file(&fp, argc, argv) == TRUE )
		//if file exists, read it into the screen array
		fill_screen_array(&fp, screen);
	else
		//initialize array with spaces
		init_screen_array(screen);


	initscr();	
	cbreak(); 		//no return needed
	curs_set(TRUE);		//show the cursor
	keypad(stdscr, TRUE);	//allow arrow keys 
	noecho();		//don't show typed characters
	start_color();		//allow color
	WINDOW *win;
	win = newwin(ROWS, COLS, 0, 0);

	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	
	draw_screen_array(win, screen);

	curs_x = 0; //(COLS / 2)-1;
	curs_y = 0; //(ROWS / 2)-1;
	wmove(win, curs_y, curs_x);

	refresh();
	wrefresh(win);

	while( (c = getch()) != 'q' ) {
		switch(c) {
			case KEY_UP:
				curs_y--;		
				wmove(win, curs_y, curs_x);
				break;
			case KEY_DOWN:
				curs_y++;
				wmove(win, curs_y, curs_x);
				break;
			case KEY_LEFT:
				curs_x--;
				wmove(win, curs_y, curs_x);
				break;
			case KEY_RIGHT:
				//mvwprintw(win, 0, 0, "Right move\n");
				curs_x++;
				wmove(win, curs_y, curs_x);
				break;
			case 'x':
			case 'X':
				wattron(win, COLOR_PAIR(1));
				waddch(win, ACS_CKBOARD | A_BOLD );
				wattroff(win, COLOR_PAIR(1));
				screen[curs_y][curs_x] = 'X';
				wmove(win, curs_y, curs_x);
				break;
			case KEY_BACKSPACE:
			case KEY_DC:
				waddch(win, 32 );
				screen[curs_y][curs_x] = 32;
				wmove(win, curs_y, curs_x);
				break;
		
			default:
				break;
		
		}
		wrefresh(win);
	}

	save_file(fp, win, argv, screen);
	endwin();

	return 0;
}
	
