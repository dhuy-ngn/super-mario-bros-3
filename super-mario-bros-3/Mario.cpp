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
#include "Point.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    vy += ay * dt;
    vx += ax * dt;

    if (abs(vx) > abs(maxVx)) vx = maxVx;
    
    if (abs(vy) > abs(maxVy)) vy = maxVy;

    if (isOnPlatform) score_stack = 0;

    if (abs(vx) == MARIO_RUNNING_SPEED)
    {
        isRunning = true;
    }

    // reset untouchable timer if untouchable time has passed
    if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
    {
        untouchable_start = 0;
        untouchable = 0;
    }

    if (GetTickCount64() - running_start >= MARIO_RUNNING_STACK_DURATION && !isFlying && !isLanding && isRunning)
    {
        running_start = GetTickCount64();
        speed_stack++;
        if (speed_stack >= MARIO_MAX_RUNNING_STACK) 
        {
            speed_stack = MARIO_MAX_RUNNING_STACK;
            canFly = true;
        }
    }
    if (GetTickCount64() - running_start > MARIO_RUNNING_STACK_DURATION && vx == 0)
    {
        canFly = false;
        isRunning = false;
        running_start = GetTickCount64();
        speed_stack--;
        if (speed_stack < 0)
        {
            speed_stack = 0;
        }
    }
    if (isAttacking)
    {
        if (GetTickCount64() - attack_stack_start >= MARIO_MAX_ATTACK_STACK_TIME) 
        {
            attack_stack_start = GetTickCount64();
            attack_ani_stack++;
        }
        if (GetTickCount64() - attack_start > MARIO_MAX_ATTACK_STACK_TIME * MARIO_MAX_ATTACK_STACK) 
        {
            isAttacking = false;
            attack_stack_start = 0;
            attack_start = 0;
            attack_ani_stack = 0;
        }
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
        {
            isOnPlatform = true;
        }
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
    else if (dynamic_cast<CBrick*>(e->obj))
        OnCollisionWithBrick(e);
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

            AddScore(x, y - GetMarioHeight() / 2);
        }
    }
    else // hit by Goomba
    {
        if (isAttacking)
            goomba->SetState(GOOMBA_STATE_KNOCKED_OUT);
        else
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
        AddScore(x, y - GetMarioHeight() / 2);
        if (koopa->IsHiding())
        {
            if (isAttacking)
                koopa->SetState(KOOPA_STATE_KNOCKED_OUT);
            else
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
        isOnPlatform = false;
    }
    else // hit by Koopa
    {
        if (koopa->IsHiding())
            if (canHoldKoopaShell)
            {
                koopa->SetState(KOOPA_STATE_HELD_BY_MARIO);
                isHoldingKoopaShell = true;
            }
            else
                if (ax >= 0)
                {
                    koopa->SetState(KOOPA_STATE_SPINNING_RIGHT);
                }
                else
                    koopa->SetState(KOOPA_STATE_SPINNING_LEFT);
        else
        {
            if (isAttacking)
                koopa->SetState(KOOPA_STATE_KNOCKED_OUT);
            else
                if (untouchable == 0)
                    LevelDown();
        }
    }
}

