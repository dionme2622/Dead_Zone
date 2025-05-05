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

	void CheckCollisions();

	static bool isPlayerGrounded;
private:
	//shared_ptr<Player>			_player;
	shared_ptr<GameObject>			_playerCamera;
	shared_ptr<GameObject>			_uiCamera;

	vector<shared_ptr<GameObject>>	_player;
	shared_ptr<GameObject>		_mainLight;
	HWND _hwnd;


private:
	vector<vector<shared_ptr<GameObject>>> _zombies;



};

