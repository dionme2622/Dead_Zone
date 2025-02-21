#include "pch.h"
#include "Engine.h"
#include "MouseInput.h"
#include "KeyInput.h"

void MouseInput::Init(HWND hwnd)
{
    _hwnd = hwnd;
    ClientToScreen(hwnd, &center);
    ScreenToClient(_hwnd, &center);

    center.x /= 2;
    center.y /= 2;
}

void MouseInput::Update(Player& player)
{
    if (INPUT->GetButtonDown(KEY_TYPE::SHIFT)) _mouseMove = !_mouseMove;

    POINT mousePos;
    SetCapture(_hwnd);
    GetCursorPos(&mousePos);
    ScreenToClient(_hwnd, &mousePos);

    POINT center = { WINDOW.width / 2, WINDOW.height / 2 };

    float deltaX = (float)(mousePos.x - center.x);
    float deltaY = (float)(mousePos.y - center.y);

    if (_mouseMove == false) {

    if (deltaX != 0 || deltaY != 0)
    {
        player.UpdateRotation(deltaX, deltaY);
    }

        POINT screenCenter = { center.x, center.y };
        ClientToScreen(_hwnd, &screenCenter);
        SetCursorPos(screenCenter.x, screenCenter.y);
    }
}



