#pragma once

#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "HUD.h"
#include "WorldMapMario.h"

#define MARIO_WALKING_SPEED_MIN		0.05f

class CWorldScene : public CScene
{
protected:
	CWorldMapMario* player;
	CHud* hud;
	vector<LPGAMEOBJECT> objects;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_OBJECTS(string line);


public:
	CWorldScene(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
};

class CWorldSceneKeyHandler : public CSceneKeyHandler
{
public:
	virtual void KeyState(BYTE* states) {};
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CWorldSceneKeyHandler(CScene* s) :CSceneKeyHandler(s) {};
};