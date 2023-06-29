#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Koopa.h"
#include "Coin.h"
#include "Portal.h"

#include "Collision.h"
#include "QuestionBlock.h"
#include "ColorBlock.h"
#include "FireTrap.h"
#include "PlayScene.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    vy += ay * dt;
    vx += ax * dt;

    if (abs(vx) > abs(maxVx)) vx = maxVx;
    
    if (abs(vy) > abs(maxVy)) vy = maxVy;

    if (abs(vx) == MARIO_RUNNING_SPEED)
    {
        canFly = true;
    }
    if (abs(vx) < MARIO_RUNNING_SPEED && !isFlying)
    {
        canFly = false;
    }

    // reset untouchable timer if untouchable time has passed
    if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
    {
        untouchable_start = 0;
        untouchable = 0;
    }

    if (GetTickCount64() - attacking_start > MARIO_ATTACKING_DURATION)
    {
        attacking_start = 0;
    }

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;

    if (y > 200)
        // TODO: De-hardcoding the 200
    {
        SetState(MARIO_STATE_FALL_OFF);
    }
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (e->ny != 0 && e->obj->IsBlocking())
    {
        vy = 0;
        if (e->ny < 0)
            isOnPlatform = true;
    }
    else
        if (e->nx != 0 && e->obj->IsBlocking())
        {
            vx = 0;
        }

    if (dynamic_cast<CGoomba*>(e->obj))
        OnCollisionWithGoomba(e);
    else if (dynamic_cast<CKoopa*>(e->obj))
        OnCollisionWithKoopa(e);
    else if (dynamic_cast<CCoin*>(e->obj))
        OnCollisionWithCoin(e);
    else if (dynamic_cast<CPortal*>(e->obj))
        OnCollisionWithPortal(e);
    else if (dynamic_cast<CQuestionBlock*>(e->obj))
        OnCollisionWithQuestionBlock(e);
    else if (dynamic_cast<CColorBlock*>(e->obj))
        OnCollisionWithColorBlock(e);
    // Collide with Piranha Plants
    else if (dynamic_cast<CFireTrap*>(e->obj))
        OnCollisionWithFireTrap(e);
    else if (dynamic_cast<CFireBullet*>(e->obj))
        OnCollisionWithFireBullet(e);
    else if (dynamic_cast<CMushroom*>(e->obj))
        OnCollisionWithMushroom(e);
    else if (dynamic_cast<CLeaf*>(e->obj))
        OnCollisionWithLeaf(e);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
    CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

    // jump on top >> kill Goomba and deflect a bit 
    if (e->ny < 0)
    {
        if (goomba->GetState() != GOOMBA_STATE_DIE)
        {
            if (goomba->GetLevel() == GOOMBA_LEVEL_PARA)
            {
                goomba->SetLevel(GOOMBA_LEVEL_NORMAL);
                goomba->SetState(GOOMBA_STATE_WALKING);
            }
            else
                goomba->SetState(GOOMBA_STATE_DIE);
            vy = -MARIO_JUMP_DEFLECT_SPEED;
            isOnPlatform = false;
        }
    }
    else // hit by Goomba
    {
        if (untouchable == 0)
            if (goomba->GetState() != GOOMBA_STATE_DIE && goomba->GetState() != GOOMBA_STATE_KNOCKED_OUT)
                LevelDown();
    }
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
    CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
    CGame* game = CGame::GetInstance();

    // jump on top
    if (e->ny < 0)
    {
        if (koopa->IsHiding())
        {
            if (nx >= 0)
                koopa->SetState(KOOPA_STATE_SPINNING_RIGHT);
            else
                koopa->SetState(KOOPA_STATE_SPINNING_LEFT);
        }
        else
        {
            if (koopa->GetState() == KOOPA_STATE_SKIPPING)
            {
                koopa->SetLevel(KOOPA_LEVEL_NORMAL);
                koopa->SetState(KOOPA_STATE_WALKING);
            }
            else
                koopa->SetState(KOOPA_STATE_HIDING);
        }
        vy = -MARIO_JUMP_DEFLECT_SPEED;
    }
    else // hit by Koopa
    {
        if (koopa->GetState() == KOOPA_STATE_HIDING && koopa->GetState() != KOOPA_STATE_KNOCKED_OUT)
            if (game->IsKeyDown(DIK_A))
            {
                SetState(MARIO_STATE_HOLD_KOOPA_SHELL);
                isHoldingKoopaShell = true;
                koopa->SetState(KOOPA_STATE_HELD_BY_MARIO);
            }
            else
                if (ax >= 0)
                    koopa->SetState(KOOPA_STATE_SPINNING_RIGHT);
                else
                    koopa->SetState(KOOPA_STATE_SPINNING_LEFT);
        else
        {
            if (untouchable == 0)
                LevelDown();
        }
    }
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
    e->obj->Delete();
    coin++;
}

