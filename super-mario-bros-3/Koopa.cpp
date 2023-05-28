#include "Koopa.h"
#include "debug.h"

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