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

private:
	shared_ptr<Player>			_player;
	shared_ptr<GameObject>		_playerCamera;
	shared_ptr<GameObject>		_uiCamera;

	HWND _hwnd;


	//TEST
	vector<shared_ptr<GameObject>>		_character;
	vector<shared_ptr<GameObject>>		_weapon;

public:
	void TestObjectPosition(shared_ptr<GameObject> sourceObj);

	shared_ptr<StructuredBuffer>	_boneFinalMatrix;  // Ư�� �������� ���� ���

};

