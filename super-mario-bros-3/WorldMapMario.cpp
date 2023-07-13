#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "WorldMapMario.h"
#include "WorldScene.h"
#include "WorldMapObject.h"
#include "debug.h"

CWorldMapMario::CWorldMapMario(float x, float y) : CGameObject()
{
	SetLevel(MARIO_LEVEL_SMALL);
	SetState(MARIO_STATE_IDLE);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	SetMove(false, false, true, false);
}

void CWorldMapMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 2;
	top = y + 2;
	right = left + PLAYER_BBOX_WIDTH - 4;
	bottom = top + PLAYER_BBOX_WIDTH - 4;
}

void CWorldMapMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;

};

void CWorldMapMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking()) 
	{
		vx = 0;
		vy = 0;
	}

	CWorldMapObject* map_obj = dynamic_cast<CWorldMapObject*>(e->obj);

	if (map_obj->GetType() == OBJECT_TYPE_PORTAL || map_obj->GetType() == OBJECT_TYPE_NODE)
	{
		SetState(PLAYER_STATE_IDLE);
		OnCollisionWithNode(e);
	}
}

void CWorldMapMario::OnCollisionWithNode(LPCOLLISIONEVENT e)
{
	CWorldMapObject* node = dynamic_cast<CWorldMapObject*>(e->obj);
	bool cgLeft, cgRight, cgUp, cgDown;
	node->GetDirection(cgLeft, cgUp, cgRight, cgDown);
	SetMove(cgLeft, cgUp, cgRight, cgDown);
	if (node->GetType() == OBJECT_TYPE_PORTAL)
		readySwitchScene = true;
	else
		readySwitchScene = false;
}

void CWorldMapMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CWorldMapMario::Render()
{
	switch (level)
	{
	case MARIO_LEVEL_SMALL:
		CAnimations::GetInstance()->Get(ID_ANI_MARIO_SMALL)->Render(x, y);
		break;
	case MARIO_LEVEL_BIG:
		CAnimations::GetInstance()->Get(ID_ANI_MARIO_BIG)->Render(x, y);
		break;
	case MARIO_LEVEL_RACCOON:
		CAnimations::GetInstance()->Get(ID_ANI_MARIO_RACCOON)->Render(x, y);
		break;
	}
	//RenderBoundingBox();
}
void CWorldMapMario::SetState(int state)
{
	switch (state)
	{
	case PLAYER_STATE_IDLE:
		start_x = x;
		start_y = y;
		vx = vy = 0;
		break;
	case PLAYER_STATE_RIGHT:
		vx = PLAYER_SPEED;
		vy = 0;
		break;
	case PLAYER_STATE_LEFT:
		vx = -PLAYER_SPEED;
		vy = 0;
		break;
	case PLAYER_STATE_UP:
		vx = 0;
		vy = -PLAYER_SPEED;
		break;
	case PLAYER_STATE_DOWN:
		vx = 0;
		vy = PLAYER_SPEED;
		break;
	}
	CGameObject::SetState(state);
}

