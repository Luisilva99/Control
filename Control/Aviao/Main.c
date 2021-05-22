#include "AviaoUse.h"



int _tmain(int argc, TCHAR * argv[]) {

	// Memória Partilhada
	HANDLE hMapFile;

	// Sincronização
	HANDLE semaphoreGate;
	HANDLE mutexEntry;
	TCHAR eventName[TAM];


	// Limites e Erros
	int valorArgumento;


	// Threads
	HANDLE hThread[3];
	DWORD dwThread[3];


	// Dados do Aviao
	PlaneData aviao;
	SharedBuffer* pShared;
	SharedBuffer shared;


#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	//#####Inicialização de Dados#####//

	ZeroMemory(aviao.partida, sizeof(TCHAR) * TAM);

	ZeroMemory(aviao.destino, sizeof(TCHAR) * TAM);

	aviao.curPass = 0;

	aviao.id = (int)GetCurrentProcessId();

	aviao.next_X = -1;

	aviao.next_Y = -1;

	aviao.final_X = -1;

	aviao.final_Y = -1;

	for (int i = 0; i < MAX_PASS; i++)
	{
		ZeroMemory(&aviao.pass[i], sizeof(Passag));
	}

	aviao.X = -1;

	aviao.Y = -1;

	aviao.voar = 0;

	_stprintf_s(eventName, TAM, TEXT("%d"), aviao.id);

	//#####----------------------#####//

	//#######Tratamento de Argumentos#######//

	if (argc != 4) {
		_tprintf(TEXT("\nNúmero de argumentos introduzidos incorreto!\nEncerrar Aviao.\n"));

		_gettch();

		return -1;
	}

	if (argv[1] != NULL)
	{
		valorArgumento = _tstoi(argv[1]);

		if (valorArgumento > MAX_PASS)
		{
			valorArgumento = MAX_PASS;

			_tprintf(TEXT("\nValor máximo de lotação do sistema foi ultrapassado!\nLotação do Avião: %d\n"), MAX_PASS);
		}

		aviao.maxPass = valorArgumento;
	}

	if (argv[2] != NULL)
	{
		valorArgumento = _tstoi(argv[2]);

		if (valorArgumento > MAX_VELO)
		{
			valorArgumento = MAX_VELO;

			_tprintf(TEXT("\nValor máximo de velocidade do sistema foi ultrapassado!\nVelocidade do Avião: %d\n"), MAX_VELO);
		}

		aviao.velocidade = valorArgumento;
	}

	if (argv[3] != NULL)
	{
		_stprintf_s(aviao.partida, TAM, TEXT("%s"), argv[3]);

		_tprintf(TEXT("\nAeroporto de inicialização: %s\n"), aviao.partida);
	}

	//#######------------------------#######//

	//#############Memória Partilhada#############//

	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		TEXT("CentralMemory"));


	if (hMapFile == NULL)
	{
		_tprintf(TEXT("Could not open file mapping object (%d).\n"), GetLastError());

		_gettch();

		return -2;
	}

	pShared = (SharedBuffer *)MapViewOfFile(hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		TAM_SHARED);

	if (pShared == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"), GetLastError());

		CloseHandle(hMapFile);

		_gettch();

		return -3;
	}

	CopyMemory(&shared, pShared, TAM_SHARED);

	aviao.buffer = pShared;

	_tprintf(TEXT("\nMemória Partilhada aberta com sucesso.\n"));

	//#############------------------#############//

	//###Verificações Iniciais###//

	if (!aviaoAeroVerify(aviao))
	{
		_tprintf(TEXT("\nAeroporto introduzido não existe no Sistema do Control.\nEncerrar Aviao %d\n"), aviao.id);

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -4;
	}

	//###---------------------###//

	//#####Sincronismos#####//

	mutexEntry = OpenMutex(MUTEX_ALL_ACCESS, FALSE, CONTROL_MUTEX_ENTRY);

	if (mutexEntry == NULL)
	{
		_tprintf(TEXT("\nErro ao abrir o mutex de sincronização!\nErro %d\n"), GetLastError());

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -5;
	}

	if (WaitForSingleObject(mutexEntry, 0) == WAIT_TIMEOUT)
	{
		_tprintf(TEXT("\nErro ao entrar no Sistema Control.\nSistema não se encontra atualmente a aceitar Aviões!\n"));

		CloseHandle(mutexEntry);

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -6;
	}

	ReleaseMutex(mutexEntry);

	semaphoreGate = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, CONTROL_SEMAPHORE_ENTRY);

	if (semaphoreGate == NULL)
	{
		_tprintf(TEXT("\nErro ao abrir o semaforo de sincronização!\nErro %d\n"), GetLastError());

		CloseHandle(mutexEntry);

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -7;
	}

	if (WaitForSingleObject(semaphoreGate, 0) == WAIT_TIMEOUT)
	{
		_tprintf(TEXT("\nErro ao entrar no Sistema Control.\nSistema encontra-se lotado!\n"));

		CloseHandle(mutexEntry);

		CloseHandle(semaphoreGate);

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -8;
	}

	_tprintf(TEXT("\nEntrada com sucesso em Control!\n"));

	aviao.mutexMoveSync = OpenMutex(
		MUTEX_ALL_ACCESS,
		FALSE,
		PLANE_MOVE_SYNC
	);

	if (aviao.mutexMoveSync == NULL)
	{
		_tprintf(TEXT("\nAbertura do mutex de sincronização de movimentação dos Aviões não foi criado com sucesso!\nErro %d\n"), GetLastError());

		CloseHandle(mutexEntry);

		CloseHandle(semaphoreGate);

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -9;
	}

	_tprintf(TEXT("\nAbertura do mutex de sincronização de movimentação dos Aviões foi criado com sucesso!\n"));// DEBUG

	aviao.eventMoveTrigger = CreateEvent(
		NULL,
		FALSE,
		FALSE,
		eventName
	);

	if (aviao.eventMoveTrigger == NULL)
	{
		_tprintf(TEXT("\nCriação do evento de iniciação de movimentação dos Aviões não foi criado com sucesso!\nErro %d\n"), GetLastError());

		CloseHandle(aviao.mutexMoveSync);

		CloseHandle(mutexEntry);

		CloseHandle(semaphoreGate);

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -10;
	}

	_tprintf(TEXT("\nCriação do evento de iniciação de movimentação dos Aviões foi criado com sucesso!\n"));

	aviao.writeBuffer = OpenMutex(
		MUTEX_ALL_ACCESS,
		FALSE,
		PLANE_MUTEX_WRITER
	);

	if (aviao.writeBuffer == NULL)
	{
		_tprintf(TEXT("\nAbertura do mutex de sincronização de escrita para o Buffer Circular não foi criado com sucesso!\nErro %d\n"), GetLastError());

		CloseHandle(aviao.eventMoveTrigger);

		CloseHandle(aviao.mutexMoveSync);

		CloseHandle(mutexEntry);

		CloseHandle(semaphoreGate);

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -11;
	}

	_tprintf(TEXT("\nAbertura do mutex de sincronização de escrita para o Buffer Circular foi criado com sucesso!\n"));// DEBUG

	aviao.readBuffer = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, CONTROL_SEMAPHORE_BUFFER_READER);

	if (aviao.readBuffer == NULL)
	{
		_tprintf(TEXT("\nErro ao abrir o semaforo de sincronização do Buffer Circular!\nErro %d\n"), GetLastError());

		CloseHandle(aviao.writeBuffer);

		CloseHandle(aviao.eventMoveTrigger);

		CloseHandle(aviao.mutexMoveSync);

		CloseHandle(mutexEntry);

		CloseHandle(semaphoreGate);

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -12;
	}

	//#####------------#####//

	_tprintf(TEXT("\nInserir este avião no sítio certo.\n\n"));//DEBUG / Necessita de ir para o buffer circular

	if (!insertAviaoTemporary(&aviao))//DEBUG / Temporariamente desta forma
	{
		_tprintf(TEXT("\nErro ao inserir aviao %d na memória partilhada.\n"), aviao.id);

		CloseHandle(aviao.readBuffer);

		CloseHandle(aviao.writeBuffer);

		CloseHandle(aviao.eventMoveTrigger);

		CloseHandle(aviao.mutexMoveSync);

		CloseHandle(mutexEntry);

		if (ReleaseSemaphore(semaphoreGate, 1, NULL) == 0)
		{
			_tprintf(TEXT("\nLibertação do semaforo não foi um sucesso!\nError: %d\n"), GetLastError());
		}
		else
		{
			CloseHandle(semaphoreGate);
		}

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -13;
	}

	//#####Threads#####//

	hThread[0] = CreateThread(
		NULL,
		0,
		tratamentoDeComandos,
		(LPVOID)&aviao,
		0,
		&dwThread[0]
	);

	if (hThread[0] == NULL)
	{
		_tprintf(TEXT("CreateThread failed, GLE=%d.\n"), GetLastError());

		CloseHandle(aviao.readBuffer);

		CloseHandle(aviao.writeBuffer);

		CloseHandle(aviao.eventMoveTrigger);

		CloseHandle(aviao.mutexMoveSync);

		CloseHandle(mutexEntry);

		if (ReleaseSemaphore(semaphoreGate, 1, NULL) == 0)
		{
			_tprintf(TEXT("\nLibertação do semaforo não foi um sucesso!\nError: %d\n"), GetLastError());
		}
		else
		{
			CloseHandle(semaphoreGate);
		}

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -14;
	}

	_tprintf(TEXT("\nCriação de Thread %d da linha de comandos foi um sucesso!\n"), dwThread[0]);

	hThread[1] = CreateThread(
		NULL,
		0,
		movePlane,
		(LPVOID)&aviao,
		0,
		&dwThread[1]
	);

	if (hThread[1] == NULL)
	{
		_tprintf(TEXT("CreateMoveThread failed, GLE=%d.\n"), GetLastError());

		CloseHandle(hThread[0]);

		CloseHandle(aviao.readBuffer);

		CloseHandle(aviao.writeBuffer);

		CloseHandle(aviao.eventMoveTrigger);

		CloseHandle(aviao.mutexMoveSync);

		CloseHandle(mutexEntry);

		if (ReleaseSemaphore(semaphoreGate, 1, NULL) == 0)
		{
			_tprintf(TEXT("\nLibertação do semaforo não foi um sucesso!\nError: %d\n"), GetLastError());
		}
		else
		{
			CloseHandle(semaphoreGate);
		}

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -15;
	}

	_tprintf(TEXT("\nCriação de Thread %d do tratamento dos movimentos do Avião foi um sucesso!\n"), dwThread[1]);

	hThread[2] = OpenEvent(
		EVENT_ALL_ACCESS,
		FALSE,
		KILLER_TRIGGER
	);

	if (hThread[2] == NULL)
	{
		_tprintf(TEXT("\nCriação do evento de Shutdown foi criado com sucesso!\nErro %d\n"), GetLastError());

		CloseHandle(hThread[1]);

		CloseHandle(hThread[0]);

		CloseHandle(aviao.readBuffer);

		CloseHandle(aviao.writeBuffer);

		CloseHandle(aviao.eventMoveTrigger);

		CloseHandle(aviao.mutexMoveSync);

		CloseHandle(mutexEntry);

		if (ReleaseSemaphore(semaphoreGate, 1, NULL) == 0)
		{
			_tprintf(TEXT("\nLibertação do semaforo não foi um sucesso!\nError: %d\n"), GetLastError());
		}
		else
		{
			CloseHandle(semaphoreGate);
		}

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -16;
	}

	_tprintf(TEXT("\nCriação do evento Shutdown foi criado com sucesso!\n"));

	//######----------------------######//

	WaitForMultipleObjects(3, hThread, FALSE, INFINITE);

	//-----------------//

	for (int i = 0; i < 3; i++)
	{
		CloseHandle(hThread[i]);
	}

	CloseHandle(aviao.readBuffer);

	CloseHandle(aviao.writeBuffer);

	CloseHandle(aviao.eventMoveTrigger);

	CloseHandle(aviao.mutexMoveSync);

	CloseHandle(mutexEntry);

	if (ReleaseSemaphore(semaphoreGate, 1, NULL) == 0)
	{
		_tprintf(TEXT("\nLibertação do semaforo não foi um sucesso!\nError: %d\n"), GetLastError());
	}
	else
	{
		CloseHandle(semaphoreGate);
	}

	UnmapViewOfFile(pShared);

	CloseHandle(hMapFile);

	return 0;
}