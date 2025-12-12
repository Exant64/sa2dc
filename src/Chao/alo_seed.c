#include <move.h>
#include <Chao/Chao.h>
#include <player.h>

// the function names, and the enum is made up since we don't have sa2 symbols

enum { MD_INIT, MD_STOP, MD_MOVE, MD_HELD_P, MD_HELD_C };

void ALO_SeedStop(task* tp) {
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
    }
}

void ALO_SeedMove(task* tp) {
    taskwk* work = tp->twp;
    MOVE_WORK* move = (MOVE_WORK*)tp->Data2;

    switch (work->smode) {
        case 0:
            CCL_Enable(tp, 0);
            CCL_Enable(tp, 1);
            CCL_Disable(tp, 2);
            work->smode++;
            work->wtimer = 0;
            break;
    }

    MOV_Control(tp);
    MOV_DetectCollision(tp);

    if (work->flag & 1) {
        // this fixed a regswap in alo_obake_head, and having it here doesn't mess with the match
        // so im assuming this was here too, it just didn't happen to affect compilation
        MOVE_WORK* move2 = (MOVE_WORK*)tp->Data2;
        NJS_VECTOR velo;
        velo.x = move->Velo.x;
        velo.y = 0;
        velo.z = move->Velo.z;
        if (njScalor(&velo) < 0.01) {
            if (work->wtimer++ > 30) {
                MOV_ClearVelo(tp);
                MOV_ClearAcc(tp);

                work->mode = MD_STOP;
                work->smode = 0;

            } else {
                work->wtimer = 0;
            }
        }
    }

    if (work->flag & TWK_FLAG_HELD) {
        work->mode = MD_HELD_P;
        work->smode = 0;
        return;
    }
}

void ALO_SeedInit(task* tp) {
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
    MOV_DetectCollision(tp);

    if (work->wtimer++ > 30) {
        work->mode = 2;
        work->smode = 0;
    }
}

void ALO_SeedHeldP(task* tp) {
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
        }
    }
}

void ALO_SeedHeldC(task* tp) {
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

    if (work->flag & TWK_FLAG_HELD) {
        ALW_CommunicationOff(tp);
        work->mode = MD_HELD_P;
        work->smode = 0;
    }

    if (!AL_TraceHoldingPosition(tp)) {
        work->mode = MD_MOVE;
        work->smode = 0;
    }
}

void ALO_SeedExecutor(task* tp) {
    taskwk* work = tp->twp;

    switch (work->mode) {
        case MD_STOP:
            ALO_SeedStop(tp);
            break;
        case MD_INIT:
            ALO_SeedInit(tp);
            break;
        case MD_MOVE:
            ALO_SeedMove(tp);
            break;
        case MD_HELD_P:
            ALO_SeedHeldP(tp);
            break;
        case MD_HELD_C:
            ALO_SeedHeldC(tp);
            break;
    }

    CCL_Entry(tp);
}

extern NJS_CNK_OBJECT* SeedObjectList[];
extern NJS_CNK_MODEL model_kage_marukage_marukage[];
void ALO_SeedDisplayer(task* tp) {
    taskwk* work = tp->twp;

    if (AL_IsOnScreen2(tp, 1.5f, 1.0f)) {
        njSetTexture(&AL_OBJECT_TEXLIST);
        njPushMatrix(NULL);

        if (work->flag & TWK_FLAG_HELD) {
            njTranslate(NULL, 0, -0.25f, 0);
        }

        njTranslateEx(&work->pos);
        njRotateY(NULL, work->ang.y);
        njScale(NULL, 1.5f, 1.5f, 1.5f);
        OnControl3D(0x2400);
        njCnkEasyDrawModel(SeedObjectList[work->ang.x]->model);
        OffControl3D(0x2400);

        if (ALW_ENTRY_WORK(tp)->CamDist < GET_GLOBAL()->CamDistShadowCutLev1) {
            njScale(NULL, 0.38f, 0.7f, 0.38f);
            njTranslate(NULL, 0, 0.1f, 0);
            njCnkModDrawModel(model_kage_marukage_marukage);
        }

        njPopMatrix(1);
    }
}

void ALO_SeedDestructor(task* tp) {
    ALW_CancelEntry(tp);
}

static const CCL_INFO colli_info[3] = {
    { 0, CI_FORM_CYLINDER, CI_PUSH_TH_CMN, 0, CI_ATTR_2000 | CI_ATTR_DAMAGE, { 0, 1.5, 0 }, 15, 3 },
    { 0, CI_FORM_SPHERE, CI_PUSH_PO_ENEMY | CI_PUSH_PO_OBJECT, CI_DMG_SET(0, 3), 0, { 0.0, 1.2, 0.0 }, 0.8f },
    { CI_KIND_AL_SEED, CI_FORM_CYLINDER, CI_PUSH_TH_CMN, CI_DMG_SET(0, 3), 0, { 0, -4, 0 }, 4.2, 8 }
};
void ALO_Seed(task* tp) {
    taskwk* work = tp->twp;

    CCL_Init(tp, colli_info, 3, 5);
    ObjectMovableInitialize(work, tp->Data2, 10);

    tp->exec = ALO_SeedExecutor;
    tp->dest = ALO_SeedDestructor;
    tp->disp = ALO_SeedDisplayer;

    work->mode = MD_INIT;
    work->smode = 0;

    ALW_Entry(ALW_CATEGORY_SEED, tp, 0);
    ALW_SetHeldOffset(tp, 0.2f);
    ALW_SetHeldRadius(tp, 0.35f);
}

task* ALO_SeedCreate(int kind, NJS_POINT3* pPos, int AngY, NJS_VECTOR* pVelo) {
    task* tp = CreateElementalTask(2, 2, ALO_Seed, "ALO_Seed");
    taskwk* work = tp->twp;
    MOVE_WORK* move = MOV_Init(tp);

    work->pos = *pPos;

    if (pVelo) {
        move->Velo = *pVelo;
    }

    work->ang.x = kind;

    MOV_SetGravity(tp, -0.05f);
    move->Offset.y = 3;
    move->BoundFloor = 0.7f;
    move->BoundFriction = 0.7f;
    move->rad = 0x1800;
    return tp;
}

void nullsub() {
}