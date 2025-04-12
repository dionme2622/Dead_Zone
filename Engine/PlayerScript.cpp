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
#include "BattleScene.h"

PlayerScript::PlayerScript(HWND hwnd, shared_ptr<Transform> playerTransform) :
	_hwnd(hwnd), _speed(300.0f), _jumpVelocity(10.0f), _currentVelocity(0.0f), 
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

	Vec3 playerPosition = GetTransform()->GetLocalPosition();
	Vec3 cameraPosition = playerPosition;
	_cameraTransform->SetLocalPosition(cameraPosition);

	// ������
	{
		Vec3 pos = _cameraTransform->GetLocalPosition();
		pos += GetTransform()->GetUp() * 3;
		pos -= GetTransform()->GetLook() * 10.f;
		_cameraTransform->SetLocalPosition(pos);
	}
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

	// �� ���͸� ��������, Y ������ �����Ͽ� ���� ���⸸ ����
	Vec3 lookVector = GetTransform()->GetLook();
	Vec3 flatLookVector = Vec3(lookVector.x, 0.0f, lookVector.z); // Y ������ 0���� ����
	flatLookVector.Normalize(); // ����ȭ�Ͽ� ���⸸ ����

	// ������ ���͵� ���� �������� ����
	Vec3 rightVector = GetTransform()->GetRight();
	Vec3 flatRightVector = Vec3(rightVector.x, 0.0f, rightVector.z);
	flatRightVector.Normalize();

	if (INPUT->GetButton(KEY_TYPE::W))
		pos += flatLookVector * adjustedSpeed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::S))
		pos -= flatLookVector * adjustedSpeed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::A))
		pos -= flatRightVector * adjustedSpeed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::D))
		pos += flatRightVector * adjustedSpeed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::CTRL))
		pos -= GetTransform()->GetUp() * adjustedSpeed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::SPACE))
		pos += GetTransform()->GetUp() * adjustedSpeed * DELTA_TIME;

	if (BattleScene::isPlayerGrounded && INPUT->GetButtonDown(KEY_TYPE::SPACE))
	{
		_currentVelocity = _jumpVelocity;
		pos.y += _currentVelocity * DELTA_TIME;
		BattleScene::isPlayerGrounded = false;
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
}




void PlayerScript::UpdateRotation(float deltaX, float deltaY)
{
	// X�� ȸ�� (Pitch, ���Ʒ�) - ī�޶󿡸� ����
	_pitch += deltaY * sensitivity;
	_pitch = max(-90 * XM_PI / 180, min(90 * XM_PI / 180, _pitch));

	// Y�� ȸ�� (Yaw, �¿�) - ĳ���Ϳ� ī�޶� ��ο� ����
	_yaw += deltaX * sensitivity;

	// �÷��̾�� Yaw ȸ���� ���� (Pitch�� ����)
	//Vec3 rotation = Vec3(0.0f, _yaw * 50, 0.0f); // Pitch�� 0���� ����
	//GetTransform()->SetLocalRotation(rotation);
	
	// ����׿�
	{
		Vec3 rotation = Vec3(_pitch * 50, _yaw * 50, 0.0f); 
		GetTransform()->SetLocalRotation(rotation);
	}

	// ī�޶�� Pitch�� Yaw ��� ����
	Vec3 cameraRotation = Vec3(_pitch * 50, _yaw * 50, 0.0f); // Pitch�� Yaw ����
	_cameraTransform->SetLocalRotation(cameraRotation);
}

void PlayerScript::UpdatePlayerOnTerrain()
{
	Vec3 pos = GetTransform()->GetLocalPosition();


	if (BattleScene::isPlayerGrounded)
	{
		_currentVelocity = 0.0f;
	}
	else
	{
		_currentVelocity += _gravity * DELTA_TIME;
		pos.y += _currentVelocity * DELTA_TIME;
	}
	GetTransform()->SetLocalPosition(pos);
}