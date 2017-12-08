#include "types.h"
#include "stat.h"
#include "user.h"

void player(void *arg){

    sleep(100);
    printf(1, "%s\n", (char *)arg);
    exit();
}

int main(int argc, char const *argv[]) {
    thread_create(player, "thread 1");
    thread_create(player, "thread 2");
    thread_create(player, "thread 3");
    while(wait()>=0);
    printf(0, "back to parent\n");
    exit();
}
