#include "FireTrap.h"
#include "Mario.h"
#include "PlayScene.h"


void CFireTrap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - FIRETRAP_BBOX_WIDTH / 2;
	top = y - FIRETRAP_BBOX_HEIGHT / 2;
	right = left + FIRETRAP_BBOX_WIDTH;
	bottom = top + FIRETRAP_BBOX_HEIGHT;
}

void CFireTrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += vy * dt;
}

void CFireTrap::Render()
{
	CAnimations* animations = CAnimations::GetInstance();

	int aniId = -1;
	if (state != FIRETRAP_STATE_DEATH)
	{
		if (isUpward)
			if (isForward)
				aniId = FIRETRAP_ANI_RIGHT_UP;
			else
				aniId = FIRETRAP_ANI_LEFT_UP;
		else
			if (isForward)
				aniId = FIRETRAP_ANI_RIGHT_DOWN;
			else
				aniId = FIRETRAP_ANI_LEFT_DOWN;
	}

	if (aniId == -1) aniId = FIRETRAP_ANI_DEATH;

	animations->Get(aniId)->Render(x, y);
}

void CFireTrap::GetDirection()
{
	CMario* mario = dynamic_cast<CMario*>(((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer());
	float mario_height;
	if (mario->GetLevel() == MARIO_LEVEL_SMALL)
		mario_height = MARIO_SMALL_BBOX_HEIGHT;
	else
		mario_height = MARIO_BIG_BBOX_HEIGHT;

	if (mario->GetY() + mario_height < maxY + height)
	{
		isUpward = true;
	}
	else
	{
		isUpward = false;
	}
	if (mario->GetX() < x)
	{
		isForward = false;
	}
	else
	{
		isForward = true;
	}
}
