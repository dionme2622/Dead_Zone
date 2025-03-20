#pragma once
#include "BaseCollider.h"

class BoxCollider : public BaseCollider
{
public:
	BoxCollider();
	BoxCollider(const Vec3 center, const Vec3 extents);

	virtual ~BoxCollider();

	virtual void FinalUpdate() override;
	virtual bool Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance) override;


public:
	vector<Vertex> GetBoxVertex(const Vec3 center, const Vec3 extents, wstring meshName);


	void SetCenter(Vec3 center) { _center = center; };
	void SetExtents(Vec3 extents) { _extents = extents; };

	

private:
	Vec3				_center		= { 0.f, 0.f, 0.f };
	Vec3				_extents	= { 1.f, 1.f, 1.f };

	BoundingBox			_boundingBox;
	
};

