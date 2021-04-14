#pragma once

#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>


#define TAM 100
#define TAM_INPUT 500
#define MAX_THREADS 20
#define BUF_SIZE 256

//Fun��o de Obten��o de inteiros
int getIntInput() {
	TCHAR valor[TAM];

	fflush(stdin);
	_fgetts(valor, TAM, stdin);

	valor[_tcslen(valor) - 1] = TEXT('\0');

	// usar este para juntar strings com valores
	//_stprintf_s(chave_nome, TAM, TEXT("Software\\AULA\\%s"), par_nome);

	return _tstoi(valor);
}

//Fun��o de Obten��o de frases
TCHAR *getTCHARInput() {
	TCHAR szMsg[TAM_INPUT];

	fflush(stdin);
	_fgetts(szMsg, TAM_INPUT, stdin);

	szMsg[_tcslen(szMsg) - 1] = TEXT('\0');

	return szMsg;
}

//Fun��o de Inicializa��o da Mem�ria Partilhada
//Recebe: Nome da mem�ria partilhada, Tamanho da mem�ria partilhada
//Retorna: Ponteiro da Estrutura Partilhada
LPTSTR startMemory(HANDLE * create, TCHAR * memoryName, DWORD memorySize) {
	LPTSTR pBuf;

	create = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		memorySize,                // maximum object size (low-order DWORD)
		memoryName);                 // name of mapping object


	if (create == NULL)
	{
		_tprintf(TEXT("Could not create file mapping object (%d).\n"),
			GetLastError());
		return NULL;
	}
	else if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		_tprintf(TEXT("J� existe uma mem�ria partilhada com este nome, vou abrir!.\n"));
	}

	pBuf = (LPTSTR)MapViewOfFile(create,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		memorySize);

	if (pBuf == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"),
			GetLastError());

		CloseHandle(create);

		return NULL;
	}

	return pBuf;
}