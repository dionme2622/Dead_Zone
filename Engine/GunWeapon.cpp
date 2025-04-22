#include "pch.h"
#include "GunWeapon.h"
#include "SceneManager.h"
#include "Transform.h"
#include "Resources.h"
#include "Texture.h"
#include "Bullet.h"


GunWeapon::GunWeapon()
{
}

void GunWeapon::Attack()
{
	_bulletObjects.push_back(make_shared<GameObject>());

	_bulletObjects.back()->SetLayerIndex(GET_SINGLE(SceneManager)->GetActiveScene()->LayerNameToIndex(L"Battle"));
	_bulletObjects.back()->AddComponent(make_shared<Transform>());
	_bulletObjects.back()->AddComponent(make_shared<Bullet>());
	_bulletObjects.back()->GetTransform()->SetLocalScale(Vec3(0.2f, 0.2f, 0.2f));
	_bulletObjects.back()->GetBullet()->SetIsFired(true);
	SetBulletPosition();
	SetBulletDirection();
	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();
		meshRenderer->SetMesh(mesh);
	}
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Deferred");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Cliff_Rock", L"..\\Resources\\Texture\\Cliff_Rock_basecolor.png");
		shared_ptr<Texture> texture2 = GET_SINGLE(Resources)->Load<Texture>(L"Cliff_Rock_Normal", L"..\\Resources\\Texture\\Cliff_Rock_normal.png");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		material->SetTexture(1, texture2);
		meshRenderer->SetMaterial(material);
	}
	_bulletObjects.back()->AddComponent(meshRenderer);

	GET_SINGLE(SceneManager)->GetActiveScene()->AddGameObject(_bulletObjects.back());


}

void GunWeapon::Reload()
{
}

void GunWeapon::DeleteBullet()
{
	// _bulletObjects에서 fired 상태가 false인 총알 제거
	_bulletObjects.erase(
		std::remove_if(_bulletObjects.begin(), _bulletObjects.end(),
			[](const shared_ptr<GameObject>& bulletObject) {
				if (!bulletObject->GetBullet()->GetIsFired())
				{
					// Scene에서 제거
					GET_SINGLE(SceneManager)->GetActiveScene()->RemoveGameObject(bulletObject);
					return true; // 벡터에서 제거
				}
				return false; // 유지
			}),
		_bulletObjects.end());

}


void GunWeapon::SetBulletPosition()
{
	Vec3 weaponPosition = GetTransform()->GetLocalPosition();
	weaponPosition += _muzzleOffset;
	_bulletObjects.back()->GetTransform()->SetLocalPosition(weaponPosition);
}

void GunWeapon::SetBulletDirection()
{
	_camera = GET_SINGLE(SceneManager)->GetActiveScene()->GetMainCamera();
	shared_ptr<Bullet> bullet = _bulletObjects.back()->GetBullet();
	shared_ptr<Camera> camera = _camera.lock();
	
	Matrix worldMat = camera->GetTransform()->GetLocalToWorldMatrix();
	Vec3 cameraDirection = Vec3(worldMat._31, worldMat._32, worldMat._33);
	
	cameraDirection.Normalize();

	_bulletObjects.back()->GetBullet()->SetDirection(cameraDirection);

	Vec3 bulletStartPos = GetTransform()->GetLocalPosition() + cameraDirection * 1.0f; // 1.0f는 오프셋 거리
	_bulletObjects.back()->GetBullet()->GetTransform()->SetLocalPosition(bulletStartPos);
}
