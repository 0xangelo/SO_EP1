# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

# define MAX_LENGTH 1024

static char *line = (char *) NULL;
char *command = {"/bin/ls"};
char *argument[] = {"/bin/ls", "-l", NULL };


void type_prompt ();

void type_prompt () {
	char cwd[MAX_LENGTH];
	getcwd (cwd, sizeof (cwd));
	strcat (cwd, ": ");
	if (line) {
		free (line);
		line = (char *) NULL;
	}
	line = readline (cwd);
	if (line && *line)
		add_history (line);
}



int main (int argc, char * argv[]) {
	int e;

	while(1) {
		type_prompt ();
		if (fork () != 0) {
			wait(0);
			type_prompt ();
		}
		else {
			e = execve(command, argument, NULL);
			printf("%d\n", e);
		}

	}
	return EXIT_SUCCESS;
}
