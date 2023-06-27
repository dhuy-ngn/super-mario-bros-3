#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_UP_DURATION	500
#define COIN_EXIST_DURATION 1000
#define COIN_SPEED	0.5f
#define COIN_GRAVITY 0.002f

#define COIN_STATE_NORMAL	0
#define COIN_STATE_FROM_QUESTION_BLOCK	1

class CCoin : public CGameObject {
protected:
	BOOLEAN isFromQuestionBlock;
	ULONGLONG appear_time;

	float ay;
	float start_y;
public:
	CCoin(float x, float y, BOOLEAN isFromQuestionBlock = false) : CGameObject(x, y)
	{
		this->start_y = y;
		this->ay = 0;
		this->isFromQuestionBlock = isFromQuestionBlock;
		this->appear_time = -1;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	void StartAppearing();
};