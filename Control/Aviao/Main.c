#include "AviaoUse.h"



int _tmain(int argc, TCHAR * argv[]) {

	// Memória Partilhada
	HANDLE hMapFile;
	LPTSTR pBuf = NULL;

	// Sincronização
	HANDLE semaphoreGate;


	// Limites e Erros
	int valorArgumento;


	// Threads
	HANDLE hThread;
	DWORD dwThread;


	// Dados do Aviao
	PlaneData aviao;
	SharedBuffer* pShared;
	SharedBuffer shared;


#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	//#######Tratamento de Argumentos#######//

	_tprintf(TEXT("\nNúmero de Argumentos: %d\n"), argc);//DEBUG

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

	if (argv[2] != NULL && argc > 1)
	{
		valorArgumento = _tstoi(argv[2]);

		if (valorArgumento > MAX_VELO)
		{
			valorArgumento = MAX_VELO;

			_tprintf(TEXT("\nValor máximo de velocidade do sistema foi ultrapassado!\nVelocidade do Avião: %d\n"), MAX_PASS);
		}

		aviao.velocidade = valorArgumento;
	}

	if (argv[3] != NULL && argc > 2)
	{
		_stprintf_s(aviao.partida, TAM, TEXT("%s"), argv[3]);

		_tprintf(TEXT("\nAeroporto inicial: %s\n"), aviao.partida);//DEBUG
	}

	//#######------------------------#######//

	//#####Inicialização de Dados#####//

	ZeroMemory(aviao.destino, sizeof(TCHAR) * TAM_INPUT);

	aviao.curPass = 0;

	aviao.id = (int) GetCurrentProcessId();

	aviao.next_X = -1;

	aviao.next_Y = -1;

	for (int i = 0; i < MAX_PASS; i++)
	{
		ZeroMemory(&aviao.pass[i], sizeof(Passag));
	}

	aviao.X = -1;

	aviao.Y = -1;

	//#####----------------------#####//

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

	pShared = (SharedBuffer *)MapViewOfFile(hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		TAM_SHARED);

	if (pShared == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"),
			GetLastError());

		CloseHandle(hMapFile);

		return -1;
	}

	CopyMemory(&shared, pShared, TAM_SHARED);

	aviao.buffer = pShared;

	_tprintf(TEXT("\nMemória Partilhada aberta com sucesso.\n"));

	//#############------------------#############//

	//#####Sincronismos#####//

	semaphoreGate = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, CONTROL_SEMAPHORE_ENTRY);

	if (semaphoreGate == NULL)
	{
		_tprintf(TEXT("\nErro ao abrir o semaforo de sincronização!\nErro %d\n"), GetLastError());
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

	_tprintf(TEXT("\nO que se encontra escrito na memória partilhada (teste):\n\n"));

	_tprintf(TEXT("\n%s\n"), aviao.buffer->msg.controlResponse);

	_gettch();

	if (ReleaseSemaphore(semaphoreGate, 1, NULL) == 0)
	{
		_tprintf(TEXT("\nLibertação do semaforo não foi um sucesso!\n"));
	}

	UnmapViewOfFile(pShared);

	CloseHandle(hMapFile);

	return 0;
}