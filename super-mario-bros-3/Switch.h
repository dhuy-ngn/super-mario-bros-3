#pragma once

#include "GameObject.h"

#define SWITCH_BBOX_WIDTH			15
#define SWITCH_BBOX_HEIGHT			15
#define SWITCH_BBOX_PRESSED_HEIGHT	7

#define SWITCH_ANI_IDLE		8005
#define SWITCH_ANI_PRESSED	8006

#define SWITCH_STATE_IDLE		0
#define SWITCH_STATE_PRESSED	1

class CSwitch :public CGameObject
{
	float start_y;
public:
	CSwitch(float x, float y) : CGameObject(x, y)
	{
		this->start_y = y;
	}
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void SetState(int state);

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	void ChangeBreakBrickToCoin();
};
