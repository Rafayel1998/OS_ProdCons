#include "stdafx.h"

ProdCons prodCons;
#define PATIENCE 5000       //this show how long main thread needs to wait for Producers, and Consumers
#define TIREDNESS 100       //the time Producers and Consumers need to sleep
#define MUCHTIREDNESS 1500  //this shows how long to pause before begining Producer and Consumer 

//this is meant to differ Producer, Consumer and _tmain visually
void SetConsoleColor(char color)
{
    HANDLE ConsoleHandle = CreateFile(_T("CONOUT$"),
                                      GENERIC_WRITE,
                                      0, NULL,
                                      OPEN_EXISTING,
                                      0, NULL);
    if (ConsoleHandle != INVALID_HANDLE_VALUE)
        SetConsoleTextAttribute(ConsoleHandle, color);
}

DWORD WINAPI Producer(void * num)
{
    HANDLE m = OpenMutex(MUTEX_ALL_ACCESS, FALSE, M);
    if (NULL == m) { _tprintf(_T("Something went wrong - %d"), GetLastError()); return -1; }
    HANDLE consoleColor = OpenMutex(MUTEX_ALL_ACCESS, FALSE, CONSOLE_COLOR);
    if (NULL == consoleColor) { _tprintf(_T("Something went wrong - %d"), GetLastError()); return -1; }
    HANDLE full = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, FULL);
    if (NULL == full) { _tprintf(_T("Something went wrong - %d"), GetLastError()); return -1; }
    HANDLE empty = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, EMPTY);
    if (NULL == empty) { _tprintf(_T("Something went wrong - %d"), GetLastError()); return -1; }

    Product item;
    int* n = static_cast<int*>(num);
    while (true)
    {
        item = prodCons.Porduce();
        WaitForSingleObject(empty, INFINITE);
        WaitForSingleObject(m, INFINITE);
        WaitForSingleObject(consoleColor, INFINITE);
        SetConsoleColor(2);
        prodCons.PutIntoStorage(item, *n);
        ReleaseMutex(consoleColor);
        ReleaseMutex(m);
        ReleaseSemaphore(full, 1, NULL);
        Sleep(TIREDNESS);
    }
    return 0;
}

DWORD WINAPI Consumer(void * num)
{
    HANDLE m = OpenMutex(MUTEX_ALL_ACCESS, FALSE, M);
    if (NULL == m) { _tprintf(_T("Something went wrong - %d"), GetLastError()); return -1; }
    HANDLE consoleColor = OpenMutex(MUTEX_ALL_ACCESS, FALSE, CONSOLE_COLOR);
    if (NULL == consoleColor) { _tprintf(_T("Something went wrong - %d"), GetLastError()); return -1; }
    HANDLE full = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, FULL);
    if (NULL == full) { _tprintf(_T("Something went wrong - %d"), GetLastError()); return -1; }
    HANDLE empty = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, EMPTY);
    if (NULL == empty) { _tprintf(_T("Something went wrong - %d"), GetLastError()); return -1; }
    
    Product item;
    int* n = static_cast<int *>(num);
    int index;
    while (true)
    {
        WaitForSingleObject(full, INFINITE);
        WaitForSingleObject(m, INFINITE);
        item = prodCons.RemoveFromStorage();
        index = prodCons.GetIndex();
        ReleaseMutex(m);
        ReleaseSemaphore(empty, 1, NULL);
        WaitForSingleObject(consoleColor, INFINITE);
        SetConsoleColor(4);
        prodCons.Consume(item, *n, index);
        ReleaseMutex(consoleColor);
        Sleep(TIREDNESS);
    }
    return 0;
}

int _tmain()
{
    HANDLE m = CreateMutex(NULL, FALSE, M);
    if (NULL == m) { _tprintf(_T("Something went wrong - %d"), GetLastError()); return -1; }
    HANDLE consoleColor = CreateMutex(NULL, FALSE, CONSOLE_COLOR);
    if (NULL == consoleColor) { _tprintf(_T("Something went wrong - %d"), GetLastError()); return -1; }
    HANDLE full = CreateSemaphore(NULL, 0, STORAGE_CAPACITY, FULL);
    if (NULL == full) { _tprintf(_T("Something went wrong - %d"), GetLastError()); return -1; }
    HANDLE empty = CreateSemaphore(NULL, STORAGE_CAPACITY, STORAGE_CAPACITY, EMPTY);
    if (NULL == empty) { _tprintf(_T("Something went wrong - %d"), GetLastError()); return -1; }

    srand(time(0));
    SetConsoleColor(3);
    _tprintf(_T("----Starting Producer-Consumer Session----\n"));
    Sleep(MUCHTIREDNESS);
    int i;
    HANDLE hThreads[PRODCNT + CONSCNT];
    DWORD IDs[PRODCNT + CONSCNT];
    for (i = 0; i < PRODCNT; i++)
    {
        int * k = new int;
        *k = i;
        hThreads[i] = CreateThread(NULL, NULL, Producer, k, NULL, &IDs[i]);
        if (NULL == hThreads[i])
        {
            _tprintf(_T("Producer %d is ill, because of %d"), i, GetLastError());
        }
    }
    Sleep(2 * MUCHTIREDNESS);
    for (; i < PRODCNT + CONSCNT; i++)
    {
        int * k = new int;
        *k = i - PRODCNT;
        hThreads[i] = CreateThread(NULL, NULL, Consumer, k, NULL, &IDs[i]);
        if (NULL == hThreads[i])
        {
            _tprintf(_T("Consumer %d is ill, because of %d"), i - PRODCNT, GetLastError());
        }
    }
    WaitForMultipleObjects(PRODCNT + CONSCNT, hThreads, TRUE, PATIENCE);
    for (i = 0; i < PRODCNT + CONSCNT; i++)
    {
        CloseHandle(hThreads[i]);
    }
    WaitForSingleObject(m, INFINITE);           //taking the mutex so that Producer and Consumer can no longer work
    WaitForSingleObject(consoleColor, INFINITE);
    SetConsoleColor(3);
    _tprintf(_T("----Ending Producer-Consumer Session----\n"));
    ReleaseMutex(consoleColor);
    system("Pause");
    return 0;
}