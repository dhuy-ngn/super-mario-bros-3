#include "Coin.h"
#include "debug.h"
#include "PlayScene.h"

void CCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_COIN)->Render(x, y);

	//RenderBoundingBox();
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	vy += ay * dt;
	y += vy * dt;

	if (isFromQuestionBlock && GetTickCount64() - appear_time > COIN_EXIST_DURATION || y > start_y)
	{
		isDeleted = true;
		CMario* mario = dynamic_cast<CMario*>(((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer());
		mario->AddScore(x, start_y);
		return;
	}
}

void CCoin::StartAppearing()
{
	this->appear_time = GetTickCount64();
	vy = -COIN_SPEED;
	ay = COIN_GRAVITY;
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}