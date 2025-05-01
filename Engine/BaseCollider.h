#pragma once
#include "Component.h"
#include "Mesh.h"
#include "bullet3/btBulletDynamicsCommon.h"

enum class ColliderType
{
	Box, Sphere, Capsule
};

class BaseCollider : public Component
{
public:
	BaseCollider(ColliderType colliderType);
	virtual ~BaseCollider();

	virtual btCollisionShape* GetShape() const = 0;

	virtual bool Intersects(const Vec4& rayOrigin, const Vec4& rayDir, float& outDist) = 0;
	virtual shared_ptr<Mesh> GetColliderMesh() = 0;


public:
	bool DebugDraw() { return _debugDraw; };			// Collider �� �׸��°�?

private:
	ColliderType	_colliderType = {};
	bool			_debugDraw	 = true;
};