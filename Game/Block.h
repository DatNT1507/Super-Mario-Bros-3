#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BLOCK 5500

#define	BLOCK_WIDTH 47
#define BLOCK_BBOX_WIDTH 47
#define BLOCK_BBOX_HEIGHT 47

class CBlock : public CGameObject {
public:
	CBlock(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	D3DXVECTOR2 GetPosition()
	{
		return D3DXVECTOR2(x, y);
	}
	int IsDirectionColliable(float nx, float ny);
};