#include "ep1.h"
#include "MULT.h"

/********************************* Protótipos *********************************/
void * arrivalM (void *time);
int tem_alguem (int p, int u);

/****************************** Variáveis globais *****************************/
int **filas;
int *p, *u;
int fila_atual = 0;

/************************ MULTIPLAS_FILAS**************************************/
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
