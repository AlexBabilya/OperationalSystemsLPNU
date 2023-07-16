#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/resource.h>
#include <string.h>
#include <barrier>
#include <fstream>
#include <chrono>
#include <vector>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_barrier_t barrier_сs;
pthread_t* HANDLES;
int ready = 0;
std::string str = "Babilya Oleksandr 13503802\n";
std::ofstream noSyncFile("noSync.txt", std::ios::app),
	condVarFile("condVar.txt", std::ios::app), barrierFile("barrier.txt", std::ios::app);
std::vector<std::string> status;

void* OutPutCondVar(void* param)
{
	pthread_mutex_lock(&lock);
	auto start = std::chrono::high_resolution_clock::now();
	ready = 1;
	int iterations = *(int*)param;
	
	for(int i = 0; i < iterations; ++i)
	{
		for(int j =0 ; j < 27; ++j )
		{
		condVarFile << str[j];
		}
	}
	
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> dur = end -start;
	std::cout <<"Thread id: "<< gettid() << "Time: " << dur.count()<<std::endl;
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&lock);
	return nullptr;
}

void* OutPutBarrier(void* param)
{
	pthread_barrier_wait(&barrier_сs);
	auto start = std::chrono::high_resolution_clock::now();
	int iterations = *(int*)param;
	
	for(int i = 0; i < iterations; ++i)
	{
		for(int j =0 ; j < 27; ++j )
		{
		barrierFile << str[j];
		}
	}
	
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> dur = end -start;
	std::cout <<"Thread id: "<< gettid() << "Time: " << dur.count()<<std::endl;
	return 0;
}

void* OutPutNoSync(void* param)
	{
	int iterations = *(int *)param;
	auto start = std::chrono::high_resolution_clock::now();
	
	for(int i=0;i<iterations;i++)
		for(int j = 0;j<27;++j)
			noSyncFile<<str[j];
			
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> dur = end -start;
	std::cout <<"Thread id: "<< gettid() << "Time: " << dur.count()<<std::endl;
	return nullptr;
}

int main()
{
	std::cout << "Iterations, threads";
	int iterations, threads;
	std::cin >> iterations ;
	std::cin >> threads;
	std::cout << "Choose synch method:\nCond Var[1] | Barrier[2]\n";
	int *thr_iter = new int;
	*thr_iter = iterations/threads;
	HANDLES = new pthread_t[threads];
	int sync;
	std::cin >> sync;
	
	switch(sync)
	{
		case (0):
		{
			for(int i = 0 ; i < threads; i++)
			{
				pthread_create(&HANDLES[i],NULL,OutPutNoSync,(void*)thr_iter);5
				status.push_back("Running");
			}
			break;
		}
		case(1):
		{
			pthread_mutex_lock(&lock);
			for(int i = 0 ; i < threads; i++)
			{
				pthread_create(&HANDLES[i],NULL,OutPutCondVar,(void*)thr_iter);
				status.push_back("Running");
			}
			while(ready == 0)
				pthread_cond_wait(&cond, &lock);
				
			pthread_mutex_unlock(&lock);
			break;
		}
		case(2):
		{
			pthread_barrier_init(&barrier_сs, NULL, 1);
			for(int i = 0 ; i < threads; i++)
			{
				pthread_create(&HANDLES[i],NULL,OutPutBarrier,(void*)thr_iter);
				status.push_back("Running");
			}
			break;
		}
	}
	
	sleep(1);
	bool run = true;
	
	while (run)
	{
		std::cout<<"Choose option: Quit[0] | Change prio[1] | Detach[2] | Cancel[3] | Change affinity[4]";
		int var;
		std::cin >> var;
		switch (var)
		{
			case (0):
			{
				pthread_barrier_destroy(&barrier_сs);
				run = false;
				break;
			}
			case (1):
			{
				int index, prio;
				std::cout << "Enter index: ";
				std::cin >> index;
				std::cout << "Enter prio: ";
				std::cin >> prio;
				struct sched_param param;
				param.sched_priority = prio;
				pthread_setschedparam(HANDLES[index],SCHED_RR, &param);
				break;
			}
			case (2):
			{
				int index;
				std::cout << "Enter index: ";
				std::cin >> index;
				pthread_detach(HANDLES[index]);6
				status[index-1]="Detached";
				break;
			}
			case (3):
			{
				int index;
				std::cout << "Enter index: ";
				std::cin >> index;
				status[index-1]="Canceled";
				pthread_cancel(HANDLES[index]);
				break;
			}
		}
	}
	return 0;
}
