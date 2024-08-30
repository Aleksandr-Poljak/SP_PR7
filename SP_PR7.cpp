// SP_PR7.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "SP_PR7.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

HWND hButtonText;                               // Кнопка Текст
HWND hButtonDrawShape;                          // Кнопка Рисовать
HWND hButtonImage;                              // Кнопка Изображение
HWND hButtonTextMetafile;                       // Кнопка воспроизведения  метафайла
HMETAFILE hmf = NULL;                           

HBITMAP hBitmap;
HDC memBit;
HDC hdc;
BITMAP bm;


bool textFlag = FALSE;
bool shapeFlag = FALSE;
bool textFlagMetafile = FALSE;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SPPR7, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPPR7));

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

    return (int)msg.wParam;
}

//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPPR7));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SPPR7);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

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

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        LPCWSTR metafileName = L"output.wmf";
        HDC hDCMetafile = CreateMetaFile(metafileName);
        if (hDCMetafile != NULL)
        {
            HFONT hFont = CreateTaskFont();
            SelectObject(hDCMetafile, hFont);
            LPCWSTR text = _T("Поляк Александр Александрович. Вывод из метафайла.");
            SetTextColor(hDCMetafile, RGB(255, 0, 0));
            SetBkMode(hDCMetafile, TRANSPARENT);
            TextOut(hDCMetafile, 20, 450, text, lstrlen(text));
            DeleteObject(hFont);
            hmf = CloseMetaFile(hDCMetafile);
        }
        
        hButtonTextMetafile = CreateWindowEx(0L, TEXT("BUTTON"), TEXT("Текст из метафайла"),
            WS_CHILD | WS_BORDER | WS_VISIBLE,
            30, 20, 140, 24, hWnd,
            (HMENU)IDC_BTN_TEXT_IN_METAFILE, hInst, NULL);
        if (hButtonTextMetafile == 0) return -1;

        hButtonText = CreateWindowEx(0L, TEXT("BUTTON"), TEXT("Текст"),
            WS_CHILD | WS_BORDER | WS_VISIBLE,
            30, 60, 80, 24, hWnd,
            (HMENU)IDC_BTN_TEXT, hInst, NULL);
        if (hButtonText == 0) return -1;

        hButtonDrawShape = CreateWindowEx(0L, TEXT("BUTTON"), TEXT("Рисовать"),
            WS_CHILD | WS_BORDER | WS_VISIBLE,
            30, 100, 80, 24, hWnd,
            (HMENU)IDC_BTN_DRAW, hInst, NULL);
        if (hButtonDrawShape == 0) return -1;

        hButtonImage = CreateWindowEx(0L, TEXT("BUTTON"), TEXT("Изображение"),
            WS_CHILD | WS_BORDER | WS_VISIBLE,
            30, 140, 110, 24, hWnd,
            (HMENU)IDC_BTN_IMAGE, hInst, NULL);
        if (hButtonImage == 0) return -1;
        return 0;
        break;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDC_BTN_TEXT:
        {
            textFlag = TRUE;
            InvalidateRect(hWnd, nullptr, TRUE);
            break;
        }
        case IDC_BTN_TEXT_IN_METAFILE:
        {
            textFlagMetafile = TRUE;
            InvalidateRect(hWnd, nullptr, TRUE);
            break;
        }
        case IDC_BTN_DRAW:
            shapeFlag = TRUE;
            InvalidateRect(hWnd, nullptr, TRUE);
            break;
        case IDC_BTN_IMAGE:          
        {
            hdc = GetDC(hWnd);
            hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
            GetObject(hBitmap, sizeof(bm), &bm);
            memBit = CreateCompatibleDC(hdc);
            SelectObject(memBit, hBitmap);
            int targetWidth = 100;
            int targetHeight = 100;

            RECT clientRect;
            GetClientRect(hWnd, &clientRect);

            int x = (clientRect.right - targetWidth) / 2;
            int y = (clientRect.bottom - targetHeight) / 2;

            StretchBlt(hdc, x, y, targetWidth, targetHeight, memBit, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

            DeleteDC(memBit);
            DeleteObject(hBitmap);
            ReleaseDC(hWnd, hdc);
            break;
        }
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
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // Отображаем текст
        if (textFlag)
        {
            HFONT hFont = CreateTaskFont();
            SelectObject(hdc, hFont);
            LPCWSTR text = _T("Поляк Александр Александрович.");
            SetTextColor(hdc, RGB(255, 0, 0));
            SetBkMode(hdc, TRANSPARENT);
            TextOut(hdc, 300, 450, text, lstrlen(text));
            DeleteObject(hFont);
            textFlag = FALSE;
        }
        if (textFlagMetafile)
        {
            if (hmf != NULL)
            { 
                PlayMetaFile(hdc, hmf); 
                textFlagMetafile = FALSE;
            }
        }
        // Рисуем звезду
        if (shapeFlag)
        {
            DrawStarFullShading(hdc, 600, 200, 80);
            DrawStarTopsShading(hdc, 400, 200, 80);
            shapeFlag = FALSE;
        }

        EndPaint(hWnd, &ps);
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

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK ||
            LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
