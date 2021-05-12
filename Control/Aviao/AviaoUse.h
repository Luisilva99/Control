#ifndef AVIAOUSE_H
#define AVIAOUSE_H

#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <winreg.h>



//Vari�veis Futuras Mem�ria//
#define MAX_THREADS 20
#define BUF_SIZE 256
//-------------------------//

//Vari�veis da Sincroniza��o//
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
//---------------------//

//Vari�veis da Sincroniza��o//
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

//Estruturas da Mem�ria Partilhada//
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

//Mem�ria Partilhada//
typedef struct
{
	SharedMsg msg;					//Avi�es e as estruturas de mensagens
	TotalCircularBuffer tCircBuffer;//Buffer circular de mensagens para ler
} SharedBuffer;
//------------------//

//Vari�veis Futuras Mem�ria//
#define TAM_SHARED sizeof(SharedBuffer)
#define BUF_CIRCULAR sizeof(TotalCircularBuffer)
#define MAP_TAM 1000
//-------------------------//

//Estruturas de Dados//
typedef struct
{
	int id;							//id do avi�o / processo do avi�o
	TCHAR destino[TAM];				//aeroporto de chegada
	int next_X, next_Y;				//coordenadas / posi��o final
	TCHAR partida[TAM];				//aeroporto de partida
	int X, Y;						//coordenadas / posi��o atual
	Passag pass[MAX_PASS];			//passageiros
	int maxPass, curPass;			//m�ximo de passageiros e tamanho atual
	int velocidade;					//velocidade do avi�o
	//TCHAR controlResponse[TAM_INPUT];//resposta do Control ao Aviao
	SharedBuffer * buffer;			//mem�ria partilhada
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

//Fun��o de Leitura do M�ximo de Aeroportos
//Retorna:
//		Valor do m�ximo de Aeroportos a ser criados
//Erros:
//		-1	-	Chave n�o existia e foi criada pela primeira vez	-	erro previsivel
//		-2	-	N�o foi poss�vel criar a chave	-	erro cr�tico
//		-3	-	Leitura de regedit falhada	-	erro cr�tico
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