#include "pch.h"
#include "Weapon.h"
#include "StructuredBuffer.h"
#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Material.h"

Weapon::Weapon() : Component(COMPONENT_TYPE::WEAPON)
{
	
}

Weapon::~Weapon()
{
}



void Weapon::SetWeaponObject(shared_ptr<GameObject> weaponObject)
{
	_weaponObject = weaponObject;
}

void Weapon::PushData()
{
	if (GetisEquipped())
	{
		GetMeshRenderer()->GetMaterial()->SetInt(2, 1);
		GetMeshRenderer()->GetMaterial()->SetMatrix(0, _characterWorldMat);
		_boneFinalMatrix->PushGraphicsData(SRV_REGISTER::t7);
	}
	else
		GetMeshRenderer()->GetMaterial()->SetInt(2, 0);
}

void Weapon::FinalUpdate()
{
}
