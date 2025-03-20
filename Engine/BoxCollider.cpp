#include "pch.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "Resources.h"
#include "Mesh.h"

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

vector<Vertex> BoxCollider::GetBoxVertex(const Vec3 center, const Vec3 extents, wstring meshName)
{
	Vec3 min = { center.x - extents.x, center.y - extents.y, center.z - extents.z };
	Vec3 max = { center.x + extents.x, center.y + extents.y, center.z + extents.z };

	// TODO
	vector<Vertex> vec(8);
	vector<uint32> idx(8);

	vec[0].pos = { min.x, min.y, min.z };
	vec[1].pos = { max.x, min.y, min.z };
	vec[2].pos = { min.x, max.y, min.z };
	vec[3].pos = { max.x, max.y, min.z };
	vec[4].pos = { min.x, min.y, max.z };
	vec[5].pos = { max.x, min.y, max.z };
	vec[6].pos = { min.x, max.y, max.z };
	vec[7].pos = { max.x, max.y, max.z };

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vec, idx);

	GET_SINGLE(Resources)->Add<Mesh>(meshName, mesh);
	

	return vec;
}
	