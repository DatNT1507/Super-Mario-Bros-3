#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define BUTTON_STATE_IDLE			8642
#define BUTTON_STATE_CAN_BE_PUSHED	8643
#define BUTTON_STATE_PUSHED			8644

#define ID_ANI_BUTTON				7531
#define ID_ANI_BUTTON_PUSHED		7532 

class CButton : public CGameObject {
public:
	CButton(float x, float y) : CGameObject(x, y) 
	{
		state = BUTTON_STATE_IDLE;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
	float GetX() {
		return x;
	}
	float GetY() {
		return y;
	}
	bool push = false;
};