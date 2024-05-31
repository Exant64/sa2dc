#include "Chao/Chao.h"
#include "playsound.h"
#include <Chao/al_egg.h>
#include <effect2.h>

void CreateEggSucceed(task* pChaoTask) {
    chaowk* cwk = GET_CHAOWK(pChaoTask);
    AL_GENE* pGene = &cwk->gene;
    CHAO_PARAM* pParam = &cwk->param;

    pParam->nbSucceed++;
    AL_SucceedGeneParam(pGene, pParam);

    if (cwk->pParam)
        AL_ClearChaoParam(cwk->pParam);
    ALW_CancelEntry(pChaoTask);

    pParam->unk_2 = 1;
    CreateChaoEgg(pGene, pParam, &GET_CHAOWK(pChaoTask)->pos, CREATE_EGG_SUCCEED, cwk->pParam);

    cwk->pParam = NULL;
}

int ALBHV_BornPon(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_UMARERU_PON, 15);
            bhv->Mode++;
            break;
    }

    if (AL_IsMotionEnd(tp))
        return BHV_RET_FINISH;

    return BHV_RET_CONTINUE;
}

int ALBHV_BornKyoro(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_UMARERU_KYORO, 15);
            bhv->Mode++;
            break;
    }

    if (AL_IsMotionEnd(tp))
        return BHV_RET_FINISH;

    return BHV_RET_CONTINUE;
}

Uint8 DecideNextType(task* tp) {
    chaowk* cwk = GET_CHAOWK(tp);
    CHAO_PARAM* pParam = &cwk->param;

    Uint8 type;

    float HPos = pParam->body.HPos;
    float VPos = pParam->body.VPos;
    float APos = pParam->body.APos;

    float Normal = GET_GLOBAL()->NormalBoundary;
    float NegNormal = -Normal;
    float Neutral = GET_GLOBAL()->NeutralBoundary;

    if (HPos > NegNormal && HPos < Normal && VPos > NegNormal && VPos < Normal) {
        int i, minicount, mini;

        type = TYPE_N_NORMAL;
        mini = 1;
        minicount = 21;

        for (i = 0; i < minicount; i++) {
            if (!AL_PartsIsMinimalFlagOn(tp, i)) {
                mini = 0;
                break;
            }
        }

        if (mini && pParam->like > 80 && pParam->nbSucceed >= 2)
            type = TYPE_N_CHAOS;

    } else if (VPos > HPos) {
        if (VPos > -HPos)
            type = TYPE_N_FLY;
        else
            type = TYPE_N_RUN;
    } else if (VPos > -HPos)
        type = TYPE_N_POWER;
    else
        type = TYPE_N_SWIM;

    if (-Neutral >= APos || APos >= Neutral) {
        if (APos > 0) {
            type++;
            AL_OnGardenFlag(0x10);
            PlayJingle((const char*)0x0C56725C);
        } else {
            type += 2;
            AL_OnGardenFlag(0x40);
            PlayJingle((const char*)0x0C567270);
        }
    }

    pParam->type = type;

    pParam->body.growth = 0;
    pParam->body.HPos = 0;
    pParam->body.VPos = 0;

    return type;
}

enum {
    MD_MAYU_BODY_INIT = 0x0,
    MD_MAYU_BODY_EXPAND = 0x1,
    MD_MAYU_BODY_FADE_IN = 0x2,
    MD_MAYU_BODY_WAIT = 0x3,
    MD_MAYU_BODY_FADE_OUT = 0x4,
    MD_MAYU_BODY_SHRINK = 0x5,
};

void sub_8C05B2D4(int a1, task* tp, char a2, char a3, short a4, NJS_VECTOR* a5);
void MayuExecutor(task* tp) {
    taskwk* wk = tp->twp;

    if (tp->ptp) {
        wk->pos = tp->ptp->twp->pos;
    }

    switch (wk->mode) {
        case MD_MAYU_BODY_INIT:
            wk->scl.y = GET_GLOBAL()->MayuInitAlpha;
            wk->mode = MD_MAYU_BODY_EXPAND;
            wk->smode = 0;
        case MD_MAYU_BODY_EXPAND:
            wk->scl.x += (1.0f - wk->scl.x) * GET_GLOBAL()->MayuExpandRatio;
            if (wk->scl.x > 0.99) {
                wk->scl.x = 1;
                wk->scl.z = (1.0f - GET_GLOBAL()->MayuInitAlpha) / GET_GLOBAL()->MayuFadeInTime;
                wk->mode = MD_MAYU_BODY_FADE_IN;
                wk->smode = 0;
            }
            break;

        case MD_MAYU_BODY_FADE_IN:
            wk->scl.y += wk->scl.z;
            if (wk->scl.y > 1) {
                wk->scl.y = 1;
                wk->scl.z = -(1.0f - GET_GLOBAL()->MayuInitAlpha) / GET_GLOBAL()->MayuFadeOutTime;
                wk->mode = MD_MAYU_BODY_WAIT;
                wk->smode = 0;
            }
            sub_8C05B2D4(0x101C, tp, 1, -5, 0x3C, &wk->pos);
            break;

        case MD_MAYU_BODY_WAIT:
            break;

        case MD_MAYU_BODY_FADE_OUT:
            wk->scl.y += wk->scl.z;
            if (wk->scl.y < GET_GLOBAL()->MayuInitAlpha) {
                sub_8C05B020(0x101D, 0, 0, -5, &wk->pos);

                wk->scl.y = GET_GLOBAL()->MayuInitAlpha;
                wk->mode = MD_MAYU_BODY_SHRINK;
                wk->smode = 0;
            }
            break;

        case MD_MAYU_BODY_SHRINK:
            wk->scl.x = wk->scl.x * GET_GLOBAL()->MayuShrinkRatio;
            if (wk->scl.x < 0.1f) {
                FreeTask(tp);
            }
            break;
    }
}

