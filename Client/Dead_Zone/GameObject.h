#pragma once
#include "Component.h"
#include "Object.h"

class Transform;
class MeshRenderer;
class Camera;
class Light;
class MonoBehaviour;
class BaseCollider;
class CapsuleCollider;
class ParticleSystem;
class WeaponManager;
class Weapon;
class Bullet;
class PlayerStats;
class BoxCollider;

class GameObject : public Object, public enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();

	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	shared_ptr<Component> GetFixedComponent(COMPONENT_TYPE type);

	shared_ptr<Transform> GetTransform();
	shared_ptr<MeshRenderer> GetMeshRenderer();
	shared_ptr<Animator> GetAnimator();
	shared_ptr<WeaponManager> GetWeaponManager();
	shared_ptr<Weapon> GetWeapon();
	shared_ptr<Bullet> GetBullet();
	shared_ptr<PlayerStats> GetPlayerStats();
	shared_ptr<RigidBody> GetRigidBody();
	shared_ptr<CharacterController> GetCharacterController();


	shared_ptr<Camera> GetCamera();
	shared_ptr<Light> GetLight();
	shared_ptr<BaseCollider> GetCollider();
	shared_ptr<CapsuleCollider> GetCapsuleCollider();
	shared_ptr<ParticleSystem> GetParticleSystem();

	void AddComponent(shared_ptr<Component> component);
	void RemoveAllComponent();

	void SetCheckFrustum(bool checkFrustum) { _checkFrustum = checkFrustum; }
	bool GetCheckFrustum() { return _checkFrustum; }
	
	void SetLayerIndex(uint8 layer) { _layerIndex = layer; }
	uint8 GetLayerIndex() { return _layerIndex; }



	void SetStatic(bool flag) { _static = flag; }
	bool IsStatic() { return _static; }


	shared_ptr<BoxCollider> GetBoxCollier() { return _boxCollider; }
	void SetBoxCollier(shared_ptr<BoxCollider> boxCollider) { _boxCollider = boxCollider; }


private:
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	vector<shared_ptr<MonoBehaviour>> _scripts;
	
	bool _checkFrustum = true;
	uint8 _layerIndex = 0;
	bool _static = true;



private:
	shared_ptr<BoxCollider> _boxCollider;

};

