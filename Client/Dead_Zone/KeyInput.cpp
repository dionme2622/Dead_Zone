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

        // Ű�� ���� ���ȴ��� Ȯ��
        bool keyPressed = (::GetAsyncKeyState(key) & 0x8000) != 0;

        if (keyPressed)
        {
            // ���� �����ӿ� ���� �־��ٸ� PRESS ����, �ƴϸ� DOWN
            state = (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN) ? KEY_STATE::PRESS : KEY_STATE::DOWN;
        }
        else
        {
            // ���� �����ӿ��� ���Ⱦ��ٸ� UP, �ƴϸ� NONE
            state = (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN) ? KEY_STATE::UP : KEY_STATE::NONE;
        }
    }
}