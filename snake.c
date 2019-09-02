/* Snake Game 
by Mike Stapleton 6/20/19

heavy use of linked lists, 
*/

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define RIGHT 1 //one move on the x axis is 2 spaces left/right
#define LEFT -1 
#define UP -1 //one move on the y axis is 1 space up/down
#define DOWN 1
#define XDELAY 50000
#define YDELAY 90000 //movement on y axis is faster so we need more delay
#define SECTION_LEN 6
#define NUM_FOOD 20
#define HIGHSCORE_FILE "./highscore.dat"


//A section of the snake
struct Section {
	int x, y;
	struct Section *next;
};

struct Food {
	int x, y;
	struct Food *next;
};

//Add a section of snake that has eaten, return new tail 
struct Section * addSection(struct Section *tail) {
	struct Section *current = tail;	
	for(int i=0; i<SECTION_LEN; i++) {
		current->next = malloc(sizeof(struct Section));
		current=current->next;
	}

	current->next =NULL;
	return current;

	
}


void print_border (int brows, int bcols) {
	for ( int i=0; i< bcols; i++ ) 
		mvprintw(0, i, "-");
	for ( int i=0; i< bcols; i++ ) 
		mvprintw(brows-1, i, "-");
	for ( int i=1; i< brows-1; i++ ) 
		mvprintw(i, 0, "|");
	for ( int i=1; i< brows-1; i++ ) 
		mvprintw(i, bcols-1, "|");
}

void free_memory(struct Section *head, struct Food *start) {

	struct Section *current_snake = head;
	struct Food *current_food = start;

	struct Section *tmps;
	struct Food *tmpf;

	while (current_food != NULL) {
		tmpf = current_food;
		current_food=current_food->next;
		free(tmpf);
	}
	while (current_snake != NULL) {
		tmps = current_snake;
		current_snake=current_snake->next;
		free(tmps);
	}

}

void print_snake(struct Section *head) {
	struct Section *current = head;
	while (current != NULL) {
		mvprintw(current->y, current->x, "o");
	        current = current->next;
	}
}

//Update the position of all the segments
void update_snake(struct Section *head, int xdir, int ydir) {
	struct Section *current = head;	
	
	//we need 2 temporary storages, 1 for the current
	//values and 1 for the previous
	int temp1x, temp1y, temp2x, temp2y;

	//calculate the new position of the head	
	temp1x = current->x + xdir;
	temp1y = current->y + ydir;

	while (current != NULL) {
		//save the value of the section you're on
		temp2x = current->x;
		temp2y = current->y;
		
		//put the previous value in current section
		current->x = temp1x;
		current->y = temp1y;

		//save the current value for the next section
		temp1x = temp2x;
		temp1y = temp2y;
		
		current = current->next;
	}
}
bool check_collision_border(struct Section *head, int rows, int cols) {
	if(head->x >= (cols-1) || head->x <= 0 ||
		head->y >=(rows-1) || head->y <= 0)
		return TRUE;
	else
		return FALSE;
}

bool check_collision_self(struct Section *head) {
	struct Section *current = head->next;
	while (current != NULL) {
		if(head->x == current->x && head->y == current->y) 
			return TRUE;
		else
			current = current->next;
	}
	return FALSE;
}

//Check entire food array if one has been eaten
bool check_food (struct Food *start, struct Section *head,
		int screen_rows, int screen_cols) {
	struct Food *food = start;	
	time_t t;
	int x = head->x;
	int y = head->y;	
	srand((unsigned)time(&t));
	while(food != NULL) {
	//if one food is eaten, change it's x, y
		if(food->x == x && food->y == y) {
			food->x = (rand() % (screen_cols-2)) + 1;
			food->y = (rand() % (screen_rows-2)) + 1;
			return TRUE;
		}
		food = food->next;
	}


}



//Print food array, making sure not print on the snake
void print_food(struct Food *start) {
	struct Food *current = start;
	while (current != NULL) {
		mvprintw(current->y, current->x, "X");
	        current = current->next;
	}
}


void create_food (struct Food *start, int screen_rows, int screen_cols) {
	struct Food *current = start;	
	time_t t;
	srand((unsigned)time(&t));

	for(int i=0; i<NUM_FOOD-1; i++) {
		current->next= malloc(sizeof(struct Section));
		current=current->next;
		current->x = (rand() % (screen_cols-2)) + 1;
		current->y = (rand() % (screen_rows-2)) + 1;
		
		
	}

	current->next =NULL;

}

