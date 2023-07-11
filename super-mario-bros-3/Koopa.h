#pragma once
#include "GameObject.h"
#include "Platform.h"

#define KOOPA_GRAVITY			0.001f
#define KOOPA_WALKING_SPEED		0.03f
#define KOOPA_SPINNING_SPEED	0.3f

#define KOOPA_ACCEL_SPIN_X 0.12f

#define KOOPA_BOUNCING_SPEED_X	0.05f
#define KOOPA_BOUNCING_SPEED_Y	0.35f

#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 26
#define KOOPA_SHELL_BBOX_HEIGHT 16

#define KOOPA_SHAKING_WIDTH	10

#define KOOPA_LEVEL_NORMAL	1
#define KOOPA_LEVEL_PARA	0

#define KOOPA_COLOR_GREEN	1
#define KOOPA_COLOR_RED 0

#define KOOPA_INACTIVE_DURATION		5000
#define KOOPA_WAKING_UP_DURATION	1000

#define KOOPA_STATE_WALKING	100

#define KOOPA_STATE_BOUNCING_RIGHT 201
#define KOOPA_STATE_BOUNCING_LEFT 200

#define	KOOPA_STATE_SKIPPING 210

#define KOOPA_STATE_SPINNING_RIGHT 301
#define	KOOPA_STATE_SPINNING_LEFT 300

#define KOOPA_STATE_HIDING 400
#define KOOPA_STATE_UPSIDE_DOWN 500

#define KOOPA_STATE_SHAKING	600
#define	KOOPA_STATE_SHAKING_UPSIDE_DOWN 601

#define KOOPA_STATE_HELD_BY_MARIO	602

#define KOOPA_STATE_KNOCKED_OUT	700

#define KOOPA_DIE_TIMEOUT 2000

#define ID_ANI_KOOPA_GREEN_WALKING_RIGHT	4101
#define ID_ANI_KOOPA_GREEN_WALKING_LEFT	4100

#define ID_ANI_KOOPA_GREEN_JUMPING_RIGHT	4301
#define ID_ANI_KOOPA_GREEN_JUMPING_LEFT	4300

#define ID_ANI_KOOPA_GREEN_SPINNING_RIGHT	4121
#define ID_ANI_KOOPA_GREEN_SPINNING_LEFT	4120

#define ID_ANI_KOOPA_GREEN_FLYING_LEFT	4310
#define ID_ANI_KOOPA_GREEN_FLYING_RIGHT 4311

#define ID_ANI_KOOPA_GREEN_SHAKING	4400
#define ID_ANI_KOOPA_GREEN_SHAKING_UPSIDE_DOWN	4401

#define ID_ANI_KOOPA_GREEN_SHELL	4170
#define ID_ANI_KOOPA_GREEN_SHELL_UPSIDE_DOWN	4320

#define ID_ANI_KOOPA_RED_WALKING_RIGHT	4601
#define ID_ANI_KOOPA_RED_WALKING_LEFT	4600

#define ID_ANI_KOOPA_RED_JUMPING_RIGHT	4801
#define ID_ANI_KOOPA_RED_JUMPING_LEFT	4800

#define ID_ANI_KOOPA_RED_SPINNING_RIGHT	4721
#define ID_ANI_KOOPA_RED_SPINNING_LEFT	4720

#define ID_ANI_KOOPA_RED_FLYING_LEFT	4810
#define ID_ANI_KOOPA_RED_FLYING_RIGHT 4811

#define ID_ANI_KOOPA_RED_SHAKING	4900
#define ID_ANI_KOOPA_RED_SHAKING_UPSIDE_DOWN	4901

#define ID_ANI_KOOPA_RED_SHELL	4670
#define ID_ANI_KOOPA_RED_SHELL_UPSIDE_DOWN	4820

class CKoopa : public CGameObject
{
protected:
	float ax;
	float ay;
	ULONGLONG inactive_start;
	ULONGLONG die_start;
	ULONGLONG wake_up_start;

	BOOLEAN canBeHeld;
	BOOLEAN isBeingHeld;
	BOOLEAN isHiding;
	BOOLEAN isSpinning;

	int level; // 1: para; 0: normal
	int color; // 1: green koopa; 0: red koopa

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	int IsCollidable() { return state != KOOPA_STATE_KNOCKED_OUT; };
	int IsBlocking() { return state != KOOPA_STATE_KNOCKED_OUT; };

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e);
	void OnCollisionWithColorBlock(LPCOLLISIONEVENT e);
	void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	void OnCollisionWithFireTrap(LPCOLLISIONEVENT e);
	void OnCollisionWithBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e);

	int GetAniIdRedKoopa();
	int GetAniIdGreenKoopa();

public:
	CKoopa(float x, float y, int level, int color);

	BOOLEAN IsHiding() { return this->isHiding; };
	BOOLEAN IsSpinning() { return this->isSpinning; };

	void SetState(int state);
	void SetLevel(int level) { this->level = level; }
	int GetLevel() { return this->level; };
	int GetColor() { return this->color; }
	BOOLEAN CanBeHeld() { return this->canBeHeld; }
	BOOLEAN IsBeingHeld() { return this->isBeingHeld; }
	void SetIsBeingHeld(BOOLEAN value) { this->isBeingHeld = value; }

	BOOLEAN ShouldTurnAround(LPGAMEOBJECT obj);
};