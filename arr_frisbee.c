#include "types.h"
#include "stat.h"
#include "user.h"

struct{
  arrloc lock;
  volatile int next; // who is to run next
  volatile int pass;
}frisbee;

int num_of_thread, num_of_pass;
volatile int f = 0;

void player(void *arg){
  int i = *(int *)arg;
  while(!f);
  while(frisbee.pass < num_of_pass){
    arrlock_acquire(&frisbee.lock, i);
    if(frisbee.pass >= num_of_pass) {
        arrlock_release(&frisbee.lock,i,num_of_thread);
	break;
    }
    frisbee.pass ++;
    printf(1, "Pass number no: %d, Thread %d is passing the token to thread %d\n",frisbee.pass, i, i+1);
    arrlock_release(&frisbee.lock,i,num_of_thread);
  }
  exit();
}

int main(int argc, char const *argv[])
{
  int i;
  int t0;
  if (argc != 3) {
      printf(1, "wrong args!!!\n" );
      exit();
  }
  num_of_thread = atoi(argv[1]);
  num_of_pass = atoi(argv[2]);
  arrlock_init(&frisbee.lock);
  frisbee.pass = 0;

  for (i = 0; i < num_of_thread; i++) {
        thread_create(player, (void *)&i);
        sleep(10);
  }
  f = 1;
  t0 = uptime();
  while(wait()>=0);
  printf(1, "\nSimulation of ARRLOCK Frisbee game has finished, %d rounds were played in total!\n", num_of_pass);
  printf(1, "total exec time = %d\n", uptime()-t0);
  exit();
}
