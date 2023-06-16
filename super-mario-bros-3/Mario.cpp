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
        if (e->ny < 0) isOnPlatform = true;
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
        }
    }
    else // hit by Goomba
    {
        if (untouchable == 0)
        {
            if (goomba->GetState() != GOOMBA_STATE_DIE && goomba->GetState() != GOOMBA_STATE_KNOCKED_OUT)
            {
                switch (level) {
                case MARIO_LEVEL_RACCOON:
                    level = MARIO_LEVEL_BIG;
                    StartUntouchable();
                    break;
                case MARIO_LEVEL_BIG:
                    level = MARIO_LEVEL_SMALL;
                    StartUntouchable();
                    break;
                default:
                    DebugOut(L">>> Mario DIE >>> \n");
                    SetState(MARIO_STATE_DIE);
                }
            }
        }
    }
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
    CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);

    // jump on top
    if (e->ny < 0)
    {
        if (koopa->GetState() == KOOPA_STATE_HIDING)
        {
            if (nx >= 0)
                koopa->SetState(KOOPA_STATE_SPINNING_RIGHT);
            else
                koopa->SetState(KOOPA_STATE_SPINNING_LEFT);
        }
        else
            koopa->SetState(KOOPA_STATE_HIDING);
        vy = -MARIO_JUMP_DEFLECT_SPEED;
    }
    else // hit by Koopa
    {
        if (koopa->GetState() == KOOPA_STATE_HIDING && koopa->GetState() != KOOPA_STATE_KNOCKED_OUT)
            if (ax >= 0)
                koopa->SetState(KOOPA_STATE_SPINNING_RIGHT);
            else
                koopa->SetState(KOOPA_STATE_SPINNING_LEFT);
        else {
            if (!untouchable)
            {
                switch (level) {
                case MARIO_LEVEL_RACCOON:
                    level = MARIO_LEVEL_BIG;
                    StartUntouchable();
                    break;
                case MARIO_LEVEL_BIG:
                    level = MARIO_LEVEL_SMALL;
                    StartUntouchable();
                    break;
                default:
                    DebugOut(L">>> Mario DIE >>> \n");
                    SetState(MARIO_STATE_DIE);
                }
            }
        }
    }

    // TODO: Mario holding Koopa's shell
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
    e->obj->Delete();
    coin++;
}

void CMario::OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e)
{
    CQuestionBlock* question_block = dynamic_cast<CQuestionBlock*>(e->obj);

    if (question_block->GetState() != QUESTION_BLOCK_STATE_INACTIVE && e->ny > 0)
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
        {
            switch (level) {
            case MARIO_LEVEL_RACCOON:
                level = MARIO_LEVEL_BIG;
                StartUntouchable();
                break;
            case MARIO_LEVEL_BIG:
                level = MARIO_LEVEL_SMALL;
                StartUntouchable();
                break;
            default:
                DebugOut(L">>> Mario DIE >>> \n");
                SetState(MARIO_STATE_DIE);
            }
        }
    }
}

