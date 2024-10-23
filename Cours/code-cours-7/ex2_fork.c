#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void){
  int r;

  r = fork();

  switch(r) {
    case -1 : /* cas d'erreur */
      perror("fork");
      exit(1);
    case 0 : /* code du fils */
      dprintf(1, "je suis le fils; mon pid est %d, celui de mon père %d\n",
           getpid(), getppid());
      break;
    default : /* code du père */
      /* à ajouter pour retarder l'exécution de la fin du père après la
       * terminaison du fils */
      // wait(NULL); 
      dprintf(1, "JE SUIS LE PÈRE; MON PID EST %d, JE VIENS DE CRÉER UN FILS, DE PID %d\n",
           getpid(), r);
  }

  /* code commun au père et au fils */
  sleep(1);
  dprintf(1, "J'ai fini!\n");

  return 0;
}
