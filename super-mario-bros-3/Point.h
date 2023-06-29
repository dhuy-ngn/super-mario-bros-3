#pragma once

#include "GameObject.h"

#define SCORE_SPRITE_100_ID		50049
#define SCORE_SPRITE_200_ID		50050
#define SCORE_SPRITE_400_ID		50051
#define SCORE_SPRITE_800_ID		50052
#define SCORE_SPRITE_1000_ID	50053
#define SCORE_SPRITE_2000_ID	50054
#define SCORE_SPRITE_4000_ID	50055
#define SCORE_SPRITE_8000_ID	50056
#define SCORE_SPRITE_1UP_ID		50057

#define SCORE_STACK_100     0
#define SCORE_STACK_200     1
#define SCORE_STACK_400     2
#define SCORE_STACK_800     3
#define SCORE_STACK_1000    4
#define SCORE_STACK_2000    5
#define SCORE_STACK_4000    6
#define SCORE_STACK_8000    7

#define SCORE_SPEED_Y 0.05f

#define SCORE_EXIST_DURATION 500

class CPoint : public CGameObject
{
    ULONGLONG appear_time = 0;
    int score_stack;

    virtual void Render();
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
public:
    CPoint(int score_stack);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

    int IsBlocking() { return 0; }
};

