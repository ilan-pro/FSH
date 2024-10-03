#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define TEMPO 1

int main(void) {
  printf("pid = %d\n", getpid());

  sleep(TEMPO);
  
  int fd = open("toto", O_WRONLY | O_CREAT | O_TRUNC, 0600);
  fprintf(stderr, "ouverture de toto effectuée\n");

  sleep(TEMPO);
  
  int sauvegarde = dup(STDOUT_FILENO);
  fprintf(stderr, "sauvegarde de la sortie initiale\n");

  sleep(TEMPO);

  /* redirection de la sortie standard */
  dup2(fd, STDOUT_FILENO);
  fprintf(stderr, "redirection vers toto effectuée\n");

  sleep(TEMPO);
  
  /* fermeture de fd */
  close(fd);
  fprintf(stderr, "fermeture du descripteur fd\n");

  sleep(TEMPO);
  
  printf("écriture sur la sortie standard après redirection vers toto\n");

  sleep(TEMPO);

  /* rétablissement de la sortie standard initiale */
  dup2(sauvegarde, STDOUT_FILENO);
  close(sauvegarde);

  fprintf(stderr, "rétablissement de la sortie standard initiale\n");

  sleep(TEMPO);

  printf("écriture sur la sortie standard après rétablissement de la sortie initiale\n");


  return 0;
}
