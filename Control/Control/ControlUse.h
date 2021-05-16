#ifndef CONTROLUSE_H
#define CONTROLUSE_H

#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <winreg.h>


//Dúvidas//
//	Q: Passageiros só podem entrar assim que os aeroportos estiverem definidos?
//
//	R: ------------------------------------------------------------------------------------------------------------------------------------------
//
//
//	Q: Os aeroportos podem ser criados a meio da execução (quando aviões já estão em funcionamento / passageiros já se encontram abordo dos aviões)?
//
//	R: ------------------------------------------------------------------------------------------------------------------------------------------
//
//
//	Q: Podemos fazer malloc em arrays para passar para a memória partilhada?
//
//	R: Tem a haver com as Views, uma memória partilhada pode ter várias Views começando com offsets diferentes. A resposta é avisada por um evento para o avião ler!
//     Temos de ter uma estrutura de resposta para os aviões ler a resposta e esta estrutura facilmente identificada para leitura dele.
//
//
//	Q: O que é consumido é as mensagens entre os Aviões e o Control?
//
//	R: Sim, é isso mesmo, mensagens com as modificações / posições dos aviões.
//
//
//	Q: Posso utilizar ponteiros na memória partilhada que apenas se referem ao inicio do meu array físico do Control para apenas visualizar a informação nos outros processos?
//	   E se fizer isto, consigo apenas ter que modificar o array físico e o ponteiro continua a apontar corretamente para este array?
//
//	R: ------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//	Q: Posso colocar os mapas em memória partilhada para o avião saber para onde ir? Ou é o Control que lhe diz para onde ele deve ir?
//
//	R: Sim e devo, mas devo ter atenção aos sincronismos de quem lê e quem escreve!!!
//
//
//	Q: Ao ler a memória partilhada, posso apenas ler uma das partes desta memória partilhada?
//	   Tipo mandar uma struct com dois int e ler apenas um específico?
//
//	R: Não é necessário, mais vale enviar tudo de uma só vez. O aviao necessita de uma Thread com um evento de nome igual para dizer para todos os aviões lerem! (pensar bem)
//
//
//	Q:
//
//	R:
//
//
//	Q:
//
//	R:
//
//
//-------//


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
#define MAX_PASS 5
#define MAX_PASS_CONTROL 20
//---------------------//

//Variáveis da Sincronização//
#define PLANE_MOVE_SYNC TEXT("MoveSync")
#define CONTROL_MUTEX_ENTRY TEXT("MutexEntry")
#define CONTROL_SEMAPHORE_ENTRY TEXT("PlaneGate")
//--------------------------//

//Estrutura passageiros//
typedef struct
{
	int tempo;						//tempo de espera / ao fim deste tempo o passageiro vai se embora
	TCHAR destino[TAM];				//aeroporto de destino
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
	SharedBuffer * shared;			//Memória Partilhada com Aviao
} ControlData;
//------------------------------//

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

//Função de Tratamento de Comandos do Control
//Recebe:
//		control	-	Dados do Control
//		comand	-	Comando introduzido pelo utilizador para Tratamento
//Retorna:
//		0	-	Comando sem espaços -> temporário
//		1	-	Comando foi tratado
int comandSwitcher(ControlData * control, TCHAR * comand);

//Função de Eliminação de Aviões
//Recebe:
//		control	-	Dados do Control
//		id		-	ID Plane a eliminar
int deletePlane(ControlData* control, int id);

#endif