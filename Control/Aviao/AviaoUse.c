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


