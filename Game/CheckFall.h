#pragma once
#include "GameObject.h"

#define CHECKFALL_GRAVITY 0.002f
#define CHECKFALL_WALKING_SPEED 0.05f


#define CHECKFALL_BBOX_WIDTH 16
#define CHECKFALL_BBOX_HEIGHT 14

#define CHECKFALL_STATE_FALL 110
#define CHECKFALL_STATE_RIGHT 111
#define CHECKFALL_STATE_LEFT 112
#define CHECKFALL_STATE_VX 113
#define ID_ANI_CHECKFALL 7500

class CCheckFall : public CGameObject
{
public:
	float ax;
	float ay;

	bool isOnPlatform = false;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CCheckFall(float x, float y);
	virtual void SetState(int state);
	D3DXVECTOR2 GetPosition()
	{
		return D3DXVECTOR2(x, y);
	}
};