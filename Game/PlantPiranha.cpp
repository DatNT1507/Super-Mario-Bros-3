#include"PlantBullet.h"
#include"Mario.h"
#include"PlantPiranha.h"
#include"PlayScene.h"
#include"Koopas.h"

CPlantPiranha::CPlantPiranha(float x, float y) :CGameObject(x, y)
{
	this->ay = 0;
	eat_time = -1;
}

void CPlantPiranha::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - PLANTPIRANHA_BBOX_WIDTH / 2;
	top = y - PLANTPIRANHA_BBOX_HEIGHT / 2;
	right = left + PLANTPIRANHA_BBOX_WIDTH;
	bottom = top + PLANTPIRANHA_BBOX_HEIGHT;
}

void CPlantPiranha::OnNoCollision(DWORD dt)
{
	y += vy * dt;
};

void CPlantPiranha::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CPlantPiranha*>(e->obj)) return;

	if (e->ny < 0)
	{
		isOnPlatform = true;
	}
	if (dynamic_cast<CKoopas*>(e->obj))
		OnCollisionWithKoopas(e);
}

void CPlantPiranha::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy = ay * dt;

	D3DXVECTOR2 plantPosition = this->GetPosition();
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
	float disPy = marioPosition.y - plantPosition.y;
	float disPx = marioPosition.x - plantPosition.x;

	if (isOnPlatform && ((disPx > -30 && disPx < 30) || disPx < -200 || disPx > 260))
	{
		ay = 0;
		hibernate = true;
		StartEat();
	}
	else if (isOnPlatform)
		SetState(PLANTPIRANHA_STATE_UP);
	else if (plantPosition.y < 368 && GetState() != PLANTPIRANHA_STATE_DOWN)
		SetState(PLANTPIRANHA_STATE_EAT);


	//SetState(PLANTPIRANHA_STATE_UP);

	if (eat_time == -1)
	{
		StartEat();
		if (!isEat)
			SetState(PLANTPIRANHA_STATE_DOWN);
	}
	else if (GetTickCount64() - eat_time > 4000 && !hibernate)
	{
		SetState(PLANTPIRANHA_STATE_DOWN);
		eat_time = -1;
		isEat = false;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CPlantPiranha::Render()
{
	int aniId = ID_ANI_PLANTPIRANHA_UP_DOWN;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CPlantPiranha::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PLANTPIRANHA_STATE_DIE:
		this->Delete();
		break;
	case PLANTPIRANHA_STATE_UP:
		ay = -PLANTBULLET_GRAVITY;
		isOnPlatform = false;
		hibernate = false;
		break;
	case PLANTPIRANHA_STATE_DOWN:
		ay = PLANTBULLET_GRAVITY;
		isEat = false;
		break;
	case PLANTPIRANHA_STATE_EAT:
		ay = 0;
		isEat = true;
		hibernate = false;
		break;
	}
}
void CPlantPiranha::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
	if (koopas->GetState() == KOOPAS_STATE_KICK_LEFT || koopas->GetState() == KOOPAS_STATE_KICK_RIGHT)
	{
		SetState(PLANTPIRANHA_STATE_DIE);
	}
}