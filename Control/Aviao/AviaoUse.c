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
		if (_tcscmp(aviao.partida, (aviao.buffer->map + i)->aeroName) == 0) {
			return 1;
		}
	}

	return 0;
}


int insertAviaoTemporary(PlaneData aviao) {
	int i = 0;

	for (; i < aviao.buffer->curPlane; i++)
	{
		if ((aviao.buffer->planes + i)->id == aviao.id)
		{
			_tprintf(TEXT("\nID do avião já existe!\n"));

			return 0;
		}
	}

	(aviao.buffer->planes + i)->id = aviao.id;
	(aviao.buffer->planes + i)->maxPass = aviao.maxPass;
	(aviao.buffer->planes + i)->velocidade = aviao.velocidade;

	for (int j = 0; j < aviao.buffer->curAero; j++)
	{
		if (_tcscmp(aviao.partida, (aviao.buffer->map + j)->aeroName) == 0) {
			_stprintf_s((aviao.buffer->planes + i)->partida, TAM, TEXT("%s"), aviao.partida);

			aviao.X = (aviao.buffer->map + j)->X;

			aviao.Y = (aviao.buffer->map + j)->Y;

			(aviao.buffer->planes + i)->X = aviao.X;

			(aviao.buffer->planes + i)->Y = aviao.Y;

			(aviao.buffer->map + j)->hangar[(aviao.buffer->map + j)->curHang].id = aviao.id;

			(aviao.buffer->map + j)->hangar[(aviao.buffer->map + j)->curHang].velocidade = aviao.velocidade;

			(aviao.buffer->map + j)->hangar[(aviao.buffer->map + j)->curHang].X = aviao.X;

			(aviao.buffer->map + j)->hangar[(aviao.buffer->map + j)->curHang].Y = aviao.Y;

			_stprintf_s((aviao.buffer->map + j)->hangar[(aviao.buffer->map + j)->curHang].partida, TAM, TEXT("%s"), aviao.partida);

			return 1;
		}
	}


	return 0;
}
