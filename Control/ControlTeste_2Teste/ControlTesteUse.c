#include "ControlTesteUse.h"

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


int readAeroLimits() {
	HKEY chave;
	LONG Ok_Key;
	TCHAR par_valor[TAM];
	LONG test_existance;
	DWORD tam_data;

	Ok_Key = RegOpenKeyExW(HKEY_CURRENT_USER, REG_SETTINGS_KEY, REG_OPTION_OPEN_LINK, KEY_READ | KEY_WRITE | KEY_SET_VALUE | KEY_QUERY_VALUE, &chave);

	if (Ok_Key == ERROR_SUCCESS)
	{
		_tprintf(TEXT("\nA chave existe.\n"));
	}
	else
	{
		_tprintf(TEXT("\nA chave não existe, vai ser criada.\n"));

		LONG Create_Key = RegCreateKeyExW(HKEY_CURRENT_USER, REG_SETTINGS_KEY, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE | KEY_SET_VALUE | KEY_QUERY_VALUE, NULL, &chave, NULL);

		if (Create_Key == ERROR_SUCCESS)
		{
			_tprintf(TEXT("\nChave foi criada com sucesso!"));

			return -1;
		}
		else
		{
			_tprintf(TEXT("\nChave não foi possível ser criada!"));

			return -2;
		}
	}

	tam_data = sizeof(par_valor);

	test_existance = RegQueryValueEx(chave, SETTINGS_AERO, 0, NULL, (LPBYTE)par_valor, &tam_data);

	if (test_existance == ERROR_SUCCESS)
	{
		_tprintf(TEXT("\nLeitura no regedit bem sucedida!\n"));
	}
	else
	{
		_tprintf(TEXT("Leitura no regedit falhada!\n"));

		return -3;
	}

	return _tstoi(par_valor);
}


int readPlaneLimits() {
	HKEY chave;
	LONG Ok_Key;
	TCHAR par_valor[TAM];
	LONG test_existance;
	DWORD tam_data;

	Ok_Key = RegOpenKeyExW(HKEY_CURRENT_USER, REG_SETTINGS_KEY, REG_OPTION_OPEN_LINK, KEY_READ | KEY_WRITE | KEY_SET_VALUE | KEY_QUERY_VALUE, &chave);

	if (Ok_Key == ERROR_SUCCESS)
	{
		_tprintf(TEXT("\nA chave existe.\n"));
	}
	else
	{
		_tprintf(TEXT("\nA chave não existe, vai ser criada.\n"));

		LONG Create_Key = RegCreateKeyExW(HKEY_CURRENT_USER, REG_SETTINGS_KEY, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE | KEY_SET_VALUE | KEY_QUERY_VALUE, NULL, &chave, NULL);

		if (Create_Key == ERROR_SUCCESS)
		{
			_tprintf(TEXT("\nChave foi criada com sucesso!"));

			return -1;
		}
		else
		{
			_tprintf(TEXT("\nChave não foi possível ser criada!"));

			return -2;
		}
	}

	tam_data = sizeof(par_valor);

	test_existance = RegQueryValueEx(chave, SETTINGS_PLANE, 0, NULL, (LPBYTE)par_valor, &tam_data);

	if (test_existance == ERROR_SUCCESS)
	{
		_tprintf(TEXT("\nLeitura no regedit bem sucedida!\n"));
	}
	else
	{
		_tprintf(TEXT("Leitura no regedit falhada!\n"));

		return -3;
	}

	return _tstoi(par_valor);
}


int createAeroLimits(int valor) {
	HKEY chave;
	LONG Ok_Key;
	LONG test_existance;
	DWORD tam_data;
	TCHAR saveValor[TAM];

	_stprintf_s(saveValor, TAM, TEXT("%d"), valor);

	Ok_Key = RegOpenKeyExW(HKEY_CURRENT_USER, REG_SETTINGS_KEY, REG_OPTION_OPEN_LINK, KEY_READ | KEY_WRITE | KEY_SET_VALUE | KEY_QUERY_VALUE, &chave);

	if (Ok_Key == ERROR_SUCCESS)
	{
		_tprintf(TEXT("\nA chave existe.\n"));
	}
	else
	{
		_tprintf(TEXT("\nErro ao abrir a chave / não existe esta chave!\n"));

		return -1;
	}

	tam_data = _tcslen(saveValor) + 1;

	test_existance = RegQueryValueEx(chave, SETTINGS_AERO, 0, NULL, (LPBYTE)valor, &tam_data);

	if (test_existance == ERROR_FILE_NOT_FOUND)
	{
		_tprintf(TEXT("\nLimite não existe, vai ser criado com este valor.\n"));

		LONG add_value = RegSetValueEx(chave, SETTINGS_AERO, 0, REG_SZ, (LPBYTE)saveValor, sizeof(saveValor));

		if (add_value == ERROR_SUCCESS)
		{
			_tprintf(TEXT("\nLimite foi adicionado com sucesso!"));
		}
		else
		{
			_tprintf(TEXT("\nErro ao adicionar o novo limite!"));

			return -2;
		}
	}
	else
	{
		_tprintf(TEXT("\nJá existe este limite vou atualizar.\n"));

		LONG add_value = RegSetValueEx(chave, SETTINGS_AERO, 0, REG_SZ, (LPBYTE)saveValor, sizeof(saveValor));

		if (add_value == ERROR_SUCCESS)
		{
			_tprintf(TEXT("\nLimite foi atualizado com sucesso!"));
		}
		else
		{
			_tprintf(TEXT("\nErro ao atualizar o novo limite!"));

			return -3;
		}
	}

	return 0;
}