void CMario::OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e)
{
    CQuestionBlock* question_block = dynamic_cast<CQuestionBlock*>(e->obj);

    if ((question_block->GetState() != QUESTION_BLOCK_STATE_INACTIVE && e->ny > 0) || (IsAttacking() && e->nx != 0))
    {
        question_block->SetState(QUESTION_BLOCK_STATE_INACTIVE);
        if (question_block->GetContain() == QUESTION_BLOCK_CONTAINS_COIN)
            coin++;
        question_block->ReleaseItem();
    }
}

void CMario::OnCollisionWithColorBlock(LPCOLLISIONEVENT e)
{
    CColorBlock* color_block = dynamic_cast<CColorBlock*>(e->obj);

    if (e->ny < 0)
    {
        isOnPlatform = true;
        vy = 0;
    }
}

void CMario::OnCollisionWithFireTrap(LPCOLLISIONEVENT e) 
{
    CFireTrap* fire_trap = dynamic_cast<CFireTrap*>(e->obj);

    if (untouchable == 0)
    {
        if (fire_trap->GetState() != FIRETRAP_STATE_DEATH)
            LevelDown();
    }
}

void CMario::OnCollisionWithFireBullet(LPCOLLISIONEVENT e)
{
    if (untouchable == 0)
        LevelDown();
}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
    if (level == MARIO_LEVEL_SMALL)
    {
        SetLevel(MARIO_LEVEL_BIG);
        e->obj->Delete();
    }
}

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
    if (level == MARIO_LEVEL_BIG)
    {
        SetLevel(MARIO_LEVEL_RACCOON);
    }
    e->obj->Delete();
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
    CPortal* p = (CPortal*)e->obj;
    CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
    int aniId = -1;
    if (!isOnPlatform)
    {
        if (abs(vx) == MARIO_RUNNING_SPEED)
        {
            if (nx >= 0)
                aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
            else
                aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
        }
        else
        {
            if (isHoldingKoopaShell)
                if (nx > 0)
                    aniId = ID_ANI_MARIO_SMALL_HOLD_KOOPA_JUMP_RIGHT;
                else
                    aniId = ID_ANI_MARIO_SMALL_HOLD_KOOPA_JUMP_LEFT;
            else
                if (nx >= 0)
                    aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
                else
                    aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
        }
    }
    else
        if (isSitting)
        {
            if (nx > 0)
                aniId = ID_ANI_MARIO_SIT_RIGHT;
            else
                aniId = ID_ANI_MARIO_SIT_LEFT;
        }
        else
            if (vx == 0)
            {
                if (isHoldingKoopaShell)
                    if (nx > 0)
                        aniId = ID_ANI_MARIO_SMALL_HOLD_KOOPA_IDLE_RIGHT;
                    else
                        aniId = ID_ANI_MARIO_SMALL_HOLD_KOOPA_IDLE_LEFT;
                else
                    if (nx > 0)
                        aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
                    else
                        aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
            }
            else if (vx > 0)
            {
                if (isHoldingKoopaShell)
                    aniId = ID_ANI_MARIO_SMALL_HOLD_KOOPA_WALK_RIGHT;
                else
                    if (ax < 0)
                        aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
                    else if (ax == MARIO_ACCEL_RUN_X)
                        if (vx == MARIO_RUNNING_SPEED)
                            aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
                        else
                            aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
                    else if (ax == MARIO_ACCEL_WALK_X)
                        aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
            }
            else // vx < 0
            {
                if (isHoldingKoopaShell)
                    aniId = ID_ANI_MARIO_SMALL_HOLD_KOOPA_WALK_LEFT;
                else
                    if (ax > 0)
                        aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
                    else if (ax == -MARIO_ACCEL_RUN_X)
                        if (vx == -MARIO_RUNNING_SPEED)
                            aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
                        else
                            aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
                    else
                        if (ax == -MARIO_ACCEL_WALK_X)
                            aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
            }

    if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

    return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
    int aniId = -1;
    if (!isOnPlatform)
    {
        if (isHoldingKoopaShell)
        {
            if (nx >= 0)
                aniId = ID_ANI_MARIO_HOLD_KOOPA_JUMP_RIGHT;
            else
                aniId = ID_ANI_MARIO_HOLD_KOOPA_JUMP_LEFT;
        }
        else
            if (abs(vx) == MARIO_RUNNING_SPEED)
            {
                if (nx >= 0)
                    aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
                else
                    aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
            }
            else
            {
                if (nx >= 0)
                    aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
                else
                    aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
            }
    }
    else
        if (isSitting)
        {
            if (nx > 0)
                aniId = ID_ANI_MARIO_SIT_RIGHT;
            else
                aniId = ID_ANI_MARIO_SIT_LEFT;
        }
        else
            if (vx == 0)
            {
                if (isHoldingKoopaShell)
                    if (nx > 0)
                        aniId = ID_ANI_MARIO_HOLD_KOOPA_IDLE_RIGHT;
                    else
                        aniId = ID_ANI_MARIO_HOLD_KOOPA_IDLE_LEFT;
                else
                    if (nx > 0)
                        aniId = ID_ANI_MARIO_IDLE_RIGHT;
                    else
                        aniId = ID_ANI_MARIO_IDLE_LEFT;
            }
            else if (vx > 0)
            {
                if (isHoldingKoopaShell)
                    aniId = ID_ANI_MARIO_HOLD_KOOPA_WALK_RIGHT;
                else
                    if (ax < 0)
                        aniId = ID_ANI_MARIO_BRACE_RIGHT;
                    else if (ax == MARIO_ACCEL_RUN_X)
                        if (vx == MARIO_RUNNING_SPEED) // Mario can only run when his speed reaches running speed
                            aniId = ID_ANI_MARIO_RUNNING_RIGHT;
                        else
                            aniId = ID_ANI_MARIO_WALKING_RIGHT;
                    else if (ax == MARIO_ACCEL_WALK_X)
                        aniId = ID_ANI_MARIO_WALKING_RIGHT;
            }
            else // vx < 0
            {
                if (isHoldingKoopaShell)
                    aniId = ID_ANI_MARIO_HOLD_KOOPA_WALK_LEFT;
                else
                    if (ax > 0)
                        aniId = ID_ANI_MARIO_BRACE_LEFT;
                    else
                        if (ax == -MARIO_ACCEL_RUN_X)
                            if (vx == -MARIO_RUNNING_SPEED)
                                aniId = ID_ANI_MARIO_RUNNING_LEFT;
                            else
                                aniId = ID_ANI_MARIO_WALKING_LEFT;
                        else
                            if (ax == -MARIO_ACCEL_WALK_X)
                                aniId = ID_ANI_MARIO_WALKING_LEFT;
            }

    if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

    return aniId;
}

