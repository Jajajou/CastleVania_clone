#include <windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <comdef.h>

// Link với các thư viện cần thiết
#pragma comment(lib, "d3d10.lib")

#define WINDOW_CLASS_NAME L"SampleWindow"
#define WINDOW_TITLE L"week1"
#define WINDOW_ICON_PATH L"Castlevania.ico"

// Màu nền (R,G,B,A), giá trị 0.0f ~ 1.0f
#define BACKGROUND_COLOR D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f)

// Kích thước cửa sổ
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// Frame rate tối đa
#define MAX_FRAME_RATE 60

// Đường dẫn texture nhân vật
#define TEXTURE_PATH_CHARACTER L"idleStanding.png"
#define TEXTURE_PATH_CHARACTER2 L"idleWalk.png"

// Vận tốc di chuyển (điều chỉnh cho phù hợp)
#define CHARACTER_SPEED 0.3f

HWND hWnd = 0;
ID3D10Device* pD3DDevice = NULL;
IDXGISwapChain* pSwapChain = NULL;
ID3D10RenderTargetView* pRenderTargetView = NULL;
ID3DX10Sprite* spriteObject = NULL;

int BackBufferWidth = 1280;
int BackBufferHeight = 720;

// Biến toàn cục về texture + sprite
ID3D10Texture2D* texCharacter = NULL;
D3DX10_SPRITE spriteCharacter;
ID3D10Texture2D* texCharacter2 = NULL;
D3DX10_SPRITE spriteCharacter2;

// Tọa độ nhân vật
float char_x = 100.0f;
float char_y = 200.0f;
float char2_x = 300.0f;
float char2_y = 200.0f;
float char2_vx = 0.1f;  // tốc độ đi ngang

// Biến lưu trạng thái phím
bool keyLeft = false;
bool keyRight = false;
bool keyUp = false;
bool keyDown = false;

// Hàm xử lý Window messages
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_LEFT:  keyLeft = true; break;
        case VK_RIGHT: keyRight = true; break;
        case VK_UP:    keyUp = true; break;
        case VK_DOWN:  keyDown = true; break;
        }
        break;

    case WM_KEYUP:
        switch (wParam)
        {
        case VK_LEFT:  keyLeft = false; break;
        case VK_RIGHT: keyRight = false; break;
        case VK_UP:    keyUp = false; break;
        case VK_DOWN:  keyDown = false; break;
        }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

// DEBUG SUPPORT
void DebugOut(const wchar_t* fmt, ...)
{
    wchar_t s[4096];
    va_list argp;
    va_start(argp, fmt);
    vswprintf_s(s, fmt, argp);
    va_end(argp);
    OutputDebugString(s);
}

void DebugOutTitle(const wchar_t* fmt, ...)
{
    wchar_t s[1024];
    va_list argp;
    va_start(argp, fmt);
    vswprintf_s(s, fmt, argp);
    va_end(argp);
    SetWindowText(hWnd, s);
}

// Khởi tạo DirectX
void InitDirectX(HWND hWnd)
{
    RECT r;
    GetClientRect(hWnd, &r);
    BackBufferWidth = r.right + 1;
    BackBufferHeight = r.bottom + 1;

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = BackBufferWidth;
    swapChainDesc.BufferDesc.Height = BackBufferHeight;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;

    HRESULT hr = D3D10CreateDeviceAndSwapChain(
        NULL,
        D3D10_DRIVER_TYPE_HARDWARE,
        NULL,
        0,
        D3D10_SDK_VERSION,
        &swapChainDesc,
        &pSwapChain,
        &pD3DDevice
    );

    if (hr != S_OK)
    {
        _com_error err(hr);
        LPCTSTR errMsg = err.ErrorMessage();
        DebugOut(L"[ERROR] D3D10CreateDeviceAndSwapChain failed: %s\n", errMsg);
        return;
    }

    ID3D10Texture2D* pBackBuffer;
    hr = pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);
    if (hr != S_OK)
    {
        DebugOut(L"[ERROR] pSwapChain->GetBuffer failed\n");
        return;
    }

    hr = pD3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
    pBackBuffer->Release();
    if (hr != S_OK)
    {
        DebugOut(L"[ERROR] CreateRenderTargetView failed\n");
        return;
    }

    pD3DDevice->OMSetRenderTargets(1, &pRenderTargetView, NULL);

    D3D10_VIEWPORT viewPort;
    viewPort.Width = BackBufferWidth;
    viewPort.Height = BackBufferHeight;
    viewPort.MinDepth = 0.0f;
    viewPort.MaxDepth = 1.0f;
    viewPort.TopLeftX = 0;
    viewPort.TopLeftY = 0;
    pD3DDevice->RSSetViewports(1, &viewPort);

    // Tạo spriteObject
    hr = D3DX10CreateSprite(pD3DDevice, 0, &spriteObject);
    if (hr != S_OK)
    {
        DebugOut(L"[ERROR] D3DX10CreateSprite failed\n");
        return;
    }

    // Thiết lập ma trận chiếu
    D3DXMATRIX matProjection;
    D3DXMatrixOrthoOffCenterLH(
        &matProjection,
        (float)viewPort.TopLeftX,
        (float)viewPort.Width,
        (float)viewPort.TopLeftY,
        (float)viewPort.Height,
        0.1f,
        10
    );
    spriteObject->SetProjectionTransform(&matProjection);

    DebugOut(L"[INFO] InitDirectX thành công\n");
}

