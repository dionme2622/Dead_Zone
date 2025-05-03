#include "pch.h"
#include "Light.h"
#include "Transform.h"
#include "Engine.h"
#include "Resources.h"
#include "Camera.h"
#include "Texture.h"
#include "SceneManager.h"
#include "KeyInput.h"


#define MATDEBUG _shadowCamera->GetCamera()->GetTransform()->GetLocalMatrix()

Light::Light() : Component(COMPONENT_TYPE::LIGHT)
{
	_shadowCamera = make_shared<GameObject>();
	_shadowCamera->AddComponent(make_shared<Transform>());
	_shadowCamera->AddComponent(make_shared<Camera>());
	_shadowCamera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
	_shadowCamera->GetTransform()->debug = true;
}

Light::~Light()
{
}

void Light::FinalUpdate()
{
	/*cout << MATDEBUG._11 << " " << MATDEBUG._12 << " " << MATDEBUG._13 << " " << MATDEBUG._14 << endl;
	cout << MATDEBUG._21 << " " << MATDEBUG._22 << " " << MATDEBUG._23 << " " << MATDEBUG._24 << endl;
	cout << MATDEBUG._31 << " " << MATDEBUG._32 << " " << MATDEBUG._33 << " " << MATDEBUG._34 << endl;
	cout << MATDEBUG._41 << " " << MATDEBUG._42 << " " << MATDEBUG._43 << " " << MATDEBUG._44 << endl;
	cout << endl;*/


	_shadowCamera->GetCamera()->GetTransform()->GetLocalMatrix();

	Vec3 look = _shadowCamera->GetTransform()->GetLocalRotation();

	_lightInfo.position = GetTransform()->GetWorldPosition();

	_shadowCamera->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());

	_shadowCamera->GetTransform()->SetLocalRotation(Vec3(0, 0, 0)); // 회전 없음

	_shadowCamera->GetTransform()->SetLocalScale(GetTransform()->GetLocalScale());

	// 빛의 방향에 맞춰 카메라가 아래를 향하도록 설정
	_shadowCamera->GetCamera()->GetTransform()->SetLocalRotation(Vec3(45, 0, 0));

	_shadowCamera->FinalUpdate();

	_shadowCamera->GetCamera()->GetTransform()->FinalUpdate();
	//_shadowCamera->GetTransform()->GetLocalMatrix();

}

void Light::Render()
{
	assert(_lightIndex >= 0);
	GetTransform()->PushData();

	if (static_cast<LIGHT_TYPE>(_lightInfo.lightType) == LIGHT_TYPE::DIRECTIONAL_LIGHT)
	{
		shared_ptr<Texture> shadowTex = GET_SINGLE(Resources)->Get<Texture>(L"ShadowTarget");
		_lightMaterial->SetTexture(2, shadowTex);

		Matrix matVP = _shadowCamera->GetCamera()->GetViewMatrix() * _shadowCamera->GetCamera()->GetProjectionMatrix();

		_lightMaterial->SetMatrix(0, matVP);
	}
	else
	{
		float scale = 2 * _lightInfo.range;
		GetTransform()->SetLocalScale(Vec3(scale, scale, scale));
	}

	_lightMaterial->SetInt(0, _lightIndex);
	_lightMaterial->PushGraphicsData();

	_volumeMesh->Render();
}

void Light::RenderShadow()
{
	_shadowCamera->GetCamera()->SortShadowObject();
	_shadowCamera->GetCamera()->Render_Shadow();
}

void Light::SetLightDirection(Vec3 direction)
{
	direction.Normalize();

	_lightInfo.direction = direction;

	GetTransform()->LookAt(direction);
}

void Light::SetLightType(LIGHT_TYPE type)
{
	_lightInfo.lightType = static_cast<int32>(type);

	switch (type)
	{
	case LIGHT_TYPE::DIRECTIONAL_LIGHT:
		_volumeMesh = GET_SINGLE(Resources)->Get<Mesh>(L"Rectangle");
		_lightMaterial = GET_SINGLE(Resources)->Get<Material>(L"DirLight");

		_shadowCamera->GetCamera()->SetScale(1.f);
		_shadowCamera->GetCamera()->SetNear(0.01);
		_shadowCamera->GetCamera()->SetFar(200);
		_shadowCamera->GetCamera()->SetWidth(200);
		_shadowCamera->GetCamera()->SetHeight(200);

		break;
	case LIGHT_TYPE::POINT_LIGHT:
		_volumeMesh = GET_SINGLE(Resources)->Get<Mesh>(L"Sphere");
		_lightMaterial = GET_SINGLE(Resources)->Get<Material>(L"PointLight");
		break;
	case LIGHT_TYPE::SPOT_LIGHT:
		_volumeMesh = GET_SINGLE(Resources)->Get<Mesh>(L"Sphere");
		_lightMaterial = GET_SINGLE(Resources)->Get<Material>(L"PointLight");
		break;
	}
}



