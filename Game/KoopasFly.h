#pragma once
#include "GameObject.h"

#define KOOPASFLY_DEFLECT_SPEED 0.3f
#define KOOPASFLY_BBOX_WIDTH 16
#define KOOPASFLY_BBOX_HEIGHT 24
#define KOOPASFLY_BBOX_HEIGHT_IDLE 16

#define KOOPASFLY_STATE_WALKING_LEFT 987
#define KOOPASFLY_STATE_WALKING_RIGHT 986
#define KOOPASFLY_STATE_FLY_RIGHT 985
#define KOOPASFLY_STATE_FLY_LEFT 984
//#define KOOPASFLY_STATE_DEFLECT 983
//#define KOOPASFLY_STATE_IDLE 982

#define ID_ANI_KOOPASFLY_WALKING_RIGHT 6100
#define ID_ANI_KOOPASFLY_WALKING_LEFT 6101
#define ID_ANI_KOOPASFLY_FLY_RIGHT 6102
#define ID_ANI_KOOPASFLY_FLY_LEFT 6103

class CKoopasFly : public CGameObject
{
protected:
	float ax;
	float ay;

	bool haveCheck = false;
	bool isKicked;
	ULONGLONG die_start;
	BOOLEAN isOnPlatform = false;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CKoopasFly(float x, float y);
	virtual void SetState(int state);
	D3DXVECTOR2 GetPosition()
	{
		return D3DXVECTOR2(x, y);
	}
	void SethaveCheck(bool b)
	{
		this->haveCheck = b;
	}
	float GetVx()
	{
		return vx;
	}
	bool isSpawn = false;
};