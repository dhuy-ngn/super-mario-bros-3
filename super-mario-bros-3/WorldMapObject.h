#pragma once
#include "GameObject.h"

#define OBJECT_TYPE_PLAYER				0
#define OBJECT_TYPE_BRICK				1
#define OBJECT_TYPE_NODE				2
#define OBJECT_TYPE_PORTAL				3
#define OBJECT_TYPE_DECORATION			4
#define OBJECT_TYPE_HAMMER				5

#define HAMMER_LIMIT_RIGHT		84
#define HAMMET_LIMIT_LEFT       46
#define OBJECT_BBOX_WIDTH		4

#define START_WALK_LIMIT	10000

#define ID_ANI_BUSH		401

class CWorldMapObject : public CGameObject
{
	int object_type;
	int scene_id;
	int ani_id;

public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual int IsBlocking() { return 0; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	CWorldMapObject(int object_type = 4, int scene_id = -1, int ani_id = ID_ANI_BUSH);
	int GetSceneId() { return scene_id; };
	bool cgLeft, cgRight, cgUp, cgDown;
	void SetDirection(bool cgLeft, bool cgUp, bool cgRight, bool cgDown)
	{
		this->cgLeft = cgLeft;
		this->cgRight = cgRight;
		this->cgUp = cgUp;
		this->cgDown = cgDown;
	};
	void GetDirection(bool& cgLeft, bool& cgUp, bool& cgRight, bool& cgDown)
	{
		cgLeft = this->cgLeft;
		cgRight = this->cgRight;
		cgUp = this->cgUp;
		cgDown = this->cgDown;
	};
	int GetType() { return this->object_type; }
};
