#include "types.h"
#include "stat.h"
#include "user.h"

seqloc lock;
volatile int next; // who is to run next
volatile int pass;
int num_of_thread, num_of_pass;
volatile int f = 0;

void player(void *arg){
  int i = *(int *)arg;
  int r;
  unsigned long seq0, seq1;
  while(!f);
  while(pass < num_of_pass){
    do{
      seq0 = lock.seq;
      r = next;
      seq1 = lock.seq;
    }while(seq0 != seq1 || seq0 & 1);

    if (i == r && pass < num_of_pass) {
          seqlock_acquire(&lock);
          pass ++;
          next ++;
          next = next % num_of_thread;
          printf(1, "Pass number no: %d, Thread %d is passing the token to thread %d\n", pass, i, next);
          seqlock_release(&lock);
      }
  }
  exit();
}

int main(int argc, char const *argv[]){
  int i;
  int t0;
  if (argc != 3) {
      printf(1, "wrong args!!!\n" );
      exit();
  }
  num_of_thread = atoi(argv[1]);
  num_of_pass = atoi(argv[2]);
  seqlock_init(&lock);
  next = 0;
  pass = 0;

  for (i = 0; i < num_of_thread; i++) {
        thread_create(player, (void *)&i);
        sleep(10);
  }
  f = 1;
  t0 = uptime();
  while(wait()>=0);
  printf(1, "\nSimulation of SEQLOCK Frisbee game has finished, %d rounds were played in total!\n", num_of_pass);
  printf(1, "total exec time = %d\n", uptime()-t0);
  exit();
}
