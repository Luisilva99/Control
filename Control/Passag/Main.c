#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include "PassagUse.h"



int _tmain(int argc, TCHAR* argv[]) {



	//Variaveis de comunicação dos pipes
	HANDLE hPipe;
	DWORD dwWritten;

	// Threads
	HANDLE hThread[2];
	DWORD dwThread[2];

	// Sincronização
	HANDLE semaphorePassagGate;

	// Dados do Passag
	Passag pass;


#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif


	//#######Tratamento de Argumentos#######//

	if (argc < 4) {
		_tprintf(TEXT("\nNúmero de argumentos introduzidos insuficiente!\nEncerrar Passag %d.\n"), (int)GetCurrentProcessId());

		_gettch();

		return -1;
	}

	if (argv[1] == NULL || argv[2] == NULL || argv[3] == NULL)
	{
		_tprintf(TEXT("\nErro!\t-\tUm dos comandos é NULL.\nEncerrar Passag %d.\n"), (int)GetCurrentProcessId());

		_gettch();

		return -2;
	}

	//aero origem / partida			DEBUG
	_stprintf_s(pass.partida, TAM, TEXT("%s"), argv[1]);

	//aero destino / chegada		DEBUG
	_stprintf_s(pass.destino, TAM, TEXT("%s"), argv[2]);

	//nome							DEBUG
	_stprintf_s(pass.nome, TAM, TEXT("%s"), argv[3]);

	//tempo de espera / opcional!	DEBUG
	if (argv[4] != NULL)
	{
		pass.tempo = _tstoi(argv[4]);
	}
	else
	{
		pass.tempo = INFINITE;
	}

	pass.voar = 0;

	//#######------------------------#######//

	listPassInfo(pass);//DEBUG

	//######Sincronização######//

	semaphorePassagGate = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, CONTROL_SEMAPHORE_PASSAG_ENTRY);

	if (semaphorePassagGate == NULL)
	{
		_tprintf(TEXT("\nErro ao abrir o semaforo de sincronização!\nErro %d\n"), GetLastError());

		//libertação de memória

		_gettch();

		return -3;
	}

	if (WaitForSingleObject(semaphorePassagGate, 0) == WAIT_TIMEOUT)
	{
		_tprintf(TEXT("\nErro ao entrar no Sistema Control.\nSistema encontra-se lotado!\n"));

		//libertação de memória

		_gettch();

		return -4;
	}

	_tprintf(TEXT("\nEntrada com sucesso em Control!\n"));

	//######-------------######//

	//########Verificação de Existencia dos Aeroportos########//



	//########----------------------------------------########//

	//######Threads######//

	hThread[0] = CreateThread(
		NULL,
		0,
		tratamentoDeComandos,
		(LPVOID)&pass,
		0,
		&dwThread[0]
	);

	if (hThread[0] == NULL)
	{
		_tprintf(TEXT("CreateThread de Tratamento de Comandos failed, GLE=%d.\n"), GetLastError());

		ReleaseSemaphore(semaphorePassagGate, 1, 0);

		CloseHandle(semaphorePassagGate);

		_gettch();

		return -6;
	}

	hThread[1] = OpenEvent(
		EVENT_ALL_ACCESS,
		FALSE,
		KILLER_TRIGGER
	);

	if (hThread[1] == NULL)
	{
		_tprintf(TEXT("\nCriação do evento de Shutdown foi criado com sucesso!\nErro %d\n"), GetLastError());

		CloseHandle(hThread[0]);

		ReleaseSemaphore(semaphorePassagGate, 1, 0);

		CloseHandle(semaphorePassagGate);

		_gettch();

		return -7;
	}

	//######-------######//

	WaitForMultipleObjects(2, hThread, FALSE, INFINITE);

	_tprintf(TEXT("\nLibertação da memória do Passag: %d\tNome: %s\n"), (int)GetCurrentProcessId(), pass.nome);

	for (int i = 0; i < 2; i++)
	{
		CloseHandle(hThread[i]);
	}

	ReleaseSemaphore(semaphorePassagGate, 1, 0);

	CloseHandle(semaphorePassagGate);

	_gettch();

	return 0;

	//hPipe = CreateFile(TEXT("\\\\.\\pipe\\Pipe"),
	//	GENERIC_READ | GENERIC_WRITE,
	//	0,
	//	NULL,
	//	OPEN_EXISTING,
	//	0,
	//	NULL);
	//if (hPipe != INVALID_HANDLE_VALUE)
	//{
	//	WriteFile(hPipe,
	//		"Hello Pipe\n",
	//		12,   // = length of string + terminating '\0' !!!
	//		&dwWritten,
	//		NULL);

	//	CloseHandle(hPipe);
	//}

	//return 0;
}

