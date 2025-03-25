#pragma once
#include <d3d10.h>
#include <d3dx10.h>

class CTexture {
protected:
    ID3D10Texture2D* _tex;  // The texture itself
    ID3D10ShaderResourceView* _rsview;  // Shader Resource View for the texture
    int _width, _height;  // Width and height of the texture

public:
    CTexture() : _tex(NULL), _rsview(NULL), _width(-1), _height(-1) {}

    CTexture(ID3D10Texture2D* tex, ID3D10ShaderResourceView* rsview)
        : _tex(tex), _rsview(rsview) {
        D3D10_TEXTURE2D_DESC desc;
        _tex->GetDesc(&desc);
        _width = desc.Width;
        _height = desc.Height;
    }

    ID3D10ShaderResourceView* getShaderResourceView() { return _rsview; }
    int getWidth() { return _width; }
    int getHeight() { return _height; }

    ~CTexture() {
        if (_rsview) _rsview->Release();
        if (_tex) _tex->Release();
    }
};

typedef CTexture* LPTEXTURE;
