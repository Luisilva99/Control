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
//	Passageiros s� podem entrar assim que os aeroportos estiverem definidos?
//
//	Os aeroportos podem ser criados a meio da execu��o (quando avi�es j� est�o em funcionamento / passageiros j� se encontram abordo dos avi�es)?
//
//	Podemos fazer malloc em arrays para passar para a mem�ria partilhada?
//
//	O que � consumido � as mensagens entre os Avi�es e o Control?
//
//	Posso utilizar ponteiros na mem�ria partilhada que apenas se referem ao inicio do meu array f�sico do Control para apenas visualizar a informa��o nos outros processos?
//	E se fizer isto, consigo apenas ter que modificar o array f�sico e o ponteiro continua a apontar corretamente para este array?
//
//-------//


//Vari�veis de Input//
#define TAM 100
#define TAM_INPUT 500
//------------------//

//Vari�veis Futuras Mem�ria//
#define MAX_THREADS 20
#define BUF_SIZE 256
#define MAP_TAM 1000
//-------------------------//

//Vari�veis do Registry//
#define REG_SETTINGS_KEY TEXT("Software\\CONTROL\\SETTINGS")
#define SETTINGS_PLANE TEXT("maxPlane")
#define MAX_PLANES 5
#define SETTINGS_AERO TEXT("maxAero")
#define MAX_AERO 5
#define MAX_PASS 5
//---------------------//

//Vari�veis da Sincroniza��o//
#define CONTROL_MUTEX TEXT("Nome")
#define CONTROL_SEMAPHORE_ENTRY TEXT("PlaneGate")
//--------------------------//

//Estrutura passageiros//
typedef struct
{
	int tempo;				//tempo de espera / ao fim deste tempo o passageiro vai se embora
	TCHAR a_chegada[TAM];	//aeroporto de chegada
	TCHAR a_partida[TAM];	//aeroporto de partida
} Passag;
//---------------------//

//Estrutura Avi�o//
typedef struct
{
	int id;					//id do avi�o
	int x, y;				//coordenadas
	TCHAR a_chegada[TAM];	//aeroporto de chegada
	TCHAR a_partida[TAM];	//aeroporto de partida
	Passag pass[MAX_PASS];	//max passageiros
	int velocidade;			//velocidade do avi�o
} Plane;
//---------------//

//Estrutura da C�lula do Mapa//
typedef struct
{
	//Restruturar esta estrutura
	int isAero;						//Se � Aeroporto ou n�o -> modificar esta estrutura
	Plane hangar[MAX_PLANES];		//Avi�es dentro do Aeroporto
	Passag passageiros[MAX_PASS];	//Passageiros em espera
	TCHAR aeroName[TAM_INPUT];		//Nome do Aeroporto
	int Y, X;						//Coordenadas do Aeroporto no Mapa
} MapUnit;
//---------------------------//

//Estrutura da Mem�ria Partilhada//
typedef struct
{
	Plane planes[MAX_PLANES * 3];	//Avi�es
	MapUnit aeros[MAX_AERO * 3];	//Aeroportos
	int maxAero, maxPlane;			//M�ximo de Avi�es e Aeroportos nos Arrays
	TCHAR msg[TAM_INPUT];			//Menssagens trocadas entre o Control e o Aviao
} SharedMemory;
//-------------------------------//

// Estrutura Template dos Dados do Control
typedef struct {
	MapUnit *map;				//Aeroportos
	int maxAero, curAero;		//M�ximo de Aeroportos e tamanho atual
	Plane *planes;				//Avi�es
	int maxPlane, curPlane;		//M�ximo de Avi�es e tamanho atual
	Passag maxPass[MAX_PASS];	//Passageiros
	SharedMemory memory;		//Mem�ria Partilhada
} ControlData;

//Fun��o de Obten��o de inteiros
//Retorna:
//		Inteiro
int getIntInput();

//Fun��o de Obten��o de frases
//Retorna:
//		Array de TCHAR
TCHAR *getTCHARInput();

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