#include <stdio.h> 
#include <windows.h> 

CRITICAL_SECTION cs;
int nextId = 1;

DWORD WINAPI print(LPVOID lpParam)
{
	char c = *(char *)lpParam;
	for (int i = 0; i < 1000; i++)
	{
		while (nextId != c)
			Sleep(1);
		EnterCriticalSection(&cs);
		printf("%d", c);
		nextId = c % 3 + 1;
		LeaveCriticalSection(&cs);
	}
	return 0;
}

void main(void)
{
	DWORD print1ID, print2ID, print3ID;
	HANDLE hPrint1, hPrint2, hPrint3;
	int c1 = 1;
	int c2 = 2;
	int c3 = 3;

	InitializeCriticalSection(&cs);

	hPrint1 = CreateThread(NULL, 0, &print, &c1, 0, &print1ID);
	hPrint2 = CreateThread(NULL, 0, &print, &c2, 0, &print2ID);
	hPrint3 = CreateThread(NULL, 0, &print, &c3, 0, &print3ID);
	int liI = getchar();
	CloseHandle(hPrint1);
	CloseHandle(hPrint2);
	CloseHandle(hPrint3);

	DeleteCriticalSection(&cs);

}