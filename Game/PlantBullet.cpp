#include"PlantBullet.h"
#include"Mario.h"
#include"Fire.h"
#include"PlayScene.h"
#include"Koopas.h"

CPlantBullet::CPlantBullet(float x, float y) :CGameObject(x, y)
{
	this->ay = 0;
	shoot_time = -1;
}

void CPlantBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - PLANTBULLET_BBOX_WIDTH / 2;
	top = y - PLANTBULLET_BBOX_HEIGHT / 2;
	right = left + PLANTBULLET_BBOX_WIDTH;
	bottom = top + PLANTBULLET_BBOX_HEIGHT;
}

void CPlantBullet::OnNoCollision(DWORD dt)
{
	y += vy * dt;
};

void CPlantBullet::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CPlantBullet*>(e->obj)) return;

	if (e->ny < 0)
	{
		isOnPlatform = true;
	}
	if (dynamic_cast<CKoopas*>(e->obj))
		OnCollisionWithKoopas(e);
}

void CPlantBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

	if (isOnPlatform && ((disPx > -30 && disPx < 30) || disPx < -200 || disPx > 200))
	{
		ay = 0;
		hibernate = true;
		StartShoot();
	}
	else if (isOnPlatform && plantPosition.x > marioPosition.x)
		SetState(PLANTBULLET_STATE_UP_LEFT);
	else if (isOnPlatform && plantPosition.x < marioPosition.x)
		SetState(PLANTBULLET_STATE_UP_RIGHT);
	else if (plantPosition.y < 349 && GetState() != PLANTBULLET_STATE_DOWN_LEFT && GetState() != PLANTBULLET_STATE_DOWN_RIGHT)
		SetState(PLANTBULLET_STATE_SHOOT);

	if (disPy > 40 && disPx < 0)
	{
		SetState(PLANTBULLET_STATE_BOT_LEFT);
	}
	else if (disPy < -40 && disPx < 0)
	{
		SetState(PLANTBULLET_STATE_TOP_LEFT);
	}
	else if ((disPy >= -40 || disPy <= 40) && disPx < 0)
	{
		SetState(PLANTBULLET_STATE_MID_LEFT);
	}
	else if (disPy > 40 && disPx > 0)
	{
		SetState(PLANTBULLET_STATE_BOT_RIGHT);
	}
	else if (disPy < -40 && disPx > 0)
	{
		SetState(PLANTBULLET_STATE_TOP_RIGHT);
	}
	else if ((disPy >= -40 || disPy <= 40) && disPx > 0)
	{
		SetState(PLANTBULLET_STATE_MID_RIGHT);
	}
	if (shoot_time == -1)
	{
		StartShoot();
		if(disPx < 0 && !isShoot)
			SetState(PLANTBULLET_STATE_DOWN_LEFT);
		else if (disPx > 0 && !isShoot)
			SetState(PLANTBULLET_STATE_DOWN_RIGHT);
	}
	else if (GetTickCount64() - shoot_time > 6500 && !hibernate)
	{
		LPGAMEOBJECT fire = new CFire(plantPosition.x, plantPosition.y);
		if (state == PLANTBULLET_STATE_BOT_LEFT)
			fire->SetState(FIRE_STATE_BOT_LEFT);
		else if (state == PLANTBULLET_STATE_TOP_LEFT)
			fire->SetState(FIRE_STATE_TOP_LEFT);
		else if (state == PLANTBULLET_STATE_MID_LEFT)
			fire->SetState(FIRE_STATE_MID_LEFT);
		else if (state == PLANTBULLET_STATE_BOT_RIGHT)
			fire->SetState(FIRE_STATE_BOT_RIGHT);
		else if (state == PLANTBULLET_STATE_TOP_RIGHT)
			fire->SetState(FIRE_STATE_TOP_RIGHT);
		else
			fire->SetState(FIRE_STATE_MID_RIGHT);
		CPlayScene* playScene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
		if (playScene != nullptr)
		{
			playScene->AddObject(fire);
		}
		if (disPx < 0)
			SetState(PLANTBULLET_STATE_DOWN_LEFT);
		else
			SetState(PLANTBULLET_STATE_DOWN_RIGHT);
		shoot_time = -1;
		isShoot = false;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CPlantBullet::Render()
{
	int aniId = ID_ANI_PLANTBULLET_TOP_LEFT;

	if (state == PLANTBULLET_STATE_BOT_LEFT)
	{
		aniId = ID_ANI_PLANTBULLET_BOT_LEFT;
	}
	else if (state == PLANTBULLET_STATE_TOP_RIGHT || state == PLANTBULLET_STATE_MID_RIGHT)
	{
		aniId = ID_ANI_PLANTBULLET_TOP_RIGHT;
	}
	else if (state == PLANTBULLET_STATE_BOT_RIGHT)
	{
		aniId = ID_ANI_PLANTBULLET_BOT_RIGHT;
	}
	else if (state == PLANTBULLET_STATE_UP_LEFT || state == PLANTBULLET_STATE_DOWN_LEFT)
	{
		aniId = ID_ANI_PLANTBULLET_UP_LEFT;
	}
	else if (state == PLANTBULLET_STATE_UP_RIGHT || state == PLANTBULLET_STATE_DOWN_RIGHT)
	{
		aniId = ID_ANI_PLANTBULLET_UP_RIGHT;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CPlantBullet::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PLANTBULLET_STATE_DIE:
		this->Delete();
		break;
	case PLANTBULLET_STATE_UP_LEFT:
		ay = -PLANTBULLET_GRAVITY;
		isOnPlatform = false;
		hibernate = false;
		break;
	case PLANTBULLET_STATE_DOWN_LEFT:
		ay = PLANTBULLET_GRAVITY;
		isShoot = false;
		break;
	case PLANTBULLET_STATE_SHOOT:
		ay = 0;
		isShoot = true;
		hibernate = false;
		break;
	case PLANTBULLET_STATE_UP_RIGHT:
		ay = -PLANTBULLET_GRAVITY;
		isOnPlatform = false;
		hibernate = false;
		break;
	case PLANTBULLET_STATE_DOWN_RIGHT:
		ay = PLANTBULLET_GRAVITY;
		isShoot = false;
		break;
	}
}

void CPlantBullet::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
	if (koopas->GetState() == KOOPAS_STATE_KICK_LEFT || koopas->GetState() == KOOPAS_STATE_KICK_RIGHT)
	{
		SetState(PLANTBULLET_STATE_DIE);
	}
}