#include "Goomba.h"
#include "Koopas.h"
#include "Reward.h"

CGoomba::CGoomba(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;
	die_start = -1;
	/*SetState(GOOMBA_STATE_WALKING);*/
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CGoomba*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
	if (dynamic_cast<CKoopas*>(e->obj))
		OnCollisionWithKoopas(e);
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	D3DXVECTOR2 goomba = this->GetPosition();
	CMario* mario = nullptr;

	for (size_t i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CMario*>(obj))
		{
			mario = dynamic_cast<CMario*>(obj);
			break;
		}
	}

	D3DXVECTOR2 marioPosition = mario->GetPosition();
	float disPx = marioPosition.x - goomba.x;

	if (disPx > -300 && disPx < 300 && !isSpawn)
	{
		isSpawn = true;
		SetPosition(goomba.x, goomba.y);
		SetState(GOOMBA_STATE_WALKING);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	int aniId = ID_ANI_GOOMBA_WALKING;
	if (state == GOOMBA_STATE_DIE)
	{
		aniId = ID_ANI_GOOMBA_DIE;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case GOOMBA_STATE_WALKING:
		ay = GOOMBA_GRAVITY;
		vx = -GOOMBA_WALKING_SPEED;
		break;
	case GOOMBA_STATE_DEFLECT:
		vy = -GOOMBA_DEFLECT_SPEED;
		break;
	}
}

void CGoomba::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
	if (koopas->GetState() == KOOPAS_STATE_KICK_LEFT || koopas->GetState() == KOOPAS_STATE_KICK_RIGHT)
	{
		CReward::getInstance()->getEffect()->SetValue(this->x, this->y, EFFECT_TYPE_POINT, 100);
		SetState(GOOMBA_STATE_DEFLECT);
	}
}