#include "Mushroom.h"
#include "PlayScene.h"

void CMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MUSHROOM_BBOX_WIDTH / 2;
	top = y - MUSHROOM_BBOX_HEIGHT / 2;
	right = left + MUSHROOM_BBOX_WIDTH;
	bottom = top + MUSHROOM_BBOX_HEIGHT;
}

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	if (state == MUSHROOM_STATE_MOVE) {
		vy = MUSHROOM_GRAVITY;
	}

};

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (start_y - y >= MUSHROOM_BBOX_HEIGHT && state == MUSHROOM_STATE_IDLE) {
		this->SetState(MUSHROOM_STATE_MOVE);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushroom::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = ID_ANI_MUSHROOM_RED;

	animations->Get(aniId)->Render(x, y);
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}
}

void CMushroom::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario = dynamic_cast<CMario*>(((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer());
	switch (state)
	{
	case MUSHROOM_STATE_IDLE:
		vy = MUSHROOM_GROWING_UP_SPEED;
		break;

	case MUSHROOM_STATE_MOVE:
		this->vy = MUSHROOM_GRAVITY;
		vx = -mario->GetVx() / mario->GetVx() * MUSHROOM_MOVING_SPEED;
		break;
	case MUSHROOM_STATE_DELETED:
		this->Delete();
		break;
	}

}