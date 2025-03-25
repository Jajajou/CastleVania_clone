#pragma once
#include "CGameObject.h"

class CPlayer : public CGameObject {
private:
    float vx, vy;  // Vận tốc di chuyển theo trục X và Y

public:
    // Constructor để khởi tạo player với vị trí ban đầu và vận tốc
    CPlayer(float x, float y, float vx, float vy, LPTEXTURE texture)
        : CGameObject(x, y, texture), vx(vx), vy(vy) {
    }

    // Phương thức cập nhật vị trí của player
    void Update(DWORD dt) override;

    // Phương thức vẽ player lên màn hình
    void Render() override;

    // Phương thức xử lý input từ bàn phím
    void HandleInput(bool keyLeft, bool keyRight, bool keyUp, bool keyDown);
};
