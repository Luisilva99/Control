#include "AviaoUse.h"



int _tmain(int argc, TCHAR * argv[]) {

	// Memória Partilhada
	HANDLE hMapFile;
	LPTSTR pBuf = NULL;

	// Sincronização
	HANDLE semaphoreGate;


	// Limites e Erros
	int maxPlane, maxAero, tipoErro, valorArgumento;


	// Threads
	HANDLE hThread;
	DWORD dwThread;

	/*
	// Dados do Control
	ControlData control;
	SharedBuffer* pShared;
	SharedBuffer shared;
	*/

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	//#############Memória Partilhada#############//

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

	_tprintf(TEXT("\nMemória Partilhada aberta com sucesso.\n"));

	//#############------------------#############//

	//#####Sincronismos#####//

	semaphoreGate = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, CONTROL_SEMAPHORE_ENTRY);

	if (semaphoreGate == NULL)
	{
		_tprintf(TEXT("\nErro ao abrir o semaforo de sincronização!\nErro %d\n"), GetLastError());
	}

	//#####Limites Registry#####//

	

	if ((maxAero = readAeroLimits()) < 0) {
		maxAero = MAX_AERO;

		if ((tipoErro = createAeroLimits(MAX_AERO)) == 0)
		{
			_tprintf(TEXT("\nDefinição do limite de Aeroportos definido no programa foi guardado no Sistema.\n"));
		}
		else
		{
			_tprintf(TEXT("\nErro crítico! - ERRO 1 - Tipo %d\n"), tipoErro);

			return -1;
		}
	}

	if ((maxPlane = readPlaneLimits()) < 0) {
		maxPlane = MAX_PLANES;

		if ((tipoErro = createPlaneLimits(MAX_PLANES)) == 0)
		{
			_tprintf(TEXT("\nDefinição do limite de Aviões definido no programa foi guardado no Sistema.\n"));
		}
		else
		{
			_tprintf(TEXT("\nErro crítico! - ERRO 2 - Tipo %d\n"), tipoErro);

			return -1;
		}
	}




	//#####------------#####//	


	//##Entrada no Control##//

	// Mutex de flag de aceitação no sistema
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
		_tprintf(TEXT("\nLibertação do semaforo não foi um sucesso!\n"));
	}

	return 0;
}