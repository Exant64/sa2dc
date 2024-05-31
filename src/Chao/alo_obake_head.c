#include <move.h>
#include <Chao/Chao.h>
#include <Chao/al_misc.h>
#include <player.h>

// the function names, and the enum is made up since we don't have sa2 symbols

enum { MD_INIT, MD_STOP, MD_MOVE, MD_HELD_P, MD_HELD_C };

void ALO_ObakeHeadStop(task* tp) {
    taskwk* work = tp->twp;

    switch (work->smode) {
        case 0:
            CCL_Enable(tp, 0);
            CCL_Enable(tp, 1);
            CCL_Disable(tp, 2);
            work->smode++;
            break;
    }

    if (work->flag & TWK_FLAG_HELD) {
        work->mode = MD_HELD_P;
        work->smode = 0;
        return;
    }

    if (CCL_IsPushed(tp)) {
        work->mode = MD_MOVE;
        work->smode = 0;
        work->wtimer = 0;
    }

    if (ALW_IsHeld(tp)) {
        work->mode = MD_HELD_C;
        work->smode = 0;
    }
}

void ALO_ObakeHeadMove(task* tp) {
    taskwk* work = tp->twp;
    MOVE_WORK* move = (MOVE_WORK*)tp->Data2;

    switch (work->smode) {
        case 0:
            CCL_Enable(tp, 0);
            CCL_Enable(tp, 1);
            CCL_Disable(tp, 2);
            work->smode++;
            break;
    }

    AL_CalcWaterAction(tp, 1.5f);

    if (work->flag & 1) {
        // this fixed a regswap between r11 and r12, also added this to alo_seed because im assuming these functions
        // were just copypasted
        MOVE_WORK* move2 = (MOVE_WORK*)tp->Data2;

        NJS_VECTOR velo;
        velo.x = move->Velo.x;
        velo.y = 0;
        velo.z = move->Velo.z;
        if (njScalor(&velo) < 0.01f) {
            if (work->wtimer++ > 10) {
                work->wtimer = 0;
                work->mode = MD_STOP;
                work->smode = 0;
            }
        }
    }

    if (work->flag & TWK_FLAG_HELD) {
        work->mode = MD_HELD_P;
        work->smode = 0;
    }

    if (ALW_IsHeld(tp)) {
        work->mode = MD_HELD_C;
        work->smode = 0;
    }

    MOV_Control(tp);
    MOV_DetectCollision(tp);
}

void ALO_ObakeHeadInit(task* tp) {
    taskwk* work = tp->twp;

    switch (work->smode) {
        case 0:
            CCL_Disable(tp, 0);
            CCL_Disable(tp, 1);
            CCL_Disable(tp, 2);
            work->smode++;
            break;
    }

    MOV_Control(tp);
    // MOV_DetectCollision(tp);

    if (work->wtimer++ > 10) {
        work->mode = 2;
        work->smode = 0;
    }
}

void ALO_ObakeHeadHeldP(task* tp) {
    taskwk* work = tp->twp;
    MOVE_WORK* move = (MOVE_WORK*)tp->Data2;

    switch (work->smode) {
        case 0:
            CCL_Disable(tp, 0);
            CCL_Disable(tp, 1);
            CCL_Enable(tp, 2);

            move->AimAng.y = work->ang.y + playertwp[0]->ang.y;
            ALW_CommunicationOff(tp);

            work->smode++;
            break;
    }

    work->ang.y = -playertwp[0]->ang.y + move->AimAng.y;
    if (!(work->flag & TWK_FLAG_HELD)) {
        if (ALW_IsHeld(tp)) {
            work->mode = MD_HELD_C;
            work->smode = 0;
        } else {
            work->mode = MD_MOVE;
            work->smode = 0;
            work->wtimer = 0;
        }
    }
}

void ALO_ObakeHeadHeldC(task* tp) {
    taskwk* work = tp->twp;

    ALW_RecieveCommand(tp);

    switch (work->smode) {
        case 0:
            CCL_Enable(tp, 0);
            CCL_Disable(tp, 1);
            CCL_Disable(tp, 2);

            work->smode++;
            work->wtimer = 0;
            break;
    }

    if (!AL_TraceHoldingPosition(tp)) {
        work->mode = MD_MOVE;
        work->smode = 0;
        work->wtimer = 0;
    }

    if (work->flag & TWK_FLAG_HELD) {
        ALW_CommunicationOff(tp);
        work->mode = MD_HELD_P;
        work->smode = 0;
    }
}

