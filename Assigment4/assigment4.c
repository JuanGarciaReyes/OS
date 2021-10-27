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
   // printf() displays the string inside quotation
   //counter(10000);
   printf("%s\n","Num of threads" );
   scanf("%d", &NUM_THREADS);
   printf("%s\n","Nums" );
   scanf("%d", &MAX);

   for (int i = 0; i < NUM_THREADS; i++) {
    printf("IN MAIN: Creating thread %d.\n", i);
    thread_args[i] = MAX;
    int result_code = pthread_create(&tid[i], NULL, counter, &thread_args[i]);
    assert(!result_code);
  }
  for (int i = 0; i < NUM_THREADS; i++) {
    int result_code = pthread_join(tid[i], NULL);
    assert(!result_code);
    printf("IN MAIN: Thread %d has ended.\n", i);
  }

  printf("MAIN program has ended.\n");
   printf("%d\n", counter1 );


}

void* counter(void* N){
  int max = *((int *)N);
  for(int i =0;i<max;i++){
    pthread_mutex_lock(&lock);
    counter1++;
    pthread_mutex_unlock(&lock);

  }
}
