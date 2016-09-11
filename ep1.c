/* Compilar com gcc -o ep1 ep1.c -Wall -std=c99 -ansi -pedantic -pthread */
#include "ep1.h"
#include "FCFS.h"
#include "SRTN.h"
#include "MULT.h"

/****************************** Variáveis globais *****************************/
FILE *in;
FILE *out;
processo *listaproc;
int d = 0;
int N = 2;
int size = 0;
int cpu;
int currenti = 0;
int escalonador;

/*********************************** MAIN *************************************/
int main (int argc, char **argv) {
    char *entrada   = argv[2];
    char *saida     = argv[3];
    if (argc > 4) d = 1;    
    in = fopen (entrada, "r");
    out = fopen (saida, "w");
    escalonador = atoi (argv[1]);

    readFile ();


    switch (escalonador) {
        case 1:
            FCFS ();
            break;
        case 2:
            SRTN ();
            break;
        case 3:
            MULTIPLAS_FILAS ();
            break;
    }
    fclose (in);
    fclose (out);
    free (listaproc);

    return 0;
}

/*********************************Funções auxiliares***************************/
void* arrival (void *time) {
    int i = 0;
    double *start = (double*)time;
    double ops;

    while (i < size) {
        ops = (double) ((clock () - *start) / CLOCKS_PER_SEC);
        if (ops >= listaproc[i].t0) {
            fprintf (stderr, "** Inicio  %s: %f %s %f %f **\n", listaproc[i].nome, listaproc[i].t0, listaproc[i].nome, listaproc[i].dt, listaproc[i].deadline);
            i++;
        }
    }

    return NULL;
}

void redimensiona () {
    N *= 2;
    listaproc = realloc (listaproc, N * sizeof (processo));
}

void readFile () {
    listaproc = malloc (N * sizeof (processo));
    
    while (fscanf (in, "%lf %s %lf %lf", &listaproc[size].t0, listaproc[size].nome, &listaproc[size].dt, &listaproc[size].deadline) != EOF) {
        size++;
        if (size == N) redimensiona ();
    }

}

void* lostime (void *voidtime) {
    double *time = (double *)voidtime, now;
    clock_t begin = clock ();
    cpu = sched_getcpu ();
    if (d) fprintf (stderr, "  %s começou a usar CPU %d\n", listaproc[currenti].nome, cpu);

    while ((now = ((double) (clock () - begin)/CLOCKS_PER_SEC)) < *time) {}
    if (d) fprintf (stderr, "  %s terminou de usar CPU %d\n", listaproc[currenti].nome, cpu);

    return NULL;
}
