#pragma once
#include "GameObject.h"

#define LEAF_BBOX_WIDTH  16
#define LEAF_BBOX_HEIGHT 16

#define LEAF_ANI_RIGHT		8004
#define LEAF_ANI_LEFT		8003

#define LEAF_UP_HEIGHT	 50

#define LEAF_GRAVITY		0.02f
#define LEAF_SPEED			0.08f
#define LEAF_SPEED_UP		0.05f

#define LEAF_STATE_IDLE		0
#define LEAF_STATE_UP		100
#define LEAF_STATE_FALLING	200

#define LEAF_MAX_FLOATING_TIME	500


class CLeaf:public CGameObject
{
	float start_x = this->x;
	float start_y = this->y;
	ULONGLONG falling_start_time = -1;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }


public:
	CLeaf(float x, float y) : CGameObject(x, y) {};
	virtual void SetState(int state);
	void StartFallingTime() { falling_start_time = GetTickCount64(); }
};