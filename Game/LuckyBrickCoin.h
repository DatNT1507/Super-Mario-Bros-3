#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

class CLuckyBrickCoin : public CGameObject {
	float ay;
	float vy;
	float preY;
	ULONGLONG fall_start;
public:
	CLuckyBrickCoin(float x, float y) : CGameObject(x, y) 
	{
		ay = 0;
		vy = 0;
		preY = 0;
		fall_start = -1;
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	D3DXVECTOR2 GetPosition()
	{
		return D3DXVECTOR2(x, y);
	}
	void SetState(int state);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void OnNoCollision(DWORD dt);
};