#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "Goomba.h"

#define GOOMBAFLY_GRAVITY 0.001f
#define GOOMBAFLY_WALKING_SPEED 0.05f
#define GOOMBAFLY_DEFLECT_SPEED 0.3f


#define GOOMBAFLY_BBOX_WIDTH 19
#define GOOMBAFLY_BBOX_HEIGHT 18
#define GOOMBAFLY_BBOX_FLY_HEIGHT 23
#define GOOMBAFLY_BBOX_HEIGHT_DIE 7

#define GOOMBAFLY_DIE_TIMEOUT 500

#define GOOMBAFLY_STATE_WALKING_LEFT 700
#define GOOMBAFLY_STATE_FLY 701
#define GOOMBAFLY_STATE_WALK_LEFT 702
#define GOOMBAFLY_STATE_WALKING_RIGHT 703
#define GOOMBAFLY_STATE_WALK_RIGHT 704
#define GOOMBAFLY_STATE_DIE 800
#define GOOMBAFLY_STATE_DEFLECT 801

#define ID_ANI_GOOMBAFLY_WALKING 5100
#define ID_ANI_GOOMBAFLY_FLY 5101

class CGoombafly : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;
	BOOLEAN isOnPlatform = false;
	bool isSpawn = false;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return GetState() != GOOMBAFLY_STATE_DEFLECT; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopas(LPCOLLISIONEVENT e);
public:
	CGoombafly(float x, float y);
	virtual void SetState(int state);
	float GetVx()
	{
		return vx;
	}
	D3DXVECTOR2 GetPosition()
	{
		return D3DXVECTOR2(x, y);
	}
};