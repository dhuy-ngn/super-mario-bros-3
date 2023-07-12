#include "Portal.h"
#include "Mario.h"
#include "Textures.h"
#include "PlayScene.h"

CPortal::CPortal(float l, float t, float r, float b, int scene_id, float start_x, float start_y, int isUpward )
{
	this->scene_id = scene_id;
	x = l; 
	y = t;
	width = r - l;
	height = b - t;
	this->start_x = start_x;
	this->start_y = start_y;
	this->isUpward = isUpward;
}

void CPortal::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	CGame::GetInstance()->Draw(x - cx, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CPortal::Render()
{
	// RenderBoundingBox();
}

void CPortal::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - width/2;
	t = y - height/2;
	r = x + width/2;
	b = y + height/2;
}

void CPortal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = dynamic_cast<CMario*>(((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer());
	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;

	mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
	GetBoundingBox(oLeft, oTop, oRight, oBottom);

	if (mRight >= oLeft && mLeft <= oRight && mBottom >= oTop && mTop <= oBottom)
	{
		if (isUpward)
		{
			if (CGame::GetInstance()->IsKeyDown(DIK_UP))
			{
				mario->SetState(MARIO_STATE_ENTERING_PIPE_UP);
				mario->StartEnteringPipe();
			}
		}
		else
		{
			if (CGame::GetInstance()->IsKeyDown(DIK_DOWN))
			{
				mario->SetState(MARIO_STATE_ENTERING_PIPE_DOWN);
				mario->StartEnteringPipe();
			}
		}

		if (mario->IsPipeDown() || mario->IsPipeUp())
		{
			if (GetTickCount64() - mario->GetPipeEnterStartTime() > MARIO_ENTER_PIPE_INTERVAL)
			{
				CGame::GetInstance()->SwitchMarioToScene(scene_id, start_x, start_y);
			}
		}
	}

	CGameObject::Update(dt);
}
