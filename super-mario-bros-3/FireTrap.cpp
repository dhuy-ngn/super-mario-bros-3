#include "FireTrap.h"
#include "Mario.h"
#include "PlayScene.h"
#include "FireBullet.h"


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
	GetDirection();
	if (y <= maxY && vy < 0)
	{
		y = maxY;
		vy = 0;
		StartAiming();
	}

	if (GetTickCount64() - delay_stop >= FIRETRAP_DELAY_STOP_TIME && delay_stop != 0)
	{
		delay_stop = 0;
		SetState(FIRETRAP_STATE_DARTING);
	}

	if (GetTickCount64() - aim_start >= FIRETRAP_AIM_TIME && aim_start != 0)
	{
		aim_start = 0;
		SetState(FIRETRAP_STATE_SHOOTING);
		StartDelay();
	}

	if (GetTickCount64() - delay_start >= FIRETRAP_DELAY_TIME && delay_start != 0)
	{
		delay_start = 0;
		if (y == maxY)
			vy = FIRETRAP_DARTING_SPEED;
		StartDelayStop();
	}

}

void CFireTrap::Render()
{
	CAnimations* animations = CAnimations::GetInstance();

	int aniId = FIRETRAP_ANI_RIGHT_UP;
	if (state != FIRETRAP_STATE_DEATH && dying_start == 0)
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

void CFireTrap::ShootBullet()
{
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	this->bullet = new CFireBullet(x, y, isUpward, isForward);

	currentScene->PushObject(bullet);
}

void CFireTrap::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario = dynamic_cast<CMario*>(((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer());
	switch (state)
	{
	case FIRETRAP_STATE_DARTING:
		vy = -FIRETRAP_DARTING_SPEED;
		break;
	case FIRETRAP_STATE_SHOOTING:
		vy = 0;
		ShootBullet();
		break;
	case FIRETRAP_STATE_INACTIVE:
		vy = 0;
		break;
	case FIRETRAP_STATE_DEATH:
		vy = 0;
		StartDying();
		break;
	}
}
