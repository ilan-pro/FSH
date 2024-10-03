#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(void) {
  
  int fd1, fd2, nb;
  char buf[32];

  fd1 = open("toto", O_RDWR | O_CREAT | O_TRUNC, 0600);

  fd2 = dup(fd1);

  nb = write(fd1, "écriture via fd1\n", 18);
  fprintf(stderr, "[écriture de %d caractères via fd1]\n", nb);
  nb = write(fd2, "écriture via fd2\n", 18);
  fprintf(stderr, "[écriture de %d caractères via fd2]\n", nb);

  /* retour au début avec fd1 */
  lseek(fd1, 0, SEEK_SET);
  fprintf(stderr, "[retour au début avec fd1]\n");

  nb = read(fd2, buf, 18);
  fprintf(stderr, "[lecture de %d caractères via fd2] ", nb); /* rappel : stderr n'est pas bufferisée */
  write(STDERR_FILENO, buf, nb);

  nb = read(fd1, buf, 18);
  fprintf(stderr, "[lecture de %d caractères via fd1] ", nb);
  write(STDERR_FILENO, buf, nb);

  return 0;
}
