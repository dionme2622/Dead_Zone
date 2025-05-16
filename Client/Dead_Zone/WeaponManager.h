#pragma once
#include "Component.h"

class Scene;
class GameObject;
class StructuredBuffer;
class Weapon;

class WeaponManager : public Component
{
public:
	WeaponManager();
	virtual ~WeaponManager();

	virtual void FinalUpdate() override;
	// 무기 리스트를 설정하거나 추가하는 함수
	void PushData();

	void AddWeapon();

	void EquipWeapon(int index);

	int GetInventorySize() { return _weaponInventory.size(); }

	vector<shared_ptr<GameObject>> GetCurrentWeapon() {	return _currentWeapon; }
private:
	Matrix									_characterWorldMat;		// 캐릭터의 World Matrix

	vector<shared_ptr<GameObject>>			_currentWeapon;			// 현재 장착하고 있는 무기
	vector<vector<shared_ptr<GameObject>>>	_weaponInventory;		// 현재 소유하고 있는 무기들
};

