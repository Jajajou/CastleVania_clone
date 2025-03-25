#pragma once
#include <d3d10.h>
#include <d3dx10.h>

class Texture {
protected:
    ID3D10Texture2D* _tex;  // The texture itself
    ID3D10ShaderResourceView* _rsview;  // Shader Resource View for the texture
    int _width, _height;  // Width and height of the texture

public:
    Texture();  // Constructor declaration
    Texture(ID3D10Texture2D* tex, ID3D10ShaderResourceView* rsview);  // Constructor declaration with arguments
    ~Texture();  // Destructor declaration

    ID3D10ShaderResourceView* getShaderResourceView();
    int getWidth() { return _width; }
    int getHeight() { return _height; }

    bool LoadFromFile(const char* filePath);  // Hàm load texture từ file
};

typedef Texture* LPTEXTURE;
