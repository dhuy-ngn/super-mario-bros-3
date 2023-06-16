#pragma once

#include "GameObject.h"

#define MARIO_TAIL_BBOX_WIDTH 10
#define MARIO_TAIL_BBOX_HEIGHT 10

class CMarioTail : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

public:
	CMarioTail(float x, float y) : CGameObject(x, y) {};
};

