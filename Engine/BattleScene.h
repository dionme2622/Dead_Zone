#pragma once
#include "Scene.h"
#include "Player.h"


class BattleScene : public Scene
{
public:
	BattleScene();
	virtual ~BattleScene() {}

	virtual void LoadScene();
	virtual void Update();
	virtual void FinalUpdate();


public:
	void CreateZombie();

	void CheckCollisions();


private:
	shared_ptr<Player>			_player;
	shared_ptr<GameObject>		_playerCamera;
	shared_ptr<GameObject>		_uiCamera;

	shared_ptr<GameObject>		_mainLight;

	HWND _hwnd;

	bool _isFirstFrame; // 첫 프레임 여부를 추적하는 멤버 변수


private:
	vector<vector<shared_ptr<GameObject>>> _zombies;
};

