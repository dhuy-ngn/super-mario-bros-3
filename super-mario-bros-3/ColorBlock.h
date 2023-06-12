#pragma once

#include "Platform.h"

class CColorBlock : public CPlatform
{
protected:
	int isBlocking;

public:
	CColorBlock(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end) : CPlatform(x, y, cell_width, cell_height, length, sprite_id_begin, sprite_id_middle, sprite_id_end)
	{
		this->isBlocking = 0;
	}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	int IsBlocking() { return this->isBlocking; }
	void SetIsBlocking(int value) { this->isBlocking = value; };
};