extern NJS_CNK_OBJECT unk_C626030[];

void MayuDisplayer(task* tp) {
    taskwk* wk = tp->twp;

    float scl = wk->scl.x * 1.3f;
    float alpha = wk->scl.y;

    SaveControl3D();
    SaveConstantAttr();
    if (alpha < 1) {
        OnControl3D(0x800);
        AddConstantAttr(0, 0x800);
        RemoveConstantAttr(0, 0x7700);
        njColorBlendingMode(0, 8);
        njColorBlendingMode(1, 6);
        SetMaterial(alpha - 1, 0, 0, 0);
    }

    njSetTexture(&AL_OBJECT_TEXLIST);
    njPushMatrixEx();
    njTranslateEx(&wk->pos);
    njRotateY(NULL, wk->ang.y);
    njScale(NULL, scl, scl, scl);
    njCnkEasyDrawObject(unk_C626030);
    njPopMatrixEx();

    SetMaterial(0, 0, 0, 0);
    LoadConstantAttr();
    LoadControl3D();
}

void MayuDestructor(task* tp) {
    if (tp->ptp) {
        GET_CHAOWK(tp->ptp)->pMayu = NULL;
    }
}

// no mayukind parameter/enum since DC cocoons are all the same color
void CreateMayu(task* tp) {
    chaowk* cwk = GET_CHAOWK(tp);
    if (!cwk->pMayu) {
        task* pMayu = CreateChildTask(2, MayuExecutor, tp);
        pMayu->disp = MayuDisplayer;
        pMayu->dest = MayuDestructor;

        cwk->pMayu = pMayu;
    }
}

void MayuFadeOut(task* tp) {
    chaowk* cwk = GET_CHAOWK(tp);
    taskwk* mayuwk;

    if (cwk->pMayu) {
        mayuwk = cwk->pMayu->twp;
    }

    mayuwk->mode = MD_MAYU_BODY_FADE_OUT;
}

