#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/resource.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
pid_t PID[8];
std::string statusArr[8];

void display(int size)
{
	for(int i=0;i<size;i++)
	{
		std::cout<<"Process: "<<i+1<<" | PID: "<<PID[i]<<" | Status: "<<statusArr[i]<<
			" | Priority: " <<getpriority(PRIO_PROCESS, PID[i])<<std::endl;
	}
}
int main()
	{
	double Step, end, left =1;
	int CountProc,status;
	//clock_t start,finish;
	creat("lab08test.txt",S_IRWXU);
	double total_time[8];
	while(1)
	{
		std::cout << "PID main process="<<getpid()<<std::endl;
		std::cout << "Tabulation will start from 0, enter end point: ";
		std::cin >> end;
		std::cout << "\nEnter step of tabulation: ";
		std::cin >> Step;
		std::cout << "\nEnter amount of process: ";
		std::cin >> CountProc;
		int iter = end/Step;
		int prociter = iter/CountProc;
		left = Step;
		for(int i=0;i<CountProc;i++)
		{
			PID[i]=fork();
			switch(PID[i])
			{
			case 0:
				execlp("//home//oleksandr/Desktop//tab","//home//oleksandr/Desktop//tab",
						std::to_string(left).c_str(),
						 std::to_string(Step).c_str(), std::to_string(prociter).c_str(), NULL
				);
				sleep(1);
				break;
			case -1:
				perror("Fork");
				break;
			default:
				//kill(PID[i],SIGSTOP);
				statusArr[i]="Running";
				sleep(1);
				break;
			}
			left += prociter*Step;
		}
		
		display(CountProc);
		bool val=true;
		while(val)
		{
			int choice;
			std::cout<<"\n\nQuit settings [0] | Change Priority [1] | Run Process [2] | Kill Process
				[3] | Suspend Process [4] | Change secirity atributes [5]\n";
			std::cin >> choice;
			switch(choice)
			{
				case 0:
					val=false;
					display(CountProc);
					break;
				case 1:
				{
					int num;
					int pr;
					std::cout << "\nEnter proces number: ";
					std::cin >> num;
					num--;
					std::cout << "\nEnter new prority: ";
					std::cin >> pr;
					setpriority(PRIO_PROCESS,PID[num],pr);
					display(CountProc);
					break;
				}
				case 2:
				{
					int index;
					std::cout<<"\nEnter process number: ";
					std::cin>>index;
					kill(PID[index-1],SIGCONT);
					statusArr[index-1]="Running";
					sleep(2);
					display(CountProc);
					break;
				}
				case 3:
				{
					int index;
					std::cout<<"\nEnter process number: ";
					std::cin>>index;
					kill(PID[index-1],SIGKILL);
					statusArr[index-1]="Killed";
					waitpid(PID[index-1],&status,0);
					sleep(1);
					display(CountProc);
					break;
				}
				case 4:
				{
					int index;
					std::cout<<"\nEnter process number: ";
					std::cin>>index;
					kill(PID[index-1],SIGSTOP);
					statusArr[index-1]="Suspended";
					sleep(1);
					display(CountProc);
					break;
				}
				case 5:
				{
					chmod("lab08test.txt", S_IRWXG);
					break;
				}
			}
		}
		for(int i=0;i<CountProc;i++)
		{
			kill(PID[i],SIGKILL);
			waitpid(PID[i],&status,0);
		}
		char choice;
		std::cout<<"\nTo exit press [Q] to continue and create new process press any other key ";
		std::cin>>choice;
		if (choice=='Q')
		{
			display(CountProc);
			return 0;
		}
	}
}
