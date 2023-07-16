#pragma once
#include <iostream>
#include <fstream>
#include <Windows.h>

#ifdef DLLOS7_EXPORTS

#define DLLOS7_EXPORTS_API extern "C" _declspec(dllexport)

#else
#define DLLOS7_EXPORTS_API extern "C" _declspec(dllimport)

#endif // DLLOS7_EXPORTS

DLLOS7_EXPORTS_API void outPut(int , std::string );
