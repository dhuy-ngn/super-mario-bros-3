#include <iostream>
#include <fstream>

#include "IntroScene.h"
#include "Textures.h"
#include "Utils.h"
#include "Brick.h"
#include "Brick.h"
#include "Leaf.h"
#include "MushRoom.h"
#include "Goomba.h"
#include "Koopa.h"
#include "debug.h"
#include "Game.h"

using namespace std;

CIntroScene::CIntroScene(int id, LPCWSTR filePath) : CScene(id, filePath)
{
	key_handler = new CIntroSceneKeyHandler(this);
	background = nullptr;
	three = nullptr;
	arrow = nullptr;
}

void CIntroScene::_ParseSection_SPRITES(string line) {
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

void CIntroScene::_ParseSection_ANIMATIONS(string line) {
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

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

void CIntroScene::_ParseSection_OBJECTS(string line) {
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y
	int tag = 0, option_tag_1 = 0, option_tag_2 = 0;
	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());
	CGameObject* obj = NULL;

	obj->SetPosition(x, y);

	objects.push_back(obj);
}

void CIntroScene::SetState(int state)
{
	switch (state)
	{
	case INTRO_SCENE_STATE_ARROW_UP:
		isArrowUp = true;
		break;
	case INTRO_SCENE_STATE_ARROW_DOWN:
		isArrowUp = false;
		break;
	}
}

void CIntroScene::Update(DWORD dt) {
	if (switch_scene_start >= SWITCH_TIME && isSwitch) {
		CGame* game = CGame::GetInstance();
		game->InitiateSwitchScene(WORLD_SCENE_ID);
	}
}

void CIntroScene::Load() {
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;
	DebugOut(L"%d", section);
	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[SPRITES]") { section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line[0] == '[' || line == "") { section = SCENE_SECTION_UNKNOWN; continue; }
		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CIntroScene::Render() {
	//BACKGROUND
	CSprites::GetInstance()->Get(ID_SPRITE_BACKGROUND)->Draw(130, 107);
	//3 SIGN
	CAnimations::GetInstance()->Get(ID_ANI_THREE)->Render(THREE_X, THREE_Y);
	//ARROW
	if (isArrowUp)
	{
		CSprites::GetInstance()->Get(ID_SPRITE_ARROW)->Draw(ARROW_X, ARROW_Y);
	}
	else
	{
		CSprites::GetInstance()->Get(ID_SPRITE_ARROW)->Draw(ARROW_X, ARROW_Y + 16);
	}
	for (size_t i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

void CIntroScene::Unload() {

	for (size_t i = 2; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	background = NULL;
	arrow = NULL;
	three = NULL;
	switch_scene_start = 0;
	isSwitch = false;

	DebugOut(L"Unload Intro Scene\n");
}

void CIntroSceneKeyHandler::OnKeyDown(int KeyCode)
{
	CIntroScene* intro = ((CIntroScene*)CGame::GetInstance()->GetCurrentScene());
	switch (KeyCode)
	{
	case DIK_UP:
		intro->SetState(INTRO_SCENE_STATE_ARROW_UP);
		break;
	case DIK_DOWN:
		intro->SetState(INTRO_SCENE_STATE_ARROW_DOWN);
		break;
	case DIK_S:
		CGame::GetInstance()->InitiateSwitchScene(WORLD_SCENE_ID);
		break;
	default:
		break;
	}
}