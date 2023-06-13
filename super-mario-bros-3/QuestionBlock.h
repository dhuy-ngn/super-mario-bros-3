#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"
#include "Mushroom.h"
#include "Coin.h"
#include "Leaf.h"

#define QUESTION_BLOCK_BBOX_WIDTH	15
#define	QUESTION_BLOCK_BBOX_HEIGHT 15

#define ID_ANI_QUESTION_BLOCK_ACTIVE  7001
#define ID_ANI_QUESTION_BLOCK_INACTIVE  7000 

#define QUESTION_BLOCK_STATE_ACTIVE	100
#define	QUESTION_BLOCK_STATE_INACTIVE	-100

#define	QUESTION_BLOCK_DEFLECT_SPEED 0.1f
#define QUESTION_BLOCK_DEFLECT_DURATION 25

#define QUESTION_BLOCK_CONTAINS_COIN	0
#define QUESTION_BLOCK_CONTAINS_MUSHROOM	1
#define QUESTION_BLOCK_CONTAINS_1UP	2
#define QUESTION_BLOCK_CONTAINS_PSWITCH	3


class CQuestionBlock : public CGameObject
{
protected:
	BOOLEAN isExhausted;

	ULONGLONG deflect_start;
	ULONGLONG deflect_end;

	float start_y;
	float vy;

	int contain = 0;

	CCoin* coin = NULL;
	CMushroom* mushroom = NULL;
	CLeaf* leaf = NULL;

public:
	CQuestionBlock(float x, float y, int contain) : CGameObject(x, y)
	{
		this->x = x;
		this->y = y;
		this->start_y = y;
		this->isExhausted = false;
		this->deflect_start = -1;
		this->deflect_end = -1;
		this->vy = 0;
		this->contain = contain;
	}
	int IsExhausted() { return isExhausted; };

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetContain() { return this->contain; }

	void ReleaseItem();
};