void ALO_ObakeHeadExecutor(task* tp) {
    taskwk* work = tp->twp;

    switch (work->mode) {
        case MD_STOP:
            ALO_ObakeHeadStop(tp);
            break;
        case MD_INIT:
            ALO_ObakeHeadInit(tp);
            break;
        case MD_MOVE:
            ALO_ObakeHeadMove(tp);
            break;
        case MD_HELD_P:
            ALO_ObakeHeadHeldP(tp);
            break;
        case MD_HELD_C:
            ALO_ObakeHeadHeldC(tp);
            break;
    }

    MOV_PreservePreviousPosition(tp);
    CCL_Entry(tp);
}

extern NJS_CNK_OBJECT object_alm_egg_eggtop[];
extern NJS_CNK_MODEL model_kage_marukage_marukage[];
void ALO_ObakeHeadDisplayer(task* tp) {
    taskwk* work = tp->twp;

    if (AL_IsOnScreen2(tp, 2.5f, 2.0f)) {
        njSetTexture(&AL_BODY_TEXLIST);
        njPushMatrix(NULL);

        njTranslate(NULL, work->pos.x, work->pos.y, work->pos.z);
        if (work->flag & 0x8000) {
            njTranslate(NULL, 0, -0.35f, 0);
        }

        njRotateY(NULL, work->ang.y);
        njPushMatrixEx();
        if (AL_IsHitKindWithNum(tp, 1, CI_KIND_AL_SHADOW)) {
            OnControl3D(0x2400);
        }

        switch (work->ang.x) {
            case 1:
                njCnkEasyDrawObject((NJS_CNK_OBJECT*)0xC8AA204);
                break;
            case 2:
                njCnkEasyDrawObject((NJS_CNK_OBJECT*)0xC8AB540);
                break;
            case 3:
                njSetTexture(&AL_OBJECT_TEXLIST);
                njTranslate(NULL, 0, 0.4f, 0);
                njCnkEasyDrawModel(object_alm_egg_eggtop[0].model);
                break;
        }

        OffControl3D(0x2400);
        njPopMatrixEx();

        if (ALW_ENTRY_WORK(tp)->CamDist < GET_GLOBAL()->CamDistShadowCutLev2) {
            if (AL_IsHitKindWithNum(tp, 1, CI_KIND_AL_SHADOW)) {
                njTranslate(NULL, 0, 0.1f, 0);
            } else {
                njTranslate(NULL, 0, 0.4f, 0);
            }

            njScale(NULL, 1.5f, 0.7f, 1.5f);
            njCnkModDrawModel(model_kage_marukage_marukage);
        }

        njPopMatrix(1);
    }
}

void ALO_ObakeHeadDestructor(task* tp) {
    ALW_CancelEntry(tp);
}

static const CCL_INFO colli_info[3] = {
    { 0, CI_FORM_CYLINDER, CI_PUSH_TH_CMN, 0, CI_ATTR_2000 | CI_ATTR_DAMAGE, { 0, 1.5, 0 }, 15, 3 },
    { 0, CI_FORM_SPHERE, CI_PUSH_PO_ENEMY | CI_PUSH_PO_OBJECT, CI_DMG_SET(0, 3), 0, { 0, 1.2, 0 }, 2 },
    { CI_KIND_AL_OBAKE_HEAD, CI_FORM_CYLINDER, CI_PUSH_TH_CMN, CI_DMG_SET(0, 3), 0, { 0, -2, 0 }, 4.2f, 5 }
};

void ALO_ObakeHead(task* tp) {
    taskwk* work = tp->twp;

    CCL_Init(tp, colli_info, 3, 5);
    ObjectMovableInitialize(work, tp->Data2, 10);

    tp->exec = ALO_ObakeHeadExecutor;
    tp->dest = ALO_ObakeHeadDestructor;
    tp->disp = ALO_ObakeHeadDisplayer;

    work->mode = MD_INIT;
    work->smode = 0;

    ALW_Entry(ALW_CATEGORY_MASK, tp, 0);
    ALW_SetHeldOffset(tp, 0.0f);
    ALW_SetHeldRadius(tp, 1.4f);
}

void ALO_ObakeHeadCreate(int kind, NJS_POINT3* pPos, int AngY, NJS_VECTOR* pVelo) {
    task* tp = CreateElementalTask(2, 4, ALO_ObakeHead, "ALO_ObakeHead");
    taskwk* work = tp->twp;
    MOVE_WORK* move = MOV_Init(tp);

    work->pos = *pPos;
    work->ang.y = AngY;

    if (pVelo) {
        move->Velo = *pVelo;
    }

    work->ang.x = kind;

    MOV_SetGravity(tp, -0.05f);
    move->Offset.y = 3;
    move->BoundFloor = 0.8f;
    move->BoundFriction = 0.7f;

    if (GetCharID(0) != 3) {
        move->rad = 0xD00;
    } else {
        move->rad = -0x1300;
    }
}

void AL_ObakeHeadUnknown() {
}