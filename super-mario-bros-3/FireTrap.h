#pragma once

#include "GameObject.h"

#define FIRETRAP_BBOX_WIDTH					16
#define FIRETRAP_BBOX_HEIGHT				32
#define FIRETRAP_DARTING_SPEED				0.03f
#define FIRETRAP_BBOX_DEATH_WIDTH			16
#define FIRETRAP_BBOX_DEATH_HEIGHT			24

#define FIRETRAP_STATE_DARTING				0
#define FIRETRAP_STATE_SHOOTING				100
#define FIRETRAP_STATE_DEATH				200
#define FIRETRAP_STATE_INACTIVE				300

#define FIRETRAP_ANI_RIGHT_UP			0
#define FIRETRAP_ANI_RIGHT_DOWN			1
#define FIRETRAP_ANI_LEFT_UP			2
#define FIRETRAP_ANI_LEFT_DOWN			3
#define FIRETRAP_ANI_DEATH				4

#define FIRETRAP_DELAY_TIME			750
#define FIRETRAP_DELAY_STOP_TIME	3000
#define FIRETRAP_AIM_TIME			750
#define FIRETRAP_DYING_TIME		250

#define PIRANHAPLANT_ACTIVE_RANGE		20

class CFireTrap:public CGameObject
{
protected:
	ULONGLONG delay_start;
	ULONGLONG delay_stop;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	int IsCollidable() { return 1; }
	int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
public:
	void GetDirection();
};

