#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

//read a file

#define ROWS 24
#define COLS 80

int main (int argc, char *argv[]) {
	struct X {
		int y;
		int x;
		bool printed;
	} ;

	int screen_rows = ROWS;
	int screen_cols = COLS;
	int rows = 0, cols = 0;
	int c = 0;
	int d;
	int i = 0;
	int j = 0;
	int count = 0;
	int delay = 20000;
	time_t t;
	srand((unsigned)time(&t));
	initscr();	
	start_color();
	curs_set(FALSE);
	keypad(stdscr, TRUE);
//	noecho();

	init_pair(1, COLOR_RED, COLOR_BLACK);
	attron(COLOR_PAIR(1));

	//create 2d array to hold text file
	int screen[ROWS][COLS] = {{0}};

	FILE *fp;
	if(argc !=2) {
		printf("Need a filename\n");
		exit(0);
	}
	if( (fp = fopen(argv[1], "r")) == NULL) {
		printf("%s cannot be opened\n", argv[1]);
		exit(0);
	}

	//put an X whereever there is an X in the file, randomly
	
	//generate a random location on the screen
	//if the location has an X in the file, print an X at that location

	//read the file into a 2d array	
	for( i=0; i<ROWS; i++) {
		for( j=0; j< COLS; j++) {
			c=getc(fp);
			screen[i][j] = c;
			if(c == 'X')
				count++;
		}
		c = getc(fp); //discard the newline from the file
	}

	//read all the Xs and put their coordinates in array of x,ys
	struct X x_arr[count];	
	int count2 = 0;
	for( i=0; i<ROWS; i++) {
		for( j=0; j< COLS; j++) {
			if(screen[i][j] == 'X') {
				struct X x = {i, j, FALSE};
				x_arr[count2] = x;
				count2++;
			}

		}
	}

	//make a structure that has coordinates of an X
	//make an array of structures for each X
	//count number of Xs
	//make a variable array of number of Xs 

	//randomly put the characters on the screen

	int cur;
	cbreak();
	nodelay(stdscr, TRUE);
	while( c != 'q' ) {
		c = getch();
		cur = rand() % count;
		if(x_arr[cur].printed == FALSE) {
			x_arr[cur].printed = TRUE;
			move(x_arr[cur].y, x_arr[cur].x);
			addch(ACS_CKBOARD | A_BOLD);
//			mvprintw(x_arr[cur].y, x_arr[cur].x, "%c", (ACS_CKBOARD | A_BOLD)); 
			usleep(delay);
			refresh();
		}
	}
		
	
	endwin();

	return 0;
	
}
