/* Compilar com gcc -o ep1 ep1.c -Wall -std=c99 -ansi -pedantic -pthread */
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sched.h>

# define QUANTUM 1
# define _GNU_SOURCE

typedef struct {
    double t0;
    char nome[128];
    double dt;
    double deadline;
} processo;

int sched_getcpu(void);
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
    void *(*start_routine)(void*), void *arg);
int pthread_join(pthread_t thread, void **value_ptr);


void* lostimeFCFS (void* voidtime);
void FCFS ();
void redimensiona ();
void readFile ();
void errentrada (processo a);

    
FILE *in;
FILE *out;
processo *listaproc;
int d = 0;
int N = 2;
int size = 0;
int cpu;
int currenti = 0;
pthread_mutex_t mutex;

void errentrada (processo a) {
    fprintf (stderr, "Linha de Trace: %f %s %f %f\n", a.t0, a.nome, a.dt, a.deadline);
}
void errsaida (char *nome, double final, double relogio, int i) {
    fprintf(out, "%s %.2f(s) %.2f(s)\n", nome, final , relogio);
    fprintf(stderr, "Linha de Saida: %s %f %f\n", nome, final, relogio);
    if (i == 1)
        fprintf(stderr, "Número de mudanças de Contexto: 0\n");
}


void FCFS () {
    int i = 0;
	clock_t begin, end, ops;
    double tf;
	pthread_t *procs;

	procs = malloc (sizeof (pthread_t) * size);
    begin = clock ();

	while (i < size) {
            ops = (clock () - begin) / CLOCKS_PER_SEC;
            if (ops >= listaproc[i].t0) {
                if (d) fprintf (stderr, "%3d: Linha de Trace: %f %s %f %f\n", currenti, listaproc[i].t0, listaproc[i].nome, listaproc[i].dt, listaproc[i].deadline);
                pthread_create (&procs[i], NULL, lostimeFCFS, &listaproc[i].dt);
                pthread_join(procs[i], NULL);
                end = clock ();
                tf = (double) (end - begin)/CLOCKS_PER_SEC;
               if (d) fprintf (stderr, "%3d: Linha de Saida: %s %f %f\n", currenti, listaproc[i].nome, tf, tf - listaproc[i].t0);
                fprintf(out, "%s %f %f\n", listaproc[i].nome, tf, tf - listaproc[i].t0);

                i++;
                currenti++;
            }
            else continue;
            
	}
    fprintf(out, "0\n");
    if (d) fprintf (stderr, "%3d: Número de mudanças de Contexto: 0\n", currenti-1);
	    
}

void* lostimeFCFS (void *voidtime) {
    double *time = (double *)voidtime;
    clock_t begin = clock ();
    cpu = sched_getcpu ();
    if (d) fprintf (stderr, "%3d: Processo %s começou a usar CPU %d\n", currenti, listaproc[currenti].nome, cpu);
	while (((clock () - begin)/CLOCKS_PER_SEC) < *time) {}
    if (d) fprintf (stderr, "%3d: Processo %s terminou de usar CPU %d\n", currenti, listaproc[currenti].nome, cpu);
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

int main (int argc, char **argv) {
    int escalonador = atoi (argv[1]);
    char *entrada   = argv[2];
    char *saida     = argv[3];
    if (argc > 4) d = 1;    
    in = fopen (entrada, "r");
    out = fopen (saida, "w");

    readFile ();

    switch (escalonador) {
        case 1:
            FCFS ();
    }
    return 0;
}
