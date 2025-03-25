#pragma once
#include <windows.h>

class CWindow {
private:
    HWND hWnd;
    const int width;
    const int height;

public:
    CWindow(int w, int h) : width(w), height(h), hWnd(NULL) {}

    HWND CreateWindowInstance(HINSTANCE hInstance, int nCmdShow);
    void SetWindowTitle(const wchar_t* title);
    HWND GetWindowHandle() const { return hWnd; }
};
