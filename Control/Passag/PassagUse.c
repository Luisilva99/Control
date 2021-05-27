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
				_tprintf(TEXT("\nDEBUG MSG - [Temporário | Deprecated] Introduz uma mensagem no primeiro avião.\n"));
			}
		}

		_tprintf(TEXT("\ninPlane stop/start - Abrir e fechar a aceitação de novos aviões.\n"));
		_tprintf(TEXT("\ninPass stop/start - Abrir e fechar a aceitação de novos passageiros.\n"));
		_tprintf(TEXT("\ncreateAero Nome posX poxY - Criar um novo aeroporto.\n"));
		_tprintf(TEXT("\nlistAero - Apresenta a lista de todos os aeroportos registados e os seus detalhes.\n"));
		_tprintf(TEXT("\nlistPlane - Apresenta a lista de todos aviões registados.\n"));
		_tprintf(TEXT("\nexit - Terminar Sistema.\n"));

		return 1;
	}
	else if (_tcscmp(auxB, TEXT("inPlane")) == 0)
	{
		if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
		{
			if (_tcscmp(auxB, TEXT("start")) == 0)
			{


				return 1;
			}
			else if (_tcscmp(auxB, TEXT("stop")) == 0)
			{


				return 1;
			}
		}

		return 0;
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