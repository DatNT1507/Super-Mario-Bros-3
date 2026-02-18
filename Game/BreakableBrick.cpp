#include "BreakableBrick.h"
#include "Brick.h"
#include "Button.h"
#include "Coin.h"
#include "PlayScene.h"

void CBreakableBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_BREAKABLEBRICK)->Render(x, y);
	//RenderBoundingBox();
}

void CBreakableBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y - BRICK_BBOX_HEIGHT / 2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void CBreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CButton* button = nullptr;

	for (size_t i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CButton*>(obj))
		{
			button = dynamic_cast<CButton*>(obj);
			break;
		}
	}
	if (button->push)
	{
		D3DXVECTOR2 brick = this->GetPosition();
		CCoin* coin = new CCoin(brick.x, brick.y);
		coin->SetPosition(brick.x, brick.y);
		coin->SetState(COIN_IDLE);
		CPlayScene* playScene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
		if (playScene)
		{
			playScene->AddObject(coin);
		}
		Delete();
	}

	/*CGameObject::Update(dt, coObjects);*/
	CCollision::GetInstance()->Process(this, dt, coObjects);
}