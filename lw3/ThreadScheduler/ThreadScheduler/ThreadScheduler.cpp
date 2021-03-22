#include <windows.h>
#include <string>
#include <iostream>
#include <string>
#include <fstream>
#include <clocale>
#include <chrono>

using namespace std;

const int THREAD_COUNT = 2;
const int OPERATION_COUNT = 10;
const string INVALID_ARGS = "Invalid arguments count. Usage: threadschedulaer.exe <file>";

struct DataForProc
{
    int threadNumber;
    int time;
    string fileName;
};

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
    DataForProc* lpData = static_cast<DataForProc*>(lpParam);
    int threadNumber = lpData->threadNumber;
    int startTime = lpData->time;
    ofstream file;
    file.open(lpData->fileName);

    for (size_t i = 0; i < OPERATION_COUNT; i++)
    {
        int operationStartTime = clock();
        unsigned long resultTime = startTime - operationStartTime;
        file << threadNumber << "|" << resultTime << endl;
    }

    /*std::string outStr = "Поток №" + std::to_string(int(lpParam)) + " выполняет свою работy\n";
    std::cout << outStr;*/
    ExitThread(0); // функция устанавливает код завершения потока в 0
}

int main(int argc, char* argv[])
{
    //unsigned long startTime = GetTickCount64();
    int startTime = clock();
    setlocale(LC_ALL, "Russian");

    /*if (argc != 2)
    {
        cout << INVALID_ARGS << endl;
        return 1;
    }*/
    string file = "input.txt";
    //ofstream file;

    //file.open(argv[1]);

    // создание  потоков
    HANDLE* handles = new HANDLE[THREAD_COUNT];
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        DataForProc* data = new DataForProc{i, startTime, file};
        /*data->threadNumber = i;
        data->time = startTime;
        data->fileName = file;*/
        handles[i] = CreateThread(NULL, 0, &ThreadProc, data, CREATE_SUSPENDED, NULL);
    }

    // запуск потоков
    for (size_t i = 0; i < THREAD_COUNT; i++)
    {
        ResumeThread(handles[i]);
    }

    // ожидание окончания работы потоков
    WaitForMultipleObjects(THREAD_COUNT, handles, true, INFINITE);

    return 0;
}