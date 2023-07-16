#include <iostream>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[])
	{
	int N = atoll(argv[1]), M = atoll(argv[2]);
	//int N = 3, M = 3;
	std::vector<std::vector<int>> matrix(N);
	srand(static_cast<unsigned int>(time(0)));
	
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			matrix[i].push_back(rand() % 1000);
	/*for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
			std::cout << matrix[i][j] << ' ';
		std::cout << std::endl;
	}*/
	
	int count = 0;
	for (int i = 0; i < N; i++)
		count += std::count(matrix[i].begin(), matrix[i].end(), matrix[0][0]);
		
	std::cout << count;
	_getch();
	return 0;
}
