#ifndef AVIAOUSE_H
#define AVIAOUSE_H

#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <winreg.h>
#include "SO2_TP_DLL_2021.h"

//Variáveis Futuras Memória//
#define MAX_THREADS 20
#define BUF_SIZE 256
//-------------------------//

//Variáveis da Sincronização//
#define CONTROL_SEMAPHORE_BUFFER_READER TEXT("ReadCircularBuffer")
#define PLANE_MUTEX_WRITER TEXT("WriteCircularBuffer")
#define KILLER_TRIGGER TEXT("KillAllSystems")
#define PLANE_MOVE_SYNC TEXT("MoveSync")
#define CONTROL_MUTEX_ENTRY TEXT("MutexEntry")
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
//-------------------------------//

//Variáveis Futuras Memória//
#define TAM_SHARED sizeof(SharedBuffer)
#define BUF_CIRCULAR sizeof(TotalCircularBuffer)
#define MAP_TAM 1000
//-------------------------//

//Memória Partilhada//
typedef struct
{
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
	int id;							//ID do avião / processo do avião
	TCHAR destino[TAM];				//Aeroporto de chegada
	int next_X, next_Y;				//Coordenadas / posição seguinte
	int final_X, final_Y;			//Coordenadas / posição final
	TCHAR partida[TAM];				//Aeroporto de partida
	int X, Y;						//Coordenadas / posição atual
	Passag pass[MAX_PASS];			//Passageiros
	int maxPass, curPass;			//Máximo de passageiros e tamanho atual
	int velocidade;					//Velocidade do avião
	int voar;						//Estado de voo (0 - parado / 1 - em voo)
	HANDLE mutexMoveSync;			//Mutex de sincronização de movimentação do Avião
	HANDLE eventMoveTrigger;		//Event para iniciar o ciclo de movimento do Avião
	HANDLE readBuffer;				//Semáforo de leitura do Buffer Circular (do lado do Control)
	HANDLE writeBuffer;				//Mutex de escrita para o Buffer Circular
	SharedBuffer * buffer;			//Memória Partilhada
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
//		lpParam	-	Dados do PlaneData
DWORD WINAPI tratamentoDeComandos(LPVOID lpParam);

//Thread de Movimentação do Avião
//Recebe:
//		lpParam	-	Dados do PlaneData
DWORD WINAPI movePlane(LPVOID lpParam);

//Função de Tratamento de Comandos do Aviao
//Recebe:
//		aviao	-	Dados do Aviao
//		comand	-	Comando introduzido pelo utilizador para Tratamento
//Retorna:
//		0	-	Comando sem espaços -> temporário
//		1	-	Comando foi tratado
int comandSwitcher(PlaneData * aviao, TCHAR * comand);

//Função de Verificação de posição vaga no Mapa para voar
//Recebe:
//		aviao	-	Dados do Aviao
//Retorna:
//		0	-	Posição encontra-se ocupada por um avião
//		1	-	Posição está livre para voar
int veryMapEmptyPlace(PlaneData * aviao);

//Função de Escrita no Buffer Circular
//Recebe:
//		aviao	-	Dados do Aviao
//		msg		-	Mensagem a ser enviada
void writeInCircularBuffer(PlaneData * aviao, TCHAR * msg);

#endif