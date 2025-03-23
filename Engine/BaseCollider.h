#pragma once
#include "Component.h"
#include "Mesh.h"

enum class ColliderType
{
	Box, Sphere, Capsule
};

class BaseCollider : public Component
{
public:
	BaseCollider(ColliderType colliderType);
	virtual ~BaseCollider();

	virtual bool Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance) = 0;
	virtual shared_ptr<Mesh> GetColliderMesh() = 0;


public:
	bool DebugDraw() { return _debugDraw; };			// Collider 를 그리는가?

private:
	ColliderType	_colliderType = {};
	bool			_debugDraw	 = true;
};