enum {
    MD_MAYU_INIT = 0x0,
    MD_MAYU_SIT_WAIT = 0x1,
    MD_MAYU_WAIT_FADE_IN = 0x2,
    MD_MAYU_CHANGE_TYPE = 0x3,
    MD_MAYU_WAIT_FADE_OUT = 0x4,
    MD_MAYU_WAIT_RELAX = 0x5,
};
int ALBHV_Mayu(task* tp) {
    chaowk* cwk = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &cwk->Behavior;

    switch (bhv->Mode) {
        case MD_MAYU_INIT:
            AL_FixPosition(tp);
            AL_DisableTouch(tp);
            switch (AL_GetMotionPosture(tp)) {
                case 0:
                    AL_SetMotionLinkStep(tp, ALM_GENKI_SUWARI, 15);
                    break;
                case 2:
                    AL_SetMotionLinkStep(tp, ALM_OKIRU, 15);
                    break;
                case 3:
                    AL_SetMotionLinkStep(tp, ALM_OKIRU, 35);
                    break;
                case 1:
                default:
                    AL_SetMotionLinkStep(tp, ALM_HIMA_SIT, 15);
                    break;
            }
            bhv->Mode = MD_MAYU_SIT_WAIT;
            bhv->Timer = 0;
            bhv->SubTimer = 0;

        case MD_MAYU_SIT_WAIT:
            if (bhv->Timer++ > GET_GLOBAL()->MayuWaitTime) {
                bhv->Mode = MD_MAYU_WAIT_FADE_IN;
                bhv->Timer = 0;
                bhv->SubTimer = 0;
                CreateMayu(tp);
            }
            break;

        case MD_MAYU_WAIT_FADE_IN:
            if (bhv->Timer++ > GET_GLOBAL()->MayuFadeInTime) {
                AL_ShapeChangeType(tp, DecideNextType(tp));
                bhv->Mode = MD_MAYU_CHANGE_TYPE;
                bhv->Timer = 0;
                bhv->SubTimer = 0;
            }
            break;

        case MD_MAYU_CHANGE_TYPE:
            if (bhv->Timer++ > GET_GLOBAL()->MayuTypeChageTime) {
                bhv->Mode = MD_MAYU_WAIT_FADE_OUT;
                bhv->Timer = 0;
                bhv->SubTimer = 0;

                MayuFadeOut(tp);

                switch (GET_CHAOPARAM(tp)->type) {
                    case TYPE_N_CHAOS:
                    case TYPE_H_CHAOS:
                    case TYPE_D_CHAOS:
                        PlayJingle((const char*)0x0C567284);
                        break;
                    default:
                        if (AL_IsHero2(tp))
                            PlayJingle((const char*)0x00C567298);
                        else if (AL_IsDark2(tp))
                            PlayJingle((const char*)0x0C5672AC);
                        break;
                }
            }
            break;

        case MD_MAYU_WAIT_FADE_OUT:
            if (bhv->Timer++ > GET_GLOBAL()->MayuFadeOutTime) {
                bhv->Mode = MD_MAYU_WAIT_RELAX;
                bhv->Timer = 0;
                bhv->SubTimer = 0;
            }
            break;

        case MD_MAYU_WAIT_RELAX:
            if (bhv->Timer++ > GET_GLOBAL()->MayuRelaxTime) {
                AL_ReleasePosition(tp);
                AL_EnableTouch(tp);
                return BHV_RET_FINISH;
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

Bool AL_CanISucceed(task* tp) {
    chaowk* cwk = GET_CHAOWK(tp);
    CHAO_PARAM* pParam = &cwk->param;

    if (pParam->like > GET_GLOBAL()->SucceedBoundaryUserLike) {
        if (AL_EmotionGetValue(tp, EM_ST_STRESS) < GET_GLOBAL()->SucceedBoundaryStress) {
            return TRUE;
        }
    }

    return FALSE;
}

enum {
    MD_DEAD_INIT = 0x0,
    MD_DEAD_SIT_WAIT = 0x1,
    MD_DEAD_WAIT_FADE_IN = 0x2,
    MD_DEAD_WAIT = 0x3,
    MD_DEAD_WAIT_FADE_OUT = 0x4,
};
int ALBHV_Dead(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch (bhv->Mode) {
        case MD_DEAD_INIT:
            AL_FixPosition(tp);
            AL_DisableTouch(tp);
            AL_FaceSetEye(tp, AL_EYE_NUM_TOHOHO, -1);
            AL_FaceSetMouth(tp, AL_MOUTH_NUM_MUSU, -1);
            AL_IconSet(tp, MD_ICON_MOJYA, 300);

            switch (AL_GetMotionPosture(tp)) {
                case 0:
                    AL_SetMotionLinkStep(tp, ALM_GENKI_SUWARI, 15);
                    break;
                case 1:
                    AL_SetMotionLinkStep(tp, ALM_HIMA_SIT, 15);
                    break;
                case 3:
                    AL_SetMotionLinkStep(tp, ALM_HIMA_SIT, 45);
                    break;
                case 2:
                    AL_SetMotionLinkStep(tp, ALM_OKIRU, 15);
                    break;
            }
            bhv->Mode = MD_DEAD_SIT_WAIT;
            bhv->Timer = 0;
            bhv->SubTimer = 0;

        case MD_DEAD_SIT_WAIT:
            if (bhv->Timer++ > GET_GLOBAL()->MayuWaitTime) {
                bhv->Mode = MD_DEAD_WAIT_FADE_IN;
                bhv->Timer = 0;
                bhv->SubTimer = 0;
                CreateMayu(tp);
            }
            break;

        case MD_DEAD_WAIT_FADE_IN:
            if (bhv->Timer++ > GET_GLOBAL()->MayuFadeInTime) {
                if (AL_CanISucceed(tp)) {
                    CreateEggSucceed(tp);
                } else {
                    PlayJingle((const char*)0x0C5672C0);
                }

                GET_CHAOWK(tp)->ChaoFlag &= ~CHAO_FLAG_DRAW_ENABLE;
                bhv->Mode = MD_DEAD_WAIT;
                bhv->Timer = 0;
                bhv->SubTimer = 0;
            }
            break;

        case MD_DEAD_WAIT:
            if (bhv->Timer++ > GET_GLOBAL()->MayuTypeChageTime) {
                bhv->Mode = MD_DEAD_WAIT_FADE_OUT;
                bhv->Timer = 0;
                bhv->SubTimer = 0;

                MayuFadeOut(tp);
            }
            break;

        case MD_DEAD_WAIT_FADE_OUT:
            if (bhv->Timer++ > GET_GLOBAL()->MayuFadeOutTime + 30) {
                KillChao(tp);
            }
            break;
    }

    return BHV_RET_CONTINUE;
}