int CMario::GetAniIdRaccoon()
{
    int aniId = -1;
    if (!isOnPlatform)
    {
        if (!isFlying && !isLanding)
            if (isHoldingKoopaShell)
                if (nx >= 0)
                    aniId = ID_ANI_MARIO_RACCOON_HOLD_KOOPA_JUMP_RIGHT;
                else
                    aniId = ID_ANI_MARIO_RACCOON_HOLD_KOOPA_JUMP_LEFT;
            else
                if (abs(ax) == MARIO_ACCEL_RUN_X)
                {
                    if (abs(vx) == MARIO_RUNNING_SPEED)
                        if (nx >= 0)
                            aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT;
                        else
                            aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT;
                    else
                        if (nx >= 0)
                            aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT;
                        else
                            aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT;
                }
                else
                {
                    if (nx >= 0)
                        aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT;
                    else
                        aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT;
                }
        else
        {
            if (isFlying)
                if (nx >= 0)
                    aniId = ID_ANI_MARIO_RACCOON_FLYING_RIGHT;
                else
                    aniId = ID_ANI_MARIO_RACCOON_FLYING_LEFT;
            if (isLanding)
                if (nx >= 0)
                    aniId = ID_ANI_MARIO_RACCOON_LANDING_RIGHT;
                else
                    aniId = ID_ANI_MARIO_RACCOON_LANDING_LEFT;
        }

    }
    else
        if (isAttacking) 
            aniId = ID_ANI_MARIO_RACCOON_ATTACK;
        else
            if (isSitting)
            {
                if (nx > 0)
                    aniId = ID_ANI_MARIO_RACCOON_SIT_RIGHT;
                else
                    aniId = ID_ANI_MARIO_RACCOON_SIT_LEFT;
            }
            else
                if (vx == 0)
                {
                    if (isHoldingKoopaShell)
                        if (nx > 0)
                            aniId = ID_ANI_MARIO_RACCOON_HOLD_KOOPA_IDLE_RIGHT;
                        else
                            aniId = ID_ANI_MARIO_RACCOON_HOLD_KOOPA_IDLE_LEFT;
                    else
                        if (nx > 0)
                            aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;
                        else
                            aniId = ID_ANI_MARIO_RACCOON_IDLE_LEFT;
                }
                else if (vx > 0)
                {
                    if (isHoldingKoopaShell)
                        aniId = ID_ANI_MARIO_RACCOON_HOLD_KOOPA_WALK_RIGHT;
                    else
                        if (ax < 0)
                            aniId = ID_ANI_MARIO_RACCOON_BRACE_RIGHT;
                        else if (ax == MARIO_ACCEL_RUN_X)
                            if (vx == MARIO_RUNNING_SPEED) // Mario can only run when his speed reaches running speed
                                aniId = ID_ANI_MARIO_RACCOON_RUNNING_RIGHT;
                            else
                                aniId = ID_ANI_MARIO_RACCOON_WALKING_RIGHT;
                        else if (ax == MARIO_ACCEL_WALK_X)
                            aniId = ID_ANI_MARIO_RACCOON_WALKING_RIGHT;
                }
                else // vx < 0
                {
                    if (isHoldingKoopaShell)
                        aniId = ID_ANI_MARIO_RACCOON_HOLD_KOOPA_WALK_LEFT;
                    else
                        if (ax > 0)
                            aniId = ID_ANI_MARIO_RACCOON_BRACE_LEFT;
                        else if (ax == -MARIO_ACCEL_RUN_X)
                            if (vx == -MARIO_RUNNING_SPEED)
                                aniId = ID_ANI_MARIO_RACCOON_RUNNING_LEFT;
                            else
                                aniId = ID_ANI_MARIO_RACCOON_WALKING_LEFT;
                        else if (ax == -MARIO_ACCEL_WALK_X)
                            aniId = ID_ANI_MARIO_RACCOON_WALKING_LEFT;
                }

    if (aniId == -1) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;

    return aniId;
}

