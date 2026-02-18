#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f

#define MARIO_ACCEL_WALK_X	0.0001f
#define MARIO_ACCEL_RUN_X	0.0002f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f

#define MARIO_GRAVITY			0.002f

#define MARIO_JUMP_DEFLECT_SPEED  0.4f

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_STATE_HOLD			602
#define MARIO_STATE_SUPER_LEFT		603
#define MARIO_STATE_SUPER_RIGHT		604


#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_HOLD_IDLE_RIGHT 1010
#define ID_ANI_MARIO_HOLD_IDLE_LEFT 1011
#define ID_ANI_MARIO_HOLD_RIGHT 1012
#define ID_ANI_MARIO_HOLD_LEFT 1013
#define ID_ANI_MARIO_KICK_RIGHT 1014
#define ID_ANI_MARIO_KICK_LEFT 1015

#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

//Super mario
#define ID_ANI_MARIO_SUPER_IDLE_RIGHT 1700
#define ID_ANI_MARIO_SUPER_IDLE_LEFT 1701

#define ID_ANI_MARIO_SUPER_WALKING_RIGHT 1800
#define ID_ANI_MARIO_SUPER_WALKING_LEFT 1801

#define ID_ANI_MARIO_SUPER_RUNNING_RIGHT 1900
#define ID_ANI_MARIO_SUPER_RUNNING_LEFT 1901

#define ID_ANI_MARIO_SUPER_BRACE_RIGHT 2300
#define ID_ANI_MARIO_SUPER_BRACE_LEFT 2301

#define ID_ANI_MARIO_SUPER_JUMP_WALK_RIGHT 2000
#define ID_ANI_MARIO_SUPER_JUMP_WALK_LEFT 2001

#define ID_ANI_MARIO_SUPER_JUMP_RUN_RIGHT 2100
#define ID_ANI_MARIO_SUPER_JUMP_RUN_LEFT 2101

#define ID_ANI_MARIO_SUPER_SIT_RIGHT 2200
#define ID_ANI_MARIO_SUPER_SIT_LEFT 2201

#define ID_ANI_MARIO_SUPER_HIT_LEFT 2400
#define ID_ANI_MARIO_SUPER_HIT_RIGHT 2401

#define ID_ANI_MARIO_SUPER_FLY_RIGHT	2402
#define ID_ANI_MARIO_SUPER_FLY_LEFT		2403

#define ID_ANI_MARIO_SUPER_FLY_RIGHT_SLOW	2404
#define ID_ANI_MARIO_SUPER_FLY_LEFT_SLOW		2405

#pragma endregion

#define GROUND_Y 160.0f




#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define	MARIO_LEVEL_SUPER	3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12


#define MARIO_UNTOUCHABLE_TIME 2500
#define MARIO_HIT_TIME 600
#define MARIO_FLY_TIME 200
#define MARIO_FLY_KICK 200
#define MARIO_UNTOUCH_TIME		400

class CMario : public CGameObject
{
	BOOLEAN isSitting;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 
	BOOLEAN untouch;

	int level;
	int untouchable;
	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;
	int coin;
	ULONGLONG timeHit;
	ULONGLONG timeFly;
	ULONGLONG timeKick;
	ULONGLONG untouch_start;

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithLuckyBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithLuckyBrickCoin(LPCOLLISIONEVENT e);
	//void OnCollisionWithBlock(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopas(LPCOLLISIONEVENT e);
	void OnCollisionWithGoombaFly(LPCOLLISIONEVENT e);
	void OnCollisionWithLeaf(LPCOLLISIONEVENT e);
	void OnCollisionWithPlantBullet(LPCOLLISIONEVENT e);
	void OnCollisionWithFire(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopasFly(LPCOLLISIONEVENT e);
	void OnCollisionWithPlantPiranha(LPCOLLISIONEVENT e);
	void OnCollisionWithBreakableBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithMapUnder(LPCOLLISIONEVENT e);
	void OnCollisionWithButton(LPCOLLISIONEVENT e);
	void OnCollisionWithLuckyBoxUp(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdSuper();

public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY;

		level = MARIO_LEVEL_SMALL;
		untouchable = 0;
		untouchable_start = -1;
		untouch_start = -1;
		timeHit = -1;
		timeFly = -1;
		timeKick = -1;
		untouch = false;
		isOnPlatform = false;
		coin = 0;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable()
	{
		return (state != MARIO_STATE_DIE);
	}

	/*int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable == 0); }*/
	int IsBlocking() { return 0; }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	int GetLevel()
	{
		return this->level;
	}
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }
	void StartHit() { timeHit = GetTickCount64(); }
	void StartFly() { timeFly = GetTickCount64(); }
	void StartKick() { timeKick = GetTickCount64(); }
	void StartUntouch() { untouch = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	D3DXVECTOR2 GetPosition()
	{
		return D3DXVECTOR2(x, y);
	}
	bool isJKeyDown = false;
	bool isJKeyDownHit = false;
	bool hitLeft = false;
	bool hitRight = false;
	bool isFly = false;
	bool aniFly = false;
	bool isHold = false;
	bool isKick = false;
	bool tele = false;
	bool inRoom = false;
	bool isSit = false;
};