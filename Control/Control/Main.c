#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include "ControlUse.h"

DWORD WINAPI MyThreadFunction(LPVOID lpParam);

// estrutura de dados para controlar as threads // Template
typedef struct {
	int start;
	int end;
	int *count;
	HANDLE *mutex;
} TDados;

int _tmain(int argc, TCHAR * argv[]) {

	// memória partilhada
	HANDLE hMapFile;
	LPTSTR pBuf = NULL;

	TCHAR *msg = NULL;

	TCHAR szMsg[BUF_SIZE];

	// limites
	int maxPlane, maxAero, tipoErro;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	//#####Limites Registry#####//

	if ((maxAero = readAeroLimits()) < 0) {
		maxAero = MAX_AERO;

		if ((tipoErro = createAeroLimits(MAX_AERO)) == 0)
		{
			_tprintf(TEXT("\nDefinição do limite de Aeroportos foi guardada no Sistema.\n"));
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
			_tprintf(TEXT("\nDefinição do limite de Aviões foi guardada no Sistema.\n"));
		}
		else
		{
			_tprintf(TEXT("\nErro crítico! - ERRO 2 - Tipo %d\n"), tipoErro);

			return -1;
		}
	}

	//#####----------------#####//

	//#######Tratamento de Argumentos#######//

	if (argv[1] != NULL)
	{
		if ((tipoErro = createAeroLimits(_tstoi(argv[1]))) == 0)
		{
			_tprintf(TEXT("\nDefinição do limite de Aeroportos foi guardada no Sistema.\n"));

			maxAero = _tstoi(argv[1]);
		}
		else
		{
			_tprintf(TEXT("\nErro crítico! - ERRO 1 - Tipo %d\nUsar valores de Sistema.\n"), tipoErro);

			if ((maxAero = readAeroLimits()) < 0) {
				maxAero = MAX_AERO;

				if ((tipoErro = createAeroLimits(MAX_AERO)) == 0)
				{
					_tprintf(TEXT("\nDefinição do limite de Aeroportos foi guardada no Sistema.\n"));
				}
				else
				{
					_tprintf(TEXT("\nErro crítico! - ERRO 1 - Tipo %d\n"), tipoErro);

					return -1;
				}
			}
		}
	}

	if (argv[2] != NULL)
	{
		if ((tipoErro = createPlaneLimits(_tstoi(argv[2]))) == 0)
		{
			_tprintf(TEXT("\nDefinição do limite de Aviões foi guardada no Sistema.\n"));

			maxPlane = _tstoi(argv[2]);
		}
		else
		{
			_tprintf(TEXT("\nErro crítico! - ERRO 2 - Tipo %d\nUsar valores de Sistema.\n"), tipoErro);

			if ((maxPlane = readPlaneLimits()) < 0) {
				maxPlane = MAX_PLANES;

				if ((tipoErro = createPlaneLimits(MAX_PLANES)) == 0)
				{
					_tprintf(TEXT("\nDefinição do limite de Aviões foi guardada no Sistema.\n"));
				}
				else
				{
					_tprintf(TEXT("\nErro crítico! - ERRO 2 - Tipo %d\n"), tipoErro);

					return -1;
				}
			}
		}
	}

	//#######------------------------#######//

	_tprintf(TEXT("\nLimite máximo de aeroportos: %d\nLimite máximo de aviões: %d\n"), maxAero, maxPlane);

	//#############Memória Partilhada#############//

	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		BUF_SIZE,                // maximum object size (low-order DWORD)
		TEXT("CentralMemory"));                 // name of mapping object


	if (hMapFile == NULL)
	{
		_tprintf(TEXT("Could not create file mapping object (%d).\n"),
			GetLastError());
		return -3;
	}
	else if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		_tprintf(TEXT("Já existe uma execução em curso.\nVou encerrar!\n"));

		CloseHandle(hMapFile);

		_gettch();

		return -2;
	}

	pBuf = (LPTSTR)MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
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

	/*if ((pBuf = startMemory(&hMapFile, TEXT("CentralMemory"), BUF_SIZE)) == NULL) {
		_tprintf(TEXT("\nErro ao iniciar a memória partilhada!\n"));

		CloseHandle(hMapFile);
		return -1;
	}*/

	//CopyMemory((PVOID)pBuf, msg, (_tcslen(msg) * sizeof(TCHAR)));

	_tprintf(TEXT("\nMemória Partilhada criada com sucesso.\n"));

	//#############------------------#############//

	_gettch();

	UnmapViewOfFile(pBuf);

	CloseHandle(hMapFile);

	return 0;
}



DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	TDados * pDataArray;
	DWORD dwWaitResult;
	BOOLEAN continuar = TRUE;

	pDataArray = (TDados*)lpParam;

	for (int i = pDataArray->start; i < pDataArray->end; i++)
	{


		if (!(i % 3))
		{

			while (1)
			{
				dwWaitResult = WaitForSingleObject(
					(*pDataArray->mutex),    // handle to mutex
					INFINITE);  // no time-out interval

				switch (dwWaitResult)
				{
					// The thread got ownership of the mutex
				case WAIT_OBJECT_0:
					(*pDataArray->count)++;
					ReleaseMutex((*pDataArray->mutex));
					break;

				case WAIT_ABANDONED:
					continue;

				default:
					continue;
				}
			}

		}

	}

	return 0;
}


