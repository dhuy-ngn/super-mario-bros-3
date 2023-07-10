#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"
#include "MarioTail.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.25f
#define MARIO_FLYING_SPEED	0.06f
#define	MARIO_FALLING_SPEED	0.1f
#define	MARIO_RACCOON_FALLING_SPEED	0.03f
#define MARIO_SPINNING_SPEED	0.08f

#define MARIO_ACCEL_WALK_X	0.001f
#define MARIO_ACCEL_RUN_X	0.0004f
#define MARIO_ACCEL_FLYING_X	0.0008f

#define MARIO_JUMP_SPEED_Y		0.33f
#define MARIO_JUMP_RUN_SPEED_Y	0.5f
#define MARIO_ACCEL_FLYING_Y 0.1f

#define MARIO_GRAVITY		0.0008f
#define MARIO_LANDING_SPEED	0.0005f

#define MARIO_JUMP_DEFLECT_SPEED  0.2f

#define MARIO_MAX_FLY_TIME	8000

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_FALL_OFF		-99
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301
#define MARIO_STATE_RACCOON_JUMP	302

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_STATE_FLY 700
#define MARIO_STATE_FLY_RIGHT 701
#define MARIO_STATE_FLY_LEFT 702

#define MARIO_STATE_LAND	800
#define MARIO_STATE_LAND_RIGHT	801
#define MARIO_STATE_LAND_LEFT	802

#define MARIO_STATE_ATTACK 900
#define MARIO_STATE_ATTACK_RELEASE 901

#define MARIO_STATE_HOLD_KOOPA_SHELL 1000
#define MARIO_STATE_HOLD_WALK_KOOPA_SHELL_RIGHT	1001
#define MARIO_STATE_HOLD_WALK_KOOPA_SHELL_LEFT	1002
#define	MARIO_STATE_HOLD_JUMP_KOOPA_SHELL_RIGHT	1003
#define	MARIO_STATE_HOLD_JUMP_KOOPA_SHELL_LEFT	1004
#define	MARIO_STATE_KICK_KOOPA_SHELL	1005


#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_HOLD_KOOPA_IDLE_RIGHT	1002
#define ID_ANI_MARIO_HOLD_KOOPA_IDLE_LEFT	1003

#define ID_ANI_MARIO_HOLD_KOOPA_WALK_RIGHT	1004
#define ID_ANI_MARIO_HOLD_KOOPA_WALK_LEFT	1005

#define ID_ANI_MARIO_HOLD_KOOPA_JUMP_RIGHT	1006
#define ID_ANI_MARIO_HOLD_KOOPA_JUMP_LEFT	1007

#define ID_ANI_MARIO_KICK_KOOPA_RIGHT	1008
#define ID_ANI_MARIO_KICK_KOOPA_LEFT	1009

#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

#define ID_ANI_MARIO_SMALL_HOLD_KOOPA_IDLE_RIGHT	1602
#define ID_ANI_MARIO_SMALL_HOLD_KOOPA_IDLE_LEFT		1603

#define ID_ANI_MARIO_SMALL_HOLD_KOOPA_WALK_RIGHT	1604
#define ID_ANI_MARIO_SMALL_HOLD_KOOPA_WALK_LEFT		1605

#define ID_ANI_MARIO_SMALL_HOLD_KOOPA_JUMP_RIGHT	1606
#define ID_ANI_MARIO_SMALL_HOLD_KOOPA_JUMP_LEFT		1607

#define ID_ANI_MARIO_SMALL_KICK_KOOPA_RIGHT	1608
#define ID_ANI_MARIO_SMALL_KICK_KOOPA_LEFT	1609

// RACCOON MARIO
#define ID_ANI_MARIO_RACCOON_IDLE_RIGHT 2400
#define ID_ANI_MARIO_RACCOON_IDLE_LEFT 2401

#define ID_ANI_MARIO_RACCOON_WALKING_RIGHT 2500
#define ID_ANI_MARIO_RACCOON_WALKING_LEFT 2501

#define ID_ANI_MARIO_RACCOON_RUNNING_RIGHT 2600
#define ID_ANI_MARIO_RACCOON_RUNNING_LEFT 2601

#define ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT 2700
#define ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT 2701

#define ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT 2800
#define ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT 2801

#define ID_ANI_MARIO_RACCOON_SIT_RIGHT 2900
#define ID_ANI_MARIO_RACCOON_SIT_LEFT 2901

#define ID_ANI_MARIO_RACCOON_BRACE_RIGHT 3000
#define ID_ANI_MARIO_RACCOON_BRACE_LEFT 3001

#define ID_ANI_MARIO_RACCOON_FLYING_RIGHT 3110
#define ID_ANI_MARIO_RACCOON_FLYING_LEFT 3111

#define ID_ANI_MARIO_RACCOON_LANDING_RIGHT 3100
#define ID_ANI_MARIO_RACCOON_LANDING_LEFT 3101

#define ID_ANI_MARIO_RACCOON_HOLD_KOOPA_IDLE_RIGHT	3112
#define ID_ANI_MARIO_RACCOON_HOLD_KOOPA_IDLE_LEFT	3113

#define ID_ANI_MARIO_RACCOON_HOLD_KOOPA_WALK_RIGHT	3114
#define ID_ANI_MARIO_RACCOON_HOLD_KOOPA_WALK_LEFT	3115

#define ID_ANI_MARIO_RACCOON_HOLD_KOOPA_JUMP_RIGHT	3116
#define ID_ANI_MARIO_RACCOON_HOLD_KOOPA_JUMP_LEFT	3117

#define ID_ANI_MARIO_RACCOON_KICK_KOOPA_RIGHT	3118
#define ID_ANI_MARIO_RACCOON_KICK_KOOPA_LEFT	3119

