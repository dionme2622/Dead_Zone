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
	virtual void DeleteBullet();
};

