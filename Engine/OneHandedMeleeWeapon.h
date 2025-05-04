#pragma once
#include "Weapon.h"

class OneHandedMeleeWeapon : public Weapon
{
public:

	OneHandedMeleeWeapon() {};
	virtual ~OneHandedMeleeWeapon() {};
public:
	virtual void Attack();
	virtual void Reload();
	virtual void SetBulletPosition();
	virtual void SetBulletDirection();
	virtual void Update(); // 추가: 무기 상태 업데이트
};

