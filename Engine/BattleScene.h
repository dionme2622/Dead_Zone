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

private:
	shared_ptr<Player>			_player;
	shared_ptr<GameObject>		_playerCamera;
	shared_ptr<GameObject>		_uiCamera;

	shared_ptr<GameObject>		_mainLight;

	HWND _hwnd;

	bool _isFirstFrame; // ù ������ ���θ� �����ϴ� ��� ����
	//TEST
public:
	void CheckCollisions();
};

