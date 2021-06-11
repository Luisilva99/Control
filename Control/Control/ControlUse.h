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
#define POS_AERO_AREA 10
#define TAM 100
#define TAM_INPUT 500
#define TAM_BUFF 5
//------------------//

//Variáveis do Registry//
#define REG_SETTINGS_KEY TEXT("Software\\CONTROL\\SETTINGS")
#define SETTINGS_PLANE TEXT("maxPlane")
#define MAX_PLANES 5
#define SETTINGS_AERO TEXT("maxAero")
#define MAX_AERO 5
#define MAX_PASS 2
#define MAX_PASS_CONTROL MAX_PASS
//---------------------//

//Variáveis da Sincronização//
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
	HANDLE Comns;					// Pipe de Comunicação deste Passag
	TCHAR msg[TAM];					// Messagem para Passag
	TCHAR resp[TAM];				// Resposta de Passag
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

//Variáveis Futuras Memória//
#define BUF_MAP sizeof(MapUnit)
#define BUF_PLANE sizeof(Plane)
#define BUF_CIRCULAR sizeof(TotalCircularBuffer)
#define MAP_TAM 1000
//-------------------------//

//Estrutura dos Dados do Control//
typedef struct {
	Passag Pass[MAX_PASS_CONTROL];	//Passageiros
	int maxPass, curPass;			//Máximo de Passageiros e tamanho atual
	HANDLE entry;					//Mutex de aceitação de novos Aviões
	int entryStopped;				//Flag de indicador de paragem de aceitação de novos Aviões
	HANDLE systemShutdown;			//Evento para terminar todos os programas
	HANDLE passagSender;			//Evento para aviso de escrita em todos os pipes dos Passag
	HANDLE readBuffer;				//Semáforo para leitura do Buffer Circular
	SharedBuffer * shared;			//Memória Partilhada com Aviao
} ControlData;
//------------------------------//

//Estrutura de Comunicação//
typedef struct {
	Passag * Pass;					//Passageiros
	MapUnit * map[MAX_AERO];		//Aeroportos
	int * curAero;					//Máximo de Aeroportos e tamanho atual
	int * curPass;					//Máximo de Passageiros e tamanho atual
} PassagComsData;
//------------------------//

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

//Função de apresentação da informação de um Passageiro
//Recebe:
//		pass	-	Estrutura de Passag com dados relacionados ao Passageiro
void listPassInfo(Passag pass);

//Função de apresentação da informação de um Passageiro
//Recebe:
//		plane	-	Estrutura de Plane com dados relacionados ao Avião
void listPlaneInfo(Plane plane);

//Thread de Tratamento de Comandos
//Recebe:
//		lpParam	-	Dados do Control
DWORD WINAPI tratamentoDeComandos(LPVOID lpParam);

//Thread de Buffer Circular
//Recebe:
//		lpParam	-	Dados do Control
DWORD WINAPI bufferCircular(LPVOID lpParam);

//Thread de Lançamento de Comunicação por Pipe
//Recebe:
//		lpParam	-	Dados do Control
DWORD WINAPI tratamentoDeComunicacao(LPVOID lpParam);

//Thread de Tratamento de Comunicação por Pipe
//Recebe:
//		lpParam	-	Dados PassagComsData
DWORD WINAPI ComsManager(LPVOID lpParam);

//Função de Verificação dos dados de entrada dos Passag
//Recebe:
//		map		-	Lista de Aeroportos no Control
//		curAero	-	Número de Aeroportos existentes no Control
//		pass	-	Passageiro atual
//		curPass	-	Número de Passageiros existentes no Control
//		msg		-	Mensagem enviada pelo Passag para Tratamento de Entrada
//Retorna:
//		0	-	Erros na introdução de informações para entrada
//		1	-	Entrada autorizada
int veryPassagEntry(MapUnit * map[], int * curAero, Passag * pass, int * curPass, TCHAR * msg);

//Função de Tratamento de Comandos do Control
//Recebe:
//		control	-	Dados do Control
//		comand	-	Comando introduzido pelo utilizador para Tratamento
//Retorna:
//		0	-	Comando sem espaços
//		1	-	Comando foi tratado
int comandSwitcher(ControlData * control, TCHAR * comand);

//Função de Eliminação de Aviões na Lista dos Dados do Control
//Recebe:
//		control	-	Dados do Control
//		pos		-	Posição do Plane nos Dados do Control a eliminar
int deletePlaneFlying(ControlData* control, int pos);

#endif