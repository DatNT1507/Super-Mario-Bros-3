#pragma once
#include <vector>
#include <string>

#include "Game.h"
#include "PlayScene.h"
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_PLAYER	7777

using namespace std;

class CPlayer : public CGameObject
{
	CGame* game/* = CGame::GetInstance()*/;
	CMario* mario/* = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer()*/;
	LPSPRITE sprite;
	string information;

	float posHudX;
	float posHudY;

	int id_world;
	int coin;
	int life;
	int point;
	int time;
public:
	CPlayer(float x, float y);
	~CPlayer();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void UpdateIndex();
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {}
};