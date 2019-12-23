#include "semaphore.h"

char flag[5];
int semd, shmd, fd;
union semun su;
struct sembuf sb;

int main(int argc, char *argv[]) {
  su.val = 1;
  sb.sem_num = 0;
  sb.sem_op = -1;
  strcpy(flag, argv[1]);
  go();
  return 0;
}

int go() {
  if (strcmp(flag, "-c") == 0) { //creating
    creates();
  } else if (strcmp(flag, "-v") == 0) { //viewing
    views();
  } else if (strcmp(flag, "-r") == 0) { //removing
    removes();
  } else {
    printf("Invalid flag\n");
  }
}

int creates() {
  semd = semget(SEMKEY, 1, IPC_CREAT | 0644); // create semaphore
  if (semd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  printf("Semaphore created\n");
  semctl(semd, 0, SETVAL, su); // set initial value

  shmd = shmget(SHKEY, sizeof(char *), IPC_CREAT | 0644); // create shared memory
  if (shmd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  printf("Shared memory created\n");

  fd = open("telephone.txt", O_CREAT | O_TRUNC | O_RDWR, 0644); // create file
  if (fd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  printf("File created\n");
  close(fd);

  return 0;
}

int removes() {
  semd = semget(SEMKEY, 1, 0);
  if (semd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  printf("Trying to get in\n");
  semop(semd, &sb, 1); // getting in, block until available

  shmd = shmget(SHKEY, sizeof(char *), 0);
  if (shmd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }

  fd = open("telephone.txt", O_RDONLY);
  if (fd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  char buff[SIZE];
  buff[0] = '\0'; // don't print weird stuff when there's nothing
  read(fd, buff, SIZE);
  if (strlen(buff) != 0) {
    *(strrchr(buff, '\n') + 1) = '\0'; // don't print weird stuff at the end
  }
  printf("The story so far:\n");
  printf("%s\n", buff);
  close(fd);

  shmctl(shmd, IPC_RMID, 0); // remove shared memory
  printf("Shared memory removed\n");
  semctl(semd, IPC_RMID, 0); // remove semaphore
  printf("Semaphore removed\n");
  remove("telephone.txt"); // remove file
  printf("File removed\n");

  return 0;
}

int views() {
  fd = open("telephone.txt", O_RDONLY);
  if (fd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  char buff[SIZE];
  buff[0] = '\0';
  read(fd, buff, SIZE);
  if (strlen(buff) != 0) {
    *(strrchr(buff, '\n') + 1) = '\0';
  }
  printf("The story so far:\n");
  printf("%s\n", buff);
  close(fd);

  return 0;
}
