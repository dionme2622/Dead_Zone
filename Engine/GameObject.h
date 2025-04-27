#pragma once
#include "Component.h"
#include "Object.h"

class Transform;
class MeshRenderer;
class Camera;
class Light;
class MonoBehaviour;
class BaseCollider;
class ParticleSystem;



enum class STAGEAREA
{
	AREA1,
	AREA2,
	AREA3,
	AREA4,
	AREA5,
	AREA6,
	AREA7,
	AREA8,
	AREA9,
	AREA10,
	AREA11,
	AREA12,
	AREA13,
	AREA14,
	AREA15,
	AREA16,
	AREA17,
	AREA18,
	AREA19,
	AREA20,
	AREA21,
	AREA22,
	AREA23,
	AREA24,
	AREA25,
	FLAG_NONE
};



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
	shared_ptr<Camera> GetCamera();
	shared_ptr<Light> GetLight();
	shared_ptr<BaseCollider> GetCollider();
	shared_ptr<ParticleSystem> GetParticleSystem();

	void AddComponent(shared_ptr<Component> component);

	void SetCheckFrustum(bool checkFrustum) { _checkFrustum = checkFrustum; }
	bool GetCheckFrustum() { return _checkFrustum; }
	
	void SetLayerIndex(uint8 layer) { _layerIndex = layer; }
	uint8 GetLayerIndex() { return _layerIndex; }

	void SetStatic(bool flag) { _static = flag; }
	bool IsStatic() { return _static; }

	void SetStageArea(STAGEAREA stageArea) { _stageArea = stageArea; }
	STAGEAREA GetArea() { return _stageArea; }

private:
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	vector<shared_ptr<MonoBehaviour>> _scripts;

	bool _checkFrustum = true;
	uint8 _layerIndex = 0;
	bool _static = true;


	STAGEAREA _stageArea = STAGEAREA::FLAG_NONE;
};

