#include "AviaoUse.h"



int _tmain(int argc, TCHAR * argv[]) {

	// Memória Partilhada
	HANDLE hMapFile;
	LPTSTR pBuf = NULL;

	// Sincronização
	HANDLE semaphoreGate;


	// Limites e Erros
	int valorArgumento, errorType = 0;


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

		_tprintf(TEXT("\nAeroporto inicial: %s\n"), aviao.partida);//DEBUG
	}

	//#######------------------------#######//

	//#############Memória Partilhada#############//

	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		TEXT("CentralMemory"));


	if (hMapFile == NULL)
	{
		_tprintf(TEXT("Could not open file mapping object (%d).\n"),
			GetLastError());

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
		_tprintf(TEXT("Could not map view of file (%d).\n"),
			GetLastError());

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

	semaphoreGate = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, CONTROL_SEMAPHORE_ENTRY);

	if (semaphoreGate == NULL)
	{
		_tprintf(TEXT("\nErro ao abrir o semaforo de sincronização!\nErro %d\n"), GetLastError());

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -5;
	}

	//#####------------#####//


	//##Entrada no Control##//

	// Mutex de flag de aceitação no sistema / Escrita no Sistema
	//-----------------WaitForSingleObject

	// Modificar para ciclo de espera
	WaitForSingleObject(semaphoreGate, INFINITE);

	_tprintf(TEXT("\nEntrada com sucesso em Control!\n"));

	// Mutex Release
	//-------------

	//##------------------##//

	_gettch();

	_tprintf(TEXT("\nInserir este avião no sítio certo.\n\n"));//DEBUG / Necessita de ir para o buffer circular

	if (!insertAviaoTemporary(&aviao))
	{
		_tprintf(TEXT("\nErro ao inserir DEBUG aviao na memória partilhada.\n"));

		if (ReleaseSemaphore(semaphoreGate, 1, NULL) == 0)
		{
			_tprintf(TEXT("\nLibertação do semaforo não foi um sucesso!\nError: %d\n"), GetLastError());
		}

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -6;
	}

	_gettch();

	//#####Threads#####//

	hThread = CreateThread(
		NULL,
		0,
		tratamentoDeComandos,
		(LPVOID)&aviao,
		0,
		&dwThread
	);

	if (hThread == NULL)
	{
		_tprintf(TEXT("CreateThread failed, GLE=%d.\n"), GetLastError());
		return -7;
	}

	//######----------------------######//

	WaitForSingleObject(hThread, INFINITE);

	//-----------------//

	if (ReleaseSemaphore(semaphoreGate, 1, NULL) == 0)
	{
		_tprintf(TEXT("\nLibertação do semaforo não foi um sucesso!\nError: %d\n"), GetLastError());
	}

	UnmapViewOfFile(pShared);

	CloseHandle(hMapFile);

	return 0;
}