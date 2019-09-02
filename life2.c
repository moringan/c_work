#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

#define COLS 5
#define ROWS 5
#define ALIVE 1
#define DEAD 0
void print_grid ( int (*array)[COLS] ) {
	for(int i=0; i<ROWS; i++) 
		for(int j=0; j< COLS; j++)
			mvprintw(i, j, "%d", array[i][j]);

}

void reverse_grid ( int (*array)[COLS] ) {
	for(int i=0; i<ROWS; i++) 
		for(int j=0; j< COLS; j++)
			if(array[i][j] == 0)
				array[i][j] = 1;
			else if (array[i][j] == 1)
				array[i][j] = 0;

}

void initialize_grid ( int (*array)[COLS] ) {
	for(int i=0; i<ROWS; i++) 
		for(int j=0; j< COLS; j++)
			array[i][j] = 0;
}

void randomize_grid( int (*array)[COLS] ) {
	time_t t;
	srand((unsigned)time(&t));
	for(int i=0; i<ROWS; i++) 
		for(int j=0; j< COLS; j++)
			array[i][j] = (rand() % 2);
}

void copy_grid( int (*from)[COLS], int (*to)[COLS] ) {

	for(int i=0; i<ROWS; i++) 
		for(int j=0; j< COLS; j++)
			to[i][j] = from[i][i];
}

void new_state( int (*array1)[COLS], int (*array2)[COLS] ) {
	//count number of neighbors for each living cell
	//and make new grid
	int count=0;
	for(int y=0; y<ROWS; y++) 
		for(int x=0; x< COLS; x++) {
			for(int y1 = -1; y1<= 1; y1++)
				for(int x1 = -1; x1<= 1; x1++)
					if( !((x1==0) && (y1==0)) )  //dont't count the cell itself!
/* check if corner/edge */		if( ((x+x1)<COLS) && ((y+y1)<ROWS) && ((x+x1)>=0) && ((y+y1)>=0) )
/* check if neighbor is alive */		if( array1[(y + y1)][(x + x1)] == ALIVE  )
							count++;
			if ( (array2[y][x] == ALIVE) && (count < 2) )
				array2[y][x] = DEAD;
			if ( (array2[y][x] == ALIVE) && ((count == 2) || (count == 3)) )
				array2[y][x] = ALIVE;	
			if ( (array2[y][x] == ALIVE) && (count > 3) )
				array2[y][x] = DEAD;
			if ( (array2[y][x] == DEAD) && (count == 3) )
				array2[y][x] = ALIVE;
			count = 0;
		}


}	
	
	
int main () {
	int screen_rows, screen_cols;
	time_t t;
	int row, col;
	int c;
	int delay = 20000;

	int grid2[ROWS][COLS];// = {
/*	0,0,1,
	0,1,1,
	0,0,0 };*/
	int grid1[ROWS][COLS] = {
	/*0,0,1,
	0,1,1,
	0,0,0 };*/

	0,0,0,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,0,0,0 };
	
/*	int grid2[ROWS][COLS] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; */

	
	initscr();
	//cbreak();	
	//nodelay(stdscr, TRUE);	
	curs_set(FALSE);
	//randomize_grid(grid1);	
	initialize_grid(grid2);	
	print_grid(grid1);
	refresh();
	usleep(550000);
	while(1) {
		clear();
		new_state(grid1, grid2);
		print_grid(grid2);
		refresh();
		usleep(550000);
		copy_grid(grid2, grid1);
		//c = getch();
	}


	endwin();

	return 0;
	
}
