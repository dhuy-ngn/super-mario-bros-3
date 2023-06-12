#include "FireBullet.h"
#include "FireTrap.h"
#include "Mario.h"

CFireBullet::CFireBullet(float bx, float by, bool up, bool right) {
	if (up) {
		y = by;
		vy = -BULLET_SPEED_Y;
	}
	else {
		y = by;
		vy = BULLET_SPEED_Y;
	}

	if (right)
	{
		x = bx;
		vx = BULLET_SPEED_X;
	}
	else
	{
		x = bx;
		vx = -BULLET_SPEED_X;
	}
}

void CFireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt, coObjects);

	x += vx * dt;
	y += vy * dt;
}

void CFireBullet::Render()
{
	CAnimations* animations = CAnimations::GetInstance();

	int aniId = BULLET_ANI_RIGHT;
	if (vx > 0)
		aniId = BULLET_ANI_RIGHT;
	else
		aniId = BULLET_ANI_LEFT;

	animations->Get(aniId)->Render(x, y);
}

void CFireBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BULLET_BBOX_WIDTH / 2;
	t = y - BULLET_BBOX_HEIGHT / 2;
	r = l + BULLET_BBOX_WIDTH;
	b = t + BULLET_BBOX_HEIGHT;
}