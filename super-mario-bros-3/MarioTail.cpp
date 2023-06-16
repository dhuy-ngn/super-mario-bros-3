#include "MarioTail.h"
#include "PlayScene.h"

void CMarioTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MARIO_TAIL_BBOX_WIDTH / 2;
	top = y - MARIO_TAIL_BBOX_HEIGHT / 2;
	right = left + MARIO_TAIL_BBOX_WIDTH;
	bottom = top + MARIO_TAIL_BBOX_HEIGHT;
}

void CMarioTail::Render()
{
	RenderBoundingBox();
}

void CMarioTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float mx, my;
	CMario* mario = dynamic_cast<CMario*>(((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer());
	
	mario->GetPosition(mx, my);
	this->SetPosition(mx - 5 * mario->GetNx(), my + 6);

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}