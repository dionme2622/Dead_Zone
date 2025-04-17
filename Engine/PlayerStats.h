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





private:
	float			_hp = 100.0f;		// ü��
	float			_mp = 100.0f;		// ���ŷ�
	float			_speed = 10.0f;		// �̵��ӵ�



};

