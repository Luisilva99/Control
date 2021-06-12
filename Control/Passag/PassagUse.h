#ifndef PASSAGUSE_H
#define PASSAGUSE_H

#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <winreg.h>


//Dúvidas//
//
//	Q: Que tipos de mensagens são trocadas entre o Passag e o Control?
//
//	R:
//
//	Q: Qual a melhor abordagem, OVERLAPPED ou por multithreaded?
//
//	R:
//
//	Q:
//
//	R:
//
//-------//


//Variáveis de Input//
#define TAM 100
#define TAM_INPUT 500
//------------------//

//Variáveis da Sincronização//
#define KILLER_TRIGGER TEXT("KillAllSystems")
#define SENDER_TRIGGER TEXT("SendMsg")
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
	HANDLE hPipe;					// Handle de coms Pipe
	TCHAR resp[TAM];				// Resposta do Control
	TCHAR msg[TAM];					// Msg do Control
	HANDLE respTrigger;				// Handle do evento de envio de mensagem para o Control
} Passag;
//---------------------//


//Função de Obtenção de inteiros
//Retorna:
//		Inteiro
int getIntInput();

//Função de Obtenção de frases
//Recebe:
//		szMsg	-	Array de TCHAR
//		tam		-	Tamanho do Array de TCHAR
void getTCHARInput(TCHAR * szMsg, int tam);

//Função de apresentação da informação de um Passageiro
//Recebe:
//		pass	-	Estrutura de Passag com dados relacionados ao Passageiro
void listPassInfo(Passag pass);

//Thread de Tratamento de Comandos
//Recebe:
//		lpParam	-	Dados do Passag
DWORD WINAPI tratamentoDeComandos(LPVOID lpParam);

//Thread de Tratamento de Comunicação por Pipe
//Recebe:
//		lpParam	-	Dados do Passag
DWORD WINAPI tratamentoDeComunicacao(LPVOID lpParam);

//Thread de Tratamento de Envio de Mensagens do Passag ao Control por Pipe
//Recebe:
//		lpParam	-	Dados do Passag
DWORD WINAPI ComsResponder(LPVOID lpParam);

//Função de Tratamento de Comandos do Passag
//Recebe:
//		pass	-	Dados do Passag
//		comand	-	Comando introduzido pelo utilizador para Tratamento
//Retorna:
//		0	-	Comando sem espaços
//		1	-	Comando foi tratado
int comandSwitcher(Passag * pass, TCHAR * comand);


#endif