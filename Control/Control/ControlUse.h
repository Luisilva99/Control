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
} Plane;
//---------------//

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
//Retorna: Inteiro
int getIntInput();

//Função de Obtenção de frases
//Retorna: Array de TCHAR
TCHAR *getTCHARInput();

//Função de Leitura do Máximo de Aeroportos
//Retorna: Valor do máximo de Aeroportos a ser criados
//Erros:
//		-1	-	Chave não existia e foi criada pela primeira vez	-	erro previsivel
//		-2	-	Não foi possível criar a chave	-	erro crítico
//		-3	-	Leitura de regedit falhada	-	erro crítico
int readAeroLimits();

//Função de Leitura do Máximo de Aviões
//Retorna: Valor do máximo de Aviões aceites
//Erros:
//		-1	-	Chave não existia e foi criada pela primeira vez	-	erro previsivel
//		-2	-	Não foi possível criar a chave	-	erro crítico
//		-3	-	Leitura de regedit falhada	-	erro crítico
int readPlaneLimits();

//Função de Definição do Máximo de Aeroportos
//Retorna: 0	-	Valor de sucesso
//Erros:
//		-1	-	Chave não existe	-	erro crítico
//		-2	-	Erro ao adicionar o limite	-	erro crítico
//		-3	-	Erro ao atualizar o limite	-	erro crítico
int createAeroLimits(int valor);

//Função de Definição do Máximo de Aviões
//Retorna: 0	-	Valor de sucesso
//Erros:
//		-1	-	Chave não existe	-	erro crítico
//		-2	-	Erro ao adicionar o limite	-	erro crítico
//		-3	-	Erro ao atualizar o limite	-	erro crítico
int createPlaneLimits(int valor);

//Thread de Tratamento de Comandos
//Recebe:	ControlData
DWORD WINAPI tratamentoDeComandos(LPVOID lpParam);

#endif