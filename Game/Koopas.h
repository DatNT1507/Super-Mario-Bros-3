#pragma once
#include "GameObject.h"

#define KOOPAS_GRAVITY 0.002f
#define KOOPAS_WALKING_SPEED 0.05f
#define KOOPAS_IDLE_SPEED 0.25f
#define KOOPAS_DEFLECT_SPEED 0.4f


#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 24
#define KOOPAS_BBOX_HEIGHT_IDLE 16

#define KOOPAS_DIE_TIMEOUT 5000

#define KOOPAS_STATE_WALKING_LEFT 777
#define KOOPAS_STATE_WALKING_RIGHT 666
#define KOOPAS_STATE_IDLE 660
#define KOOPAS_STATE_KICK_LEFT 661
#define KOOPAS_STATE_KICK_RIGHT 662
#define KOOPAS_STATE_HOLD 663
#define KOOPAS_STATE_DEFLECT 664

#define ID_ANI_KOOPAS_WALKING_RIGHT 6500
#define ID_ANI_KOOPAS_WALKING_LEFT 6501
#define ID_ANI_KOOPAS_IDLE 6502
#define ID_ANI_KOOPAS_IDLE_RIGHT 6503
#define ID_ANI_KOOPAS_IDLE_LEFT 6504

class CKoopas : public CGameObject
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
	virtual int IsBlocking() { return 1; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithLuckyBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopas(LPCOLLISIONEVENT e);
	void OnCollisionWithBreakableBrick(LPCOLLISIONEVENT e);

public:
	CKoopas(float x, float y);
	virtual void SetState(int state);
	D3DXVECTOR2 GetPosition()
	{
		return D3DXVECTOR2(x, y);
	}
	void SethaveCheck(bool b)
	{
		this->haveCheck = b;
	}
	int checkLevel;
	bool isSpawn = false;
};