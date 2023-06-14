#pragma once
#include "GameObject.h"

#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_WALKING_SPEED 0.05f


#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 6

#define GOOMBA_DIE_TIMEOUT 500

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_KNOCKED_OUT	300	
#define GOOMBA_STATE_SKIPPING	400

#define ID_ANI_YELLOW_GOOMBA_WALKING 5000
#define ID_ANI_YELLOW_GOOMBA_DIE 5001
#define ID_ANI_RED_PARAGOOMBA_SKIPPING	5002
#define ID_ANI_RED_GOOMBA_WALKING 5003
#define ID_ANI_RED_GOOMBA_DIE	5004

#define GOOMBA_LEVEL_NORMAL 0
#define GOOMBA_LEVEL_PARA	1

#define GOOMBA_COLOR_YELLOW	0
#define GOOMBA_COLOR_RED	1

class CGoomba : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;

	int color; // 1: red; 0: yellow
	int level; // 1: para; 0: normal

	int paragoomba_jump_stack;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	int IsCollidable() { return state != GOOMBA_STATE_KNOCKED_OUT; }
	int IsBlocking() { return state != GOOMBA_STATE_KNOCKED_OUT; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);

public:
	CGoomba(float x, float y, int color = 0, int level = 0) : CGameObject(x, y) {
		this->color = color;
		this->level = level;
		this->ax = 0;
		this->ay = GOOMBA_GRAVITY;
		die_start = -1;
		SetState(GOOMBA_STATE_WALKING);
		this->paragoomba_jump_stack = 0;
	};
	virtual void SetState(int state);
	int GetAniIdRedGoomba();
	int GetAniIdYellowGoomba();
	int GetLevel() { return this->level; }
	void SetLevel(int level) { this->level = level; }
};