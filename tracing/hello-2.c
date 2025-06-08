// hello_trace.c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* thread_func(void* arg) {
    for (int i = 0; i < 1000; ++i)
        sched_yield();  // 強制 context switch
    return NULL;
}

int main() {
    pthread_t t;
    printf("Start tracing demo\n");
    pthread_create(&t, NULL, thread_func, NULL);
    write(1, "syscall write\n", 14);  // sys_enter_write
    pthread_join(t, NULL);
    return 0;
}