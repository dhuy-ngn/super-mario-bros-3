#include "Point.h"
#include "debug.h"

CPoint::CPoint(int score_stack)
{
	vx = 0;
	vy = -SCORE_SPEED_Y;
	isDeleted = false;
	appear_time = GetTickCount64();
	this->score_stack = score_stack;
}

void CPoint::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
	y += vy * dt;
	if (GetTickCount64() - appear_time >= SCORE_EXIST_DURATION)
		Delete();
}

void CPoint::Render() 
{
	switch (score_stack)
	{
	case SCORE_STACK_100:
		CSprites::GetInstance()->Get(SCORE_SPRITE_100_ID)->Draw(x, y);
		break;
	case SCORE_STACK_200:
		CSprites::GetInstance()->Get(SCORE_SPRITE_200_ID)->Draw(x, y);
		break;
	case SCORE_STACK_400:
		CSprites::GetInstance()->Get(SCORE_SPRITE_400_ID)->Draw(x, y);
		break;
	case SCORE_STACK_800:
		CSprites::GetInstance()->Get(SCORE_SPRITE_800_ID)->Draw(x, y);
		break;
	case SCORE_STACK_1000:
		CSprites::GetInstance()->Get(SCORE_SPRITE_1000_ID)->Draw(x, y);
		break;
	case SCORE_STACK_2000:
		CSprites::GetInstance()->Get(SCORE_SPRITE_2000_ID)->Draw(x, y);
		break;
	case SCORE_STACK_4000:
		CSprites::GetInstance()->Get(SCORE_SPRITE_4000_ID)->Draw(x, y);
		break;
	case SCORE_STACK_8000:
		CSprites::GetInstance()->Get(SCORE_SPRITE_8000_ID)->Draw(x, y);
		break;
	default:
		CSprites::GetInstance()->Get(SCORE_SPRITE_1UP_ID)->Draw(x, y);
		break;
	}
}

void CPoint::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = r = b = t = 0;
}