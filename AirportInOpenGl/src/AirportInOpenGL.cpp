// AirportInOpenGL.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "AirportInOpenGL.h"
#include <windowsx.h >
#include "mmsystem.h"	// For game loop timer
#include <tchar.h>
#include "Game.h"
#include "Keyboard.h"

#define MAX_LOADSTRING 100
// Message for requesting a new frame
#define WM_GAMEFRAME (WM_USER + 1)

// Global Variables:
HINSTANCE		hInst;								// current instance
WCHAR			szTitle[MAX_LOADSTRING];			// The title bar text
WCHAR			szWindowClass[MAX_LOADSTRING];		// The main window class name
RECT			rect;
HWND			hWnd;								// An identifier for the window to draw on 
Game			game;								// Holds a scenes that can be rendered
unsigned int	mmTimerDelay = 1000 / 50;			//50Hz = 20ms per frame
unsigned int	mmTimerId;
bool			mmTimerInitialised = false;			// If the timer has been started
POINT currMousePos;
POINT lastMousePos;
POINT resetMousePos;
POINT resultPos;
													// keyboard state stored here 256 bits = 32 unsigned chars
static unsigned char keyState[32]{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static unsigned char masks[8]{ 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Options(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Menu(HWND, UINT, WPARAM, LPARAM);
void CALLBACK		GetTimerEvent(UINT IDEvent, UINT reserved, DWORD dwUser, DWORD	dwReserved1, DWORD	dwReserved2);
void DealWithKeyPress(HWND hWnd, WPARAM wParam, bool isKeydown);
bool InitialiseMMTimer();
void ShutdownMMTimer();



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	Log::AppendToLogFileWithDate("====== Game started ======");
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_AIRPORTINOPENGL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
	if (!InitialiseMMTimer())
	{
		return FALSE;
	}


	// INFO: this is where the mouse will reappear if outside the safe zone
	resetMousePos.x = (rect.left + rect.right) / 2;
	resetMousePos.y = (rect.top + rect.bottom) / 2;

	// INFO: the initial difference should be 0
	currMousePos.x = resetMousePos.x;
	currMousePos.y = resetMousePos.y;
	lastMousePos = currMousePos;

	// INFO: the result is affected by the mouse movement - you would rotate the camera instead
	resultPos.x = 0;
	resultPos.y = 0;

	POINT temp = currMousePos;
	ClientToScreen(hWnd, &temp);
	SetCursorPos(temp.x, temp.y);
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_AIRPORTINOPENGL));

    MSG msg;

    // Main message loop:

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_AIRPORTINOPENGL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_AIRPORTINOPENGL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;				// Handle to device context
    switch (message)
    {
	case WM_GAMEFRAME:
		game.Update();
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		break;
	case WM_CREATE:
		// here is where we create our open gl context
		hdc = GetDC(hWnd);
		GetClientRect(hWnd, &rect);
		game.CreateGLWindow(hdc, rect, hWnd);

		// prepare vbo's and vao's and setup shader
		game.PrepareToDraw();
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		game.Draw();
		SwapBuffers(hdc);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_SIZE:
		GetClientRect(hWnd, &rect);
		hdc = GetDC(hWnd);
		// pass to opengl
		game.Resize(hdc, rect);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case IDM_CONTROLS:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_CONTROLSBOX), hWnd, About);
				break;
			case IDM_OPTIONS:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_OPTIONSBOX), hWnd, Options);
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_KEYDOWN:
		// handle esc press
		if (wParam == VK_ESCAPE)
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ESCMENU), hWnd, Menu);
		}
		Keyboard::setKeyDown(short(wParam));
		break;
	case WM_MOUSEMOVE:
		game.HandleMouse();
		break;
	
	case WM_KEYUP:
		Keyboard::setKeyUp(short(wParam));
		break;

    case WM_DESTROY:
        PostQuitMessage(0);
		Log::AppendToLogFileWithDate("====== Game exited ======\n\n");
        break;
	default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for options box.
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

