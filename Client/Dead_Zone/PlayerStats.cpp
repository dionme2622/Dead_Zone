#include "pch.h"
#include "PlayerStats.h"
#include "Resources.h"
#include "Transform.h"
#include "KeyInput.h"
#include "Timer.h"
#include "MeshData.h"

PlayerStats::PlayerStats() : Component(COMPONENT_TYPE::PLAYERSTATS)
{
	
}

PlayerStats::~PlayerStats()
{
}

void PlayerStats::FinalUpdate()
{
	
}

void PlayerStats::ApplyDamage(float damage)
{
	_hp -= damage;
}









