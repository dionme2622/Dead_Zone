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

	void SetObjectArea();
	void UpdatePlayerAreas();

	void CheckCollisions();


	vector<STAGEAREA> GetAdjacentAreas(STAGEAREA playerArea);

	STAGEAREA CalculateObjectArea(const Vec3& position);

	

	static bool isPlayerGrounded;
private:
	shared_ptr<Player>			_player;
	

	shared_ptr<GameObject>		_playerCamera;
	shared_ptr<GameObject>		_uiCamera;

	shared_ptr<GameObject>		_mainLight;

	HWND _hwnd;


private:
	vector<vector<shared_ptr<GameObject>>> _zombies;

	unordered_map<STAGEAREA, vector<shared_ptr<GameObject>>> _areaObjects;

};

