#include "WorldMapObject.h"
#include "debug.h"

CWorldMapObject::CWorldMapObject(int object_type, int scene_id, int ani_id)
{
	vx = vy = 0;
	cgDown = cgUp = cgLeft = cgRight = false;
	this->object_type = object_type;
	this->scene_id = scene_id;
	this->ani_id = ani_id;
}

void CWorldMapObject::Render()
{
	//RenderBoundingBox();
	if (object_type == OBJECT_TYPE_HAMMER)
	{
		if (vx > 0)
			CAnimations::GetInstance()->Get(420)->Render(x, y);
		else
			CAnimations::GetInstance()->Get(421)->Render(x, y);
	}
	else if (ani_id < 10000)
		CAnimations::GetInstance()->Get(ani_id)->Render(x, y);
	else
		CSprites::GetInstance()->Get(ani_id)->Draw(x, y);
}

void CWorldMapObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	if (object_type == OBJECT_TYPE_HAMMER && x >= HAMMER_LIMIT_RIGHT && vx > 0) 
	{
		vx = -vx;
	}
	if (object_type == OBJECT_TYPE_HAMMER && x <= HAMMET_LIMIT_LEFT && vx < 0) 
	{
		vx = -vx;
	}

	CGameObject::Update(dt);
}

void CWorldMapObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (object_type != OBJECT_TYPE_DECORATION)
	{
		left = x + 2;
		top = y + 2;
		right = left + OBJECT_BBOX_WIDTH;
		bottom = top + OBJECT_BBOX_WIDTH;
	}
}