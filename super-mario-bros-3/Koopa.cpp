#include "Koopa.h"
#include "debug.h"
#include "QuestionBlock.h"
#include "ColorBlock.h"
#include "FireTrap.h"

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - KOOPA_BBOX_WIDTH / 2;
	top = y - KOOPA_BBOX_HEIGHT / 2;
	right = left + KOOPA_BBOX_WIDTH;
	bottom = top + KOOPA_BBOX_HEIGHT;
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == KOOPA_STATE_UPSIDE_DOWN) && (GetTickCount64() - die_start > KOOPA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}
	if ((state == KOOPA_STATE_HIDING) && (GetTickCount64() - inactive_start > KOOPA_INACTIVE_DURATION))
	{
		SetState(KOOPA_STATE_SHAKING);
		return;
	}
	if (((state == KOOPA_STATE_SHAKING) || (state == KOOPA_STATE_SHAKING_UPSIDE_DOWN)) && (GetTickCount64() - inactive_start > KOOPA_WAKING_UP_DURATION))
	{
		SetState(KOOPA_STATE_WALKING);
		return;
	}
	if ((state == KOOPA_STATE_KNOCKED_OUT) && (GetTickCount64() - die_start > KOOPA_DIE_TIMEOUT)) {
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopa::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = ID_ANI_KOOPA_GREEN_WALKING_LEFT;

	if (IsRedKoopa())
		aniId = ID_ANI_KOOPA_RED_WALKING_LEFT;
	else
		aniId = ID_ANI_KOOPA_GREEN_WALKING_LEFT;

	if (level == KOOPA_LEVEL_NORMAL)
		if (IsRedKoopa())
			aniId = GetAniIdRedKoopaNormal();
		else
			aniId = GetAniIdGreenKoopaNormal();
	else if (level == KOOPA_LEVEL_WINGS)
		if (IsRedKoopa())
			aniId = GetAniIdRedKoopaWings();
		else
			aniId = GetAniIdGreenKoopaWings();

	animations->Get(aniId)->Render(x, y);

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CKoopa::OnNoCollision(DWORD dt) 
{
	x += vx * dt;
	y += vy * dt;
}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
	}
	else
		if (e->nx != 0 && e->obj->IsBlocking())
		{
			vx = -vx;
		}
	if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	//if (dynamic_cast<CGoomba*>(e->obj))
		//OnCollisionWithGoomba(e);
	if (dynamic_cast<CQuestionBlock*>(e->obj))
		OnCollisionWithQuestionBlock(e);
	if (dynamic_cast<CFireTrap*>(e->obj))
		OnCollisionWithFireTrap(e);
	if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatform(e);
}

int CKoopa::GetAniIdGreenKoopaNormal() {
	int aniId = ID_ANI_KOOPA_GREEN_WALKING_LEFT;
	if (vx >= 0)
		aniId = ID_ANI_KOOPA_GREEN_WALKING_RIGHT;
	switch (state) {
	case KOOPA_STATE_HIDING:
		aniId = ID_ANI_KOOPA_GREEN_SHELL;
		break;
	case KOOPA_STATE_UPSIDE_DOWN:
		aniId = ID_ANI_KOOPA_GREEN_SHELL_UPSIDE_DOWN;
		break;
	case KOOPA_STATE_SPINNING_LEFT:
		aniId = ID_ANI_KOOPA_GREEN_SPINNING_LEFT;
		break;
	case KOOPA_STATE_SPINNING_RIGHT:
		aniId = ID_ANI_KOOPA_GREEN_SPINNING_RIGHT;
		break;
	case KOOPA_STATE_SHAKING:
		aniId = ID_ANI_KOOPA_GREEN_SHAKING;
		break;
	case KOOPA_STATE_SHAKING_UPSIDE_DOWN:
		aniId = ID_ANI_KOOPA_GREEN_SHAKING_UPSIDE_DOWN;
		break;
	case KOOPA_STATE_KNOCKED_OUT:
		aniId = ID_ANI_KOOPA_GREEN_SHELL_UPSIDE_DOWN;
		break;
	}
	return aniId;
}

