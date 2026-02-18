#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Mario.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_DEFLECT_SPEED  0.4f
#define COIN_DEFLECT  44
#define COIN_IDLE  45

class CCoin : public CGameObject {
	float ay;
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithLuckyBrickCoin(LPCOLLISIONEVENT e);
	virtual int IsCollidable() { return 1; };
public:
	CCoin(float x, float y) : CGameObject(x, y) 
	{
		ay = MARIO_GRAVITY;
		SetState(COIN_IDLE);
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	void SetState(int);
	void OnNoCollision(DWORD dt);
};