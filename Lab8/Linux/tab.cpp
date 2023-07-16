#include <stdio.h>
#include <iostream>
#include <math.h>
#include <sys/times.h>
#include <sys/resource.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cmath>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>

double fact(double n)
{
	double result=n;
	for(int i=1;i<=n;++i)
		result*=i;
	return result;
}

int main(int argc,char *argv[])
{
	sleep(1);
	double left = atof(argv[1]);
	double Step = atof(argv[2]);
	int iters = atof(argv[3]);
	double FuncResult;
	clock_t timeBegin = clock();
	
	for(double i= 1; i < iters; ++i)
	{
		FuncResult = pow(-1.0,i)*pow(left,2.0*i)/fact(2.0*i);
		left += Step;
	}
	
	open("lab08test.txt",S_IRWXU);
	std::ofstream file("lab08test.txt", std::ios::app);
	clock_t timeEnd = clock();
	clock_t timeWork = timeEnd - timeBegin;
	std::cout << "x - " << left <<" y - " << FuncResult <<std::endl;
	std::cout<< "\nProcess ID: "<<getpid()<<" \nProcess Time= "<< double(timeWork)/10000.0<<std::endl;
	file << "x - " << left <<" y - " << FuncResult <<std::endl
		 << "\nProcess ID: "<<getpid()<<" \nProcess Time= "<< double(timeWork)/10000.0<<std::endl;
	file.close();
	return 0;
}
