
/*
  Same includes as in u6_1.c. Copy Pasted some code of the given source code.
*/

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>


#include <signal.h>

// Constants
#define PHILOSOPHER_COUNT 5
#define EAT_TIME 1
#define THINK_TIME 3

#define THINKING 0
#define HUNGRY 1
#define EATING 2

// For easier usage
#define SELF process_number
#define LEFT (process_number==0) ? PHILOSOPHER_COUNT : process_number-1
#define RIGHT (process_number+1)%PHILOSOPHER_COUNT


// Globabl variables
volatile int eat_count;
int semset;
int process_number;
char* philospher_states;

/* ----- SEMAPHORE HELPER FUNCTIONS ----- */

// Init a semaphore -> INIT
void sem_init(int semnr, int value) {
    int erg;
    erg = semctl(semset, semnr, SETVAL, value);
}

// Signal a semaphore to continue -> UP
void sem_signal(int semnr) {
    struct sembuf semops;
    int erg;
    semops.sem_num = semnr;
    semops.sem_op = 1; // Up the semaphore
    semops.sem_flg = 0;

    erg = semop(semset, &semops, 1);
}

// Wait for a semaphore -> DOWN
void sem_wait(int semnr) {
    struct sembuf semops;
    int erg;
    semops.sem_num = semnr;
    semops.sem_op = -1; // Down the semaphore
    semops.sem_flg = 0;
    erg = semop(semset, &semops, 1);
}

/* ----- Dining Philosophers Problem -----*/

void sigTermHandler(){
	printf("Philosopher #%d has eaten %d times.\n", process_number, eat_count);
	exit(0);
}


// Test whether a philosopher can eat
void test_and_eat(int p){
	if (philospher_states[p] == HUNGRY &&
			philospher_states[LEFT] != EATING &&
			philospher_states[RIGHT] != EATING){

		philospher_states[p]=EATING;
    sem_signal(p);
	}
}

// One grabs fork
void grab_forks(){
  sem_wait(PHILOSOPHER_COUNT); // Down mutex
	printf("Philosopher #%d is hungry.\n", process_number);
	philospher_states[SELF]=HUNGRY;
	test_and_eat(SELF);
  sem_signal(PHILOSOPHER_COUNT);
  sem_wait(SELF);
}

// Put away forks
void put_away_forks(){
  sem_wait(PHILOSOPHER_COUNT); // Down mutex
	philospher_states[SELF]=THINKING;
	test_and_eat(LEFT);
	test_and_eat(RIGHT);
  sem_signal(PHILOSOPHER_COUNT); // Up mutex
}

// Child process
void child(){
  eat_count=0;
	signal(SIGINT, sigTermHandler);

  printf("Philosopher %d joined the table.\n", process_number);

  while(1){
  	printf("Philosopher #%d is thinking.\n", process_number);
  	sleep(THINK_TIME);
  	grab_forks();
  	printf("Philosopher #%d is eating.\n", process_number);
    eat_count += 1;
  	sleep(EAT_TIME); // Eat.
  	put_away_forks();
  }
}

// Parent process, wait until all children are finished
void parent(){
  wait(NULL);
}

// Main Method
int main(int argc, char *argv[]) {

  // Create Shared memory
  int shmID;
  shmID = shmget(IPC_PRIVATE, PHILOSOPHER_COUNT, IPC_CREAT | 0x1ff);
  philospher_states = (char*) shmat(shmID, NULL, 0);

  if ((long) shmID == -1 || (long) shmat == -1){
		printf("Fehler: kann keinen Speicher\n.");
		exit(1);
	}

  // Create Semaphores, last semaphore is mutex
  semset = semget(IPC_PRIVATE, PHILOSOPHER_COUNT + 1, IPC_CREAT | 0x1ff);
  if (semset == -1) {
    printf("Fehler: keine Semaphoren mehr verfuegbar!\n");
    exit(1);
  }

  // Init all semaphores and mutex
  int i;
  for (i = 0; i < PHILOSOPHER_COUNT; i++) sem_init(i,0); // Init semaphores to 0
  sem_init(i,1); // Init mutex to 1

  // At the beginnging every philospher is thinking
  for (i = 0; i < PHILOSOPHER_COUNT; i++){
  	philospher_states[i]=THINKING;
  }

  // Create the children processes
  int pid;
  for(process_number = 0; process_number < PHILOSOPHER_COUNT; process_number++){
  		pid=fork();
  		if (pid == 0) break;
  }

  if (pid == 0){
    child();
  } else if (pid < 0){
  	printf("Fehler: Kann die Kind Prozesse nicht erstellen\n");
    exit(1);
  } else {
    parent();
  }

  return 0;
}

