#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#define ITER 1000
#define MAX 200

void *thread_increment(void *arg);
void *thread_decrement(void *arg);
int x;
sem_t empty, fill, m;

int main() {
	pthread_t tid1, tid2;
	//최대 MAX번 lock 가능
	sem_init(&empty, 0, MAX);
	// producer가 무조건 awaek 해줘야 하기 때문에 대기.
	sem_init(&fill, 0, 0);
	sem_init(&m, 0, 1);

	pthread_create(&tid1, NULL, thread_increment, NULL);
	pthread_create(&tid2, NULL, thread_decrement, NULL);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	if (x != 0)
		printf("BOOM! counter=%d\n", x);
	else
		printf("OK counter=%d\n", x);
}

/* thread routine */
void * thread_increment (void *arg) {
	int i, val;
	for (i=0; i< ITER ; i++) {
		//producer 
		sem_wait(&empty); // 최대 MAX 번 lock 가능 
		sem_wait(&m); // 공유자원 guard
		val = x;
		printf("%u: %d\n", (unsigned int) pthread_self(), val);
		x = val + 1;
		sem_post(&m); 
		sem_post(&fill); //produce 했으니, consumer가 소비해야함.
	}
	return NULL;
}

void * thread_decrement (void *arg) {
	int i, val;
	for (i=0; i< ITER ; i++) {
		sem_wait(&fill); //producter가 awake시 소비
		sem_wait(&m); //공유자원 guard
		val = x;
		printf("%u: %d\n", (unsigned int) pthread_self(), val);
		x = val - 1;
		sem_post(&m); 
		sem_post(&empty); //MAX번 이상 produce 있다면 block 되어있기에, 소비하나 했으니 awake.
	}
	return NULL;
}