// Tải tài nguyên (sprite nhân vật)
void LoadResources()
{
	// Tải texture nhân vật dieu khien duoc
    ID3D10Resource* pD3D10Resource = NULL;
    HRESULT hr = D3DX10CreateTextureFromFile(
        pD3DDevice,
        TEXTURE_PATH_CHARACTER,
        NULL,
        NULL,
        &pD3D10Resource,
        NULL
    );

    if (FAILED(hr))
    {
        DebugOut(L"[ERROR] Không load được texture: %s\n", TEXTURE_PATH_CHARACTER);
        return;
    }

    pD3D10Resource->QueryInterface(__uuidof(ID3D10Texture2D), (LPVOID*)&texCharacter);
    pD3D10Resource->Release();

    if (!texCharacter)
    {
        DebugOut(L"[ERROR] Chuyển ID3D10Resource sang ID3D10Texture2D thất bại\n");
        return;
    }

    D3D10_TEXTURE2D_DESC desc;
    texCharacter->GetDesc(&desc);

    D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    ZeroMemory(&SRVDesc, sizeof(SRVDesc));
    SRVDesc.Format = desc.Format;
    SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = desc.MipLevels;

    ID3D10ShaderResourceView* charTextureRV = NULL;
    pD3DDevice->CreateShaderResourceView(texCharacter, &SRVDesc, &charTextureRV);

    // Thiết lập spriteCharacter
    spriteCharacter.pTexture = charTextureRV;
    spriteCharacter.TexCoord.x = 0;
    spriteCharacter.TexCoord.y = 0;
    spriteCharacter.TexSize.x = 1.0f;
    spriteCharacter.TexSize.y = 1.0f;
    spriteCharacter.TextureIndex = 0;
    spriteCharacter.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    DebugOut(L"[INFO] Đã load texture nhân vật 1 thành công\n");

	// Tải texture nhân vật 2
    ID3D10Resource* pD3D10Resource2 = NULL;
    HRESULT hr2 = D3DX10CreateTextureFromFile(pD3DDevice, TEXTURE_PATH_CHARACTER2, NULL, NULL, &pD3D10Resource2, NULL);
    pD3D10Resource2->QueryInterface(__uuidof(ID3D10Texture2D), (LPVOID*)&texCharacter2);
    pD3D10Resource2->Release();

    // Tạo Shader Resource View cho nhân vật 2
    D3D10_TEXTURE2D_DESC desc2;
    texCharacter2->GetDesc(&desc2);

    D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc2 = {};
    SRVDesc2.Format = desc2.Format;
    SRVDesc2.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    SRVDesc2.Texture2D.MipLevels = desc2.MipLevels;

    ID3D10ShaderResourceView* charTextureRV2 = NULL;
    pD3DDevice->CreateShaderResourceView(texCharacter2, &SRVDesc2, &charTextureRV2);

    // Thiết lập spriteCharacter2
    spriteCharacter2.pTexture = charTextureRV2;
    spriteCharacter2.TexCoord = D3DXVECTOR2(0, 0);
    spriteCharacter2.TexSize = D3DXVECTOR2(1, 1);
    spriteCharacter2.TextureIndex = 0;
    spriteCharacter2.ColorModulate = D3DXCOLOR(1, 1, 1, 1);


    DebugOut(L"[INFO] Đã load texture nhân vật 2 thành công\n");
}

