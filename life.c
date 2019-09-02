#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define DELAY 120000
#define ALIVE 1
#define DEAD 0

void print_grid ( int rows, int cols, int (*array)[cols] ) {
	for(int i=0; i<rows; i++) 
		for(int j=0; j< cols; j++) {
			if(array[i][j] == DEAD)
				//mvprintw(i, j, "%d", array[i][j]);
				mvprintw(i, j, " ");
			else {
				move(i, j);
				addch(ACS_CKBOARD);
			}
		}

}

void initialize_grid ( int rows, int cols, int (*array)[cols] ) {
	for(int i=0; i<rows; i++) 
		for(int j=0; j< cols; j++)
			array[i][j] = DEAD;
}

void randomize_grid( int rows, int cols, int (*array)[cols] ) {
	time_t t;
	srand((unsigned)time(&t));
	for(int i=0; i<rows; i++) 
		for(int j=0; j< cols; j++) 
			if((rand() % 2) == 1)
				array[i][j] = ALIVE;
			else
				array[i][j] = DEAD;
}

void copy_grid( int rows, int cols, int (*from)[COLS], int (*to)[COLS] ) {

	for(int i=0; i<rows; i++) 
		for(int j=0; j< cols; j++)
			to[i][j] = from[i][j];
}

void new_state( int rows, int cols, int (*array1)[cols], int (*array2)[cols] ) {
	//count number of neighbors for each living cell
	//and make new grid
	int count=0;
	for(int y=0; y<rows; y++) 
		for(int x=0; x< cols; x++) {

			for(int y1 = -1; y1<= 1; y1++)
				for(int x1 = -1; x1<= 1; x1++)

					if( !((x1==0) && (y1==0)) )  //dont't count the cell itself!
/* check if corner/edge */		if( ((x+x1)<cols) && ((y+y1)<rows) 
						&& ((x+x1)>=0) && ((y+y1)>=0) )
/* check if neighbor is alive */		if( array1[(y + y1)][(x + x1)] == ALIVE  )
							count++;
			if ( (array1[y][x] == ALIVE) && (count < 2) )
				array2[y][x] = DEAD;
			else if ( (array1[y][x] == ALIVE) && ((count == 2) || (count == 3)) )
				array2[y][x] = ALIVE;	
			else if ( (array1[y][x] == ALIVE) && (count > 3) )
				array2[y][x] = DEAD;
			else if ( (array1[y][x] == DEAD) && (count == 3) )
				array2[y][x] = ALIVE;
			else array2[y][x] = array1[y][x];
			count = 0;
		}
}	
	
//load the file into the grid array
void load_file(FILE *fp, int rows, int cols, int (*array)[cols] ) {
	int c;
	int i = 0;
	int j = 0;
	while((c = getc(fp)) != EOF) {
		
		if(c == '0') {
			array[i][j] = DEAD;
			j++;
		}
		else if (c == '1') {
			array[i][j] = ALIVE;
			j++;
		}
		else if(c == '\n') {
			i++;
			j=0;
		}
	}
	
}

int main (int argc, char *argv[]) {
	int cols = 0;
	int rows = 0;
	int c;
	FILE *fp;

	if(argc !=2) {
		printf("Need a filename\n");
		exit(0);
	}
	if( (fp = fopen(argv[1], "r")) == NULL) {
		printf("%s cannot be opened\n", argv[1]);
		exit(0);
	}

	//count columns
	while((c = getc(fp)) != '\n') {
		if(c == '0') 
			cols++;
	}
	fseek(fp, 0, SEEK_SET);

	//count rows
	fseek(fp, 0, SEEK_SET);
	while( (c = getc(fp)) != EOF ) {
		if(c == '\n')
			rows++;
	}
	//rows = cols;
	fseek(fp, 0, SEEK_SET);
	
	//variable length 2d array
	int (*grid1)[cols] = malloc(sizeof(int[rows][cols]));
	int (*grid2)[cols] = malloc(sizeof(int[rows][cols]));

	//set grids to all zeros
	initialize_grid(rows, cols, grid1);	
	initialize_grid(rows, cols, grid2);	

	load_file(fp, rows, cols, grid1);

	//ncurses initializations
	initscr();
	curs_set(FALSE);
	start_color();

	//color pairs
	init_pair(1, COLOR_RED, COLOR_BLACK);
	attron(COLOR_PAIR(1));


	initialize_grid(rows, cols, grid2);	
	
	while(1) {
		print_grid(rows, cols, grid1);
		refresh();

		usleep(DELAY);
		clear();
		new_state(rows, cols, grid1, grid2);

		print_grid(rows, cols, grid2);
		refresh();

		usleep(DELAY);
		clear();
		new_state(rows, cols, grid2, grid1);
	}

	free(grid1);
	free(grid2);
	fclose(fp);
	endwin();

	return 0;
	
}
