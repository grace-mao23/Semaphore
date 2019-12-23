#include "semaphore.h"

char flag[5];
int semd, shmd, fd;
struct sembuf sb;

int main(int argc, char *argv[]) {
  sb.sem_num = 0;
  sb.sem_op = -1;
  strcpy(flag, argv[1]);
  go();
  return 0;
}

void go() {
  if (strcmp(flag, "-c") == 0) { //creating
    create();
  } else if (strcmp(flag, "-v") == 0) {//viewing

  } else if (strcmp(flag, "-r") == 0) {//removing
    semd = semget(KEY, 1, 0);
    if (semd < 0) {
      printf("Error: %s\n", strerror(errno));
      
    }
  } else {
    printf("Invalid flag");
  }
}

void create() {
  semd = semget(KEY, 1, IPC_CREAT | 0644);
  if (semd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  printf("Semaphore created");

  shmd = shmget(KEY, size_of(char *), IPC_CREAT | 0644);
  if (shmd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  printf("Shared memory created");

  fd = open("telephone.txt", O_CREAT | O_TRUNC | O_RDWR, 0644);
  if (fd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  printf("File created");
  close(fd);
}
