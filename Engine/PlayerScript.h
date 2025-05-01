#pragma once
#include "MonoBehaviour.h"
#include <bullet3\btBulletDynamicsCommon.h>

class PlayerScript : public MonoBehaviour
{
public:
	PlayerScript(HWND hwnd, bool isLocal, int playerId);
	virtual ~PlayerScript();

	virtual void LateUpdate() override;

	void UpdatePlayerInput();

	void UpdateRotation(float deltaX, float deltaY);

	void UpdateKeyInput();

	void UpdateMouseInput();

	bool IsGrounded(btRigidBody* body, btDiscreteDynamicsWorld* world);
	// �̵� ����
public:
	void UpdatePlayerOnTerrain();

private:
	HWND _hwnd;
	bool _isLocal;
	int  _playerId;

	shared_ptr<btRigidBody> _body = nullptr;

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

	bool _mouseMove;
};

