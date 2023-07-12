#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "HUD.h"
//#include "Koopas.h"

#define MAX_MAP_SIZE_X	2487


class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	CMario* player;		
	CHud* hud;

	vector<LPGAMEOBJECT> objects;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);
	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	vector<LPGAMEOBJECT> GetAllObject() { return this->objects; }

	LPGAMEOBJECT GetPlayer() { return player; }
	void SetPlayer(CMario* p) {
		player = p; 
		objects.push_back(player); 
		if (p->GetLevel() == MARIO_LEVEL_RACCOON)
			p->SetLevel(MARIO_LEVEL_RACCOON);
	}

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);

	void PushObject(LPGAMEOBJECT obj) { this->objects.push_back(obj); }
	void UnshiftObject(LPGAMEOBJECT obj) { this->objects.insert(objects.begin() + 1, obj); }
};

typedef CPlayScene* LPPLAYSCENE;

