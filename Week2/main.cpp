#include <windows.h>
#include "game.h"
#include "CWindow.h"
#include "CPlayer.h"

#define PLAYER_TEXTURE_PATH "idleWalk1.png"

CPlayer* simon;
LPTEXTURE playerTexture;

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

    // Load texture cho Simon (Giả sử bạn có một hàm load texture như sau)
    playerTexture = new Texture();  // Khởi tạo texture cho Simon
    playerTexture->LoadFromFile(PLAYER_TEXTURE_PATH);  // Hàm LoadFromFile cần được định nghĩa trong CTexture

    // Khởi tạo Simon với vị trí và texture
    simon = new CPlayer(100, 100, 0, 0, playerTexture);

    // Game loop - cập nhật và vẽ Simon
    MSG msg;
    while (true) {
        // Xử lý thông báo hệ thống
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Cập nhật và vẽ Simon
        simon->HandleInput(keyLeft, keyRight, keyUp, keyDown);  // Cập nhật hướng di chuyển
        simon->Update(16);  // Cập nhật vị trí (thời gian delta giả sử là 16ms)
        simon->Render();    // Vẽ Simon lên màn hình
    }

    // Cleanup
    game->Cleanup();
    return 0;

    return 0;
}
