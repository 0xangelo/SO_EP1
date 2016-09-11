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

/****************************** Variáveis globais *****************************/
extern FILE *in;
extern FILE *out;
extern processo *listaproc;
extern int d;
extern int N;
extern int size;
extern int cpu;
extern int currenti;
extern int escalonador;

/********************************* Protótipos *********************************/
int sched_getcpu(void);
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
    void *(*start_routine)(void*), void *arg);
int pthread_join(pthread_t thread, void **value_ptr);


void * lostime (void* voidtime);
void * arrival (void *time);


void redimensiona ();
void readFile ();


void daumplus (int m);
