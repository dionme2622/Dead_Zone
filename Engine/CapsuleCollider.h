#pragma once
#include "BaseCollider.h"

class CapsuleCollider : public BaseCollider
{
public:
	CapsuleCollider(float radius, float height);

	virtual ~CapsuleCollider();

	virtual void FinalUpdate() override;

public:
	// PhysicsBody 컴포넌트가 이 shape을 참조해서 rigidbody 생성
	virtual btCollisionShape* GetShape() const { return _shape.get(); }
	// 순수 shape 생성, DebugDraw(), RaycastIntersect() 등
	virtual bool Intersects(const Vec4& rayOrigin, const Vec4& rayDir, float& outDist) override;
	virtual shared_ptr<Mesh> GetColliderMesh() override;

	void SetRadius(float radius) { _radius = radius; };
	void SetHeight(float height) { _height = height; };


private:
	float		_radius = 0.5f;
	float		_height = 1.0f;

	shared_ptr<btCapsuleShape>		_shape;

};

