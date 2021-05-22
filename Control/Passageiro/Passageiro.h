#pragma once

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
#define MAX_PASS 5
#define MAX_PASS_CONTROL 20
//---------------------//



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