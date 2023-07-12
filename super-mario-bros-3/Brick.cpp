#include "Brick.h"
#include "Mushroom.h"
#include "PlayScene.h"
#include "Switch.h"

void CBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state == BRICK_STATE_EXHAUSTED)
		animations->Get(ID_ANI_BRICK_INACTIVE)->Render(x, y);
	else
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

	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = dynamic_cast<CMario*>(current_scene->GetPlayer());
	CMarioTail* tail = mario->GetMarioTail();
	if (mario->GetLevel() == MARIO_LEVEL_RACCOON && mario->IsAttacking())
	{
		float tLeft, tTop, tRight, tBottom;
		float oLeft, oTop, oRight, oBottom;

		tail->GetBoundingBox(tLeft, tTop, tRight, tBottom);
		GetBoundingBox(oLeft, oTop, oRight, oBottom);

		if (tRight >= oLeft && tLeft <= oRight && tBottom >= oTop && tTop <= oBottom && state != BRICK_STATE_EXHAUSTED)
		{
			if (item == BRICK_ITEM_TYPE_MUSHROOM)
			{
				SetState(BRICK_STATE_EXHAUSTED);
				CMushroom* mushroom = new CMushroom(x, y, MUSHROOM_TYPE_GREEN);
				current_scene->PushObject(mushroom);
				mushroom->SetState(MUSHROOM_STATE_IDLE);
			}
			else if (item == BRICK_ITEM_TYPE_SWITCH)
			{
				SetState(BRICK_STATE_EXHAUSTED);
				CSwitch* pSwitch = new CSwitch(x, y);
				current_scene->PushObject(pSwitch);
				pSwitch->SetState(SWITCH_STATE_IDLE);
			}
			else
				isDeleted = true;
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
