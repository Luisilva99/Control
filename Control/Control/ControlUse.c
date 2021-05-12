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
		_tprintf(TEXT("\nChegada / Atual: %s"), pass.destino);
		_tprintf(TEXT("\nPartida / Destino: %s"), pass.partida);
		_tprintf(TEXT("\nTempo de Espera: %d\n"), pass.tempo);
	}
}


void listPlaneInfo(Plane plane) {
	if (plane.id != 0)
	{
		_tprintf(TEXT("\nID: %d"), plane.id);
		_tprintf(TEXT("\nChegada / Atual: %s"), plane.destino);
		_tprintf(TEXT("\nPartida / Destino: %s"), plane.partida);
		_tprintf(TEXT("\nVelocidade: %d"), plane.velocidade);
		_tprintf(TEXT("\nPosição atual: [X=%d,Y=%d]\n"), plane.X, plane.Y);
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
				_tprintf(TEXT("\nAviões são permitidos entrar no Sistema.\n"));
				return 1;
			}
			else if (_tcscmp(auxB, TEXT("stop")) == 0)
			{
				_tprintf(TEXT("\nAviões não são permitidos entrar no Sistema.\n"));
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
		if (control->curAero < control->maxAero)
		{
			if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
			{
				int index = 0, X = -1, Y = -1;
				TCHAR nome[TAM_INPUT];

				for (; index < control->maxAero; index++)
				{
					if (_tcscmp((control->map + index)->aeroName, auxB) == 0)
					{
						_tprintf(TEXT("\nJá existe um Aeroporto com esse nome.\n"));

						return 0;
					}
					else if (_tcscmp((control->map + index)->aeroName, TEXT("")) == 0)
					{
						_tprintf(TEXT("\nEncontrei um espaço vazio.\nNome válido.\n"));//DEBUG

						_stprintf_s(nome, TAM_INPUT, TEXT("%s"), auxB);

						break;
					}
				}

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
					_tprintf(TEXT("\nCoordenadas inválidas!\n"));//DEBUG

					return 0;
				}

				for (int i = 0; i < control->curAero; i++)
				{
					if (((control->map + i)->X == X) && ((control->map + i)->Y == Y))
					{
						_tprintf(TEXT("\nCoordenadas ocupadas!\n"));//DEBUG

						return 0;
					}
				}

				_stprintf_s((control->map + index)->aeroName, TAM_INPUT, TEXT("%s"), nome);

				(control->map + index)->X = X;

				(control->map + index)->Y = Y;

				control->curAero += 1;

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
		if (control->curAero <= 0)
		{
			_tprintf(TEXT("\nNão existem Aeroportos no Mapa.\n"));

			return 0;
		}

		for (int i = 0; i < control->curAero; i++)
		{
			_tprintf(TEXT("\n\nAeroporto %s\n"), (control->map + i)->aeroName);
			_tprintf(TEXT("\nPassageiros dentro do Aeroporto:\n"));
			for (int j = 0; j < MAX_PASS; j++)
			{
				listPassInfo((control->map + i)->passageiros[j]);
			}
			_tprintf(TEXT("\nAviões dentro do Aeroporto:\n"));
			for (int j = 0; j < MAX_PASS; j++)
			{
				listPlaneInfo((control->map + i)->hangar[j]);
			}
			_tprintf(TEXT("\nPosição do Aeroporto: [X\t=\t%d,Y\t=\t%d]\n\n"), (control->map + i)->X, (control->map + i)->Y);
		}

		return 1;
	}
	else if (_tcscmp(auxB, TEXT("DEBUG")) == 0)
	{
		if ((auxB = _tcstok_s(NULL, TEXT(" "), &auxA)) != NULL)
		{
			if (_tcscmp(auxB, TEXT("MSG")) == 0) {
				_stprintf_s(control->shared->msg.controlResponse, TAM_INPUT, TEXT("%s"), auxA);

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

