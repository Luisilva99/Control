#ifndef CONTROLUSE_H
#define CONTROLUSE_H

#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <winreg.h>

//Variáveis de Input//
#define TAM 100
#define TAM_INPUT 500
//------------------//

//Variáveis Futuras Memória//
#define MAX_THREADS 20
#define BUF_SIZE 256
#define MAP_TAM 1000
//-------------------------//

//Variáveis do Registry//
#define REG_SETTINGS_KEY TEXT("Software\\CONTROL\\SETTINGS")
#define SETTINGS_PLANE TEXT("maxPlane")
#define MAX_PLANES 5
#define SETTINGS_AERO TEXT("maxAero")
#define MAX_AERO 5
//---------------------//

//Variáveis da Sincronização//
#define CONTROL_MUTEX TEXT("Nome")
#define CONTROL_SEMAPHORE_ENTRY TEXT("PlaneGate")
//--------------------------//

//Estrutura Avião//
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

//Estrutura da Célula do Mapa//
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

//Função de Obtenção de inteiros
//Retorna:
//		Inteiro
int getIntInput();

//Função de Obtenção de frases
//Retorna:
//		Array de TCHAR
TCHAR *getTCHARInput();

//Função de Leitura do Máximo de Aeroportos
//Retorna:
//		Valor do máximo de Aeroportos a ser criados
//Erros:
//		-1	-	Chave não existia e foi criada pela primeira vez	-	erro previsivel
//		-2	-	Não foi possível criar a chave	-	erro crítico
//		-3	-	Leitura de regedit falhada	-	erro crítico
int readAeroLimits();

//Função de Leitura do Máximo de Aviões
//Retorna:
//		Valor do máximo de Aviões aceites
//Erros:
//		-1	-	Chave não existia e foi criada pela primeira vez	-	erro previsivel
//		-2	-	Não foi possível criar a chave	-	erro crítico
//		-3	-	Leitura de regedit falhada	-	erro crítico
int readPlaneLimits();

//Função de Definição do Máximo de Aeroportos
//Recebe:
//		valor	-	número máximo de aeroportos a atualizar
//Retorna:
//		0	-	Valor de sucesso
//Erros:
//		-1	-	Chave não existe	-	erro crítico
//		-2	-	Erro ao adicionar o limite	-	erro crítico
//		-3	-	Erro ao atualizar o limite	-	erro crítico
int createAeroLimits(int valor);

//Função de Definição do Máximo de Aviões
//Recebe:
//		valor	-	número máximo de aviões a atualizar
//Retorna:
//		0	-	Valor de sucesso
//Erros:
//		-1	-	Chave não existe	-	erro crítico
//		-2	-	Erro ao adicionar o limite	-	erro crítico
//		-3	-	Erro ao atualizar o limite	-	erro crítico
int createPlaneLimits(int valor);

//Thread de Tratamento de Comandos
//Recebe:
//		lpParam	-	Dados do Control
DWORD WINAPI tratamentoDeComandos(LPVOID lpParam);

//Função de Tratamento de Comandos do Control
//Recebe:
//		control	-	Dados do Control
//		comand	-	Comando introduzido pelo utilizador para Tratamento
//Retorna:
//		0	-	Comando sem espaços -> temporário
//		1	-	Comando foi tratado
int comandSwitcher(ControlData * control, TCHAR * comand);

#endif