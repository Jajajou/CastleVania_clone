#pragma once
#include <Windows.h>
#include <d3dx10.h>
#include "Texture.h"

class CGameObject {
protected:
    float x, y;            // Position of the object
    LPTEXTURE texture;     // Texture to be used by the object

public:
    CGameObject(float x, float y, LPTEXTURE textureL);
    virtual void Update(DWORD dt);   // Virtual update method (to be overridden)
    virtual void Render();                // Method to render the object

    void SetPosition(float x, float y);
    float GetX() const;
    float GetY() const;

    ~CGameObject() {
        if (texture != NULL) delete texture; // Clean up the texture
    }
};
