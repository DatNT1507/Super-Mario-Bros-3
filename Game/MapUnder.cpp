#include "MapUnder.h"
#include "Block.h"

void CMapUnder::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_BLOCK)->Render(x, y);
	//RenderBoundingBox();
}

void CMapUnder::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MAPUNDER_BBOX_WIDTH / 2;
	t = y - MAPUNDER_BBOX_HEIGHT / 2;
	r = l + MAPUNDER_BBOX_WIDTH;
	b = t + MAPUNDER_BBOX_HEIGHT;
}