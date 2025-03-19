#include "pch.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "Transform.h"


BoxCollider::BoxCollider() : BaseCollider(ColliderType::Box)
{
	_boundingBox.Center = _center;
	_boundingBox.Extents = _extents;
}	

BoxCollider::BoxCollider(Vec3 center, Vec3 extents) : BaseCollider(ColliderType::Box), _center(center), _extents(extents)
{
	_boundingBox.Center = _center;
	_boundingBox.Extents = _extents;
}

BoxCollider::~BoxCollider()
{

}

void BoxCollider::FinalUpdate()
{
	// TODO : 바운딩 박스가 오브젝트를 따라다녀야 한다.
	
	//_boundingBox.Center = GetGameObject()->GetTransform()->GetWorldPosition();

	//Vec3 scale = GetGameObject()->GetTransform()->GetLocalScale();
	//_boundingSphere.Radius = _radius * max(max(scale.x, scale.y), scale.z);

	_boundingBox.Center = _center;
	_boundingBox.Extents = _extents;
}

bool BoxCollider::Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance)
{
	return _boundingBox.Intersects(rayOrigin, rayDir, OUT distance);
}

void BoxCollider::GetBoxCorners(const Vec3 center, const Vec3 extents)
{
	Vec3 min = { center.x - extents.x, center.y - extents.y, center.z - extents.z };
	Vec3 max = { center.x + extents.x, center.y + extents.y, center.z + extents.z };

	// TODO
	vector<Vertex> v;

}
	