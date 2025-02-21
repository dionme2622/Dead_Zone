#pragma once
#include "Scene.h"
#include "Player.h"


class BattleScene : public Scene
{
public:
	BattleScene();
	virtual ~BattleScene() {}

	void Init();

	virtual void LoadScene();
	virtual void Update();

private:
	shared_ptr<Player>			_player;
	shared_ptr<GameObject>		_playerCamera;
	HWND		_hwnd;
};

