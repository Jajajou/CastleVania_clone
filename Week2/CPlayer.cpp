#include "CPlayer.h"
#include "game.h"

// Cập nhật vị trí của player dựa trên vận tốc (velocity)
void CPlayer::Update(DWORD dt) {
    x += vx * dt;  // Di chuyển theo trục X
    y += vy * dt;  // Di chuyển theo trục Y

    // Giới hạn để player không ra ngoài màn hình
    int screenWidth = game::GetInstance()->GetBackBufferWidth();
    int screenHeight = game::GetInstance()->GetBackBufferHeight();

    // Đảm bảo player không ra ngoài màn hình
    if (x < 0) x = 0;
    if (x >= screenWidth - 50) x = screenWidth - 16;  
    if (y < 0) y = 0;
    if (y >= screenHeight - 50) y = screenHeight - 32;  // Giới hạn kích thước player 16x32
}

// Vẽ player lên màn hình
void CPlayer::Render() {
    game::GetInstance()->Draw(x, y, texture);
}

// Xử lý input từ bàn phím
void CPlayer::HandleInput(bool keyLeft, bool keyRight, bool keyUp, bool keyDown) {
    // Điều khiển di chuyển trái/phải
    if (keyLeft) {
        vx = -0.1f;  // Di chuyển sang trái
    }
    else if (keyRight) {
        vx = 0.1f;   // Di chuyển sang phải
    }
    else {
        vx = 0.0f;   // Không di chuyển nếu không bấm trái hoặc phải
    }

    // Điều khiển di chuyển lên/xuống
    if (keyUp) {
        vy = -0.1f;  // Di chuyển lên
    }
    else if (keyDown) {
        vy = 0.1f;   // Di chuyển xuống
    }
    else {
        vy = 0.0f;   // Không di chuyển nếu không bấm lên hoặc xuống
    }
}
