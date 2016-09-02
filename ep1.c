/* Compilar com gcc -o ep1 ep1.c -Wall -std=c99 -ansi -pedantic -pthread */
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>


# define QUANTUM 1

typedef struct {
    double t0;
    char nome[128];
    double dt;
    double deadline;
} processo;

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
    void *(*start_routine)(void*), void *arg);
int pthread_join(pthread_t thread, void **value_ptr);
void* lostimeFCFS (void* voidtime);
void FCFS ();
void redimensiona ();
void readFile ();

    
FILE *in;
FILE *out;
processo *listaproc;
int N = 2;
int size = 0;
pthread_mutex_t mutex;

void FCFS () {
	int i;
	clock_t tf;
	pthread_t *procs;
	pthread_mutex_init(&mutex, NULL);

	procs = malloc (sizeof (pthread_t) * size);

	for (i = 0; i < size; i++) {
            pthread_create (&procs[i], NULL, lostimeFCFS, &listaproc[i].dt);
            pthread_join(procs[i], NULL);
            tf = clock ();
            fprintf (out, "%s", listaproc[i].nome);
            fprintf(out, " %Lf", (long double) tf);
            fprintf(out, " %Lf\n", (long double) tf - listaproc[i].t0);
	}

	pthread_mutex_destroy(&mutex);
	    
}

void* lostimeFCFS (void *voidtime) {
	double i = 0;
        double *time = (double *)voidtime;

	while (i < *time) {
		pthread_mutex_lock (&mutex);
		sleep (QUANTUM);
		i += QUANTUM;
		pthread_mutex_unlock (&mutex);
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
