#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK 10000
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define BRICK_ITEM_TYPE_MUSHROOM	1
#define BRICK_ITEM_TYPE_SWITCH		2
#define BRICK_STATE_EXHAUSTED	1000

#define	BRICK_DEFLECT_SPEED 0.1f
#define BRICK_DEFLECT_DURATION 25

class CBrick : public CGameObject {
protected:
	int item;
	ULONGLONG deflect_start = 0;
	ULONGLONG deflect_end = 0;

	float start_y;
public:
	CBrick(float x, float y, int item = 0) : CGameObject(x, y)
	{
		this->start_y = y;
		this->item = item;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 1; }
	void SetState(int state);

	int GetItemType() { return this->item; }
};