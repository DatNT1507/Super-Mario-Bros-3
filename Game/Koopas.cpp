#include "Koopas.h"
#include "CheckFall.h"
#include "PlayScene.h"
#include "LuckyBrick.h"
#include "Reward.h"
CKoopas::CKoopas(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;
	isKicked = false;
	die_start = 0;
	haveCheck = false;
	//SetState(KOOPAS_STATE_WALKING_LEFT);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
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

void CKoopas::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CKoopas*>(e->obj)) return;
	if (dynamic_cast<CKoopasFly*>(e->obj)) return;

	if (!haveCheck)
	{
		D3DXVECTOR2 koopasPosition = this->GetPosition();
		CCheckFall* checkfall = new CCheckFall(koopasPosition.x, koopasPosition.y);

		if (e->ny != 0 && GetState() == KOOPAS_STATE_WALKING_LEFT)
		{
			checkfall->SetPosition(koopasPosition.x - 10, koopasPosition.y);
			checkfall->SetState(CHECKFALL_STATE_LEFT);
			CPlayScene* playScene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
			if (playScene)
			{
				playScene->AddObject(checkfall);
			}
		}
		else if (e->ny != 0 && GetState() == KOOPAS_STATE_WALKING_RIGHT)
		{
			checkfall->SetPosition(koopasPosition.x + 10, koopasPosition.y);
			checkfall->SetState(CHECKFALL_STATE_RIGHT);
			CPlayScene* playScene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
			if (playScene)
			{
				playScene->AddObject(checkfall);
			}
		}
		
		haveCheck = true;
	}
	
	if (e->ny != 0)
	{
		vy = 0;
		isOnPlatform = true;
	}
	else if (GetState() != KOOPAS_STATE_KICK_LEFT && GetState() != KOOPAS_STATE_KICK_RIGHT && GetState() != KOOPAS_STATE_IDLE)
	{
		if (vx < 0)
			SetState(KOOPAS_STATE_WALKING_RIGHT);
		else
			SetState(KOOPAS_STATE_WALKING_LEFT);
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
	if (dynamic_cast<CLuckyBrick*>(e->obj))
		OnCollisionWithLuckyBrick(e);
	else if (dynamic_cast<CKoopas*>(e->obj))
		OnCollisionWithKoopas(e);
	else if (dynamic_cast<CBreakableBrick*>(e->obj))
		OnCollisionWithBreakableBrick(e);
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	
	if ((state == KOOPAS_STATE_IDLE) && (GetTickCount64() - die_start > KOOPAS_DIE_TIMEOUT))
	{
		SetState(KOOPAS_STATE_WALKING_LEFT);
		D3DXVECTOR2 koopasPosition = this->GetPosition();
		SetPosition(koopasPosition.x, koopasPosition.y - 5);
		isKicked = false;
	}
	else if ((state == KOOPAS_STATE_HOLD) && (GetTickCount64() - die_start > 8000))
	{
		SetState(KOOPAS_STATE_WALKING_LEFT);
		D3DXVECTOR2 koopasPosition = this->GetPosition();
		SetPosition(koopasPosition.x, koopasPosition.y - 5);
		isKicked = false;
	}
	
	D3DXVECTOR2 koopas = this->GetPosition();
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
	float disPx = marioPosition.x - koopas.x;

	if (disPx > -200 && disPx < 200 && !isSpawn)
	{
		isSpawn = true;
		SetPosition(koopas.x, koopas.y);
		SetState(KOOPAS_STATE_WALKING_LEFT);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CKoopas::Render()
{
	int aniId = ID_ANI_KOOPAS_WALKING_RIGHT;
	if (state == KOOPAS_STATE_WALKING_LEFT)
	{
		aniId = ID_ANI_KOOPAS_WALKING_LEFT;
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

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_WALKING_LEFT:
		vx = -KOOPAS_WALKING_SPEED;
		ay = KOOPAS_GRAVITY;
		break;
	case KOOPAS_STATE_WALKING_RIGHT:
		vx = KOOPAS_WALKING_SPEED;
		ay = KOOPAS_GRAVITY;
		break;
	case KOOPAS_STATE_IDLE:
		vx = 0;
		ay = KOOPAS_GRAVITY;
		isKicked = true;
		die_start = GetTickCount64();
		haveCheck = false;
		break;
	case KOOPAS_STATE_KICK_LEFT:
		vx = KOOPAS_IDLE_SPEED;
		ay = KOOPAS_GRAVITY;
		break;
	case KOOPAS_STATE_KICK_RIGHT:
		vx = -KOOPAS_IDLE_SPEED;
		ay = KOOPAS_GRAVITY;
		break;
	case KOOPAS_STATE_HOLD:
		ay = 0;
		break;
	case KOOPAS_STATE_DEFLECT:
		vy = -KOOPAS_DEFLECT_SPEED;
		SetState(KOOPAS_STATE_IDLE);
		break;
	}
}

void CKoopas::OnCollisionWithLuckyBrick(LPCOLLISIONEVENT e)
{
	CLuckyBrick* luckybrick = dynamic_cast<CLuckyBrick*>(e->obj);
	if (GetState() == KOOPAS_STATE_KICK_LEFT || GetState() == KOOPAS_STATE_KICK_RIGHT)
	{
		if (e->nx != 0 and luckybrick->GetState() != LUCKYBRICK_STATE5)
		{
			luckybrick->SetState(LUCKYBRICK_STATE_DEFLECT);
			D3DXVECTOR2 luckyBrickPosition = luckybrick->GetPosition();
			if (checkLevel)
			{
				float mushroomX = luckyBrickPosition.x;
				float mushroomY = luckyBrickPosition.y - 20;

				CMushroom* mushroom = new CMushroom(mushroomX, mushroomY);
				mushroom->SetPosition(mushroomX, mushroomY);

				CPlayScene* playScene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
				if (playScene)
				{
					playScene->AddObject(mushroom);
				}
			}
			else
			{
				float leafX = luckyBrickPosition.x;
				float leafY = luckyBrickPosition.y - 20;

				CLeaf* leaf = new CLeaf(leafX, leafY);
				leaf->SetPosition(leafX, leafY);

				CPlayScene* playScene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
				if (playScene)
				{
					playScene->AddObject(leaf);
				}
			}
		}
	}
}

void CKoopas::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
	if (koopas->GetState() == KOOPAS_STATE_KICK_LEFT || koopas->GetState() == KOOPAS_STATE_KICK_RIGHT)
	{
		SetState(KOOPAS_STATE_DEFLECT);
	}
}

void CKoopas::OnCollisionWithBreakableBrick(LPCOLLISIONEVENT e)
{
	CBreakableBrick* breakabelbrick = dynamic_cast<CBreakableBrick*>(e->obj);

	if (e->nx != 0)
		if (state == KOOPAS_STATE_KICK_LEFT || state == KOOPAS_STATE_KICK_RIGHT)
		{
			CReward::getInstance()->getEffectBreakBrick(this->x, this->y);
			breakabelbrick->Delete();
		}
}