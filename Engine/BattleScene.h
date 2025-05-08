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

	void AddSpotLights();

	static bool isPlayerGrounded;
	vector<shared_ptr<GameObject>> GetPlayers() { return _player; };
	vector<shared_ptr<GameObject>>	GetSpotLights() { return _spotLights; }


public:

private:
	shared_ptr<GameObject>							player1;
	shared_ptr<GameObject>							_playerCamera;
	shared_ptr<GameObject>							_uiCamera;

	vector<shared_ptr<GameObject>>					_player;
	shared_ptr<GameObject>							_mainLight;
	vector<shared_ptr<GameObject>>					_spotLights;
	shared_ptr<GameObject>							_sunObject;
	HWND _hwnd;


private:
	vector<vector<shared_ptr<GameObject>>> _zombies;



};

