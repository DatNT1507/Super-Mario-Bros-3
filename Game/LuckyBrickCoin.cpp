#include"LuckyBrickCoin.h"
#include"LuckyBrick.h"
void CLuckyBrickCoin::Render()
{
	int aniId = ID_ANI_LUCKYBRICK;
	if (state == LUCKYBRICK_STATE5 || state == LUCKYBRICK_STATE_DEFLECT)
	{
		aniId = ID_ANI_LUCKYBRICK_STATE5;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CLuckyBrickCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - LUCKYBRICK_BBOX_WIDTH / 2;
	t = y - LUCKYBRICK_BBOX_HEIGHT / 2;
	r = l + LUCKYBRICK_BBOX_WIDTH;
	b = t + LUCKYBRICK_BBOX_HEIGHT;
}

void CLuckyBrickCoin::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CLuckyBrickCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	D3DXVECTOR2 brick = this->GetPosition();
	if (state != LUCKYBRICK_STATE_DEFLECT && state != LUCKYBRICK_STATE5)
	{
		preY = brick.y;
	}

	if ((preY - brick.y) < 0.5 && state == LUCKYBRICK_STATE_DEFLECT && GetTickCount64() - fall_start > 10)
	{
		SetState(LUCKYBRICK_STATE5);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CLuckyBrickCoin::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LUCKYBRICK_STATE_DEFLECT:
		ay = 0.0005f;
		vy = -LUCKYBRICK_DEFLECT_SPEED;
		fall_start = GetTickCount64();
		break;
	case LUCKYBRICK_STATE5:
		ay = 0;
		vy = 0;
		break;
	}
}