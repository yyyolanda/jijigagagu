#include "types.h"
#include "stat.h"
#include "user.h"

struct{
  seqloc lock;
  volatile int next; // who is to run next
  volatile int pass;
}frisbee;

int num_of_thread, num_of_pass;
volatile int f = 0;

void player(void *arg){
  int i = *(int *)arg;
  int r;
  unsigned long seq0, seq1;
  while(!f);
  while(frisbee.pass < num_of_pass){
    do{
      seq0 = frisbee.lock.seq;
      r = frisbee.next;
      seq1 = frisbee.lock.seq;
    }while(seq0 != seq1 || seq0 & 1);

    if (i == r && frisbee.pass < num_of_pass) {
          seqlock_acquire(&frisbee.lock);
          frisbee.pass ++;
          frisbee.next ++;
          frisbee.next = frisbee.next % num_of_thread;
          printf(1, "Pass number no: %d, Thread %d is passing the token to thread %d\n", frisbee.pass, i, frisbee.next);
          seqlock_release(&frisbee.lock);
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
  seqlock_init(&frisbee.lock);
  frisbee.next = 0;
  frisbee.pass = 0;

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
