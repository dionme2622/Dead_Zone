#include "pch.h"
#include "PlayerScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "KeyInput.h"
#include "MouseInput.h"
#include "Timer.h"
#include "Engine.h"
#include "WeaponManager.h"
#include "Animator.h"

PlayerScript::PlayerScript(HWND hwnd, bool isLocal, int playerId)
{
	_hwnd = hwnd;
	_isLocal = isLocal;
	_playerId = playerId;
	// Player�� ���� ���� �ʱ�ȭ �ܰ�

	_speed = 5.0f;
	_jumpVelocity = 500.0f;
	_currentVelocity = 0.0f;
	_gravity = 9.8f;
	_isGrounded = true;
}

PlayerScript::~PlayerScript()
{
}

void PlayerScript::LateUpdate()
{
	

	//UpdatePlayerOnTerrain();

	if (_isLocal)
	{
		// 1) ���� �Է� �о �� ĳ���� ������ ��� �� Ŭ���̾�Ʈ ����
		UpdatePlayerInput();
	}
	else
	{
		// 2) ���� �÷��̾�: ��Ʈ��ũ ���¸� �޾Ƽ� Transform�� ����
		// TODO : ���⼭ �ٸ� �÷��̾� ��ü���� pos ���� �޾ƿͼ� GetTransform()->SetLocalPosition(pos); �� �ϸ� �ȴ�.
		// EX) ������ ���� �ڱ� ID�� �´� ��ü�� pos ���� �޾ƿͼ� GetTransform()->SetLocalPosition(pos); �� �ϸ� �ȴ�.
		//GetTransform()->SetLocalPosition(Vec3(0.f, 10 * DELTA_TIME, 0.f));		// �ӽ�

		
		/*auto state = NetworkManager::Get()->GetPlayerState(_playerId);
		ApplyNetworkState(state.position, state.rotationEuler, state.equippedWeapon);*/
	}
}



void PlayerScript::UpdatePlayerInput()
{
	UpdateKeyInput();

	UpdateMouseInput();
}

void PlayerScript::UpdateKeyInput()
{
	// 1) ���� ��ġ ����
	Vec3 oldPos = GetTransform()->GetLocalPosition();
	Vec3 newPos = oldPos;
	
	// 2) �Է¿� ���� newPos ����	
	if (INPUT->GetButton(KEY_TYPE::W))
		newPos += GetTransform()->GetLook() * _speed * DELTA_TIME;
	if (INPUT->GetButton(KEY_TYPE::S))
		newPos -= GetTransform()->GetLook() * _speed * DELTA_TIME;
	if (INPUT->GetButton(KEY_TYPE::A))
		newPos -= GetTransform()->GetRight() * _speed * DELTA_TIME;
	if (INPUT->GetButton(KEY_TYPE::D))
		newPos += GetTransform()->GetRight() * _speed * DELTA_TIME;
	if (INPUT->GetButton(KEY_TYPE::Q))
		newPos += GetTransform()->GetLook() * _speed * 2 * DELTA_TIME;
	if (INPUT->GetButton(KEY_TYPE::CTRL))
		newPos -= GetTransform()->GetUp() * _speed * DELTA_TIME;

	if (_isGrounded && INPUT->GetButtonDown(KEY_TYPE::SPACE))
	{
		_currentVelocity = _jumpVelocity;
		newPos.y += _currentVelocity * DELTA_TIME;
		_isGrounded = false;
	}

	if (INPUT->GetButton(KEY_TYPE::KEY_1))
	{
		GetWeaponManager()->EquipWeapon(0);
	}
	
	if (INPUT->GetButton(KEY_TYPE::KEY_2))
	{
		GetWeaponManager()->EquipWeapon(1);
	}
	
	if (INPUT->GetButton(KEY_TYPE::KEY_3))
	{	
		GetWeaponManager()->EquipWeapon(2);
	}

	if (INPUT->GetButton(KEY_TYPE::LEFTCLICK))
	{
		// Shoot �ִϸ��̼� Ʈ����
		GetAnimator()->SetTrigger("Shoot");
	}

	// 3) ���� �̵��� ���
	Vec3 delta = newPos - oldPos;
	// TODO : pos ���� ������ ������.

	// 4) �ʴ� �ӵ��� ��ȯ
	float currentSpeed = 0.0f;
	if (delta.LengthSquared() > 0.0f)   // >0 �̸�
	{
		float dist = delta.Length();    // �̵� �Ÿ�
		currentSpeed = dist / DELTA_TIME;
		// (���� �ִϸ����Ͱ� 0~1 ���� ���� ����ϸ�
		//  currentSpeed /= _speed; // maxSpeed �� ����ȭ)
	}
	 
	// 5) Animator �� ����
	GetAnimator()->SetFloat("Speed", currentSpeed);
	printf("�ӵ�: %f\n", currentSpeed);
	// 6) ��ġ ����
	GetTransform()->SetLocalPosition(newPos);
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
	// X�� ȸ�� (Pitch, ���Ʒ�)
	_pitch += deltaY * sensitivity;
	_pitch = max(-90 * XM_PI / 180, min(90 * XM_PI / 180, _pitch));

	_yaw += deltaX * sensitivity;

	rotation.x = _pitch * 0;
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






