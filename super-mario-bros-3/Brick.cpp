#include "Brick.h"
#include "Mushroom.h"

void CBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_BRICK)->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;

	if (state == BRICK_STATE_EXHAUSTED)
	{
		if (GetTickCount64() - deflect_start >= BRICK_DEFLECT_DURATION)
		{
			vy = BRICK_DEFLECT_SPEED;
		}
		if (GetTickCount64() - deflect_end >= BRICK_DEFLECT_DURATION && y >= start_y)
		{
			y = start_y;
			vy = 0;
		}
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - BRICK_BBOX_WIDTH/2;
	t = y - BRICK_BBOX_HEIGHT/2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void CBrick::SetState(int state)
{
	switch (state) {
	case BRICK_STATE_EXHAUSTED:
		deflect_start = GetTickCount64();
		deflect_end = deflect_start + BRICK_DEFLECT_DURATION;
		vy = -BRICK_DEFLECT_SPEED;
		break;
	}

	CGameObject::SetState(state);
}