int createPlaneLimits(int valor) {
	HKEY chave;
	LONG Ok_Key;
	LONG test_existance;
	DWORD tam_data;
	TCHAR saveValor[TAM];

	_stprintf_s(saveValor, TAM, TEXT("%d"), valor);

	Ok_Key = RegOpenKeyExW(HKEY_CURRENT_USER, REG_SETTINGS_KEY, REG_OPTION_OPEN_LINK, KEY_READ | KEY_WRITE | KEY_SET_VALUE | KEY_QUERY_VALUE, &chave);

	if (Ok_Key == ERROR_SUCCESS)
	{
		_tprintf(TEXT("\nA chave existe.\n"));
	}
	else
	{
		_tprintf(TEXT("\nErro ao abrir a chave / não existe esta chave!\n"));

		return -1;
	}

	tam_data = _tcslen(saveValor) + 1;

	test_existance = RegQueryValueEx(chave, SETTINGS_PLANE, 0, NULL, (LPBYTE)valor, &tam_data);

	if (test_existance == ERROR_FILE_NOT_FOUND)
	{
		_tprintf(TEXT("\nLimite não existe, vai ser criado com este valor.\n"));

		LONG add_value = RegSetValueEx(chave, SETTINGS_PLANE, 0, REG_SZ, (LPBYTE)saveValor, sizeof(saveValor));

		if (add_value == ERROR_SUCCESS)
		{
			_tprintf(TEXT("\nLimite foi adicionado com sucesso!"));
		}
		else
		{
			_tprintf(TEXT("\nErro ao adicionar o novo limite!"));

			return -2;
		}
	}
	else
	{
		_tprintf(TEXT("\nJá existe este limite vou atualizar.\n"));

		LONG add_value = RegSetValueEx(chave, SETTINGS_PLANE, 0, REG_SZ, (LPBYTE)saveValor, sizeof(saveValor));

		if (add_value == ERROR_SUCCESS)
		{
			_tprintf(TEXT("\nLimite foi atualizado com sucesso!"));
		}
		else
		{
			_tprintf(TEXT("\nErro ao atualizar o novo limite!"));

			return -3;
		}
	}

	return 0;
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


void listPlaneInfo(Plane plane) {
	if (plane.id != 0)
	{
		_tprintf(TEXT("\n#############\nID: %d"), plane.id);
		_tprintf(TEXT("\nChegada / Destino: %s"), plane.destino);
		_tprintf(TEXT("\nPartida / Atual: %s"), plane.partida);
		_tprintf(TEXT("\nVelocidade: %d"), plane.velocidade);
		plane.voar ? _tprintf(TEXT("\nEstado: Em voo")) : _tprintf(TEXT("\nEstado: Parado"));
		_tprintf(TEXT("\nPosição atual: [X=%3d,Y=%3d]"), plane.X, plane.Y);
		if (plane.voar)
		{
			_tprintf(TEXT("\nPosição seguinte: [X=%3d,Y=%3d]"), plane.next_X, plane.next_Y);
		}
		!_tcslen(plane.destino) ? _tprintf(TEXT("")) : _tprintf(TEXT("\nPosição final: [X=%3d,Y=%3d]"), plane.final_X, plane.final_Y);
		_tprintf(TEXT("\nNº de Passageiros embarcados: %d\n#############\n"), plane.curPass);
	}
}


int comandSwitcher(ControlData * control, TCHAR * comand) {
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
				_tprintf(TEXT("\nDEBUG MSG mensagem - Tenta enviar uma mensagem global para os Passag conectados.\n"));
			}
		}

		_tprintf(TEXT("\ninPlane stop/start - Abrir e fechar a aceitação de novos aviões.\n"));
		_tprintf(TEXT("\ninPass stop/start - Abrir e fechar a aceitação de novos passageiros.\n"));
		_tprintf(TEXT("\ncreateAero Nome posX poxY - Criar um novo aeroporto.\n"));
		_tprintf(TEXT("\nlistAero - Apresenta a lista de todos os aeroportos registados e os seus detalhes.\n"));
		_tprintf(TEXT("\nlistPlane - Apresenta a lista de todos aviões registados.\n"));
		_tprintf(TEXT("\nlistPassag - Apresenta a lista de todos passageiros registados.\n"));
		_tprintf(TEXT("\nexit - Terminar Sistema.\n"));

		return 1;
	}
	else if (_tcscmp(auxB, TEXT("inPlane")) == 0)
	{
		if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
		{
			if (_tcscmp(auxB, TEXT("start")) == 0)
			{
				if (control->entryStopped == 0)
				{
					_tprintf(TEXT("\nAviões continuam a ser permitidos entrar no Sistema.\n"));
				}
				else
				{
					ReleaseMutex(control->entry);

					control->entryStopped = 0;

					_tprintf(TEXT("\nAviões são permitidos entrar no Sistema.\n"));
				}

				return 1;
			}
			else if (_tcscmp(auxB, TEXT("stop")) == 0)
			{
				if (control->entryStopped == 1)
				{
					_tprintf(TEXT("\nAviões continuam a não ser permitidos entrar no Sistema.\n"));
				}
				else
				{
					WaitForSingleObject(control->entry, INFINITE);

					control->entryStopped = 1;

					_tprintf(TEXT("\nAviões não são permitidos entrar no Sistema.\n"));
				}

				return 1;
			}
		}

		return 0;
	}
	else if (_tcscmp(auxB, TEXT("inPass")) == 0)
	{
		if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
		{
			if (_tcscmp(auxB, TEXT("start")) == 0)
			{
				_tprintf(TEXT("\nPassageiros são permitidos entrar no Sistema.\n"));
				return 1;
			}
			else if (_tcscmp(auxB, TEXT("stop")) == 0)
			{
				_tprintf(TEXT("\nPassageiros não são permitidos entrar no Sistema.\n"));
				return 1;
			}
		}

		return 0;
	}
	else if (_tcscmp(auxB, TEXT("createAero")) == 0)
	{
		if (control->shared->curAero < control->shared->maxAero)
		{
			if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
			{
				int index = 0, X = -1, Y = -1;
				TCHAR nome[TAM_INPUT];

				//<Verificação de Nomes>//
				for (; index < control->shared->maxAero; index++)
				{
					if (_tcscmp((control->shared->map + index)->aeroName, auxB) == 0)
					{
						_tprintf(TEXT("\nJá existe um Aeroporto com esse nome.\n"));

						return 0;
					}
					else if (_tcscmp((control->shared->map + index)->aeroName, TEXT("")) == 0)
					{
						_tprintf(TEXT("\nEncontrei um espaço vazio.\nNome válido.\n"));

						_stprintf_s(nome, TAM_INPUT, TEXT("%s"), auxB);

						break;
					}
				}
				//<####################>//

				//<Verificação das Coordenadas>//
				if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
				{
					X = _tstoi(auxB);
				}
				else
				{
					return 0;
				}

				if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
				{
					Y = _tstoi(auxB);
				}
				else
				{
					return 0;
				}

				if ((X < 0 || X > MAP_TAM) || (Y < 0 || Y > MAP_TAM))
				{
					_tprintf(TEXT("\nCoordenadas inválidas!\n"));

					return 0;
				}
				//<###########################>//

				//<Verificação de Área de Distanciamento>//
				for (int i = 0; i < control->shared->curAero; i++)
				{
					int left_X = ((control->shared->map)[i].X - POS_AERO_AREA);

					int right_X = ((control->shared->map)[i].X + POS_AERO_AREA);

					int bottom_Y = ((control->shared->map)[i].Y - POS_AERO_AREA);

					int top_Y = ((control->shared->map)[i].Y + POS_AERO_AREA);

					if ((left_X < X && X < right_X) && (bottom_Y < Y && Y < top_Y))
					{
						_tprintf(TEXT("\nCoordenadas ocupadas!\nEspaço aéreo ocupado por outro Aeroporto.\n"));

						return 0;
					}
				}
				//<#####################################>//

				_stprintf_s((control->shared->map)[index].aeroName, TAM, TEXT("%s"), nome);

				(control->shared->map)[index].X = X;

				(control->shared->map)[index].Y = Y;

				(control->shared->curAero)++;

				_tprintf(TEXT("\nAeroporto %s criado com sucesso na posição [X=%d,Y=%d]\n"), nome, X, Y);

				return 1;
			}
		}
		else
		{
			_tprintf(TEXT("\nNão é possível criar mais Aeroportos.\nLimite máximo alcançado.\n"));
		}

		return 0;
	}
	else if (_tcscmp(auxB, TEXT("listAero")) == 0)
	{
		if (control->shared->curAero <= 0)
		{
			_tprintf(TEXT("\nNão existem Aeroportos no Mapa.\n"));

			return 0;
		}

		for (int i = 0; i < control->shared->curAero; i++)
		{
			_tprintf(TEXT("\n_________________________________\nAeroporto %s\n"), (control->shared->map)[i].aeroName);
			_tprintf(TEXT("\nPassageiros dentro do Aeroporto: %d\n"), (control->shared->map)[i].curPass);
			for (int j = 0; j < (control->shared->map)[i].curPass; j++)
			{
				listPassInfo((control->shared->map)[i].passageiros[j]);
			}
			_tprintf(TEXT("\nAviões dentro do Aeroporto: %d\n"), (control->shared->map)[i].curHang);
			for (int j = 0; j < (control->shared->map)[i].curHang; j++)
			{
				listPlaneInfo((control->shared->map)[i].hangar[j]);
			}
			_tprintf(TEXT("\nPosição do Aeroporto: [X=%3d,Y=%3d]\n_________________________________\n"), (control->shared->map)[i].X, (control->shared->map)[i].Y);
		}

		return 1;
	}
	else if (_tcscmp(auxB, TEXT("listPlane")) == 0)
	{
		if (control->shared->curPlane <= 0)
		{
			_tprintf(TEXT("\nNão existem Aviões no Sistema.\n"));

			return 0;
		}

		for (int i = 0; i < control->shared->curPlane; i++)
		{
			listPlaneInfo((control->shared->planes)[i]);
		}

		return 1;
	}
	else if (_tcscmp(auxB, TEXT("listPassag")) == 0)
	{
		if (control->curPass <= 0)
		{
			_tprintf(TEXT("\nNão existem Passageiros no Sistema.\n"));

			return 0;
		}

		for (int i = 0; i < control->maxPass; i++)
		{
			listPassInfo((control->Pass)[i]);
		}

		return 1;
	}
	else if (_tcscmp(auxB, TEXT("exit")) == 0)
	{
		SetEvent(control->systemShutdown);

		return 1;
	}
	else if (_tcscmp(auxB, TEXT("DEBUG")) == 0)
	{
		if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
		{
			if (_tcscmp(auxB, TEXT("MSG")) == 0) {
				if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL) {
					//_stprintf_s(control->msg, TAM, TEXT("%s"), auxB);

					//SetEvent(control->passagSender);

					return 1;
				}

				return 0;
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
	ControlData * pDataArray;
	pDataArray = (ControlData*)lpParam;

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


DWORD WINAPI bufferCircular(LPVOID lpParam)
{
	ControlData * control;
	control = (ControlData*)lpParam;

	while (1)
	{
		WaitForSingleObject(control->readBuffer, INFINITE);

		int i;

		if ((control->shared->tCircBuffer.locReadAtual) == 0)
		{
			i = 0;
		}
		else
		{
			i = (control->shared->tCircBuffer.locReadAtual) % (control->shared->tCircBuffer.maxBuffer);
		}

		TCHAR * auxA;
		TCHAR * auxB = _tcstok_s(control->shared->tCircBuffer.buffer[i].msg, TEXT(" "), &auxA);

		if (auxB == NULL)
		{
			_stprintf_s(control->shared->tCircBuffer.buffer[i].msg, TAM_INPUT, TEXT("%s"), TEXT(""));

			(control->shared->tCircBuffer.locReadAtual)++;

			continue;
		}

		if (_tcscmp(auxB, TEXT("Embarcar")) == 0)
		{
			int id;

			if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
			{
				id = _tstoi(auxB);

				int i = 0, j = 0, r = 0;
				DWORD n;

				for (; i < control->shared->curPlane; i++)
				{
					if ((control->shared->planes)[i].id == id) {
						break;
					}
				}

				for (; j < control->shared->curAero; j++)
				{
					if (_tcscmp((control->shared->map)[j].aeroName, (control->shared->planes)[i].partida) == 0) {
						break;
					}
				}

				for (; r < (control->shared->map)[j].curHang; r++)
				{
					if ((control->shared->map)[j].hangar[r].id == id) {
						break;
					}
				}

				for (int h = 0; h < (control->shared->map)[j].curPass; h++)
				{
					if (_tcscmp((control->shared->map)[j].passageiros[h].destino, (control->shared->planes)[i].destino) == 0)
					{
						//#######Aviso de Chegada ao Destino aos Passag#######//
						int kk = 0;

						for (; kk < control->maxPass; kk++)
						{
							if (_tcscmp(control->Pass[kk].nome, (control->shared->map)[j].passageiros[h].nome) == 0)
							{
								_stprintf_s((*control).Pass[kk].msg, TAM, TEXT("%s"), TEXT(""));

								_stprintf_s((*control).Pass[kk].resp, TAM, TEXT("%s"), TEXT("EMBARCADO"));

								_stprintf_s((control->shared->map)[j].passageiros[h].msg, TAM, TEXT("%s"), TEXT(""));

								_stprintf_s((control->shared->map)[j].passageiros[h].resp, TAM, TEXT("%s"), TEXT("EMBARCADO"));

								(control->shared->map)[j].passageiros[h].Comns = control->Pass[kk].Comns;

								break;
							}
						}

						if (!WriteFile(control->Pass[kk].Comns, (control->shared->map)[j].passageiros[h].resp, _tcslen((control->shared->map)[j].passageiros[h].resp) * sizeof(TCHAR), &n, NULL)) {
							_tprintf(TEXT("[ERRO] Escrever no pipe! (WriteFile)\n"));

							break;
						}

						FlushFileBuffers(control->Pass[kk].Comns);

						_stprintf_s((control->shared->map)[j].passageiros[h].resp, TAM, TEXT("%s"), TEXT(""));

						//#######--------------------------------------#######//
					}
				}

				for (int h = 0; h < (control->shared->map)[j].curPass; h++)
				{
					if (_tcscmp((control->shared->map)[j].passageiros[h].destino, (control->shared->planes)[i].destino) == 0)
					{
						if ((control->shared->planes)[i].curPass < (control->shared->planes)[i].maxPass)
						{
							(control->shared->planes)[i].pass[(control->shared->planes)[i].curPass] = (control->shared->map)[j].passageiros[h];

							(control->shared->planes)[i].curPass++;

							(control->shared->map)[j].hangar[r].pass[(control->shared->planes)[i].curPass] = (control->shared->map)[j].passageiros[h];

							(control->shared->map)[j].hangar[r].curPass++;

							if ((control->shared->map)[j].curPass == 1)
							{
								_stprintf_s((control->shared->map)[j].passageiros[0].partida, TAM, TEXT("%s"), TEXT(""));
								_stprintf_s((control->shared->map)[j].passageiros[0].destino, TAM, TEXT("%s"), TEXT(""));
								_stprintf_s((control->shared->map)[j].passageiros[0].nome, TAM, TEXT("%s"), TEXT(""));
								_stprintf_s((control->shared->map)[j].passageiros[0].msg, TAM, TEXT("%s"), TEXT(""));
								_stprintf_s((control->shared->map)[j].passageiros[0].resp, TAM, TEXT("%s"), TEXT(""));

								(control->shared->map)[j].passageiros[0].Comns = 0;

								(control->shared->map)[j].passageiros[0].tempo = 0;
								(control->shared->map)[j].passageiros[0].voar = 0;

								(control->shared->map)[j].curPass--;

								h--;
							}
							else
							{
								int k = h;

								for (; k < (control->shared->map)[j].curPass - 1; k++)
								{
									(control->shared->map)[j].passageiros[k] = (control->shared->map)[j].passageiros[k + 1];
								}

								_stprintf_s((control->shared->map)[j].passageiros[k + 1].partida, TAM, TEXT("%s"), TEXT(""));
								_stprintf_s((control->shared->map)[j].passageiros[k + 1].destino, TAM, TEXT("%s"), TEXT(""));
								_stprintf_s((control->shared->map)[j].passageiros[k + 1].nome, TAM, TEXT("%s"), TEXT(""));
								_stprintf_s((control->shared->map)[j].passageiros[k + 1].msg, TAM, TEXT("%s"), TEXT(""));
								_stprintf_s((control->shared->map)[j].passageiros[k + 1].resp, TAM, TEXT("%s"), TEXT(""));

								(control->shared->map)[j].passageiros[k + 1].Comns = 0;

								(control->shared->map)[j].passageiros[k + 1].tempo = 0;
								(control->shared->map)[j].passageiros[k + 1].voar = 0;

								(control->shared->map)[j].curPass--;

								h--;
							}
						}
						else
						{
							break;
						}
					}
				}
			}
		}
		else if (_tcscmp(auxB, TEXT("Terminar")) == 0)
		{
			int id;

			if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
			{
				id = _tstoi(auxB);

				for (int i = 0; i < control->shared->curPlane; i++)
				{
					if ((control->shared->planes)[i].id == id)
					{
						if ((control->shared->planes)[i].voar)
						{
							if (deletePlaneFlying(control, i))
							{
								_tprintf(TEXT("\nAvião %d sofreu um acidente e foi perdido!\n"), id);
							}
							else
							{
								_tprintf(TEXT("\nErro ao tentar terminar o Avião %d.\n"), id);
							}

							break;
						}
						else
						{
							int k = 0, t = 0;

							for (; k < control->shared->curAero; k++)
							{
								if (_tcscmp((control->shared->planes)[i].partida, (control->shared->map)[k].aeroName) == 0)
								{
									break;
								}
							}

							for (; t < (control->shared->map)[k].curHang; t++)
							{
								if ((control->shared->map)[k].hangar[t].id == id)
								{
									for (int op = t; op < (control->shared->map)[k].curHang; op++)
									{
										(control->shared->map)[k].hangar[op] = (control->shared->map)[k].hangar[op + 1];
									}

									(control->shared->map)[k].hangar[(control->shared->map)[k].curHang].curPass = 0;


									_stprintf_s((control->shared->map)[k].hangar[(control->shared->map)[k].curHang].destino, TAM, TEXT("%s"), TEXT(""));
									_stprintf_s((control->shared->map)[k].hangar[(control->shared->map)[k].curHang].partida, TAM, TEXT("%s"), TEXT(""));

									ZeroMemory((control->shared->map)[k].hangar[(control->shared->map)[k].curHang].pass, sizeof(Passag) * MAX_PASS);

									(control->shared->map)[k].hangar[(control->shared->map)[k].curHang].curPass = 0;
									(control->shared->map)[k].hangar[(control->shared->map)[k].curHang].id = 0;
									(control->shared->map)[k].hangar[(control->shared->map)[k].curHang].maxPass = 0;
									(control->shared->map)[k].hangar[(control->shared->map)[k].curHang].next_X = 0;
									(control->shared->map)[k].hangar[(control->shared->map)[k].curHang].next_Y = 0;
									(control->shared->map)[k].hangar[(control->shared->map)[k].curHang].final_X = 0;
									(control->shared->map)[k].hangar[(control->shared->map)[k].curHang].final_Y = 0;
									(control->shared->map)[k].hangar[(control->shared->map)[k].curHang].velocidade = 0;
									(control->shared->map)[k].hangar[(control->shared->map)[k].curHang].X = 0;
									(control->shared->map)[k].hangar[(control->shared->map)[k].curHang].Y = 0;
									(control->shared->map)[k].hangar[(control->shared->map)[k].curHang].voar = 0;

									(control->shared->map)[k].curHang--;

									break;
								}
							}

							if (deletePlaneFlying(control, i))
							{
								_tprintf(TEXT("\nPiloto do Avião %d reformou-se e foi perdido!\n"), id);
							}
							else
							{
								_tprintf(TEXT("\nErro ao tentar terminar o Avião %d.\n"), id);
							}

							break;
						}
					}
				}
			}
		}
		else if (_tcscmp(auxB, TEXT("Destino")) == 0)
		{
			int id, final_X, final_Y;
			TCHAR destino[TAM], partida[TAM];

			if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
			{
				id = _tstoi(auxB);

				if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
				{
					_stprintf_s(partida, TAM, TEXT("%s"), auxB);

					if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
					{
						_stprintf_s(destino, TAM, TEXT("%s"), auxB);

						if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
						{
							final_X = _tstoi(auxB);

							if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
							{
								final_Y = _tstoi(auxB);

								for (int i = 0; i < control->shared->curAero; i++)
								{
									if (_tcscmp(partida, (control->shared->map)[i].aeroName) == 0)
									{
										for (int k = 0; k < (control->shared->map)[i].curHang; k++)
										{
											if ((control->shared->map)[i].hangar[k].id == id)
											{
												_stprintf_s((control->shared->map)[i].hangar[k].destino, TAM, TEXT("%s"), destino);

												(control->shared->map)[i].hangar[k].final_X = final_X;

												(control->shared->map)[i].hangar[k].final_Y = final_Y;

												break;
											}
										}

										break;
									}
								}
							}
						}
					}
				}
			}
		}
		else if (_tcscmp(auxB, TEXT("viagemStart")) == 0)
		{
			int id, k = 0;
			DWORD n;

			if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
			{
				id = _tstoi(auxB);

				for (; k < control->shared->curPlane; k++)
				{
					if ((control->shared->planes)[k].id == id)
					{
						break;
					}
				}

				//#######Aviso de Chegada ao Destino aos Passag#######//

				for (int i = 0; i < (control->shared->planes)[k].curPass; i++)
				{
					_stprintf_s((control->shared->planes)[k].pass[i].resp, TAM, TEXT("%s"), TEXT("VIAJAR"));

					if (!WriteFile((control->shared->planes)[k].pass[i].Comns, (control->shared->planes)[k].pass[i].resp, _tcslen((control->shared->planes)[k].pass[i].resp) * sizeof(TCHAR), &n, NULL)) {
						_tprintf(TEXT("[ERRO] Escrever no pipe! (WriteFile)\n"));

						break;
					}

					FlushFileBuffers((control->shared->planes)[k].pass[i].Comns);
				}

				//#######--------------------------------------#######//

				if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
				{
					for (int i = 0; i < control->shared->curAero; i++)
					{
						if (_tcscmp((control->shared->map)[i].aeroName, auxB) == 0)
						{
							for (int k = 0; k < (control->shared->map)[i].curHang; k++)
							{
								(control->shared->map)[i].hangar[k] = (control->shared->map)[i].hangar[k + 1];
							}

							_stprintf_s((control->shared->map)[i].hangar[(control->shared->map)[i].curHang].destino, TAM, TEXT("%s"), TEXT(""));
							_stprintf_s((control->shared->map)[i].hangar[(control->shared->map)[i].curHang].partida, TAM, TEXT("%s"), TEXT(""));

							ZeroMemory((control->shared->map)[i].hangar[(control->shared->map)[i].curHang].pass, sizeof(Passag) * control->shared->planes[i].maxPass);

							(control->shared->map)[i].hangar[(control->shared->map)[i].curHang].curPass = 0;
							(control->shared->map)[i].hangar[(control->shared->map)[i].curHang].id = 0;
							(control->shared->map)[i].hangar[(control->shared->map)[i].curHang].maxPass = 0;
							(control->shared->map)[i].hangar[(control->shared->map)[i].curHang].next_X = 0;
							(control->shared->map)[i].hangar[(control->shared->map)[i].curHang].next_Y = 0;
							(control->shared->map)[i].hangar[(control->shared->map)[i].curHang].final_X = 0;
							(control->shared->map)[i].hangar[(control->shared->map)[i].curHang].final_Y = 0;
							(control->shared->map)[i].hangar[(control->shared->map)[i].curHang].velocidade = 0;
							(control->shared->map)[i].hangar[(control->shared->map)[i].curHang].X = 0;
							(control->shared->map)[i].hangar[(control->shared->map)[i].curHang].Y = 0;
							(control->shared->map)[i].hangar[(control->shared->map)[i].curHang].voar = 0;

							(control->shared->map)[i].curHang--;
						}
					}
				}
			}
		}
		else if (_tcscmp(auxB, TEXT("viagemEnd")) == 0)
		{
			int id, k = 0;
			DWORD n;

			if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
			{
				id = _tstoi(auxB);

				for (; k < control->shared->curPlane; k++)
				{
					if ((control->shared->planes)[k].id == id)
					{
						break;
					}
				}

				//#######Aviso de Chegada ao Destino aos Passag#######//

				for (int i = 0; i < (control->shared->planes)[k].curPass; i++)
				{
					_stprintf_s((control->shared->planes)[k].pass[i].resp, TAM, TEXT("%s"), TEXT("CHEGOU"));

					if (!WriteFile((control->shared->planes)[k].pass[i].Comns, (control->shared->planes)[k].pass[i].resp, _tcslen((control->shared->planes)[k].pass[i].resp) * sizeof(TCHAR), &n, NULL)) {
						_tprintf(TEXT("[ERRO] Escrever no pipe! (WriteFile)\n"));

						break;
					}

					FlushFileBuffers((control->shared->planes)[k].pass[i].Comns);
				}

				//#######--------------------------------------#######//

				if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
				{
					for (int i = 0; i < control->shared->curAero; i++)
					{
						if (_tcscmp((control->shared->map)[i].aeroName, auxB) == 0)
						{
							if ((control->shared->map)[i].maxHang == (control->shared->map)[i].curHang)
							{
								//DEBUG / Eventualmente nunca irá acontecer
								continue;
							}

							_stprintf_s((control->shared->map)[i].hangar[(control->shared->map)[i].curHang].destino, TAM, TEXT("%s"), TEXT(""));
							_stprintf_s((control->shared->map)[i].hangar[(control->shared->map)[i].curHang].partida, TAM, TEXT("%s"), auxB);

							ZeroMemory((control->shared->map)[i].hangar[(control->shared->map)[i].curHang].pass, sizeof(Passag) * control->shared->planes[i].maxPass);

							(control->shared->map)[i].hangar[(control->shared->map)[i].curHang].curPass = 0;
							(control->shared->map)[i].hangar[(control->shared->map)[i].curHang].id = (control->shared->planes)[k].id;
							(control->shared->map)[i].hangar[(control->shared->map)[i].curHang].maxPass = (control->shared->planes)[k].maxPass;
							(control->shared->map)[i].hangar[(control->shared->map)[i].curHang].next_X = 0;
							(control->shared->map)[i].hangar[(control->shared->map)[i].curHang].next_Y = 0;
							(control->shared->map)[i].hangar[(control->shared->map)[i].curHang].final_X = 0;
							(control->shared->map)[i].hangar[(control->shared->map)[i].curHang].final_Y = 0;
							(control->shared->map)[i].hangar[(control->shared->map)[i].curHang].velocidade = (control->shared->planes)[k].velocidade;
							(control->shared->map)[i].hangar[(control->shared->map)[i].curHang].X = (control->shared->map)[i].X;
							(control->shared->map)[i].hangar[(control->shared->map)[i].curHang].Y = (control->shared->map)[i].Y;
							(control->shared->map)[i].hangar[(control->shared->map)[i].curHang].voar = 0;

							(control->shared->map)[i].curHang++;

							break;
						}
					}
				}
			}
		}

		_stprintf_s(control->shared->tCircBuffer.buffer[i].msg, TAM_INPUT, TEXT("%s"), TEXT(""));

		(control->shared->tCircBuffer.locReadAtual)++;
	}

	return 0;
}


int veryPassagEntry(MapUnit * map[], int * curAero, Passag * pass, int * curPass, TCHAR * msg) {
	TCHAR * auxA;
	TCHAR * auxB = _tcstok_s(msg, TEXT(" "), &auxA);

	if (auxB == NULL)
	{
		return 0;
	}

	if (_tcscmp(auxB, TEXT("Passag")) == 0)
	{
		TCHAR partida[TAM], chegada[TAM], nome[TAM];
		int tempo = INFINITE, p = 0, c = 0, i = 0;

		if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
		{
			_stprintf_s(nome, TAM, TEXT("%s"), auxB);
		}

		if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
		{
			_stprintf_s(partida, TAM, TEXT("%s"), auxB);
		}

		if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
		{
			_stprintf_s(chegada, TAM, TEXT("%s"), auxB);
		}

		if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
		{
			tempo = _tstoi(auxB);
		}

		for (; i < (*curAero); i++)
		{
			if (_tcscmp((*map)[i].aeroName, partida) == 0)
			{
				p = 1;

				break;
			}
		}

		for (int k = 0; k < (*curAero); k++)
		{
			if (_tcscmp((*map)[k].aeroName, chegada) == 0)
			{
				c = 1;

				break;
			}
		}

		if (p && c)
		{
			_stprintf_s(pass->partida, TAM, TEXT("%s"), partida);

			_stprintf_s(pass->destino, TAM, TEXT("%s"), chegada);

			_stprintf_s(pass->nome, TAM, TEXT("%s"), nome);

			(*curPass)++;

			_stprintf_s((*map)[i].passageiros[(*map)[i].curPass].nome, TAM, TEXT("%s"), nome);
			_stprintf_s((*map)[i].passageiros[(*map)[i].curPass].partida, TAM, TEXT("%s"), partida);
			_stprintf_s((*map)[i].passageiros[(*map)[i].curPass].destino, TAM, TEXT("%s"), chegada);
			_stprintf_s((*map)[i].passageiros[(*map)[i].curPass].msg, TAM, TEXT("%s"), TEXT(""));
			_stprintf_s((*map)[i].passageiros[(*map)[i].curPass].resp, TAM, TEXT("%s"), TEXT(""));

			(*map)[i].passageiros[(*map)[i].curPass].Comns = pass->Comns;

			(*map)[i].passageiros[(*map)[i].curPass].voar = 0;
			(*map)[i].passageiros[(*map)[i].curPass].tempo = tempo;

			(*map)[i].curPass++;

			return 1;
		}
	}

	return 0;
}


DWORD WINAPI tratamentoDeComunicacao(LPVOID lpParam) {
	ControlData * pDataArray;
	pDataArray = (ControlData*)lpParam;

	HANDLE hThread[MAX_PASS_CONTROL];
	DWORD dwThread[MAX_PASS_CONTROL];
	PassagComsData pass[MAX_PASS_CONTROL];

	for (int i = 0; i < MAX_PASS_CONTROL; i++)
	{
		(*pDataArray).Pass[i].Comns = CreateNamedPipe(PASSAG_PIPE,
			PIPE_ACCESS_DUPLEX |     // read/write access
			FILE_FLAG_OVERLAPPED,
			PIPE_WAIT | PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
			MAX_PASS_CONTROL,
			sizeof(TCHAR) * TAM,
			sizeof(TCHAR) * TAM,
			1000,
			NULL);

		if ((*pDataArray).Pass[i].Comns == INVALID_HANDLE_VALUE) {
			_tprintf(TEXT("[ERRO] Criar Named Pipe! (CreateNamedPipe)"));

			return -1;
		}
	}

	for (int i = 0; i < MAX_PASS_CONTROL; i++)
	{
		pass[i].Pass = &(*pDataArray).Pass[i];

		pass[i].curAero = &(*pDataArray->shared).curAero;

		for (int j = 0; j < MAX_AERO; j++)
		{
			pass[i].map[j] = &(*pDataArray->shared).map[j];
		}

		pass[i].curPass = &(*pDataArray).curPass;

		pass[i].ative = 0;
	}

	//#####Lançamento ComsManager's#####//

	for (int i = 0; i < MAX_PASS_CONTROL; i++)
	{
		hThread[i] = CreateThread(
			NULL,
			0,
			ComsManager,
			(LPVOID)&pass[i],
			0,
			&dwThread[i]
		);

		if (hThread[i] == NULL)
		{
			_tprintf(TEXT("CreateThread for Pipe failed, GLE=%d.\n"), GetLastError());

			return -2;
		}
	}

	//#####------------------------#####//

	WaitForMultipleObjects(MAX_PASS_CONTROL, hThread, TRUE, INFINITE);

	for (int i = 0; i < MAX_PASS_CONTROL; i++)
	{
		CloseHandle(hThread[i]);
	}

	return 0;
}


DWORD WINAPI ComsManager(LPVOID lpParam) {
	PassagComsData * pass;
	pass = (PassagComsData*)lpParam;

	BOOL ret;
	DWORD n;

	while (1) {
		_tprintf(TEXT("[CONTROL] Esperar ligação de um Passag (ConnectNamedPipe)\n"));

		if (!ConnectNamedPipe(pass->Pass->Comns, NULL)) {
			_tprintf(TEXT("[ERRO] Ligação a um Passag! (ConnectNamedPipe\n"));

			return -3;
		}
		do {
			_stprintf_s(pass->Pass->resp, TAM, TEXT("%s"), TEXT(""));

			ret = ReadFile(pass->Pass->Comns, pass->Pass->msg, sizeof(pass->Pass->msg), &n, NULL);

			pass->Pass->msg[n / sizeof(TCHAR)] = '\0';

			_tprintf(TEXT("[CONTROL] Recebi %d bytes: '%s'... (ReadFile)\n"), n, pass->Pass->msg);

			if (_tcscmp(pass->Pass->msg, TEXT("EXIT")) == 0)
			{
				//###Retirar Passag do Aeroporto###//

				if (deletePassagAero(pass, pass->Pass->partida))
				{
					_tprintf(TEXT("\nPassag %s saiu do Sistema Control.\n"), pass->Pass->nome);
				}
				else
				{
					_tprintf(TEXT("\n[ERROR] Passag %s provocou um erro ao tentar sair do Sistema Control.\n"), pass->Pass->nome);
				}

				//###---------------------------###//

				_stprintf_s(pass->Pass->destino, TAM, TEXT("%s"), TEXT(""));
				_stprintf_s(pass->Pass->partida, TAM, TEXT("%s"), TEXT(""));
				_stprintf_s(pass->Pass->nome, TAM, TEXT("%s"), TEXT(""));
				_stprintf_s(pass->Pass->resp, TAM, TEXT("%s"), TEXT(""));
				_stprintf_s(pass->Pass->msg, TAM, TEXT("%s"), TEXT(""));
				pass->Pass->tempo = 0;
				pass->Pass->voar = 0;

				(*pass->curPass)--;

				break;
			}

			if (pass->ative == 0)
			{
				if (!veryPassagEntry(pass->map, pass->curAero, pass->Pass, pass->curPass, pass->Pass->msg))
				{
					_stprintf_s(pass->Pass->resp, TAM, TEXT("%s"), TEXT("NO_ENTER"));
				}
				else
				{
					_stprintf_s(pass->Pass->resp, TAM, TEXT("%s"), TEXT("YES_ENTER"));

					pass->ative = 1;
				}
			}

			if (!WriteFile(pass->Pass->Comns, pass->Pass->resp, _tcslen(pass->Pass->resp) * sizeof(TCHAR), &n, NULL)) {
				_tprintf(TEXT("[ERRO] Escrever no pipe! (WriteFile)\n"));

				break;
			}

			FlushFileBuffers(pass->Pass->Comns);

			_tprintf(TEXT("[CONTROL] Enviei %d bytes ao Passag (WriteFile)\n"), n);

			//Parte de cima colocar lá fora e implementar uma thread para leitura com um sinal criado para Passag sinalizar que pretende escrever algo a nós

		} while (_tcscmp(pass->Pass->resp, TEXT("NO_ENTER")) != 0 && _tcscmp(pass->Pass->msg, TEXT("EXIT")) != 0);

		_tprintf(TEXT("[CONTROL] Desligar o pipe (DisconnectNamedPipe)\n"));

		pass->ative = 0;

		_stprintf_s(pass->Pass->msg, TAM, TEXT("%s"), TEXT(""));

		_stprintf_s(pass->Pass->resp, TAM, TEXT("%s"), TEXT(""));

		if (!DisconnectNamedPipe(pass->Pass->Comns)) {
			_tprintf(TEXT("[ERRO] Desligar o pipe! (DisconnectNamedPipe)"));

			return -4;
		}
	}

	return 0;
}


int deletePlaneFlying(ControlData* control, int pos) {
	DWORD n;

	//#######Aviso de Acidente aos Passag#######//

	for (int i = 0; i < (control->shared->planes)[pos].curPass; i++)
	{
		_stprintf_s((control->shared->planes)[pos].pass[i].msg, TAM, TEXT("%s"), TEXT(""));

		_stprintf_s((control->shared->planes)[pos].pass[i].resp, TAM, TEXT("%s"), TEXT("EXIT"));

		if (!WriteFile((control->shared->planes)[pos].pass[i].Comns, (control->shared->planes)[pos].pass[i].resp, _tcslen((control->shared->planes)[pos].pass[i].resp) * sizeof(TCHAR), &n, NULL)) {
			_tprintf(TEXT("[ERRO] Escrever no pipe! (WriteFile)\n"));

			break;
		}

		FlushFileBuffers((control->shared->planes)[pos].pass[i].Comns);

		_stprintf_s((control->shared->planes)[pos].pass[i].resp, TAM, TEXT("%s"), TEXT(""));
	}

	//#######----------------------------#######//

	if (control->shared->curPlane == 1)
	{
		_stprintf_s(control->shared->planes[pos].destino, TAM, TEXT("%s"), TEXT(""));
		_stprintf_s(control->shared->planes[pos].partida, TAM, TEXT("%s"), TEXT(""));

		ZeroMemory(control->shared->planes[pos].pass, sizeof(Passag) * control->maxPass);

		control->shared->planes[pos].curPass = 0;
		control->shared->planes[pos].id = 0;
		control->shared->planes[pos].maxPass = 0;
		control->shared->planes[pos].next_X = 0;
		control->shared->planes[pos].next_Y = 0;
		control->shared->planes[pos].final_X = 0;
		control->shared->planes[pos].final_Y = 0;
		control->shared->planes[pos].velocidade = 0;
		control->shared->planes[pos].X = 0;
		control->shared->planes[pos].Y = 0;
		control->shared->planes[pos].voar = 0;

		control->shared->curPlane--;

		return 1;
	}
	else
	{
		int j = pos;

		for (; j < (control->shared->curPlane - 1); j++)
		{
			control->shared->planes[j] = control->shared->planes[j + 1];
		}

		_stprintf_s(control->shared->planes[j + 1].destino, TAM, TEXT("%s"), TEXT(""));
		_stprintf_s(control->shared->planes[j + 1].partida, TAM, TEXT("%s"), TEXT(""));

		ZeroMemory(control->shared->planes[j + 1].pass, sizeof(Passag) * control->maxPass);

		control->shared->planes[j + 1].curPass = 0;
		control->shared->planes[j + 1].id = 0;
		control->shared->planes[j + 1].maxPass = 0;
		control->shared->planes[j + 1].next_X = 0;
		control->shared->planes[j + 1].next_Y = 0;
		control->shared->planes[j + 1].final_X = 0;
		control->shared->planes[j + 1].final_Y = 0;
		control->shared->planes[j + 1].velocidade = 0;
		control->shared->planes[j + 1].X = 0;
		control->shared->planes[j + 1].Y = 0;
		control->shared->planes[j + 1].voar = 0;

		control->shared->curPlane--;

		return 1;
	}

	return 0;
}


int deletePassagAero(PassagComsData * pass, TCHAR * partida) {
	for (int i = 0; i < (*pass).curAero; i++)
	{
		if (_tcscmp((pass->map)[i]->aeroName, partida) == 0)
		{
			if ((*(pass->map)[i]).curPass == 1)
			{
				_stprintf_s((pass->map)[i]->passageiros[0].destino, TAM, TEXT("%s"), TEXT(""));
				_stprintf_s((pass->map)[i]->passageiros[0].partida, TAM, TEXT("%s"), TEXT(""));
				_stprintf_s((pass->map)[i]->passageiros[0].resp, TAM, TEXT("%s"), TEXT(""));
				_stprintf_s((pass->map)[i]->passageiros[0].msg, TAM, TEXT("%s"), TEXT(""));
				_stprintf_s((pass->map)[i]->passageiros[0].nome, TAM, TEXT("%s"), TEXT(""));
				(pass->map)[i]->passageiros[0].tempo = 0;
				(pass->map)[i]->passageiros[0].voar = 0;

				pass->map[i]->curPass--;

				return 1;
			}
			else
			{
				int pos = 0, k;

				for (; pos < (*(pass->map)[i]).curPass; pos++)
				{
					if (_tcscmp(((pass->map)[i]->passageiros)[pos].nome, pass->Pass->nome) == 0)
					{
						break;
					}
				}

				for (k = pos; k < (*(pass->map)[i]).curPass - 1; k++)
				{
					_stprintf_s((pass->map)[i]->passageiros[k].destino, TAM, TEXT("%s"), (pass->map)[i]->passageiros[k + 1].destino);
					_stprintf_s((pass->map)[i]->passageiros[k].partida, TAM, TEXT("%s"), (pass->map)[i]->passageiros[k + 1].partida);
					_stprintf_s((pass->map)[i]->passageiros[k].resp, TAM, TEXT("%s"), (pass->map)[i]->passageiros[k + 1].resp);
					_stprintf_s((pass->map)[i]->passageiros[k].msg, TAM, TEXT("%s"), (pass->map)[i]->passageiros[k + 1].msg);
					_stprintf_s((pass->map)[i]->passageiros[k].nome, TAM, TEXT("%s"), (pass->map)[i]->passageiros[k + 1].nome);
					(pass->map)[i]->passageiros[k].tempo = (pass->map)[i]->passageiros[k + 1].tempo;
					(pass->map)[i]->passageiros[k].voar = (pass->map)[i]->passageiros[k + 1].voar;
				}

				_stprintf_s((pass->map)[i]->passageiros[k + 1].destino, TAM, TEXT("%s"), TEXT(""));
				_stprintf_s((pass->map)[i]->passageiros[k + 1].partida, TAM, TEXT("%s"), TEXT(""));
				_stprintf_s((pass->map)[i]->passageiros[k + 1].resp, TAM, TEXT("%s"), TEXT(""));
				_stprintf_s((pass->map)[i]->passageiros[k + 1].msg, TAM, TEXT("%s"), TEXT(""));
				_stprintf_s((pass->map)[i]->passageiros[k + 1].nome, TAM, TEXT("%s"), TEXT(""));
				(pass->map)[i]->passageiros[k + 1].tempo = 0;
				(pass->map)[i]->passageiros[k + 1].voar = 0;

				pass->map[i]->curPass--;

				return 1;
			}
		}
	}

	return 0;
}
