#include "Passageiro.h"
#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>




int _tmain(int argc, TCHAR* argv[]) {



    //Variaveis de comunicação dos pipes
    HANDLE hPipe;
    DWORD dwWritten;



//########################################
#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

//########################################



   hPipe = CreateFile(TEXT("\\\\.\\pipe\\Pipe"),
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);
   if (hPipe != INVALID_HANDLE_VALUE)
   {
        WriteFile(hPipe,
            "Hello Pipe\n",
             12,   // = length of string + terminating '\0' !!!
             &dwWritten,
             NULL);

        CloseHandle(hPipe);
   }

        return (0);
}

