#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


#define TRUE 1

buffer_item buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

int insertPointer = 0, removePointer = 0;

void *producer(void *param);
void *consumer(void *param);

//insert item into buffer, return 0 if successful, otherwise return -1
int insert_item(buffer_item item)
{
//Empty Semaphore
sem_wait(&empty);

//mutex lock 
pthread_mutex_lock(&mutex);

//Insert the item and move the pointer
buffer[insertPointer]=item;
insertPointer= (insertPointer + 1) % BUFFER_SIZE;

//Release mutex lock and full semaphore
pthread_mutex_unlock(&mutex);
sem_post(&full);
return 0; //success
}

//remove an object from buffer placing it in item return 0 if successful, otherwise return -1
int remove_item(buffer_item *item)
{
//Full Semaphore
sem_wait(&full);

//mutex lock to protect buffer
pthread_mutex_lock(&mutex);

//Remove the item and move the pointer
*item = buffer[insertPointer];
buffer[insertPointer] = 0;
insertPointer = (insertPointer -1) % BUFFER_SIZE;

pthread_mutex_unlock(&mutex);
sem_post(&empty);
return 0;
}


int main(int argc, char *argv[])
{
int sleepTime, producerThreads, consumerThreads, START_NUMBER;
int i, j;

if(argc != 5)
{
fprintf(stderr, "Useage: <sleep time> <producer threads> <consumer threads> <start number>n");
return -1;
}

sleepTime = atoi(argv[1]);
producerThreads = atoi(argv[2]);
consumerThreads = atoi(argv[3]);
START_NUMBER = atoi(argv[4]);

//Initialize the the locks
printf("%d\n",pthread_mutex_init(&mutex, NULL));
printf("%d\n",sem_init(&empty, 0, BUFFER_SIZE));
printf("%d\n",sem_init(&full, 0, 0));
srand(time(0));

//Create the producer and consumer threads
for(i = 0; i < producerThreads; i++)
{
pthread_t tid;
pthread_attr_t attr;
pthread_attr_init(&attr);
pthread_create(&tid, &attr, producer, NULL);
}

for(j = 0; j < consumerThreads; j++)
{
pthread_t tid;
pthread_attr_t attr;
pthread_attr_init(&attr);
pthread_create(&tid, &attr, consumer, NULL);
}

//Sleep for user specified time
sleep(sleepTime);
return 0;
}

void *producer(void *param)
{
buffer_item random;
int r;

while(TRUE)
{
r = rand() % BUFFER_SIZE;
sleep(r);
random = rand();

if(insert_item(random))
fprintf(stderr, "Error");

printf("Producer produced %d \n", random);

}

}

void *consumer(void *param)
{
buffer_item random;
int r;

while(TRUE)
{
r = rand() % BUFFER_SIZE;
sleep(r);

if(remove_item(&random))
fprintf(stderr, "Error Consuming");
else if (random != 0) 
printf("Consumer consumed %d \n", random);
}
}


