#include <windows.h>
#include "game.h"
#include "CWindow.h"
#include "CPlayer.h"

CPlayer* simon;

bool keyLeft = false, keyRight = false, keyUp = false, keyDown = false;

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        switch (wParam) {
        case VK_LEFT: keyLeft = true; break;
        case VK_RIGHT: keyRight = true; break;
        case VK_UP: keyUp = true; break;
        case VK_DOWN: keyDown = true; break;
        }
        break;

    case WM_KEYUP:
        switch (wParam) {
        case VK_LEFT: keyLeft = false; break;
        case VK_RIGHT: keyRight = false; break;
        case VK_UP: keyUp = false; break;
        case VK_DOWN: keyDown = false; break;
        }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    CWindow window(1280, 720);
    HWND hWnd = window.CreateWindowInstance(hInstance, nCmdShow);

    game* game = game::GetInstance();
    game->Init(hWnd);

    // Run game loop (empty for now)
    game->Run();

    // Cleanup
    game->Cleanup();

    return 0;
}
