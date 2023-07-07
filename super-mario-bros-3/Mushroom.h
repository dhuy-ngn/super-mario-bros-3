#pragma once

#include "GameObject.h"

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

#define MUSHROOM_STATE_IDLE 100
#define MUSHROOM_STATE_MOVE 200
#define MUSHROOM_STATE_DELETED 300

#define MUSHROOM_MOVING_SPEED 0.08f
#define MUSHROOM_GROWING_UP_SPEED -0.015f
#define MUSHROOM_GRAVITY 0.2f

#define MUSHROOM_TYPE_RED	0
#define	MUSHROOM_TYPE_GREEN 1

#define ID_ANI_MUSHROOM_RED	8001
#define ID_ANI_MUSHROOM_GREEN 8002

class CMushroom : public CGameObject
{
protected:
	int type;
	float start_x = this->x;
	float start_y = this->y;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CMushroom(float x, float y, int type = MUSHROOM_TYPE_RED) : CGameObject(x, y)
	{
		this->type = type;
	};
	virtual void SetState(int state);
};