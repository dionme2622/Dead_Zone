#pragma once
#include "BaseCollider.h"

class BoxCollider : public BaseCollider
{
public:
	BoxCollider(const Vec3& center = { 0,0,0 }, const Vec3& extents = { 1,1,1 });
	virtual ~BoxCollider();


	virtual void FinalUpdate() override;
	virtual bool Intersects(const Vec4& rayOrigin, const Vec4& rayDir, float& outDistance) override;

	virtual btCollisionShape* GetShape() const { return _shape.get(); }

public:
	virtual shared_ptr<Mesh> GetColliderMesh() override;

	Vec3 GetCenter() { return _center; }

	void SetCenter(Vec3 center) { _center = center; }
	void SetExtents(Vec3 extents) { _extents = extents; }

	

public:
	Vec3				_center		= { 0.f, 0.f, 0.f };
	Vec3				_extents	= { 1.f, 1.f, 1.f };

	shared_ptr<btBoxShape>			_shape;
	
};

