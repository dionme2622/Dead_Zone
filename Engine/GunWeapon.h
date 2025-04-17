#pragma once
#include "Weapon.h"

class GunWeapon : public Weapon
{
public:

	GunWeapon() {};
	virtual ~GunWeapon() {};
public:
	virtual void Attack();
	virtual void Reload();
};

