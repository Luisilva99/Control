#ifndef CONTROLUSE_H
#define CONTROLUSE_H

#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <winreg.h>

//Vari�veis de Input//
#define TAM 100
#define TAM_INPUT 500
//------------------//

//Vari�veis Futuras Mem�ria//
#define MAX_THREADS 20
#define BUF_SIZE 256
#define MAP_TAM 1000
//-------------------------//

//Vari�veis do Registry//
#define REG_SETTINGS_KEY TEXT("Software\\CONTROL\\SETTINGS")
#define SETTINGS_PLANE TEXT("maxPlane")
#define MAX_PLANES 5
#define SETTINGS_AERO TEXT("maxAero")
#define MAX_AERO 5
//---------------------//

//Vari�veis da Sincroniza��o//
#define CONTROL_MUTEX TEXT("Nome")
#define CONTROL_SEMAPHORE_ENTRY TEXT("PlaneGate")
//--------------------------//

//Estrutura Avi�o//
typedef struct
{
	int id;
	int x, y;	//coordenadas
	TCHAR a_chegada[TAM];	//aeroporto de chegada
	TCHAR a_partida[TAM];	//aeroporto de partida
} Plane;
//---------------//

//Estrutura do Controller

typedef struct
{
	

} Controller;

//
//Estrutura passageiros

typedef struct
{
	int tempo;	//tempo de espera
	TCHAR a_chegada[TAM];	//aeroporto de chegada
	TCHAR a_partida[TAM];	//aeroporto de partida

} Passag;

//Estrutura da C�lula do Mapa//
typedef struct
{
	int isAero;
	Plane seaPlane;
	Plane hangar[MAX_PLANES];
	TCHAR aeroName[TAM_INPUT];
	int Y;
	int X;
} MapUnit;
//---------------------------//

// Estrutura Template dos Dados do Control
typedef struct {
	MapUnit *map;
} ControlData;

//Fun��o de Obten��o de inteiros
//Retorna:
//		Inteiro
int getIntInput();

//Fun��o de Obten��o de frases
//Retorna:
//		Array de TCHAR
TCHAR *getTCHARInput();

//Fun��o de Leitura do M�ximo de Aeroportos
//Retorna:
//		Valor do m�ximo de Aeroportos a ser criados
//Erros:
//		-1	-	Chave n�o existia e foi criada pela primeira vez	-	erro previsivel
//		-2	-	N�o foi poss�vel criar a chave	-	erro cr�tico
//		-3	-	Leitura de regedit falhada	-	erro cr�tico
int readAeroLimits();

//Fun��o de Leitura do M�ximo de Avi�es
//Retorna:
//		Valor do m�ximo de Avi�es aceites
//Erros:
//		-1	-	Chave n�o existia e foi criada pela primeira vez	-	erro previsivel
//		-2	-	N�o foi poss�vel criar a chave	-	erro cr�tico
//		-3	-	Leitura de regedit falhada	-	erro cr�tico
int readPlaneLimits();

//Fun��o de Defini��o do M�ximo de Aeroportos
//Recebe:
//		valor	-	n�mero m�ximo de aeroportos a atualizar
//Retorna:
//		0	-	Valor de sucesso
//Erros:
//		-1	-	Chave n�o existe	-	erro cr�tico
//		-2	-	Erro ao adicionar o limite	-	erro cr�tico
//		-3	-	Erro ao atualizar o limite	-	erro cr�tico
int createAeroLimits(int valor);

//Fun��o de Defini��o do M�ximo de Avi�es
//Recebe:
//		valor	-	n�mero m�ximo de avi�es a atualizar
//Retorna:
//		0	-	Valor de sucesso
//Erros:
//		-1	-	Chave n�o existe	-	erro cr�tico
//		-2	-	Erro ao adicionar o limite	-	erro cr�tico
//		-3	-	Erro ao atualizar o limite	-	erro cr�tico
int createPlaneLimits(int valor);

//Thread de Tratamento de Comandos
//Recebe:
//		lpParam	-	Dados do Control
DWORD WINAPI tratamentoDeComandos(LPVOID lpParam);

//Fun��o de Tratamento de Comandos do Control
//Recebe:
//		control	-	Dados do Control
//		comand	-	Comando introduzido pelo utilizador para Tratamento
//Retorna:
//		0	-	Comando sem espa�os -> tempor�rio
//		1	-	Comando foi tratado
int comandSwitcher(ControlData * control, TCHAR * comand);

#endif