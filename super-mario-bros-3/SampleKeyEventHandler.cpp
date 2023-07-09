#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_S:
		mario->SetState(MARIO_STATE_JUMP);
		if (mario->CanFly())
			mario->SetState(MARIO_STATE_FLY);
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_RACCOON);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_R: // reset
		//Reload();
		break;
	case DIK_A:
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON && mario->IsIdle())
			mario->SetState(MARIO_STATE_ATTACK);
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S:
		mario->SetState(MARIO_STATE_RELEASE_JUMP);
		if (game->IsKeyDown(DIK_A))
			mario->HoldKoopaShell();
		break;
	case DIK_D:
		mario->SetState(MARIO_STATE_ATTACK_RELEASE);
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	case DIK_X:
		mario->SetState(MARIO_STATE_LAND);
		break;
	case DIK_A:
		if (mario->IsHoldingKoopaShell())
			mario->ReleaseKoopaShell();
		else 
			mario->SetState(MARIO_STATE_ATTACK_RELEASE);
		mario->SetState(MARIO_STATE_IDLE);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE* states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A))
		{
			mario->HoldKoopaShell();
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		}
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A))
		{
			mario->HoldKoopaShell();
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		}
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else if (game->IsKeyDown(DIK_X))
	{
		mario->SetState(MARIO_STATE_LAND);
		if (game->IsKeyDown(DIK_LEFT))
			mario->SetState(MARIO_STATE_LAND_LEFT);
		if (game->IsKeyDown(DIK_RIGHT))
			mario->SetState(MARIO_STATE_LAND_RIGHT);
	}
	else if (game->IsKeyDown(DIK_S))
	{
		if (mario->CanFly())
		{
			if (!mario->IsFlying())
				mario->StartFlying();
			mario->SetState(MARIO_STATE_FLY);
			if (game->IsKeyDown(DIK_LEFT))
				mario->SetState(MARIO_STATE_FLY_LEFT);
			if (game->IsKeyDown(DIK_RIGHT))
				mario->SetState(MARIO_STATE_FLY_RIGHT);
			if (game->IsKeyDown(DIK_A))
				mario->HoldKoopaShell();
		}
	}
	else if (game->IsKeyDown(DIK_A))
	{
		if (mario->IsHoldingKoopaShell())
			mario->SetState(MARIO_STATE_HOLD_KOOPA_SHELL);
	}
	else
	{
		mario->SetState(MARIO_STATE_IDLE);
	}
}