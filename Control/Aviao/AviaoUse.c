#include "AviaoUse.h"




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
		_tprintf(TEXT("\nA chave n�o existe, vai ser criada.\n"));

		LONG Create_Key = RegCreateKeyExW(HKEY_CURRENT_USER, REG_SETTINGS_KEY, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE | KEY_SET_VALUE | KEY_QUERY_VALUE, NULL, &chave, NULL);

		if (Create_Key == ERROR_SUCCESS)
		{
			_tprintf(TEXT("\nChave foi criada com sucesso!"));

			return -1;
		}
		else
		{
			_tprintf(TEXT("\nChave n�o foi poss�vel ser criada!"));

			return -2;
		}
	}

	tam_data = sizeof(par_valor);

	test_existance = RegQueryValueEx(chave, SETTINGS_AERO, 0, NULL, (LPBYTE)par_valor, &tam_data);

	if (test_existance == ERROR_SUCCESS)
	{
		_tprintf(TEXT("\nLeitura no regedit bem sucedida!\n"));
		//_tprintf(TEXT("\nM�ximo de Aeroportos permitidos criar: %s\n"), par_valor);
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
		_tprintf(TEXT("\nA chave n�o existe, vai ser criada.\n"));

		LONG Create_Key = RegCreateKeyExW(HKEY_CURRENT_USER, REG_SETTINGS_KEY, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE | KEY_SET_VALUE | KEY_QUERY_VALUE, NULL, &chave, NULL);

		if (Create_Key == ERROR_SUCCESS)
		{
			_tprintf(TEXT("\nChave foi criada com sucesso!"));

			return -1;
		}
		else
		{
			_tprintf(TEXT("\nChave n�o foi poss�vel ser criada!"));

			return -2;
		}
	}

	tam_data = sizeof(par_valor);

	test_existance = RegQueryValueEx(chave, SETTINGS_PLANE, 0, NULL, (LPBYTE)par_valor, &tam_data);

	if (test_existance == ERROR_SUCCESS)
	{
		_tprintf(TEXT("\nLeitura no regedit bem sucedida!\n"));
		//_tprintf(TEXT("\nM�ximo de Avi�es permitidos no sistema: %s\n"), par_valor);
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
		_tprintf(TEXT("\nErro ao abrir a chave / n�o existe esta chave!\n"));

		return -1;
	}

	tam_data = _tcslen(saveValor) + 1;

	test_existance = RegQueryValueEx(chave, SETTINGS_AERO, 0, NULL, (LPBYTE)valor, &tam_data);

	if (test_existance == ERROR_FILE_NOT_FOUND)
	{
		_tprintf(TEXT("\nLimite n�o existe, vai ser criado com este valor.\n"));

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
		_tprintf(TEXT("\nJ� existe este limite vou atualizar.\n"));

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
		_tprintf(TEXT("\nErro ao abrir a chave / n�o existe esta chave!\n"));

		return -1;
	}

	tam_data = _tcslen(saveValor) + 1;

	test_existance = RegQueryValueEx(chave, SETTINGS_PLANE, 0, NULL, (LPBYTE)valor, &tam_data);

	if (test_existance == ERROR_FILE_NOT_FOUND)
	{
		_tprintf(TEXT("\nLimite n�o existe, vai ser criado com este valor.\n"));

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
		_tprintf(TEXT("\nJ� existe este limite vou atualizar.\n"));

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