// Update logic: đọc trạng thái phím và di chuyển nhân vật
void Update(DWORD dt)
{
    float distance = CHARACTER_SPEED * dt; // quãng đường di = vận tốc * thời gian frame

    if (keyLeft)  char_x -= distance;
    if (keyRight) char_x += distance;
    if (keyUp)    char_y -= distance;
    if (keyDown)  char_y += distance;

    // Giới hạn để nhân vật không chạy ra ngoài cửa sổ
    if (char_x < 0) char_x = 0;
    if (char_x > (BackBufferWidth - 16))  // giả sử nhân vật rộng 16px
        char_x = (float)(BackBufferWidth - 16);

    if (char_y < 0) char_y = 0;
    if (char_y > (BackBufferHeight - 32)) // giả sử nhân vật cao 32px
        char_y = (float)(BackBufferHeight - 32);

	// Di chuyển nhân vật 2
	char2_x += char2_vx * dt;
    if (char2_x < 0 || char2_x > BackBufferWidth - 17) {
        char2_vx *= -1;
    }
}

// Vẽ 1 frame
void Render()
{
    if (pD3DDevice == NULL) return;

    // Xoá màn hình với màu nền
    pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);

    // Bắt đầu vẽ
    spriteObject->Begin(D3DX10_SPRITE_SORT_TEXTURE);

    // Tạo ma trận tịnh tiến cho nhân vật
    D3DXMATRIX matTranslation, matTranslation2;
    D3DXMatrixTranslation(&matTranslation, char_x, (float)(BackBufferHeight - char_y), 0.1f);
    D3DXMatrixTranslation(&matTranslation2, char2_x, (float)(BackBufferHeight - char2_y), 0.1f);

    // Nếu nhân vật 16x32, ta scale như sau
    D3DXMATRIX matScaling, matScaling2;
    D3DXMatrixScaling(&matScaling, 2 * 16.0f, 2 * 32.0f, 1.0f);
    D3DXMatrixScaling(&matScaling2, 2 * 16.0f, 2 * 32.0f, 1.0f);

    // Gán ma trận world
    spriteCharacter.matWorld = (matScaling * matTranslation);
	spriteCharacter2.matWorld = (matScaling2 * matTranslation2);    

    spriteObject->DrawSpritesImmediate(&spriteCharacter, 1, 0, 0);
	spriteObject->DrawSpritesImmediate(&spriteCharacter2, 1, 0, 0);

    spriteObject->End();

    // Lật buffer
    pSwapChain->Present(0, 0);

    // Hiển thị vị trí lên title (debug)
    //DebugOutTitle(L"%s (x=%.1f, y=%.1f)", WINDOW_TITLE, char_x, char_y);
}

// Tạo cửa sổ Win32
HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hInstance = hInstance;
    wc.lpfnWndProc = (WNDPROC)WinProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = WINDOW_CLASS_NAME;
    wc.hIconSm = NULL;

    RegisterClassEx(&wc);

    HWND hWnd = CreateWindow(
        WINDOW_CLASS_NAME,
        WINDOW_TITLE,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        ScreenWidth,
        ScreenHeight,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        DWORD ErrCode = GetLastError();
        DebugOut(L"[ERROR] CreateWindow failed! ErrCode: %d\n", ErrCode);
        return 0;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return hWnd;
}

// Vòng lặp game
int Run()
{
    MSG msg;
    int done = 0;
    ULONGLONG frameStart = GetTickCount64();
    ULONGLONG tickPerFrame = 1000 / MAX_FRAME_RATE;

    while (!done)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) done = 1;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        ULONGLONG now = GetTickCount64();
        ULONGLONG dt = now - frameStart;

        if (dt >= tickPerFrame)
        {
            frameStart = now;
            Update((DWORD)dt);
            Render();
        }
        else
        {
            Sleep((DWORD)(tickPerFrame - dt));
        }
    }
    return 1;
}

// Dọn dẹp
void Cleanup()
{
    if (pRenderTargetView) { pRenderTargetView->Release(); }
    if (pSwapChain) { pSwapChain->Release(); }
    if (pD3DDevice) { pD3DDevice->Release(); }
    if (spriteObject) { spriteObject->Release(); spriteObject = NULL; }
    DebugOut(L"[INFO] Cleanup hoàn tất\n");
}

// Hàm main
int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    hWnd = CreateGameWindow(hInstance, nCmdShow, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (hWnd == 0) return 0;

    InitDirectX(hWnd);
    LoadResources();
    Run();
    Cleanup();

    return 0;
}
