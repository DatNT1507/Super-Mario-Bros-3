#pragma once
#include "GameObject.h"

#define PLANTPIRANHA_BBOX_WIDTH 16
#define PLANTPIRANHA_BBOX_HEIGHT 22

#define PLANTPIRANHA_STATE_UP_DOWN 1350
#define PLANTPIRANHA_STATE_EAT 1351
#define PLANTPIRANHA_STATE_DOWN 1352
#define PLANTPIRANHA_STATE_UP 1353
#define PLANTPIRANHA_STATE_DIE 1354

#define ID_ANI_PLANTPIRANHA_UP_DOWN 1345		

class CPlantPiranha : public CGameObject
{
protected:
	float ay;
	ULONGLONG eat_time;
	bool isOnPlatform = false;
	bool hibernate = false;
	bool isEat = false;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopas(LPCOLLISIONEVENT e);

public:
	CPlantPiranha(float x, float y);
	virtual void SetState(int state);
	D3DXVECTOR2 GetPosition()
	{
		return D3DXVECTOR2(x, y);
	}
	void StartEat() { eat_time = GetTickCount64(); }
};