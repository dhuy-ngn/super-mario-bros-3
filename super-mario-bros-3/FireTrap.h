#pragma once

#include "GameObject.h"
#include "FireBullet.h"

#define FIRETRAP_BBOX_WIDTH					16
#define FIRETRAP_BBOX_HEIGHT				32
#define FIRETRAP_DARTING_SPEED				0.03f
#define FIRETRAP_BBOX_DEATH_WIDTH			16
#define FIRETRAP_BBOX_DEATH_HEIGHT			24

#define FIRETRAP_STATE_DARTING				0
#define FIRETRAP_STATE_SHOOTING				100
#define FIRETRAP_STATE_DEATH				200
#define FIRETRAP_STATE_INACTIVE				300

#define FIRETRAP_ANI_RIGHT_UP			7100
#define FIRETRAP_ANI_RIGHT_DOWN			7101
#define FIRETRAP_ANI_LEFT_UP			7110
#define FIRETRAP_ANI_LEFT_DOWN			7111

#define FIRETRAP_DELAY_TIME			750
#define FIRETRAP_DELAY_STOP_TIME	3000
#define FIRETRAP_AIM_TIME			750
#define FIRETRAP_DYING_TIME		250

#define PIRANHAPLANT_ACTIVE_RANGE		20

class CFireTrap:public CGameObject
{
protected:
	ULONGLONG delay_start = 0;
	ULONGLONG delay_stop = 0;
	ULONGLONG dying_start = 0;
	ULONGLONG shooting_start = 0;
	ULONGLONG aim_start = 0;
	float maxY = 0;
	float height = 0;
	BOOLEAN isUpward = false;
	BOOLEAN isForward = false;
	CFireBullet* bullet = NULL;
public:
	CFireTrap(float x, float y) {
		this->maxY = y - FIRETRAP_BBOX_HEIGHT;
		this->height = FIRETRAP_BBOX_HEIGHT;

		SetState(FIRETRAP_STATE_DARTING);
	}

	void SetState(int state);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();

	int IsCollidable() { return 1; }
	int IsBlocking() { return 0; }

	void GetDirection();
	void StartDelay() { delay_start = GetTickCount64(); }
	void StartDelayStop() { delay_stop = GetTickCount64(); }
	void StartDying() { dying_start = GetTickCount64(); }
	void StartShooting() { shooting_start = GetTickCount64(); }
	void StartAiming() { aim_start = GetTickCount64(); }
	void ShootBullet();
};

