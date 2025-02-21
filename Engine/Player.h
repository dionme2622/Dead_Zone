#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "MeshRenderer.h"

class GameObject;
class Mesh;
class Material;
class Shader;
class Texture;
class MeshRenderer;
class Camera;


class Player : public Component
{
public:
	Player();
	virtual ~Player();


	shared_ptr<GameObject> GetGameObject() { return _obj; }

	float GetSpeed() { return _speed; }

public:
	virtual void LateUpdate() override;

private:
	shared_ptr<GameObject>		_obj;
	shared_ptr<Mesh>			_characterMesh;
	shared_ptr<Material>		_material;
	shared_ptr<Shader>			_shader;
	shared_ptr<Texture>			_texture;
	shared_ptr<Texture>			_normal;
	shared_ptr<MeshRenderer>	_meshRenderer;

	uint8						_textureNum;

	shared_ptr<GameObject>		_camera;

	float						_speed = 1.f;


	// 입력 관련
public:
	void UpdateRotation(float deltaX, float deltaY);

private:
	float sensitivity = 0.001f;

	float _pitch = 0.0f; // 위/아래 각도
	float _yaw = 0.0f; // 좌우 각도
	Vec3 rotation; // 카메라 회전
};

