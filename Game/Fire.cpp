#include"Fire.h"
#include"PlantBullet.h"

CFire::CFire(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;
	delete_start = -1;
	SetState(FIRE_STATE_TOP_RIGHT);
}

void CFire::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - FIRE_BBOX_WIDTH / 2;
	top = y - FIRE_BBOX_HEIGHT / 2;
	right = left + FIRE_BBOX_WIDTH;
	bottom = top + FIRE_BBOX_HEIGHT;
}

void CFire::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

//void CFire::OnCollisionWith(LPCOLLISIONEVENT e)
//{
//	if (!e->obj->IsBlocking()) return;
//	if (dynamic_cast<CFire*>(e->obj)) return;
//
//	
//}

void CFire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy = ay * dt;
	vx = ax * dt;

	if (GetTickCount64() - delete_start > 5000)
	{
		this->Delete();
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CFire::Render()
{
	int aniId = ID_ANI_FIRE;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CFire::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FIRE_STATE_MID_LEFT:
		ay = 0;
		ax = -FIRE_SPEED;
		delete_start = GetTickCount64();
		break;
	case FIRE_STATE_TOP_LEFT:
		ay = -FIRE_GRAVITY;
		ax = -FIRE_SPEED;
		delete_start = GetTickCount64();
		break;
	case FIRE_STATE_BOT_LEFT:
		ay = FIRE_GRAVITY;
		ax = -FIRE_SPEED;
		delete_start = GetTickCount64();
		break;
	case FIRE_STATE_MID_RIGHT:
		ay = 0;
		ax = FIRE_SPEED;
		delete_start = GetTickCount64();
		break;
	case FIRE_STATE_TOP_RIGHT:
		ay = -FIRE_GRAVITY;
		ax = FIRE_SPEED;
		delete_start = GetTickCount64();
		break;
	case FIRE_STATE_BOT_RIGHT:
		ay = FIRE_GRAVITY;
		ax = FIRE_SPEED;
		delete_start = GetTickCount64();
		break;
	}
}