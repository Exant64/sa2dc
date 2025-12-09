#include <shinobi.h>

#include <task.h>
#include <c_colli.h>
#include <move.h>
#include <shadow.h>
#include <player.h>

#include <Chao/al_texlist.h>
#include <Chao/al_world.h>
#include <Chao/al_colli_kind.h>
#include <Chao/al_control.h>
#include <Chao/al_global.h>

extern NJS_CNK_MODEL model_kage_marukage_marukage[];

typedef struct BALL_WORK {
    float inv_radius;
    int HeldAng;
    int RotSpd;
    NJS_POINT3 axis;
    NJS_MATRIX mat;
} BALL_WORK;

enum {
    CI_ID_BODY1 = 0,
    CI_ID_BODY2 = 1,
    CI_ID_SWITCH = 2,
};

#define GET_MOVE_WK(tp) ((MOVE_WORK*)tp->Data2)
#define GET_BALL_WK(tp) ((BALL_WORK*)tp->UnknownA_ptr)

extern task* pBallTask; // C682CB0;
extern int BallFlag; // lbl_0C682CB4
extern int BallWaterFlag; // 0C682CB8

static const CCL_INFO colli[] = {
    CCL_CYLINDER(0, 0x70, 0, 0x2400, 0, 1.5f, 0, 15.f, 3.f, 0),
    CCL_SPHERE(0, 6, 0xC, 0, 0, 0, 0, 2.0, 0, 0),
    CCL_CYLINDER(CI_KIND_AL_BALL, 0x70, 0xC, 0, 0, -2.0f, 0, 4.2f, 5.f, 0),
};

// can't make static cuz it's an unused function and it would get unlinked
// because of this, the rest are most likely not static either, but eh
void lbl_0C53D1A0(task* tp) {
    taskwk* work = tp->twp;
    MOVE_WORK* move = GET_MOVE_WK(tp);

    if(move->Timer++ > 3) {
        Angle3 rot;
        
        move->Timer = 0;
        move->BottomY = GetShadowPos(work->pos.x, work->pos.y + 4.f, work->pos.z, &rot);
    }

    work->flag &= ~1;

    if(work->pos.y < move->BottomY) {
        if(move->Velo.y < 0) move->Velo.y *= -0.5f;
        
        move->Velo.x *= 0.85f;
        move->Velo.z *= 0.85f;

        work->pos.y = move->BottomY;
        work->flag |= 1;
    }
}

static void lbl_0C53D238(task* tp) {
    taskwk* work = tp->twp;
    MOVE_WORK* movewk = GET_MOVE_WK(tp);
    colliwk* cwp = work->cwp;
    BALL_WORK* ballwk = GET_BALL_WK(tp);

    switch(work->smode) {
        case 0:
            CCL_Enable(tp, CI_ID_BODY1);
            CCL_Enable(tp, CI_ID_BODY2);
            CCL_Disable(tp, CI_ID_SWITCH);
            work->smode++;
            break;
    }

    if(work->flag & 0x8000) {
        work->mode = 2;
        work->smode = 0;
        return;
    }
    
    if(CCL_IsHit(tp)) {
        work->mode = 1;
        work->smode = 0;
    }
}

