#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

int _tmain(int argc, TCHAR * argv[]) {

	// mem�ria partilhada
	HANDLE hMapFile;
	LPTSTR pBuf = NULL;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	return 0;
}