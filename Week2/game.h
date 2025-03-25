#pragma once
#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include "Texture.h"

#pragma comment(lib, "d3d10.lib")

class game {
private:
    HWND hWnd;
    int backBufferWidth, backBufferHeight;
    ID3D10Device* pD3DDevice;
    IDXGISwapChain* pSwapChain;
    ID3D10RenderTargetView* pRenderTargetView;
    ID3DX10Sprite* spriteObject;
	static game* __instance;
    static int backBufferWidth, backBufferHeight;

public:
    // Singleton Pattern
    static game* GetInstance();

    void Init(HWND hWnd);                // Initialize DirectX and the game
    void LoadResources();               // Load any necessary resources (e.g., textures)
    void Cleanup();                     // Release resources
    void Run();                         // Start the game loop
	void Draw(float x, float y, LPTEXTURE texture);  // Draw a texture to the screen
    int GetBackBufferWidth() const;
	int GetBackBufferHeight() const;

    ID3D10Device* GetDirect3DDevice() { return pD3DDevice; }
    IDXGISwapChain* GetSwapChain() { return pSwapChain; }
    ID3D10RenderTargetView* GetRenderTargetView() { return pRenderTargetView; }
    ID3DX10Sprite* GetSpriteHandler() { return spriteObject; }
};
