#include "semaphore.h"

int shmd, semd, fd;
struct sembuf sb;

int main() {
  sb.sem_num = 0;
  sb.sem_op = -1;
  go_write();
  return 0;
}

int go_write() {
  printf("Trying to get in\n");
  semd = semget(KEY, 1, 0);
  if (semd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  semop(semd, &sb, 1);
  shmd = shmget(KEY, sizeof(char *), 0);
  if (shmd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  fd = open("telephone.txt", O_RDWR, 0644);
  char *last = shmat(shmd, 0, 0);
  printf("Last addition: %s\n\n", last);
  char next[SIZE];
  printf("Your addition: ");
  fgets(next, SIZE, stdin);
  int i = 0;
  for (; i < sizeof(next); i++) {
    if (next[i] == '\n') {
      next[i] = '\0';
    }
  }
  strcpy(last, next);
  shmdt(last);
  close(fd);
  sb.sem_op = 1;
  semop(semd, &sb, 1);
  return 0;
}
