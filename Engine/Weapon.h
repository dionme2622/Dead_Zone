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
	bool GetisEquipped() { return _isEquipped; }


	void SetCharacterMatrix(Matrix matrix) { _characterWorldMat = matrix; }
	void SetBoneFinalMatrix(shared_ptr<StructuredBuffer> boneMatrix) { _boneFinalMatrix = boneMatrix; }
	void SetisEquipped(bool boolen) { _isEquipped = boolen; }

public:
	virtual void Attack() = 0;
	virtual void Reload() = 0;


public:
	virtual void FinalUpdate() override;

protected:
	shared_ptr<GameObject>			_weaponObject;
	shared_ptr<Matrix>				_offsetMat;				// 무기의 Offset Matrix
	Matrix							_characterWorldMat;		// 무기의 Offset Matrix
	shared_ptr<StructuredBuffer>	_boneFinalMatrix;		// 뼈들의 최종 행렬

	bool							_isEquipped = false;				// 무기가 장착되어있는가?
};


