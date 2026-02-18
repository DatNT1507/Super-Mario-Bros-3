#include "BreakableBrick.h"
#include "Brick.h"
#include "Button.h"

void CButton::Render()
{
	int aniId = ID_ANI_BREAKABLEBRICK;
	if (state == BUTTON_STATE_CAN_BE_PUSHED)
	{
		aniId = ID_ANI_BUTTON;
	}
	else if (state == BUTTON_STATE_PUSHED)
	{
		aniId = ID_ANI_BUTTON_PUSHED;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CButton::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y - BRICK_BBOX_HEIGHT / 2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void CButton::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	
	}
}