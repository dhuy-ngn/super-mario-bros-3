#pragma once

#include "GameObject.h"

#define BULLET_BBOX_WIDTH	9
#define BULLET_BBOX_HEIGHT	9

#define BULLET_SPEED_X	0.05f
#define BULLET_SPEED_Y	0.04f
#define BULLET_SPEED_Y_MAX  0.03f

#define BULLET_ANI_RIGHT		7120
#define BULLET_ANI_LEFT			7121	

class CFireBullet : public CGameObject
{
    virtual void Render();
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
public:
    CFireBullet(float bx, float by, bool up, bool right);
    void GetBoundingBox(float& l, float& t, float& r, float& b);
    int IsCollidable() { return 1; };
    int IsBlocking() { return 0; }
};