#ifndef CONTROLUSE_H
#define CONTROLUSE_H

#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <winreg.h>


//D�vidas//
//	Q: Passageiros s� podem entrar assim que os aeroportos estiverem definidos?
//
//	R: ------------------------------------------------------------------------------------------------------------------------------------------
//
//
//	Q: Os aeroportos podem ser criados a meio da execu��o (quando avi�es j� est�o em funcionamento / passageiros j� se encontram abordo dos avi�es)?
//
//	R: ------------------------------------------------------------------------------------------------------------------------------------------
//
//
//	Q: Podemos fazer malloc em arrays para passar para a mem�ria partilhada?
//
//	R: Tem a haver com as Views, uma mem�ria partilhada pode ter v�rias Views come�ando com offsets diferentes. A resposta � avisada por um evento para o avi�o ler!
//     Temos de ter uma estrutura de resposta para os avi�es ler a resposta e esta estrutura facilmente identificada para leitura dele.
//
//
//	Q: O que � consumido � as mensagens entre os Avi�es e o Control?
//
//	R: Sim, � isso mesmo, mensagens com as modifica��es / posi��es dos avi�es.
//
//
//	Q: Posso utilizar ponteiros na mem�ria partilhada que apenas se referem ao inicio do meu array f�sico do Control para apenas visualizar a informa��o nos outros processos?
//	   E se fizer isto, consigo apenas ter que modificar o array f�sico e o ponteiro continua a apontar corretamente para este array?
//
//	R: ------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//	Q: Posso colocar os mapas em mem�ria partilhada para o avi�o saber para onde ir? Ou � o Control que lhe diz para onde ele deve ir?
//
//	R:
//
//
//	Q: Ao ler a mem�ria partilhada, posso apenas ler uma das partes desta mem�ria partilhada?
//	   Tipo mandar uma struct com dois int e ler apenas um espec�fico?
//
//	R:
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


//Vari�veis de Input//
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
#define MAX_PASS 5
#define MAX_PASS_CONTROL 20
//---------------------//

//Vari�veis da Sincroniza��o//
#define CONTROL_MUTEX TEXT("Nome")
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

//Estrutura Avi�o//
typedef struct
{
	int id;							//id do avi�o / processo do avi�o
	TCHAR destino[TAM];				//aeroporto de destino
	int next_X, next_Y;				//coordenadas / posi��o final
	TCHAR partida[TAM];				//aeroporto de partida
	int X, Y;						//coordenadas / posi��o atual
	Passag pass[MAX_PASS];			//passageiros
	int maxPass, curPass;			//m�ximo de passageiros e tamanho atual
	int velocidade;					//velocidade do avi�o
	//TCHAR controlResponse[TAM_INPUT];//resposta do Control ao Aviao
} Plane;
//---------------//

//Estrutura da C�lula do Mapa//
typedef struct
{
	Plane hangar[MAX_PLANES];		//Avi�es dentro do Aeroporto
	Passag passageiros[MAX_PASS];	//Passageiros em espera no Aeroporto
	TCHAR aeroName[TAM_INPUT];		//Nome do Aeroporto
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
#define BUF_MAP sizeof(MapUnit)
#define BUF_PLANE sizeof(Plane)
#define BUF_CIRCULAR sizeof(TotalCircularBuffer)
#define MAP_TAM 1000
//-------------------------//

//Estrutura dos Dados do Control//
typedef struct {
	Passag Pass[MAX_PASS_CONTROL];	//Passageiros
	int maxPass, curPass;			//M�ximo de Passageiros e tamanho atual
	Plane planes[MAX_PLANES];		//Avi�es e as estruturas de mensagens
	int maxPlane, curPlane;			//M�ximo de Avi�es e tamanho atual
	MapUnit map[MAX_AERO];			//Aeroportos
	int maxAero, curAero;			//M�ximo de Aeroportos e tamanho atual
	SharedBuffer * shared;			//Mem�ria Partilhada com Aviao
} ControlData;
//------------------------------//

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

//Fun��o de Tratamento de Comandos do Control
//Recebe:
//		control	-	Dados do Control
//		comand	-	Comando introduzido pelo utilizador para Tratamento
//Retorna:
//		0	-	Comando sem espa�os -> tempor�rio
//		1	-	Comando foi tratado
int comandSwitcher(ControlData * control, TCHAR * comand);

#endif