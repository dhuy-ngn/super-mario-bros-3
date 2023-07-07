#include "Switch.h"
#include "Brick.h"
#include "PlayScene.h"
#include "Coin.h"

void CSwitch::Render() 
{
	int aniId = SWITCH_ANI_IDLE;
	CAnimations* animations = CAnimations::GetInstance();
	if (state == SWITCH_STATE_PRESSED)
		aniId = SWITCH_ANI_PRESSED;
	else
		aniId = SWITCH_ANI_IDLE;
	animations->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CSwitch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (state == SWITCH_STATE_IDLE)
	{
		y += vy * dt;
		if (start_y - y >= SWITCH_BBOX_HEIGHT)
		{
			vy = 0;
			y = start_y - SWITCH_BBOX_HEIGHT - 0.1f;
			SetState(SWITCH_STATE_IDLE);
		}
	}
}

void CSwitch::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case SWITCH_STATE_IDLE:
		vy = -0.05f;
		break;
	case SWITCH_STATE_PRESSED:
		break;
	}
}

void CSwitch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - SWITCH_BBOX_WIDTH / 2;
	t = y - SWITCH_BBOX_HEIGHT / 2;
	r = l + SWITCH_BBOX_WIDTH;
	b = t + SWITCH_BBOX_HEIGHT;
}