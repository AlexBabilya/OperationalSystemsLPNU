#include <iostream>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <aclapi.h>

int main(int argc, char* argv[])
{
    int N = atoll(argv[1]), M = atoll(argv[2]), K = atoll(argv[3]), L = atoll(argv[4]);
    PACL pACL = NULL;
    HANDLE myhandle;
    DWORD mythreadid;

    myhandle = CreateFile(L"text.txt", GENERIC_ALL, FILE_SHARE_WRITE, NULL, CREATE_NEW,FILE_ATTRIBUTE_NORMAL, NULL);
    std::vector<std::vector<int>> matrix(N);
    srand(static_cast<unsigned int>(time(0)));
    std::ifstream myfile("D:\\QTReal\\Apps\\build-OSLab-Qt_6_2_4_mingw_64_a9d8f5-Debug\\matrix.txt");
    std::ofstream file("text.txt",std::ios::app);
    int val;
    file << "Process ID:" << GetCurrentProcessId() << std::endl << "Reading matrix..." << std::endl;
    std::cout << "Process ID:" << GetCurrentProcessId() << std::endl << "Reading matrix..."<<std::endl;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
        {
        myfile >> val;
        matrix[i].push_back(val);
        }

    file << std::endl << "Finding how much apears " << matrix[0][0]
         << " in matrix " << N << "x" << M << std::endl << "In range from: " << K << " to" 
         << L << std::endl << std::endl << "Processing...";

    std::cout << std::endl << "Finding how much apears " << matrix[0][0]
              << " in matrix " << N << "x" << M << std::endl << "In range from: " << K << " to" 
              << L << std::endl << std::endl << "Processing...";

    int count = 0;
    for (int i = K; i < L; i++)
        count += std::count(matrix[i].begin(), matrix[i].end(), matrix[0][0]);

    file << std::endl << std::endl <<"Element "<<matrix[0][0]<<" aperas " << count << "times"<<std::endl;
    std::cout << std::endl << std::endl << "Element " << matrix[0][0] << " aperas " << count
              << " times" << std::endl << "Press any key to exit, press [1] to change sec atr.";
              
    int choise;
    std::cin >> choise;
    if (choise == 1)
    {
        mythreadid = SetNamedSecurityInfoA(
            LPSTR("text.txt"),
            SE_FILE_OBJECT,
            DACL_SECURITY_INFORMATION,
            NULL,
            NULL,
            pACL,
            NULL
        );
        if (mythreadid == ERROR_SUCCESS)
        {
            std::cout << "SUCCESS";
            CloseHandle(myhandle);
            _getch();
        return 0;
        }
        if (mythreadid != ERROR_ACCESS_DENIED)
         {   
            std::cout << "ERROR";
            CloseHandle(myhandle);
            _getch();
            return 0;
        }
    }
    
    CloseHandle(myhandle);
    _getch();
    return 0;
}