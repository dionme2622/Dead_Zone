#include "pch.h"
#include "Light.h"
#include "Transform.h"
#include "Engine.h"
#include "Resources.h"
#include "Camera.h"
#include "Texture.h"
#include "SceneManager.h"

Light::Light() : Component(COMPONENT_TYPE::LIGHT)
{
	_shadowCamera = make_shared<GameObject>();
	_shadowCamera->AddComponent(make_shared<Transform>());
	_shadowCamera->AddComponent(make_shared<Camera>());
	_shadowCamera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
}

Light::~Light()
{
}

void Light::FinalUpdate()
{
	_lightInfo.position = GetTransform()->GetLocalPosition();
	_shadowCamera->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());
	_shadowCamera->GetTransform()->SetLocalScale(GetTransform()->GetLocalScale());

	// ���� �������� ������ ī�޶� ����
	Vec3 lightDir = Vec3(_lightInfo.direction.x, _lightInfo.direction.y, _lightInfo.direction.z);
	lightDir.Normalize();
	_shadowCamera->GetTransform()->LookAt(_shadowCamera->GetTransform()->GetLocalPosition() + lightDir);


	// Transform�� ���� ���� Ȯ��
	Vec3 look = _shadowCamera->GetTransform()->GetLook();
	cout << "Shadow Camera look: " << look.x << ", " << look.y << ", " << look.z << endl;

	_shadowCamera->FinalUpdate();
}

void Light::Render()
{
	Vec3 look = _shadowCamera->GetTransform()->GetLocalRotation();
	assert(_lightIndex >= 0);

	GetTransform()->PushData();

	if (static_cast<LIGHT_TYPE>(_lightInfo.lightType) == LIGHT_TYPE::DIRECTIONAL_LIGHT)
	{
		shared_ptr<Texture> shadowTex = GET_SINGLE(Resources)->Get<Texture>(L"ShadowTarget");
		_lightMaterial->SetTexture(2, shadowTex);

		Matrix matVP = _shadowCamera->GetCamera()->GetViewMatrix() * _shadowCamera->GetCamera()->GetProjectionMatrix();
		// ���Ʈ������ ȸ�� ���
		/*cout << "View Matrix: " << 
			_shadowCamera->GetCamera()->GetViewMatrix()._31 << ", " << 
			_shadowCamera->GetCamera()->GetViewMatrix()._32 << ", " <<
			_shadowCamera->GetCamera()->GetViewMatrix()._33 << ", " 
			<< endl;*/
		

		
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
		_shadowCamera->GetCamera()->SetWidth(500);
		_shadowCamera->GetCamera()->SetHeight(500);

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



