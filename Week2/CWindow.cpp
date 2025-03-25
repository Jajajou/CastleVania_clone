#include "CWindow.h"

HWND CWindow::CreateWindowInstance(HINSTANCE hInstance, int nCmdShow) {
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hInstance = hInstance;
    wc.lpfnWndProc = DefWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"GameWindowClass";
    wc.hIconSm = NULL;

    RegisterClassEx(&wc);

    hWnd = CreateWindow(
        L"GameWindowClass",
        L"Game Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        NULL, NULL, hInstance, NULL
    );

    if (!hWnd) {
        DWORD errCode = GetLastError();
        MessageBox(NULL, L"Failed to create window", L"Error", MB_OK);
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return hWnd;
}

void CWindow::SetWindowTitle(const wchar_t* title) {
    SetWindowText(hWnd, title);
}

