#include "lib.h"

void outPut(char* str, int sizeOfString, int iter)
{
	for(int i=0;i<iter;i++)
		for(int j=0;j<sizeOfString;j++)
			printf("%c", str[j]);
}
