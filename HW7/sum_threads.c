#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define PARTS 4

int *array;
long sum = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int from;
    int to;
} range;

void *worker(void *arg) {
    range *r = arg;
    long part = 0;
    for (int i = r->from; i < r->to; i++)
        part += array[i];

    pthread_mutex_lock(&mutex);
    sum += part;
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(void) {
    int n;
    printf("Enter N: ");
    scanf("%d", &n);

    array = malloc(n * sizeof(int));
    if (array == NULL)
        return 1;

    for (int i = 0; i < n; i++)
        array[i] = rand() % 100;

    printf("Array:");
    for (int i = 0; i < n; i++)
        printf(" %d", array[i]);
    printf("\n");

    pthread_t threads[PARTS];
    range ranges[PARTS];
    const int step = n / PARTS;
    for (int i = 0; i < PARTS; i++) {
        ranges[i].from = i * step;
        ranges[i].to = i == PARTS - 1 ? n : (i + 1) * step;
        pthread_create(&threads[i], NULL, worker, &ranges[i]);
    }

    for (int i = 0; i < PARTS; i++)
        pthread_join(threads[i], NULL);

    printf("Sum: %ld\n", sum);

    free(array);
    return 0;
}
