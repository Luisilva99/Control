#include "AviaoUse.h"


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


int aviaoAeroVerify(PlaneData aviao) {
	for (int i = 0; i < aviao.buffer->curAero; i++)
	{
		if (_tcscmp(aviao.partida, (aviao.buffer->map)[i].aeroName) == 0) {
			if ((aviao.buffer->map)[i].curHang == (aviao.buffer->map)[i].maxHang)
			{
				return 0;
			}

			return 1;
		}
	}

	return 0;
}


int insertAviaoTemporary(PlaneData * aviao) {
	int i = 0;

	for (; i < aviao->buffer->curPlane; i++)
	{
		if ((aviao->buffer->planes)[i].id == aviao->id)
		{
			_tprintf(TEXT("\nID do avião já existe!\nNão poden existir dois Aviões ou mais com o mesmo ID.\n"));

			return 0;
		}
	}

	for (int j = 0; j < aviao->buffer->curAero; j++)
	{
		if (_tcscmp(aviao->partida, (aviao->buffer->map)[j].aeroName) == 0) {

			if ((aviao->buffer->map)[j].curHang == (aviao->buffer->map)[j].maxHang)
			{
				_tprintf(TEXT("\nAeroporto encontra-se em lotação total!\n"));

				return 0;
			}

			(aviao->buffer->planes)[i].id = aviao->id;

			(aviao->buffer->planes)[i].maxPass = aviao->maxPass;

			(aviao->buffer->planes)[i].velocidade = aviao->velocidade;

			_stprintf_s((aviao->buffer->planes)[i].partida, TAM, TEXT("%s"), aviao->partida);

			aviao->X = (aviao->buffer->map)[j].X;

			aviao->Y = (aviao->buffer->map)[j].Y;

			(aviao->buffer->planes)[i].X = aviao->X;

			(aviao->buffer->planes)[i].Y = aviao->Y;

			(aviao->buffer->map)[j].hangar[(aviao->buffer->map)[j].curHang].id = aviao->id;

			(aviao->buffer->map)[j].hangar[(aviao->buffer->map)[j].curHang].velocidade = aviao->velocidade;

			(aviao->buffer->map)[j].hangar[(aviao->buffer->map)[j].curHang].X = aviao->X;

			(aviao->buffer->map)[j].hangar[(aviao->buffer->map)[j].curHang].Y = aviao->Y;

			(aviao->buffer->map)[j].hangar[(aviao->buffer->map)[j].curHang].voar = 0;

			_stprintf_s((aviao->buffer->map)[j].hangar[(aviao->buffer->map)[j].curHang].partida, TAM, TEXT("%s"), aviao->partida);

			(aviao->buffer->map)[j].curHang++;

			(aviao->buffer->curPlane)++;

			return 1;
		}
	}

	return 0;
}


void listPlaneInfo(PlaneData aviao) {
	_tprintf(TEXT("\n__________________________________\n\nID: %d"), aviao.id);
	_tprintf(TEXT("\nChegada / Destino: %s"), aviao.destino);
	_tprintf(TEXT("\nPartida / Atual: %s"), aviao.partida);
	_tprintf(TEXT("\nVelocidade: %d"), aviao.velocidade);
	aviao.voar ? _tprintf(TEXT("\nEstado: Em voo")) : _tprintf(TEXT("\nEstado: Parado"));
	_tprintf(TEXT("\nPosição Atual: [X=%3d,Y=%3d]"), aviao.X, aviao.Y);
	!_tcslen(aviao.destino) ? _tprintf(TEXT("\nDestino  ainda não foi definidos!")) : _tprintf(TEXT("\nPosição Final: [X=%3d,Y=%3d]"), aviao.final_X, aviao.final_Y);
	_tprintf(TEXT("\nNº de Passageiros embarcados: %d\n__________________________________\n"), aviao.curPass);
}


