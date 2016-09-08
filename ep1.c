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


void * lostime (void* voidtime);
void * arrival (void *time);


void FCFS ();


void redimensiona ();
void readFile ();

void SRTN ();


void MULTIPLAS_FILAS ();
void * arrivalM (void *time);
int tem_alguem (int p, int u);
void daumplus (int m);

FILE *in;
FILE *out;
processo *listaproc;
int **filas;
int *p, *u;
int d = 0;
int N = 2;
int size = 0;
int cpu;
int currenti = 0;
int escalonador;
int fila_atual = 0;

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
            if (listaproc[currenti].dt > 0) {
                if ((double) ((clock () - begin)/CLOCKS_PER_SEC) >= listaproc[currenti].t0)
                    break;
            }
            else currenti++;
        }
        flag = 0;

        for (j = 0; j < size; j++) {
            if ((listaproc[j].t0 <= (double) ((clock () - begin)/CLOCKS_PER_SEC)) && 
                (listaproc[j].dt < listaproc[currenti].dt) && 
                (listaproc[j].dt > 0)) {

                currenti = j;
                flag = 1;
            }
        }
        if (flag) contexto++;
        if (listaproc[currenti].dt > 0) {

            if (listaproc[currenti].dt < QUANTUM) {
                QUANTUM = listaproc[currenti].dt;
                pthread_create (&run, NULL, lostime, &QUANTUM);
                pthread_join (run, NULL);
            }
            else {
                pthread_create (&run, NULL, lostime, &QUANTUM);
                pthread_join (run, NULL);
            }
            QUANTUM = 0.1;

            listaproc[currenti].dt = listaproc[currenti].dt - QUANTUM;

            if (listaproc[currenti].dt <= 0) {
                end = clock ();
                tf = (double) (end - begin)/CLOCKS_PER_SEC;

                fprintf(out, "%s %f %f\n", listaproc[currenti].nome, tf, tf - listaproc[currenti].t0);
            
                if (d) fprintf(stderr, "Fim do %s : %s %f %f\n", listaproc[currenti].nome, listaproc[currenti].nome, tf, tf - listaproc[currenti].t0);
                i++;
                currenti = 0;
            }
        }
    }
    fprintf(out, "Mudanças de Contexto: %d\n", contexto);
    if (d) fprintf (stderr, "Terminado. Número de mudanças de Contexto: %d\n", contexto);

}
void MULTIPLAS_FILAS () {
    int i, terminados = 0, contexto = 0;
    double QUANTUM;
    clock_t begin, end;
    double tf;
    pthread_t run, time;

    filas = malloc (5 * sizeof (int *));
    for (i = 0; i < 5; i++)
        filas[i] = malloc (size * sizeof (int));
    p = malloc (5 * sizeof (int));
    u = malloc (5 * sizeof (int));
    for (i = 0; i < 5; i++)  p[i] = u[i] = 0;

    begin = clock ();
    pthread_create (&time, NULL, arrivalM, &begin);
    
    while (terminados < size) {
        if (tem_alguem (p[fila_atual], u[fila_atual])) {
            currenti = filas[fila_atual][p[fila_atual]];

            QUANTUM = (fila_atual + 1) * 0.200001;

            if (listaproc[currenti].dt < QUANTUM) {
                QUANTUM = listaproc[currenti].dt;
                pthread_create (&run, NULL, lostime, &QUANTUM);
                pthread_join (run, NULL);
            }
            else {
                pthread_create (&run, NULL, lostime, &QUANTUM);
                pthread_join (run, NULL);
            }
            listaproc[currenti].dt = listaproc[currenti].dt - QUANTUM;


          

            if (listaproc[currenti].dt > 0) {
                contexto++;
                if (fila_atual < 4) {
                    p[fila_atual]++;
                    filas[fila_atual + 1][u[fila_atual + 1]++] = currenti;
                }
            }
            else {
                end = clock ();
                tf = (double) (end - begin)/CLOCKS_PER_SEC;
                fprintf(out, "%s %f %f\n", listaproc[currenti].nome, tf, tf - listaproc[currenti].t0);
                if (d) fprintf(stderr, "Fim do %s : %s %f %f\n", listaproc[currenti].nome, listaproc[currenti].nome, tf, tf - listaproc[currenti].t0);
                p[fila_atual]++;
                terminados++;
            }

            fila_atual = 0;

        }
        else fila_atual = (fila_atual + 1) % 5;

    }
    fprintf(out, "Mudanças de Contexto: %d\n", contexto);
    if (d) fprintf (stderr, "Terminado. Número de mudanças de Contexto: %d\n", contexto);

    free (p);
    free (u);
    for (i = 0; i < 5; i++)
        free (filas[i]);
    free (filas);

}

void * arrivalM (void *time) {
    int i = 0;
    double *start = (double*)time;
    double ops;

    while (i < size) {
        ops = (double) ((clock () - *start) / CLOCKS_PER_SEC);
        if (ops >= listaproc[i].t0) {
            filas[0][u[0]++] = i;
            if (d) fprintf (stderr, "** Inicio  %s: %f %s %f %f **\n", listaproc[i].nome, listaproc[i].t0, listaproc[i].nome, listaproc[i].dt, listaproc[i].deadline);
            i++;
        }
    }

    return NULL;
}

int tem_alguem (int p, int u) {
    return u - p;
}



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
  
    fprintf(out, "Mudanças de Contexto: 0\n");
    if (d) fprintf (stderr, "Terminado. Número de mudanças de Contexto: 0\n");
    free (procs);
        
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
