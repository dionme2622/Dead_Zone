#pragma once
#include "Camera.h"
#include "Player.h"
class MouseInput
{
    DECLARE_SINGLE(MouseInput);
public:
    void Init(HWND hwnd);
    void Update(Player& obj);
        

private:
    POINT center;
    HWND _hwnd;

    bool _mouseMove;
};
