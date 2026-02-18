#include "Player.h"
#include "Status.h"
#include "AssetIDs.h"
CPlayer::CPlayer(float x, float y) :CGameObject(x, y)
{
	game = CGame::GetInstance();
	mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	information = "";
	sprite = CSprites::GetInstance()->Get(ID_SPRITE_PLAYER);
	id_world = 0;
	coin = 0;
	life = 0;
	point = 0;
	time = 0;
	posHudX = x;
	posHudY = y;
}

CPlayer::~CPlayer() {}

void CPlayer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	UpdateIndex();
	int remainTime = 0;
	if (mario->GetState() != MARIO_STATE_DIE) time += dt;
	remainTime = DEFAULT_TIME_PLAY - time / CLOCKS_PER_SEC;
	CStatus::getInstance()->UpdatePlayTime(time / CLOCKS_PER_SEC);
	if (remainTime <= 0)
	{
		remainTime = 0;
		mario->SetState(MARIO_STATE_DIE);
	}
	string point_str = to_string(point);
	while (point_str.length() < 7) point_str = "0" + point_str;

	string time_str = to_string(remainTime);
	while (time_str.length() < 3) time_str = "0" + time_str;

	string world_str = to_string(id_world);

	string life_str = to_string(life);

	string coin_str = to_string(coin);
	while (coin_str.length() < 2) coin_str = "0" + coin_str;

	information = "                         " + world_str + "                             " + coin_str + "\n";
	information += "                         " + life_str + " " + point_str + "       " + time_str + "\n";
	CGameObject::Update(dt, coObjects);
}

void CPlayer::Render()
{
	sprite->Draw(posHudX, posHudY, 1.0f);
	RECT rect;
	SetRect(&rect, 0, 175, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (game->GetFont() != NULL)
		game->GetFont()->DrawTextA(game->GetSpriteHandler(), information.c_str(), -1, &rect, DT_LEFT, D3DXCOLOR(255 / 255, 255 / 255, 255 / 255, 100));
}

void CPlayer::UpdateIndex()
{
	CStatus::getInstance()->GetData(id_world, coin, life, point);
}