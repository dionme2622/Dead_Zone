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
#include "WeaponManager.h"
#include "Weapon.h"
#include "Animator.h"
#include "RigidBody.h"
#include "PhysicsSystem.h"
PlayerScript::PlayerScript(HWND hwnd, bool isLocal, int playerId, shared_ptr<CharacterController> controller)
{
	_hwnd = hwnd;
	_isLocal = isLocal;
	_playerId = playerId;
	_controller = controller;
	// Player�� ���� ���� �ʱ�ȭ �ܰ�
	_speed = 0.0f;
}

PlayerScript::~PlayerScript()
{
}

void PlayerScript::FinalUpdate()
{
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
	_speed = 0.f;
	// 1) ���� ��ġ ����
	Vec3 currentPos = GetTransform()->GetLocalPosition();

	// 2) WASD �Է¿� ���� ����(dir) ���
	Vec3 dir(0, 0, 0);
	if (INPUT->GetButton(KEY_TYPE::W)) dir += GetTransform()->GetLook(), _speed = 5.0f;
	if (INPUT->GetButton(KEY_TYPE::S)) dir -= GetTransform()->GetLook(), _speed = 5.0f;
	if (INPUT->GetButton(KEY_TYPE::A)) dir -= GetTransform()->GetRight(), _speed = 5.0f;
	if (INPUT->GetButton(KEY_TYPE::D)) dir += GetTransform()->GetRight(), _speed = 5.0f;
	if (INPUT->GetButton(KEY_TYPE::SHIFT) && _speed > 1.0f) _speed = 10.f;
	if (dir.LengthSquared() > 0.0f)
		dir.Normalize();

	const float fixedStep = 1.0f / 60.0f;
	_controller->Move(dir * _speed * fixedStep);

	// 3) ���� �پ� ���� ���� ����
	if (INPUT->GetButtonDown(KEY_TYPE::SPACE) && _controller->IsOnGround())
	{
		_controller->Jump();
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
		if (GetWeaponManager()) {
			GetAnimator()->SetTrigger("Shoot");
			GetWeaponManager()->GetCurrentWeapon()[0]->GetWeapon()->SetBulletPosition();
			GetWeaponManager()->GetCurrentWeapon()[0]->GetWeapon()->SetBulletDirection();
			GetWeaponManager()->GetCurrentWeapon()[0]->GetWeapon()->Attack();
			_isShoot = true;
		}
	}

	if (INPUT->GetButtonUp(KEY_TYPE::LEFTCLICK))
	{
		_isShoot = false;
	}

	if (INPUT->GetButton(KEY_TYPE::CTRL)) {
		_isAiming = true;

	}
	else if (INPUT->GetButtonUp(KEY_TYPE::CTRL)) {
		_isAiming = false;
	}

	Vec3 delta = currentPos - _prevPosition;
	delta.y = 0;
	float currentSpeed = delta.Length() / DELTA_TIME;

	// 5) Animator �� ����
	GetAnimator()->SetFloat("Speed", _speed);
	GetAnimator()->SetBool("isJumping", !_controller->IsOnGround());
	GetAnimator()->SetBool("isAiming", _isAiming);
	GetAnimator()->SetBool("isShooting", _isShoot);

	//printf("����?: %d\n", !_controller->IsOnGround());
	//printf("�ӵ�: %f\n", currentSpeed);
	/*printf("����: %f %f %f\n", _prevPosition.x, _prevPosition.y, _prevPosition.z);
	printf("����: %f %f %f\n", currentPos.x, currentPos.y, currentPos.z);*/

	_prevPosition = currentPos;
}


void PlayerScript::UpdateMouseInput()
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

	rotation.x = _pitch * 50;
	rotation.y = _yaw * 50;
	rotation.z = 0.0;

	GetTransform()->SetLocalRotation(rotation);
}
