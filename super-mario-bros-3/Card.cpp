#include "Card.h"

CCard::CCard() {
	vx = 0;
	vy = 0;
	card = CARD_STATE_RANDOM;
}

void CCard::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - CARD_BBOX_WIDTH / 2;
	t = y - CARD_BBOX_WIDTH / 2;
	r = l + CARD_BBOX_WIDTH;
	b = t + CARD_BBOX_WIDTH;
}

void CCard::Render() 
{
	switch (card)
	{
	case 0:
		CAnimations::GetInstance()->Get(ID_ANI_CARD_RANDOM)->Render(x, y);
		break;
	case 1:
		CSprites::GetInstance()->Get(ID_SPRITE_CARD_MUSHROOM)->Draw(x, y);
		break;
	case 2:
		CSprites::GetInstance()->Get(ID_SPRITE_CARD_FIREFLOWER)->Draw(x, y);
		break;
	case 3:
		CSprites::GetInstance()->Get(ID_SPRITE_CARD_STAR)->Draw(x, y);
		break;
	}
}

void CCard::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CGameObject::Update(dt);
	if (start == 0) 
	{
		start = GetTickCount64();
	}
	if (GetTickCount64() - start > CARD_RANDOM_TIME) 
	{
		card++;
		if (card > 3) 
		{
			card = 1;
		}
		start = 0;
	}
}