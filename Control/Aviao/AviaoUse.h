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

//Vari�veis Futuras Mem�ria//
#define MAX_THREADS 20
#define BUF_SIZE 256
//-------------------------//

//Vari�veis da Sincroniza��o//
#define CONTROL_SEMAPHORE_BUFFER_READER TEXT("ReadCircularBuffer")
#define PLANE_MUTEX_WRITER TEXT("WriteCircularBuffer")
#define KILLER_TRIGGER TEXT("KillAllSystems")
#define PLANE_MOVE_SYNC TEXT("MoveSync")
#define CONTROL_MUTEX_ENTRY TEXT("MutexEntry")
#define CONTROL_SEMAPHORE_ENTRY TEXT("PlaneGate")
//--------------------------//

//Vari�veis de Input//
#define TAM 100
#define TAM_INPUT 500
#define TAM_BUFF 5
//------------------//

//Vari�veis do Registry//
#define REG_SETTINGS_KEY TEXT("Software\\CONTROL\\SETTINGS")
#define MAX_PASS 5
#define MAX_VELO 3
#define SETTINGS_AERO TEXT("maxAero")
#define MAX_AERO 5
#define MAX_PLANES 5
//---------------------//

//Vari�veis da Sincroniza��o//
#define CONTROL_MUTEX TEXT("Nome")
#define CONTROL_SEMAPHORE_ENTRY TEXT("PlaneGate")
//--------------------------//

//Estrutura passageiros//
typedef struct
{
	int tempo;						// Tempo de espera / ao fim deste tempo o passageiro vai se embora
	int voar;						// Estado atual (em espera / em viagem)
	TCHAR nome[TAM];				// Nome do passageiro
	TCHAR destino[TAM];				// Aeroporto de destino
	TCHAR partida[TAM];				// Aeroporto de partida
	HANDLE Comns;					// Pipe de Comunicação deste Passag
	TCHAR msg[TAM];					// Messagem para Passag
	TCHAR resp[TAM];				// Resposta de Passag
} Passag;
//---------------------//

//Estrutura Avi�o//
typedef struct
{
	int id;							//id do avi�o / processo do avi�o
	TCHAR destino[TAM];				//aeroporto de destino
	int next_X, next_Y;				//coordenadas / posi��o seguinte
	int final_X, final_Y;			//coordenadas / posi��o final
	TCHAR partida[TAM];				//aeroporto de partida
	int X, Y;						//coordenadas / posi��o atual
	Passag pass[MAX_PASS];			//passageiros
	int maxPass, curPass;			//m�ximo de passageiros e tamanho atual
	int velocidade;					//velocidade do avi�o
	int voar;						//estado de voo (0 - parado / 1 - em voo)
	TCHAR controlResponse[TAM_INPUT];//resposta do Control ao Aviao
} Plane;
//---------------//

//Estrutura da C�lula do Mapa//
typedef struct
{
	Plane hangar[MAX_PLANES];		//Avi�es dentro do Aeroporto
	int maxHang, curHang;			//m�ximo de avi�es no Hangar e tamanho atual
	Passag passageiros[MAX_PASS];	//Passageiros em espera no Aeroporto
	int maxPass, curPass;			//m�ximo de passageiros e tamanho atual
	TCHAR aeroName[TAM];			//Nome do Aeroporto
	int Y, X;						//Coordenadas do Aeroporto no Mapa
} MapUnit;
//---------------------------//

//Estruturas da Mem�ria Partilhada//
typedef struct {
	TCHAR msg[TAM_INPUT];
} CircularBuffer;

typedef struct {
	CircularBuffer buffer[TAM_BUFF];
	int locReadAtual, locWriteAtual, maxBuffer;
} TotalCircularBuffer;
//-------------------------------//

//Vari�veis Futuras Mem�ria//
#define TAM_SHARED sizeof(SharedBuffer)
#define BUF_CIRCULAR sizeof(TotalCircularBuffer)
#define MAP_TAM 1000
//-------------------------//

