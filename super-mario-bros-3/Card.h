#pragma once
#include "GameObject.h"

#define CARD_STATE_RANDOM		0
#define CARD_STATE_MUSHROOM		1
#define CARD_STATE_FIREFLOWER	2
#define CARD_STATE_STAR			3

#define ID_ANI_CARD_RANDOM				5007
#define ID_SPRITE_CARD_MUSHROOM		50010
#define ID_SPRITE_CARD_FIREFLOWER	50011
#define ID_SPRITE_CARD_STAR			50012

#define CARD_RANDOM_TIME	100

#define CARD_BBOX_WIDTH		16

#define CARD_RISING_SPEED	0.1f

class CCard : public CGameObject
{
public:
	int card = 0;
	ULONGLONG start = 0;
	BOOLEAN isPicked = false;
	ULONGLONG picked_by_mario_start = 0;
	CCard(float x, float y);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	void PickedByMario()
	{
		isPicked = true;
		picked_by_mario_start = GetTickCount64();
	}

	int GetCard() { return this->card; }
};