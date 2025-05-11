#include "pch.h"
#include "CameraScript.h"
#include "PlayerScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "KeyInput.h"
#include "MouseInput.h"
#include "Engine.h"

CameraScript::CameraScript(HWND hwnd) :
_hwnd(hwnd)
{
}

void CameraScript::FinalUpdate()
{
	UpdateMouseInput();
}

void CameraScript::UpdateMouseInput()
{
	if (INPUT->GetButtonDown(KEY_TYPE::Q))
		_mouseMove = !_mouseMove;

	POINT mousePos;
	SetCapture(_hwnd);
	GetCursorPos(&mousePos);
	ScreenToClient(_hwnd, &mousePos);

	POINT center = { WINDOW.width / 2, WINDOW.height / 2 };
	float deltaX = (float)(mousePos.x - center.x);
	float deltaY = (float)(mousePos.y - center.y);


	if (_mouseMove == false)
	{
		if (deltaX != 0 || deltaY != 0)
		{
			UpdateRotation(deltaX, deltaY);
		}

		// 마우스 위치를 중앙으로 고정
		POINT screenCenter = { center.x, center.y };
		ClientToScreen(_hwnd, &screenCenter);
		SetCursorPos(screenCenter.x, screenCenter.y);
	}
}

void CameraScript::UpdateRotation(float deltaX, float deltaY)
{
	// X축 회전 (Pitch, 위아래) - 카메라에만 적용
	_pitch += deltaY * sensitivity;
	_pitch = max(-90 * XM_PI / 180, min(90 * XM_PI / 180, _pitch));

	// Y축 회전 (Yaw, 좌우) - 캐릭터와 카메라 모두에 적용
	_yaw += deltaX * sensitivity;

	rotation.x = _pitch * 50;
	rotation.y = _yaw * 50;
	rotation.z = 0.0;

	GetTransform()->SetLocalRotation(rotation);
}

