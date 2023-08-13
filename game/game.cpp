// game.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "game.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

HBITMAP wall = NULL;
HBITMAP box = NULL;

int mx = 24;
int my = 24;

int mapdata[25][25]
{
    { 1,1,1,1,1 },
    { 1,0,0,0,1 },
    { 1,0,0,0,1 },
    { 1,0,0,0,1 },
    { 1,1,1,1,1 }
};

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        0, 0, 3840, 2160, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

void DrawSprite(RECT* prc, HBITMAP g_hbmBall, HDC hdc, int x, int y)
{
    HDC hdcBuffer = CreateCompatibleDC(hdc);
    HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, prc->right, prc->bottom);
    HBITMAP hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, g_hbmBall);

    FillRect(hdcBuffer, prc, (HBRUSH)GetStockObject(WHITE_BRUSH));

    BitBlt(hdcBuffer, x, y, 24, 24, hdcMem, 0, 0, SRCAND);

    SelectObject(hdcMem, g_hbmBall);
    BitBlt(hdcBuffer, x, y, 24, 24, hdcMem, 0, 0, SRCPAINT);

    BitBlt(hdc, 0, 0, prc->right, prc->bottom, hdcBuffer, 0, 0, SRCCOPY);

    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);

    SelectObject(hdcBuffer, hbmOldBuffer);
    DeleteDC(hdcBuffer);
    DeleteObject(hbmBuffer);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_DESTROY) {
        DeleteObject(wall);
        DeleteObject(box);

        PostQuitMessage(0);
    }
    else if (uMsg == WM_PAINT) {
        RECT rcClient;
        PAINTSTRUCT ps;
        HDC h_dc = BeginPaint(hWnd, &ps);

        GetClientRect(hWnd, &rcClient);
        DrawSprite(&rcClient, box, h_dc, 24, 24);

        for (size_t y = 0; y < 28; y++)
        {
            for (size_t x = 0; x < 22; x++)
            {
                if (mapdata[y][x] == 1)
                {
                    HDC hdcMem = CreateCompatibleDC(h_dc);
                    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, wall);

                    BitBlt(h_dc, x * 24, y * 24, 24, 24, hdcMem, 0, 0, SRCCOPY);

                    SelectObject(hdcMem, hbmOld);
                    DeleteDC(hdcMem);
                }
            }
        }
        EndPaint(hWnd, &ps);
    }
    else if (uMsg == WM_CREATE) {
        BITMAP bm;

        wall = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1));
        if (wall == NULL)
            MessageBoxA(hWnd, "Could not load IDB_BITMAP1!", "Error", MB_ICONEXCLAMATION);

        GetObject(wall, sizeof(bm), &bm);

        box = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP2));
        if (box == NULL)
            MessageBoxA(hWnd, "Could not load IDB_BITMAP1!", "Error", MB_ICONEXCLAMATION);

        GetObject(box, sizeof(bm), &bm);
    }
    else if (uMsg == WM_KEYDOWN) {
        RECT rcClient;
        HDC hdc = GetDC(hWnd);

        if (wParam == VK_RIGHT) {
            if (mapdata[(my / 24)][mx / 24 + 1] == 0)
            {
                GetClientRect(hWnd, &rcClient);

                mx += 24;
                DrawSprite(&rcClient, box, hdc, mx, my);
            }
        }
        if (wParam == VK_LEFT) {
            if (mapdata[my / 24][mx / 24 - 1] == 0)
            {
                GetClientRect(hWnd, &rcClient);

                mx -= 24;
                DrawSprite(&rcClient, box, hdc, mx, my);
            }
        }
        if (wParam == VK_DOWN) {
            if (mapdata[my / 24 + 1][mx / 24] == 0)
            {
                GetClientRect(hWnd, &rcClient);

                my += 24;
                DrawSprite(&rcClient, box, hdc, mx, my);
            }
        }
        if (wParam == VK_UP) {
            if (mapdata[my / 24 - 1][mx / 24] == 0)
            {
                GetClientRect(hWnd, &rcClient);

                my -= 24;
                DrawSprite(&rcClient, box, hdc, mx, my);
            }
        }
        for (size_t y = 0; y < 28; y++)
        {
            for (size_t x = 0; x < 22; x++)
            {
                if (mapdata[y][x] == 1)
                {
                    HDC hdcMem = CreateCompatibleDC(hdc);
                    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, wall);

                    BitBlt(hdc, x * 24, y * 24, 24, 24, hdcMem, 0, 0, SRCCOPY);

                    SelectObject(hdcMem, hbmOld);
                    DeleteDC(hdcMem);
                }
            }
        }

        ReleaseDC(hWnd, hdc);
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS wc;

    wchar_t my_class_name[] = L"미로게임";
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = my_class_name;
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wc);

    HWND hWnd = CreateWindow(my_class_name, L"미로게임",
        WS_OVERLAPPEDWINDOW, 100, 90, 400, 350, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}