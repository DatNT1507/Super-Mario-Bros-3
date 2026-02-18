#include "CheckFall.h"
#include "Koopas.h"

CCheckFall::CCheckFall(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = CHECKFALL_GRAVITY;
}

void CCheckFall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - CHECKFALL_BBOX_WIDTH / 2;
	top = y - CHECKFALL_BBOX_HEIGHT / 2;
	right = left + CHECKFALL_BBOX_WIDTH;
	bottom = top + CHECKFALL_BBOX_HEIGHT;
}

void CCheckFall::OnNoCollision(DWORD dt)
{
	float preY = y;
	x += vx * dt;
	y += vy * dt;
	if (preY != y and isOnPlatform)
		SetState(CHECKFALL_STATE_FALL);
};

void CCheckFall::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0)
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
		SetState(CHECKFALL_STATE_VX);
}

void CCheckFall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	CKoopas* koopas = nullptr;

	for (size_t i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CKoopas*>(obj))
		{
			koopas = dynamic_cast<CKoopas*>(obj);
			break;
		}
	}
	D3DXVECTOR2 checkfall = this->GetPosition();
	D3DXVECTOR2 koopasPosition = koopas->GetPosition();
	float dis = abs(checkfall.x - koopasPosition.x);

	if (GetState() == CHECKFALL_STATE_VX && dis > 20 && koopas->GetState() != KOOPAS_STATE_IDLE
		&& koopas->GetState() != KOOPAS_STATE_KICK_LEFT && 
		koopas->GetState() != KOOPAS_STATE_KICK_RIGHT && koopas->GetState() != KOOPAS_STATE_HOLD)
	{
		this->Delete();
		koopas->SethaveCheck(false);
	}
	else if (GetState() == CHECKFALL_STATE_FALL && koopas->GetState() == KOOPAS_STATE_WALKING_LEFT)
	{
		koopas->SetState(KOOPAS_STATE_WALKING_RIGHT);
		this->Delete();
		koopas->SethaveCheck(false);
	}
	else if (GetState() == CHECKFALL_STATE_FALL && koopas->GetState() == KOOPAS_STATE_WALKING_RIGHT)
	{
		koopas->SetState(KOOPAS_STATE_WALKING_LEFT);
		this->Delete();
		koopas->SethaveCheck(false);
	}


	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CCheckFall::Render()
{
	int aniId = ID_ANI_CHECKFALL;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CCheckFall::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case CHECKFALL_STATE_RIGHT:
		vx = KOOPAS_WALKING_SPEED;
		break;
	case CHECKFALL_STATE_LEFT:
		vx = -KOOPAS_WALKING_SPEED;
		break;
	case CHECKFALL_STATE_FALL:
		isOnPlatform = false;
		break;
	}
}