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
	// ���� ����Ʈ�� �����ϰų� �߰��ϴ� �Լ�
	void PushData();

	void AddWeapon();

	void EquipWeapon(int index);

	int GetInventorySize() { return _weaponInventory.size(); }

	vector<shared_ptr<GameObject>> GetCurrentWeapon() {	return _currentWeapon; }
private:
	Matrix									_characterWorldMat;		// ĳ������ World Matrix

	vector<shared_ptr<GameObject>>			_currentWeapon;			// ���� �����ϰ� �ִ� ����
	vector<vector<shared_ptr<GameObject>>>	_weaponInventory;		// ���� �����ϰ� �ִ� �����
};

