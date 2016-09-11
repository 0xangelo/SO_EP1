#include "SRTN.h"
#include "ep1.h"

/********************************SRTN******************************************/
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