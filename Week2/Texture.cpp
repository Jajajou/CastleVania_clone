#include "Texture.h"
#include <d3d10.h>
#include <d3dx10.h>
#include "game.h"

Texture::Texture() : _tex(NULL), _rsview(NULL), _width(-1), _height(-1) {}

Texture::Texture(ID3D10Texture2D* tex, ID3D10ShaderResourceView* rsview)
    : _tex(tex), _rsview(rsview) {
    D3D10_TEXTURE2D_DESC desc;
    tex->GetDesc(&desc);
    _width = desc.Width;
    _height = desc.Height;
}

Texture::~Texture() {
    if (_rsview) _rsview->Release();
    if (_tex) _tex->Release();
}

ID3D10ShaderResourceView* Texture::getShaderResourceView() {
    return _rsview;  // Return the Shader Resource View
}

bool Texture::LoadFromFile(const char* filePath) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, filePath, -1, NULL, 0);
    wchar_t* wideFilePath = new wchar_t[size_needed];
    MultiByteToWideChar(CP_UTF8, 0, filePath, -1, wideFilePath, size_needed);

    ID3D10Resource* pD3D10Resource = NULL;
    HRESULT hr = D3DX10CreateTextureFromFile(
        game::GetInstance()->GetDirect3DDevice(),
        wideFilePath,
        NULL,
        NULL,
        &pD3D10Resource,
        NULL
    );

    if (FAILED(hr)) {
        return false;  // Return false if the texture could not be loaded
    }

    // Convert ID3D10Resource to ID3D10Texture2D
    hr = pD3D10Resource->QueryInterface(__uuidof(ID3D10Texture2D), (LPVOID*)&_tex);
    pD3D10Resource->Release();

    if (FAILED(hr)) {
        return false;  // Return false if conversion fails
    }

    // Create Shader Resource View for texture
    D3D10_TEXTURE2D_DESC desc;
    _tex->GetDesc(&desc);

    D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    ZeroMemory(&SRVDesc, sizeof(SRVDesc));
    SRVDesc.Format = desc.Format;
    SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = desc.MipLevels;

    hr = game::GetInstance()->GetDirect3DDevice()->CreateShaderResourceView(_tex, &SRVDesc, &_rsview);

    if (FAILED(hr)) {
        return false;  // Return false if SRV creation fails
    }

    return true;  // Return true if texture is successfully loaded
}

