#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#define NUM 5

sem_t forks[NUM]; // forks
sem_t once;
sem_t eat;

int eat_cnt[NUM]; 

void pickup(int philosopher_num){
	sem_wait(&forks[philosopher_num % NUM]);
}

void putdown(int philosopher_num){
	sem_post(&forks[philosopher_num % NUM]);
}

void thinking(int philosopher_num){
	printf("philosopher %d is thinking\n", philosopher_num);
	return;
}

void eating(int philosopher_num){
	printf("philosopher %d is eating\n", philosopher_num);
        eat_cnt[philosopher_num]++;
	return;
}

void *philosopher(void *arg){
	int philosopher_num;
	philosopher_num = (unsigned long int) arg;

	while(1){
		sem_wait(&once);
		pickup(philosopher_num);
		printf("philosopher %d picks up the fork %d.\n", philosopher_num, philosopher_num);
		pickup(philosopher_num + 1);
		printf("philosopher %d picks up the fork %d.\n", philosopher_num, (philosopher_num + 1) % NUM);
		sem_post(&once);
		eating(philosopher_num);
		putdown(philosopher_num + 1);
		printf("philosopher %d puts down the fork %d.\n", philosopher_num, (philosopher_num + 1) % NUM);
		putdown(philosopher_num);
		printf("philosopher %d puts down the fork %d.\n", philosopher_num, philosopher_num);
		thinking(philosopher_num);
	}
	return NULL;
}

int main(){
	pthread_t threads[NUM];
	for(int i=0; i<NUM; i++){
		sem_init(&forks[i], 0, 1);
	}
	sem_init(&once, 0, 1);
	sem_init(&eat, 0, 1);
	time_t start_time = time(NULL);
	for(unsigned long int i=0; i<NUM; i++){
		pthread_create(&threads[i], NULL, philosopher, (void*)i);
	}

	while(time(NULL) - start_time < 600);
	//for(int i=0; i<NUM; i++){
	//	pthread_join(threads[i], NULL);
	//}
	sem_wait(&eat);
	int total = 0;
	for(int i = 0; i < NUM; i++){
		printf("Philosopher %d dined %d times.\n", i, eat_cnt[i]);
		total += eat_cnt[i];
	}
	printf("Hold & Wait\n");
	printf("Total number of dining is %d\n", total);
	printf("NO DEADLOCK\n");
	return 0;
}
