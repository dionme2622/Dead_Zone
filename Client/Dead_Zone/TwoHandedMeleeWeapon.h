#pragma once
#include "Weapon.h"

class TwoHandedMeleeWeapon : public Weapon
{
public:

	TwoHandedMeleeWeapon() {};
	virtual ~TwoHandedMeleeWeapon() {};
public:
	virtual void Attack();
	virtual void Reload();
	virtual void SetBulletPosition();
	virtual void SetBulletDirection();
	virtual void Update(); // �߰�: ���� ���� ������Ʈ
};

