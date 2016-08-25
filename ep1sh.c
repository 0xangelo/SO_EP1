# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

# define MAX_LENGTH 1024

static char *line = (char *) NULL;
char *command;
char *argument[5];


void type_prompt ();
int splitter (char line[], char *dic[]);

void type_prompt () {
    int n;
    char cwd[MAX_LENGTH];
    char *dir;
    dir = getcwd (cwd, sizeof (cwd));
    if (dir == NULL) printf ("Erro no diretorio! Cuidado!");
    strcat (cwd, ": ");
    if (line) {
        free (line);
        line = (char *) NULL;
    }
    line = readline (cwd);
    if (line && *line) {
        add_history (line);
        n = splitter (line, argument);
        argument[n] = NULL;
        command = argument[0];
    }

}

int splitter (char line[], char *dic[]) {
    char *token;
    int n = 0;
    token = strtok (line, " ");
    while (token != NULL) {
        dic[n++] = token;
        token = strtok (NULL, " ");
    }
    return n;
 }

int main (int argc, char * argv[]) {

    int e, pid;

    while(1) {
        type_prompt ();
        if ((pid = fork ()) != 0) {
            wait(0);
        }
        else {
            e = execve(command, argument, NULL);
            printf("%d\n", e);
        }

    }
    return EXIT_SUCCESS;
}
