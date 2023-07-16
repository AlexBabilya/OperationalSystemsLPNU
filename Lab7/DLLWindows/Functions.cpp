#include "pch.h"
#include "functions.h"

void outPut( int size, std::string data)
{
	std::ofstream file("test.txt",std::ios::app);
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < data.length(); ++j)
			file << data[j];
	file.close();
}
