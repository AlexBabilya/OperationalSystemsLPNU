#include <iostream>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <chrono>

void (*func) (int, std::string);
std::string data = " Oleksandr Babilya 13503821";

void main()
{
	HINSTANCE h;
	std::string str = "C:\\Users\\home\\source\\repos\\dllOS7\\Debug\\dllOS7.dll";
	std::wstring stemp = std::wstring(str.begin(), str.end());
	LPCWSTR sw = stemp.c_str();
	h = LoadLibrary(sw);
	
	if (h==NULL)
	{
		std::cout<<"Error12 \n";
		return;
	}
	
	func = (void (*) (int, std::string))GetProcAddress(h, "outPut");
	if (!func)
	{
		printf("No such func \ n");
		return;
	}
	
	int numberOfIterations;
	std::cout << "Enter number of iterations: ";
	std::cin >> numberOfIterations;
	func(numberOfIterations, data);
	FreeLibrary(h);
}
