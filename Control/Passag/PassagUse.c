#include "PassagUse.h"

int getIntInput() {
	TCHAR valor[TAM];

	fflush(stdin);
	_fgetts(valor, TAM, stdin);

	valor[_tcslen(valor) - 1] = TEXT('\0');

	// usar este para juntar strings com valores
	//_stprintf_s(chave_nome, TAM, TEXT("Software\\AULA\\%s"), par_nome);

	return _tstoi(valor);
}


void getTCHARInput(TCHAR * szMsg, int tam) {
	fflush(stdin);
	_fgetts(szMsg, tam, stdin);

	szMsg[_tcslen(szMsg) - 1] = TEXT('\0');
}


void listPassInfo(Passag pass) {
	if (_tcscmp(pass.partida, TEXT("")) != 0)
	{
		_tprintf(TEXT("\n--------------------\nNome: %s"), pass.nome);
		_tprintf(TEXT("\nChegada / Destino: %s"), pass.destino);
		_tprintf(TEXT("\nPartida / Atual: %s"), pass.partida);
		pass.voar ? _tprintf(TEXT("\nEstado: Em Viagem")) : _tprintf(TEXT("\nEstado: Em Espera"));

		if (pass.tempo != INFINITE)
		{
			_tprintf(TEXT("\nTempo de Espera: %d"), pass.tempo);
		}
		_tprintf(TEXT("\n--------------------\n"));
	}
}


int comandSwitcher(Passag * pass, TCHAR * comand) {
	TCHAR * auxA;
	TCHAR * auxB = _tcstok_s(comand, TEXT(" "), &auxA);

	if (auxB == NULL)
	{
		return 0;
	}

	//####Comandos####//

	if (_tcscmp(auxB, TEXT("help")) == 0)
	{
		_tprintf(TEXT("\nComandos:\n\n"));
		_tprintf(TEXT("\nhelp - Lista de comandos.\n"));


		if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
		{
			if (_tcscmp(auxB, TEXT("DEBUG")) == 0) {
				_tprintf(TEXT("\nDEBUG MSG mensagem - Envia uma mensagem para o Control.\n"));
			}
		}

		_tprintf(TEXT("\ninfoPass - Apresenta a informação atual do Passageiro.\n"));
		_tprintf(TEXT("\nexit - Terminar Sistema.\n"));

		return 1;
	}
	else if (_tcscmp(auxB, TEXT("infoPass")) == 0)
	{
		listPassInfo((*pass));

		return 1;
	}
	else if (_tcscmp(auxB, TEXT("DEBUG")) == 0)
	{
		if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
		{
			if (_tcscmp(auxB, TEXT("MSG")) == 0) {

				return 1;
			}

			return 0;
		}

		return 0;
	}

	//####--------####//

	return 0;
}


DWORD WINAPI tratamentoDeComandos(LPVOID lpParam)
{
	Passag * pDataArray;
	pDataArray = (Passag*)lpParam;

	TCHAR *comando = (TCHAR *)malloc(sizeof(TCHAR) * TAM_INPUT);

	_tprintf(TEXT("\nhelp - Lista de comandos.\n\n"));

	do
	{
		_tprintf(TEXT("\n> "));
		getTCHARInput(comando, TAM_INPUT);
		comandSwitcher(pDataArray, comando) ? _tprintf(TEXT("")) : _tprintf(TEXT("\nComando Incorreto.\n"));
	} while (_tcscmp(comando, TEXT("exit")));

	free(comando);

	return 0;
}


DWORD WINAPI tratamentoDeComunicacao(LPVOID lpParam)
{
	Passag * pDataArray;
	pDataArray = (Passag*)lpParam;

	int i = 0;
	BOOL ret;
	DWORD n;

	_tprintf(TEXT("[LEITOR] Esperar pelo pipe '%s' (WaitNamedPipe)\n"), PASSAG_PIPE);

	if (!WaitNamedPipe(PASSAG_PIPE, NMPWAIT_WAIT_FOREVER)) {
		_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'! (WaitNamedPipe)\n"), PASSAG_PIPE);

		return -1;
	}

	_tprintf(TEXT("[LEITOR] Ligação ao pipe do escritor... (CreateFile)\n"));

	pDataArray->hPipe = CreateFile(PASSAG_PIPE, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (pDataArray->hPipe == NULL) {
		_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'! (CreateFile)\n"), PASSAG_PIPE);

		return -2;
	}

	_tprintf(TEXT("[LEITOR] Liguei-me...\n"));

	_stprintf_s(pDataArray->msg, TAM, TEXT("Passag %s %s %s %d"), pDataArray->nome, pDataArray->partida, pDataArray->destino, pDataArray->tempo);

	if (!WriteFile(pDataArray->hPipe, pDataArray->msg, _tcslen(pDataArray->msg) * sizeof(TCHAR), &n, NULL)) {
		_tprintf(TEXT("[ERRO] Escrever no pipe! (WriteFile)\n"));

		return -3;
	}

	FlushFileBuffers(pDataArray->hPipe);

	while (1) {
		ret = ReadFile(pDataArray->hPipe, pDataArray->msg, sizeof(pDataArray->msg), &n, NULL);
		pDataArray->msg[n / sizeof(TCHAR)] = '\0';
		if (!ret || !n) {
			_tprintf(TEXT("[LEITOR] %d %d... (ReadFile)\n"), ret, n);

			break;
		}
		_tprintf(TEXT("[LEITOR] Recebi %d bytes: '%s'... (ReadFile)\n"), n, pDataArray->msg);

		if (_tcscmp(pDataArray->msg, TEXT("NO")))
		{
			_tprintf(TEXT("\nResposta negativa de entrada do Passag %s no Sistema Control.\n"), pDataArray->nome);

			break;
		}
	}

	CloseHandle(pDataArray->hPipe);

	return 0;
}