static void lbl_0C53D2C0(task* tp) {
    taskwk* work = tp->twp;
    MOVE_WORK* movewk = GET_MOVE_WK(tp);
    colliwk* cwp = work->cwp;
    BALL_WORK* ballwk = GET_BALL_WK(tp);

    if(AL_IsMovable()) {
        
    }

    switch(work->smode) {
        case 0:
            CCL_Enable(tp, CI_ID_BODY1);
            CCL_Enable(tp, CI_ID_BODY2);
            CCL_Disable(tp, CI_ID_SWITCH);
            work->smode++;
            break;
    }

    if(CCL_IsPushed(tp)) {
        NJS_VECTOR diff;
        NJS_VECTOR direct;
        float scalor;
        float inner1, inner2;
        
        diff.x = work->pos.x - movewk->PrePos.x;
        diff.y = work->pos.y - movewk->PrePos.y;
        diff.z = work->pos.z - movewk->PrePos.z;

        direct = diff;

        scalor = njScalor(&direct);
        if(scalor >= 0.01f) {
            njUnitVector(&direct);
        }

        inner1 = njInnerProduct(&direct, &movewk->Velo);
        inner2 = njInnerProduct(&direct, &diff);

        if(inner2 > inner1) {
            float x = 1.1f * (inner2 - inner1);
            
            if(AL_IsHitKindWithNum(tp, 1, CI_KIND_AL_CHAO)) {
                x *= 2.9f;
                movewk->Velo.y += 0.28f;
            }
            
            if(x > 1) x = 1;

            movewk->Velo.x += direct.x * x;
            movewk->Velo.y += direct.y * x;
            movewk->Velo.z += direct.z * x;
        }
    }

    if ( work->pos.y < movewk->WaterY + 1.5f ) {
        if ( work->pos.y > movewk->WaterY - 1.5 ) {
            movewk->Acc.y += -movewk->Gravity * (movewk->WaterY - work->pos.y + 1.5f) * 0.66666666f;
        }
        else {
            movewk->Acc.y += -2.f * movewk->Gravity;
        }
        
        movewk->Velo.x *= 0.975f;
        movewk->Velo.y *= 0.93f;
        movewk->Velo.z *= 0.975f;
        
        ballwk->RotSpd = NJM_DEG_ANG(NJM_ANG_DEG(ballwk->RotSpd * 0.96f));
        BallWaterFlag = 1;
    }

    MOV_Control(tp);
    MOV_PreservePreviousPosition(tp);
    MOV_DetectCollision(tp);

    if(work->flag & 1) {
        float scalor;
        
        movewk->Velo.x *= 0.99f;
        movewk->Velo.z *= 0.99f;

        scalor = njScalor(&movewk->Velo);
        if(scalor > 0.0001) {
            {
                const NJS_VECTOR normal = {0, 1, 0};
                
                njOuterProduct(&normal, &movewk->Velo, &ballwk->axis);
                
                if(njScalor(&ballwk->axis) > 0) {
                    njUnitVector(&ballwk->axis);
                }
                else {
                    ballwk->axis.x = 0;
                    ballwk->axis.y = 1;
                    ballwk->axis.z = 0;
                }
            }
            
            {
                NJS_VECTOR normal = {0, 1, 0};
                NJS_VECTOR velo;
                float scalor;
                float inner = njInnerProduct(&normal, &movewk->Velo);
                
                normal.x *= inner;
                normal.y *= inner;
                normal.z *= inner;

                velo.x = movewk->Velo.x - normal.x;
                velo.y = movewk->Velo.y - normal.y;
                velo.z = movewk->Velo.z - normal.z;

                scalor = njScalor(&velo);
                ballwk->RotSpd = NJM_RAD_ANG(scalor * ballwk->inv_radius);
            }
        }
        else {
            ballwk->RotSpd = 0;
        }
    }

    if(ballwk->RotSpd) {
        NJS_MATRIX tmp_mat;

        njUnitMatrix(&tmp_mat);
        njRotate(&tmp_mat, &ballwk->axis, ballwk->RotSpd);
        njMultiMatrix(&tmp_mat, &ballwk->mat);

        njSetMatrix(&ballwk->mat, &tmp_mat);
    }

    if(work->flag & 0x8000) {
        ballwk->RotSpd = 0;
        work->mode = 2;
        work->smode = 0;
    }
}

static void lbl_0C53D660(task* tp) {
    taskwk* work = tp->twp;
    MOVE_WORK* movewk = GET_MOVE_WK(tp);
    
    switch(work->smode) {
        case 0:
            CCL_Disable(tp, CI_ID_BODY1);
            CCL_Disable(tp, CI_ID_BODY2);
            CCL_Enable(tp, CI_ID_SWITCH);

            movewk->AimAng.y = work->ang.y + playertwp[0]->ang.y;

            ALW_CommunicationOff(tp);
            
            work->smode++;
            break;
    }
    
    work->ang.y = -playertwp[0]->ang.y + movewk->AimAng.y;
    
    if(!(work->flag & 0x8000)) {
        movewk->Velo.x *= 0.4f;
        movewk->Velo.y *= 0.7f;
        movewk->Velo.z *= 0.4f;
    
        MOV_ClearAcc(tp);
        MOV_PreservePreviousPosition(tp);
    
        work->mode = 1;
        work->smode = 0;
    }
}

static void lbl_0C53D6F8(task *tp) {
    taskwk* work = tp->twp;
    MOVE_WORK* movewk = GET_MOVE_WK(tp);

    Uint32 command = ALW_RecieveCommand(tp);
    
    switch(work->smode) {
        case 0:
            CCL_Disable(tp, CI_ID_BODY1);
            CCL_Disable(tp, CI_ID_BODY2);
            CCL_Disable(tp, CI_ID_SWITCH);

            work->smode++;
            work->wtimer = 0;
            
            break;
    }

    switch (command) {
        case 2:
            if(work->wtimer++ > 180) {
                work->wtimer = 0;
                
                --work->id;
                if(work->id == 0) {
                    ALW_SendCommand(tp, 4);
                    FreeTask(tp);
                }
            }
            break;
        case 3:
            work->mode = 1;
            work->smode = 0;     
            break;
    }

    if(!AL_TraceHoldingPosition(tp)){
        work->mode = 1;
        work->smode = 0;
    }
}

