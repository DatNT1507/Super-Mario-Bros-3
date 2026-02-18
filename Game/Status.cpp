#include "Status.h"

CStatus* CStatus::instance = NULL;

CStatus::CStatus()
{
	isInit = false;
	id_world = 1;
	coin = 0;
	life = 4;
	point = 0;
	time = 300;
}

void CStatus::GetData(int& id_world, int& coin, int& life, int& point)
{
	id_world = this->id_world;
	coin = this->coin;
	life = this->life;
	point = this->point;
}

void CStatus::SaveData(int coin, int life, int point, int time)
{
	this->coin = coin;
	this->life = life;
	this->point = point;
	this->time = time;
}