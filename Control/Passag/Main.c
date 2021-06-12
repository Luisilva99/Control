#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include "PassagUse.h"



int _tmain(int argc, TCHAR* argv[]) {
	// Threads
	HANDLE hThread[3];
	DWORD dwThread[3];

	// Sincronização
	HANDLE semaphorePassagGate;
	TCHAR respTriggerName[TAM_INPUT];

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
	_stprintf_s(pass.nome, TAM, TEXT("%s%d"), argv[3], (int)GetCurrentProcessId());

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

		_gettch();

		return -3;
	}

	if (WaitForSingleObject(semaphorePassagGate, 0) == WAIT_TIMEOUT)
	{
		_tprintf(TEXT("\nErro ao entrar no Sistema Control.\nSistema encontra-se lotado!\n"));

		_gettch();

		return -4;
	}

	_tprintf(TEXT("\nEntrada com sucesso nas vagas do Sistema Control!\n"));

	_stprintf_s(respTriggerName, TAM_INPUT, TEXT("%s%d"), pass.nome, (int)GetCurrentProcessId());

	pass.respTrigger = CreateEvent(
		NULL,
		FALSE,
		FALSE,
		respTriggerName
	);

	if (pass.respTrigger == NULL)
	{
		_tprintf(TEXT("\nCriação do evento %s de terminação de todos os sistemas não foi criado com sucesso!\nErro %d\n"), respTriggerName, GetLastError());

		ReleaseSemaphore(semaphorePassagGate, 1, 0);

		CloseHandle(semaphorePassagGate);

		_gettch();

		return -5;
	}

	_tprintf(TEXT("\nCriação do evento %s de envio de mensagens para o Sistema Control foi criado com sucesso!\n"), respTriggerName);

	//######-------------######//

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

		CloseHandle(pass.respTrigger);

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

		CloseHandle(pass.respTrigger);

		ReleaseSemaphore(semaphorePassagGate, 1, 0);

		CloseHandle(semaphorePassagGate);

		_gettch();

		return -7;
	}

	hThread[2] = CreateThread(
		NULL,
		0,
		tratamentoDeComunicacao,
		(LPVOID)&pass,
		0,
		&dwThread[2]
	);

	if (hThread[2] == NULL)
	{
		_tprintf(TEXT("CreateThread de Tratamento de Comunicação por Pipe failed, GLE=%d.\n"), GetLastError());

		CloseHandle(hThread[1]);

		CloseHandle(hThread[0]);

		CloseHandle(pass.respTrigger);

		ReleaseSemaphore(semaphorePassagGate, 1, 0);

		CloseHandle(semaphorePassagGate);

		_gettch();

		return -8;
	}

	//######-------######//

	WaitForMultipleObjects(3, hThread, FALSE, INFINITE);

	_tprintf(TEXT("\nLibertação da memória do Passag: %d\tNome: %s\n"), (int)GetCurrentProcessId(), pass.nome);

	for (int i = 0; i < 3; i++)
	{
		TerminateThread(hThread[i], dwThread[i]);

		CloseHandle(hThread[i]);
	}

	CloseHandle(pass.respTrigger);

	ReleaseSemaphore(semaphorePassagGate, 1, 0);

	CloseHandle(semaphorePassagGate);

	_gettch();

	return 0;
}

