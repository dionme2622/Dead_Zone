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
	shared_ptr<StructuredBuffer>	_boneFinalMatrix;  // 특정 프레임의 최종 행렬
	
	bool							_boneFinalUpdated = false;
};
