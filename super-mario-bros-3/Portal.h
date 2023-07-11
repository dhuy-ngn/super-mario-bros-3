#pragma once

#include "GameObject.h"

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
public:
	CPortal(float l, float t, float r, float b, int scene_id, float start_x, float start_y);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	void RenderBoundingBox(void);
	
	int GetSceneId() { return scene_id;  }
	float GetSceneStartX() { return start_x; }
	float GetSceneStartY() { return start_y; }
	int IsBlocking() { return 0; }
};