// Message handler for menu box.
INT_PTR CALLBACK Menu(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDRESUME || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			POINT center;
			center.x = rect.right / 2;
			center.y = rect.bottom / 2;
			ClientToScreen(hWnd, &center);
			SetCursorPos(center.x, center.y);
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDQUIT)
		{
			PostQuitMessage(0);
			Log::AppendToLogFileWithDate("====== Game exited ======\n\n");
		}

		else if (LOWORD(wParam) == IDM_OPTIONS)
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_OPTIONSBOX), hWnd, Options);
		}
		else if (LOWORD(wParam) == IDM_CONTROLS)
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_CONTROLSBOX), hWnd, About);
		}
		else if (LOWORD(wParam) == IDM_ABOUT)
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		}
		else if (LOWORD(wParam) == IDRELOAD)
		{
			game.reloadScene();
			EndDialog(hDlg, LOWORD(wParam));
			POINT center;
			center.x = rect.right / 2;
			center.y = rect.bottom / 2;
			ClientToScreen(hWnd, &center);
			SetCursorPos(center.x, center.y);
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK Options(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			wchar_t Text[300] = { 0 };
			GetWindowText(GetDlgItem(hDlg, IDC_EDIT1), Text, 300);
			try {
				game.setSensitivity(stof(Text));
			}
			catch(...){}
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

bool InitialiseMMTimer()
{
	TIMECAPS	tc;

	timeGetDevCaps(&tc, sizeof(TIMECAPS));

	if ((mmTimerDelay < tc.wPeriodMin) || (mmTimerDelay > tc.wPeriodMax))
	{
		return false;
	}
	timeBeginPeriod(mmTimerDelay);

	mmTimerId = timeSetEvent(mmTimerDelay,		//another flash
		mmTimerDelay,
		GetTimerEvent,
		(DWORD)hWnd,
		TIME_PERIODIC);		//TIME_ONESHOT

	if (mmTimerId)			//returns null if fails...
	{
		mmTimerInitialised = true;
	}
	return mmTimerInitialised;
}

void ShutdownMMTimer()
{
	MMRESULT	err;
	if (mmTimerInitialised)
	{
		err = timeKillEvent(mmTimerId);

		if (err != TIMERR_NOERROR)
		{
			OutputDebugString(L"MM Timer shutdown error\n");
			if (err == MMSYSERR_INVALPARAM)
			{
				OutputDebugString(L"Invalid Paramters\n");
			}
			else
			{
				OutputDebugString(L"Unknown error\n");
			}
		}
		mmTimerInitialised = false;
	}
	err = timeEndPeriod(mmTimerDelay);

	if (err != TIMERR_NOERROR)
	{
		OutputDebugString(L"Timer end period error\n");
	}
}

void CALLBACK GetTimerEvent(UINT IDEvent, UINT reserved, DWORD dwUser,			//we store hwnd here
	DWORD	dwReserved1,
	DWORD	dwReserved2)

	//LPTIMECALLBACK GetTimerEvent(UINT IDEvent, UINT reserved, DWORD dwUser,			//we store hwnd here
	//	DWORD	dwReserved1,
	//	DWORD	dwReserved2)

{
	UNREFERENCED_PARAMETER(IDEvent);
	UNREFERENCED_PARAMETER(reserved);
	UNREFERENCED_PARAMETER(dwUser);
	UNREFERENCED_PARAMETER(dwReserved1);		//ams warnings level 4
	UNREFERENCED_PARAMETER(dwReserved2);

	PostMessage((HWND)dwUser, WM_GAMEFRAME, 0, 0);
}


void	DealMouseMove(HWND hWnd, WPARAM wParam)
{
	
}

static bool IsKeyPressed(unsigned char keyCode)
{
	// we could test keys state here...
	// keyCode is in 0-0xFF  - see ms VK_ codes.
	int keyByte = (keyCode & 0xF8) >> 3;
	unsigned char keyMask = masks[(keyCode & 0x07)];
	return ((keyState[keyByte] & keyMask) != 0);
}