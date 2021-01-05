#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define FALSE 0
#define TRUE 1

#define EOL	1
#define ARG	2
#define AMPERSAND 3
#define OUTRD	4
#define INRD 	5
#define PIPE 	6

#define FOREGROUND 0
#define BACKGROUND 1

static char	input[512];
static char	tokens[1024];
char		*ptr, *tok;

int get_token(char **outptr)
{
	int	type;

	*outptr = tok;
	while ((*ptr == ' ') || (*ptr == '\t')) ptr++;

	*tok++ = *ptr;

	switch (*ptr++) {
		case '\0' : type = EOL; break;
		case '&' : type = AMPERSAND; break;
		case '>' : type = OUTRD; break;		
		case '<' : type = INRD; break;
		case '|' : type = PIPE; break; 
		default : type = ARG;
			while ((*ptr != ' ') && (*ptr != '&') &&
				(*ptr != '\t') && (*ptr != '\0') && 
				(*ptr != '>') && (*ptr != '<') && (*ptr != '|'))
				*tok++ = *ptr++;
	}
	*tok++ = '\0';
	return(type);
}

int execute(char **comm, int how)
{
	int	pid;

	if ((pid = fork()) < 0) {
		fprintf(stderr, "minish : fork error\n");
		return(-1);
	}
	else if (pid == 0) {
		execvp(*comm, comm);
		fprintf(stderr, "minish : command not found\n");
		exit(127);
	}
	if (how == BACKGROUND) {	/* Background execution */
		printf("[%d]\n", pid);
		return 0;
	}		
	/* Foreground execution */
	while (waitpid(pid, NULL, 0) < 0)
		if (errno != EINTR) return -1;
	return 0;
}

// OUTRD(>) execute function
int execute_outrd(char **lcomm, char **rcomm, int how)
{
	int	pid1;
	int	fd;

	if ((pid1 = fork()) < 0) {
		fprintf(stderr, "minish : fork error\n");
		return(-1);
	}
	else if (pid1 == 0) {
		fd = open(*rcomm, O_RDWR | O_CREAT | S_IROTH, 0644); 
		if (fd < 0) {
			perror("error");
			exit(-1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		execvp(*lcomm, lcomm);
		fprintf(stderr, "minish : command not found\n");
		exit(0);
	}	

	wait();		/* parent */
}

// INRD(<) execute function
int execute_inrd(char **lcomm, char **rcomm, int how)
{
	int	pid1;
	int	fd;

	if ((pid1 = fork()) < 0) {
		fprintf(stderr, "minish : fork error\n");
		return(-1);
	}
	else if (pid1 == 0) { /* child */
		fd = open(*rcomm, O_RDONLY);
		if (fd < 0) {
			perror("error");
			exit(-1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
		execvp(*lcomm, lcomm);
		fprintf(stderr, "minish : command not found\n");
		exit(0);
	}	

	wait();		/* parent */
}

// PIPE(|) execute function
int execute_pipe(char **lcomm, char **rcomm, int how)
{
	int	p[2];
	int 	pid1, pid2;
	
	if ((pid1 = fork()) < 0) {
		fprintf(stderr, "minish : fork error\n");
		return(-1);
	}
	else if (pid1 == 0) { /* first child */
		pipe(p);
		pid2 = fork();
		if (pid2 == 0) { /* second child */
			dup2(p[0], STDIN_FILENO);
			close(p[0]);
			close(p[1]);
			execvp(*rcomm, rcomm);
			fprintf(stderr, "minish : command not found\n");
			exit(127);
		}
		dup2(p[1], STDOUT_FILENO);
		close(p[0]);
		close(p[1]);
		execvp(*lcomm, lcomm);
		fprintf(stderr, "minish : command not found\n");
		exit(0);
	}

	wait();		/* parent */
}

int parse_and_execute(char *input)
{
	char	*arg[1024];	// command before OUTRD(>), INRD(<), PIPE(|) from the start
	char 	*aarg[1024];	// command after OUTRD(>), INRD(<), PIPE(|)
	int	type, how;
	int	quit = FALSE;
	int	narg = 0;
	int 	naarg = 0;
	int	finished = FALSE;
	int 	outrd_mark = 0;
	int 	inrd_mark = 0;
	int	pipe_mark = 0;

	ptr = input;
	tok = tokens;

	while (!finished) {
		if (outrd_mark | inrd_mark | pipe_mark)	type = get_token(&aarg[naarg]);
		else	type = get_token(&arg[narg]);

		switch (type) {
		case ARG :
			if (outrd_mark | inrd_mark | pipe_mark)	naarg++;
			else	narg++;
			break;
		case OUTRD :
			outrd_mark = 1;
			break;
		case INRD :
			inrd_mark = 1;
			break;
		case PIPE :
			pipe_mark = 1;
			break;
		case EOL :
		case AMPERSAND:
			if (!strcmp(arg[0], "quit")) quit = TRUE;
			else if (!strcmp(arg[0], "exit")) quit = TRUE;
			else {
				how = (type == AMPERSAND) ? BACKGROUND : FOREGROUND;
				arg[narg] = NULL; 
				if (outrd_mark | inrd_mark | pipe_mark)	aarg[naarg] = NULL; // append NULL
				if (narg != 0) {
					if (outrd_mark)	execute_outrd(arg, aarg, how);
					else if (inrd_mark)	execute_inrd(arg, aarg, how);
					else if (pipe_mark)	execute_pipe(arg, aarg, how);
					else	execute(arg, how);
				}	
			}
			narg = 0;
			if (type == EOL)
				finished = TRUE;
			break; 
		}
	}
	return quit;
}

main()
{
    	char 	*arg[1024];
	int 	quit;

	printf("msh # ");
	while (gets(input)) {
		quit = parse_and_execute(input);
		if (quit) break;
		printf("msh # ");
	}
}