static void ALO_BallExecutor(task* tp) {
    taskwk* work = tp->twp;
    MOVE_WORK* movewk = GET_MOVE_WK(tp);

    BallWaterFlag = 0;

    movewk->Flag |= 4;

    switch(work->mode) {
        case 0:
            lbl_0C53D238(tp);
            break;
        case 1:
            lbl_0C53D2C0(tp);
            break;
        case 2:
            lbl_0C53D660(tp);
            break;
        case 3:
            lbl_0C53D6F8(tp);
            break;
    }

    if(work->mode == 1) {
        if(njScalor(&movewk->Velo) > 0.4f) {
            BallFlag = 1;
        }
        else {
            BallFlag = 0;
        }
    }
    else {
        BallFlag = 0;
    }

    CCL_Entry(tp);
}

static void ALO_BallDisplayer(task* tp) {
    taskwk* work = tp->twp;
    BALL_WORK* ball = GET_BALL_WK(tp);

    if(AL_IsOnScreen2(tp, 2.f, 0.f)) {
        njPushMatrix(NULL);
        njTranslateV(NULL, &work->pos);

        if(work->flag & 0x8000) {
            njTranslate(NULL, 0, 0.4f, 0);
        }

        njPushMatrixEx();
        if(AL_IsHitKindWithNum(tp, 1, CI_KIND_AL_SHADOW)) {
            OnControl3D(0x2400);
        }

        njMultiMatrix(NULL, ball->mat);
        
        switch(AL_GetStageNumber()){
            case 1:
                njCnkEasyDrawObject((NJS_CNK_OBJECT*)0xC632FC8);
                break;
            case 2:
                njSetTexture(&AL_TOY_TEXLIST);
                njCnkEasyDrawObject((NJS_CNK_OBJECT*)0xC6336FC);
                break;
            case 3:
                njSetTexture(&AL_TOY_TEXLIST);
                njCnkEasyDrawObject((NJS_CNK_OBJECT*)0xC633DB4);
                break;
        }
        
        OffControl3D(0x2400);
        njPopMatrixEx();

        if (ALW_ENTRY_WORK(tp)->CamDist < GET_GLOBAL()->CamDistShadowCutLev2) {
            if(AL_IsHitKindWithNum(tp, 1, CI_KIND_AL_SHADOW)) {
                njTranslate(NULL, 0, -1.45f, 0);
            }
            else {
                njTranslate(NULL, 0, -1, 0);
            }
            
            njScale(NULL, 1.8f, 0.7f, 1.8f);
            njCnkModDrawModel(model_kage_marukage_marukage);
        }

        njPopMatrix(1);
    }
}

static void ALO_BallDestroyer(task* tp){
    ALW_CancelEntry(tp);
}

static void ALO_Ball(task* tp) {
    taskwk* work = tp->twp;

    CCL_Init(tp, colli, sizeof(colli) / sizeof(colli[0]), 5);
    ObjectMovableInitialize(work, GET_MOVE_WK(tp), 10);

    tp->exec = ALO_BallExecutor;
    tp->dest = ALO_BallDestroyer;
    tp->disp = ALO_BallDisplayer;

    work->mode = 1;
    work->smode = 0;

    ALW_Entry(ALW_CATEGORY_TOY, tp, ALW_KIND_BALL);

    pBallTask = tp;
}

void ALO_BallCreate(NJS_POINT3* pPos, NJS_VECTOR* pVelo) {
    task* tp = CreateElementalTask(2, 2, ALO_Ball, "ALO_Ball");
    taskwk* work = tp->twp;
    BALL_WORK* ball;
    MOVE_WORK* move;
    
    tp->UnknownA_ptr = syCalloc(1, sizeof(BALL_WORK));

    ball = GET_BALL_WK(tp);

    ball->inv_radius = 0.66666666f;
    ball->HeldAng = 0;
    ball->RotSpd = 0;
    
    ball->axis.x = 0;
    ball->axis.y = 1;
    ball->axis.z = 0;
    
    njUnitMatrix(&ball->mat);
    
    move = MOV_Init(tp);

    work->pos = *pPos;
    move->Velo = *pVelo;

    MOV_SetGravity(tp, -0.05f);
    
    move->Top = 1.5f;
    move->Bottom = -2.5f;
    move->BoundFloor = 0.85f;
    move->Offset.y = 1.f;
    move->BoundFriction = 0.99f;

    if(GetCharID(0) != 3) move->rad = 3328;
    else move->rad = -4864;
    
    MOV_PreservePreviousPosition(tp);
    BallWaterFlag = 0;
}