#define ID_ANI_MARIO_RACCOON_ATTACK 3300

#define ID_SPRITE_MARIO_WHACK_LEFT_1	12813
#define ID_SPRITE_MARIO_WHACK_LEFT_2	12814
#define ID_SPRITE_MARIO_WHACK_LEFT_3	12815
#define ID_SPRITE_MARIO_WHACK_LEFT_4	12816

#define ID_SPRITE_MARIO_WHACK_RIGHT_1	12803
#define ID_SPRITE_MARIO_WHACK_RIGHT_2	12804
#define ID_SPRITE_MARIO_WHACK_RIGHT_3	12805
#define ID_SPRITE_MARIO_WHACK_RIGHT_4	12806


#pragma endregion

#define GROUND_Y 160.0f

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_RACCOON	3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 22
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)
#define MARIO_RACCOON_SIT_HEIGHT_ADJUST ((MARIO_RACCOON_BBOX_HEIGHT - MARIO_RACCOON_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  10
#define MARIO_SMALL_BBOX_HEIGHT 14

#define MARIO_RACCOON_BBOX_WIDTH	14
#define MARIO_RACCOON_BBOX_HEIGHT	24
#define MARIO_RACCOON_SITTING_BBOX_WIDTH  14
#define MARIO_RACCOON_SITTING_BBOX_HEIGHT 16

#define MARIO_UNTOUCHABLE_TIME 2500
#define MARIO_RUNNING_STACK_DURATION	200
#define MARIO_MAX_RUNNING_STACK	7
#define MARIO_MAX_ATTACK_STACK_TIME 70
#define MARIO_MAX_ATTACK_STACK	5

class CMario : public CGameObject
{
	CMarioTail* tail;

	BOOLEAN isSitting;
	BOOLEAN isAttacking;
	BOOLEAN isLanding;
	BOOLEAN isFlying;
	BOOLEAN isRunning;
	BOOLEAN isHoldingKoopaShell;
	BOOLEAN canFly;
	BOOLEAN canLand;
	BOOLEAN canHoldKoopaShell;

	float maxVx;
	float maxVy;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	int level;
	int untouchable;
	int attack_ani_stack;
	int tail_direction;
	ULONGLONG untouchable_start = 0;
	ULONGLONG fly_up_start = 0;
	ULONGLONG attack_start = 0;
	ULONGLONG attack_stack_start = 0;
	ULONGLONG running_start = 0;
	ULONGLONG falling_start = 0;
	BOOLEAN isOnPlatform;

	int coin;
	int score_stack;
	int score;
	int life;
	int speed_stack;

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithColorBlock(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithFireTrap(LPCOLLISIONEVENT e);
	void OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e);
	void OnCollisionWithFireBullet(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithLeaf(LPCOLLISIONEVENT e);
	void OnCollisionWithBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithSwitch(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdRaccoon();

public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		tail = NULL;
		isHoldingKoopaShell = false;
		isSitting = false;
		isAttacking = false;
		isFlying = false;
		canFly = false;
		canHoldKoopaShell = false;
		attack_ani_stack = 0;
		attack_start = 0;
		attack_stack_start =0;
		maxVx = 0.0f;
		maxVy = 999.0f;
		ax = 0.0f;
		if (level != MARIO_LEVEL_RACCOON)
			ay = MARIO_GRAVITY;
		else
			ay = 0;

		level = MARIO_LEVEL_SMALL;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		coin = 0;
		fly_up_start = -1;
		score_stack = 0;
		score = 0;
		life = 4;
		speed_stack = 0;
		tail_direction = 0;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable()
	{
		return (state != MARIO_STATE_DIE);
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable == 0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	void StartUntouchable()
	{
		untouchable = 1;
		untouchable_start = GetTickCount64();
	}

	void AddScore(float x, float y);
	void AddScore1000(float x, float y);
	void AddScore1Up(float, float y);
	int GetMarioHeight();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	int GetLevel() { return this->level; }
	int GetCoin() { return this->coin; }
	int GetLife() { return this->life; }
	int GetScore() { return this->score; }
	int GetTailDirection() { return this->tail_direction; }
	int GetSpeedStack() { return this->speed_stack; }
	BOOLEAN IsAttacking() { return this->isAttacking; }
	BOOLEAN IsFlying() { return this->isFlying; }
	BOOLEAN IsLanding() { return this->isLanding; }
	BOOLEAN IsRunning() { return this->isRunning; }
	BOOLEAN CanFly() { return this->canFly; }
	BOOLEAN CanLand() { return this->canLand; }
	void SetCanLandToTrue() { this->canLand = true; }
	BOOLEAN IsIdle() { return vx == 0; }
	void HoldKoopaShell() { this->canHoldKoopaShell = true; }
	void ReleaseKoopaShell() { this->canHoldKoopaShell = false; }
	BOOLEAN CanHoldKoopaShell() { return this->canHoldKoopaShell; }
	BOOLEAN IsHoldingKoopaShell() { return this->isHoldingKoopaShell; }
	BOOLEAN ShouldTurnOnCamY()
	{
		return (this->isFlying || this->isLanding || y < -50 || y >= -50 && vy >= 0);
	}
	void StartFlying() { fly_up_start = GetTickCount64(); isFlying = true; }
	void StartAttacking() { attack_start = GetTickCount64(); attack_stack_start = GetTickCount64(); }
	void StartRunning() { running_start = GetTickCount64(); }
	void LevelDown();
	void GainScore();
	void Gain1000Score() { this->score += 1000; }
	void GainLife();
	void GainCoin();

	BOOLEAN IsTailCollidingWithObject(LPCOLLISIONEVENT e);
};