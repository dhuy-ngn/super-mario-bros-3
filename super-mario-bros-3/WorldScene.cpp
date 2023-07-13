#include <iostream>
#include <fstream>

#include "WorldScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Brick.h"
#include "debug.h"
#include "Mario.h"
#include "WorldMapMario.h"
#include "WorldMapObject.h"

#define SCENE_SECTION_UNKNOWN		-1
#define SCENE_SECTION_ASSETS		1
#define SCENE_SECTION_OBJECTS		2
#define SCENE_SECTION_SPRITES		3
#define SCENE_SECTION_ANIMATIONS	4


#define ASSETS_SECTION_UNKNOWN		-1
#define ASSETS_SECTION_SPRITES		1
#define ASSETS_SECTION_ANIMATIONS	2

#define MAX_SCENE_LINE	1024

using namespace std;

CWorldScene::CWorldScene(int id, LPCWSTR filePath) : CScene(id, filePath)
{
	player = NULL;
	hud = NULL;
	key_handler = new CWorldSceneKeyHandler(this);
}

void CWorldScene::_ParseSection_SPRITES(string line)
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

void CWorldScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (unsigned int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

#define OBJECT_TYPE_PLAYER		0
#define OBJECT_TYPE_GENERAL		1
#define OBJECT_TYPE_NODE		2
#define OBJECT_NODE_PORTAL		3


/*
	Parse a line in section [OBJECTS]
*/
void CWorldScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 2) return; // skip invalid lines - an object set must have at least id, x, y
	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_PLAYER:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] PLAYER object was created before!\n");
			return;
		}
		obj = new CWorldMapMario(x, y);
		player = (CWorldMapMario*)obj;
		DebugOut(L"[INFO] Player object created! %f %f\n", x, y);
		break;
	case OBJECT_TYPE_NODE:
	case OBJECT_TYPE_PORTAL:
	{
		bool cgLeft, cgRight, cgUp, cgDown;
		cgLeft = atoi(tokens[3].c_str());
		cgUp = atoi(tokens[4].c_str());
		cgRight = atoi(tokens[5].c_str());
		cgDown = atoi(tokens[6].c_str());
		int scene_id = atoi(tokens[7].c_str());
		int ani_id = atoi(tokens[8].c_str());
		obj = new CWorldMapObject(object_type, scene_id, ani_id);
		((CWorldMapObject*)obj)->SetDirection(cgLeft, cgUp, cgRight, cgDown);
		break;
	}
	case OBJECT_TYPE_GENERAL:
	{
		int object_type = atoi(tokens[3].c_str());
		int ani_id = atoi(tokens[4].c_str());
		obj = new CWorldMapObject(object_type, -1, ani_id);
		if (object_type == OBJECT_TYPE_HAMMER)
			obj->SetSpeed(MARIO_WALKING_SPEED_MIN / 2, 0);
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	objects.push_back(obj);
}

void CWorldScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[ASSETS]") {
			section = SCENE_SECTION_ASSETS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }
		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		}
	}

	f.close();

	CGame::GetInstance()->SetCamPos(-5, 0);
	hud = new CHud(HUD_TYPE_WORLDSCENE);
	hud->SetPosition(126, 187);

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CWorldScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{

		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
		objects[i]->Update(dt, &coObjects);

	hud->Update(dt);
}

void CWorldScene::Render()
{
	for (unsigned int i = 0; i < objects.size(); i++)
		objects[i]->Render();
	 hud->Render();
}

void CWorldScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}

void CWorldScene::LoadAssets(LPCWSTR assetFile)
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

/*
	Unload current scene
*/
void CWorldScene::Unload()
{
	for (unsigned int i = 0; i < objects.size() - 1; i++)
		delete objects[i];
	objects.clear();
	delete hud;
	player = NULL;
	hud = nullptr;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
void CWorldSceneKeyHandler::OnKeyDown(int KeyCode)
{
	LPGAME game = CGame::GetInstance();
	CWorldMapMario* player = ((CWorldScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (player != NULL)
	{
		switch (KeyCode)
		{
		case DIK_RIGHT:
			if (player->cgRight)
				player->SetState(PLAYER_STATE_RIGHT);
			break;
		case DIK_LEFT:
			if (player->cgLeft)
				player->SetState(PLAYER_STATE_LEFT);
			break;
		case DIK_UP:
			if (player->cgUp)
				player->SetState(PLAYER_STATE_UP);
			break;
		case DIK_DOWN:
			if (player->cgDown)
				player->SetState(PLAYER_STATE_DOWN);
			break;
		case DIK_S:
			if (player->IsReadyToSwitchScene()) 
			{
				player->GoToPlayScene();
			}
			break;
		}
	}
}