int CKoopa::GetAniIdRedKoopaNormal() {
	int aniId = ID_ANI_KOOPA_RED_WALKING_LEFT;
	if (vx >= 0)
		aniId = ID_ANI_KOOPA_RED_WALKING_RIGHT;
	switch (state) {
	case KOOPA_STATE_HIDING:
		aniId = ID_ANI_KOOPA_RED_SHELL;
		break;
	case KOOPA_STATE_UPSIDE_DOWN:
		aniId = ID_ANI_KOOPA_RED_SHELL_UPSIDE_DOWN;
		break;
	case KOOPA_STATE_SPINNING_LEFT:
		aniId = ID_ANI_KOOPA_RED_SPINNING_LEFT;
		break;
	case KOOPA_STATE_SPINNING_RIGHT:
		aniId = ID_ANI_KOOPA_RED_SPINNING_RIGHT;
		break;
	case KOOPA_STATE_SHAKING:
		aniId = ID_ANI_KOOPA_RED_SHAKING;
		break;
	case KOOPA_STATE_SHAKING_UPSIDE_DOWN:
		aniId = ID_ANI_KOOPA_RED_SHAKING_UPSIDE_DOWN;
		break;
	}
	return aniId;
}

int CKoopa::GetAniIdRedKoopaWings() {
	int aniId = ID_ANI_KOOPA_RED_FLYING_LEFT;
	if (vx >= 0)
		aniId = ID_ANI_KOOPA_RED_FLYING_RIGHT;
	return aniId;
}
int CKoopa::GetAniIdGreenKoopaWings() {
	int aniId = ID_ANI_KOOPA_GREEN_FLYING_LEFT;
	if (vx >= 0)
		aniId = ID_ANI_KOOPA_GREEN_FLYING_RIGHT;
	return aniId;
}

CKoopa::CKoopa(float x, float y, int level, int color) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPA_GRAVITY;
	this->level = level;
	this->color = color;
	inactive_start = -1;
	die_start = -1;
	SetState(KOOPA_STATE_WALKING);
}


void CKoopa::SetState(int state)
{
	if (this->state == KOOPA_STATE_UPSIDE_DOWN) return;

	switch (state) {
	case KOOPA_STATE_WALKING:
		wake_up_start = -1;
		inactive_start = -1;	// Reset inactive counter for Koopa
		vx = -KOOPA_WALKING_SPEED;
		break;
	case KOOPA_STATE_HIDING:
		inactive_start = GetTickCount64();
		vx = 0.0f;
		break;
	case KOOPA_STATE_SPINNING_LEFT:
		vx = -KOOPA_SPINNING_SPEED;
		break;
	case KOOPA_STATE_SPINNING_RIGHT:
		vx = KOOPA_SPINNING_SPEED;
		break;
	case KOOPA_STATE_SHAKING:
		wake_up_start = GetTickCount64();
		break;

		// Flying Koopas
	case KOOPA_STATE_BOUNCING_LEFT:
		vy = -KOOPA_BOUNCING_SPEED_Y;
		vx = -KOOPA_BOUNCING_SPEED_X;
		break;

	case KOOPA_STATE_BOUNCING_RIGHT:
		break;

	case KOOPA_STATE_UPSIDE_DOWN:
		break;

	case KOOPA_STATE_KNOCKED_OUT:
		die_start = GetTickCount64();
		vy = -0.7f;
		break;
	}

	CGameObject::SetState(state);
}

void CKoopa::SetLevel(int l) {
	level = l;
}

void CKoopa::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);

	if (koopa->IsSpinning()) {
		vx = KOOPA_SPINNING_SPEED / 2;
		SetState(KOOPA_STATE_KNOCKED_OUT);
	}
	else
	{
		koopa->vx = -koopa->vx;
		return;
	}
}

void CKoopa::OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e)
{
	CQuestionBlock* question_block = dynamic_cast<CQuestionBlock*>(e->obj);

	if (question_block->GetState() != QUESTION_BLOCK_STATE_INACTIVE && IsSpinning())
		question_block->SetState(QUESTION_BLOCK_STATE_INACTIVE);
	else return;
}

void CKoopa::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	CPlatform* platform = dynamic_cast <CPlatform*>(e->obj);

	if (state == KOOPA_STATE_WALKING && IsRedKoopa())
	{
		if (vx > 0 && x >= platform->GetX() + KOOPA_BBOX_WIDTH)
			vx = -vx;
		if (vx < 0 && x <= platform->GetMaxX() - KOOPA_BBOX_WIDTH)
			vx = -vx;
	}
}

void CKoopa::OnCollisionWithFireTrap(LPCOLLISIONEVENT e)
{
	if (state == KOOPA_STATE_SPINNING_LEFT || state == KOOPA_STATE_SPINNING_RIGHT || state == KOOPA_STATE_HIDING)
	{
		e->obj->Delete();
	}
}
