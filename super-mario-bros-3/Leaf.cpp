#include "Leaf.h"

void CLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - LEAF_BBOX_WIDTH / 2;
	top = y - LEAF_BBOX_HEIGHT / 2;
	right = left + LEAF_BBOX_WIDTH;
	bottom = top + LEAF_BBOX_HEIGHT;
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	y += vy * dt;

	if (state == LEAF_STATE_FALLING) {
		if (GetTickCount64() - falling_start_time >= LEAF_MAX_FLOATING_TIME) {
			vx = -vx;
			this->StartFallingTime();
		}
	}
	if (state == LEAF_STATE_UP) {
		if (start_y - y >= LEAF_UP_HEIGHT) {
			SetState(LEAF_STATE_FALLING);
		}
	}
	CGameObject::Update(dt, coObjects);
}

void CLeaf::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = LEAF_ANI_RIGHT;
	if (state == LEAF_STATE_FALLING)
	{
		if (vx >= 0)
			aniId = LEAF_ANI_RIGHT;
		else
			aniId = LEAF_ANI_LEFT;
	}
	
	animations->Get(aniId)->Render(x, y);
}

void CLeaf::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LEAF_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case LEAF_STATE_UP:
		vy = -LEAF_SPEED_UP;
		break;
	case LEAF_STATE_FALLING:
		vy = LEAF_GRAVITY;
		vx = LEAF_SPEED;
		StartFallingTime();
		break;
	}
}