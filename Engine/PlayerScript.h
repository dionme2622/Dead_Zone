#pragma once
#include "MonoBehaviour.h"

class PlayerScript : public MonoBehaviour
{
public:
	PlayerScript();
	virtual ~PlayerScript();

	virtual void LateUpdate() override;

	void UpdatePlayerInput();


	// �̵� ����
public:
	void UpdatePlayerOnTerrain();

private:
	float _speed;     // ������ �ִ� �̵� �ӵ�
	float _jumpVelocity;    // ���� �ʱ� �ӵ�
	float _currentVelocity; // ���� ���� �ӵ�
	float _gravity;         // �߷� ���ӵ�
	bool _isGrounded;       // ���� ����ִ��� ����

};

