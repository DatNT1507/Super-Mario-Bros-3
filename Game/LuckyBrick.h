#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_LUCKYBRICK 3500
#define ID_ANI_LUCKYBRICK_STATE5 3501
#define LUCKYBRICK_WIDTH 16
#define LUCKYBRICK_BBOX_WIDTH 16
#define LUCKYBRICK_BBOX_HEIGHT 16
#define LUCKYBRICK_STATE5 555
#define LUCKYBRICK_STATE_DEFLECT 556
#define LUCKYBRICK_DEFLECT_SPEED 0.1f
class CLuckyBrick : public CGameObject {
	float ay;
	float vy;
	float preY;
	ULONGLONG fall_start;
public:
	CLuckyBrick(float x, float y) : CGameObject(x, y) 
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