#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "HUD.h"
#include "Koopa.h"
#include "FireTrap.h"
#include "PiranhaPlant.h"
#include "ColorBlock.h"
#include "QuestionBlock.h"
//#include "Koopas.h"

#define MAX_MAP_SIZE_X	2487


class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	CMario* player;		
	CHud* hud;

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> monsters; // koopas, goombas, firetraps, fire bullets, piranha fall into this category
	vector<LPGAMEOBJECT> special_platforms; // color blocks, bricks and question blocks fall into this category
	vector<LPGAMEOBJECT> power_ups; // mushrooms and leaves fall into this category
	vector<LPGAMEOBJECT> general_items; // everything else, except mario

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
	BOOLEAN IsMonster(LPGAMEOBJECT obj) 
	{
		return (dynamic_cast<CGoomba*>(obj)
			|| dynamic_cast<CKoopa*>(obj)
			|| dynamic_cast<CFireTrap*>(obj)
			|| dynamic_cast<CFireBullet*>(obj)
			|| dynamic_cast<CPiranhaPlant*>(obj));
	}
	BOOLEAN IsSpecialPlatform(LPGAMEOBJECT obj)
	{
		return (dynamic_cast<CColorBlock*>(obj)
			|| dynamic_cast<CQuestionBlock*>(obj)
			|| dynamic_cast<CBrick*>(obj));
	}
	BOOLEAN IsPowerUp(LPGAMEOBJECT obj)
	{
		return (dynamic_cast<CLeaf*>(obj)
			|| dynamic_cast<CMushroom*>(obj));
	}
};

typedef CPlayScene* LPPLAYSCENE;