void CMario::OnCollisionWithFireBullet(LPCOLLISIONEVENT e)
{
    if (untouchable == 0)
    {
        switch (level) {
        case MARIO_LEVEL_RACCOON:
            level = MARIO_LEVEL_BIG;
            StartUntouchable();
            break;
        case MARIO_LEVEL_BIG:
            level = MARIO_LEVEL_SMALL;
            StartUntouchable();
            break;
        default:
            DebugOut(L">>> Mario DIE >>> \n");
            SetState(MARIO_STATE_DIE);
        }
    }
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
        e->obj->Delete();
    }
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
    if (!isOnPlatform || vy < 0)
    {
        if (isSpinning) {
            aniId = ID_ANI_MARIO_SMALL_SPIN;
        }
        else
            if (abs(vx) == MARIO_RUNNING_SPEED)
            {
                if (nx >= 0)
                    aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
                else
                    aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
            }
            else
            {
                if (nx >= 0)
                    aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
                else
                    aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
            }
    }
    else
        if (isSpinning) {
            aniId = ID_ANI_MARIO_SMALL_SPIN;
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
                    if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
                    else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
                }
                else if (vx > 0)
                {
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
                    if (ax > 0)
                        aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
                    else if (ax == -MARIO_ACCEL_RUN_X)
                        if (vx == -MARIO_RUNNING_SPEED)
                            aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
                        else
                            aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
                    else if (ax == -MARIO_ACCEL_WALK_X)
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
    else if (isSitting)
    {
        if (nx > 0)
            aniId = ID_ANI_MARIO_SIT_RIGHT;
        else
            aniId = ID_ANI_MARIO_SIT_LEFT;
    }
    else
        if (vx == 0)
        {
            if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
            else aniId = ID_ANI_MARIO_IDLE_LEFT;
        }
        else if (vx > 0)
        {
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
            if (ax > 0)
                aniId = ID_ANI_MARIO_BRACE_LEFT;
            else if (ax == -MARIO_ACCEL_RUN_X)
                if (vx == -MARIO_RUNNING_SPEED)
                    aniId = ID_ANI_MARIO_RUNNING_LEFT;
                else
                    aniId = ID_ANI_MARIO_WALKING_LEFT;
            else if (ax == -MARIO_ACCEL_WALK_X)
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
        if (isSpinning) {
            aniId = ID_ANI_MARIO_RACCOON_SPIN;
        }
        else
            if (vy >= 0 && !isFlying)
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
                if (isFlying)
                    if (nx >= 0)
                        aniId = ID_ANI_MARIO_RACCOON_FLYING_RIGHT;
                    else
                        aniId = ID_ANI_MARIO_RACCOON_FLYING_LEFT;
                else
                    if (nx >= 0)
                        aniId = ID_ANI_MARIO_RACCOON_LANDING_RIGHT;
                    else
                        aniId = ID_ANI_MARIO_RACCOON_LANDING_LEFT;

    }
    else
        if (isSpinning) {
            aniId = ID_ANI_MARIO_RACCOON_SPIN;
        }
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
                    if (nx > 0) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;
                    else aniId = ID_ANI_MARIO_RACCOON_IDLE_LEFT;
                }
                else if (vx > 0)
                {
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

    RenderBoundingBox();

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
        if (isSpinning) break;
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
        if (isSpinning) break;
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
        isFlying = true;
        vy = MARIO_LANDING_SPEED;
        ax = 0.0f;
        vx = 0.0f;
        break;

    case MARIO_STATE_LAND_RIGHT:
        if (isSitting) break;
        if (isOnPlatform) break;
        if (level != MARIO_LEVEL_RACCOON) break;
        isFlying = true;
        vy = MARIO_LANDING_SPEED;
        maxVx = MARIO_FLYING_SPEED;
        ax = MARIO_ACCEL_FLYING_X;
        nx = 1;
        break;

    case MARIO_STATE_LAND_LEFT:
        if (isSitting) break;
        if (isOnPlatform) break;
        if (level != MARIO_LEVEL_RACCOON) break;
        isFlying = true;
        vy = MARIO_LANDING_SPEED;
        maxVx = -MARIO_FLYING_SPEED;
        ax = -MARIO_ACCEL_FLYING_X;
        nx = -1;
        break;

        // RACCOON MARIO LANDING
    //case MARIO_STATE_LAND:
    //    if (isSitting) break;
    //    if (level != MARIO_LEVEL_RACCOON) break;
    //    ay = MARIO_RACCOON_GRAVITY;
    //    break;

    case MARIO_STATE_FLY:
        if (isSitting) break;
        if (isOnPlatform) break;
        if (level != MARIO_LEVEL_RACCOON) break;
        isFlying = true;
        ay = -MARIO_ACCEL_FLYING_Y;
        maxVy = -MARIO_FLYING_SPEED;
        break;

    case MARIO_STATE_FLY_LEFT:
        if (isSitting) break;
        if (isOnPlatform) break;
        if (level != MARIO_LEVEL_RACCOON) break;
        isFlying = true;
        ay = -MARIO_ACCEL_FLYING_Y;
        maxVy = -MARIO_FLYING_SPEED;
        nx = -1;
        break;

    case MARIO_STATE_FLY_RIGHT:
        if (isSitting) break;
        if (isOnPlatform) break;
        if (level != MARIO_LEVEL_RACCOON) break;
        isFlying = true;
        ay = -MARIO_ACCEL_FLYING_Y;
        maxVy = -MARIO_FLYING_SPEED;
        nx = 1;
        break;

        // IDLE
    case MARIO_STATE_IDLE:
        ax = 0.0f;
        vx = 0.0f;
        isSpinning = false;
        isFlying = false;
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
            left = x - MARIO_RACCOON_SITTING_BBOX_WIDTH / 2 + 6 * nx; // shift the bbox left to the right a bit so the tail has nothing to do with mario
            top = y - MARIO_RACCOON_SITTING_BBOX_HEIGHT / 2;
            right = left + MARIO_RACCOON_SITTING_BBOX_WIDTH;
            bottom = top + MARIO_RACCOON_SITTING_BBOX_HEIGHT;
        }
        else
        {
            left = x - MARIO_RACCOON_BBOX_WIDTH / 2 + 6 * nx; // shift the bbox left to the right a bit so the tail has nothing to do with mario
            top = y - MARIO_RACCOON_BBOX_HEIGHT / 2;
            right = left + MARIO_RACCOON_BBOX_WIDTH;
            bottom = top + MARIO_RACCOON_BBOX_HEIGHT;
        }
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
}

