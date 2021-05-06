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
//
//-------//


//Variáveis de Input//
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
//---------------------//

//Variáveis da Sincronização//
#define CONTROL_MUTEX TEXT("Nome")
#define CONTROL_SEMAPHORE_ENTRY TEXT("PlaneGate")
//--------------------------//

//Estrutura passageiros//
typedef struct
{
	int tempo;						//tempo de espera / ao fim deste tempo o passageiro vai se embora
	TCHAR a_chegada[TAM];			//aeroporto de chegada
	TCHAR a_partida[TAM];			//aeroporto de partida
} Passag;
//---------------------//

//Estrutura Avião//
typedef struct
{
	int id;							//id do avião
	int x, y;						//coordenadas
	TCHAR a_chegada[TAM];			//aeroporto de chegada
	TCHAR a_partida[TAM];			//aeroporto de partida
	Passag pass[MAX_PASS];			//max passageiros
	int velocidade;					//velocidade do avião
	TCHAR controlResponse[TAM_INPUT];//resposta do Control ao Aviao
} Plane;
//---------------//

//Estrutura da Célula do Mapa//
typedef struct
{
	Plane hangar[MAX_PLANES];		//Aviões dentro do Aeroporto
	Passag passageiros[MAX_PASS];	//Passageiros em espera no Aeroporto
	TCHAR aeroName[TAM_INPUT];		//Nome do Aeroporto
	int Y, X;						//Coordenadas do Aeroporto no Mapa
} MapUnit;
//---------------------------//

//Estruturas da Memória Partilhada//
typedef struct {
	TCHAR msg[TAM_INPUT];
} CircularBuffer;

typedef struct {
	CircularBuffer buffer[TAM_BUFF];
	int locReadAtual, locWriteAtual;
} TotalCircularBuffer;
//-------------------------------//

//Variáveis Futuras Memória//
#define BUF_MAP sizeof(MapUnit)
#define BUF_PLANE sizeof(Plane)
#define BUF_CIRCULAR sizeof(TotalCircularBuffer)
#define MAP_TAM 1000
//-------------------------//

//Estrutura dos Dados do Control//
typedef struct {
	Passag Pass[MAX_PASS];			//Passageiros
	MapUnit map[MAX_AERO];			//Aeroportos
	int maxAero, curAero;			//Máximo de Aeroportos e tamanho atual
	Plane planes[MAX_PLANES];		//Aviões e as estruturas de mensagens
	int maxPlane, curPlane;			//Máximo de Aviões e tamanho atual
	HANDLE planeViews[MAX_PLANES];
	TotalCircularBuffer tCircBuffer;
} ControlData;
//------------------------------//

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