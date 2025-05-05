#include "pch.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "StructuredBuffer.h"
#include "Transform.h"
#include "WeaponManager.h"
#include "Weapon.h"
#include "Animator.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "MeshData.h"
#include "Resources.h"

#include "GunWeapon.h"
#include "TwoHandedMeleeWeapon.h"


WeaponManager::WeaponManager() : Component(COMPONENT_TYPE::WEAPONMANAGER)
{
	// TODO : �÷��̾� ĳ���� ���� �� ���� ��ü�� ���� �����Ѵ�.
	AddWeapon();
	
}

WeaponManager::~WeaponManager()
{
}

void WeaponManager::PushData()
{
	// ĳ���� World Matrix, Bone�� ���� ��� Update
	_characterWorldMat = GetTransform()->GetLocalToWorldMatrix();								// ĳ������ World Matrix Update

	for (auto& weapon : _currentWeapon)
	{
		weapon->GetWeapon()->SetCharacterMatrix(_characterWorldMat);							// Weapon ��ü�� ĳ������ ���� ����� ������.
		weapon->GetWeapon()->SetBoneFinalMatrix(GetAnimator()->GetBoneKeyFrameMatrix());		// Weapon ��ü�� ���� ���� ����� ������.
	}
}

void WeaponManager::FinalUpdate()
{
	
}

void WeaponManager::AddWeapon()
{
	shared_ptr<MeshData> Rifle = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\SA_Wep_AssaultRifle01.bin"); // MeshData* meshData
	vector<shared_ptr<GameObject>> gameObjects = Rifle->Instantiate();

	for (auto& gameObject : gameObjects)
	{
		shared_ptr<Weapon> weapon = make_shared<GunWeapon>();
		gameObject->SetCheckFrustum(false);
		gameObject->SetStatic(true);
		gameObject->AddComponent(weapon);
		gameObject->GetMeshRenderer()->SetRender(false);
		GET_SINGLE(SceneManager)->GetActiveScene()->AddGameObject(gameObject);
	}
	shared_ptr<GameObject> rootObject = gameObjects[0];

	rootObject->GetTransform()->SetLocalPosition(Vec3(0.2412, -0.033, 0.017));
	rootObject->GetTransform()->SetLocalRotation(Vec3(0.0, 90.0f, 86.f));

	_weaponInventory.push_back(gameObjects);



	shared_ptr<MeshData> SMG_data = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\SA_Wep_SMG.bin"); // MeshData* meshData
	vector<shared_ptr<GameObject>> SMG = SMG_data->Instantiate();

	for (auto& gameObject : SMG)
	{
		shared_ptr<Weapon> weapon = make_shared<GunWeapon>();
		gameObject->SetCheckFrustum(false);
		gameObject->SetStatic(true);
		gameObject->AddComponent(weapon);
		gameObject->GetMeshRenderer()->SetRender(false);
		GET_SINGLE(SceneManager)->GetActiveScene()->AddGameObject(gameObject);
	}

	SMG[0]->GetTransform()->SetLocalPosition(Vec3(0.2412, -0.033, 0.017));
	SMG[0]->GetTransform()->SetLocalRotation(Vec3(0.0, 90.0f, 87.0f));

	_weaponInventory.push_back(SMG);

	shared_ptr<MeshData> Bat_data = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\SA_Wep_baseballBat.bin"); // MeshData* meshData
	vector<shared_ptr<GameObject>> Bat = Bat_data->Instantiate();

	for (auto& gameObject : Bat)
	{
		shared_ptr<Weapon> weapon = make_shared<TwoHandedMeleeWeapon>();
		gameObject->SetCheckFrustum(false);
		gameObject->SetStatic(true);
		gameObject->AddComponent(weapon);
		gameObject->GetMeshRenderer()->SetRender(false);
		GET_SINGLE(SceneManager)->GetActiveScene()->AddGameObject(gameObject);
	}

	Bat[0]->GetTransform()->SetLocalPosition(Vec3(0.1242781, -0.0314697, 0.24));
	Bat[0]->GetTransform()->SetLocalRotation(Vec3(0.0, 0.0f, 0.f));

	_weaponInventory.push_back(Bat);

	EquipWeapon(0);

}

void WeaponManager::EquipWeapon(int index)
{
	if (index > GetInventorySize() - 1)					// �ٲٷ��� ������ Index�� �κ��丮 Size���� �۴ٸ� �ߴ��Ѵ�.
		return;

	for (auto& weapon : _currentWeapon)				// ���� ����� ���� �����Ѵ�.
	{
		weapon->GetWeapon()->SetisEquipped(false);
		weapon->GetMeshRenderer()->SetRender(false);
	}

	_currentWeapon = _weaponInventory[index];

	for (auto& weapon : _currentWeapon)				// ���� ���⸦ �����Ѵ�.
	{
		weapon->GetWeapon()->SetisEquipped(true);
		weapon->GetMeshRenderer()->SetRender(true);

	}
}
