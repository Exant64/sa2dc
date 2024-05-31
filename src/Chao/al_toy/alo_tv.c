#include <Chao/Chao.h>
#include <c_colli.h>
#include <Chao/al_material.h>

enum {
    MD_TV_SLEEP = 0,
    MD_TV_ON = 1,
    MD_TV_ON_MAIN = 2,
    MD_TV_OFF = 3,
    MD_TV_OFF_MAIN = 4,
    MD_TV_NOISE = 5,
    MD_TV_NOISE_MAIN = 6,
    MD_TV_SPORTS = 7,
    MD_TV_SPORTS_MAIN = 8,
    MD_TV_ANIME = 9,
    MD_TV_ANIME_MAIN = 10,
    MD_TV_DRAMA = 11,
    MD_TV_DRAMA_MAIN = 12,
};

extern NJS_CNK_OBJECT object_alo_tv_pos_monitor[];
extern NJS_CNK_OBJECT object_alo_tv_pos_pos[];
extern NJS_CNK_MODEL model_kage_marukage_marukage[];

Uint16 TexNumScreenSports[] = { 86, 92, 93, 94, 95 };
Uint16 TexNumScreenAnime[] = { 96, 97, 98 };
Uint16 TexNumScreenDrama[] = { 99, 100, 101 };
Uint16 TexNumScreenOn[] = { 105, 104, 103, 102 };
Uint16 TexNumScreenOff[] = { 102, 103, 104, 105 };
Uint16 TexNumScreenNoise[] = { 106, 107 };
task* pTVTask = 0;

const CCL_INFO ccl_C5675D4[1] = { CCL_SPHERE(0, 119, 12, 32768, 0, 1, 0, 2, 0, 0) };

int sub_C53E9C0(void) {
    if (!pTVTask)
        return -1;
    return pTVTask->twp->mode;
}

Bool ALO_GetTVWatchPos(NJS_POINT3* pPos) {
    if (!pTVTask)
        return FALSE;
    else {
        taskwk* work = pTVTask->twp;
        float mul = 10 + njRandom() * 3;
        int ang = work->ang.y + 0x8000 + NJM_DEG_ANG(njRandom() * 80 - 40);
        pPos->x = njSin(ang) * mul + work->pos.x;
        pPos->y = work->pos.y;
        pPos->z = njCos(ang) * mul + work->pos.z;
    }

    return TRUE;
}

