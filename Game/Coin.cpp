#include "Coin.h"
#include "LuckyBrickCoin.h"
#include "Reward.h"
#include "Status.h"
void CCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_COIN)->Render(x, y);

	//RenderBoundingBox();
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}

void CCoin::SetState(int state)
{
	if (state == COIN_DEFLECT)
	{
		vy = -COIN_DEFLECT_SPEED;
	}
	CGameObject::SetState(state);
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetState() == COIN_IDLE)
		vy = 0;
	else
		vy += ay * dt;

	/*CGameObject::Update(dt, coObjects);*/
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CCoin::OnNoCollision(DWORD dt)
{
	y += vy * dt;
}

void CCoin::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CLuckyBrickCoin*>(e->obj))
		OnCollisionWithLuckyBrickCoin(e);
}

void CCoin::OnCollisionWithLuckyBrickCoin(LPCOLLISIONEVENT e)
{
	CLuckyBrickCoin* luckybrickcoin = dynamic_cast<CLuckyBrickCoin*>(e->obj);
	if (e->ny < 0)
	{
		CReward::getInstance()->getEffect()->SetValue(this->x, this->y, EFFECT_TYPE_POINT, 100);
		CStatus::getInstance()->AddCoin(1);
		this->Delete();
	}
}