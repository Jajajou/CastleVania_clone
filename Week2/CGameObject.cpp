#include "CGameObject.h"
#include "Texture.h"

CGameObject::CGameObject(float x, float y, LPTEXTURE texture) {
	this->x = x;
	this->y = y;
	this->texture = texture;
}

void CGameObject::Update(DWORD dt) {
	// Do nothing by default
}

void CGameObject::SetPosition(float x, float y) {
	this->x = x;
	this->y = y;
}

float CGameObject::GetX() const {
	return x;
}

float CGameObject::GetY() const {
	return y;
}	
