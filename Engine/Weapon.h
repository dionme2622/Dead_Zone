#pragma once
#include "Component.h"
#include "Mesh.h"

class Material;
class StructuredBuffer;
class Mesh;

class Weapon : public Component
{
public:
	Weapon();
	virtual ~Weapon();

public:
	void SetWeaponObject(shared_ptr<GameObject> weaponObject);
	void PushData();
	shared_ptr<StructuredBuffer> GetBoneFinalMatirx() { return _boneFinalMatrix; }

public:
	virtual void FinalUpdate() override;

private:
	shared_ptr<GameObject>			_weaponObject;
	shared_ptr<StructuredBuffer>	_boneFinalMatrix;  // Ư�� �������� ���� ���
	
	bool							_boneFinalUpdated = false;
};
