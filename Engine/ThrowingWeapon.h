#pragma once
#include "Weapon.h"

class ThrowingWeapon : public Weapon
{
public:

	ThrowingWeapon() {};
	virtual ~ThrowingWeapon() {};
public:
	virtual void Attack();
	virtual void Reload();
	virtual void SetBulletPosition();
	virtual void SetBulletDirection();
	virtual void Update(); // �߰�: ���� ���� ������Ʈ
};

