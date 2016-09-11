#include "FCFS.h"
#include "ep1.h"
/********************************FCFS******************************************/
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
