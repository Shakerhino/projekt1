// projekt1.cpp : Definiuje punkt wejścia dla aplikacji.
//



#include "projekt1.h"
#include "stdafx.h"
#include <vector>
#include <cstdio>
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <string>

#define MAX_LOADSTRING 100

int timestep = 10;
// Zmienne globalne:
HINSTANCE hInst;                                // bieżące wystąpienie
WCHAR szTitle[MAX_LOADSTRING];                  // Tekst paska tytułu
WCHAR szWindowClass[MAX_LOADSTRING];            // nazwa klasy okna głównego
std::vector<float> Data;
int T = 1;//zmiana na osi czasu 
int A = 2;//amplituda
int n = 0;//ilosc pominietych probek
HWND g_hPrzycisk;
HWND g_hPrzycisk2, g_hPrzycisk3, g_hPrzycisk4;
HWND hText;
int z = 0;
HWND hwndButton;

// Przekaż dalej deklaracje funkcji dołączone w tym module kodu:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

RECT drawArea1 = { 0, 0, 1200, 400 };
RECT drawArea2 = { 0, 200, 200, 200 };

void inputData()
{
    float x;
    std::fstream plik;
    plik.open("outputCatapult01.txt", std::ios::in);
    int i = 0;
    if (plik.good() == true)
    {
        while (!plik.eof()) {
            plik >> x;
            if (i == 0) {
                Data.push_back(x);
            }
            if (i == 11)i = -1;
            i++;
        }
        plik.close();
    }
    else std::cout << "Dostep do pliku zostal zabroniony!" << std::endl;

}

int OnCreate(HWND window)
{
    inputData();
    return 0;
}

void MyOnPaint(HDC hdc)
{
    Graphics graphics(hdc);
    Pen pen(Color(255, 255, 255, 255));
    Pen pen2(Color(255, 0, 0, 0));
    inputData();
    int x = 0;//czas
    float a = Data.front();
    int y = 1000 * a;//zmienna koncowa 
    int z = 0;//zmienna poczatkowa czyli koncowa chwile wczesniej
    float q1 = 0, q2 = 0, q3 = 0, q4 = 0;
    if (Data.size() < n) {}//usuwanie n probek z wektora
    else {
        if (n > 0) {
            for (int i = 0; i < n; i++) {
                Data.erase(Data.begin());
            }
        }
    }
    while (!Data.empty()) {//rysowanie wykresu i kompasu
        x++;
        q1 = 1000 + 50 * sin(a * 3.14 / 180);
        q2 = 100 + 50 * cos(a * 3.14 / 180);
        q3 = 1000 - 50 * sin(a * 3.14 / 180);
        q4 = 100 - 50 * cos(a * 3.14 / 180);
        graphics.DrawLine(&pen2, T * (x), 100 - A * (z) / 2000, T * (x + 1), 100 - A * (y / 2000));//wykres
        graphics.DrawLine(&pen2, q1, q2, q3, q4); //kompas

        Sleep(timestep);
        a = Data.front();
        Data.erase(Data.begin());
        if (Data.size() > 0) {
            graphics.DrawLine(&pen, q1, q2, q3, q4); //kompas usuniecie poprzedniej 
        }

        z = y;
        y = 1000 * a;
    }

}

void repaintWindow(HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea)
{
    if (drawArea == NULL)
        InvalidateRect(hWnd, NULL, TRUE); // repaint all
    else
        InvalidateRect(hWnd, drawArea, TRUE); //repaint drawArea
    hdc = BeginPaint(hWnd, &ps);
    MyOnPaint(hdc);
    EndPaint(hWnd, &ps);
    if (Data.size() == 0)
    {
        //   KillTimer(hWnd, TMR_1);
    }
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Inicjuj ciągi globalne
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PROJEKT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Wykonaj inicjowanie aplikacji:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJEKT1));

    OnCreate(HWND(hInstance));
    MSG msg;

    // Główna pętla komunikatów:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    GdiplusShutdown(gdiplusToken);
    return (int)msg.wParam;
}



