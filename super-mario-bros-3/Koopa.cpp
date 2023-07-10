#include "Koopa.h"
#include "debug.h"
#include "QuestionBlock.h"
#include "ColorBlock.h"
#include "FireTrap.h"
#include "PlayScene.h"

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!IsHiding() && !IsSpinning())
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT;
	}
	else
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_SHELL_BBOX_HEIGHT / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_SHELL_BBOX_HEIGHT;
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (state == KOOPA_STATE_UPSIDE_DOWN && GetTickCount64() - die_start > KOOPA_DIE_TIMEOUT && die_start > 0)
	{
		isDeleted = true;
		return;
	}
	if (state == KOOPA_STATE_HIDING && GetTickCount64() - inactive_start > KOOPA_INACTIVE_DURATION)
	{
		SetState(KOOPA_STATE_SHAKING);
		return;
	}
	if (state == KOOPA_STATE_SHAKING || state == KOOPA_STATE_SHAKING_UPSIDE_DOWN && GetTickCount64() - inactive_start > KOOPA_WAKING_UP_DURATION)
	{
		y -= (KOOPA_BBOX_HEIGHT - KOOPA_SHELL_BBOX_HEIGHT) / 2;
		SetState(KOOPA_STATE_WALKING);
		return;
	}
	if (state == KOOPA_STATE_KNOCKED_OUT && GetTickCount64() - die_start > KOOPA_DIE_TIMEOUT && die_start > 0) 
	{
		isDeleted = true;
		return;
	}
	if (isBeingHeld)
	{
		CMario* mario = dynamic_cast<CMario*>(((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer());
		float mx, my;
		mario->GetPosition(mx, my);

		if (mario->IsHoldingKoopaShell())
			switch (mario->GetLevel())
			{
			case MARIO_LEVEL_SMALL:
				SetPosition(mx + 9 * mario->GetMarioDirection(), my - 2);
				break;
			case MARIO_LEVEL_BIG:
				SetPosition(mx + 8 * mario->GetMarioDirection(), my + 2);
				break;
			case MARIO_LEVEL_RACCOON:
				SetPosition(mx + 12 * mario->GetMarioDirection(), my + 2);
				break;
			}
		else
		{
			if (mario->GetMarioDirection() > 0)
			{
				SetState(KOOPA_STATE_SPINNING_RIGHT);
			}
			else
			{
				SetState(KOOPA_STATE_SPINNING_LEFT);
			}
			isBeingHeld = false;
		}
	}

	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = dynamic_cast<CMario*>(current_scene->GetPlayer());
	CMarioTail* tail = mario->GetMarioTail();
	if (mario->GetLevel() == MARIO_LEVEL_RACCOON && mario->IsAttacking())
	{
		float tLeft, tTop, tRight, tBottom;
		float oLeft, oTop, oRight, oBottom;

		tail->GetBoundingBox(tLeft, tTop, tRight, tBottom);
		GetBoundingBox(oLeft, oTop, oRight, oBottom);

		if (tRight >= oLeft && tLeft <= oRight && tBottom >= oTop && tTop <= oBottom && state != KOOPA_STATE_KNOCKED_OUT)
		{
			SetState(KOOPA_STATE_KNOCKED_OUT);
		}
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopa::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = ID_ANI_KOOPA_GREEN_WALKING_LEFT;

	if (color == KOOPA_COLOR_GREEN)
	{
		aniId = GetAniIdGreenKoopa();
	}
	else
	{
		aniId = GetAniIdRedKoopa();
	}

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
	if (!e->obj->IsBlocking()) return;

	if (e->ny != 0)
	{
		vy = 0;
		if (e->ny < 0)
			if (level == KOOPA_LEVEL_PARA)
				SetState(KOOPA_STATE_SKIPPING);
		else
			ay = KOOPA_GRAVITY;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}

	if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	if (dynamic_cast<CQuestionBlock*>(e->obj))
		OnCollisionWithQuestionBlock(e);
	if (dynamic_cast<CFireTrap*>(e->obj))
		OnCollisionWithFireTrap(e);
	if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatform(e);
	if (dynamic_cast<CBrick*>(e->obj))
		OnCollisionWithBrick(e);
}

int CKoopa::GetAniIdGreenKoopa() 
{
	int aniId = ID_ANI_KOOPA_GREEN_WALKING_LEFT;

	if (level == KOOPA_LEVEL_PARA)
	{
		if (vx > 0)
			aniId = ID_ANI_KOOPA_GREEN_FLYING_RIGHT;
		else
			aniId = ID_ANI_KOOPA_GREEN_FLYING_LEFT;
	}
	else
		if (isHiding)
			aniId = ID_ANI_KOOPA_GREEN_SHELL;
		else
			if (isSpinning)
				if (vx > 0)
					aniId = ID_ANI_KOOPA_GREEN_SPINNING_RIGHT;
				else
					aniId = ID_ANI_KOOPA_GREEN_SPINNING_LEFT;
			else
				if (vx >= 0)
					aniId = ID_ANI_KOOPA_GREEN_WALKING_RIGHT;
				else
					aniId = ID_ANI_KOOPA_GREEN_WALKING_LEFT;
	return aniId;
}

int CKoopa::GetAniIdRedKoopa() {
	int aniId = ID_ANI_KOOPA_RED_WALKING_LEFT;
	if (isHiding)
		aniId = ID_ANI_KOOPA_RED_SHELL;
	else if (isSpinning)
		if (vx > 0)
			aniId = ID_ANI_KOOPA_RED_SPINNING_RIGHT;
		else
			aniId = ID_ANI_KOOPA_RED_SPINNING_LEFT;
	else
		if (vx >= 0)
			aniId = ID_ANI_KOOPA_RED_WALKING_RIGHT;
		else
			aniId = ID_ANI_KOOPA_RED_WALKING_LEFT;
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
	isBeingHeld = false;
	if (level == KOOPA_LEVEL_PARA)
		SetState(KOOPA_STATE_SKIPPING);
	else
		SetState(KOOPA_STATE_WALKING);
}


void CKoopa::SetState(int state)
{
	switch (state) {
	case KOOPA_STATE_WALKING:
		canBeHeld = true;
		isHiding = false;
		isSpinning = false;
		isBeingHeld = false;
		vx = -KOOPA_WALKING_SPEED;
		vy = 0;
		break;

	case KOOPA_STATE_HIDING:
		canBeHeld = true;
		isHiding = true;
		inactive_start = GetTickCount64();
		vx = 0.0f;
		break;

	case KOOPA_STATE_SPINNING_LEFT:
		canBeHeld = false;
		isHiding = false;
		isSpinning = true;
		vx = -KOOPA_SPINNING_SPEED;
		break;

	case KOOPA_STATE_SPINNING_RIGHT:
		canBeHeld = false;
		isHiding = false;
		isSpinning = true;
		vx = KOOPA_SPINNING_SPEED;
		vy = 0;
		break;

	case KOOPA_STATE_SHAKING:
		isHiding = true;
		wake_up_start = GetTickCount64();
		break;

	case KOOPA_STATE_HELD_BY_MARIO:
		isBeingHeld = true;
		canBeHeld = true;
		isHiding = true;
		vx = 0;
		vy = 0;
		inactive_start = GetTickCount64();
		break;

		// Flying Koopas
	case KOOPA_STATE_SKIPPING:
		canBeHeld = false;
		isHiding = false;
		isSpinning = false;
		isBeingHeld = false;
		if (level == KOOPA_LEVEL_NORMAL)
			SetState(KOOPA_STATE_WALKING);
		vy = -KOOPA_BOUNCING_SPEED_Y;
		vx = -KOOPA_BOUNCING_SPEED_X;
		break;

	case KOOPA_STATE_UPSIDE_DOWN:
		break;

	case KOOPA_STATE_KNOCKED_OUT:
		die_start = GetTickCount64();
		vy = -0.5f;
		break;
	}

	CGameObject::SetState(state);
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
	CMario* mario = dynamic_cast<CMario*>(((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer());

	if (question_block->GetState() != QUESTION_BLOCK_STATE_INACTIVE && IsSpinning())
	{
		question_block->SetState(QUESTION_BLOCK_STATE_INACTIVE);
		question_block->ReleaseItem();
	}
}

void CKoopa::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	CPlatform* platform = dynamic_cast <CPlatform*>(e->obj);

	if (state == KOOPA_STATE_WALKING && color == KOOPA_COLOR_RED)
	{
		if (vx > 0 && x >= platform->GetX() + KOOPA_BBOX_WIDTH)
			vx = -vx;
		if (vx < 0 && x <= platform->GetMaxX() - KOOPA_BBOX_WIDTH)
			vx = -vx;
	}
}

void CKoopa::OnCollisionWithFireTrap(LPCOLLISIONEVENT e)
{
	if (state == KOOPA_STATE_SPINNING_LEFT || state == KOOPA_STATE_SPINNING_RIGHT)
	{
		e->obj->Delete();
	}
}

void CKoopa::OnCollisionWithBrick(LPCOLLISIONEVENT e)
{
	if (state == KOOPA_STATE_SPINNING_LEFT || state == KOOPA_STATE_SPINNING_RIGHT)
	{
		e->obj->Delete();
	}
}
