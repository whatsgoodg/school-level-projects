#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#define ITER 1000

int reader_count, write_count;
int read_count[20];
sem_t x, t, wsem;

void *Read(void *arg);
void *Write(void *arg);

int main(){
	pthread_t threads[20];
	sem_init(&x, 0, 1);
	sem_init(&wsem, 0, 1);
	sem_init(&t, 0, 1);
	time_t start_time = time(NULL);

	for(int i = 0; i < 10; i++){
                pthread_create(&threads[i], NULL, Write, NULL);
        }
        for(unsigned long int i = 0; i < 5; i++){
                pthread_create(&threads[i], NULL, Read, (void*)i);
        }

	while(time(NULL) - start_time < 1);

	int sum = 0;
	for(int i = 0; i < 20; i++) sum += read_count[i];
	printf("Reader read %d times.\n", sum);
	printf("Writer wrote %d times.\n", write_count);	
	return 0;

}

void *Write(void *arg){
	while(1){
		sem_wait(&wsem);
		write_count++;
		sem_post(&wsem);	
	}
}

void *Read(void *arg){
	while(1){
		sem_wait(&x);
		reader_count++;
		if(reader_count == 1) sem_wait(&wsem);
		sem_post(&x);

		read_count[(unsigned long int)arg]++;
		
		sem_wait(&x);
		reader_count--;
		if(reader_count == 0) sem_post(&wsem);
		sem_post(&x);

        }
}
