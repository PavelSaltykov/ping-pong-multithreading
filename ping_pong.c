#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void ping()
{
    pthread_mutex_lock(&mutex);
    while (1)
    {
        printf("ping\n");
        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond, &mutex);
        sleep(1);
    }
}

void pong()
{
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);
    while (1)
    {
        pthread_cond_wait(&cond, &mutex);
        sleep(1);
        printf("pong\n");
        pthread_cond_signal(&cond);
    }
}

int main()
{
    pthread_t ping_thread;
    pthread_t pong_thread;

    pthread_mutex_lock(&mutex);
    pthread_create(&pong_thread, NULL, (void *)&pong, NULL);
    pthread_cond_wait(&cond, &mutex);

    pthread_create(&ping_thread, NULL, (void *)&ping, NULL);
    pthread_mutex_unlock(&mutex);

    pthread_join(ping_thread, NULL);
    return 0;
}