//Mem�ria Partilhada//
typedef struct
{
	Plane planes[MAX_PLANES];		//Avi�es e as estruturas de mensagens
	int maxPlane, curPlane;			//M�ximo de Avi�es e tamanho atual
	MapUnit map[MAX_AERO];			//Aeroportos
	int maxAero, curAero;			//M�ximo de Aeroportos e tamanho atual

	TotalCircularBuffer tCircBuffer;//Buffer circular de mensagens para ler
} SharedBuffer;
//------------------//

//Estruturas de Dados//
typedef struct
{
	int id;							//ID do avi�o / processo do avi�o
	TCHAR destino[TAM];				//Aeroporto de chegada
	int next_X, next_Y;				//Coordenadas / posi��o seguinte
	int final_X, final_Y;			//Coordenadas / posi��o final
	TCHAR partida[TAM];				//Aeroporto de partida
	int X, Y;						//Coordenadas / posi��o atual
	Passag pass[MAX_PASS];			//Passageiros
	int maxPass, curPass;			//M�ximo de passageiros e tamanho atual
	int velocidade;					//Velocidade do avi�o
	int voar;						//Estado de voo (0 - parado / 1 - em voo)
	HANDLE mutexMoveSync;			//Mutex de sincroniza��o de movimenta��o do Avi�o
	HANDLE eventMoveTrigger;		//Event para iniciar o ciclo de movimento do Avi�o
	HANDLE readBuffer;				//Sem�foro de leitura do Buffer Circular (do lado do Control)
	HANDLE writeBuffer;				//Mutex de escrita para o Buffer Circular
	SharedBuffer * buffer;			//Mem�ria Partilhada
} PlaneData;
//-------------------//

//Fun��o de Obten��o de inteiros
//Retorna:
//		Inteiro
int getIntInput();

//Fun��o de Obten��o de frases
//Recebe:
//		szMsg	-	Array de TCHAR
//		tam		-	Tamanho do Array de TCHAR
void getTCHARInput(TCHAR * szMsg, int tam);

//Fun��o de Verifica��o de introdu��o do Aeroporto de Partida
//Recebe:
//		aviao	-	Estrutura dos dados do programa
//Retorna:
//		1	-	O aeroporto definido inicialmente existe na mem�ria partilhada do Control
//		0	-	O aeroporto definido inicialmente n�o existe na mem�ria partilhada do Control
int aviaoAeroVerify(PlaneData aviao);

//Fun��o Tempor�ria de introdu��o dos dados do Aviao na Mem�ria Partilhada
//Recebe:
//		aviao	-	Ponteiro da Estrutura dos dados do programa
//Retorna:
//		0		-	N�o foi poss�vel introduzir o avi�o / existe algum erro no Sistema do Control
//		1		-	Foi poss�vel introduzir o avi�o numa posi��o livre
int insertAviaoTemporary(PlaneData * aviao);

//Fun��o de apresenta��o da informa��o de um Passageiro
//Recebe:
//		plane	-	Estrutura de Plane com dados relacionados ao Avi�o
void listPlaneInfo(PlaneData plane);

//Thread de Tratamento de Comandos
//Recebe:
//		lpParam	-	Dados do PlaneData
DWORD WINAPI tratamentoDeComandos(LPVOID lpParam);

//Thread de Movimenta��o do Avi�o
//Recebe:
//		lpParam	-	Dados do PlaneData
DWORD WINAPI movePlane(LPVOID lpParam);

//Fun��o de Tratamento de Comandos do Aviao
//Recebe:
//		aviao	-	Dados do Aviao
//		comand	-	Comando introduzido pelo utilizador para Tratamento
//Retorna:
//		0	-	Comando sem espa�os
//		1	-	Comando foi tratado
int comandSwitcher(PlaneData * aviao, TCHAR * comand);

//Fun��o de Verifica��o de posi��o vaga no Mapa para voar
//Recebe:
//		aviao	-	Dados do Aviao
//Retorna:
//		0	-	Posi��o encontra-se ocupada por um avi�o
//		1	-	Posi��o est� livre para voar
int veryMapEmptyPlace(PlaneData * aviao);

//Fun��o de Escrita no Buffer Circular
//Recebe:
//		aviao	-	Dados do Aviao
//		msg		-	Mensagem a ser enviada
void writeInCircularBuffer(PlaneData * aviao, TCHAR * msg);

#endif
