#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int n;
static pthread_mutex_t* locks;

void cudaMemcpy()
{

  sleep(500);
}


int compute_sums()
{
  cudaMemcpy();
}


void* prefix_sum(void* p) 
{
  int self = * ((int*) p);
  printf ("I, %d, have just woken from contemplating existence\n", self);
  pthread_mutex_lock(locks + self);   

  printf ("I, %d, have picked up my left fork.  I must have another nap\n", self);
  usleep(rand() % (n * 100));  

 
  pthread_mutex_lock(locks + ((self + 1) % n));
  printf ("I, %d, have picked up my right fork\n", self);

  pthread_mutex_unlock(locks + ((self + 1) % n));
  pthread_mutex_unlock(locks + self);   
  
  sleep(1000);
  printf ("I have dined spendidly this evening %d\n", self);

  return NULL;
}

int main(int argc, char** argv)
{
  
  int i;
  pthread_t* threads;
  pthread_attr_t* thread_attrs;
  int *data;

  if (argc > 1) 
    {
      n = atoi(argv[1]);
    }
  else n = 5;

  printf ("Creating %d hungry philosophers\n", n);

  threads = calloc(sizeof(pthread_t), n);
  thread_attrs = calloc(sizeof(pthread_attr_t), n);
  data = calloc(sizeof(int), n); 
  locks = calloc(sizeof(pthread_mutex_t), n);
  
  for (i = 0; i < n ; i++)
    {
      pthread_attr_init(thread_attrs + i);
      pthread_mutex_init(locks + i, NULL);
      data[i] = i;
    }
  for (i = 0; i < n ; i++)
    {
      pthread_create(threads + i, thread_attrs + i, &prefix_sum, data + i);
    }
  compute_sums();
  while (--i > 0)
    {
      pthread_join(threads[i], NULL);
    }
  

}

