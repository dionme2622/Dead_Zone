#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "MeshRenderer.h"

class GameObject;
class PlayerStats : public Component
{
public:
	PlayerStats();
	virtual ~PlayerStats();


public:
	virtual void FinalUpdate() override;


public:
	bool IsDead() { return _hp <= 0.0; }

	void ApplyDamage(float damage);
	

private:
	float			_hp = 100.0f;		// 체력
	float			_mp = 100.0f;		// 정신력
	float			_speed = 100.0f;		// 이동속도
	
};

