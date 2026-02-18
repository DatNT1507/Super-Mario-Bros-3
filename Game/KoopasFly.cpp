#include "KoopasFly.h"
#include "CheckFall.h"
#include "PlayScene.h"
#include "LuckyBrick.h"
#include "Koopas.h"

CKoopasFly::CKoopasFly(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPAS_GRAVITY;
	isKicked = false;
	die_start = 0;
	//SetState(KOOPASFLY_STATE_WALKING_LEFT);
}

void CKoopasFly::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPAS_STATE_IDLE or state == KOOPAS_STATE_KICK_LEFT or state == KOOPAS_STATE_KICK_RIGHT)
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT_IDLE / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT_IDLE;
	}
	else
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT;
	}
}

void CKoopasFly::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopasFly::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CKoopasFly*>(e->obj)) return;
	if (dynamic_cast<CKoopas*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
		isOnPlatform = true;
	}
	else if (GetState() != KOOPAS_STATE_KICK_LEFT && GetState() != KOOPAS_STATE_KICK_RIGHT && GetState() != KOOPAS_STATE_IDLE && state != KOOPASFLY_STATE_FLY_LEFT && state != KOOPASFLY_STATE_FLY_RIGHT)
	{
		if (vx < 0 && state != KOOPASFLY_STATE_WALKING_LEFT)
			SetState(KOOPAS_STATE_WALKING_RIGHT);
		else if (vx > 0 && state != KOOPASFLY_STATE_WALKING_RIGHT)
			SetState(KOOPAS_STATE_WALKING_LEFT);
		else if (vx < 0)
			SetState(KOOPASFLY_STATE_WALKING_RIGHT);
		else
			SetState(KOOPASFLY_STATE_WALKING_LEFT);
	}
	else if (e->nx != 0)
	{
		if (vx < 0 && state != KOOPASFLY_STATE_FLY_LEFT)
			SetState(KOOPASFLY_STATE_FLY_RIGHT);
		else if (vx > 0 && state != KOOPASFLY_STATE_FLY_RIGHT)
			SetState(KOOPASFLY_STATE_FLY_LEFT);
		else
			vx = -vx;
	}
}

void CKoopasFly::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == KOOPASFLY_STATE_WALKING_LEFT || state == KOOPASFLY_STATE_WALKING_RIGHT) && isOnPlatform)
	{
		if (vx > 0)
			SetState(KOOPASFLY_STATE_FLY_RIGHT);
		else
			SetState(KOOPASFLY_STATE_FLY_LEFT);
	}
	else if (((state == KOOPASFLY_STATE_FLY_LEFT) || (state == KOOPASFLY_STATE_FLY_RIGHT)) && isOnPlatform)
	{
		if (vx > 0)
			SetState(KOOPASFLY_STATE_WALKING_RIGHT);
		else
			SetState(KOOPASFLY_STATE_WALKING_LEFT);
	}

	D3DXVECTOR2 koopasfly = this->GetPosition();
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
	float disPx = marioPosition.x - koopasfly.x;

	if (disPx > -200 && disPx < 200 && !isSpawn)
	{
		isSpawn = true;
		SetPosition(koopasfly.x, koopasfly.y);
		SetState(KOOPASFLY_STATE_WALKING_LEFT);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CKoopasFly::Render()
{
	int aniId = ID_ANI_KOOPASFLY_WALKING_RIGHT;
	if (state == KOOPASFLY_STATE_WALKING_LEFT)
	{
		aniId = ID_ANI_KOOPASFLY_WALKING_LEFT;
	}
	else if (state == KOOPAS_STATE_IDLE || state == KOOPAS_STATE_HOLD)
	{
		aniId = ID_ANI_KOOPAS_IDLE;
	}
	else if (state == KOOPAS_STATE_KICK_LEFT)
	{
		aniId = ID_ANI_KOOPAS_IDLE_RIGHT;
	}
	else if (state == KOOPAS_STATE_KICK_RIGHT)
	{
		aniId = ID_ANI_KOOPAS_IDLE_LEFT;
	}
	else if (state == KOOPASFLY_STATE_FLY_RIGHT)
	{
		aniId = ID_ANI_KOOPASFLY_FLY_RIGHT;
	}
	else if (state == KOOPASFLY_STATE_FLY_LEFT)
	{
		aniId = ID_ANI_KOOPASFLY_FLY_LEFT;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CKoopasFly::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPASFLY_STATE_WALKING_LEFT:
		vx = -KOOPAS_WALKING_SPEED;
		ay = KOOPAS_GRAVITY;
		break;
	case KOOPASFLY_STATE_WALKING_RIGHT:
		vx = KOOPAS_WALKING_SPEED;
		ay = KOOPAS_GRAVITY;
		break;
	case KOOPASFLY_STATE_FLY_RIGHT:
		vx = KOOPAS_WALKING_SPEED;
		vy = -KOOPASFLY_DEFLECT_SPEED;
		ay = 0.001f;
		isOnPlatform = false;
		break;
	case KOOPASFLY_STATE_FLY_LEFT:
		vx = -KOOPAS_WALKING_SPEED;
		vy = -KOOPASFLY_DEFLECT_SPEED;
		ay = 0.001f;
		isOnPlatform = false;
		break;
	}
}