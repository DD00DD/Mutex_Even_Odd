#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  condition_var = PTHREAD_COND_INITIALIZER;

void *functionOdd();
void *functionEven();
int  count = 0;
int total = 0;
#define COUNT_DONE 10

int main()
{
   pthread_t thread1, thread2;

   printf("Starting total: %d\n", total);

   pthread_create( &thread1, NULL, &functionEven, NULL);
   pthread_create( &thread2, NULL, &functionOdd, NULL);

   pthread_join(thread1, NULL);
   pthread_join(thread2, NULL);


   printf("Final total: %d\n", total);


   exit(0);
}


void *functionEven()
{
   while (count+1 < COUNT_DONE)
   {
      // Lock mutex and then wait for signal to relase mutex
      pthread_mutex_lock( &count_mutex );

      // Wait while functionOdd() operates on count and total
      // mutex unlocked if condition varialbe in functionOdd() signaled.
      pthread_cond_wait( &condition_var, &count_mutex );
      count++;
      total = total + 2;
      printf("Total value functionEven: %d\n", total);

      pthread_mutex_unlock( &count_mutex );

      if(count > COUNT_DONE) return(NULL);
    }
}


void *functionOdd()
{
    while (count+1 < COUNT_DONE)
    {
       pthread_mutex_lock( &count_mutex );

       if( (count % 2) == 0)
       {
          // Condition of if statement has been met. 
          // Signal to free waiting thread by freeing the mutex.
          // Note: functionEven() is now permitted to modify "count" and "total".
          pthread_cond_signal( &condition_var );
       }
       else
       {
          count++;
          total++;
          printf("Total value functionOdd:  %d\n", total);
       }

       pthread_mutex_unlock( &count_mutex );

       if(count > COUNT_DONE) return(NULL);
    }
}
