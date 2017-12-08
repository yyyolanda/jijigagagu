#include "types.h"
#include "user.h"
#include "param.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"

int thread_create(void(*start_routine)(void *), void *arg){
  void *stack;
  int tid;

  stack = malloc(2 * PGSIZE); // set up the new thread stack

  if ((uint)stack % PGSIZE){
    stack = stack + (PGSIZE - ((uint)stack % PGSIZE));
  }

  if ((tid = clone((void *)stack)) != 0){
    return tid;
  }

  start_routine(arg);
  exit();
  return tid;
}

/*spinlock*/
void spinlock_init(sploc *lock){
    lock->locked = 0;
}

void spinlock_acquire(sploc *lock){
    while(xchg(&lock->locked,1) != 0);
}

void spinlock_release(sploc *lock){
    xchg(&lock->locked,0);
}

/*arraybasedlock*/
void arrlock_init(arrloc *lock){
    int i;
    for(i = 0; i < MAXTHREADN; i++)
        lock->is_available[i] = 0;
    lock->is_available[0] = 1;
}

void arrlock_acquire(arrloc *lock, int i){
    while(xchg(&lock->is_available[i],0) != 1);
    //while(&lock->can_serve[me] == 0);
}

void arrlock_release(arrloc *lock, int i, int num){ //thread in use
    xchg(&lock->is_available[i],0);
    xchg(&lock->is_available[(i+1)%num],1);
}

/*seqlock*/
void seqlock_init(seqloc *lock){
    lock->locked = 0;
    lock->seq = 0;
}

void seqlock_acquire(seqloc *lock){
    while(xchg(&lock->locked,1) != 0);
    lock->seq ++;
}

void seqlock_release(seqloc *lock){
    lock->seq ++;
    xchg(&lock->locked,0);
}
