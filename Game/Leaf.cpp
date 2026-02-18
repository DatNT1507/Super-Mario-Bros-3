#include"Leaf.h"

CLeaf::CLeaf(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = LEAF_GRAVITY;
	fall_start = -1;
	SetState(LEAF_STATE_FALL_LEFT);
}

void CLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - LEAF_BBOX_WIDTH / 2;
	top = y - LEAF_BBOX_HEIGHT / 2;
	right = left + LEAF_BBOX_WIDTH;
	bottom = top + LEAF_BBOX_HEIGHT;
}

void CLeaf::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CLeaf::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CLeaf*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
		isCollidable = true;
	}
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy = ay * dt;
	vx += ax * dt;

	if ((state == LEAF_STATE_FALL_LEFT) && (GetTickCount64() - fall_start > 500))
	{
		SetState(LEAF_STATE_FALL_RIGHT);
	}
	else if ((state == LEAF_STATE_FALL_RIGHT) && (GetTickCount64() - fall_start > 500))
	{
		SetState(LEAF_STATE_FALL_LEFT);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CLeaf::Render()
{
	int aniId = ID_ANI_LEAF_FALL;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CLeaf::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LEAF_STATE_FALL_LEFT:
		fall_start = GetTickCount64();
		vx = -LEAF_SPEED;
		/*vy = 0;*/
		break;
	case LEAF_STATE_FALL_RIGHT:
		fall_start = GetTickCount64();
		vx = LEAF_SPEED;
		/*vy = 0;*/
		break;
	}
}