void CMario::Render()
{
    CAnimations* animations = CAnimations::GetInstance();
    int aniId = -1;

    if (state == MARIO_STATE_DIE)
        aniId = ID_ANI_MARIO_DIE;
    else if (level == MARIO_LEVEL_BIG)
        aniId = GetAniIdBig();
    else if (level == MARIO_LEVEL_SMALL)
        aniId = GetAniIdSmall();
    else if (level == MARIO_LEVEL_RACCOON)
        aniId = GetAniIdRaccoon();

    animations->Get(aniId)->Render(x, y);

    // RenderBoundingBox();

    DebugOutTitle(L"Mario can fly: %d", canFly);
}

void CMario::SetState(int state)
{
    // DIE is the end state, cannot be changed! 
    if (this->state == MARIO_STATE_DIE || this->state == MARIO_STATE_FALL_OFF) return;

    switch (state)
    {
    case MARIO_STATE_RUNNING_RIGHT:
        if (isSitting) break;
        maxVx = MARIO_RUNNING_SPEED;
        ax = MARIO_ACCEL_RUN_X;
        nx = 1;
        break;
    case MARIO_STATE_RUNNING_LEFT:
        if (isSitting) break;
        maxVx = -MARIO_RUNNING_SPEED;
        ax = -MARIO_ACCEL_RUN_X;
        nx = -1;
        break;
    case MARIO_STATE_WALKING_RIGHT:
        if (isSitting) break;
        maxVx = MARIO_WALKING_SPEED;
        ax = MARIO_ACCEL_WALK_X;
        nx = 1;
        break;
    case MARIO_STATE_WALKING_LEFT:
        if (isSitting) break;
        maxVx = -MARIO_WALKING_SPEED;
        ax = -MARIO_ACCEL_WALK_X;
        nx = -1;
        break;

        // MARIO JUMPING
    case MARIO_STATE_JUMP:
        if (isSitting) break;
        if (isAttacking) break;
        if (isLanding) break;
        if (isOnPlatform)
        {
            isOnPlatform = false;
            if (abs(this->vx) == MARIO_RUNNING_SPEED)
                vy = -MARIO_JUMP_RUN_SPEED_Y;
            else
                vy = -MARIO_JUMP_SPEED_Y;
        }
        break;

    case MARIO_STATE_RELEASE_JUMP:
        ay = MARIO_GRAVITY;
        maxVy = 999.0f;
        break;

        // BIG & RACCOON MARIO SITTING
    case MARIO_STATE_SIT:
        if (isAttacking) break;
        if (isHoldingKoopaShell) break;
        if (isFlying) break;
        if (isLanding) break;
        if (isOnPlatform && level != MARIO_LEVEL_SMALL)
        {
            state = MARIO_STATE_IDLE;
            isSitting = true;
            vx = 0; vy = 0.0f;
            if (level == MARIO_LEVEL_BIG)
                y += MARIO_SIT_HEIGHT_ADJUST;
            else
                y += MARIO_RACCOON_SIT_HEIGHT_ADJUST;
        }
        break;

    case MARIO_STATE_SIT_RELEASE:
        if (isSitting)
        {
            isSitting = false;
            state = MARIO_STATE_IDLE;
            if (level == MARIO_LEVEL_BIG)
                y -= MARIO_SIT_HEIGHT_ADJUST;
            else
                y -= MARIO_RACCOON_SIT_HEIGHT_ADJUST;
        }
        break;

        // RACCOON MARIO LAND
    case MARIO_STATE_LAND:
        if (isSitting) break;
        if (isOnPlatform) break;
        if (level != MARIO_LEVEL_RACCOON) break;
        isLanding = true;
        isFlying = false;
        vy = MARIO_LANDING_SPEED;
        ax = 0.0f;
        vx = 0.0f;
        break;

    case MARIO_STATE_LAND_RIGHT:
        if (isSitting) break;
        if (isOnPlatform) break;
        if (level != MARIO_LEVEL_RACCOON) break;
        isLanding = true;
        isFlying = false;
        vy = MARIO_LANDING_SPEED;
        maxVx = MARIO_FLYING_SPEED;
        ax = MARIO_ACCEL_FLYING_X;
        nx = 1;
        break;

    case MARIO_STATE_LAND_LEFT:
        if (isSitting) break;
        if (isOnPlatform) break;
        if (level != MARIO_LEVEL_RACCOON) break;
        isLanding = true;
        isFlying = false;
        vy = MARIO_LANDING_SPEED;
        maxVx = -MARIO_FLYING_SPEED;
        ax = -MARIO_ACCEL_FLYING_X;
        nx = -1;
        break;

    case MARIO_STATE_FLY:
        if (isSitting) break;
        if (isOnPlatform) break;
        if (level != MARIO_LEVEL_RACCOON) break;
        isLanding = false;
        isFlying = true;
        ay = -MARIO_ACCEL_FLYING_Y;
        maxVy = -MARIO_FLYING_SPEED;
        break;

    case MARIO_STATE_FLY_LEFT:
        if (isSitting) break;
        if (isOnPlatform) break;
        if (level != MARIO_LEVEL_RACCOON) break;
        isLanding = false;
        isFlying = true;
        ay = -MARIO_ACCEL_FLYING_Y;
        maxVy = -MARIO_FLYING_SPEED;
        nx = -1;
        break;

    case MARIO_STATE_FLY_RIGHT:
        if (isSitting) break;
        if (isOnPlatform) break;
        if (level != MARIO_LEVEL_RACCOON) break;
        isLanding = false;
        isFlying = true;
        ay = -MARIO_ACCEL_FLYING_Y;
        maxVy = -MARIO_FLYING_SPEED;
        nx = 1;
        break;

    case MARIO_STATE_ATTACK:
        if (isSitting) break;
        if (!isOnPlatform) break;
        if (level != MARIO_LEVEL_RACCOON) break;
        isAttacking = true;
        if (GetTickCount64() - attacking_start > MARIO_ATTACKING_DURATION)
        {
            state = MARIO_STATE_ATTACK_RELEASE;
        }
        break;

    case MARIO_STATE_ATTACK_RELEASE:
        if (!isOnPlatform)  break;
        if (isSitting) break;
        isAttacking = false;
        state = MARIO_STATE_IDLE;
        break;

    case MARIO_STATE_HOLD_KOOPA_SHELL:
        if (isSitting) break;
        ax = 0.0f;
        vx = 0.0f;
        isHoldingKoopaShell = true;
        isFlying = false;
        isLanding = false;
        isAttacking = false;
        break;

    case MARIO_STATE_HOLD_WALK_KOOPA_SHELL_RIGHT:
        maxVx = MARIO_WALKING_SPEED;
        ax = MARIO_ACCEL_WALK_X;
        nx = 1;
        isHoldingKoopaShell = true;
        break;

    case MARIO_STATE_HOLD_WALK_KOOPA_SHELL_LEFT:
        maxVx = -MARIO_WALKING_SPEED;
        ax = -MARIO_ACCEL_WALK_X;
        nx = -1;
        isHoldingKoopaShell = true;
        break;

    case MARIO_STATE_KICK_KOOPA_SHELL:
        isHoldingKoopaShell = false;
        state = MARIO_STATE_IDLE;
        break;

        // IDLE
    case MARIO_STATE_IDLE:
        ax = 0.0f;
        vx = 0.0f;
        attacking_start = 0;
        isFlying = false;
        isLanding = false;
        isAttacking = false;
        isHoldingKoopaShell = false;
        break;

    case MARIO_STATE_DIE:
        vy = -MARIO_JUMP_DEFLECT_SPEED;
        vx = 0;
        ax = 0;
        break;

    case MARIO_STATE_FALL_OFF:
        vx = 0;
        ax = 0;
        break;
    }

    CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    if (level == MARIO_LEVEL_BIG)
    {
        if (isSitting)
        {
            left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
            top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
            right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
            bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
        }
        else
        {
            left = x - MARIO_BIG_BBOX_WIDTH / 2;
            top = y - MARIO_BIG_BBOX_HEIGHT / 2;
            right = left + MARIO_BIG_BBOX_WIDTH;
            bottom = top + MARIO_BIG_BBOX_HEIGHT;
        }
    }
    if (level == MARIO_LEVEL_SMALL)
    {
        left = x - MARIO_SMALL_BBOX_WIDTH / 2;
        top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
        right = left + MARIO_SMALL_BBOX_WIDTH;
        bottom = top + MARIO_SMALL_BBOX_HEIGHT;
    }
    if (level == MARIO_LEVEL_RACCOON)
    {
        if (isSitting)
        {
            // shift the bbox left to the same direction of where mario is facing a bit so the tail has nothing to do with mario
            left = x - MARIO_RACCOON_SITTING_BBOX_WIDTH / 2 + 4 * nx;
            top = y - MARIO_RACCOON_SITTING_BBOX_HEIGHT / 2;
            right = left + MARIO_RACCOON_SITTING_BBOX_WIDTH;
            bottom = top + MARIO_RACCOON_SITTING_BBOX_HEIGHT;
        }
        else
        {
            // shift the bbox left to the same direction of where mario is facing a bit so the tail has nothing to do with mario
            left = x - MARIO_RACCOON_BBOX_WIDTH / 2 + 4 * nx;
            top = y - MARIO_RACCOON_BBOX_HEIGHT / 2;
            right = left + MARIO_RACCOON_BBOX_WIDTH;
            bottom = top + MARIO_RACCOON_BBOX_HEIGHT;
        }
    }
}

void CMario::LevelDown()
{
    switch (level) 
    {
    case MARIO_LEVEL_RACCOON:
        SetLevel(MARIO_LEVEL_BIG);
        StartUntouchable();
        break;
    case MARIO_LEVEL_BIG:
        SetLevel(MARIO_LEVEL_SMALL);
        StartUntouchable();
        break;
    default:
        DebugOut(L">>> Mario DIE >>> \n");
        SetState(MARIO_STATE_DIE);
    }
}

void CMario::SetLevel(int l)
{
    // Adjust position to avoid falling off platform
    if (this->level == MARIO_LEVEL_SMALL)
    {
        y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
    }
    if (this->level == MARIO_LEVEL_BIG)
    {
        y -= (MARIO_RACCOON_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT) / 2;
    }
    level = l;
    if (level == MARIO_LEVEL_RACCOON)
    {
        CPlayScene* current_scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
        this->tail = new CMarioTail(x - MARIO_TAIL_BBOX_WIDTH / 2 * nx, y + 6);
        current_scene->PushObject(this->tail);
    }
    else
    {
        // Clear the tail if Mario's level is set from Raccoon to Big
        if (tail != NULL)
            if (!tail->IsDeleted())
                tail->Delete();
            else return;
    }
}
