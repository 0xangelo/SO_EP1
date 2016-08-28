#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdio.h>


typedef struct {
  volatile int x;
  volatile int enter1, enter2, turn;
} SHARED;

/* Esta variável será compartilhada entre dois processos */
SHARED *Comp;

int main(int ac, char **av)
{
  int i;
  int smid;
  int sim = ac > 1;

  /* shmget aloca um bloco de memória compartilhada e retorna um 'handle' */
  smid = shmget(42,				/* identificador */
		sizeof(SHARED),			/* tamanho */
		IPC_CREAT | 0600) ;		/* permissão de acesso */

  /* shmat retorna um ponteiro para a região compartilhada */
  Comp = (SHARED *)shmat(smid, 0, 0);
  
  /* inicialização */
  Comp->x = 0;
  Comp->enter1 = Comp->enter2 = 0;
  Comp->turn = 1;

  /* fork() é uma chamada para o sistema que cria uma cópia do processo */
  /* corrente. */
  /* Logo após a chamada existem dois processos idênticos, cada um com */
  /* seu contexto.  No processo "filho", fork() retorna 0. */
	
  if ((fork()) == 0) {
	/* Filho */
	int y, j;

	for (j=0; j<1000; j++) {
	  if (sim) {
		Comp->enter1 = 1;
		Comp->turn   = 2;
		while (Comp->enter2 == 1 && Comp->turn == 2) usleep(1);
	  }
	  
	  for (i=0; i< 5000;i++) {
		y = Comp->x;
		y += 2;
		Comp->x = y;
		
	  }
	  Comp->enter1 = 0;
	}
  }
  else {
	/* Pai */
	int y,j;

	for (j=0; j<1000; j++) {
	  if (sim) {
		Comp->enter2 = 1;
		Comp->turn   = 1;
		while (Comp->enter1 == 1 && Comp->turn == 1) usleep(1);
	  }
	  for (i=0; i< 5000;i++) {
		y = Comp->x;
		y += 1;
		Comp->x = y;
	  }
	  Comp->enter2 = 0;
	}

	/* espera o filho terminar */
	wait(NULL);

	/* imprime o valor final */
	printf("x = %d\n", Comp->x);

	/* libera a memória */
	shmctl(smid,IPC_RMID, NULL);
  }
  return 0;
}

                                                              
