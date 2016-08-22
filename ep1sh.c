# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define MAX_LENGTH 1024

void type_prompt ();
void read_command ();

void type_prompt () {
	char cwd[MAX_LENGTH];
	getcwd (cwd, sizeof (cwd));
	printf ("(%s): ", cwd);

}
void read_command (char command[], char paramenters[]) {
	scanf ("%s %s", command, paramenters);
}


int main (int argc, char * argv[]) {

	char command[MAX_LENGTH], paramenters[MAX_LENGTH];

	while(1) {
		type_prompt ();
		read_command (command, paramenters);


	}
	return EXIT_SUCCESS;
}
