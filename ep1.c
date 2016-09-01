#include <pthread.h>
#include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>


typedef struct {
    double t0;
    char nome[128];
    double dt;
    double deadline;
} processo;



FILE *in;
FILE *out;
processo *listaproc;
int N = 2;
int size = 0;

void FCFS () {
    
}

void redimensiona () {
    N *= 2;
    listaproc = realloc (listaproc, N * sizeof (processo));
}

void readFile () {
    listaproc = malloc (N * sizeof (processo));
    
    while (fscanf (in, "%lf %s %lf %lf", &listaproc[size].t0, listaproc[size].nome, &listaproc[size].dt, &listaproc[size].deadline) != 0) {
        size++;
        if (size == N) redimensiona ();
    }

}

int main (int argc, char **argv) {
    int escalonador = atoi (argv[1]);
    char *entrada   = argv[2];
    char *saida     = argv[3];
    
    in = fopen (entrada, "r");
    out = fopen (saida, "w");

    readFile ();

    switch (escalonador) {
        case 1:
            FCFS ();
    }

    return 0;
}
