#include "pch.h"
#include "Component.h"
#include "GameObject.h"

Component::Component(COMPONENT_TYPE type) : Object(OBJECT_TYPE::COMPONENT), _type(type)
{

}

Component::~Component()
{
}

shared_ptr<GameObject> Component::GetGameObject()
{
	return _gameObject.lock();
}

shared_ptr<Transform> Component::GetTransform()
{
	return _gameObject.lock()->GetTransform();
}

shared_ptr<BaseCollider> Component::GetCollider()
{
	return _gameObject.lock()->GetCollider();
}

shared_ptr<MeshRenderer> Component::GetMeshRenderer()
{
	return _gameObject.lock()->GetMeshRenderer();
}

shared_ptr<Animator> Component::GetAnimator()
{
	return _gameObject.lock()->GetAnimator();
}

shared_ptr<WeaponManager> Component::GetWeaponManager()
{
	return _gameObject.lock()->GetWeaponManager();
}

shared_ptr<Weapon> Component::GetWeapon()
{
	return _gameObject.lock()->GetWeapon();
}

shared_ptr<Bullet> Component::GetBullet()
{
	return _gameObject.lock()->GetBullet();
}

shared_ptr<PlayerStats> Component::GetPlayerStats()
{
	return _gameObject.lock()->GetPlayerStats();
}

shared_ptr<RigidBody> Component::GetRigidBody()
{
	return _gameObject.lock()->GetRigidBody();
}
