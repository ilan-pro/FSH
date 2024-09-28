#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <getopt.h>

#include <stdio.h>

#define BUFFER_SIZE 10

char usage[] = " prend trois paramètres\n";

int do_sed(int fd, int del_char, char car1, char car2);

/* ./my-sed car1 car2 filename remplace chaque occurrence du caractère
 * car1 par car2 dans le fichier de référence filename.  
 * ./my-sed -d car1 filename supprime les occurrences du caractère car1
 * dans le fichier de référence filename. */
int main(int argc, char * argv[]) {
  int result;
  int del_char = 0;
  char car1, car2 = '\0';
  int fd;

  if(argc != 4){ 
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

  /* TODO: ouvrir en lecture/écriture le fichier dont la référence est 
   * passée en paramètre. L'ouverture échoue si le fichier n'est pas
   * accessible */
	
  /* TODO: appeler do_sed avec les bons arguments */

  /* TODO: fermer le fichier */
	
cleanup_return:
  return result;
}


int do_sed(int fd, int del_char, char car1, char car2) {
  int result = 0;
  char * buf_in = NULL, * buf_out = NULL; /* buffers de lecture et d'écriture */
  int lect = 0, ecr = 0; /* respectivement nombre d'octets lus et nombre
                            d'octets écrits dans le fichier */

  /* TODO: allouer les buffers de lecture et d'écriture */


  while(1) {
    /* TODO: placer la tête de lecture au bon endroit */

    /* TODO:  lire dans le fichier (tant qu'il y a quelque chose à lire) */

    /* TODO: mettre à jour la variable lect */

    /* TODO: écrire dans le buffer d'écriture l'entrée modifiée selon la
     * valeur de del_char */
		
    /* TODO: placer la tête d'écriture au bon endroit */

    /* TODO: écrire le résultat dans le fichier */
		
    /* TODO: mettre à jour la variable ecr */
		
  }

  if (del_char && ecr < lect) {
    /* TODO: tronquer la fin du fichier */
  }
   
cleanup_return:
  /* TODO: libérer les buffers */

  return result;
}