//
//  FUNKCJA: MyRegisterClass()
//
//  PRZEZNACZENIE: Rejestruje klasę okna.
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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJEKT1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PROJEKT1);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNKCJA: InitInstance(HINSTANCE, int)
//
//   PRZEZNACZENIE: Zapisuje dojście wystąpienia i tworzy okno główne
//
//   KOMENTARZE:
//
//        W tej funkcji dojście wystąpienia jest zapisywane w zmiennej globalnej i
//        jest tworzone i wyświetlane okno główne programu.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Przechowuj dojście wystąpienia w naszej zmiennej globalnej

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);


    g_hPrzycisk = CreateWindowEx(0, L"BUTTON", L"Generuj", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        100, 400, 150, 30, hWnd, NULL, hInstance, NULL);

    g_hPrzycisk2 = CreateWindowEx(0, L"BUTTON", L"reset ", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        100, 450, 150, 30, hWnd, NULL, hInstance, NULL);

    g_hPrzycisk3 = CreateWindowEx(0, L"BUTTON", L"Amplituda up ", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        300, 400, 150, 30, hWnd, NULL, hInstance, NULL);

    g_hPrzycisk4 = CreateWindowEx(0, L"BUTTON", L"Amplituda down ", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        300, 450, 150, 30, hWnd, NULL, hInstance, NULL);
    hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
        TEXT("przyspieszenie 5ms"),                  // the caption of the button
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
        500, 400,                                  // the left and top co-ordinates
        150, 30,                              // width and height
        hWnd,                                 // parent window handle
        (HMENU)ID_BUTTON1,                   // the ID of your button
        hInstance,                            // the instance of your application
        NULL);                               // extra bits you dont really need
    hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
        TEXT("spowolnienie 5ms"),                  // the caption of the button
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
        500, 450,                                  // the left and top co-ordinates
        150, 30,                              // width and height
        hWnd,                                 // parent window handle
        (HMENU)ID_BUTTON2,                   // the ID of your button
        hInstance,                            // the instance of your application
        NULL);                               // extra bits you dont really need
    hText = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
        WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL, 800, 400, 200, 50, hWnd, NULL, hInstance, NULL);

    SetWindowText(hText, L"Ilosc probek bledu (liczba)");



    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNKCJA: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PRZEZNACZENIE: Przetwarza komunikaty dla okna głównego.
//
//  WM_COMMAND  - przetwarzaj menu aplikacji
//  WM_PAINT    - Maluj okno główne
//  WM_DESTROY  - opublikuj komunikat o wyjściu i wróć
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    switch (message)
    {
    case WM_COMMAND:
    {
        if ((HWND)lParam == g_hPrzycisk)
        {
            repaintWindow(hWnd, hdc, ps, &drawArea1);
        }
        if ((HWND)lParam == g_hPrzycisk2)
        {
            A = 2;
            repaintWindow(hWnd, hdc, ps, &drawArea1);
        }
        if ((HWND)lParam == g_hPrzycisk3)
        {
            A = 3;
            Sleep(100);
            repaintWindow(hWnd, hdc, ps, &drawArea1);
        }
        if ((HWND)lParam == g_hPrzycisk4)
        {
            if (A < 0.4) {}
            else {
                A = 1;
            }
            repaintWindow(hWnd, hdc, ps, &drawArea1);
        }
        int wmId = LOWORD(wParam);
        // Analizuj zaznaczenia menu:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case ID_BUTTON1:
            timestep = timestep - 2;
            repaintWindow(hWnd, hdc, ps, &drawArea1);
            // SetTimer(hWnd, TMR_1, 30, 0);

            break;
        case ID_BUTTON2:
            timestep = timestep + 5;
            repaintWindow(hWnd, hdc, ps, &drawArea1);
            //  SetTimer(hWnd, TMR_1, 30, 0);
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        MyOnPaint(hdc);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
        // case WM_TIMER:
          //  switch (wParam)
       //     {
       //    case TMR_1: // OgĂłlne rysowanie ruchĂłw
         //      SetTimer(hWnd, TMR_1, 30, NULL);
         //      break;
         //  }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Procedura obsługi komunikatów dla okna informacji o programie.
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
