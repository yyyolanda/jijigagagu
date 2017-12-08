#include "types.h"
#include "stat.h"
#include "user.h"

struct{
  sploc lock;
  volatile int next; // who is to run next
  volatile int pass;
}frisbee;

int num_of_thread, num_of_pass;
volatile int f = 0;

void player(void *arg){
  int i = *(int *)arg;
  while(!f);
  while(frisbee.pass < num_of_pass){
    spinlock_acquire(&frisbee.lock);
    if(i == frisbee.next && frisbee.pass < num_of_pass){
      frisbee.pass ++;
      frisbee.next ++;
      frisbee.next = frisbee.next % num_of_thread;
      printf(1, "Pass number no: %d, Thread %d is passing the token to thread %d\n",frisbee.pass, i, frisbee.next);
    }
    spinlock_release(&frisbee.lock);
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
  spinlock_init(&frisbee.lock);
  frisbee.next = 0;
  frisbee.pass = 0;

  for (i = 0; i < num_of_thread; i++) {
        thread_create(player, (void *)&i);
        sleep(10);
  }
  f = 1;
  t0 = uptime();
  while(wait()>=0);
  printf(1, "\nSimulation of SPINLOCK Frisbee game has finished, %d rounds were played in total!\n", num_of_pass);
  printf(1, "total exec time = %d\n", uptime()-t0);
  exit();
}
