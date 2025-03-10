#pragma once
#include "MonoBehaviour.h"

class PlayerScript : public MonoBehaviour
{
public:
	PlayerScript();
	virtual ~PlayerScript();

	virtual void LateUpdate() override;

	void UpdatePlayerInput();

	void UpdateRotation(float deltaX, float deltaY);

	void UpdateKeyInput();

	void UpdateMouseInput();


	// �̵� ����
public:
	void UpdatePlayerOnTerrain();

private:
	float _speed;     // ������ �ִ� �̵� �ӵ�
	float _jumpVelocity;    // ���� �ʱ� �ӵ�
	float _currentVelocity; // ���� ���� �ӵ�
	float _gravity;         // �߷� ���ӵ�
	bool _isGrounded;       // ���� ����ִ��� ����


private:
	float sensitivity = 0.001f;

	float _pitch = 0.0f; // ��/�Ʒ� ����
	float _yaw = 0.0f; // �¿� ����
	Vec3 rotation; // ī�޶� ȸ��



	HWND _hwnd;
	bool _mouseMove;
};

