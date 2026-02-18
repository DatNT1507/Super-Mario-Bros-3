#pragma once
#include "GameObject.h"

#define LEAF_GRAVITY 0.001f
#define LEAF_SPEED	0.05f

#define LEAF_BBOX_WIDTH 16
#define LEAF_BBOX_HEIGHT 16


#define LEAF_STATE_FALL_LEFT 107
#define LEAF_STATE_FALL_RIGHT 108

#define ID_ANI_LEAF_FALL 3010

class CLeaf : public CGameObject
{
protected:
	float ax;
	float ay;
	ULONGLONG fall_start;
	bool isCollidable = false;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return !isCollidable; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CLeaf(float x, float y);
	virtual void SetState(int state);
};