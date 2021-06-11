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
#define POS_AERO_AREA 10
#define TAM 100
#define TAM_INPUT 500
#define TAM_BUFF 5
//------------------//

//Vari�veis do Registry//
#define REG_SETTINGS_KEY TEXT("Software\\CONTROL\\SETTINGS")
#define SETTINGS_PLANE TEXT("maxPlane")
#define MAX_PLANES 5
#define SETTINGS_AERO TEXT("maxAero")
#define MAX_AERO 5
#define MAX_PASS 2
#define MAX_PASS_CONTROL MAX_PASS
//---------------------//

//Vari�veis da Sincroniza��o//
#define CONTROL_SEMAPHORE_BUFFER_READER TEXT("ReadCircularBuffer")
#define PLANE_MUTEX_WRITER TEXT("WriteCircularBuffer")
#define KILLER_TRIGGER TEXT("KillAllSystems")
#define SENDER_TRIGGER TEXT("SendMsg")
#define PLANE_MOVE_SYNC TEXT("MoveSync")
#define CONTROL_MUTEX_ENTRY TEXT("MutexEntry")
#define CONTROL_SEMAPHORE_ENTRY TEXT("PlaneGate")
#define CONTROL_SEMAPHORE_PASSAG_ENTRY TEXT("PassagGate")
#define PASSAG_PIPE TEXT("\\\\.\\pipe\\PassagComs")
//--------------------------//

//Estrutura passageiros//
typedef struct
{
	int tempo;						// Tempo de espera / ao fim deste tempo o passageiro vai se embora
	int voar;						// Estado atual (em espera / em viagem)
	TCHAR nome[TAM];				// Nome do passageiro
	TCHAR destino[TAM];				// Aeroporto de destino
	TCHAR partida[TAM];				// Aeroporto de partida
	HANDLE Comns;					// Pipe de Comunica��o deste Passag
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

//Vari�veis Futuras Mem�ria//
#define BUF_MAP sizeof(MapUnit)
#define BUF_PLANE sizeof(Plane)
#define BUF_CIRCULAR sizeof(TotalCircularBuffer)
#define MAP_TAM 1000
//-------------------------//

//Estrutura dos Dados do Control//
typedef struct {
	Passag Pass[MAX_PASS_CONTROL];	//Passageiros
	int maxPass, curPass;			//M�ximo de Passageiros e tamanho atual
	HANDLE entry;					//Mutex de aceita��o de novos Avi�es
	int entryStopped;				//Flag de indicador de paragem de aceita��o de novos Avi�es
	HANDLE systemShutdown;			//Evento para terminar todos os programas
	HANDLE passagSender;			//Evento para aviso de escrita em todos os pipes dos Passag
	HANDLE readBuffer;				//Sem�foro para leitura do Buffer Circular
	SharedBuffer * shared;			//Mem�ria Partilhada com Aviao
} ControlData;
//------------------------------//

//Estrutura de Comunica��o//
typedef struct {
	Passag * Pass;					//Passageiros
	MapUnit * map[MAX_AERO];		//Aeroportos
	int * curAero;					//M�ximo de Aeroportos e tamanho atual
	int * curPass;					//M�ximo de Passageiros e tamanho atual
} PassagComsData;
//------------------------//

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

//Fun��o de apresenta��o da informa��o de um Passageiro
//Recebe:
//		pass	-	Estrutura de Passag com dados relacionados ao Passageiro
void listPassInfo(Passag pass);

//Fun��o de apresenta��o da informa��o de um Passageiro
//Recebe:
//		plane	-	Estrutura de Plane com dados relacionados ao Avi�o
void listPlaneInfo(Plane plane);

//Thread de Tratamento de Comandos
//Recebe:
//		lpParam	-	Dados do Control
DWORD WINAPI tratamentoDeComandos(LPVOID lpParam);

//Thread de Buffer Circular
//Recebe:
//		lpParam	-	Dados do Control
DWORD WINAPI bufferCircular(LPVOID lpParam);

//Thread de Lan�amento de Comunica��o por Pipe
//Recebe:
//		lpParam	-	Dados do Control
DWORD WINAPI tratamentoDeComunicacao(LPVOID lpParam);

//Thread de Tratamento de Comunica��o por Pipe
//Recebe:
//		lpParam	-	Dados PassagComsData
DWORD WINAPI ComsManager(LPVOID lpParam);

//Fun��o de Verifica��o dos dados de entrada dos Passag
//Recebe:
//		map		-	Lista de Aeroportos no Control
//		curAero	-	N�mero de Aeroportos existentes no Control
//		pass	-	Passageiro atual
//		curPass	-	N�mero de Passageiros existentes no Control
//		msg		-	Mensagem enviada pelo Passag para Tratamento de Entrada
//Retorna:
//		0	-	Erros na introdu��o de informa��es para entrada
//		1	-	Entrada autorizada
int veryPassagEntry(MapUnit * map[], int * curAero, Passag * pass, int * curPass, TCHAR * msg);

//Fun��o de Tratamento de Comandos do Control
//Recebe:
//		control	-	Dados do Control
//		comand	-	Comando introduzido pelo utilizador para Tratamento
//Retorna:
//		0	-	Comando sem espa�os
//		1	-	Comando foi tratado
int comandSwitcher(ControlData * control, TCHAR * comand);

//Fun��o de Elimina��o de Avi�es na Lista dos Dados do Control
//Recebe:
//		control	-	Dados do Control
//		pos		-	Posi��o do Plane nos Dados do Control a eliminar
int deletePlaneFlying(ControlData* control, int pos);

#endif