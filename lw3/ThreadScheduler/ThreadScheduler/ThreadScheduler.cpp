#include <windows.h>
#include <string>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>

using namespace std;

const int THREAD_COUNT = 2;
const int OPERATION_COUNT = 1000000;
const int CYCLE_COUNT = 20;
const string INVALID_ARGS = "Invalid arguments count. Usage: ThreadScheduler.exe <fileForFirstThread> <fileForSecondThread>";

struct DataForProc
{
    int threadNumber;
    clock_t time;
    string fileName;
};

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
    DataForProc* lpData = static_cast<DataForProc*>(lpParam);
    int threadNumber = lpData->threadNumber + 1;
    clock_t startTime = lpData->time;
    ofstream file;
    file.open(lpData->fileName);
    for (size_t i = 0; i < CYCLE_COUNT; i++)
    {
        for (size_t j = 0; j < OPERATION_COUNT; j++)
        {
            int operation = j;
        }
        clock_t operationStartTime = clock();
        clock_t resultTime = operationStartTime - startTime;
        std::string outStr = to_string(threadNumber) + "|" + to_string(resultTime) + "\n";
        file << outStr;
    }
    ExitThread(0); // функция устанавливает код завершения потока в 0
}

int main(int argc, char* argv[])
{
    auto startTime = clock();
    setlocale(LC_ALL, "Russian");

    if (argc != 3)
    {
        cout << INVALID_ARGS << endl;
        return 1;
    }
    vector<string> files;
    files.push_back(argv[1]);
    files.push_back(argv[2]);

    // создание  потоков
    HANDLE* handles = new HANDLE[THREAD_COUNT];
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        DataForProc* data = new DataForProc{i, startTime, files[i]};
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