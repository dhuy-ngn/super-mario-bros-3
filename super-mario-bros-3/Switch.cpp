#include "Switch.h"
#include "Brick.h"
#include "PlayScene.h"
#include "Coin.h"

void CSwitch::Render() 
{
	int aniId = SWITCH_ANI_IDLE;
	CAnimations* animations = CAnimations::GetInstance();
	if (state == SWITCH_STATE_PRESSED)
		aniId = SWITCH_ANI_PRESSED;
	else
		aniId = SWITCH_ANI_IDLE;
	animations->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CSwitch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (state == SWITCH_STATE_IDLE)
	{
		y += vy * dt;
		if (start_y - y >= SWITCH_BBOX_HEIGHT)
		{
			vy = 0;
			y = start_y - SWITCH_BBOX_HEIGHT - 0.1f;
			SetState(SWITCH_STATE_IDLE);
		}
	}
	else
	{
		y = start_y - SWITCH_BBOX_PRESSED_HEIGHT - 0.1f;
	}
	if (GetTickCount64() - switch_effect_start > SWITCH_MAX_INTERVAL && switch_effect_start != 0)
	{
		switch_effect_start = GetTickCount64();
		CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		vector<LPGAMEOBJECT> objects = currentScene->GetAllObject();
		for (size_t i = 0; i < objects.size(); i++)
		{
			if (dynamic_cast<CBrick*>(objects.at(i)) && !objects.at(i)->IsDeleted() && dynamic_cast<CBrick*>(objects.at(i))->GetItemType() == 0)
			{
				float bx, by;
				CBrick* brick = dynamic_cast<CBrick*>(objects.at(i));
				brick->GetPosition(bx, by);
				CCoin* coin = new CCoin(bx, by);
				currentScene->PushObject(coin);
				brick->Delete();
			}
			else if (dynamic_cast<CCoin*>(objects.at(i)) && !objects.at(i)->IsDeleted())
			{
				float cx, cy;
				CCoin* coin = dynamic_cast<CCoin*>(objects.at(i));
				coin->GetPosition(cx, cy);
				CBrick* brick = new CBrick(cx, cy);
				currentScene->PushObject(brick);
				coin->Delete();
			}
		}
	}
}

void CSwitch::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case SWITCH_STATE_IDLE:
		vy = -0.05f;
		break;
	case SWITCH_STATE_PRESSED:
		switch_effect_start = GetTickCount64();
		CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		vector<LPGAMEOBJECT> objects = currentScene->GetAllObject();
		for (size_t i = 0; i < objects.size(); i++)
		{
			if (dynamic_cast<CBrick*>(objects.at(i)) && !objects.at(i)->IsDeleted() && dynamic_cast<CBrick*>(objects.at(i))->GetItemType() == 0)
			{
				float bx, by;
				CBrick* brick = dynamic_cast<CBrick*>(objects.at(i));
				brick->GetPosition(bx, by);
				CCoin* coin = new CCoin(bx, by);
				currentScene->PushObject(coin);
				brick->Delete();
			}
			else if (dynamic_cast<CCoin*>(objects.at(i)) && !objects.at(i)->IsDeleted())
			{
				float cx, cy;
				CCoin* coin = dynamic_cast<CCoin*>(objects.at(i));
				coin->GetPosition(cx, cy);
				CBrick* brick = new CBrick(cx, cy);
				currentScene->PushObject(brick);
				coin->Delete();
			}
		}
		break;
	}
}

void CSwitch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - SWITCH_BBOX_WIDTH / 2;
	t = y - SWITCH_BBOX_HEIGHT / 2;
	r = l + SWITCH_BBOX_WIDTH;
	b = t + SWITCH_BBOX_HEIGHT;
}