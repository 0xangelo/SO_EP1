/* Compilar com gcc -o NOME NOME.c -Wall -std=c99 -ansi -pedantic -pthread */
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
    clock_t tf;
} processo;

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine)(void*), void *arg);
int pthread_join(pthread_t thread, void **value_ptr);
void* lostimeFCFS (void* voidtime);
void FCFS ();
void SRTN ();
void redimensiona ();
void readFile ();

/***************************** Variáveis Globais *******************************/

FILE *in;
FILE *out;
processo *listaproc;
int size = 0;
pthread_mutex_t *mutex;

/***************************** Escalonador FCFS ********************************/

void FCFS () {
    int i;
    pthread_t *procs;
    pthread_mutex_init(mutex, NULL);

    procs = malloc (sizeof (pthread_t) * size);

    for (i = 0; i < size; i++) {
        pthread_create (&procs[i], NULL, lostimeFCFS, &listaproc[i].dt);
        pthread_join(procs[i], NULL);
        listaproc[i].tf = clock ();
        fprintf (out, "%s", listaproc[i].nome);
        fprintf(out, " %Lf", (long double) listaproc[i].tf);
        fprintf(out, " %Lf\n", (long double) listaproc[i].tf - listaproc[i].t0);
    }

    pthread_mutex_destroy(mutex);
	    
}

void* lostimeFCFS (void *voidtime) {
    double i = 0;
    double *time = (double *)voidtime;

    while (i < *time) {
        pthread_mutex_lock (mutex);
        sleep (QUANTUM);
        i += QUANTUM;
        pthread_mutex_unlock (mutex);
    }

    return NULL;
}

/***************************** Escalonador SRTN ********************************/

void SRTN () {
    double t_atual = 0.0;      /* tempo teórico dentro da execução do escalonador */
    int i;
    int recebidos = 0;         /* recebidos armazena o número de programas recebidos até t_atual pelo trace */
    int array[size];
    pthread_t *procs;
    time_t inicio;             /* referência para o cálculo do tempo final a partir da execução do escalonador */
    
    mutex = malloc (sizeof (pthread_mutex_t) * size);
    for (i = 0; i < size; i++) {
        pthread_mutex_init(&mutex[i], NULL);
        pthread_mutex_lock (mutex[i]);
        pthread_create (&procs[i], NULL, lostimeSRTN, i);
        array[i] = i;
    }

    /* Rascunho do código */
    /* A ideia é ficar de olho na entrada e verificar se um novo 
       programa entrou no tempo que se passou durante o ciclo 
       anterior. Sendo esse o caso, deve-se reordenar a fila de 
       processos com base no tempo de execução restante de cada um.
       "array" seria um vetor de inteiros correspondentes aos índices
       dos processos em "listaproc". O quicksort ordenaria esse vetor
       com base no tempo restante (dt) de cada processo, deixando o
       vetor em ordem crescente de tempo restante dos processos cor-
       respondentes aos índices. 
       Como haverão processos que terminarão com o tempo, ou seja, 
       "listaproc[array[x]].dt <= 0", percorremos "array" até  
       acharmos o primeiro processo na fila com tempo restante para 
       rodar. Caso percorramos o vetor e não haja processos  
       restantes, o programa está terminado.
       (Linha 111) Importante: quicksort só ordena a fila de processos
       até o último processo recebido. A listaproc e array já estão
       em ordem de chegada, portanto ordenar de 0 até "recebidos" não
       engloba programas que ainda estão por vir.                  */

    inicio = clock();
    /* while(1) { */
    /*     while (recebidos < size && listaproc[recebidos].t0 < t_atual) recebidos++;  */
    /*     pthread_mutex_lock (&mutex[i]);                                             */
    /*     quicksort (array, 0, recebidos);                                            */
    /*     for (i = 0; i < recebidos; i++)                                             */
    /*         if (listaproc[array[i]].dt > 0) break;                                  */
    /*     if (i >= size) break;                                                       */
    /*     pthread_mutex_unlock (mutex[array[i]]);                                     */
    /*     sleep(QUANTUM);                                                             */
    /*     t_atual += QUANTUM;                                                         */
    /* }                                                                               */

    for (i = 0; i < size; i++) {
        fprintf (out, "%s", listaproc[i].nome);
        fprintf(out, " %Lf", (long double) listaproc[i].tf);
        fprintf(out, " %Lf\n", (long double) listaproc[i].tf - listaproc[i].t0);
    }
    
    
}

void * lostimeSRTN (void * arg) {
    int proc = (int *) arg;

    while (listaproc[proc].dt > 0) {
        pthread_mutex_lock (&mutex[proc]);
        sleep (QUANTUM);
        listaproc[proc].dt -= QUANTUM;
        pthread_mutex_unlock (&mutex[proc]);
    }
    listaproc[proc].tf = clock () - inicio;
    return NULL;
}

/***************************** Escalonador FILAS *******************************/

/* void FILAS () { */


/* } */

/***************************** Leitura da entrada ******************************/

void readFile () {
    int N = 2;
    listaproc = malloc (N * sizeof (processo));
    
    while (fscanf (in, "%lf %s %lf %lf", &listaproc[size].t0, listaproc[size].nome, &listaproc[size].dt, &listaproc[size].deadline) != EOF) {
        size++;
        if (size == N) redimensiona (&N);
    }

}

void redimensiona (int * N) {
    *N *= 2;
    listaproc = realloc (listaproc, *N * sizeof (processo));
}

/*********************************** MAIN **************************************/

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
    case 2:
        SRTN ();
    }

    return 0;
}
