#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define SHKEY 24601
#define SEMKEY 24602
#define SIZE 1000

union semun {
	int val;		// Value for SETVAL
	struct semid_ds *buf;		//Buffer for IPC_STAT, IPC_SET
	unsigned short *array;	//Array for GETALL, SETALL
	struct seminfo *__buf;	//Buffer for IPC_INFO (Linux-specific)
};

int go();
int creates();
int removes();
int views();
int go_write();
