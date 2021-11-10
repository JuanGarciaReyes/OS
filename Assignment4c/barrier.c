#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common_threads.h"

// If done correctly, each child should print their "before" message
// before either prints their "after" message. Test by adding sleep(1)
// calls in various locations.

// You likely need two semaphores to do this correctly, and some
// other integers to track things.

typedef struct __barrier_t {
    // add semaphores and other information here
    sem_t t1, t2;
    sem_t mutex;
    int count;
    int no_ocur;

} barrier_t;


// the single barrier we are using for this program
barrier_t b;

void barrier_init(barrier_t *b, int num_threads) {
    // initialization code goes here
    //COUNTER
    b-> count = 0;
    //No of threads happening
    b-> no_ocur = num_threads;
    Sem_init(&b-> t1,0);
    Sem_init(&b-> t2,0);
    Sem_init(&b-> mutex, 1);

}

void barrier(barrier_t *b) {
  //locks the count and increments
    Sem_wait(&b->mutex);
    b-> count++;
    //if the count equals the num of threads
    if(b-> count == b-> no_ocur){
      for(int i = 0; i< b-> no_ocur; i++){
  //releases the t1
        Sem_post(&b-> t1);
      }
    }
    //releases the counter
    Sem_post(&b->mutex);
    //locks t1
    Sem_wait(&b->t1);
    Sem_wait(&b->mutex);
    //locks the count and increments
    b-> count--;
    //if the count equals the num of threads
    if(b-> count == 0){
      for(int i = 0; i< b-> no_ocur; i++){
        Sem_post(&b-> t2);
      }
    }
    //releases the MUTEX and t2
    Sem_post(&b->mutex);
    Sem_wait(&b->t2);

    // barrier code goes here
}

//
// XXX: don't change below here (just run it!)
//
typedef struct __tinfo_t {
    int thread_id;
} tinfo_t;

void *child(void *arg) {
    tinfo_t *t = (tinfo_t *) arg;
    printf("child %d: before\n", t->thread_id);
    barrier(&b);
    printf("child %d: after\n", t->thread_id);
    return NULL;
}


// run with a single argument indicating the number of
// threads you wish to create (1 or more)
int main(int argc, char *argv[]) {
    assert(argc == 2);
    int num_threads = atoi(argv[1]);
    assert(num_threads > 0);

    pthread_t p[num_threads];
    tinfo_t t[num_threads];

    printf("parent: begin\n");
    barrier_init(&b, num_threads);

    int i;
    for (i = 0; i < num_threads; i++) {
	t[i].thread_id = i;
	Pthread_create(&p[i], NULL, child, &t[i]);
    }

    for (i = 0; i < num_threads; i++)
	Pthread_join(p[i], NULL);

    printf("parent: end\n");
    return 0;
}
