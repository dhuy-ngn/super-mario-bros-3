#pragma once
#include "GameObject.h"
#include "Game.h"
#include "FireBullet.h"
#include "Utils.h"
#include "Portal.h"


#define PLAYER_SPEED	0.2f

#define PLAYER_STATE_IDLE			0
#define PLAYER_STATE_RIGHT			100
#define PLAYER_STATE_LEFT			111
#define PLAYER_STATE_UP				200
#define PLAYER_STATE_DOWN			201

#define PLAYER_BBOX_WIDTH	8	

// mario ani
#define ID_ANI_MARIO_SMALL		501
#define ID_ANI_MARIO_BIG		502
#define ID_ANI_MARIO_RACCOON	503

class CWorldMapMario : public CGameObject
{
protected:
	int level;
	int coin;
	int life;
	int score;
	float start_x;
	float start_y;
	BOOLEAN isReadyToSwitchScene = false;

public:
	bool cgLeft, cgRight, cgUp, cgDown;
	int sceneId;
	CWorldMapMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);

	void SetLevel(int l) { level = l; };
	void SetCoin(int coin) { this->coin = coin; }
	void SetLife(int life) { this->life = life; }
	void SetScore(int score) { this->score = score; }
	void SetMove(bool cLeft, bool cUp, bool cRight, bool cDown)
	{
		cgLeft = cLeft;
		cgRight = cRight;
		cgUp = cUp;
		cgDown = cDown;
	};

	void GoToPlayScene()
	{
		int scene_id = 5;
		CGame::GetInstance()->InitiateSwitchScene(scene_id);
	}

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithNode(LPCOLLISIONEVENT e);
	int GetLevel() { return this->level; }
	int GetCoin() { return this->coin; }
	int GetLife() { return this->life; }
	int GetScore() { return this->score; }
	BOOLEAN IsReadyToSwitchScene() { return this->isReadyToSwitchScene; }
};