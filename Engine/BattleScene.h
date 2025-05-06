#pragma once
#include "Scene.h"
#include "PlayerStats.h"


class BattleScene : public Scene
{
public:
	BattleScene();
	virtual ~BattleScene() {}

	virtual void LoadScene();
	virtual void Update();


public:
	void CreateZombie();
	
	void UpdateSunOrbit();

	static bool isPlayerGrounded;
private:
	shared_ptr<GameObject>			player1;
	shared_ptr<GameObject>			_playerCamera;
	shared_ptr<GameObject>			_uiCamera;

	vector<shared_ptr<GameObject>>	_player;
	shared_ptr<GameObject>			_mainLight;
	shared_ptr<GameObject>			_sunObject;
	HWND _hwnd;


private:
	vector<vector<shared_ptr<GameObject>>> _zombies;



};

