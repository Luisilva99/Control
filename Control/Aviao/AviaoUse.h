#ifndef AVIAOUSE_H
#define AVIAOUSE_H

#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <winreg.h>
#include "../Aviao/SO2_TP_DLL_2021.h"

//Variáveis Futuras Memória//
#define MAX_THREADS 20
#define BUF_SIZE 256
//-------------------------//

//Variáveis da Sincronização//
#define CONTROL_SEMAPHORE_ENTRY TEXT("PlaneGate")
//--------------------------//


//Variáveis de Input//
#define TAM 100
#define TAM_INPUT 500
#define TAM_BUFF 5
//------------------//

//Variáveis do Registry//
#define REG_SETTINGS_KEY TEXT("Software\\CONTROL\\SETTINGS")
#define MAX_PASS 5
#define MAX_VELO 3
#define SETTINGS_AERO TEXT("maxAero")
#define MAX_AERO 5
#define MAX_PLANES 5
//---------------------//

//Variáveis da Sincronização//
#define CONTROL_MUTEX TEXT("Nome")
#define CONTROL_SEMAPHORE_ENTRY TEXT("PlaneGate")
//--------------------------//

//Estrutura passageiros//
typedef struct
{
	int tempo;						//tempo de espera / ao fim deste tempo o passageiro vai se embora
	TCHAR destino[TAM];				//aeroporto de chegada
	TCHAR partida[TAM];				//aeroporto de partida
} Passag;
//---------------------//

//Estrutura Avião//
typedef struct
{
	int id;							//id do avião / processo do avião
	TCHAR destino[TAM];				//aeroporto de destino
	int next_X, next_Y;				//coordenadas / posição seguinte
	int final_X, final_Y;			//coordenadas / posição final
	TCHAR partida[TAM];				//aeroporto de partida
	int X, Y;						//coordenadas / posição atual
	Passag pass[MAX_PASS];			//passageiros
	int maxPass, curPass;			//máximo de passageiros e tamanho atual
	int velocidade;					//velocidade do avião
	int voar;						//estado de voo (0 - parado / 1 - em voo)
	TCHAR controlResponse[TAM_INPUT];//resposta do Control ao Aviao
} Plane;
//---------------//

//Estrutura da Célula do Mapa//
typedef struct
{
	Plane hangar[MAX_PLANES];		//Aviões dentro do Aeroporto
	int maxHang, curHang;			//máximo de aviões no Hangar e tamanho atual
	Passag passageiros[MAX_PASS];	//Passageiros em espera no Aeroporto
	int maxPass, curPass;			//máximo de passageiros e tamanho atual
	TCHAR aeroName[TAM];			//Nome do Aeroporto
	int Y, X;						//Coordenadas do Aeroporto no Mapa
} MapUnit;
//---------------------------//

//Estruturas da Memória Partilhada//
typedef struct {
	TCHAR msg[TAM_INPUT];
} CircularBuffer;

typedef struct {
	CircularBuffer buffer[TAM_BUFF];
	int locReadAtual, locWriteAtual, maxBuffer;
} TotalCircularBuffer;

typedef struct
{
	int id;							//ID do user a receber
	int X, Y;						//Coordenadas respondidas
	TCHAR controlResponse[TAM_INPUT];//resposta do Control ao Aviao
} SharedMsg;
//-------------------------------//

//Variáveis Futuras Memória//
#define TAM_SHARED sizeof(SharedBuffer)
#define BUF_CIRCULAR sizeof(TotalCircularBuffer)
#define MAP_TAM 1000
//-------------------------//

//Memória Partilhada//
typedef struct
{
	//SharedMsg msg;					//Aviões e as estruturas de mensagens

	Plane planes[MAX_PLANES];		//Aviões e as estruturas de mensagens
	int maxPlane, curPlane;			//Máximo de Aviões e tamanho atual
	MapUnit map[MAX_AERO];			//Aeroportos
	int maxAero, curAero;			//Máximo de Aeroportos e tamanho atual

	TotalCircularBuffer tCircBuffer;//Buffer circular de mensagens para ler
} SharedBuffer;
//------------------//

//Estruturas de Dados//
typedef struct
{
	int id;							//id do avião / processo do avião
	TCHAR destino[TAM];				//aeroporto de chegada
	int next_X, next_Y;				//coordenadas / posição seguinte
	int final_X, final_Y;			//coordenadas / posição final
	TCHAR partida[TAM];				//aeroporto de partida
	int X, Y;						//coordenadas / posição atual
	Passag pass[MAX_PASS];			//passageiros
	int maxPass, curPass;			//máximo de passageiros e tamanho atual
	int velocidade;					//velocidade do avião
	int voar;						//estado de voo (0 - parado / 1 - em voo)
	SharedBuffer * buffer;			//memória partilhada
} PlaneData;
//-------------------//

//Função de Obtenção de inteiros
//Retorna:
//		Inteiro
int getIntInput();

//Função de Obtenção de frases
//Recebe:
//		szMsg	-	Array de TCHAR
//		tam		-	Tamanho do Array de TCHAR
void getTCHARInput(TCHAR * szMsg, int tam);

//Função de Verificação de introdução do Aeroporto de Partida
//Recebe:
//		aviao	-	Estrutura dos dados do programa
//Retorna:
//		1	-	O aeroporto definido inicialmente existe na memória partilhada do Control
//		0	-	O aeroporto definido inicialmente não existe na memória partilhada do Control
int aviaoAeroVerify(PlaneData aviao);

//Função Temporária de introdução dos dados do Aviao na Memória Partilhada
//Recebe:
//		aviao	-	Ponteiro da Estrutura dos dados do programa
//Retorna:
//		0		-	Não foi possível introduzir o avião / existe algum erro no Sistema do Control
//		1		-	Foi possível introduzir o avião numa posição livre
int insertAviaoTemporary(PlaneData * aviao);

//Função de apresentação da informação de um Passageiro
//Recebe:
//		plane	-	Estrutura de Plane com dados relacionados ao Avião
void listPlaneInfo(PlaneData plane);

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
int comandSwitcher(PlaneData * , TCHAR * comand);

#endif