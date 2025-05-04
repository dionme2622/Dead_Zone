#include "pch.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "Resources.h"
#include "Mesh.h"
#include "PhysicsSystem.h"

BoxCollider::BoxCollider(const Vec3& center, const Vec3& extents) : BaseCollider(ColliderType::Box), _center(center), _extents(extents)
{
	// 1) ���� �ڽ� ��� ���� (half-extents)
	_shape = std::make_unique<btBoxShape>(
		btVector3(_extents.x, _extents.y, _extents.z));

	// 2) CompoundShape���� ���� ������(center) ����
	_compound = std::make_unique<btCompoundShape>();
	btTransform localTrans;
	localTrans.setIdentity();
	localTrans.setOrigin(
		btVector3(_center.x, _center.y, _center.z));
	_compound->addChildShape(localTrans, _shape.get());
}		

BoxCollider::~BoxCollider()
{

}

void BoxCollider::FinalUpdate()
{
	auto wm = GetTransform()->GetLocalToWorldMatrix();
	auto trans = Matrix::CreateTranslation(_center.x, _center.y, _center.z);
	GET_SINGLE(DebugRenderer)
		->AddBox(
			{ _center, _extents }, // ���� �ٿ�� ����
			wm,                 // �����ϡ�ȸ������ġ ��� ����
			{ 0,1,0,1 }            // �÷�
		);
}

bool BoxCollider::Intersects(const Vec4& rayOrigin, const Vec4& rayDir, float& outDistance) {
	auto world = GET_SINGLE(PhysicsSystem)->GetCollisionWorld();
	btVector3 from(rayOrigin.x, rayOrigin.y, rayOrigin.z);
	btVector3 to(rayOrigin.x + rayDir.x, rayOrigin.y + rayDir.y, rayOrigin.z + rayDir.z);
	btCollisionWorld::ClosestRayResultCallback callback(from, to);
	world->rayTest(from, to, callback);
	if (callback.hasHit()) {
		outDistance = callback.m_closestHitFraction;
		return true;
	}
	return false;
}




shared_ptr<Mesh> BoxCollider::GetColliderMesh()
{
	Vec3 min = { _center.x - _extents.x, _center.y - _extents.y, _center.z - _extents.z };
	Vec3 max = { _center.x + _extents.x, _center.y + _extents.y, _center.z + _extents.z };

	vector<Vertex> vec(24);
	// �ո�
	vec[0] = Vertex(Vec3(min.x, min.y, min.z), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[1] = Vertex(Vec3(min.x, max.y, min.z), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[2] = Vertex(Vec3(max.x, max.y, min.z), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[3] = Vertex(Vec3(max.x, min.y, min.z), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	// �޸�
	vec[4] = Vertex(Vec3(min.x, min.y, max.z), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[5] = Vertex(Vec3(max.x, min.y, max.z), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[6] = Vertex(Vec3(max.x, max.y, max.z), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[7] = Vertex(Vec3(min.x, max.y, max.z), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	// ����
	vec[8] = Vertex(Vec3(min.x, max.y, min.z), Vec2(0.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[9] = Vertex(Vec3(min.x, max.y, max.z), Vec2(0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[10] = Vertex(Vec3(max.x, max.y, max.z), Vec2(1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[11] = Vertex(Vec3(max.x, max.y, min.z), Vec2(1.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	// �Ʒ���
	vec[12] = Vertex(Vec3(min.x, min.y, min.z), Vec2(1.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[13] = Vertex(Vec3(max.x, min.y, min.z), Vec2(0.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[14] = Vertex(Vec3(max.x, min.y, max.z), Vec2(0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[15] = Vertex(Vec3(min.x, min.y, max.z), Vec2(1.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	// ���ʸ�
	vec[16] = Vertex(Vec3(min.x, min.y, max.z), Vec2(0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vec[17] = Vertex(Vec3(min.x, max.y, max.z), Vec2(0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vec[18] = Vertex(Vec3(min.x, max.y, min.z), Vec2(1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vec[19] = Vertex(Vec3(min.x, min.y, min.z), Vec2(1.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	// �����ʸ�
	vec[20] = Vertex(Vec3(max.x, min.y, min.z), Vec2(0.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vec[21] = Vertex(Vec3(max.x, max.y, min.z), Vec2(0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vec[22] = Vertex(Vec3(max.x, max.y, max.z), Vec2(1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vec[23] = Vertex(Vec3(max.x, min.y, max.z), Vec2(1.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));

	vector<uint32> idx(36);

	// �ո�
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// �޸�
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// ����
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// �Ʒ���
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// ���ʸ�
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// �����ʸ�
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vec, idx);

	return mesh;
	
}
	