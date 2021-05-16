#include "ControlUse.h"

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
		//_tprintf(TEXT("\nMáximo de Aeroportos permitidos criar: %s\n"), par_valor);
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
		//_tprintf(TEXT("\nMáximo de Aviões permitidos no sistema: %s\n"), par_valor);
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
		_tprintf(TEXT("\n--------------------\nChegada / Atual: %s"), pass.destino);
		_tprintf(TEXT("\nPartida / Destino: %s"), pass.partida);
		_tprintf(TEXT("\nTempo de Espera: %d\n--------------------\n"), pass.tempo);
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
		_tprintf(TEXT("\nPosição atual: [X=%d,Y=%d]"), plane.X, plane.Y);
		if (plane.voar)
		{
			_tprintf(TEXT("\nPosição seguinte: [X=%d,Y=%d]"), plane.next_X, plane.next_Y);
		}
		!_tcslen(plane.destino) ? _tprintf(TEXT("")) : _tprintf(TEXT("\nPosição final: [X=%d,Y=%d]"), plane.final_X, plane.final_Y);
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
				_tprintf(TEXT("\nDEBUG MSG - [Temporário | Deprecated] Introduz uma mensagem no primeiro avião.\n"));
			}
		}

		_tprintf(TEXT("\ninPlane stop/start - Abrir e fechar a aceitação de novos aviões.\n"));
		_tprintf(TEXT("\ninPass stop/start - Abrir e fechar a aceitação de novos passageiros.\n"));
		_tprintf(TEXT("\ncreateAero Nome posX poxY - Criar um novo aeroporto.\n"));
		_tprintf(TEXT("\nlistAero - Apresenta a lista de todos os aeroportos registados.\n"));
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
			_tprintf(TEXT("\nNão é possível criar mais Aeroportos.\nLimite máximo alcançado.\n"));//DEBUG
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
			_tprintf(TEXT("\nPosição do Aeroporto: [X=%d,Y=%d]\n_________________________________\n"), (control->shared->map)[i].X, (control->shared->map)[i].Y);
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
	ControlData * pDataArray;
	pDataArray = (ControlData*)lpParam;

	TCHAR *comando = (TCHAR *)malloc(sizeof(TCHAR) * TAM_INPUT);

	_tprintf(TEXT("\nhelp - Lista de comandos.\n\n"));

	do
	{
		_tprintf(TEXT("\n> "));
		getTCHARInput(comando, TAM_INPUT);
		//DEBUG - pode ocorrer aqui erro uma vez que o "comando" é modificado diretamente - se alguém introduzir "exit ok" a variável modificada terá "exit"
		comandSwitcher(pDataArray, comando) ? _tprintf(TEXT("")) : _tprintf(TEXT("\nComando Incorreto.\n"));
	} while (_tcscmp(comando, TEXT("exit")));

	free(comando);

	return 0;
}


DWORD WINAPI bufferCircular(LPVOID lpParam)
{
	ControlData * pDataArray;
	pDataArray = (ControlData*)lpParam;



	return 0;
}


int deletePlane(ControlData* control, int id) {
	for (int i = 0; i < control->shared->curPlane; i++)
	{
		if (control->shared->planes[i].id == id)
		{
			if (control->shared->curPlane == i)
			{
				_stprintf_s(control->shared->planes[i].destino, TAM, TEXT("%s"), TEXT(""));
				_stprintf_s(control->shared->planes[i].partida, TAM, TEXT("%s"), TEXT(""));
				ZeroMemory(control->shared->planes[i].pass, sizeof(Passag) * MAX_PASS);
				control->shared->planes[i].curPass = 0;
				control->shared->planes[i].id = 0;
				control->shared->planes[i].maxPass = 0;
				control->shared->planes[i].next_X = 0;
				control->shared->planes[i].next_Y = 0;
				control->shared->planes[i].final_X = 0;
				control->shared->planes[i].final_Y = 0;
				control->shared->planes[i].velocidade = 0;
				control->shared->planes[i].X = 0;
				control->shared->planes[i].Y = 0;

				control->shared->curPlane -= 1;

				return 1;
			}
			else
			{
				int j;

				for (j = i; j < control->shared->curPlane; j++)
				{
					control->shared->planes[j] = control->shared->planes[j + 1];
				}

				_stprintf_s(control->shared->planes[j].destino, TAM, TEXT("%s"), TEXT(""));
				_stprintf_s(control->shared->planes[j].partida, TAM, TEXT("%s"), TEXT(""));
				ZeroMemory(control->shared->planes[j].pass, sizeof(Passag) * MAX_PASS);
				control->shared->planes[j].curPass = 0;
				control->shared->planes[j].id = 0;
				control->shared->planes[j].maxPass = 0;
				control->shared->planes[j].next_X = 0;
				control->shared->planes[j].next_Y = 0;
				control->shared->planes[j].final_X = 0;
				control->shared->planes[j].final_Y = 0;
				control->shared->planes[j].velocidade = 0;
				control->shared->planes[j].X = 0;
				control->shared->planes[j].Y = 0;

				control->shared->curPlane -= 1;

				return 1;
			}
		}
	}

	return 0;
}

