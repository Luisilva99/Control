#ifndef AVIAOUSE_H
#define AVIAOUSE_H

#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <winreg.h>



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

//Memória Partilhada//
typedef struct
{
	SharedMsg msg;					//Aviões e as estruturas de mensagens
	TotalCircularBuffer tCircBuffer;//Buffer circular de mensagens para ler
} SharedBuffer;
//------------------//

//Variáveis Futuras Memória//
#define TAM_SHARED sizeof(SharedBuffer)
#define BUF_CIRCULAR sizeof(TotalCircularBuffer)
#define MAP_TAM 1000
//-------------------------//

//Estruturas de Dados//
typedef struct
{
	int id;							//id do avião / processo do avião
	TCHAR destino[TAM];				//aeroporto de chegada
	int next_X, next_Y;				//coordenadas / posição final
	TCHAR partida[TAM];				//aeroporto de partida
	int X, Y;						//coordenadas / posição atual
	Passag pass[MAX_PASS];			//passageiros
	int maxPass, curPass;			//máximo de passageiros e tamanho atual
	int velocidade;					//velocidade do avião
	//TCHAR controlResponse[TAM_INPUT];//resposta do Control ao Aviao
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

//Função de Leitura do Máximo de Aeroportos
//Retorna:
//		Valor do máximo de Aeroportos a ser criados
//Erros:
//		-1	-	Chave não existia e foi criada pela primeira vez	-	erro previsivel
//		-2	-	Não foi possível criar a chave	-	erro crítico
//		-3	-	Leitura de regedit falhada	-	erro crítico
int readAeroLimits();





//Recebe:
//		control - dados do aviao
//Retorna:
//		0 - nomes iguais
//		1 - nomes diferentes
int verificaIDPlane(PlaneData* aviao, int tam);


//Recebe:
//		controlo - dados do aviao
//		tam	- tamanho do array de avioes
//		id - id do aviao a eliminar
int deletePlane(PlaneData* aviao, int tam, int id);

#endif