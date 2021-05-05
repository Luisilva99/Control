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
//	Passageiros só podem entrar assim que os aeroportos estiverem definidos?
//
//	Os aeroportos podem ser criados a meio da execução (quando aviões já estão em funcionamento / passageiros já se encontram abordo dos aviões)?
//
//	Podemos fazer malloc em arrays para passar para a memória partilhada?
//
//	O que é consumido é as mensagens entre os Aviões e o Control?
//
//	Posso utilizar ponteiros na memória partilhada que apenas se referem ao inicio do meu array físico do Control para apenas visualizar a informação nos outros processos?
//	E se fizer isto, consigo apenas ter que modificar o array físico e o ponteiro continua a apontar corretamente para este array?
//
//-------//


//Variáveis de Input//
#define TAM 100
#define TAM_INPUT 500
//------------------//

//Variáveis Futuras Memória//
#define MAX_THREADS 20
#define BUF_SIZE 256
#define MAP_TAM 1000
//-------------------------//

//Variáveis do Registry//
#define REG_SETTINGS_KEY TEXT("Software\\CONTROL\\SETTINGS")
#define SETTINGS_PLANE TEXT("maxPlane")
#define MAX_PLANES 5
#define SETTINGS_AERO TEXT("maxAero")
#define MAX_AERO 5
#define MAX_PASS 5
//---------------------//

//Variáveis da Sincronização//
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

//Estrutura Avião//
typedef struct
{
	int id;					//id do avião
	int x, y;				//coordenadas
	TCHAR a_chegada[TAM];	//aeroporto de chegada
	TCHAR a_partida[TAM];	//aeroporto de partida
	Passag pass[MAX_PASS];	//max passageiros
	int velocidade;			//velocidade do avião
} Plane;
//---------------//

//Estrutura da Célula do Mapa//
typedef struct
{
	//Restruturar esta estrutura
	int isAero;						//Se é Aeroporto ou não -> modificar esta estrutura
	Plane hangar[MAX_PLANES];		//Aviões dentro do Aeroporto
	Passag passageiros[MAX_PASS];	//Passageiros em espera
	TCHAR aeroName[TAM_INPUT];		//Nome do Aeroporto
	int Y, X;						//Coordenadas do Aeroporto no Mapa
} MapUnit;
//---------------------------//

//Estrutura da Memória Partilhada//
typedef struct
{
	Plane planes[MAX_PLANES * 3];	//Aviões
	MapUnit aeros[MAX_AERO * 3];	//Aeroportos
	int maxAero, maxPlane;			//Máximo de Aviões e Aeroportos nos Arrays
	TCHAR msg[TAM_INPUT];			//Menssagens trocadas entre o Control e o Aviao
} SharedMemory;
//-------------------------------//

// Estrutura Template dos Dados do Control
typedef struct {
	MapUnit *map;				//Aeroportos
	int maxAero, curAero;		//Máximo de Aeroportos e tamanho atual
	Plane *planes;				//Aviões
	int maxPlane, curPlane;		//Máximo de Aviões e tamanho atual
	Passag maxPass[MAX_PASS];	//Passageiros
	SharedMemory memory;		//Memória Partilhada
} ControlData;

//Função de Obtenção de inteiros
//Retorna:
//		Inteiro
int getIntInput();

//Função de Obtenção de frases
//Retorna:
//		Array de TCHAR
TCHAR *getTCHARInput();

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
int comandSwitcher(ControlData * control, TCHAR * comand);

#endif