#pragma once

#include "GameObject.h"

#define MARIO_ENTER_PIPE_INTERVAL 1500
/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 

	float width;
	float height; 

	float start_x;
	float start_y;

	int isUpward;

public:
	CPortal(float l, float t, float r, float b, int scene_id, float start_x, float start_y, int isUpward);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void RenderBoundingBox(void);
	
	int GetSceneId() { return scene_id;  }
	float GetSceneStartX() { return this->start_x; }
	float GetSceneStartY() { return this->start_y; }
	int IsBlocking() { return 0; }
};