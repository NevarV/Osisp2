// lab2.cpp: Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "lab2.h"
#include "iostream"

#define MAX_LOADSTRING 100
#define INDENT 5

HINSTANCE hInst;                               
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
TCHAR buf[8096], tempBuf[8096];
int M = 4, N = 2;
int oldM = M, oldN = N;
int textLen = 0;
int oneBoxCount = 0;
int modFromCount = 0;
int minHeight = 500;
int fullCount = 0;
bool flag = true;
DWORD dwTemp;
int coloumnWidth = 0;
int rowHeight = 0;
int widthCount = 0;
LOGFONT lf = { 20 , 7 , 0 };
HFONT hFont;
bool isFont = false;
TCHAR* buffer;
int charWidth = 10;
int minWidth = 15;
int sign = 0;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	FILE * ptrFile = fopen("D:\\Учёба\\3-й Курс\\5-ый сем\\ОСиСП\\Osisp3\\Osisp3\\TextSource\\text.txt", "rb");
	fseek(ptrFile, 0, SEEK_END);                          // устанавливаем позицию в конец файла
	long lSize = ftell(ptrFile);                            // получаем размер в байтах
	rewind(ptrFile);                                       // устанавливаем указатель в конец файла
	char* tempBuffer = (char*)malloc((lSize + 1) * sizeof(char));
	buffer = (LPWSTR)malloc(sizeof(wchar_t)*(lSize+1)); // выделить память для хранения содержимого файла
	size_t result = fread(tempBuffer, 1, lSize, ptrFile);       // считываем файл в буфер*/
	swprintf(tempBuf, lSize + 1, L"%hs", tempBuffer);
	int I = 1;
	buf[0] = tempBuf[0];
	for (int i = 1; i < _tcslen(tempBuf); i++)
	{
		if (tempBuf[i]!='\n')
			buf[I++] = tempBuf[i];
	}
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB2));
    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT clientRect = {0};
	PAINTSTRUCT ps;
	HDC hdc = NULL;


    switch (message)
    {
    case WM_PAINT:
        {			
			hdc = BeginPaint(hWnd, &ps);
			SelectObject(hdc, hFont);
			GetClientRect(hWnd, &clientRect);
			LPTSTR pTarget = NULL, pSource = &(buf[0]);
			pTarget = &(buf[0]);
			int len = _tcslen(&(buf[0]));
			oneBoxCount = len > (M * N) ? len / (M * N) : 1;
			modFromCount = len % (M * N);
			int fullcount = 0;
			_tcscpy(pTarget, pSource);
			Rectangle(hdc, INDENT, INDENT, clientRect.right - INDENT, clientRect.bottom - INDENT);
			
			coloumnWidth = (clientRect.right - 2 * INDENT) / N;
			rowHeight = (clientRect.bottom - 2 * INDENT) / M;

			for (int i = 1; i < N; i++)
			{
				MoveToEx(hdc, INDENT + coloumnWidth * i, INDENT, NULL);
				LineTo(hdc, INDENT + coloumnWidth * i, clientRect.bottom - INDENT);
			}
			
			widthCount = coloumnWidth / lf.lfWidth;
			int HeightPlus = 0;
			int savedBoxCount = oneBoxCount;
			for (int j = 0; j < M; j++)//цикл по строкам
			{
				int oldHeightPlus = 0;
				int heightPlus;
				for (int i = 0; i < N; i++)//цикл по столбам
				{					
					if ((modFromCount > 0) && (len > (M * N)))
					{
						oneBoxCount = savedBoxCount + 1;
						modFromCount--;
					}
					else
						oneBoxCount = savedBoxCount;

					heightPlus = 0;
					if (fullcount < len)
					{
						int count = 0;
						while (count != oneBoxCount)
						{
							if ((oneBoxCount - count) * lf.lfWidth <= coloumnWidth)
							{
								TextOut(hdc, INDENT + (coloumnWidth - (oneBoxCount - count) * lf.lfWidth) / 2 + 2 + i * coloumnWidth, INDENT + 5 + HeightPlus + heightPlus, pTarget, oneBoxCount - count);
								
								pTarget += oneBoxCount - count;
								fullcount += oneBoxCount - count;
								count = oneBoxCount;
							}
							else
							{
								TextOut(hdc, INDENT + (coloumnWidth - widthCount  * lf.lfWidth) / 2 + 2 + i * coloumnWidth, INDENT + 5 + HeightPlus + heightPlus, pTarget, widthCount);
								count += widthCount;
								pTarget += widthCount;
								fullcount += widthCount;
							}
							heightPlus += lf.lfHeight;
						}					
					}
					else
					{
						if (i == 0)
							heightPlus += lf.lfHeight;
					}
					if (heightPlus > oldHeightPlus)
						oldHeightPlus = heightPlus;
				}
				HeightPlus += oldHeightPlus;
				if (j != M - 1)
				{
					MoveToEx(hdc, INDENT, INDENT + HeightPlus, NULL);
					LineTo(hdc, clientRect.right - INDENT, INDENT + HeightPlus);
				}
				RECT wndRect = {0};
				GetWindowRect(hWnd, &wndRect);
				GetClientRect(hWnd, &clientRect);
				minHeight = HeightPlus + 2 * INDENT + 65;
			}
			
			EndPaint(hWnd, &ps);
			if (flag) 
			{				
				flag = false;
				RECT wndRect = { 0 };
				GetWindowRect(hWnd, &wndRect);
				MoveWindow(hWnd, wndRect.left, wndRect.top, wndRect.right - wndRect.left, minHeight, TRUE);
			}
			if (oldM != M || oldN != N)
			{
				RECT wndRect = { 0 };
				GetWindowRect(hWnd, &wndRect);
				MoveWindow(hWnd, wndRect.left, wndRect.top, wndRect.right - wndRect.left, minHeight, TRUE);
				oldM = M;
				oldN = N;
			}
			if (isFont)
			{
				isFont = false;
				RECT wndRect = { 0 };
				GetWindowRect(hWnd, &wndRect);
				MoveWindow(hWnd, wndRect.left, wndRect.top, wndRect.right - wndRect.left, minHeight, TRUE);
			}
        }
        break;
	case WM_SIZE:
	{
		RECT wndRect = { 0 };
		GetWindowRect(hWnd, &wndRect);
		MoveWindow(hWnd, wndRect.left, wndRect.top, wndRect.right - wndRect.left, minHeight, TRUE);
		if(wndRect.right - wndRect.left < 2 * INDENT + N * minWidth)
			MoveWindow(hWnd, wndRect.left, wndRect.top, 2 * INDENT + N * minWidth, wndRect.bottom - wndRect.top, TRUE);
	}
		break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
			case VK_UP:
				if (M > 1)
				{
					M--;
					InvalidateRect(hWnd, NULL, TRUE);
				}
				break;
			case VK_DOWN:
				M++;
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			case VK_LEFT:
				if (N > 1)
					N--;
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			case VK_RIGHT:
				GetClientRect(hWnd, &clientRect);
				if ((clientRect.right - clientRect.left) / ++N > minWidth)
					InvalidateRect(hWnd, NULL, TRUE);
				else
					N--;
				break;
		}
	}		
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
