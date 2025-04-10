#include "pch.h"
#include "Weapon.h"
#include "StructuredBuffer.h"
#include "GameObject.h"
#include "Animator.h"

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
	_boneFinalMatrix = _weaponObject->GetAnimator()->GetBoneKeyFrameMatirx();// 무기 소유 객체->GetAnimator()->GetBoneFinalMatirx();


	_boneFinalMatrix->PushGraphicsData(SRV_REGISTER::t7);
}

void Weapon::FinalUpdate()
{
}
