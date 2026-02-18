#pragma once
#include "GameObject.h"

#define MUSHROOM_GRAVITY 0.002f
#define MUSHROOM_WALKING_SPEED 0.05f


#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16


#define MUSHROOM_STATE_WALKING 100

#define ID_ANI_MUSHROOM_WALKING 3000
#define ID_ANI_MUSHROOM_GREEN_WALKING 3001

class CMushroom : public CGameObject
{
protected:
	float ax;
	float ay;
	bool isGreen;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CMushroom(float x, float y, bool isGreen = false);
	virtual void SetState(int state);
	bool GetGreen()
	{
		return isGreen;
	}
};