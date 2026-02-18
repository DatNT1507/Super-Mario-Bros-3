#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_MAP 8500

#define	MAP_WIDTH 2816
#define MAP_BBOX_WIDTH 2816
#define MAP_BBOX_HEIGHT 626

class CMap : public CGameObject {
public:
	CMap(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};