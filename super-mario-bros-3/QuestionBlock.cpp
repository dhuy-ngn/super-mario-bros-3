#include "QuestionBlock.h"
#include "PlayScene.h"

void CQuestionBlock::Render()
{
	int aniId = ID_ANI_QUESTION_BLOCK_ACTIVE;
	if (state == QUESTION_BLOCK_STATE_INACTIVE)
	{
		aniId = ID_ANI_QUESTION_BLOCK_INACTIVE;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CQuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	y += vy * dt;

	if (state == QUESTION_BLOCK_STATE_INACTIVE)
	{
		if (GetTickCount64() - deflect_start >= QUESTION_BLOCK_DEFLECT_DURATION)
		{
			vy = QUESTION_BLOCK_DEFLECT_SPEED;
		}
		if (GetTickCount64() - deflect_end >= QUESTION_BLOCK_DEFLECT_DURATION && y >= start_y)
		{
			y = start_y;
			vy = 0;
		}
	}

	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = dynamic_cast<CMario*>(current_scene->GetPlayer());
	CMarioTail* tail = mario->GetMarioTail();
	if (mario->GetLevel() == MARIO_LEVEL_RACCOON && mario->IsAttacking() && state != QUESTION_BLOCK_STATE_INACTIVE)
	{
		float tLeft, tTop, tRight, tBottom;
		float oLeft, oTop, oRight, oBottom;

		tail->GetBoundingBox(tLeft, tTop, tRight, tBottom);
		GetBoundingBox(oLeft, oTop, oRight, oBottom);

		if (tRight >= oLeft && tLeft <= oRight && tBottom >= oTop && tTop <= oBottom)
		{
			SetState(QUESTION_BLOCK_STATE_INACTIVE);
			if (GetContain() == QUESTION_BLOCK_CONTAINS_COIN)
				mario->GainCoin();
			ReleaseItem();
		}
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CQuestionBlock::SetState(int state) {
	switch (state) {
	case QUESTION_BLOCK_STATE_ACTIVE:
		vy = 0;
		break;
	case QUESTION_BLOCK_STATE_INACTIVE:
		deflect_start = GetTickCount64();
		deflect_end = deflect_start + QUESTION_BLOCK_DEFLECT_DURATION;
		vy = -QUESTION_BLOCK_DEFLECT_SPEED;
		break;
	}


	CGameObject::SetState(state);
}

void CQuestionBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - QUESTION_BLOCK_BBOX_WIDTH / 2;
	t = y - QUESTION_BLOCK_BBOX_HEIGHT / 2;
	r = l + QUESTION_BLOCK_BBOX_WIDTH;
	b = t + QUESTION_BLOCK_BBOX_HEIGHT;
}

void CQuestionBlock::ReleaseItem()
{
	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = dynamic_cast<CMario*>(current_scene->GetPlayer());

	switch (contain)
	{
	case QUESTION_BLOCK_CONTAINS_COIN:
		coin = new CCoin(x, y - QUESTION_BLOCK_BBOX_HEIGHT, true);
		coin->StartAppearing();
		current_scene->UnshiftObject(coin);
		break;
	case QUESTION_BLOCK_CONTAINS_MUSHROOM:
		switch (mario->GetLevel())
		{
		case MARIO_LEVEL_SMALL:
			mushroom = new CMushroom(x, y);
			current_scene->UnshiftObject(mushroom);
			mushroom->SetState(MUSHROOM_STATE_IDLE);
			break;
		default:
			leaf = new CLeaf(x, y);
			current_scene->UnshiftObject(leaf);
			leaf->SetState(LEAF_STATE_UP);
			break;
		}
	default:
		break;
	}
}
