#include "lib.h"
#include <stdio.h>
#include <dlfcn.h>

int main()
{
	void *ext_library;
	void (*func)(char*, int, int);
	ext_library = dlopen("./libDynamicLib.so", RTLD_LAZY);
	
	if(!ext_library)
	{
		printf("Error");
		return 0;
	}
	
	func = (void (*)(char*, int ,int))dlsym(ext_library,"outPut");
	
	if(!func)
	{
		printf("Error");
		return 0;
	}
	
	char *str ="Babilya Oleksandr 13503802\n";
	func(str,28,100);
	return 0;
}
