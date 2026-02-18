#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define	MAPUNDER_WIDTH 20
#define MAPUNDER_BBOX_WIDTH 20
#define MAPUNDER_BBOX_HEIGHT 20

class CMapUnder : public CGameObject {
public:
	CMapUnder(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};