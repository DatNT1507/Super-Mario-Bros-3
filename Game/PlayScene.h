#pragma once

#include <vector>

#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Mushroom.h"
#include "LuckyBrick.h"
#include "LuckyBrickCoin.h"
#include "Block.h"
#include "Koopas.h"
#include "CheckFall.h"
#include "Map.h"
#include "GoombaFly.h"
#include "Leaf.h"
#include "PlantBullet.h"
#include "Fire.h"
#include "Pipe.h"
#include "KoopasFly.h"
#include "PlantPiranha.h"
#include "BreakableBrick.h"
#include "MapUnder.h"
#include "Button.h"
#include "TileMaps.h"
#include "Effects.h"
#include "LuckyBoxUp.h"
#include "Player.h"

class CPlayScene : public CScene
{
	CGame* game = CGame::GetInstance();
	//CTileMaps* tilemaps = CTileMaps::GetInstance();
	CTextures* textures = CTextures::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();
	vector<vector<LPSPRITE>> map;
	int numsRowInMap;
	int numsColInMap;
	float cameraIndexFollowY;

	vector<LPGAMEOBJECT> objects;
	//vector<CPath*> listPaths;
	int currentIdPath;

	//Take a sample like brick
	vector<LPGAMEOBJECT> listStaticObjectsToRender;
	//Take a sample such as mario, koopas
	vector<LPGAMEOBJECT> listMovingObjectsToRender;
	//For instance is mushroom, leaf, coin
	vector< LPGAMEOBJECT> listCanEarnObjectsToRender;
	//Mario jump into it
	vector< LPGAMEOBJECT> listPipeObjectsToRender;

	bool isGameOver = false;
protected:
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;

	LPGAMEOBJECT hud;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void _ParseSection_TILEMAP(string line);
	void _ParseSection_PATHOVERWORLD(string line);

	void LoadAssets(LPCWSTR assetFile);
	void LoadMap(LPCWSTR filePath);
	void RenderMap();

public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }

	void Clear();
	void PurgeDeletedObjects();
	void AddObject(LPGAMEOBJECT object) { objects.push_back(object); }
	void UpdateCurrentIdPath(int index) {
		if (this->id != 2) return;
		this->currentIdPath += index;
	}

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;