#include "AviaoUse.h"

int _tmain(int argc, TCHAR * argv[]) {

	// Mem�ria Partilhada
	HANDLE hMapFile;
	LPTSTR pBuf = NULL;

	// Sincroniza��o
	HANDLE semaphoreGate;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	//#############Mem�ria Partilhada#############//

	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		TEXT("CentralMemory"));


	if (hMapFile == NULL)
	{
		_tprintf(TEXT("Could not open file mapping object (%d).\n"),
			GetLastError());
		return -3;
	}

	pBuf = (LPTSTR)MapViewOfFile(hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		BUF_SIZE);

	if (pBuf == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"),
			GetLastError());

		CloseHandle(hMapFile);

		return -1;
	}

	//CopyMemory((PVOID)pBuf, msg, (_tcslen(msg) * sizeof(TCHAR)));

	_tprintf(TEXT("\nMem�ria Partilhada aberta com sucesso.\n"));

	//#############------------------#############//

	//#####Sincronismos#####//

	semaphoreGate = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, CONTROL_SEMAPHORE_ENTRY);

	if (semaphoreGate == NULL)
	{
		_tprintf(TEXT("\nErro ao abrir o semaforo de sincroniza��o!\nErro %d\n"), GetLastError());
	}

	//#####------------#####//

	//##Entrada no Control##//

	// Mutex de flag de aceita��o no sistema
	//-----------------WaitForSingleObject

	// Modificar para ciclo de espera
	WaitForSingleObject(semaphoreGate, INFINITE);

	_tprintf(TEXT("\nEntrada com sucesso em Control!\n"));

	// Mutex Release
	//-------------

	//##------------------##//

	_gettch();

	if (ReleaseSemaphore(semaphoreGate, 1, NULL) == 0)
	{
		_tprintf(TEXT("\nLiberta��o do semaforo n�o foi um sucesso!\n"));
	}

	return 0;
}