int comandSwitcher(PlaneData * aviao, TCHAR * comand) {
	TCHAR * auxA;
	TCHAR * auxB = _tcstok_s(comand, TEXT(" "), &auxA);
	TCHAR msg[TAM_INPUT];

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
				_tprintf(TEXT("\nDEBUG viagemStop - Obriga o voo a parar a viagem.\n"));
			}
		}

		_tprintf(TEXT("\ndestino nomeAero - Define o próximo destino do Avião.\n"));
		_tprintf(TEXT("\ndestinoList - Apresenta a lista de todos os destinos.\n"));
		_tprintf(TEXT("\nviagemStart - Inicia a viagem para o destino definido.\n"));
		_tprintf(TEXT("\ninfoSelf - Apresenta a informação atual do Avião.\n"));
		_tprintf(TEXT("\nembarque - Avisa o Control que o Avião está pronto para receber passageiros.\n"));
		_tprintf(TEXT("\nexit - Terminar Sistema.\n"));

		return 1;
	}
	else if (_tcscmp(auxB, TEXT("infoSelf")) == 0)
	{
		listPlaneInfo(*(aviao));

		return 1;
	}
	else if (_tcscmp(auxB, TEXT("exit")) == 0)
	{
		_stprintf_s(msg, TAM_INPUT, TEXT("Terminar %d"), aviao->id);

		writeInCircularBuffer(aviao, msg);

		return 1;
	}
	else if (!(aviao->voar)) {
		if (_tcscmp(auxB, TEXT("destino")) == 0)
		{
			if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
			{
				int i = 0, j = 0, k = 0, h = 0;

				if (_tcscmp(auxB, aviao->partida) == 0)
				{
					_tprintf(TEXT("\nDestino inválido!\n"));

					return 0;
				}

				for (; i < aviao->buffer->curAero; i++)
				{
					if (_tcscmp(auxB, (aviao->buffer->map)[i].aeroName) == 0)
					{
						_tprintf(TEXT("\nDestino em Mapa de Control foi encontrado.\n"));

						break;
					}
				}

				_stprintf_s(aviao->destino, TAM, TEXT("%s"), (aviao->buffer->map)[i].aeroName);

				(*aviao).final_X = (aviao->buffer->map)[i].X;

				(*aviao).final_Y = (aviao->buffer->map)[i].Y;

				for (; j < aviao->buffer->curPlane; j++)
				{
					if ((aviao->buffer->planes)[j].id == aviao->id)
					{
						_tprintf(TEXT("\nAvião em Control foi encontrado.\n"));

						break;
					}
				}

				_stprintf_s((aviao->buffer->planes)[j].destino, TAM, TEXT("%s"), aviao->destino);

				(aviao->buffer->planes)[j].final_X = (*aviao).final_X;

				(aviao->buffer->planes)[j].final_Y = (*aviao).final_Y;

				_stprintf_s(msg, TAM_INPUT, TEXT("Destino %d %s %s %d %d"), aviao->id, aviao->partida, aviao->destino, aviao->final_X, aviao->final_Y);

				writeInCircularBuffer(aviao, msg);

				return 1;
			}

			return 0;
		}
		else if (_tcscmp(auxB, TEXT("destinoList")) == 0)
		{
			for (int i = 0; i < aviao->buffer->curAero; i++)
			{
				if (_tcscmp(aviao->partida, (aviao->buffer->map)[i].aeroName) != 0)
				{
					_tprintf(TEXT("\nAeroporto: %s \t\t Pos: [X=%d;Y=%d]\n"), (aviao->buffer->map)[i].aeroName, (aviao->buffer->map)[i].X, (aviao->buffer->map)[i].Y);
				}
			}

			return 1;
		}
		else if (_tcscmp(auxB, TEXT("viagemStart")) == 0)
		{
			if (!_tcslen(aviao->destino))
			{
				_tprintf(TEXT("\nNenhum destino atualmente definido.\n"));

				return 0;
			}

			SetEvent(aviao->eventMoveTrigger);

			return 1;
		}
		else if (_tcscmp(auxB, TEXT("embarque")) == 0)
		{
			if (!_tcslen(aviao->destino))
			{
				_tprintf(TEXT("\nNenhum destino atualmente definido.\n"));

				return 0;
			}

			_stprintf_s(msg, TAM_INPUT, TEXT("Embarcar %d"), aviao->id);

			writeInCircularBuffer(aviao, msg);

			//Cópia dos passageiros embarcados para este avião para a estrutura principal de info dele

			return 1;
		}
	}
	else if (_tcscmp(auxB, TEXT("DEBUG")) == 0)
	{
		if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
		{
			if (_tcscmp(auxB, TEXT("viagemStop")) == 0) {
				int j = 0;

				for (; j < aviao->buffer->curPlane; j++)
				{
					if ((aviao->buffer->planes)[j].id == aviao->id)
					{
						break;
					}
				}

				aviao->voar = 0;

				(aviao->buffer->planes)[j].voar = aviao->voar;

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
	PlaneData * pDataArray;
	pDataArray = (PlaneData*)lpParam;

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


DWORD WINAPI movePlane(LPVOID lpParam)
{
	PlaneData * aviao;
	aviao = (PlaneData*)lpParam;
	TCHAR msg[TAM_INPUT];

	do
	{
		WaitForSingleObject(aviao->eventMoveTrigger, INFINITE);

		int j = 0;

		for (; j < aviao->buffer->curPlane; j++)
		{
			if ((aviao->buffer->planes)[j].id == aviao->id)
			{
				_tprintf(TEXT("\nAvião em Control foi encontrado.\n"));//DEBUG

				break;
			}
		}

		aviao->voar = 1;

		(aviao->buffer->planes)[j].voar = aviao->voar;

		_stprintf_s(msg, TAM_INPUT, TEXT("viagemStart %d %s"), aviao->id, aviao->partida);

		writeInCircularBuffer(aviao, msg);

		while (aviao->X != aviao->final_X && aviao->Y != aviao->final_Y)
		{
			WaitForSingleObject(aviao->mutexMoveSync, INFINITE);

			int times = 0;

			while (times < aviao->velocidade)
			{
				int result = move((*aviao).X, (*aviao).Y, (*aviao).final_X, (*aviao).final_Y, &(*aviao).next_X, &(*aviao).next_Y);

				if (result == 0)
				{
					_tprintf(TEXT("\nAvião %d na posição [X=%3d;Y=%3d]\n"), aviao->id, aviao->X, aviao->Y);

					break;
				}
				else if(result == 2)
				{
					_tprintf(TEXT("\nErro na função move!\n"));

					return -1;
				}

				if (colisionDetector(aviao, (*aviao).next_X, (*aviao).next_Y))
				{
					if (!colisionDetector(aviao, (*aviao).X, (*aviao).Y + 1))//Cima
					{
						_tprintf(TEXT("\nDEBUG: Desvio Cima.\n"));

						aviao->Y = (*aviao).Y + 1;
					}
					else if (!colisionDetector(aviao, (*aviao).X, (*aviao).Y - 1))//Baixo
					{
						_tprintf(TEXT("\nDEBUG: Desvio Baixo.\n"));

						aviao->Y = (*aviao).Y - 1;
					}
					else if (!colisionDetector(aviao, (*aviao).X + 1, (*aviao).Y))//Direita
					{
						_tprintf(TEXT("\nDEBUG: Desvio Direita.\n"));

						aviao->X = (*aviao).X + 1;
					}
					else if (!colisionDetector(aviao, (*aviao).X - 1, (*aviao).Y))//Esquerda
					{
						_tprintf(TEXT("\nDEBUG: Desvio Esquerda.\n"));

						aviao->X = (*aviao).X - 1;
					}
					else if (!colisionDetector(aviao, (*aviao).X + 1, (*aviao).Y + 1))//Direita + Cima
					{
						_tprintf(TEXT("\nDEBUG: Desvio Direita + Cima.\n"));

						aviao->X = (*aviao).X + 1;

						aviao->Y = (*aviao).Y + 1;
					}
					else if (!colisionDetector(aviao, (*aviao).X - 1, (*aviao).Y - 1))//Equerda + Baixo
					{
						_tprintf(TEXT("\nDEBUG: Desvio Esquerda + Baixo.\n"));

						aviao->X = (*aviao).X - 1;

						aviao->Y = (*aviao).Y - 1;
					}
					else if (!colisionDetector(aviao, (*aviao).X - 1, (*aviao).Y + 1))//Esquerda + Cima
					{
						_tprintf(TEXT("\nDEBUG: Desvio Esquerda + Cima.\n"));

						aviao->X = (*aviao).X - 1;

						aviao->Y = (*aviao).Y + 1;
					}
					else if (!colisionDetector(aviao, (*aviao).X + 1, (*aviao).Y - 1))//Direita + Baixo
					{
						_tprintf(TEXT("\nDEBUG: Desvio Direita + Baixo.\n"));

						aviao->X = (*aviao).X + 1;

						aviao->Y = (*aviao).Y - 1;
					}
				}
				else
				{
					_tprintf(TEXT("\nSem colisão!\n"));

					aviao->X = aviao->next_X;

					aviao->Y = aviao->next_Y;
				}

				_tprintf(TEXT("\nAvião %d na posição [X=%3d;Y=%3d]\n"), aviao->id, aviao->X, aviao->Y);

				times++;
			}

			(aviao->buffer->planes)[j].X = aviao->X;

			(aviao->buffer->planes)[j].Y = aviao->Y;

			ReleaseMutex(aviao->mutexMoveSync);
		}

		_stprintf_s(msg, TAM_INPUT, TEXT("viagemEnd %d %s"), (*aviao).id, (*aviao).destino);

		writeInCircularBuffer(aviao, msg);

		aviao->voar = 0;

		(aviao->buffer->planes)[j].voar = aviao->voar;

		_stprintf_s((aviao->buffer->planes)[j].partida, TAM, TEXT("%s"), aviao->destino);

		_stprintf_s(aviao->partida, TAM, TEXT("%s"), aviao->destino);

		_stprintf_s((aviao->buffer->planes)[j].destino, TAM, TEXT("%s"), TEXT(""));

		_stprintf_s(aviao->destino, TAM, TEXT("%s"), TEXT(""));
	} while (1);

	return 0;
}


void writeInCircularBuffer(PlaneData * aviao, TCHAR * msg) {
	WaitForSingleObject(aviao->writeBuffer, INFINITE);

	int i;

	if ((aviao->buffer->tCircBuffer.locWriteAtual) == 0)
	{
		i = 0;
	}
	else
	{
		i = (aviao->buffer->tCircBuffer.locWriteAtual) % (aviao->buffer->tCircBuffer.maxBuffer);
	}

	_stprintf_s(aviao->buffer->tCircBuffer.buffer[i].msg, TAM_INPUT, TEXT("%s"), msg);

	(aviao->buffer->tCircBuffer.locWriteAtual)++;

	ReleaseMutex(aviao->writeBuffer);

	ReleaseSemaphore(aviao->readBuffer, 1, 0);
}


int colisionDetector(PlaneData * aviao, int nextX, int nextY) {
	int i = 0;

	if (0 > nextX || nextX > MAP_TAM)
	{
		return 1;
	}

	if (0 > nextY || nextY > MAP_TAM)
	{
		return 1;
	}

	for (; i < aviao->buffer->curPlane; i++)
	{
		if ((aviao->buffer->planes)[i].id != aviao->id && (aviao->buffer->planes)[i].voar == 1) {
			if (nextX == (aviao->buffer->planes)[i].X && nextY == (aviao->buffer->planes)[i].Y)
			{
				_tprintf(TEXT("\nColisão eminente detetada!\n"));

				return 1;
			}
		}
	}

	return 0;
}

