#include <iostream>
#include "functions.h"
#include <fstream>
#include <Windows.h>
#include <vector>
#include <chrono>

std::string data = " Oleksandr Babilya 13503821";

void main()
{
	int numberOfIterations;
	std::cout << "Enter number of iterations: ";
	std::cin >> numberOfIterations;
	outPut(numberOfIterations, data);
}
