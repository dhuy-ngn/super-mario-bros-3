#pragma once

#include "Platform.h"

class CColorBlock : public CPlatform
{
public:
	CColorBlock(float x, float y,
		float cell_width, float cell_height, int length)
		: CPlatform(x, y, cell_width, cell_height, length, 90000, 90000, 90000) {}

	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	int IsBlocking() { return 1; }
};