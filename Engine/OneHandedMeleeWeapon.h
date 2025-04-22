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
	virtual void DeleteBullet();
};

