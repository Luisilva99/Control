// ControlTeste_2Teste.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "ControlTeste_2Teste.h"
#include "ControlTesteUse.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int, ControlData*);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Login(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	createAero(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	infoAero(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	infoPassag(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	infoPlane(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    LinhaDeComandos(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	/*UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);*/

	// TODO: Place code here.
	// Memória Partilhada
	HANDLE hMapFile;

	// Limites e Erros
	int maxPlane, maxAero, tipoErro, valorArgumento;

	// Sincroniza��o
	HANDLE semaphoreGate;
	HANDLE semaphorePassagGate;
	HANDLE mutexMoveSync;
	HANDLE mutexWriters;

	// Threads
	HANDLE hThread[3];
	DWORD dwThread[3];

	// Dados do Control
	ControlData control;
	SharedBuffer * pShared;
	SharedBuffer shared;


	//// Named Pipes
	//HANDLE hPipe;
	//char buffer[1024];
	//DWORD dwRead;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	//#####Limites Registry#####//

	if ((maxAero = readAeroLimits()) < 0) {
		maxAero = MAX_AERO;

		if ((tipoErro = createAeroLimits(MAX_AERO)) == 0)
		{
			_tprintf(TEXT("\nDefinição do limite de Aeroportos definido no programa foi guardado no Sistema.\n"));
		}
		else
		{
			_tprintf(TEXT("\nErro crítico! - ERRO 1 - Tipo %d\n"), tipoErro);

			return -1;
		}
	}

	if ((maxPlane = readPlaneLimits()) < 0) {
		maxPlane = MAX_PLANES;

		if ((tipoErro = createPlaneLimits(MAX_PLANES)) == 0)
		{
			_tprintf(TEXT("\nDefinição do limite de Aviões definido no programa foi guardado no Sistema.\n"));
		}
		else
		{
			_tprintf(TEXT("\nErro crítico! - ERRO 2 - Tipo %d\n"), tipoErro);

			return -2;
		}
	}

	//#####----------------#####//

	//#######Tratamento de Argumentos#######//

	if (_tcsclen(lpCmdLine) > 0)
	{
		TCHAR * auxA;
		TCHAR * auxB = _tcstok_s(lpCmdLine, TEXT(" "), &auxA);

		if (auxB == NULL || !_tcslen(auxA))
		{
			return 0;
		}

		if (auxB != NULL)
		{
			valorArgumento = _tstoi(auxB);

			if (valorArgumento > MAX_AERO || valorArgumento <= 0)
			{
				valorArgumento = MAX_AERO;
			}

			if ((tipoErro = createAeroLimits(valorArgumento)) == 0)
			{
				_tprintf(TEXT("\nDefinição do limite de Aeroportos foi guardada no Sistema.\n"));

				maxAero = valorArgumento;
			}
			else
			{
				_tprintf(TEXT("\nErro crítico! - ERRO 1 - Tipo %d\nUsar valores de Sistema.\n"), tipoErro);

				if ((maxAero = readAeroLimits()) < 0) {
					maxAero = MAX_AERO;

					if ((tipoErro = createAeroLimits(MAX_AERO)) == 0)
					{
						_tprintf(TEXT("\nDefinição do limite de Aeroportos foi guardada no Sistema.\n"));
					}
					else
					{
						_tprintf(TEXT("\nErro crítico! - ERRO 1 - Tipo %d\n"), tipoErro);

						return -3;
					}
				}
			}
		}

		if (auxA != NULL)
		{
			//Limitar aqui para o máximo definido no programa//DEBUG
			valorArgumento = _tstoi(auxA);

			if (valorArgumento > MAX_PLANES || valorArgumento <= 0)
			{
				valorArgumento = MAX_PLANES;
			}

			if ((tipoErro = createPlaneLimits(valorArgumento)) == 0)
			{
				_tprintf(TEXT("\nDefinição do limite de Aviões foi guardada no Sistema.\n"));

				maxPlane = valorArgumento;
			}
			else
			{
				_tprintf(TEXT("\nErro crítico! - ERRO 2 - Tipo %d\nUsar valores de Sistema.\n"), tipoErro);

				if ((maxPlane = readPlaneLimits()) < 0) {
					maxPlane = MAX_PLANES;

					if ((tipoErro = createPlaneLimits(MAX_PLANES)) == 0)
					{
						_tprintf(TEXT("\nDefinição do limite de Aviões foi guardada no Sistema.\n"));
					}
					else
					{
						_tprintf(TEXT("\nErro crítico! - ERRO 2 - Tipo %d\n"), tipoErro);

						return -4;
					}
				}
			}
		}
	}

	//#######------------------------#######//

	_tprintf(TEXT("\nLimite máximo de aeroportos: %d\nLimite máximo de aviões: %d\n"), maxAero, maxPlane);// DEBUG

	//###########Inicializa��o Padr�o dos Dados do Control###########//

	for (int i = 0; i < MAX_PASS_CONTROL; i++)
	{
		ZeroMemory(&control.Pass[i], sizeof(Passag));
	}

	control.curPass = 0;

	control.maxPass = MAX_PASS_CONTROL;

	control.entryStopped = 0;

	ZeroMemory(&shared, sizeof(SharedBuffer));

	shared.maxAero = maxAero;

	shared.maxPlane = maxPlane;

	shared.tCircBuffer.maxBuffer = TAM_BUFF;

	for (int i = 0; i < shared.maxAero; i++)
	{
		shared.map[i].maxHang = MAX_PLANES;

		shared.map[i].maxPass = MAX_PASS;
	}

	pShared = (SharedBuffer *)malloc(sizeof(SharedBuffer));

	_tprintf(TEXT("\nInicialização da memória física do Control foi um Sucesso!\n"));// DEBUG

	//###########-----------------------------------------###########//

	//#############Memória Partilhada#############//

	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		sizeof(SharedBuffer),                // maximum object size (low-order DWORD)
		TEXT("CentralMemory"));                 // name of mapping object


	if (hMapFile == NULL)
	{
		_tprintf(TEXT("Could not create file mapping object (%d).\n"), GetLastError());

		CloseHandle(hMapFile);

		_gettch();

		return -5;
	}
	else if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		_tprintf(TEXT("Já existe uma execução em curso.\nVou encerrar!\n"));

		CloseHandle(hMapFile);

		_gettch();

		return -6;
	}

	pShared = (SharedBuffer *)MapViewOfFile(hMapFile,	// handle to map object
		FILE_MAP_ALL_ACCESS,				// read/write permission
		0,									// Por onde o resto a mais da memória pode ser mapeada
		0,									// Por onde a memória irá começar a ser mapeada
		sizeof(SharedBuffer));

	if (pShared == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"), GetLastError());

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -7;
	}

	CopyMemory(pShared, &shared, sizeof(SharedBuffer));

	control.shared = pShared;

	_tprintf(TEXT("\nMemória Partilhada criada com sucesso e copiado o buffer circular.\n"));// DEBUG

	//#############------------------#############//

	//#####Sincronismos#####//

	mutexMoveSync = CreateMutex(
		NULL,
		FALSE,
		PLANE_MOVE_SYNC
	);

	if (mutexMoveSync == NULL)
	{
		_tprintf(TEXT("\nCriação do mutex de sincronização de movimentação dos Aviões não foi criado com sucesso!\nErro %d\n"), GetLastError());

		CloseHandle(mutexMoveSync);

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -8;
	}

	_tprintf(TEXT("\nCriação do mutex de sincronização de movimentação dos Aviões foi criado com sucesso!\n"));// DEBUG

	semaphoreGate = CreateSemaphore(
		NULL,
		maxPlane,
		maxPlane,
		CONTROL_SEMAPHORE_ENTRY
	);

	if (semaphoreGate == NULL)
	{
		_tprintf(TEXT("\nCriação do semaforo de entrada de aviões não foi criado com sucesso!\nErro %d\n"), GetLastError());

		CloseHandle(semaphoreGate);

		CloseHandle(mutexMoveSync);

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -9;
	}

	_tprintf(TEXT("\nCriação do semaforo de entrada de aviões foi criado com sucesso!\n"));// DEBUG

	control.entry = CreateMutex(
		NULL,
		FALSE,
		CONTROL_MUTEX_ENTRY
	);

	if (control.entry == NULL)
	{
		_tprintf(TEXT("\nCriação do mutex de permissão de entrada de aviões não foi criado com sucesso!\nErro %d\n"), GetLastError());

		CloseHandle(semaphoreGate);

		CloseHandle(mutexMoveSync);

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -10;
	}

	_tprintf(TEXT("\nCriação do mutex de permissão de entrada de aviões foi criado com sucesso!\n"));// DEBUG

	control.systemShutdown = CreateEvent(
		NULL,
		TRUE,
		FALSE,
		KILLER_TRIGGER
	);

	if (control.systemShutdown == NULL)
	{
		_tprintf(TEXT("\nCriação do evento de terminação de todos os sistemas não foi criado com sucesso!\nErro %d\n"), GetLastError());

		CloseHandle(control.entry);

		CloseHandle(semaphoreGate);

		CloseHandle(mutexMoveSync);

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -11;
	}

	_tprintf(TEXT("\nCriação do evento de terminação de todos os sistemas foi criado com sucesso!\n"));

	mutexWriters = CreateMutex(
		NULL,
		FALSE,
		PLANE_MUTEX_WRITER
	);

	if (mutexWriters == NULL)
	{
		_tprintf(TEXT("\nMutex de escrita no Buffer Circular não foi criado com sucesso!\nErro %d\n"), GetLastError());

		CloseHandle(control.systemShutdown);

		CloseHandle(semaphoreGate);

		CloseHandle(mutexMoveSync);

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -12;
	}

	_tprintf(TEXT("\nMutex de escrita no Buffer Circular foi criado com sucesso!\n"));// DEBUG

	control.readBuffer = CreateSemaphore(
		NULL,
		0,
		TAM_BUFF,
		CONTROL_SEMAPHORE_BUFFER_READER
	);

	if (control.readBuffer == NULL)
	{
		_tprintf(TEXT("\nSemáforo de leitura do Buffer Circular não foi criado com sucesso!\nErro %d\n"), GetLastError());

		CloseHandle(mutexWriters);

		CloseHandle(control.systemShutdown);

		CloseHandle(semaphoreGate);

		CloseHandle(mutexMoveSync);

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -13;
	}

	_tprintf(TEXT("\nSemáforo de leitura do Buffer Circular foi criado com sucesso!\n"));// DEBUG

	semaphorePassagGate = CreateSemaphore(
		NULL,
		MAX_PASS,
		MAX_PASS,
		CONTROL_SEMAPHORE_PASSAG_ENTRY
	);

	if (semaphorePassagGate == NULL)
	{
		_tprintf(TEXT("\nSemáforo de entrada de Passageiros não foi criado com sucesso!\nErro %d\n"), GetLastError());

		CloseHandle(control.readBuffer);

		CloseHandle(mutexWriters);

		CloseHandle(control.systemShutdown);

		CloseHandle(semaphoreGate);

		CloseHandle(mutexMoveSync);

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -14;
	}

	_tprintf(TEXT("\nSemáforo de entrada de Passageiros foi criado com sucesso!\n"));// DEBUG

	control.passagSender = CreateEvent(
		NULL,
		FALSE,
		FALSE,
		SENDER_TRIGGER
	);

	if (control.passagSender == NULL)
	{
		_tprintf(TEXT("\nCriação do evento de envio de mensagens por Pipes não foi criado com sucesso!\nErro %d\n"), GetLastError());

		CloseHandle(semaphorePassagGate);

		CloseHandle(control.readBuffer);

		CloseHandle(mutexWriters);

		CloseHandle(control.systemShutdown);

		CloseHandle(semaphoreGate);

		CloseHandle(mutexMoveSync);

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		_gettch();

		return -15;
	}

	_tprintf(TEXT("\nCriação do evento de envio de mensagens por Pipes foi criado com sucesso!\n"));//DEBUG

	//#####------------#####//

	//######Lançamento das Threads######//


	hThread[0] = CreateThread(
		NULL,
		0,
		tratamentoDeComandos,
		(LPVOID)&control,
		0,
		&dwThread[0]
	);

	if (hThread[0] == NULL)
	{
		_tprintf(TEXT("CreateThread failed, GLE=%d.\n"), GetLastError());

		CloseHandle(control.passagSender);

		CloseHandle(semaphorePassagGate);

		CloseHandle(control.readBuffer);

		CloseHandle(mutexWriters);

		CloseHandle(control.systemShutdown);

		CloseHandle(control.entry);

		CloseHandle(semaphoreGate);

		CloseHandle(mutexMoveSync);

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		return -16;
	}

	hThread[1] = CreateThread(
		NULL,
		0,
		bufferCircular,
		(LPVOID)&control,
		0,
		&dwThread[1]
	);

	if (hThread[1] == NULL)
	{
		_tprintf(TEXT("CreateBufferThread failed, GLE=%d.\n"), GetLastError());

		CloseHandle(hThread[0]);

		CloseHandle(control.passagSender);

		CloseHandle(semaphorePassagGate);

		CloseHandle(control.readBuffer);

		CloseHandle(mutexWriters);

		CloseHandle(control.systemShutdown);

		CloseHandle(control.entry);

		CloseHandle(semaphoreGate);

		CloseHandle(mutexMoveSync);

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		return -17;
	}

	hThread[2] = CreateThread(
		NULL,
		0,
		tratamentoDeComunicacao,
		(LPVOID)&control,
		0,
		&dwThread[2]
	);

	if (hThread[2] == NULL)
	{
		_tprintf(TEXT("Create Passag Coms Test failed, GLE=%d.\n"), GetLastError());

		CloseHandle(hThread[1]);

		CloseHandle(hThread[0]);

		CloseHandle(control.passagSender);

		CloseHandle(semaphorePassagGate);

		CloseHandle(control.readBuffer);

		CloseHandle(mutexWriters);

		CloseHandle(control.systemShutdown);

		CloseHandle(control.entry);

		CloseHandle(semaphoreGate);

		CloseHandle(mutexMoveSync);

		UnmapViewOfFile(pShared);

		CloseHandle(hMapFile);

		return -18;
	}

	//######----------------------######//

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CONTROLTESTE2TESTE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow, &control))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CONTROLTESTE2TESTE));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	_tprintf(TEXT("\nLibertação das Threads criadas!\nLibertação dos HANDLES de sincronização!\nLibertação da Memória Partilhada!\n"));

	for (int i = 0; i < 3; i++)
	{
		CloseHandle(hThread[i]);
	}

	CloseHandle(control.passagSender);

	CloseHandle(semaphorePassagGate);

	CloseHandle(control.readBuffer);

	CloseHandle(mutexWriters);

	CloseHandle(control.systemShutdown);

	CloseHandle(control.entry);

	CloseHandle(semaphoreGate);

	CloseHandle(mutexMoveSync);

	UnmapViewOfFile(pShared);

	CloseHandle(hMapFile);

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CONTROLTESTE2TESTE));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CONTROLTESTE2TESTE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, ControlData * control)
{
	HDC hdc;
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1000,
		1000,
		NULL,
		NULL,
		hInstance,
		control);

	if (!hWnd)
	{
		return FALSE;
	}

	//IMAGEM-BITMAP
	control->hBmp = (HBITMAP)LoadImage(NULL, TEXT("bitmap2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);  //imagem do aviao(bloco preto)
	GetObject(control->hBmp, sizeof(control->bmp), &(*control).bmp);

	hdc = GetDC(hWnd);
	control->bmpDC = CreateCompatibleDC(hdc);
	SelectObject(control->bmpDC, control->hBmp);
	ReleaseDC(hWnd, hdc);


	//meter  imagem no centro
	GetClientRect(hWnd, &(*control).rect);
	control->xBitmap = (control->rect.right / 2) - (control->bmp.bmWidth / 2);
	control->yBitmap = (control->rect.bottom / 2) - (control->bmp.bmHeight / 2);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//Para o Parametro enviado janela
	LONG_PTR userdata = GetWindowLongPtr(hWnd, GWLP_USERDATA);
	ControlData *control = NULL;

	HDC hdc;
	RECT rect;
	PAINTSTRUCT ps;
	TCHAR aux[TAM_INPUT];

	if (message == WM_CREATE)
	{
		CREATESTRUCT* pCreate = (CREATESTRUCT*)(lParam);
		control = (ControlData*)pCreate->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)control);
	}
	else
	{
		control = (ControlData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ID_TESTE_TESTE:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, LinhaDeComandos);
			break;
		case ID_FILE_LOGIN:
			DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hWnd, createAero, &control);
			break;
		case ID_FUNCIONALIDADESEXTRA_PASSAGREGISTADOS:
			DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG4), hWnd, infoPassag, &control);
			break;
		case ID_FUNCIONALIDADESEXTRA_AVIAOREGISTADOS:
			DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG4), hWnd, infoPlane, &control);
			break;
		case ID_FUNCIONALIDADESEXTRA_AEROPORTOSREGISTADOS:
			DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG4), hWnd, infoAero, &control);
			break;
		case ID_FILE_PARARENTRADAAVI32773:
			if (control->entryStopped == 0)
			{
				WaitForSingleObject(control->entry, INFINITE);

				(*control).entryStopped = 1;

				break;
			}

			if (control->entryStopped == 1)
			{
				ReleaseMutex(control->entry);

				(*control).entryStopped = 0;

				break;
			}
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:  //evento disparado para fazer refresh da janela
		hdc = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 200, 0)));
		SetTextColor(hdc, RGB(0, 0, 0));
		SetBkColor(hdc, RGB(255, 255, 255));

		BitBlt(hdc, control->xBitmap, control->yBitmap, control->bmp.bmWidth, control->bmp.bmHeight, control->bmpDC, 0, 0, SRCCOPY);


		EndPaint(hWnd, &ps);
		break;

	case WM_SIZE:       //isto faz com que a iamgem fique sempre centralizada mesmo com redimensionamento da janela
		control->xBitmap = (LOWORD(lParam) / 2) - (control->bmp.bmWidth / 2);
		control->yBitmap = (HIWORD(lParam) / 2) - (control->bmp.bmHeight / 2);

	case WM_LBUTTONDOWN:
		rect.top = LOWORD(lParam);
		rect.bottom = HIWORD(lParam);
		_stprintf_s(aux, TAM_INPUT, TEXT("PosX= %d,PosY= %d"), rect.top, rect.bottom);
		MessageBox(hWnd, aux, L"Coordenadas do cursor:", MB_OK);

		break;
	case WM_ERASEBKGND:
		return TRUE;
	case WM_DESTROY:
		SetEvent(control->systemShutdown);

		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Login(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR lpszPassword[16];
	WORD cchPassword;

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		// Set the default push button to "OK" when the user enters text.
		if (HIWORD(wParam) == EN_CHANGE &&
			LOWORD(wParam) == IDC_EDIT2)
		{
			SendMessage(hDlg,
				DM_SETDEFID,
				(WPARAM)IDOK,
				(LPARAM)0);
		}
		switch (wParam)
		{
		case IDOK:
			// Get number of characters.
			cchPassword = (WORD)SendDlgItemMessage(hDlg,
				IDC_EDIT2,
				EM_LINELENGTH,
				(WPARAM)0,
				(LPARAM)0);
			if (cchPassword >= 16)
			{
				MessageBox(hDlg,
					L"Too many characters.",
					L"Error",
					MB_OK);

				EndDialog(hDlg, TRUE);
				return FALSE;
			}
			else if (cchPassword == 0)
			{
				MessageBox(hDlg,
					L"No characters entered.",
					L"Error",
					MB_OK);

				EndDialog(hDlg, TRUE);
				return FALSE;
			}

			// Put the number of characters into first word of buffer.
			*((LPWORD)lpszPassword) = cchPassword;

			// Get the characters.
			SendDlgItemMessage(hDlg,
				IDC_EDIT2,
				EM_GETLINE,
				(WPARAM)0,       // line 0
				(LPARAM)lpszPassword);

			// Null-terminate the string.
			lpszPassword[cchPassword] = 0;

			if (_tcscmp(lpszPassword, TEXT("password")))
			{
				_stprintf_s(lpszPassword, 15, TEXT("%s"), TEXT("YES"));
				lpszPassword[cchPassword] = 0;
			}
			else
			{
				_stprintf_s(lpszPassword, 15, TEXT("%s"), TEXT("NO"));
				lpszPassword[cchPassword] = 0;
			}

			MessageBox(hDlg,
				lpszPassword,
				L"Did it work?",
				MB_OK);

			// Call a local password-parsing function.
			//ParsePassword(lpszPassword);

			EndDialog(hDlg, TRUE);
			return TRUE;

		case IDCANCEL:
			EndDialog(hDlg, TRUE);
			return TRUE;
		}
	}

	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK createAero(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	//Para o Parametro enviado janela
	LONG_PTR userdata = GetWindowLongPtr(hDlg, GWLP_USERDATA);
	ControlData *control = NULL;

	if (message == WM_INITDIALOG)
	{
		CREATESTRUCT* pCreate = (CREATESTRUCT*)(lParam);
		control = (ControlData*)pCreate->lpCreateParams;
		SetWindowLongPtr(hDlg, GWLP_USERDATA, (LONG_PTR)control);
	}
	else
	{
		control = (ControlData*)GetWindowLongPtr(hDlg, GWLP_USERDATA);
	}

	TCHAR aeronome[TAM], aeroX[TAM], aeroY[TAM];
	WORD cchaeronome, cchX, cchY;

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		control = (ControlData*)GetWindowLongPtr(hDlg, GWLP_USERDATA);

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		// Set the default push button to "OK" when the user enters text.
		if (HIWORD(wParam) == EN_CHANGE && LOWORD(wParam) == IDC_EDIT1)
		{
			SendMessage(hDlg,
				DM_SETDEFID,
				(WPARAM)IDOK,
				(LPARAM)0);
		}

		switch (wParam)
		{
		case IDOK:
			// Get number of characters.
			cchaeronome = (WORD)SendDlgItemMessage(hDlg,
				IDC_EDIT1,
				EM_LINELENGTH,
				(WPARAM)0,
				(LPARAM)0);

			if (cchaeronome >= TAM)
			{
				MessageBox(hDlg,
					L"Too many characters.",
					L"Error",
					MB_OK);

				EndDialog(hDlg, TRUE);
				return FALSE;
			}
			else if (cchaeronome == 0)
			{
				MessageBox(hDlg,
					L"No characters entered.",
					L"Error",
					MB_OK);

				EndDialog(hDlg, TRUE);
				return FALSE;
			}

			// Put the number of characters into first word of buffer.
			*((LPWORD)aeronome) = cchaeronome;

			// Get the characters.
			SendDlgItemMessage(hDlg,
				IDC_EDIT1,
				EM_GETLINE,
				(WPARAM)0,       // line 0
				(LPARAM)aeronome);

			// Null-terminate the string.
			aeronome[cchaeronome] = 0;

			int index = 0;

			if (control->shared->curAero > 0)
			{
				for (; index < control->shared->curAero; index++)
				{
					if (_tcscmp(aeronome, control->shared->map[index].aeroName) == 0)
					{
						MessageBox(hDlg,
							L"Aeroporto com este nome já existe.",
							L"Error",
							MB_OK);

						EndDialog(hDlg, TRUE);
						return FALSE;
					}
					else if (_tcscmp(control->shared->map[index].aeroName, TEXT("")) == 0)
					{
						break;
					}
				}

				int X, Y;

				// Get number of characters.
				cchX = (WORD)SendDlgItemMessage(hDlg,
					IDC_EDIT2,
					EM_LINELENGTH,
					(WPARAM)0,
					(LPARAM)0);

				SendDlgItemMessage(hDlg,
					IDC_EDIT2,
					EM_GETLINE,
					(WPARAM)0,       // line 0
					(LPARAM)aeroX);

				// Null-terminate the string.
				aeroX[cchX] = 0;

				// Get number of characters.
				cchY = (WORD)SendDlgItemMessage(hDlg,
					IDC_EDIT3,
					EM_LINELENGTH,
					(WPARAM)0,
					(LPARAM)0);

				SendDlgItemMessage(hDlg,
					IDC_EDIT3,
					EM_GETLINE,
					(WPARAM)0,       // line 0
					(LPARAM)aeroY);

				// Null-terminate the string.
				aeroY[cchY] = 0;

				X = _tstoi(aeroX);

				Y = _tstoi(aeroY);

				if ((X < 0 || X > MAP_TAM) || (Y < 0 || Y > MAP_TAM))
				{
					MessageBox(hDlg,
						L"Coordenadas inválidas!",
						L"Error",
						MB_OK);

					EndDialog(hDlg, TRUE);
					return FALSE;
				}
				//<###########################>//

				//<Verificação de Área de Distanciamento>//
				for (int i = 0; i < control->shared->curAero; i++)
				{
					int left_X = ((control->shared->map)[i].X - POS_AERO_AREA);

					int right_X = ((control->shared->map)[i].X + POS_AERO_AREA);

					int bottom_Y = ((control->shared->map)[i].Y - POS_AERO_AREA);

					int top_Y = ((control->shared->map)[i].Y + POS_AERO_AREA);

					if ((left_X < X && X < right_X) && (bottom_Y < Y && Y < top_Y))
					{
						MessageBox(hDlg,
							L"Coordenadas ocupadas!\nEspaço aéreo ocupado por outro Aeroporto.",
							L"Error",
							MB_OK);

						EndDialog(hDlg, TRUE);
						return FALSE;
					}
				}
				//<#####################################>//

				if (control->shared->curAero < control->shared->maxAero)
				{
					_stprintf_s((control->shared->map)[index].aeroName, TAM, TEXT("%s"), aeronome);

					(control->shared->map)[index].X = X;

					(control->shared->map)[index].Y = Y;

					(control->shared->curAero)++;
				}
				else
				{
					MessageBox(hDlg,
						L"Não é possível criar mais Aeroportos.\nLimite máximo alcançado.",
						L"Error",
						MB_OK);

					EndDialog(hDlg, TRUE);
					return FALSE;
				}
			}
			else
			{
				int X, Y;

				// Get number of characters.
				cchX = (WORD)SendDlgItemMessage(hDlg,
					IDC_EDIT2,
					EM_LINELENGTH,
					(WPARAM)0,
					(LPARAM)0);

				SendDlgItemMessage(hDlg,
					IDC_EDIT2,
					EM_GETLINE,
					(WPARAM)0,       // line 0
					(LPARAM)aeroX);

				// Null-terminate the string.
				aeroX[cchX] = 0;

				// Get number of characters.
				cchY = (WORD)SendDlgItemMessage(hDlg,
					IDC_EDIT3,
					EM_LINELENGTH,
					(WPARAM)0,
					(LPARAM)0);

				SendDlgItemMessage(hDlg,
					IDC_EDIT3,
					EM_GETLINE,
					(WPARAM)0,       // line 0
					(LPARAM)aeroY);

				// Null-terminate the string.
				aeroY[cchY] = 0;

				X = _tstoi(aeroX);

				Y = _tstoi(aeroY);

				if ((X < 0 || X > MAP_TAM) || (Y < 0 || Y > MAP_TAM))
				{
					MessageBox(hDlg,
						L"Coordenadas inválidas!",
						L"Error",
						MB_OK);

					EndDialog(hDlg, TRUE);
					return FALSE;
				}
				//<###########################>//

				_stprintf_s((control->shared->map)[index].aeroName, TAM, TEXT("%s"), aeronome);

				(control->shared->map)[index].X = X;

				(control->shared->map)[index].Y = Y;

				(control->shared->curAero)++;
			}

			MessageBox(hDlg,
				aeronome,
				L"Aeroporto adicionado com Sucesso!",
				MB_OK);

			EndDialog(hDlg, TRUE);
			return TRUE;

		case IDCANCEL:
			EndDialog(hDlg, TRUE);
			return TRUE;
		}
	}

	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK LinhaDeComandos(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			TCHAR comand[TAM];
			WORD cchcomand;

			// Get number of characters.
			cchcomand = (WORD)SendDlgItemMessage(hDlg,
				IDC_EDIT1,
				EM_LINELENGTH,
				(WPARAM)0,
				(LPARAM)0);

			if (cchcomand >= TAM)
			{
				MessageBox(hDlg,
					L"Too many characters.",
					L"Error",
					MB_OK);

				EndDialog(hDlg, TRUE);
			}
			else if (cchcomand == 0)
			{
				MessageBox(hDlg,
					L"No characters entered.",
					L"Error",
					MB_OK);

				EndDialog(hDlg, TRUE);
			}

			// Put the number of characters into first word of buffer.
			*((LPWORD)comand) = cchcomand;

			// Get the characters.
			SendDlgItemMessage(hDlg,
				IDC_EDIT1,
				EM_GETLINE,
				(WPARAM)0,       // line 0
				(LPARAM)comand);

			// Null-terminate the string.
			comand[cchcomand] = 0;

			/*for (int i = 0; i < _tcslen(comand) - 1; i++)
			{
				_puttch(comand[i]);
			}

			_puttch(TEXT('\n'));*/

			EndDialog(hDlg, TRUE);
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK infoPassag(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	//Para o Parametro enviado janela
	LONG_PTR userdata = GetWindowLongPtr(hDlg, GWLP_USERDATA);
	ControlData *control = NULL;
	TCHAR * output;

	if (message == WM_INITDIALOG)
	{
		CREATESTRUCT* pCreate = (CREATESTRUCT*)(lParam);
		control = (ControlData*)pCreate->lpCreateParams;
		SetWindowLongPtr(hDlg, GWLP_USERDATA, (LONG_PTR)control);
	}
	else
	{
		control = (ControlData*)GetWindowLongPtr(hDlg, GWLP_USERDATA);
	}

	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		control = (ControlData*)GetWindowLongPtr(hDlg, GWLP_USERDATA);

		HWND hwndList = GetDlgItem(hDlg, IDC_LIST1);

		if (control->curPass > 0)
		{
			for (int i = 0; i < control->curPass; i++)
			{
				output = malloc(sizeof(TCHAR) * 500);

				ZeroMemory(output, sizeof(TCHAR) * 500);

				_stprintf_s(output, 500, TEXT("Nome: %s"), control->Pass[i].nome);
				_stprintf_s(output, 500, TEXT("%s | Chegada / Destino: %s"), output, control->Pass[i].destino);
				_stprintf_s(output, 500, TEXT("%s | Partida / Atual: %s"), output, control->Pass[i].partida);
				control->Pass[i].voar ? _stprintf_s(output, 500, TEXT("%s | Estado: Em Viagem"), output) : _stprintf_s(output, 500, TEXT("%s | Estado: Em Espera"), output);
				if (control->Pass[i].tempo != INFINITE)
				{
					_stprintf_s(output, 500, TEXT("%s | Tempo de Espera: %d"), output, control->Pass[i].tempo);
				}

				int pos = (int)SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)output);

				SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM)i);

				free(output);
			}
		}
		else
		{
			output = malloc(sizeof(TCHAR) * 500);

			ZeroMemory(output, sizeof(TCHAR) * 500);

			_stprintf_s(output, 500, TEXT("%s"), TEXT("Não existem passageiros dentro do Sistema."));

			int pos = (int)SendMessage(hwndList, LB_ADDSTRING, 0,
				(LPARAM)output);

			SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM)0);

			free(output);
		}

		SetFocus(hwndList);

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK infoAero(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	//Para o Parametro enviado janela
	LONG_PTR userdata = GetWindowLongPtr(hDlg, GWLP_USERDATA);
	ControlData *control = NULL;
	TCHAR * output;

	if (message == WM_INITDIALOG)
	{
		CREATESTRUCT* pCreate = (CREATESTRUCT*)(lParam);
		control = (ControlData*)pCreate->lpCreateParams;
		SetWindowLongPtr(hDlg, GWLP_USERDATA, (LONG_PTR)control);
	}
	else
	{
		control = (ControlData*)GetWindowLongPtr(hDlg, GWLP_USERDATA);
	}

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		control = (ControlData*)GetWindowLongPtr(hDlg, GWLP_USERDATA);

		HWND hwndList = GetDlgItem(hDlg, IDC_LIST1);

		if (control->shared->curAero > 0)
		{
			for (int i = 0; i < control->shared->curAero; i++)
			{
				output = malloc(sizeof(TCHAR) * 700);

				ZeroMemory(output, sizeof(TCHAR) * 700);

				_stprintf_s(output, 700, TEXT("Aeroporto %s"), control->shared->map[i].aeroName);
				_stprintf_s(output, 700, TEXT("%s | Passageiros dentro do Aeroporto: %d"), output, control->shared->map[i].curPass);
				_stprintf_s(output, 700, TEXT("%s | Aviões dentro do Aeroporto: %d"), output, control->shared->map[i].curHang);
				_stprintf_s(output, 700, TEXT("%s | Posição do Aeroporto: [X=%3d,Y=%3d]"), output, control->shared->map[i].X, control->shared->map[i].Y);

				int pos = (int)SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)output);

				SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM)i);

				free(output);
			}
		}
		else
		{
			output = malloc(sizeof(TCHAR) * 500);

			ZeroMemory(output, sizeof(TCHAR) * 500);

			_stprintf_s(output, 500, TEXT("%s"), TEXT("Não existem aeroportos dentro do Sistema."));

			int pos = (int)SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)output);

			SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM)0);

			free(output);
		}

		SetFocus(hwndList);

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK infoPlane(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	//Para o Parametro enviado janela
	LONG_PTR userdata = GetWindowLongPtr(hDlg, GWLP_USERDATA);
	ControlData *control = NULL;
	TCHAR * output;

	if (message == WM_INITDIALOG)
	{
		CREATESTRUCT* pCreate = (CREATESTRUCT*)(lParam);
		control = (ControlData*)pCreate->lpCreateParams;
		SetWindowLongPtr(hDlg, GWLP_USERDATA, (LONG_PTR)control);
	}
	else
	{
		control = (ControlData*)GetWindowLongPtr(hDlg, GWLP_USERDATA);
	}

	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		control = (ControlData*)GetWindowLongPtr(hDlg, GWLP_USERDATA);

		HWND hwndList = GetDlgItem(hDlg, IDC_LIST1);

		if (control->shared->curPlane > 0)
		{
			for (int i = 0; i < control->shared->curPlane; i++)
			{
				output = malloc(sizeof(TCHAR) * 700);

				ZeroMemory(output, sizeof(TCHAR) * 700);

				_stprintf_s(output, 700, TEXT("ID: %d"), control->shared->planes[i].id);
				_stprintf_s(output, 700, TEXT("%s | Chegada / Destino: %s"), output, control->shared->planes[i].destino);
				_stprintf_s(output, 700, TEXT("%s | Partida / Atual: %s"), output, control->shared->planes[i].partida);
				_stprintf_s(output, 700, TEXT("%s | Velocidade: %d"), output, control->shared->planes[i].velocidade);
				control->shared->planes[i].voar ? _stprintf_s(output, 700, TEXT("%s | Estado: Em Voo"), output) : _stprintf_s(output, 700, TEXT("%s | Estado: Parado"), output);
				_stprintf_s(output, 700, TEXT("%s | Posição atual: [X=%3d,Y=%3d]"), output, control->shared->planes[i].X, control->shared->planes[i].Y);
				if (control->shared->planes[i].voar)
				{
					_stprintf_s(output, 700, TEXT("%s | Posição seguinte: [X=%3d,Y=%3d]"), output, control->shared->planes[i].next_X, control->shared->planes[i].next_Y);
				}
				!_tcslen(control->shared->planes[i].destino) ? _stprintf_s(output, 700, TEXT("%s"), output) : _stprintf_s(output, 700, TEXT("%s | Posição final: [X=%3d,Y=%3d]"), output, control->shared->planes[i].final_X, control->shared->planes[i].final_Y);
				_stprintf_s(output, 700, TEXT("%s | Nº de Passageiros embarcados: %d"), output, control->shared->planes[i].curPass);

				int pos = (int)SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)output);

				SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM)i);

				free(output);
			}
		}
		else
		{
			output = malloc(sizeof(TCHAR) * 700);

			ZeroMemory(output, sizeof(TCHAR) * 700);

			_stprintf_s(output, 700, TEXT("%s"), TEXT("Não existem aviões dentro do Sistema."));

			int pos = (int)SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)output);

			SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM)0);

			free(output);
		}

		SetFocus(hwndList);

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
