#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include "ControlUse.h"



int _tmain(int argc, TCHAR * argv[]) {

	// Memória Partilhada
	HANDLE hMapFile;
	LPTSTR pBuf = NULL;
	LPTSTR * pPlaneBuf;

	// Limites e Erros
	int maxPlane, maxAero, tipoErro;

	// Sincroniza��o
	HANDLE semaphoreGate;

	// Threads
	HANDLE hThread;
	DWORD dwThread;

	// Dados do Control
	ControlData control;

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

	_tprintf(TEXT("\nNúmero de Argumentos: %d\n"), argc);

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

	if (argv[2] != NULL && argc > 1)
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

	_tprintf(TEXT("\nLimite máximo de aeroportos: %d\nLimite máximo de aviões: %d\n"), maxAero, maxPlane);// DEBUG

	//###########Inicializa��o Padr�o dos Dados do Control###########//

	control.map = malloc(maxAero * BUF_MAP);
	control.curAero = 0;
	control.maxAero = maxAero;
	control.planes = malloc(maxPlane * BUF_PLANE);
	control.curPlane = 0;
	control.maxPlane = maxPlane;
	control.planeViews = malloc(maxPlane * sizeof(HANDLE));
	control.tCircBuffer.locReadAtual = 0;
	control.tCircBuffer.locWriteAtual = 0;

	for (int i = 0; i < TAM_BUFF; i++)
	{
		ZeroMemory(&control.tCircBuffer.buffer[i], sizeof(TCHAR) * TAM_INPUT);
	}

	pPlaneBuf = malloc(maxPlane * sizeof(LPTSTR));

	_tprintf(TEXT("\nInicializa��o da memória física do Control foi um Sucesso!\n"));// DEBUG

	//###########-----------------------------------------###########//

	//#############Memória Partilhada#############//

	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		BUF_CIRCULAR + (maxPlane * BUF_PLANE),                // maximum object size (low-order DWORD)
		TEXT("CentralMemory"));                 // name of mapping object


	if (hMapFile == NULL)
	{
		_tprintf(TEXT("Could not create file mapping object (%d).\n"),
			GetLastError());
		return -3;
	}
	else if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		_tprintf(TEXT("Já existe uma execu��o em curso.\nVou encerrar!\n"));

		CloseHandle(hMapFile);

		_gettch();		//programa fica a espera dum input, s� avan�a quando for pressionado uma tecla

		return -2;
	}

	//Primeiro para a memória partilhada//DEBUG
	pBuf = (LPTSTR)MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		BUF_CIRCULAR);

	if (pBuf == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"),
			GetLastError());

		CloseHandle(hMapFile);

		return -1;
	}

	CopyMemory((PVOID)pBuf, &control.tCircBuffer, BUF_CIRCULAR);

	_tprintf(TEXT("\nMemória Partilhada criada com sucesso e copiado o buffer circular.\n"));// DEBUG

	//#############------------------#############//

	//#######Mapeamento das Respostas / Informações dos Aviões#######//

	for (int i = 0; i < maxPlane; i++)
	{
		*(control.planeViews + i) = (LPTSTR)MapViewOfFile(hMapFile,   // handle to map object
			FILE_MAP_ALL_ACCESS, // read/write permission
			0,
			0,
			BUF_PLANE);

		if (*(control.planeViews + i) == NULL)
		{
			_tprintf(TEXT("Could not map view of file (%d).\n"),
				GetLastError());

			CloseHandle(hMapFile);

			return -(100 + i);
		}

		CopyMemory((PVOID)*(control.planeViews + i), (control.planes + i), BUF_PLANE);

		_tprintf(TEXT("\nMemória Partilhada criada com sucesso para um segmento de Avião.\n"));// DEBUG
	}

	//#######-------------------------------------------------#######//

	//#####Sincronismos#####//

	semaphoreGate = CreateSemaphore(
		NULL,
		maxPlane,
		maxPlane,
		CONTROL_SEMAPHORE_ENTRY
	);

	if (semaphoreGate == NULL)
	{
		_tprintf(TEXT("\nCriação do semaforo de entrada de aviões não foi criado com sucesso!\nErro %d\n"), GetLastError());

		return -1;
	}

	_tprintf(TEXT("\nCriação do semaforo de entrada de aviões foi criado com sucesso!\n"));// DEBUG

	//#####------------#####//

	//######Lançamento das Threads######//

	hThread = CreateThread(
		NULL,
		0,
		tratamentoDeComandos,
		(LPVOID)&control,
		0,
		&dwThread
	);

	if (hThread == NULL)
	{
		_tprintf(TEXT("CreateThread failed, GLE=%d.\n"), GetLastError());
		return -8;
	}

	//######----------------------######//

	WaitForSingleObject(hThread, INFINITE);

	_tprintf(TEXT("\nLibertação das Threads criadas!\n"));

	//########Libertação da Memória Alocada########//

	for (int i = 0; i < maxPlane; i++)
	{
		UnmapViewOfFile((pPlaneBuf + i));
	}

	free(pPlaneBuf);

	free(control.map);

	free(control.planes);

	_tprintf(TEXT("\nLibertação da memória física do Control foi um Sucesso!\n"));// DEBUG

	//########-----------------------------########//

	CloseHandle(semaphoreGate);

	UnmapViewOfFile(pBuf);

	CloseHandle(hMapFile);

	return 0;
}
