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

private:
	//shared_ptr<Player>			_player;
	shared_ptr<GameObject>		_playerCamera;
	shared_ptr<GameObject>		_uiCamera;

	shared_ptr<GameObject>		_player;
	HWND _hwnd;

public:

};

