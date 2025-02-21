#include "pch.h"
#include "Player.h"
#include "Resources.h"
#include "Transform.h"
#include "KeyInput.h"
#include "Timer.h"

Player::Player() : Component(COMPONENT_TYPE::PLAYER)
{
	_obj = make_shared<GameObject>();
	_obj->AddComponent(make_shared<Transform>());
	_obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
	_obj->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 350.f));
	_obj->SetStatic(false);

	_meshRenderer = make_shared<MeshRenderer>();

	_characterMesh = GET_SINGLE(Resources)->LoadSphereMesh();
	_characterMesh->SetName(L"player");
	_material = make_shared<Material>();
	_shader = GET_SINGLE(Resources)->Get<Shader>(L"Deferred");
	_texture = GET_SINGLE(Resources)->Load<Texture>(L"Cliff_Rock", L"..\\Resources\\Texture\\Cliff_Rock_basecolor.png");
	_normal = GET_SINGLE(Resources)->Load<Texture>(L"Cliff_Rock_Normal", L"..\\Resources\\Texture\\Cliff_Rock_normal.png");

	_material->SetShader(_shader);
	_material->SetTexture(0, _texture);
	_material->SetTexture(1, _normal);

	_meshRenderer->SetMesh(_characterMesh);
	_meshRenderer->SetMaterial(_material);

	_obj->AddComponent(_meshRenderer);

}

Player::~Player()
{
}

void Player::LateUpdate()
{
	//UpdateRotation()
}



void Player::UpdateRotation(float deltaX, float deltaY)
{
	// X축 회전 (Pitch, 위아래)
	_pitch += deltaY * sensitivity;
	_pitch = max(-90 * XM_PI / 180, min(90 * XM_PI / 180, _pitch));
	printf("%f\r", _pitch);

	_yaw += deltaX * sensitivity;

	rotation.x = _pitch;
	rotation.y = _yaw;
	rotation.z = 0.0;

	_obj->GetTransform()->SetLocalRotation(rotation);
}

