//A simple linux shell

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define	DELAY	2
#define MAX_ARGSLEN 1000
#define MAX_ARGLEN 25
#define MAX_ARGS 100

void free_args(char *list[]) {
	for(; *list != NULL; list++)
		free(*list);
}

void printargs(char *list[]) {
	for(; *list != NULL; list++)
		printf("%s ", *list);
	printf("\n");
}

int count_spaces(char *string) {
	int count = 0;
	for(; *string != '\0'; string++) 
		if(*string == ' ')
			count++;
	return count;
}

void parse_input(char *string1, char **arglist ) {

	char string2[MAX_ARGLEN];
	char *buf;
	int i = 0;
	int j = 0;
	int k = 0;
	//remove leading whitespace	
	for(; (string1[i] == ' ') || (string1[i] == '\t'); i++)
		;
	while( string1[i] != '\n' ) {
		//read the arg until whitespace is found (or newline)
		while ( (string1[i] != ' ') && (string1[i] != '\t') && (string1[i] != '\n') ) 
			string2[j++] = string1[i++];
		string2[j] = '\0';

		//copy the arg to the arglist
		buf = malloc(j+1);	//allocate j+1 bytes to buf
		strcpy(buf, string2); //copies j+1 bytes from string2 to buf
		arglist[k++] = buf;  //add buf to arglist
		//increment i past whitespace	
		for(; (string1[i] == ' ') || (string1[i] == '\t'); i++)
			;
		j = 0;
	}
	arglist[k] = NULL;
}

void execute (char **arglist ) {
	int pid;
	int exitstatus;
	
	pid = fork();
	switch (pid) {
		case -1:
			perror("fork failed");
			exit(1);
		case 0:
			execvp(arglist[0], arglist);
			perror("execvp failed");
			exit(1);
		default:
			while (wait(&exitstatus) != pid) 
				;
			printf("Child exited with status %d %d\n", exitstatus>>8, exitstatus&0x0377);
			
	}
}

void main()
{
	char *arglist[MAX_ARGS];
	char args[MAX_ARGSLEN];

	while(1) {
		//read input from the user
		if (fgets(args, MAX_ARGSLEN, stdin) == NULL)
			perror("fgets");
		else {	
			//separate args and put in args array
			parse_input(args, arglist);

			//printargs(arglist);
			execute(arglist);
		}
		//printargs(arglist);
		free_args(arglist);
	}
	


}


