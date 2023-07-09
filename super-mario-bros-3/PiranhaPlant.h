#pragma once
#include "GameObject.h"
#include "Utils.h"
#include "debug.h"

#define PIRANHAPLANT_BBOX_WIDTH					20
#define PIRANHAPLANT_BBOX_HEIGHT			24

#define PIRANHAPLANT_BBOX_HEIGHT_ANI		28

#define PIRANHAPLANT_STATE_IDLE				0

#define PIRANHAPLANT_ANI_IDLE				7300

#define PIRANHAPLANT_DARTING_SPEED				0.03f

#define PIRANHAPLANT_STATE_BITING			100
#define PIRANHAPLANT_STATE_DEATH			200
#define PIRANHAPLANT_STATE_INACTIVE			300
#define PIRANHAPLANT_BITING_TIME			1000
#define PIRANHAPLANT_DELAY_STOP_TIME		3000
#define PIRANHAPLANT_ACTIVE_RANGE			20

class CPiranhaPlant : public CGameObject
{
	float limitY = 0;
	ULONGLONG delay_stop = 0;
	ULONGLONG biting_start = 0;
	ULONGLONG dying_start = 0;
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int state);

	void StartBitting() { biting_start = GetTickCount64(); }
	void StartDying() { dying_start = GetTickCount64(); }
	void StartDelayStop() { delay_stop = GetTickCount64(); }

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	CPiranhaPlant(float x, float y);
};