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
	_characterWorldMat = _weaponObject->GetTransform()->GetLocalToWorldMatrix();
	_boneFinalMatrix = _weaponObject->GetAnimator()->GetBoneKeyFrameMatirx();// 무기 소유 객체->GetAnimator()->GetBoneFinalMatirx();

	GetMeshRenderer()->GetMaterial()->SetMatrix(0, _characterWorldMat);
	
	_boneFinalMatrix->PushGraphicsData(SRV_REGISTER::t7);
}

void Weapon::FinalUpdate()
{
}
