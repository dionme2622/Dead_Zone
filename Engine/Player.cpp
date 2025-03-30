#include "pch.h"
#include "Player.h"
#include "Resources.h"
#include "Transform.h"
#include "KeyInput.h"
#include "Timer.h"
#include "MeshData.h"
#include "SceneManager.h"  
#include "BoxCollider.h"

Player::Player() : Component(COMPONENT_TYPE::PLAYER)
{
	shared_ptr<MeshData> FemaleSoldier = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\SA_Character_FemaleSoldier.bin"); // MeshData* meshData
	_objs = FemaleSoldier->Instantiate();
}

Player::~Player()
{
}

void Player::FinalUpdate()
{
}








