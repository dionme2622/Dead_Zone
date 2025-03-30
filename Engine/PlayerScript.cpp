#include "pch.h"
#include "PlayerScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "KeyInput.h"
#include "MouseInput.h"
#include "Timer.h"
#include "Engine.h"
#include "BoxCollider.h"
#include "SceneManager.h"

PlayerScript::PlayerScript(HWND hwnd, shared_ptr<Transform> playerTransform) :
	_hwnd(hwnd), _speed(100.0f), _jumpVelocity(5.0f), _currentVelocity(0.0f), 
	_gravity(-9.8f), _isGrounded(true), _pitch(0.0f), _yaw(0.0f), _mouseMove(false), 
	_cameraTransform(playerTransform)
{
}

PlayerScript::~PlayerScript()
{
}

void PlayerScript::LateUpdate()
{
	UpdatePlayerInput();

	//UpdatePlayerOnTerrain();


	// ī�޶� ��ġ ������Ʈ
	Vec3 cameraPosition = GetTransform()->GetLocalPosition();
	_cameraTransform->SetLocalPosition(cameraPosition);

	Vec3 cameraRotate = GetTransform()->GetLocalRotation();
	_cameraTransform->SetLocalRotation(cameraRotate);
}


void PlayerScript::UpdatePlayerInput()
{
	UpdateKeyInput();

	UpdateMouseInput();
}

void PlayerScript::UpdateKeyInput()
{
	Vec3 pos = GetTransform()->GetLocalPosition();
	Vec3 scale = GetTransform()->GetLocalScale();

	float adjustedSpeed = _speed / scale.x; // �����Ͽ� ���� �ӵ��� ����

	if (INPUT->GetButton(KEY_TYPE::W))
		pos += GetTransform()->GetLook() * adjustedSpeed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::S))
		pos -= GetTransform()->GetLook() * adjustedSpeed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::A))
		pos -= GetTransform()->GetRight() * adjustedSpeed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::D))
		pos += GetTransform()->GetRight() * adjustedSpeed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::CTRL))
		pos -= GetTransform()->GetUp() * adjustedSpeed * DELTA_TIME;

	if (_isGrounded && INPUT->GetButtonDown(KEY_TYPE::SPACE))
	{
		_currentVelocity = _jumpVelocity;
		pos.y += _currentVelocity * DELTA_TIME;
		_isGrounded = false;
	}

	GetTransform()->SetLocalPosition(pos);


}


void PlayerScript::UpdateMouseInput()
{
	if (INPUT->GetButtonDown(KEY_TYPE::SHIFT))
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

		// ���콺 ��ġ�� �߾����� ����
		POINT screenCenter = { center.x, center.y };
		ClientToScreen(_hwnd, &screenCenter);
		SetCursorPos(screenCenter.x, screenCenter.y);

	}

	// ī�޶� ��ġ ������Ʈ
	
}




void PlayerScript::UpdateRotation(float deltaX, float deltaY)
{
	// X�� ȸ�� (Pitch, ���Ʒ�)
	_pitch += deltaY * sensitivity;
	_pitch = max(-90 * XM_PI / 180, min(90 * XM_PI / 180, _pitch));

	_yaw += deltaX * sensitivity;

	rotation.x = _pitch * 50;
	rotation.y = _yaw * 50;
	rotation.z = 0.0;

	GetTransform()->SetLocalRotation(rotation);
}

void PlayerScript::UpdatePlayerOnTerrain()
{
	Vec3 pos = GetTransform()->GetLocalPosition();
	float terrainHeight = -130.f;  // �����ؾߵ�

	if (pos.y <= terrainHeight)
	{
		pos.y = terrainHeight;
		_currentVelocity = 0.0f;
		_isGrounded = true;
	}
	else
	{
		_currentVelocity -= _gravity * DELTA_TIME * 100;
		pos.y += _currentVelocity * DELTA_TIME;
		_isGrounded = false;
	}
	GetTransform()->SetLocalPosition(pos);
}