void ALO_TVExecutor(task* tp) {
    taskwk* work = tp->twp;
    switch (work->mode) {
        case MD_TV_SLEEP:
            break;
        case MD_TV_ON:
            work->ang.x = 0;
            AL_MatChangeObjectTexture(object_alo_tv_pos_monitor, TexNumScreenOn[work->ang.x]);
            work->mode++;
            work->smode = 0;
        case MD_TV_ON_MAIN:
            if (work->id++ > 4) {
                work->id = 0;
                work->ang.x++;
                if (work->ang.x > 3) {
                    float randval = njRandom();
                    work->ang.x = 3;
                    if (randval < 0.3f) {
                        work->mode = MD_TV_SPORTS;
                        work->smode = 0;
                        work->wtimer = 0;
                        work->id = 0;

                        PlayJingle("chao_g_tv_sports.adx");
                    } else if (randval < 0.6f) {
                        work->mode = MD_TV_ANIME;
                        work->smode = 0;
                        work->wtimer = 0;
                        work->id = 0;

                        PlayJingle("chao_g_tv_cartoon.adx");
                    } else if (randval < 0.9f) {
                        work->mode = MD_TV_DRAMA;
                        work->smode = 0;
                        work->wtimer = 0;
                        work->id = 0;

                        PlayJingle("chao_g_tv_drama.adx");
                    } else {
                        work->mode = MD_TV_NOISE;
                        work->smode = 0;
                        work->wtimer = 0;
                        work->id = 0;
                    }
                }
                AL_MatChangeObjectTexture(object_alo_tv_pos_monitor, TexNumScreenOn[work->ang.x]);
            }
            break;
        case MD_TV_OFF:
            work->ang.x = 0;
            AL_MatChangeObjectTexture(object_alo_tv_pos_monitor, TexNumScreenOff[work->ang.x]);
            InitJingle();
            work->mode++;
            work->smode = 0;
        case MD_TV_OFF_MAIN:
            if (work->id++ > 4) {
                work->id = 0;

                work->ang.x++;
                if (work->ang.x > 3) {
                    work->ang.x = 3;
                    work->mode = MD_TV_SLEEP;
                    work->smode = 0;
                    work->wtimer = 0;
                    work->id = 0;
                }

                AL_MatChangeObjectTexture(object_alo_tv_pos_monitor, TexNumScreenOff[work->ang.x]);
            }
            break;

        case MD_TV_NOISE:
            work->ang.x = 0;
            AL_MatChangeObjectTexture(object_alo_tv_pos_monitor, TexNumScreenNoise[work->ang.x]);
            work->mode++;
            work->smode = 0;
        case MD_TV_NOISE_MAIN:
            if (work->id++ > 1) {
                work->id = 0;

                work->ang.x++;
                if (work->ang.x > 1) {
                    work->ang.x = 0;
                }

                AL_MatChangeObjectTexture(object_alo_tv_pos_monitor, TexNumScreenNoise[work->ang.x]);
            }

            if (CCL_IsHitKind2(tp, CI_KIND_AL_CHANGE)) {
                work->mode = MD_TV_ANIME;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;
            }
            break;

        case MD_TV_SPORTS:
            work->ang.x = 0;
            AL_MatChangeObjectTexture(object_alo_tv_pos_monitor, TexNumScreenSports[work->ang.x]);
            work->mode++;
            work->smode = 0;
        case MD_TV_SPORTS_MAIN:
            if (work->id++ > 8) {
                work->id = 0;

                work->ang.x++;
                if (work->ang.x > 4) {
                    work->ang.x = 0;
                }

                AL_MatChangeObjectTexture(object_alo_tv_pos_monitor, TexNumScreenSports[work->ang.x]);
            }

            if (CCL_IsHitKind2(tp, CI_KIND_AL_CHANGE)) {
                work->mode = MD_TV_ANIME;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;
            }
            break;

        case MD_TV_ANIME:
            work->ang.x = 0;
            AL_MatChangeObjectTexture(object_alo_tv_pos_monitor, TexNumScreenAnime[work->ang.x]);
            work->mode++;
            work->smode = 0;
        case MD_TV_ANIME_MAIN:
            if (work->id++ > 12) {
                work->id = 0;

                work->ang.x++;
                if (work->ang.x > 2) {
                    work->ang.x = 0;
                }

                AL_MatChangeObjectTexture(object_alo_tv_pos_monitor, TexNumScreenAnime[work->ang.x]);
            }

            if (CCL_IsHitKind2(tp, CI_KIND_AL_CHANGE)) {
                work->mode = MD_TV_DRAMA;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;
            }
            break;

        case MD_TV_DRAMA:
            work->ang.x = 0;
            AL_MatChangeObjectTexture(object_alo_tv_pos_monitor, TexNumScreenDrama[work->ang.x]);
            work->mode++;
            work->smode = 0;
        case MD_TV_DRAMA_MAIN:
            if (work->id++ > 150) {
                work->id = 0;
                work->ang.x = (Uint16)(njRandom() * 2.9f);
                AL_MatChangeObjectTexture(object_alo_tv_pos_monitor, TexNumScreenDrama[work->ang.x]);
            }

            if (CCL_IsHitKind2(tp, CI_KIND_AL_CHANGE)) {
                work->mode = MD_TV_SPORTS;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;
            }
            break;

        default:
            break;
    }
    switch (work->mode) {
        case MD_TV_SLEEP:
            if (!CCL_IsHitKind2(tp, CI_KIND_AL_TOUCH))
                break;
            {
                work->mode = MD_TV_ON;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;
                AL_AllFieldCreateT(CI_KIND_AL_TV, &work->pos, 25.5, 3600);
            }
            break;
        default:
            if (work->wtimer++ > 3600) {
                work->mode = MD_TV_OFF;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;
            }
            break;
    }
    CCL_Entry(tp);
}

int AL_IsOnScreen2(task* tp, float x, float y);
void ALO_TVDisplayer(task* tp) {
    taskwk* work = tp->twp;

    if (AL_IsOnScreen2(tp, 3, 1)) {
        njSetTexture(&AL_TOY_TEXLIST);
        njPushMatrixEx();
        njTranslate(0, 0, -1.5f, 0);
        njTranslateEx(&work->pos);
        njRotateY(0, work->ang.y);
        njCnkEasyDrawObject(object_alo_tv_pos_pos);
        if (ALW_ENTRY_WORK(tp)->CamDist < GET_GLOBAL()->CamDistShadowCutLev2) {
            njTranslate(0, 0, 0.5f, 0);
            njScale(0, 2, 1, 1.5f);
            njCnkModDrawModel(model_kage_marukage_marukage);
        }
        njPopMatrixEx();
    }
}
void ALO_TVDestroyer(task* tp) {
    ALW_CancelEntry(tp);
    pTVTask = 0;
}

void CreateALO_TV(NJS_VECTOR* pos, int ang) {
    task* tp = CreateElementalTask(2, 4, ALO_TVExecutor, "ALO_TVExecutor");
    taskwk* work;
    NJS_VECTOR stack;

    pTVTask = tp;

    work = tp->twp;
    CCL_Init(tp, ccl_C5675D4, 1, 4);
    ALW_Entry(ALW_CATEGORY_TOY, tp, ALW_KIND_TV);
    work->pos = *pos;
    work->ang.y = ang;
    work->ang.x = 104;
    AL_MatChangeObjectTexture(object_alo_tv_pos_monitor, work->ang.x);
    tp->disp = ALO_TVDisplayer;
    tp->dest = ALO_TVDestroyer;
}