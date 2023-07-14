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

class CCard : public CGameObject
{
public:
	int card = 0;
	ULONGLONG start = 0;
	CCard();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	int GetCard() { return this->card; }
};