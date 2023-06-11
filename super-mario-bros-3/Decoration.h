#pragma once
#include "GameObject.h"

class CDecoration : public CGameObject
{
protected:		
	float cellWidth;
	float cellHeight;
	int spriteId;

public:
	CDecoration(float x, float y,
		float cell_width, float cell_height, int spriteId) :CGameObject(x, y)
	{
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteId = spriteId;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	int IsBlocking() { return 0; }
};
