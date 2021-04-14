#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

DWORD WINAPI MyThreadFunction(LPVOID lpParam);
int obterValorDoUtilizador();

// estrutura de dados para controlar as threads
typedef struct {
	int start;
	int end;
	int *count;
	HANDLE *mutex;
} TDados;

#define TAM 100

// função da(s) thread(s)
// ...
// número * máximo * de threads
// podem (e devem) ser menos
#define MAX_THREADS 20

int _tmain(int argc, TCHAR * argv[]) {
	// matriz de handles das threads
	HANDLE hThreads[MAX_THREADS];
	DWORD dwThreadIdArray[MAX_THREADS];
	// Matriz de dados para as threads;
	TDados tdados[MAX_THREADS];
	// número efectivo de threads
	int numthreads;
	// limite superior
	unsigned int limsup;

	unsigned int range;

	int counter = 0;

	HANDLE ghMutex;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif

	_tprintf(TEXT("\nLimite sup. -> "));
	limsup = obterValorDoUtilizador();

	_tprintf(TEXT("\nNum threads -> "));
	numthreads = obterValorDoUtilizador();

	// verificação do máximo de threads limitadas pelo sistema
	if (numthreads > MAX_THREADS)
		numthreads = MAX_THREADS;

	// INICIO DO MUTEX
	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex

	if (ghMutex == NULL)
	{
		_tprintf(TEXT("CreateMutex error: %d\n"), GetLastError());
		return -1;
	}

	// FAZER prepara e cria threads

	for (int i = 0, liminf = 1; i < numthreads; i++)
	{

		range = ((limsup - liminf - 1) / numthreads);

		if (i == 0)			tdados[i].start = liminf;		else			tdados[i].start = tdados[i - 1].end + 1;		tdados[i].end = tdados[i].start + range;		//Qd o número for muito grande pode fazer overflow e não conseguir entrar no if		if ((tdados[i].end != limsup) && ((tdados[i].end + range) > limsup))			tdados[i].end = limsup;

		tdados[i].mutex = &ghMutex;
		tdados[i].count = &counter;
		//tdados[i].start = ((limsup / numthreads) * i);
		//tdados[i].end = ((limsup / numthreads) * i) + (limsup / numthreads);

		_tprintf(TEXT("\nValor mínimo: %d \nValor máximo: %d \n"), tdados[i].start, tdados[i].end);

		hThreads[i] = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size
			MyThreadFunction,       // thread function name
			&tdados[i],          // argument to thread function
			CREATE_SUSPENDED,                      // use default creation flags
			&dwThreadIdArray[i]);   // returns the thread identifier

		if (hThreads[i] == NULL)
		{
			_tprintf(TEXT("\nCreateThread Error\n"));
		}
	}

	for (int i = 0; i < numthreads; i++)
	{
		if (ResumeThread(hThreads[i]) == -1)
		{
			_tprintf(TEXT("\nErro ao resume da Thread!!!\n"));
		}
	}

	// manda as threads começar
	// FAZER aguarda / controla as threads

	WaitForMultipleObjects(MAX_THREADS, hThreads, TRUE, INFINITE);

	for (int i = 0; i < numthreads; i++)
	{
		CloseHandle(hThreads[i]);
	}

	CloseHandle(ghMutex);

	// manda as threads parar
	// Cód. ref. para aguardar por uma tecla – caso faça falta
	// _tprintf(TEXT("\nCarregue numa tecla"));
	// _gettch();

	_tprintf(TEXT("\nMúltiplos de 3 entre 1 a %d : %d\n"), limsup, counter);

	return 0;
}



DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	TDados * pDataArray;
	DWORD dwWaitResult;
	BOOLEAN continuar = TRUE;

	pDataArray = (TDados*)lpParam;

	for (int i = pDataArray->start; i < pDataArray->end; i++)
	{


		if (!(i % 3))
		{

			while (1)
			{
				dwWaitResult = WaitForSingleObject(
					(*pDataArray->mutex),    // handle to mutex
					INFINITE);  // no time-out interval

				switch (dwWaitResult)
				{
					// The thread got ownership of the mutex
				case WAIT_OBJECT_0:
					(*pDataArray->count)++;
					ReleaseMutex((*pDataArray->mutex));
					break;

				case WAIT_ABANDONED:
					continue;

				default:
					continue;
				}
			}

		}

	}

	return 0;
}


int obterValorDoUtilizador() {
	TCHAR valor[TAM];

	fflush(stdin);
	_fgetts(valor, TAM, stdin);

	valor[_tcslen(valor) - 1] = TEXT('\0');

	// usar este para juntar strings com valores
	//_stprintf_s(chave_nome, TAM, TEXT("Software\\AULA\\%s"), par_nome);

	return _tstoi(valor);
}

