#include <stdio.h>
#include<pthread.h>
#include <assert.h>
int counter1=0;
int MAX = 0; //count the number
int NUM_THREADS;
pthread_t tid[256];
int thread_args[256];
pthread_mutex_t lock;
void* counter(void* N);

int main() {

   printf("%s\n","Num of threads" );
   scanf("%d", &NUM_THREADS);

   //It creates the number of thread
   for (int i = 0; i < NUM_THREADS; i++) {
    printf("IN MAIN: Creating thread %d.\n", i);
    thread_args[i] = i;
    int result_code = pthread_create(&tid[i], NULL, counter, &thread_args[i]);
    assert(!result_code);
  }
// It ensures that the thread is done
  for (int i = 0; i < NUM_THREADS; i++) {
    int result_code = pthread_join(tid[i], NULL);
    assert(!result_code);
    printf("IN MAIN: Thread %d has ended.\n", i);
  }

  printf("MAIN program has ended.\n");


}
//while the the thread is not equal to the counter it will run Then we add 1 to the counterto led The next thread to run
//
void* counter(void* N){
  int tid = *((int *)N);
  while(tid != counter1){
  }
  printf("%d\n",tid );
  counter1++;
  }
