#include <Chao/Chao.h>
#include <c_colli.h>
#include <Chao/al_material.h>

enum {
    MD_RADICASE_SLEEP = 0x0,
    MD_RADICASE_ON = 0x1,
    MD_RADICASE_OFF = 0x2,
    MD_RADICASE_POPS = 0x3,
    MD_RADICASE_ROCK = 0x4,
    MD_RADICASE_CLASSIC = 0x5,
    MD_RADICASE_70S = 0x6,
};

extern NJS_CNK_OBJECT object_alo_radicase_pos_pos[];
extern NJS_CNK_MODEL model_kage_marukage_marukage[];

extern task* pRadicaseTask;

const CCL_INFO ccl_C56752C[1] = { CCL_SPHERE(0, 119, 12, 32768, 0, 0, 0, 2, 0, 0) };

int sub_C53E4E0(void) {
    if (!pRadicaseTask)
        return -1;
    return pRadicaseTask->twp->mode;
}

Bool ALO_GetRadicaseListenPos(NJS_POINT3* pPos) {
    if (!pRadicaseTask)
        return FALSE;
    else {
        taskwk* work = pRadicaseTask->twp;
        float mul = 10 + njRandom() * 3;
        int ang = work->ang.y + 0x8000 + NJM_DEG_ANG(njRandom() * 80 - 40);

        pPos->x = njSin(ang) * mul + work->pos.x;
        pPos->y = work->pos.y;
        pPos->z = njCos(ang) * mul + work->pos.z;
    }

    return TRUE;
}

void ALO_RadicaseExecutor(task* tp) {
    taskwk* work = tp->twp;
    switch (work->mode) {
        case MD_RADICASE_SLEEP:
            break;
        case MD_RADICASE_ON: {
            float randval = njRandom();
            if (randval < 0.25f) {
                work->mode = MD_RADICASE_POPS;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;
                PlayJingle("chao_g_radicase1.adx");
            } else if (randval < 0.5f) {
                work->mode = MD_RADICASE_ROCK;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;
                PlayJingle("chao_g_radicase2.adx");
            } else if (randval < 0.75f) {
                work->mode = MD_RADICASE_CLASSIC;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;
                PlayJingle("chao_g_radicase3.adx");
            } else {
                work->mode = MD_RADICASE_70S;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;
                PlayJingle("chao_g_radicase4.adx");
            }
        } break;
        case MD_RADICASE_OFF:
            InitJingle();
            work->mode = MD_RADICASE_SLEEP;
            work->smode = 0;
            work->wtimer = 0;
            work->id = 0;
            break;
        case MD_RADICASE_POPS:
            if (CCL_IsHitKind2(tp, CI_KIND_AL_CHANGE)) {
                work->mode = MD_RADICASE_ROCK;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;
                PlayJingle("chao_g_radicase2.adx");
            }
            break;
        case MD_RADICASE_ROCK:
            if (CCL_IsHitKind2(tp, CI_KIND_AL_CHANGE)) {
                work->mode = MD_RADICASE_CLASSIC;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;
                PlayJingle("chao_g_radicase3.adx");
            }
            break;
        case MD_RADICASE_CLASSIC:
            if (CCL_IsHitKind2(tp, CI_KIND_AL_CHANGE)) {
                work->mode = MD_RADICASE_70S;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;
                PlayJingle("chao_g_radicase4.adx");
            }
            break;
        case MD_RADICASE_70S:
            if (CCL_IsHitKind2(tp, CI_KIND_AL_CHANGE)) {
                work->mode = MD_RADICASE_POPS;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;
                PlayJingle("chao_g_radicase1.adx");
            }
            break;
    }

    switch (work->mode) {
        case MD_RADICASE_SLEEP:
            if (CCL_IsHitKind2(tp, CI_KIND_AL_TOUCH)) {
                work->mode = MD_RADICASE_ON;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;

                AL_AllFieldCreateT(CI_KIND_AL_RADICASE, &work->pos, 25.5, 3600);
            }
            work->ang.x = AdjustAngle(work->ang.x, 0, 3072);
            break;
        default:
            if (work->wtimer++ > 3600) {
                work->mode = MD_RADICASE_OFF;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;
            }
            work->ang.x += 3072;
            break;
    }
    CCL_Entry(tp);
}

int AL_IsOnScreen2(task* tp, float x, float y);
void ALO_RadicaseDisplayer(task* tp) {
    taskwk* work = tp->twp;
    float val = njSin(work->ang.x) * 0.05f + 1.0f;
    if (AL_IsOnScreen2(tp, 3, 1)) {
        njSetTexture(&AL_TOY_TEXLIST);
        njPushMatrixEx();
        njTranslate(0, 0, -1.5f, 0);
        njTranslateEx(&work->pos);
        njRotateY(0, work->ang.y);
        njScale(0, val, 2 - val, 1);
        njCnkEasyDrawObject(object_alo_radicase_pos_pos);
        if (ALW_ENTRY_WORK(tp)->CamDist < GET_GLOBAL()->CamDistShadowCutLev2) {
            njTranslate(0, 0, 0.5f, 0);
            njScale(0, 2, 1, 1.5f);
            njCnkModDrawModel(model_kage_marukage_marukage);
        }
        njPopMatrixEx();
    }
}
void ALO_RadicaseDestroyer(task* tp) {
    ALW_CancelEntry(tp);
    pRadicaseTask = 0;
}

void CreateALO_Radicase(NJS_VECTOR* pos, int ang) {
    task* tp = CreateElementalTask(2, 4, ALO_RadicaseExecutor, "ALO_RadicaseExecutor");
    taskwk* work;
    NJS_VECTOR stack;

    pRadicaseTask = tp;

    work = tp->twp;
    CCL_Init(tp, ccl_C56752C, 1, 4);
    ALW_Entry(ALW_CATEGORY_TOY, tp, ALW_KIND_RADICASE);
    work->pos = *pos;
    work->ang.y = ang;
    tp->disp = ALO_RadicaseDisplayer;
    tp->dest = ALO_RadicaseDestroyer;
}