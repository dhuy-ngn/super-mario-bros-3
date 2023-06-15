#include "Goomba.h"
#include "Koopa.h"
#include "PlayScene.h"

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
		if (level == GOOMBA_LEVEL_NORMAL)
		{
			left = x - GOOMBA_BBOX_WIDTH / 2;
			top = y - GOOMBA_BBOX_HEIGHT / 2;
			right = left + GOOMBA_BBOX_WIDTH;
			bottom = top + GOOMBA_BBOX_HEIGHT;
		}
		else
		{
			left = x - GOOMBA_PARA_BBOX_WIDTH / 2;
			top = y - GOOMBA_PARA_BBOX_HEIGHT / 2;
			right = left + GOOMBA_PARA_BBOX_WIDTH;
			bottom = top + GOOMBA_PARA_BBOX_HEIGHT;
		}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CGoomba*>(e->obj)) return;
	if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);

	if (e->ny != 0)
	{
		vy = 0;
		if (e->ny < 0 && level == GOOMBA_LEVEL_PARA && state != GOOMBA_STATE_DIE)
		{
			if (paragoomba_jump_stack == 3)
			{
				SetState(GOOMBA_STATE_HIGH_SKIP);
			}
			else
			{
				SetState(GOOMBA_STATE_SKIP);
			}
		}
		else
			ay = GOOMBA_GRAVITY;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == GOOMBA_STATE_DIE && GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT) || (state == GOOMBA_STATE_KNOCKED_OUT && y >= 200))
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	int aniId = -1;
	if (color == GOOMBA_COLOR_RED)
		aniId = GetAniIdRedGoomba();
	else
		aniId = GetAniIdYellowGoomba();

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		break;
	case GOOMBA_STATE_KNOCKED_OUT:
		die_start = GetTickCount64();
		vy = -0.7f;
		break;
	case GOOMBA_STATE_SKIP:
		ay = GOOMBA_GRAVITY;
		vy = -GOOMBA_SKIP_SPEED;
		vx = -GOOMBA_WALKING_SPEED;
		paragoomba_jump_stack++;
		break;
	case GOOMBA_STATE_HIGH_SKIP:
		vy = -GOOMBA_HIGHSKIP_SPEED;
		ay = GOOMBA_GRAVITY;
		vx = -GOOMBA_WALKING_SPEED;
		paragoomba_jump_stack = 0;
		break;
	}
}

void CGoomba::OnCollisionWithKoopa(LPCOLLISIONEVENT e) {
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);

	if (koopa->IsSpinning())
	{
		vx = KOOPA_SPINNING_SPEED / 2;
		SetState(GOOMBA_STATE_KNOCKED_OUT);
	}
	else return;
}

int CGoomba::GetAniIdRedGoomba()
{
	int aniId = -1;
	if (level == GOOMBA_LEVEL_PARA)
	{
		aniId = ID_ANI_RED_PARAGOOMBA_SKIPPING;
	}
	else
	{
		switch (state)
		{
		case GOOMBA_STATE_WALKING:
			aniId = ID_ANI_RED_GOOMBA_WALKING;
			break;
		case GOOMBA_STATE_DIE:
			aniId = ID_ANI_RED_GOOMBA_DIE;
			break;
		case GOOMBA_STATE_HIGH_SKIP:
			aniId = ID_ANI_RED_PARAGOOMBA_HIGH_SKIPPING;
			break;
		case GOOMBA_STATE_SKIP:
			aniId = ID_ANI_RED_PARAGOOMBA_SKIPPING;
			break;
		}
	}

	if (aniId == -1) aniId = ID_ANI_RED_GOOMBA_WALKING;
	return aniId;
}

int CGoomba::GetAniIdYellowGoomba()
{
	int aniId = -1;
	switch (state)
	{
	case GOOMBA_STATE_WALKING:
		aniId = ID_ANI_YELLOW_GOOMBA_WALKING;
		break;
	case GOOMBA_STATE_DIE:
		aniId = ID_ANI_YELLOW_GOOMBA_DIE;
		break;
	}
	if (aniId == -1) aniId = ID_ANI_YELLOW_GOOMBA_WALKING;
	return aniId;
}