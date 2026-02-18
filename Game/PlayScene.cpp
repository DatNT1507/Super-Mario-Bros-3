#include <iostream>
#include <fstream>
#include "AssetIDs.h"
#include <sstream>

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "Define.h"

#include "SampleKeyEventHandler.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	player = NULL;
	hud = NULL;
	cameraIndexFollowY = 0;
	key_handler = new CSampleKeyHandler(this);
	numsRowInMap = 0;
	numsColInMap = 0;
	currentIdPath = 0;
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_TILEMAP 3
#define SCENE_SECTION_PATHOVERWORLD 4

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_TILEMAP(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 8) return;
	LPCWSTR filePath = ToLPCWSTR(tokens[0]);
	int mapWidth = atoi(tokens[1].c_str());
	int mapHeight = atoi(tokens[2].c_str());
	int tileWidth = atoi(tokens[3].c_str());
	int tileHeight = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());
	int texWidth = atoi(tokens[6].c_str());
	int texHeight = atoi(tokens[7].c_str());

	this->numsColInMap = mapWidth / tileWidth;
	this->numsRowInMap = mapHeight / tileHeight;
	int numsColInTex = texWidth / tileWidth;
	int numsRowInTex = texHeight / tileHeight;
	int IDSprite = 0;

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}
	for (int i = 0; i < numsRowInTex; i++)
	{
		for (int j = 0; j < numsColInTex; j++)
		{
			int ID = ID_SPRITE_TILE + IDSprite;
			CSprites::GetInstance()->Add(ID, tileWidth * j, tileHeight * i, tileWidth * (j + 1) - 1, tileHeight * (i + 1) - 1, tex);
			IDSprite++;
		}
	}
	LoadMap(filePath);
}


void CPlayScene::_ParseSection_PATHOVERWORLD(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < 8) return;
	int idPath = atoi(tokens[0].c_str());
	float pathX = (float)atof(tokens[1].c_str());
	float pathY = (float)atof(tokens[2].c_str());
	int directUp = atoi(tokens[3].c_str());
	int directDown = atoi(tokens[4].c_str());
	int directRight = atoi(tokens[5].c_str());
	int directLeft = atoi(tokens[6].c_str());
	int scene_id = atoi(tokens[7].c_str());
	DebugOut(L"[INFO]%d, %f, %f, %d, %d, %d, %d\n", idPath, pathX, pathY, directUp, directDown, directRight, directLeft);
}
/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(x, y); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(x, y); break;
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;
	case OBJECT_TYPE_MUSHROOM: obj = new CMushroom(x, y); break;
	case OBJECT_TYPE_LUCKYBRICK: obj = new CLuckyBrick(x, y); break;
	case OBJECT_TYPE_LUCKYBRICKCOIN: obj = new CLuckyBrickCoin(x, y); break;
	case OBJECT_TYPE_BLOCK: obj = new CBlock(x, y); break;
	case OBJECT_TYPE_KOOPAS: obj = new CKoopas(x, y); break;
	case OBJECT_TYPE_CHECKFALL: obj = new CCheckFall(x, y); break;
	case OBJECT_TYPE_MAP: obj = new CMap(x, y); break;
	case OBJECT_TYPE_GOOMBAFLY: obj = new CGoombafly(x, y); break;
	case OBJECT_TYPE_LEAF: obj = new CLeaf(x, y); break;
	case OBJECT_TYPE_PLANTBULLET: obj = new CPlantBullet(x, y); break;
	case OBJECT_TYPE_FIRE: obj = new CFire(x, y); break;
	case OBJECT_TYPE_PIPE: obj = new CPipe(x, y); break;
	case OBJECT_TYPE_KOOPASFLY: obj = new CKoopasFly(x, y); break;
	case OBJECT_TYPE_PLANTPIRANHA: obj = new CPlantPiranha(x, y); break;
	case OBJECT_TYPE_BREAKABLEBRICK: obj = new CBreakableBrick(x, y); break;
	case OBJECT_TYPE_MAPUNDER: obj = new CMapUnder(x, y); break;
	case OBJECT_TYPE_BUTTON: obj = new CButton(x, y); break;
	case OBJECT_TYPE_EFFECTS: obj = new CEffects(x, y, EFFECT_TYPE_POINT, 400); break;
	case OBJECT_TYPE_LUCKYBOXUP: obj = new CLuckyBoxUp(x, y); break;
	case OBJECT_TYPE_PLAYER:
	{
		obj = new CPlayer(x, y);
		hud = (CPlayer*)obj;
		break;
	}

	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}

	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;


	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);


	objects.push_back(obj);
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line == "[TILEMAP]") { section = SCENE_SECTION_TILEMAP; continue; };
		if (line == "[PATHOVERWORLD]") { section = SCENE_SECTION_PATHOVERWORLD; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_TILEMAP: _ParseSection_TILEMAP(line); break;
		case SCENE_SECTION_PATHOVERWORLD: _ParseSection_PATHOVERWORLD(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
	hud->Update(dt);
	CMario* mario = (CMario*)player;
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();
	cx -= game->GetBackBufferWidth() / 2;
	cy -= game->GetBackBufferHeight() / 2;
	
	if (cx < 0) cx = 0;
	else if (cx > 2495) cx = 2495;
	if (cy < 80)	cy = 50;
	if (cy > 120 && cy < 325)
		CGame::GetInstance()->SetCamPos(cx, 245 /*cy*/);
	else if (cy < 120)
		CGame::GetInstance()->SetCamPos(cx, cy);
	else if (cy > 325 && mario->inRoom)
		CGame::GetInstance()->SetCamPos(2048, 435);
	PurgeDeletedObjects();
}

void CPlayScene::RenderMap()
{
	D3DXVECTOR3 camPosition = game->GetCameraPositon();
	int startColDraw = (int)camPosition.x / TILE_WIDTH;
	int endColDraw = startColDraw + SCREEN_WIDTH / TILE_WIDTH;

	for (int i = 0; i < numsRowInMap; i++)
	{
		for (int j = startColDraw; j <= endColDraw; j++)
		{
			float x = (j - startColDraw) * TILE_WIDTH * 1.0f + camPosition.x - (int)camPosition.x % TILE_WIDTH;
			float y = i * TILE_HEIGHT * 1.0f;
			map[i][j]->Draw(x, y);
		}
	}
}

void CPlayScene::Render()
{
	RenderMap();
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}

void CPlayScene::LoadMap(LPCWSTR filePath) {
	DebugOut(L"[INFO] Start loading map from : %s \n", filePath);
	CSprites* sprites = CSprites::GetInstance();
	ifstream f;
	f.open(filePath);
	if (f.fail())
	{
		f.close();
		return;
	}
	int ID = 0;
	int curRow = 0;
	int curCol = 0;
	string line;
	while(!f.eof())
	{
		getline(f, line);
		vector<LPSPRITE> spriteLine;
		stringstream ss(line);
		int n;
		while (ss >> n)
		{
			ID = n;
			spriteLine.push_back(sprites->Get(ID));
		}
		map.push_back(spriteLine);
	}
	f.close();
	return;
}