void CMario::OnCollisionWithBrick(LPCOLLISIONEVENT e)
{
    CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
    CBrick* brick = dynamic_cast<CBrick*>(e->obj);
    float bx, by;
    brick->GetPosition(bx, by);
    if (e->ny > 0)
    {
        if (brick->GetItemType() == BRICK_ITEM_TYPE_MUSHROOM && brick->GetState()!= BRICK_STATE_EXHAUSTED)
        {
            brick->SetState(BRICK_STATE_EXHAUSTED);
            CMushroom* mushroom = new CMushroom(bx, by, MUSHROOM_TYPE_GREEN);
            current_scene->UnshiftObject(mushroom);
            mushroom->SetState(MUSHROOM_STATE_IDLE);
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

    if (e->ny < 0)
        isOnPlatform = true;

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
    CMushroom* mushroom = dynamic_cast<CMushroom*>(e->obj);
    if (mushroom->GetType() == MUSHROOM_TYPE_RED)
        if (level == MARIO_LEVEL_SMALL)
            SetLevel(MARIO_LEVEL_BIG);
        else
            AddScore1000(x, y- GetMarioHeight() / 2);
    else
        AddScore1Up(x, y - GetMarioHeight() / 2);
    e->obj->Delete();
}

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
    SetLevel(MARIO_LEVEL_RACCOON);
    if (level == MARIO_LEVEL_RACCOON)
    {
        AddScore1000(x, y - MARIO_RACCOON_BBOX_HEIGHT / 2);
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

    if (isAttacking && attack_ani_stack != 0)
    {
        if (nx > 0)
        {
            if (attack_ani_stack % 4 == 1) CSprites::GetInstance()->Get(ID_SPRITE_MARIO_WHACK_RIGHT_1)->Draw(x, y);
            if (attack_ani_stack % 4 == 2) CSprites::GetInstance()->Get(ID_SPRITE_MARIO_WHACK_RIGHT_2)->Draw(x, y);
            if (attack_ani_stack == 3) CSprites::GetInstance()->Get(ID_SPRITE_MARIO_WHACK_RIGHT_3)->Draw(x, y);
            if (attack_ani_stack == 4) CSprites::GetInstance()->Get(ID_SPRITE_MARIO_WHACK_RIGHT_4)->Draw(x, y);
        }
        else
        {
            if (attack_ani_stack % 4 == 1) CSprites::GetInstance()->Get(ID_SPRITE_MARIO_WHACK_LEFT_1)->Draw(x, y);
            if (attack_ani_stack % 4 == 2) CSprites::GetInstance()->Get(ID_SPRITE_MARIO_WHACK_LEFT_2)->Draw(x, y);
            if (attack_ani_stack == 3) CSprites::GetInstance()->Get(ID_SPRITE_MARIO_WHACK_LEFT_3)->Draw(x, y);
            if (attack_ani_stack == 4) CSprites::GetInstance()->Get(ID_SPRITE_MARIO_WHACK_LEFT_4)->Draw(x, y);
        }
    }
    else
    {
        if (state == MARIO_STATE_DIE)
            aniId = ID_ANI_MARIO_DIE;
        else if (level == MARIO_LEVEL_BIG)
            aniId = GetAniIdBig();
        else if (level == MARIO_LEVEL_SMALL)
            aniId = GetAniIdSmall();
        else if (level == MARIO_LEVEL_RACCOON)
            aniId = GetAniIdRaccoon();

        animations->Get(aniId)->Render(x, y);
    }
    // RenderBoundingBox();

    DebugOutTitle(L"Mario attack stack: %d", attack_ani_stack);
}

void CMario::SetState(int state)
{
    // DIE is the end state, cannot be changed! 
    if (this->state == MARIO_STATE_DIE || this->state == MARIO_STATE_FALL_OFF) return;

    switch (state)
    {
    case MARIO_STATE_RUNNING_RIGHT:
        if (isSitting) break;
        if (isAttacking) break;
        isRunning = true;
        maxVx = MARIO_RUNNING_SPEED;
        ax = MARIO_ACCEL_RUN_X;
        nx = 1;
        break;
    case MARIO_STATE_RUNNING_LEFT:
        if (isSitting) break;
        if (isAttacking) break;
        isRunning = true;
        maxVx = -MARIO_RUNNING_SPEED;
        ax = -MARIO_ACCEL_RUN_X;
        nx = -1;
        break;
    case MARIO_STATE_WALKING_RIGHT:
        if (isSitting) break;
        isRunning = false;
        maxVx = MARIO_WALKING_SPEED;
        ax = MARIO_ACCEL_WALK_X;
        nx = 1;
        break;
    case MARIO_STATE_WALKING_LEFT:
        if (isSitting) break;
        isRunning = false;
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
        falling_start = GetTickCount64();
        canLand = false;
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
        if (level != MARIO_LEVEL_RACCOON) break;
        isAttacking = true;
        vx = 0;
        StartAttacking();
        break;

    case MARIO_STATE_ATTACK_RELEASE:
        if (!isOnPlatform)  break;
        if (isSitting) break;
        isRunning = false;
        isAttacking = false;
        attack_ani_stack = 0;
        state = MARIO_STATE_IDLE;
        break;

    case MARIO_STATE_HOLD_KOOPA_SHELL:
        if (isSitting) break;
        isRunning = false;
        ax = 0.0f;
        vx = 0.0f;
        isHoldingKoopaShell = true;
        isFlying = false;
        isLanding = false;
        isAttacking = false;
        break;

    case MARIO_STATE_HOLD_WALK_KOOPA_SHELL_RIGHT:
        isRunning = false;
        isHoldingKoopaShell = true;
        maxVx = MARIO_WALKING_SPEED;
        ax = MARIO_ACCEL_WALK_X;
        nx = 1;
        isHoldingKoopaShell = true;
        break;

    case MARIO_STATE_HOLD_WALK_KOOPA_SHELL_LEFT:
        isRunning = false;
        isHoldingKoopaShell = true;
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
        attack_start = 0;
        attack_stack_start = 0;
        falling_start = 0;
        isRunning = false;
        isFlying = false;
        isLanding = false;
        isAttacking = false;
        isHoldingKoopaShell = false;
        break;

    case MARIO_STATE_DIE:
        vy = -MARIO_JUMP_DEFLECT_SPEED;
        isRunning = false;
        vx = 0;
        ax = 0;
        break;

    case MARIO_STATE_FALL_OFF:
        isRunning = false;
        vx = 0;
        ax = 0;
        break;
    }

    CGameObject::SetState(state);
}

void CMario::AddScore(float x, float y)
{
    CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

    CPoint* point = new CPoint(this->score_stack);

    point->SetPosition(x, y);
    currentScene->PushObject(point);

    GainScore();

    score_stack++;
}

void CMario::AddScore1000(float x, float y)
{
    CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

    CPoint* point = new CPoint(SCORE_STACK_1000);

    point->SetPosition(x, y);
    currentScene->PushObject(point);

    Gain1000Score();
}

void CMario::AddScore1Up(float x, float y)
{
    CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

    CPoint* point = new CPoint(-1);

    point->SetPosition(x, y);
    currentScene->PushObject(point);
}

int CMario::GetMarioHeight()
{
    switch (level)
    {
    case MARIO_LEVEL_SMALL:
        return MARIO_SMALL_BBOX_HEIGHT;
        break;
    case MARIO_LEVEL_BIG:
        return MARIO_BIG_BBOX_HEIGHT;
        break;
    case MARIO_LEVEL_RACCOON:
        return MARIO_RACCOON_BBOX_HEIGHT;
        break;
    }
    return 0;
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

void CMario::GainScore()
{
    switch (score_stack)
    {
    case SCORE_STACK_100:
        score += 100;
        break;
    case SCORE_STACK_200:
        score += 200;
        break;
    case SCORE_STACK_400:
        score += 400;
        break;
    case SCORE_STACK_800:
        score += 800;
        break;
    case SCORE_STACK_1000:
        score += 1000;
        break;
    case SCORE_STACK_2000:
        score += 2000;
        break;
    case SCORE_STACK_4000:
        score += 4000;
        break;
    case SCORE_STACK_8000:
        score += 8000;
        break;
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
        if (tail != nullptr)
            if (!tail->IsDeleted())
            tail->Delete();
        else return;
    }
}
