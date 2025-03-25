#include "game.h"
#include <comdef.h>
#include <windows.h>

int game::backBufferWidth = 1280;   // Define the variable once
int game::backBufferHeight = 720;   // Define the variable once

game* game::__instance = nullptr;

game* game::GetInstance() {
    if (__instance == nullptr) {
        __instance = new game();
    }
    return __instance;
}

void game::Init(HWND hWnd) {
    this->hWnd = hWnd;

    RECT r;
    GetClientRect(hWnd, &r);
    backBufferWidth = r.right - r.left;
    backBufferHeight = r.bottom - r.top;

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = backBufferWidth;
    swapChainDesc.BufferDesc.Height = backBufferHeight;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;

    HRESULT hr = D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION,
        &swapChainDesc, &pSwapChain, &pD3DDevice);
    if (hr != S_OK) {
        _com_error err(hr);
        LPCTSTR errMsg = err.ErrorMessage();
        MessageBox(NULL, errMsg, L"Error", MB_OK);
        return;
    }

    ID3D10Texture2D* pBackBuffer;
    hr = pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);
    if (hr != S_OK) {
        MessageBox(NULL, L"Failed to get buffer", L"Error", MB_OK);
        return;
    }

    hr = pD3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
    pBackBuffer->Release();

    if (hr != S_OK) {
        MessageBox(NULL, L"Failed to create render target view", L"Error", MB_OK);
        return;
    }

    pD3DDevice->OMSetRenderTargets(1, &pRenderTargetView, NULL);

    D3D10_VIEWPORT viewPort = {};
    viewPort.Width = backBufferWidth;
    viewPort.Height = backBufferHeight;
    viewPort.MinDepth = 0.0f;
    viewPort.MaxDepth = 1.0f;
    viewPort.TopLeftX = 0;
    viewPort.TopLeftY = 0;
    pD3DDevice->RSSetViewports(1, &viewPort);

    hr = D3DX10CreateSprite(pD3DDevice, 0, &spriteObject);
    if (hr != S_OK) {
        MessageBox(NULL, L"Failed to create sprite object", L"Error", MB_OK);
        return;
    }
}

void game::LoadResources() {
    // For now, no resources to load (e.g., textures or models)
}

void game::Cleanup() {
    if (pRenderTargetView) pRenderTargetView->Release();
    if (pSwapChain) pSwapChain->Release();
    if (pD3DDevice) pD3DDevice->Release();
    if (spriteObject) spriteObject->Release();
}

void game::Run() {
    // Placeholder for game loop, will be implemented later in the `CGameLoop` class
}

void game::Draw(float x, float y, LPTEXTURE texture) {
    if (texture == NULL) return;

    D3DXMATRIX matTranslation;
    D3DXMatrixTranslation(&matTranslation, x, y, 0.0f);

    D3DX10_SPRITE sprite;
    ZeroMemory(&sprite, sizeof(D3DX10_SPRITE));

    sprite.pTexture = texture->getShaderResourceView();   // SRV từ texture
    sprite.TexCoord = D3DXVECTOR2(0.0f, 0.0f);            // Vẽ từ góc trên trái
    sprite.TexSize = D3DXVECTOR2(1.0f, 1.0f);             // Vẽ toàn bộ texture
    sprite.ColorModulate = D3DXCOLOR(1, 1, 1, 1);         // Không đổi màu
    sprite.TextureIndex = 0;
    sprite.matWorld = matTranslation;

    spriteObject->DrawSpritesImmediate(&sprite, 1, 0, 0);
}

int game::GetBackBufferWidth() const {
    return backBufferWidth;
}

int game::GetBackBufferHeight() const {
	return backBufferHeight;
}
