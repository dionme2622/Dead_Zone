#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Light.h"
#include "MonoBehaviour.h"
#include "ParticleSystem.h"
#include "Animator.h"
#include "BaseCollider.h"
#include "CapsuleCollider.h"
#include "WeaponManager.h"
#include "Weapon.h"
#include "Bullet.h"
#include "PlayerStats.h"
#include "RigidBody.h"

GameObject::GameObject() : Object(OBJECT_TYPE::GAMEOBJECT)
{

}

GameObject::~GameObject()
{

}

void GameObject::Awake()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Awake();
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Awake();
	}
}

void GameObject::Start()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Start();
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Start();
	}
}

void GameObject::Update()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Update();
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Update();
	}
}

void GameObject::LateUpdate()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->LateUpdate();
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->LateUpdate();
	}
}

void GameObject::FinalUpdate()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->FinalUpdate();
	}
}

shared_ptr<Component> GameObject::GetFixedComponent(COMPONENT_TYPE type)
{
	uint8 index = static_cast<uint8>(type);
	assert(index < FIXED_COMPONENT_COUNT);
	return _components[index];
}

shared_ptr<Transform> GameObject::GetTransform()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::TRANSFORM);
	return static_pointer_cast<Transform>(component);
}

shared_ptr<MeshRenderer> GameObject::GetMeshRenderer()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::MESH_RENDERER);
	return static_pointer_cast<MeshRenderer>(component);
}

shared_ptr<Animator> GameObject::GetAnimator()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::ANIMATOR);
	return static_pointer_cast<Animator>(component);
}

shared_ptr<WeaponManager> GameObject::GetWeaponManager()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::WEAPONMANAGER);
	return static_pointer_cast<WeaponManager>(component);
}

shared_ptr<Weapon> GameObject::GetWeapon()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::WEAPON);
	return static_pointer_cast<Weapon>(component);
}

shared_ptr<Bullet> GameObject::GetBullet()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::BULLET);
	return static_pointer_cast<Bullet>(component);
}

shared_ptr<PlayerStats> GameObject::GetPlayerStats()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::PLAYERSTATS);
	return static_pointer_cast<PlayerStats>(component);
}

shared_ptr<RigidBody> GameObject::GetRigidBody()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::RIGIDBODY);
	return static_pointer_cast<RigidBody>(component);
}

shared_ptr<Camera> GameObject::GetCamera()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::CAMERA);
	return static_pointer_cast<Camera>(component);
}

shared_ptr<Light> GameObject::GetLight()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::LIGHT);
	return static_pointer_cast<Light>(component);
}

shared_ptr<BaseCollider> GameObject::GetCollider()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::COLLIDER);
	return static_pointer_cast<BaseCollider>(component);
}

shared_ptr<CapsuleCollider> GameObject::GetCapsuleCollider()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::COLLIDER);
	return static_pointer_cast<CapsuleCollider>(component);
}

shared_ptr<ParticleSystem> GameObject::GetParticleSystem()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::PARTICLE_SYSTEM);
	return static_pointer_cast<ParticleSystem>(component);
}

void GameObject::AddComponent(shared_ptr<Component> component)
{
	component->SetGameObject(shared_from_this());

	uint8 index = static_cast<uint8>(component->GetType());
	if (index < FIXED_COMPONENT_COUNT)
	{
		_components[index] = component;
	}
	else
	{
		_scripts.push_back(dynamic_pointer_cast<MonoBehaviour>(component));
	}
}