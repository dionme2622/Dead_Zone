#include "pch.h"
#include "KeyInput.h"


void KeyInput::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_states.resize(KEY_TYPE_COUNT, KEY_STATE::NONE);
}

void KeyInput::Update()
{
    HWND hwnd = ::GetActiveWindow();
    if (_hwnd != hwnd)
    {
        for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
            _states[key] = KEY_STATE::NONE;
        return;
    }

    for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
    {
        KEY_STATE& state = _states[key];

        // 키가 현재 눌렸는지 확인
        bool keyPressed = (::GetAsyncKeyState(key) & 0x8000) != 0;

        if (keyPressed)
        {
            // 이전 프레임에 눌려 있었다면 PRESS 유지, 아니면 DOWN
            state = (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN) ? KEY_STATE::PRESS : KEY_STATE::DOWN;
        }
        else
        {
            // 이전 프레임에서 눌렸었다면 UP, 아니면 NONE
            state = (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN) ? KEY_STATE::UP : KEY_STATE::NONE;
        }
    }
}