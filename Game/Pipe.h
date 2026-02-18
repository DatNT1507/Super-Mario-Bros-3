#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_PIPE 3456
#define PIPE_WIDTH 30
#define PIPE_BBOX_WIDTH 30
#define PIPE_BBOX_HEIGHT 48

class CPipe : public CGameObject {
public:
	CPipe(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
	{
		if (this->x == 360 || this->x == 1800 || this->x == 1864)
			isR = true;
		else
			isR = false;
		CGameObject::Update(dt, coObjects);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	bool isR;
};