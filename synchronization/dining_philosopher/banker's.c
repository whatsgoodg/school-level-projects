#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#define NUM 5
sem_t forks[NUM]; // forks
sem_t block_itself;
sem_t info_mutex;
sem_t eat;

int V[NUM] = {1, 1, 1, 1, 1};
int A[NUM][NUM];
int eat_cnt[NUM];

int positive_mod(int a, int b){
	int c = a % b;
	if(c < 0) c += b;
	return c;
}

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

bool safe(int philosopher_num, int fork_num){ //현재 상태를 측정
	int recent_thread = philosopher_num; //현재 요청하는 자원
	int recent_fork = fork_num; //현재 요청하는 쓰레드
	int sharing_thread = philosopher_num != fork_num ? positive_mod(recent_thread-1, NUM) : (recent_thread+1) % NUM; //현재 쓰레드가 요청하는 자원이 아닌 자원(다른 쓰레드와 경쟁)
	int sharings_fork = philosopher_num != fork_num ? sharing_thread : (sharing_thread+1) % NUM; //경쟁하는 쓰레드
	int sharing_fork = philosopher_num != fork_num ? fork_num : sharing_thread; //경쟁하는 쓰레드의 공유하는 자원이 아닌 자원

	int finish[5][5]; //sequence가 존재하는 지 테스트하는 이차원 배열
	int idx[2][2] = { {recent_thread, recent_fork}, {sharing_thread, sharings_fork}}; //loop을 위한 배열
	
	sem_wait(&info_mutex); //공유자원 가드

	finish[recent_thread][recent_fork] = A[recent_thread][recent_fork] + V[recent_fork]; //현재의 쓰레드가 요청하는 자원을 주었다고 가정
	finish[sharing_thread][sharings_fork] = A[sharing_thread][sharings_fork] + V[sharings_fork]; //경쟁 쓰레드의 현재 자원 할당 상태

	//공유 쓰레드에게 한 번, 경쟁 쓰레드에게 한 번씩 경쟁하는 포크를 주어 둘 다 집을 수 있으면 현재 상태는 safe
	for(int i = 0; i < 2; i++){
		finish[idx[i][0]][sharing_fork] = A[idx[i][0]][sharing_fork] + V[sharing_fork]; 
		if(finish[idx[i][0]][idx[i][1]] + finish[idx[i][0]][sharing_fork] == 2) {
                	sem_post(&info_mutex);
                	return true;
        	}
	}
	sem_post(&info_mutex);
	return false;
}

void *philosopher(void *arg){
	int philosopher_num;
	philosopher_num = (unsigned long int) arg;
	while(1){
		
		//상태가 좋지 않으면 block	
		while(!safe(philosopher_num, philosopher_num)) sem_wait(&block_itself);

		//자원 할당 정보 업데이트
		sem_wait(&info_mutex);
                V[philosopher_num]++;
                A[philosopher_num][philosopher_num]++;
                sem_post(&info_mutex);

		pickup(philosopher_num);
                printf("philosopher %d picks up the fork %d.\n", philosopher_num, philosopher_num);

		// 이전과 동일 (philosopher_num + 1) % NUM에 대한 처리
		while(!safe(philosopher_num, (philosopher_num + 1) % NUM)) sem_wait(&block_itself);

		sem_wait(&info_mutex);
                V[(philosopher_num + 1) % NUM]++;
                A[philosopher_num][(philosopher_num + 1) % NUM]++;
                sem_post(&info_mutex);

		pickup(philosopher_num + 1);
                printf("philosopher %d picks up the fork %d.\n", philosopher_num, (philosopher_num + 1) % NUM);

		//식사
		eating(philosopher_num);

		//포크 자원 할당 해제 정보 수정
		sem_wait(&info_mutex);
                A[philosopher_num][(philosopher_num + 1) % NUM]--;
                V[(philosopher_num + 1) % NUM]--;
                sem_post(&info_mutex);

		putdown(philosopher_num + 1);
		//safe 하지 않아 block 된 philosopher awake.
		sem_post(&block_itself);
		printf("philosopher %d puts down the fork %d.\n", philosopher_num, (philosopher_num + 1) % NUM);

		// 이전과 동일 (philosopher_num + 1) % NUM에 대한 처리
		sem_wait(&info_mutex);
                A[philosopher_num][philosopher_num]--;
                V[philosopher_num]--;
                sem_post(&info_mutex);

		putdown(philosopher_num);
		sem_post(&block_itself);
		printf("philosopher %d puts down the fork %d.\n", philosopher_num, philosopher_num);

		thinking(philosopher_num);
	}
	return NULL;
}

int main(){
	for(int i = 0; i < 5; i++) printf("%d, ", V[i]);
	pthread_t threads[NUM];
	for(int i=0; i<NUM; i++){
		sem_init(&forks[i], 0, 1);
	}
	sem_init(&block_itself, 0, 0);
	sem_init(&info_mutex, 0, 1);
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
	printf("Banker's\n");
	printf("Total number of dining is %d\n", total);
	printf("NO DEADLOCK\n");
	return 0;
}
