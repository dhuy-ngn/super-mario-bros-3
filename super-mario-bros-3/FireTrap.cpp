#include "FireTrap.h"


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
}