//saves highscore, returns current highscore
int set_highscore(int cur_score) {
	int prev_score;
	FILE *fp;

	fp = fopen(HIGHSCORE_FILE, "r");
	if(fp) {  
		//file exists
		fclose(fp);
		//open for reading and writing
		fp = fopen(HIGHSCORE_FILE, "r+");
		if(fp == NULL) {
			printf("Error opening %s in read/write mode\n", HIGHSCORE_FILE);
			exit(0);
		}
		prev_score = getc(fp);
		if(cur_score > prev_score) {
			fseek(fp, 0L, SEEK_SET);
			putc(cur_score, fp);
		}
		else {
			cur_score = prev_score;
		}
		fclose(fp);
	}
	else {
		//file doesn't exist (or there was an error)
		//try to create the file 
		fp = fopen(HIGHSCORE_FILE, "w");
		if(fp == NULL) {
			printf("Error creating file %s\n", HIGHSCORE_FILE);
			exit(0);
		}
		putc(cur_score, fp);
		fclose(fp);
	}
	return cur_score;
}

int main () {
		
	int screen_rows, screen_cols;  //size of screen
	int ydirection = 0; 
	int xdirection = RIGHT; //snake starts out going right
	int c;    
	int delay = XDELAY; //snake start out with x axis delay speed
	int score = 0;

	bool food_found = FALSE;

	//Head and Tail of the snake
	struct Section *head, *tail;
	struct Food *start;
	struct Food *current;

	//nurses initializations
	initscr();
	noecho();
	curs_set(FALSE);
	getmaxyx(stdscr, screen_rows, screen_cols);
	cbreak();	
	nodelay(stdscr, TRUE);

		
	
	while(1) {
		
		
		//create head pointer and it's starting coordinates
		head = malloc(sizeof(struct Section));
		//create food pointer
		start = malloc(sizeof(struct Food));
		head->x = 20;
		head->y = 20;
		start->x = 30;
		start->y = 22;
		start->next = NULL;
		//create the food linked list
		create_food(start, screen_rows, screen_cols);

		head->next = NULL;
		
		tail = head;       //starts out just a one char head
		
		while(1) {
			clear();
			//if eats food, redraw food and add a section
			if(food_found = check_food(start, head, screen_rows, screen_cols)) {
				score++;
				food_found = FALSE;
				tail = addSection(tail);
			}

		        if ((c = getch()) != ERR) {
				switch(c) {        //user input
					case 119:
						ydirection = UP;
						xdirection = 0;
						delay = YDELAY; 
						break;
					case 115:
						ydirection = DOWN;
						xdirection = 0;
						delay = YDELAY;
						break;
					case 100:
						xdirection = RIGHT;
						ydirection = 0;
						delay = XDELAY;
						break;
					case 97:
						xdirection = LEFT;
						ydirection = 0;
						delay = XDELAY;
						break;
					default:
						break;
				}
			} 
	
			//update snake position x and y 
			update_snake(head, xdirection, ydirection);
	
			
			//check if head touches border or snake
			if(check_collision_border(head, screen_rows, 	screen_cols))
				break;
			if(check_collision_self(head))
				break;
			
			print_border(screen_rows, screen_cols); //print the 	border
			
			print_snake(head); //print the snake
			print_food(start); //print the food
	
			refresh();		
			usleep(delay);
	
	
		}
		clear();
		nodelay(stdscr, FALSE);
		mvprintw(5, 10, "Score: %d", score);
		mvprintw(10, 10, "High Score: %d", set_highscore(score));
		mvprintw(15, 10, "Do you want to play again? y/n");
		while((c=getch()) != ERR) {
			switch (c) {
				case 'y': goto again;
				case 'Y': goto again;
				case 'n': goto end;
				case 'N': goto end;
				default: break;
			}
		}
		
		again:	//play again
		free_memory(head, start);
		nodelay(stdscr, TRUE);
		ydirection = 0; 
		xdirection = RIGHT; //snake starts out going right

		delay = XDELAY; //snake start out with x axis delay speed

		score = 0;

	}
	end: //play no more
	current = start;
	
	endwin();

	free_memory(head, start);
	return 0;
	
}

