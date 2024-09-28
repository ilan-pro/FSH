#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <getopt.h>

#include <stdio.h>

#define BUFFER_SIZE 1024

char usage[] = " prend deux paramètres\n";

int do_tr(int del_char, char car1, char car2);

/* ./my-tr car1 car2 recopie l'entrée standard sur la sortie standard en
 * remplaçant chaque occurrence du caractère car1 par car2.  
 * ./my-tr -d car1 recopie l'entrée standard sur la sortie standard en 
 * supprimant les occurrences du caractère car1. */
int main(int argc, char * argv[]) {
  int result;
  int del_char = 0;
  char car1, car2 = '\0'; 

  if(argc != 3){ 
    write(STDERR_FILENO, argv[0], strlen(argv[0]));
    write(STDERR_FILENO, usage, strlen(usage));
    result = 1;
    goto cleanup_return;
  }

  int opt;
  if ((opt = getopt(argc, argv, "d")) != -1) {
    /* TODO: gérer l'option -d */
  }
  else {
    /* TODO: gérer le cas sans option */
  }

  /* TODO: appeler do_tr avec les bons arguments */

cleanup_return:
  return result;
}

/* recopie l'entrée standard sur la sortie standard en :
   - remplaçant chaque occurrence du caractère car1 par car2 si del_char = 0,
   - supprimant les occurrences du caractère car1 si del_char = 1 */
int do_tr(int del_char, char car1, char car2) {
  int result = 0;
  char * buf_in = NULL, * buf_out = NULL; /* buffers de lecture et d'écriture */

  /* TODO: allouer les buffers de lecture et d'écriture */
  
  while(1) {
    /* TODO: lire l'entrée (tant qu'il y a quelque chose à lire) */

    /* TODO: écrire dans le buffer d'écriture l'entrée modifiée
     * selon la valeur de del_char */
		
    /* TODO: écrire le résultat sur la sortie standard */
	 
  }
  
cleanup_return:
  /* TODO: libérer les buffers */
	
  return result;
}

