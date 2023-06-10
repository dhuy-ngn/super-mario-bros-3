#pragma once

#include "GameObject.h"

#define PIRANHAPLANT_BBOX_WIDTH					16
#define PIRANHAPLANT_GREEN_BBOX_HEIGHT			24
#define PIRANHAPLANT_RED_BBOX_HEIGHT			32
#define PIRANHAPLANT_DARTING_SPEED				0.03f
#define PIRANHAPLANT_BBOX_DEATH_WIDTH			16
#define PIRANHAPLANT_BBOX_DEATH_HEIGHT			24

#define PIRANHAPLANT_STATE_DARTING				0
#define PIRANHAPLANT_STATE_SHOOTING				100
#define PIRANHAPLANT_STATE_DEATH				200
#define PIRANHAPLANT_STATE_INACTIVE				300

#define PIRANHAPLANT_ANI_RIGHT_UP			0
#define PIRANHAPLANT_ANI_RIGHT_DOWN			1
#define PIRANHAPLANT_ANI_LEFT_UP			2
#define PIRANHAPLANT_ANI_LEFT_DOWN			3
#define PIRANHAPLANT_ANI_DEATH				4

#define PIRANHAPLANT_DELAY_TIME			750
#define PIRANHAPLANT_DELAY_STOP_TIME	3000
#define PIRANHAPLANT_AIM_TIME			750
#define PIRANHAPLANT_DIYING_TIME		250

#define PIRANHAPLANT_ACTIVE_RANGE		20

class CFireTrap:public CGameObject
{
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	int IsCollidable() { return 1; }
	int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
public:
	void GetDirection();
};

