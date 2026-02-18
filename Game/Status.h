#pragma once
#include <string>
#include <iostream>

#include "debug.h"
#include "PlayScene.h"

class CStatus
{
private:
	bool isInit;
	static CStatus* instance;

	int id_world;
	int coin;
	int life;
	int point;
	int time;

	CStatus();
	~CStatus()
	{
	}
public:
	static CStatus* getInstance()
	{
		if (instance == NULL)
		{
			DebugOut(L"[INFO]Creating DataManager.\n");
			instance = new CStatus;
		}
		return instance;
	}

	static void Release()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}

	void startInit()
	{
		if (isInit == true) return;
		isInit = true;
	}

	void Unload()
	{
		isInit = false;
	}

	void GetData(int& id_world, int& coin, int& life, int& point);
	void SaveData(int coin, int life, int point, int time);
	void UpdateIdWorld(int id_world) { this->id_world = id_world; }
	void AddPoint(int point) { this->point += point; }
	void AddCoin(int coin) { this->coin += coin; }
	void AddLife(int life) { this->life += life; }
	void UpdatePlayTime(int time) { this->time = time; }

};
typedef CStatus* LPDATAMANAGER;