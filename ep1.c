/* Compilar com gcc -o ep1 ep1.c -Wall -std=c99 -ansi -pedantic -pthread */
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sched.h>

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


void* lostime (void* voidtime);
void FCFS ();


void redimensiona ();
void readFile ();

void SRTN ();

    
FILE *in;
FILE *out;
processo *listaproc;
int d = 0;
int N = 2;
int size = 0;
int cpu;
int currenti = 0;
int shortest = 0;


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


void FCFS () {
    int i = 0;
	clock_t begin, end, ops;
    double tf;
    double c;
	pthread_t *procs;
    pthread_t time;



	procs = malloc (sizeof (pthread_t) * size);
    begin = clock ();

    if (d) pthread_create (&time, NULL, arrival, &begin);

	while (i < size) {
            ops = (clock () - begin) / CLOCKS_PER_SEC;
            c = (double) ops;
            if (c >= listaproc[i].t0) {
                pthread_create (&procs[i], NULL, lostime, &listaproc[i].dt);
                pthread_join(procs[i], NULL);
                end = clock ();
                tf = (double) (end - begin)/CLOCKS_PER_SEC;
                if (d) fprintf (stderr, "Fim do %dº Processo: %s %f %f\n", currenti+1, listaproc[i].nome, tf, tf - listaproc[i].t0);
                fprintf(out, "%s %f %f\n", listaproc[i].nome, tf, tf - listaproc[i].t0);

                i++;
                currenti++;
            }
            else continue;
            
	}
  
    fprintf(out, "0\n");
    if (d) fprintf (stderr, "Terminado. Número de mudanças de Contexto: 0\n");
	    
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
    double *time = (double *)voidtime;
    clock_t begin = clock ();
    cpu = sched_getcpu ();
    if (d) fprintf (stderr, "  %s começou a usar CPU %d\n", listaproc[shortest].nome, cpu);

    while (((double) (clock () - begin)/CLOCKS_PER_SEC) < *time) {}
    if (d-1) fprintf (stderr, "  %s terminou de usar CPU %d\n", listaproc[shortest].nome, cpu);
    return NULL;
}

void SRTN () {
    double QUANTUM;
    int i = 0, j, contexto = 0, flag = 0;
    clock_t begin, end;
    double tf;
    pthread_t run, time;
    QUANTUM = 0.1;
    begin = clock ();

    if (d) pthread_create (&time, NULL, arrival, &begin);

    while (i < size) {
        while (1) {
            if (listaproc[shortest].dt > 0) {
                if ((double) ((clock () - begin)/CLOCKS_PER_SEC) >= listaproc[shortest].t0)
                    break;
            }
            else shortest++;
        }
        flag = 0;

        for (j = 0; j < size; j++) {
            if ((listaproc[j].t0 <= (double) ((clock () - begin)/CLOCKS_PER_SEC)) && 
                (listaproc[j].dt < listaproc[shortest].dt) && 
                (listaproc[j].dt > 0)) {

                shortest = j;
                flag = 1;
            }
        }
        if (flag) contexto++;
        if (listaproc[shortest].dt > 0) {
            pthread_create (&run, NULL, lostime, &QUANTUM);
            pthread_join (run, NULL);
            listaproc[shortest].dt = listaproc[shortest].dt - QUANTUM;
            if (listaproc[shortest].dt <= 0) {
                end = clock ();
                tf = (double) (end - begin)/CLOCKS_PER_SEC;

                fprintf(out, "%s %f %f\n", listaproc[shortest].nome, tf, tf - listaproc[shortest].t0);
            
                if (d) fprintf(stderr, "Fim do %s : %s %f %f\n", listaproc[shortest].nome, listaproc[shortest].nome, tf, tf - listaproc[shortest].t0);
                i++;
                shortest = 0;
            }
        }
    }
    fprintf(out, "Mudanças de Contexto: %d\n", contexto);
    if (d) fprintf (stderr, "Terminado. Número de mudanças de Contexto: %d\n", contexto);

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
            break;
        case 2:
            SRTN ();
            break;
    }
    return 0;
}
